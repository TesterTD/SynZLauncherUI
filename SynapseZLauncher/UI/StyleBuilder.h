#pragma once

#include <string>

class StyleBuilder final {
public:
    StyleBuilder() = delete;

    static std::wstring Build();

private:
    static std::wstring GlobalStyles();
    static std::wstring TitlebarStyles();
    static std::wstring SettingsStyles();
    static std::wstring EditorStyles();
    static std::wstring TabStyles();
    static std::wstring ScriptsPanelStyles();
    static std::wstring ToolbarStyles();
    static std::wstring EmptyStateStyles();
};