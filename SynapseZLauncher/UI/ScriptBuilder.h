#pragma once

#include <string>

class ScriptBuilder final {
public:
    ScriptBuilder() = delete;

    static std::wstring Build();

private:
    static std::wstring CoreFunctions();
    static std::wstring EventBindings();
    static std::wstring MonacoSetup();
    static std::wstring MonacoTheme();
    static std::wstring LuaLanguage();
    static std::wstring LuaCompletions();
    static std::wstring EditorFactory();
    static std::wstring TabManagement();
    static std::wstring SettingsSetup();
    static std::wstring ResizeHandling();
    static std::wstring StatusFunctions();
    static std::wstring ScriptListFunctions();
};