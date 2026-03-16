#pragma once

#include <windows.h>
#include <wrl.h>
#include <wrl/client.h>
#include <wrl/event.h>
#include <string>
#include <functional>
#include "../Dependencies/WebView2.h"

using Microsoft::WRL::ComPtr;
using Microsoft::WRL::Callback;

class WebViewHost final {
public:
    using MessageCallback = std::function<void(const std::wstring&)>;

    WebViewHost();
    ~WebViewHost() = default;

    WebViewHost(const WebViewHost&) = delete;
    WebViewHost& operator=(const WebViewHost&) = delete;

    void SetMessageCallback(MessageCallback callback);
    bool Initialize(HWND parentWindow);
    void NavigateToHtml(const std::wstring& html);
    void ExecuteScript(const std::wstring& script);
    void Focus();
    void ResizeToFit();

private:
    void ConfigureSettings();
    void RegisterMessageHandler();

    HWND                            parentHwnd_;
    ComPtr<ICoreWebView2Controller> controller_;
    ComPtr<ICoreWebView2>           webview_;
    MessageCallback                 messageCallback_;
    std::wstring                    pendingHtml_;
};