#include "SynapseZAPI.h"
#include <fstream>
#include <random>
#include <shlobj.h>
#include <tlhelp32.h>
#include <psapi.h>

std::string SynapseZAPI::latestError_;

std::string SynapseZAPI::GetLatestErrorMessage() {
    return latestError_;
}

std::string SynapseZAPI::GetBasePath() {
    char buffer[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, buffer))) {
        return std::string(buffer) + "\\Synapse Z";
    }
    return "";
}

std::string SynapseZAPI::GenerateRandomFileName(int length) {
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; ++i) {
        result += charset[distribution(generator)];
    }

    return result;
}

int SynapseZAPI::Execute(const std::string& script, DWORD processId) {
    std::string basePath      = GetBasePath();
    std::string binPath       = basePath + "\\bin";
    std::string schedulerPath = binPath + "\\scheduler";

    if (GetFileAttributesA(binPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        latestError_ = "Bin folder not found";
        return 1;
    }

    if (GetFileAttributesA(schedulerPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        latestError_ = "Scheduler folder not found";
        return 2;
    }

    std::string fileName = GenerateRandomFileName(10) + ".lua";
    std::string filePath;

    if (processId == 0) {
        filePath = schedulerPath + "\\" + fileName;
    } else {
        filePath = schedulerPath + "\\PID" + std::to_string(processId) + "_" + fileName;
    }

    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        latestError_ = "No access to write file";
        return 3;
    }

    file << script << "@@FileFullyWritten@@";
    file.close();
    return 0;
}

std::vector<DWORD> SynapseZAPI::GetRobloxProcesses() {
    std::vector<DWORD> processes;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return processes;
    }

    PROCESSENTRY32W entry = {};
    entry.dwSize = sizeof(entry);

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
                processes.push_back(entry.th32ProcessID);
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return processes;
}

bool SynapseZAPI::IsSynapseZInstance(DWORD processId) {
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (!process) {
        return false;
    }

    wchar_t imagePath[MAX_PATH] = {};
    DWORD pathSize = MAX_PATH;
    bool pathObtained = false;

    if (QueryFullProcessImageNameW(process, 0, imagePath, &pathSize)) {
        pathObtained = true;
    }

    CloseHandle(process);

    if (!pathObtained) {
        return false;
    }

    HANDLE file = CreateFileW(
        imagePath, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, OPEN_EXISTING, 0, nullptr
    );

    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }

    char header[0x1000] = {};
    DWORD bytesRead = 0;
    ReadFile(file, header, sizeof(header), &bytesRead, nullptr);
    CloseHandle(file);

    std::string content(header, bytesRead);
    return content.find(".grh") != std::string::npos;
}

std::vector<DWORD> SynapseZAPI::GetSynzRobloxInstances() {
    std::vector<DWORD> allProcesses = GetRobloxProcesses();
    std::vector<DWORD> synapseInstances;

    for (DWORD pid : allProcesses) {
        if (IsSynapseZInstance(pid)) {
            synapseInstances.push_back(pid);
        }
    }

    return synapseInstances;
}

bool SynapseZAPI::AreAllInstancesSynz() {
    auto all = GetRobloxProcesses();
    if (all.empty()) {
        return false;
    }
    return GetSynzRobloxInstances().size() == all.size();
}