#pragma once

#include "SynapseSession.h"
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>

class SessionManager final {
public:
    using SessionEventHandler  = std::function<void(SynapseSession*)>;
    using ConsoleEventHandler  = std::function<void(SynapseSession*, int, const std::string&)>;

    SessionManager() = delete;

    static void StartInstancesTimer();
    static void StopInstancesTimer();
    static void Execute(const std::string& source, DWORD processId = 0);
    static std::map<DWORD, SynapseSession*>& GetInstances();
    static void RemoveSession(DWORD processId);

    static SessionEventHandler  OnSessionAdded;
    static SessionEventHandler  OnSessionRemoved;
    static ConsoleEventHandler  OnSessionOutput;

private:
    static void MonitorLoop();
    static void HandleSessionMessage(SynapseSession* session,
        const std::string& command, const std::string& data, int index);

    static std::map<DWORD, SynapseSession*> sessions_;
    static std::mutex                        sessionsMutex_;
    static std::thread                       monitorThread_;
    static std::atomic<bool>                 monitoring_;
};