#pragma once

#include <string>

class LayoutBuilder final {
public:
    LayoutBuilder() = delete;

    static std::wstring Build();

private:
    static std::wstring Titlebar();
    static std::wstring EditorView();
    static std::wstring SettingsView();
    static std::wstring Toolbar();
    static std::wstring ScriptsPanel();
    static std::wstring TabBar();
    static std::wstring EditorContainer();
};