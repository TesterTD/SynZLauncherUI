#include "WebViewHost.h"

WebViewHost::WebViewHost()
    : parentHwnd_(nullptr) {
}

void WebViewHost::SetMessageCallback(MessageCallback callback) {
    messageCallback_ = std::move(callback);
}

bool WebViewHost::Initialize(HWND parentWindow) {
    parentHwnd_ = parentWindow;

    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* environment) -> HRESULT {
                if (FAILED(result)) {
                    return result;
                }

                return environment->CreateCoreWebView2Controller(
                    parentHwnd_,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(result)) {
                                return result;
                            }

                            controller_ = controller;
                            controller_->get_CoreWebView2(&webview_);

                            ResizeToFit();
                            ConfigureSettings();
                            RegisterMessageHandler();

                            if (!pendingHtml_.empty()) {
                                webview_->NavigateToString(pendingHtml_.c_str());
                                pendingHtml_.clear();
                            }

                            Focus();
                            return S_OK;
                        }
                    ).Get()
                );
            }
        ).Get()
    );

    return SUCCEEDED(hr);
}

void WebViewHost::NavigateToHtml(const std::wstring& html) {
    if (!webview_) {
        pendingHtml_ = html;
        return;
    }
    webview_->NavigateToString(html.c_str());
}

void WebViewHost::ExecuteScript(const std::wstring& script) {
    if (webview_) {
        webview_->ExecuteScript(script.c_str(), nullptr);
    }
}

void WebViewHost::Focus() {
    if (controller_) {
        controller_->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
    }
}

void WebViewHost::ResizeToFit() {
    if (!controller_) {
        return;
    }

    RECT bounds;
    GetClientRect(parentHwnd_, &bounds);
    controller_->put_Bounds(bounds);
}

void WebViewHost::ConfigureSettings() {
    ComPtr<ICoreWebView2Settings> settings;
    webview_->get_Settings(&settings);

    settings->put_AreDefaultContextMenusEnabled(FALSE);
    settings->put_AreDevToolsEnabled(FALSE);
    settings->put_IsStatusBarEnabled(FALSE);
    settings->put_IsZoomControlEnabled(FALSE);
    settings->put_IsWebMessageEnabled(TRUE);
}

void WebViewHost::RegisterMessageHandler() {
    webview_->add_WebMessageReceived(
        Callback<ICoreWebView2WebMessageReceivedEventHandler>(
            [this](ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                LPWSTR rawMessage = nullptr;
                args->TryGetWebMessageAsString(&rawMessage);

                if (rawMessage && messageCallback_) {
                    messageCallback_(std::wstring(rawMessage));
                    CoTaskMemFree(rawMessage);
                }

                return S_OK;
            }
        ).Get(),
        nullptr
    );
}