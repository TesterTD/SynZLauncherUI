#pragma once

#include <string>

class StringUtils final {
public:
    StringUtils() = delete;

    static std::wstring EscapeForJs(const std::string& input);
    static std::wstring EscapeForJsW(const std::wstring& input);
    static std::string  WideToUtf8(const std::wstring& input);
    static std::wstring Utf8ToWide(const std::string& input);
};