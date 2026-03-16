#include "LayoutBuilder.h"

std::wstring LayoutBuilder::Build() {
    std::wstring html;

    html += Titlebar();
    html += EditorView();
    html += SettingsView();

    return html;
}

std::wstring LayoutBuilder::Titlebar() {
    std::wstring s;

    s += L"<div class=\"titlebar\">";
    s += L"<div class=\"drag-region\" id=\"drag-region\"></div>";
    s += L"<div class=\"tb-logo\">";
    s += L"<img src=\"https://raw.githubusercontent.com/nicehash/NiceHashQuickMiner/main/images/favicon.ico\" onerror=\"this.style.display='none'\" alt=\"\">";
    s += L"<span class=\"tb-name\">SynZLauncher</span>";
    s += L"</div>";
    s += L"<div class=\"tb-right\">";
    s += L"<button class=\"settings-btn\" id=\"btn-settings\" title=\"Settings\">";
    s += L"<svg viewBox=\"0 0 24 24\"><line x1=\"4\" y1=\"6\" x2=\"20\" y2=\"6\"/>";
    s += L"<line x1=\"4\" y1=\"12\" x2=\"20\" y2=\"12\"/><line x1=\"4\" y1=\"18\" x2=\"20\" y2=\"18\"/></svg>";
    s += L"</button>";
    s += L"<button class=\"wc\" id=\"btn-min\"><svg viewBox=\"0 0 10 10\"><line x1=\"1\" y1=\"5\" x2=\"9\" y2=\"5\"/></svg></button>";
    s += L"<button class=\"wc\" id=\"btn-max\"><svg viewBox=\"0 0 10 10\"><rect x=\"1\" y=\"1\" width=\"8\" height=\"8\"/></svg></button>";
    s += L"<button class=\"wc wc-close\" id=\"btn-close\"><svg viewBox=\"0 0 10 10\">";
    s += L"<line x1=\"1\" y1=\"1\" x2=\"9\" y2=\"9\"/><line x1=\"9\" y1=\"1\" x2=\"1\" y2=\"9\"/></svg></button>";
    s += L"</div></div>";

    return s;
}

std::wstring LayoutBuilder::EditorView() {
    std::wstring s;

    s += L"<div class=\"view active\" id=\"view-editor\">";
    s += L"<div class=\"main-content\">";
    s += L"<div class=\"editor-area\">";
    s += TabBar();
    s += EditorContainer();
    s += L"</div>";
    s += ScriptsPanel();
    s += L"</div>";
    s += Toolbar();
    s += L"</div>";

    return s;
}

std::wstring LayoutBuilder::TabBar() {
    std::wstring s;

    s += L"<div class=\"tabbar\" id=\"tabbar\">";
    s += L"<button class=\"tab-add\" id=\"btn-new-tab\">";
    s += L"<svg viewBox=\"0 0 12 12\"><line x1=\"6\" y1=\"1\" x2=\"6\" y2=\"11\"/>";
    s += L"<line x1=\"1\" y1=\"6\" x2=\"11\" y2=\"6\"/></svg></button>";
    s += L"</div>";

    return s;
}

std::wstring LayoutBuilder::EditorContainer() {
    std::wstring s;

    s += L"<div class=\"editors\" id=\"editors\">";
    s += L"<div class=\"empty-state\" id=\"empty-state\">";
    s += L"<div class=\"empty-icon\"><svg viewBox=\"0 0 24 24\">";
    s += L"<path d=\"M13 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V9z\"/>";
    s += L"<polyline points=\"13 2 13 9 20 9\"/></svg></div>";
    s += L"<div class=\"empty-text\">No tabs open :(<br>";
    s += L"<span class=\"empty-link\" id=\"empty-open\">Create one?</span></div>";
    s += L"</div></div>";

    return s;
}

std::wstring LayoutBuilder::ScriptsPanel() {
    std::wstring s;

    s += L"<div class=\"scripts-panel\" id=\"scripts-panel\">";
    s += L"<div class=\"scripts-header\">";
    s += L"<span class=\"scripts-title\">Scripts</span>";
    s += L"<button class=\"scripts-refresh\" id=\"btn-refresh-scripts\" title=\"Refresh\">";
    s += L"<svg viewBox=\"0 0 24 24\"><path d=\"M1 4v6h6\"/><path d=\"M23 20v-6h-6\"/>";
    s += L"<path d=\"M20.49 9A9 9 0 0 0 5.64 5.64L1 10m22 4l-4.64 4.36A9 9 0 0 1 3.51 15\"/></svg>";
    s += L"</button></div>";
    s += L"<div class=\"scripts-list\" id=\"scripts-list\">";
    s += L"<div class=\"scripts-empty\" id=\"scripts-empty\">No scripts found.<br>";
    s += L"Place .lua files in<br>the Scripts folder.</div>";
    s += L"</div></div>";

    return s;
}

