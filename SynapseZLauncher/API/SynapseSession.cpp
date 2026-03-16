#include "SynapseSession.h"
#include "SessionManager.h"
#include <vector>

static constexpr DWORD PIPE_ACCESS = GENERIC_READ | GENERIC_WRITE;
static constexpr DWORD PIPE_SHARE  = FILE_SHARE_READ | FILE_SHARE_WRITE;

SynapseSession::SynapseSession()
    : processId_(0)
    , running_(false) {
}

SynapseSession::~SynapseSession() {
    running_ = false;
    if (sessionThread_.joinable()) {
        sessionThread_.detach();
    }
}

void SynapseSession::QueueCommand(const std::string& command) {
    std::lock_guard<std::mutex> lock(mutex_);
    commandQueue_.push_back(command);
}

void SynapseSession::Execute(const std::string& source) {
    QueueCommand("execute " + source);
}

void SynapseSession::AddMessageCallback(MessageCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    messageCallbacks_.push_back(std::move(callback));
}

bool SynapseSession::ConnectToPipe(const std::string& pipeName, HANDLE& outHandle) {
    if (!WaitNamedPipeA(pipeName.c_str(), 10)) {
        return false;
    }

    outHandle = CreateFileA(
        pipeName.c_str(), PIPE_ACCESS, PIPE_SHARE,
        nullptr, OPEN_EXISTING, 0, nullptr
    );

    if (outHandle == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD mode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE;
    SetNamedPipeHandleState(outHandle, &mode, nullptr, nullptr);

    return true;
}

bool SynapseSession::SendData(HANDLE pipe, const std::string& data) {
    DWORD written = 0;
    return WriteFile(pipe, data.c_str(), static_cast<DWORD>(data.size()), &written, nullptr) != FALSE;
}

std::string SynapseSession::ReadData(HANDLE pipe) {
    DWORD dummy = 0;
    ReadFile(pipe, nullptr, 0, &dummy, nullptr);

    DWORD available = 0;
    if (!PeekNamedPipe(pipe, nullptr, 0, nullptr, &available, nullptr) || available == 0) {
        return "";
    }

    std::vector<char> buffer(available);
    DWORD bytesRead = 0;
    ReadFile(pipe, buffer.data(), available, &bytesRead, nullptr);

    return std::string(buffer.data(), bytesRead);
}

bool SynapseSession::Initialize(DWORD processId) {
    processId_ = processId;
    std::string initialPipe = "\\\\.\\pipe\\synz-" + std::to_string(processId);

    HANDLE handle;
    if (!ConnectToPipe(initialPipe, handle)) {
        return false;
    }

    SendData(handle, "new");
    std::string response = ReadData(handle);

    if (response.empty()) {
        CloseHandle(handle);
        return false;
    }

    pipeName_ = response;
    CloseHandle(handle);

    running_ = true;
    sessionThread_ = std::thread(&SynapseSession::RunSessionLoop, this);
    sessionThread_.detach();

    return true;
}

void SynapseSession::RunSessionLoop() {
    while (running_) {
        if (!WaitNamedPipeA(pipeName_.c_str(), NMPWAIT_WAIT_FOREVER)) {
            continue;
        }

        HANDLE pipe = CreateFileA(
            pipeName_.c_str(), PIPE_ACCESS, PIPE_SHARE,
            nullptr, OPEN_EXISTING, 0, nullptr
        );

        if (pipe == INVALID_HANDLE_VALUE) {
            continue;
        }

        DWORD mode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE;
        SetNamedPipeHandleState(pipe, &mode, nullptr, nullptr);

        while (running_) {
            std::vector<std::string> currentCommands;

            {
                std::lock_guard<std::mutex> lock(mutex_);
                currentCommands = commandQueue_;
                commandQueue_.clear();
            }

            currentCommands.push_back("read");

            std::string commandCount = std::to_string(currentCommands.size());
            DWORD written = 0;

            if (!WriteFile(pipe, commandCount.c_str(),
                static_cast<DWORD>(commandCount.size()), &written, nullptr)) {
                CloseHandle(pipe);
                goto session_end;
            }

            for (const auto& command : currentCommands) {
                SendData(pipe, command);

                std::string responseStr = ReadData(pipe);
                if (responseStr.empty()) {
                    continue;
                }

                unsigned long long responseCount = 0;
                try {
                    responseCount = std::stoull(responseStr);
                } catch (...) {
                    continue;
                }

                for (unsigned long long i = 0; i < responseCount; ++i) {
                    std::string data = ReadData(pipe);

                    std::lock_guard<std::mutex> lock(mutex_);
                    for (auto& callback : messageCallbacks_) {
                        callback(command, data, static_cast<int>(i));
                    }
                }
            }

            Sleep(5);
        }

        CloseHandle(pipe);
    }

session_end:
    SessionManager::RemoveSession(processId_);
}