#pragma once

#include <windows.h>
#include <string>
#include <vector>

class SynapseZAPI final {
public:
    SynapseZAPI() = delete;

    static std::string GetLatestErrorMessage();
    static int Execute(const std::string& script, DWORD processId = 0);
    static std::vector<DWORD> GetRobloxProcesses();
    static bool IsSynapseZInstance(DWORD processId);
    static std::vector<DWORD> GetSynzRobloxInstances();
    static bool AreAllInstancesSynz();

private:
    static std::string GenerateRandomFileName(int length);
    static std::string GetBasePath();

    static std::string latestError_;
};