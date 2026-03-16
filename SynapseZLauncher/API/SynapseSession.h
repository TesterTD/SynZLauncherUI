#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <functional>
#include <atomic>

class SynapseSession final {
public:
    using MessageCallback = std::function<void(const std::string&, const std::string&, int)>;

    SynapseSession();
    ~SynapseSession();

    SynapseSession(const SynapseSession&) = delete;
    SynapseSession& operator=(const SynapseSession&) = delete;

    bool Initialize(DWORD processId);
    void Execute(const std::string& source);
    void QueueCommand(const std::string& command);
    void AddMessageCallback(MessageCallback callback);

    DWORD GetProcessId() const { return processId_; }
    const std::string& GetPipeName() const { return pipeName_; }

private:
    void RunSessionLoop();
    bool ConnectToPipe(const std::string& pipeName, HANDLE& outHandle);
    bool SendData(HANDLE pipe, const std::string& data);
    std::string ReadData(HANDLE pipe);

    DWORD                       processId_;
    std::string                 pipeName_;
    std::vector<std::string>    commandQueue_;
    std::vector<MessageCallback> messageCallbacks_;
    std::mutex                  mutex_;
    std::thread                 sessionThread_;
    std::atomic<bool>           running_;
};