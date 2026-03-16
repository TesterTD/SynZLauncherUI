#include "SessionManager.h"
#include "SynapseZAPI.h"

std::map<DWORD, SynapseSession*> SessionManager::sessions_;
std::mutex                        SessionManager::sessionsMutex_;
std::thread                       SessionManager::monitorThread_;
std::atomic<bool>                 SessionManager::monitoring_(false);

SessionManager::SessionEventHandler  SessionManager::OnSessionAdded;
SessionManager::SessionEventHandler  SessionManager::OnSessionRemoved;
SessionManager::ConsoleEventHandler  SessionManager::OnSessionOutput;

void SessionManager::StartInstancesTimer() {
    if (monitoring_) {
        return;
    }

    monitoring_ = true;
    monitorThread_ = std::thread(&SessionManager::MonitorLoop);
    monitorThread_.detach();
}

void SessionManager::StopInstancesTimer() {
    monitoring_ = false;
}

void SessionManager::MonitorLoop() {
    while (monitoring_) {
        Sleep(2000);
        if (!monitoring_) break;

        auto robloxProcesses = SynapseZAPI::GetRobloxProcesses();

        for (DWORD pid : robloxProcesses) {
            {
                std::lock_guard<std::mutex> lock(sessionsMutex_);
                if (sessions_.count(pid)) {
                    continue;
                }
            }

            if (!SynapseZAPI::IsSynapseZInstance(pid)) {
                continue;
            }

            auto* session = new SynapseSession();

            {
                std::lock_guard<std::mutex> lock(sessionsMutex_);
                sessions_[pid] = session;
            }

            session->AddMessageCallback(
                [session](const std::string& command, const std::string& data, int index) {
                    HandleSessionMessage(session, command, data, index);
                }
            );

            if (session->Initialize(pid)) {
                if (OnSessionAdded) {
                    OnSessionAdded(session);
                }
            } else {
                std::lock_guard<std::mutex> lock(sessionsMutex_);
                sessions_.erase(pid);
                delete session;
            }
        }
    }
}

void SessionManager::HandleSessionMessage(SynapseSession* session,
    const std::string& command, const std::string& data, int) {

    if (command != "read") {
        return;
    }

    size_t spacePos = data.find(' ');
    if (spacePos == std::string::npos) {
        return;
    }

    std::string subCommand = data.substr(0, spacePos);
    std::string payload    = data.substr(spacePos + 1);

    if (subCommand == "output") {
        size_t typeEnd = payload.find(' ');
        if (typeEnd == std::string::npos) {
            return;
        }

        int outputType = 0;
        try {
            outputType = std::stoi(payload.substr(0, typeEnd));
        } catch (...) {}

        std::string outputText = payload.substr(typeEnd + 1);

        if (OnSessionOutput) {
            OnSessionOutput(session, outputType, outputText);
        }
    } else if (subCommand == "error") {
        if (OnSessionOutput) {
            OnSessionOutput(session, 3, payload);
        }
    }
}

void SessionManager::Execute(const std::string& source, DWORD processId) {
    std::lock_guard<std::mutex> lock(sessionsMutex_);

    if (processId == 0) {
        for (auto& [pid, session] : sessions_) {
            session->Execute(source);
        }
    } else {
        auto it = sessions_.find(processId);
        if (it != sessions_.end()) {
            it->second->Execute(source);
        }
    }
}

std::map<DWORD, SynapseSession*>& SessionManager::GetInstances() {
    return sessions_;
}

void SessionManager::RemoveSession(DWORD processId) {
    SynapseSession* session = nullptr;

    {
        std::lock_guard<std::mutex> lock(sessionsMutex_);
        auto it = sessions_.find(processId);
        if (it != sessions_.end()) {
            session = it->second;
            sessions_.erase(it);
        }
    }

    if (session && OnSessionRemoved) {
        OnSessionRemoved(session);
    }
}