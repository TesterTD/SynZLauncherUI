#pragma once

#include <windows.h>
#include <string>
#include "WebViewHost.h"

class Application final {
public:
    explicit Application(HINSTANCE instanceHandle);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    int Run();

private:
    bool RegisterWindowClass();
    bool CreateMainWindow();
    void ToggleMaximize();
    void OnMessage(const std::wstring& message);
    void InitializeSynapseAPI();
    void HandleExecute(const std::wstring& script);
    void HandleOpen();
    void HandleSave(const std::wstring& data);
    void HandleLoadScript(const std::wstring& fileName);

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    HWND                hwnd_;
    HINSTANCE           instanceHandle_;
    WebViewHost         webview_;
    bool                maximized_;

    static Application* instance_;

    static constexpr const wchar_t* WINDOW_CLASS_NAME = L"SynZLauncherClass";
    static constexpr const wchar_t* WINDOW_TITLE      = L"SynZLauncher";
    static constexpr int            DEFAULT_WIDTH      = 860;
    static constexpr int            DEFAULT_HEIGHT     = 540;
    static constexpr int            MIN_WIDTH          = 500;
    static constexpr int            MIN_HEIGHT         = 320;
    static constexpr int            BORDER_SIZE        = 5;
    static constexpr int            TITLEBAR_HEIGHT    = 34;
};