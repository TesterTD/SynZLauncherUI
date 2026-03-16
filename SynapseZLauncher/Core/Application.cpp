#include "Application.h"
#include "../UI/HtmlBuilder.h"
#include "../Utils/FileDialog.h"
#include "../Utils/StringUtils.h"
#include "../Utils/ScriptManager.h"
#include "../API/SynapseZAPI.h"
#include "../API/SessionManager.h"
#include <windowsx.h>
#include <dwmapi.h>

Application* Application::instance_ = nullptr;

Application::Application(HINSTANCE instanceHandle)
    : hwnd_(nullptr)
    , instanceHandle_(instanceHandle)
    , maximized_(false) {
    instance_ = this;
}

Application::~Application() {
    SessionManager::StopInstancesTimer();
    instance_ = nullptr;
}

int Application::Run() {
    if (!RegisterWindowClass()) {
        return -1;
    }

    if (!CreateMainWindow()) {
        return -1;
    }

    webview_.SetMessageCallback(
        [this](const std::wstring& message) {
            OnMessage(message);
        }
    );

    webview_.Initialize(hwnd_);
    webview_.NavigateToHtml(HtmlBuilder::Build());

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);

    InitializeSynapseAPI();

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}

bool Application::RegisterWindowClass() {
    WNDCLASSEXW windowClass       = {};
    windowClass.cbSize            = sizeof(windowClass);
    windowClass.style             = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc       = WindowProc;
    windowClass.hInstance          = instanceHandle_;
    windowClass.hCursor           = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground     = CreateSolidBrush(RGB(20, 20, 20));
    windowClass.lpszClassName     = WINDOW_CLASS_NAME;
    windowClass.hIcon             = static_cast<HICON>(
        LoadImageW(instanceHandle_, L"IDI_APP", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE)
    );

    if (!windowClass.hIcon) {
        windowClass.hIcon = static_cast<HICON>(
            LoadImageW(nullptr, L"Images\\SynapseZ.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE)
        );
    }

    if (!windowClass.hIcon) {
        windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    }

    return RegisterClassExW(&windowClass) != 0;
}

bool Application::CreateMainWindow() {
    int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX         = (screenWidth - DEFAULT_WIDTH) / 2;
    int posY         = (screenHeight - DEFAULT_HEIGHT) / 2;

    hwnd_ = CreateWindowExW(
        WS_EX_APPWINDOW,
        WINDOW_CLASS_NAME,
        WINDOW_TITLE,
        WS_POPUP | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        posX, posY,
        DEFAULT_WIDTH, DEFAULT_HEIGHT,
        nullptr, nullptr, instanceHandle_, nullptr
    );

    if (!hwnd_) {
        return false;
    }

    MARGINS margins = { 1, 1, 1, 1 };
    DwmExtendFrameIntoClientArea(hwnd_, &margins);

    BOOL useDarkMode = TRUE;
    DwmSetWindowAttribute(hwnd_, 20, &useDarkMode, sizeof(useDarkMode));

    return true;
}

void Application::ToggleMaximize() {
    if (maximized_) {
        ShowWindow(hwnd_, SW_RESTORE);
        maximized_ = false;
    } else {
        ShowWindow(hwnd_, SW_MAXIMIZE);
        maximized_ = true;
    }
}

void Application::InitializeSynapseAPI() {
    SessionManager::OnSessionOutput = [this](SynapseSession*, int type, const std::string& output) {
        std::wstring escaped = StringUtils::EscapeForJs(output);
        std::wstring script  = L"if(typeof addConsoleOutput==='function')addConsoleOutput('"
                             + escaped + L"'," + std::to_wstring(type) + L");";
        webview_.ExecuteScript(script);
    };

    SessionManager::StartInstancesTimer();
}

void Application::HandleExecute(const std::wstring& scriptContent) {
    std::string script = StringUtils::WideToUtf8(scriptContent);

    auto& instances = SessionManager::GetInstances();
    if (!instances.empty()) {
        SessionManager::Execute(script);
    } else {
        int result = SynapseZAPI::Execute(script);
        if (result != 0) {
            std::string errorMsg = SynapseZAPI::GetLatestErrorMessage();
            std::wstring escaped = StringUtils::EscapeForJs(errorMsg);
            webview_.ExecuteScript(
                L"if(typeof addConsoleOutput==='function')addConsoleOutput('"
                + escaped + L"',3);"
            );
        }
    }
    webview_.Focus();
}

void Application::HandleOpen() {
    std::wstring path = FileDialog::OpenFile(hwnd_);

    if (!path.empty()) {
        std::string content  = FileDialog::ReadFileContent(path);
        std::wstring escaped = StringUtils::EscapeForJs(content);
        webview_.ExecuteScript(
            L"(function(){var ed=getActive();"
            L"if(ed){ed.setValue('" + escaped + L"');ed.focus();}})();"
        );
    }
    webview_.Focus();
}

void Application::HandleSave(const std::wstring& data) {
    std::wstring path = FileDialog::SaveFile(hwnd_);

    if (!path.empty()) {
        std::string content = StringUtils::WideToUtf8(data);
        FileDialog::WriteFileContent(path, content);
    }
    webview_.Focus();
}

void Application::HandleLoadScript(const std::wstring& fileName) {
    std::wstring fullPath = ScriptManager::GetScriptsFolder() + L"\\" + fileName;
    std::string content   = FileDialog::ReadFileContent(fullPath);
    std::wstring escaped  = StringUtils::EscapeForJs(content);
    std::wstring nameEsc  = StringUtils::EscapeForJsW(fileName);

    webview_.ExecuteScript(
        L"(function(){newTab('" + escaped + L"','" + nameEsc + L"');})();"
    );
    webview_.Focus();
}

void Application::OnMessage(const std::wstring& message) {
    if (message == L"wc-close") {
        DestroyWindow(hwnd_);
    }
    else if (message == L"wc-min") {
        ShowWindow(hwnd_, SW_MINIMIZE);
    }
    else if (message == L"wc-max") {
        ToggleMaximize();
    }
    else if (message.rfind(L"drag-start:", 0) == 0) {
        if (maximized_) return;
        int screenX = 0, screenY = 0;
        swscanf_s(message.c_str(), L"drag-start:%d:%d", &screenX, &screenY);
        ReleaseCapture();
        SendMessageW(hwnd_, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(screenX, screenY));
    }
    else if (message.rfind(L"exec:", 0) == 0) {
        HandleExecute(message.substr(5));
    }
    else if (message == L"open") {
        HandleOpen();
    }
    else if (message.rfind(L"savedata:", 0) == 0) {
        HandleSave(message.substr(9));
    }
    else if (message == L"refresh-scripts") {
        ScriptManager::RefreshScripts(webview_);
    }
    else if (message.rfind(L"load-script:", 0) == 0) {
        HandleLoadScript(message.substr(12));
    }
}

LRESULT CALLBACK Application::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SIZE:
        if (instance_) {
            instance_->webview_.ResizeToFit();
        }
        if (wParam == SIZE_MAXIMIZED && instance_) {
            instance_->maximized_ = true;
        }
        if (wParam == SIZE_RESTORED && instance_) {
            instance_->maximized_ = false;
        }
        return 0;

    case WM_ACTIVATE:
        if (instance_ && LOWORD(wParam) != WA_INACTIVE) {
            instance_->webview_.Focus();
        }
        return 0;

    case WM_NCCALCSIZE:
        if (wParam) return 0;
        break;

    case WM_NCHITTEST: {
        POINT cursorPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ScreenToClient(hwnd, &cursorPos);

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        if (instance_ && !instance_->maximized_) {
            const int border = BORDER_SIZE;

            if (cursorPos.y < border) {
                if (cursorPos.x < border) return HTTOPLEFT;
                if (cursorPos.x > clientRect.right - border) return HTTOPRIGHT;
                return HTTOP;
            }
            if (cursorPos.y > clientRect.bottom - border) {
                if (cursorPos.x < border) return HTBOTTOMLEFT;
                if (cursorPos.x > clientRect.right - border) return HTBOTTOMRIGHT;
                return HTBOTTOM;
            }
            if (cursorPos.x < border) return HTLEFT;
            if (cursorPos.x > clientRect.right - border) return HTRIGHT;
        }
        return HTCLIENT;
    }

    case WM_SETFOCUS:
        if (instance_) {
            instance_->webview_.Focus();
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_GETMINMAXINFO: {
        auto minMaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
        minMaxInfo->ptMinTrackSize = { MIN_WIDTH, MIN_HEIGHT };

        if (instance_ && instance_->maximized_) {
            HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
            MONITORINFO monitorInfo = {};
            monitorInfo.cbSize = sizeof(monitorInfo);

            if (GetMonitorInfoW(monitor, &monitorInfo)) {
                minMaxInfo->ptMaxPosition.x = monitorInfo.rcWork.left;
                minMaxInfo->ptMaxPosition.y = monitorInfo.rcWork.top;
                minMaxInfo->ptMaxSize.x     = monitorInfo.rcWork.right - monitorInfo.rcWork.left;
                minMaxInfo->ptMaxSize.y     = monitorInfo.rcWork.bottom - monitorInfo.rcWork.top;
            }
        }
        return 0;
    }
    }

    return DefWindowProcW(hwnd, message, wParam, lParam);
}