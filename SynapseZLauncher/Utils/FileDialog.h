#pragma once

#include <windows.h>
#include <string>

class FileDialog final {
public:
    FileDialog() = delete;

    static std::wstring OpenFile(HWND owner);
    static std::wstring SaveFile(HWND owner);
    static std::string  ReadFileContent(const std::wstring& path);
    static void         WriteFileContent(const std::wstring& path, const std::string& data);
};