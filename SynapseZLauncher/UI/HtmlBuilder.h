#pragma once

#include <string>

class HtmlBuilder final {
public:
    HtmlBuilder() = delete;

    static std::wstring Build();
};