std::wstring LayoutBuilder::Toolbar() {
    std::wstring s;

    s += L"<div class=\"toolbar\">";
    s += L"<div class=\"tb-l\">";

    s += L"<button class=\"btn\" id=\"btn-run\">";
    s += L"<svg viewBox=\"0 0 24 24\"><polygon points=\"5 3 19 12 5 21 5 3\"/></svg>Execute</button>";
    s += L"<div class=\"sep\"></div>";

    s += L"<button class=\"btn\" id=\"btn-clear\">";
    s += L"<svg viewBox=\"0 0 24 24\"><polyline points=\"3 6 5 6 21 6\"/>";
    s += L"<path d=\"M19 6l-1 14H6L5 6\"/><path d=\"M10 11v6m4-6v6\"/>";
    s += L"<path d=\"M9 6V4h6v2\"/></svg>Clear</button>";

    s += L"<button class=\"btn\" id=\"btn-open\">";
    s += L"<svg viewBox=\"0 0 24 24\"><path d=\"M3 19V7l4-4h5l2 2h7v14H3z\"/>";
    s += L"<path d=\"M7 3v4H3\"/></svg>Open</button>";

    s += L"<button class=\"btn\" id=\"btn-save\">";
    s += L"<svg viewBox=\"0 0 24 24\"><path d=\"M19 21H5V3h11l3 3v15z\"/>";
    s += L"<rect x=\"8\" y=\"13\" width=\"8\" height=\"8\"/>";
    s += L"<rect x=\"9\" y=\"3\" width=\"6\" height=\"5\"/></svg>Save</button>";

    s += L"</div></div>";

    return s;
}

std::wstring LayoutBuilder::SettingsView() {
    std::wstring s;

    s += L"<div class=\"view\" id=\"view-settings\">";
    s += L"<div class=\"settings-view\">";
    s += L"<div class=\"settings-inner\">";

    s += L"<div class=\"settings-header\">";
    s += L"<span class=\"settings-title\">Settings</span>";
    s += L"<button class=\"settings-back\" id=\"btn-back\">";
    s += L"<svg viewBox=\"0 0 10 10\"><polyline points=\"6 1 2 5 6 9\"/></svg>Back</button>";
    s += L"</div>";

    s += L"<div class=\"sp-group\">";
    s += L"<div class=\"sp-group-title\">Typography</div>";
    s += L"<div class=\"sp-row\"><span class=\"sp-label\">Font Family</span>";
    s += L"<select class=\"sp-select\" id=\"sp-font\" style=\"min-width:140px;\">";
    s += L"<option value=\"'Cascadia Mono','Consolas',monospace\" selected>Cascadia Mono</option>";
    s += L"<option value=\"'Consolas',monospace\">Consolas</option>";
    s += L"<option value=\"'Fira Code',monospace\">Fira Code</option>";
    s += L"<option value=\"'JetBrains Mono',monospace\">JetBrains Mono</option>";
    s += L"<option value=\"'Courier New',monospace\">Courier New</option>";
    s += L"</select></div>";
    s += L"<div class=\"sp-row\"><span class=\"sp-label\">Font Size</span>";
    s += L"<div style=\"display:flex;align-items:center;gap:8px;\">";
    s += L"<input type=\"range\" class=\"sp-range\" id=\"sp-fontsize\" min=\"10\" max=\"24\" value=\"13\">";
    s += L"<span class=\"sp-val\" id=\"sp-fontsize-val\">13</span></div></div>";
    s += L"</div>";

    s += L"<div class=\"sp-sep\"></div>";

    s += L"<div class=\"sp-group\">";
    s += L"<div class=\"sp-group-title\">Editor</div>";
    s += L"<div class=\"sp-row\"><span class=\"sp-label\">Tab Size</span>";
    s += L"<select class=\"sp-select\" id=\"sp-tabsize\">";
    s += L"<option value=\"2\">2 spaces</option><option value=\"4\" selected>4 spaces</option>";
    s += L"<option value=\"8\">8 spaces</option></select></div>";
    s += L"<div class=\"sp-row\"><span class=\"sp-label\">Word Wrap</span>";
    s += L"<select class=\"sp-select\" id=\"sp-wordwrap\">";
    s += L"<option value=\"off\" selected>Off</option><option value=\"on\">On</option></select></div>";
    s += L"<div class=\"sp-row\"><span class=\"sp-label\">Line Numbers</span>";
    s += L"<select class=\"sp-select\" id=\"sp-linenums\">";
    s += L"<option value=\"on\" selected>On</option><option value=\"off\">Off</option></select></div>";
    s += L"</div>";

    s += L"<div class=\"sp-sep\"></div>";

    s += L"<div class=\"sp-group\">";
    s += L"<div class=\"sp-group-title\">Appearance</div>";
    s += L"<div class=\"sp-row\"><span class=\"sp-label\">Minimap</span>";
    s += L"<input type=\"checkbox\" class=\"sp-check\" id=\"sp-minimap\"></div>";
    s += L"<div class=\"sp-row\"><span class=\"sp-label\">Smooth Cursor</span>";
    s += L"<input type=\"checkbox\" class=\"sp-check\" id=\"sp-smooth\" checked></div>";
    s += L"</div>";

    s += L"</div></div></div>";

    return s;
}