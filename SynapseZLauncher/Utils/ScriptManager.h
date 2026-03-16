#pragma once

#include <windows.h>
#include <string>
#include <vector>

class WebViewHost;

class ScriptManager final {
public:
    ScriptManager() = delete;

    static std::wstring              GetScriptsFolder();
    static std::vector<std::wstring> ListScripts();
    static void                      RefreshScripts(WebViewHost& webview);

private:
    static void FindFilesWithExtension(
        const std::wstring& folder,
        const std::wstring& extension,
        std::vector<std::wstring>& results
    );
};