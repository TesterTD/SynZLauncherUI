#include "ScriptManager.h"
#include "../Core/WebViewHost.h"
#include "StringUtils.h"

std::wstring ScriptManager::GetScriptsFolder() {
    wchar_t exePath[MAX_PATH] = {};
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);

    std::wstring directory(exePath);
    size_t lastSlash = directory.find_last_of(L"\\/");

    if (lastSlash != std::wstring::npos) {
        directory = directory.substr(0, lastSlash);
    }

    return directory + L"\\Scripts";
}

void ScriptManager::FindFilesWithExtension(
    const std::wstring& folder,
    const std::wstring& extension,
    std::vector<std::wstring>& results) {

    std::wstring searchPattern = folder + L"\\*" + extension;

    WIN32_FIND_DATAW findData;
    HANDLE findHandle = FindFirstFileW(searchPattern.c_str(), &findData);

    if (findHandle != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                results.push_back(findData.cFileName);
            }
        } while (FindNextFileW(findHandle, &findData));

        FindClose(findHandle);
    }
}

std::vector<std::wstring> ScriptManager::ListScripts() {
    std::vector<std::wstring> scripts;
    std::wstring folder = GetScriptsFolder();

    FindFilesWithExtension(folder, L".lua",  scripts);
    FindFilesWithExtension(folder, L".luau", scripts);
    FindFilesWithExtension(folder, L".txt",  scripts);

    return scripts;
}

void ScriptManager::RefreshScripts(WebViewHost& webview) {
    std::wstring folder = GetScriptsFolder();
    CreateDirectoryW(folder.c_str(), nullptr);

    auto scripts = ListScripts();

    std::wstring jsCall = L"populateScripts([";
    for (size_t i = 0; i < scripts.size(); ++i) {
        if (i > 0) {
            jsCall += L",";
        }
        jsCall += L"'" + StringUtils::EscapeForJsW(scripts[i]) + L"'";
    }
    jsCall += L"]);";

    webview.ExecuteScript(jsCall);
}