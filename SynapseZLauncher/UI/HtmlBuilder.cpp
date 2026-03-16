#include "HtmlBuilder.h"
#include "StyleBuilder.h"
#include "LayoutBuilder.h"
#include "ScriptBuilder.h"

std::wstring HtmlBuilder::Build() {
    std::wstring html;

    html += L"<!DOCTYPE html><html><head><meta charset=\"utf-8\">";
    html += L"<style>";
    html += StyleBuilder::Build();
    html += L"</style></head><body>";
    html += LayoutBuilder::Build();
    html += L"<script>";
    html += ScriptBuilder::Build();
    html += L"</script></body></html>";

    return html;
}