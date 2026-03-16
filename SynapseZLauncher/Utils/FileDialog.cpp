#include "FileDialog.h"
#include <commdlg.h>
#include <fstream>
#include <sstream>

std::wstring FileDialog::OpenFile(HWND owner) {
    wchar_t filePath[MAX_PATH] = {};

    OPENFILENAMEW dialog   = {};
    dialog.lStructSize     = sizeof(dialog);
    dialog.hwndOwner       = owner;
    dialog.lpstrFilter     = L"Lua Files\0*.lua;*.luau;*.txt\0All Files\0*.*\0";
    dialog.lpstrFile       = filePath;
    dialog.nMaxFile        = MAX_PATH;
    dialog.Flags           = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    return GetOpenFileNameW(&dialog) ? filePath : L"";
}

std::wstring FileDialog::SaveFile(HWND owner) {
    wchar_t filePath[MAX_PATH] = {};

    OPENFILENAMEW dialog   = {};
    dialog.lStructSize     = sizeof(dialog);
    dialog.hwndOwner       = owner;
    dialog.lpstrFilter     = L"Lua Files\0*.lua\0Text Files\0*.txt\0All Files\0*.*\0";
    dialog.lpstrFile       = filePath;
    dialog.nMaxFile        = MAX_PATH;
    dialog.Flags           = OFN_OVERWRITEPROMPT;
    dialog.lpstrDefExt     = L"lua";

    return GetSaveFileNameW(&dialog) ? filePath : L"";
}

std::string FileDialog::ReadFileContent(const std::wstring& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return "";
    }

    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

void FileDialog::WriteFileContent(const std::wstring& path, const std::string& data) {
    std::ofstream file(path, std::ios::binary);
    if (file) {
        file << data;
    }
}