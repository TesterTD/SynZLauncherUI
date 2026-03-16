#include "StringUtils.h"
#include <windows.h>

std::wstring StringUtils::EscapeForJs(const std::string& input) {
    std::wstring result;
    result.reserve(input.size() * 2);

    for (unsigned char ch : input) {
        switch (ch) {
        case '\\': result += L"\\\\"; break;
        case '\'': result += L"\\'";  break;
        case '\n': result += L"\\n";  break;
        case '\r': result += L"\\r";  break;
        case '\t': result += L"\\t";  break;
        default:   result += static_cast<wchar_t>(ch); break;
        }
    }

    return result;
}

std::wstring StringUtils::EscapeForJsW(const std::wstring& input) {
    std::wstring result;
    result.reserve(input.size() * 2);

    for (wchar_t ch : input) {
        switch (ch) {
        case L'\\': result += L"\\\\"; break;
        case L'\'': result += L"\\'";  break;
        case L'\n': result += L"\\n";  break;
        case L'\r': result += L"\\r";  break;
        case L'\t': result += L"\\t";  break;
        default:    result += ch; break;
        }
    }

    return result;
}

std::string StringUtils::WideToUtf8(const std::wstring& input) {
    if (input.empty()) {
        return "";
    }

    int size = WideCharToMultiByte(
        CP_UTF8, 0,
        input.c_str(), static_cast<int>(input.size()),
        nullptr, 0, nullptr, nullptr
    );

    std::string result(size, 0);
    WideCharToMultiByte(
        CP_UTF8, 0,
        input.c_str(), static_cast<int>(input.size()),
        result.data(), size, nullptr, nullptr
    );

    return result;
}

std::wstring StringUtils::Utf8ToWide(const std::string& input) {
    if (input.empty()) {
        return L"";
    }

    int size = MultiByteToWideChar(
        CP_UTF8, 0,
        input.c_str(), static_cast<int>(input.size()),
        nullptr, 0
    );

    std::wstring result(size, 0);
    MultiByteToWideChar(
        CP_UTF8, 0,
        input.c_str(), static_cast<int>(input.size()),
        result.data(), size
    );

    return result;
}