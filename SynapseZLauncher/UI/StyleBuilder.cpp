#include "StyleBuilder.h"

std::wstring StyleBuilder::Build() {
    std::wstring styles;

    styles += GlobalStyles();
    styles += TitlebarStyles();
    styles += SettingsStyles();
    styles += EditorStyles();
    styles += TabStyles();
    styles += ScriptsPanelStyles();
    styles += ToolbarStyles();
    styles += EmptyStateStyles();

    return styles;
}

std::wstring StyleBuilder::GlobalStyles() {
    std::wstring s;

    s += L"*{margin:0;padding:0;box-sizing:border-box;border-radius:0;}";
    s += L":root{--bg:#181818;--bg1:#1e1e1e;--bg2:#141414;--bg3:#202020;";
    s += L"--border:#252525;--border2:#2d2d2d;";
    s += L"--fg:#999;--fg2:#686868;--fg3:#424242;--fg4:#c0c0c0;";
    s += L"--acc:#333;--acc2:#2a2a2a;}";
    s += L"html,body{width:100%;height:100%;overflow:hidden;background:var(--bg);color:var(--fg);";
    s += L"font-family:'Segoe UI',system-ui,sans-serif;font-size:12px;}";
    s += L"body{display:flex;flex-direction:column;}";
    s += L".view{display:none;flex-direction:column;flex:1;overflow:hidden;}";
    s += L".view.active{display:flex;}";
    s += L".main-content{display:flex;flex:1;overflow:hidden;}";
    s += L".editor-area{display:flex;flex-direction:column;flex:1;overflow:hidden;min-width:0;}";

    s += L"@keyframes spin-smooth{0%{transform:rotate(0deg);}100%{transform:rotate(360deg);}}";

    return s;
}

std::wstring StyleBuilder::TitlebarStyles() {
    std::wstring s;

    s += L".titlebar{height:34px;background:var(--bg2);border-bottom:1px solid var(--border);";
    s += L"display:flex;align-items:center;flex-shrink:0;position:relative;}";
    s += L".tb-logo{display:flex;align-items:center;gap:8px;padding:0 14px;flex-shrink:0;position:relative;z-index:1;}";
    s += L".tb-logo img{width:16px;height:16px;}";
    s += L".tb-name{font-size:11px;font-weight:400;color:#b0b0b0;letter-spacing:2px;text-transform:uppercase;}";
    s += L".drag-region{position:absolute;left:0;top:0;right:0;bottom:0;cursor:default;z-index:0;}";
    s += L".tb-right{margin-left:auto;display:flex;align-items:stretch;height:34px;flex-shrink:0;position:relative;z-index:1;}";
    s += L".wc{width:46px;height:34px;display:flex;align-items:center;justify-content:center;";
    s += L"background:none;border:none;cursor:pointer;color:var(--fg3);transition:background 0.12s,color 0.12s;flex-shrink:0;}";
    s += L".wc svg{width:10px;height:10px;stroke:currentColor;fill:none;stroke-width:1.5;stroke-linecap:round;stroke-linejoin:round;}";
    s += L".wc:hover{background:rgba(255,255,255,0.05);color:var(--fg);}";
    s += L".wc-close:hover{background:#c0392b!important;color:#fff!important;}";
    s += L".settings-btn{width:40px;height:34px;display:flex;align-items:center;justify-content:center;";
    s += L"background:none;border:none;cursor:pointer;color:var(--fg3);transition:background 0.12s,color 0.12s;}";
    s += L".settings-btn svg{width:14px;height:14px;stroke:currentColor;fill:none;stroke-width:1.8;stroke-linecap:round;}";
    s += L".settings-btn:hover{background:rgba(255,255,255,0.05);color:var(--fg);}";

    return s;
}

std::wstring StyleBuilder::SettingsStyles() {
    std::wstring s;

    s += L".settings-view{background:var(--bg);padding:0;overflow-y:auto;}";
    s += L".settings-inner{max-width:520px;width:100%;margin:0 auto;padding:24px 20px 30px;}";
    s += L".settings-header{display:flex;align-items:center;justify-content:space-between;margin-bottom:20px;}";
    s += L".settings-title{font-size:13px;color:#b0b0b0;letter-spacing:2px;text-transform:uppercase;font-weight:400;}";
    s += L".settings-back{height:24px;padding:0 12px;background:transparent;border:1px solid var(--border);";
    s += L"color:#a0a0a0;font-size:11px;cursor:pointer;font-family:inherit;letter-spacing:0.3px;text-transform:uppercase;";
    s += L"display:flex;align-items:center;gap:5px;transition:all 0.1s;}";
    s += L".settings-back:hover{background:var(--acc2);color:#d0d0d0;border-color:var(--border2);}";
    s += L".settings-back svg{width:10px;height:10px;stroke:currentColor;fill:none;stroke-width:2;stroke-linecap:round;stroke-linejoin:round;}";
    s += L".sp-group{display:flex;flex-direction:column;gap:10px;margin-bottom:16px;}";
    s += L".sp-group-title{font-size:9px;color:var(--fg3);letter-spacing:1.5px;text-transform:uppercase;margin-bottom:2px;}";
    s += L".sp-row{display:flex;align-items:center;justify-content:space-between;gap:10px;min-height:26px;}";
    s += L".sp-label{font-size:11.5px;color:#888;}";
    s += L".sp-select{background:var(--bg2);border:1px solid var(--border);color:var(--fg);font-size:11px;";
    s += L"padding:5px 10px;font-family:inherit;outline:none;cursor:pointer;min-width:100px;transition:border-color 0.12s;}";
    s += L".sp-select:hover{border-color:var(--border2);}";
    s += L".sp-select:focus{border-color:#444;}";
    s += L".sp-range{width:100px;accent-color:#555;cursor:pointer;}";
    s += L".sp-check{width:15px;height:15px;accent-color:#555;cursor:pointer;}";
    s += L".sp-val{font-size:11px;color:var(--fg2);min-width:22px;text-align:center;font-variant-numeric:tabular-nums;}";
    s += L".sp-sep{height:1px;background:var(--border);margin:8px 0;}";

    return s;
}

std::wstring StyleBuilder::EditorStyles() {
    std::wstring s;

    s += L".editors{flex:1;position:relative;overflow:hidden;}";
    s += L".ep{position:absolute;left:0;top:0;width:100%;height:100%;visibility:hidden;}";
    s += L".ep.active{visibility:visible;}";

    return s;
}

std::wstring StyleBuilder::TabStyles() {
    std::wstring s;

    s += L".tabbar{height:30px;background:var(--bg2);border-bottom:1px solid var(--border);";
    s += L"display:flex;align-items:flex-end;padding:0 6px;flex-shrink:0;overflow-x:auto;overflow-y:hidden;}";
    s += L".tabbar::-webkit-scrollbar{height:0;}";
    s += L".tab{height:26px;padding:0 10px 0 14px;display:flex;align-items:center;gap:8px;";
    s += L"background:transparent;border:1px solid transparent;border-bottom:none;font-size:11px;color:var(--fg3);";
    s += L"cursor:default;white-space:nowrap;font-family:inherit;letter-spacing:0.2px;position:relative;bottom:-1px;";
    s += L"transition:color 0.1s,background 0.1s;user-select:none;}";
    s += L".tab:hover{color:var(--fg2);}";
    s += L".tab.active{background:var(--bg1);border-color:var(--border);border-bottom-color:var(--bg1);color:var(--fg);}";
    s += L".tab.drag-over{border-left:2px solid #555!important;}";
    s += L".tab-name{pointer-events:none;}";
    s += L".tab-rename{background:#232323;border:1px solid #444;color:var(--fg);font-size:11px;font-family:inherit;";
    s += L"padding:1px 4px;outline:none;width:80px;letter-spacing:0.2px;}";
    s += L".tab-x{width:14px;height:14px;display:flex;align-items:center;justify-content:center;opacity:0;";
    s += L"transition:opacity 0.1s;background:none;border:none;cursor:pointer;color:var(--fg3);padding:0;}";
    s += L".tab-x svg{width:8px;height:8px;stroke:currentColor;fill:none;stroke-width:2;stroke-linecap:round;}";
    s += L".tab:hover .tab-x,.tab.active .tab-x{opacity:0.45;}";
    s += L".tab-x:hover{opacity:1!important;background:rgba(255,255,255,0.07);}";
    s += L".tab-add{width:28px;height:26px;display:flex;align-items:center;justify-content:center;";
    s += L"background:none;border:none;cursor:pointer;color:var(--fg3);flex-shrink:0;position:relative;bottom:-1px;padding:0;}";
    s += L".tab-add svg{width:12px;height:12px;stroke:currentColor;fill:none;stroke-width:1.8;stroke-linecap:round;}";
    s += L".tab-add:hover{color:var(--fg2);}";

    return s;
}

std::wstring StyleBuilder::ScriptsPanelStyles() {
    std::wstring s;

    s += L".scripts-panel{width:180px;background:var(--bg2);border-left:1px solid var(--border);";
    s += L"display:flex;flex-direction:column;flex-shrink:0;overflow:hidden;}";
    s += L".scripts-header{height:30px;display:flex;align-items:center;justify-content:space-between;padding:0 12px;";
    s += L"border-bottom:1px solid var(--border);flex-shrink:0;}";
    s += L".scripts-title{font-size:9px;color:#888;letter-spacing:1.5px;text-transform:uppercase;font-weight:400;}";
    s += L".scripts-refresh{width:20px;height:20px;display:flex;align-items:center;justify-content:center;";
    s += L"background:none;border:none;cursor:pointer;color:var(--fg3);transition:color 0.12s;padding:0;}";
    s += L".scripts-refresh svg{width:12px;height:12px;stroke:currentColor;fill:none;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round;}";
    s += L".scripts-refresh:hover{color:var(--fg);}";
    s += L".scripts-refresh.spinning svg{animation:spin-smooth 0.5s ease-in-out;}";
    s += L".scripts-list{flex:1;overflow-y:auto;overflow-x:hidden;padding:4px 0;}";
    s += L".scripts-list::-webkit-scrollbar{width:4px;}";
    s += L".scripts-list::-webkit-scrollbar-track{background:transparent;}";
    s += L".scripts-list::-webkit-scrollbar-thumb{background:#282828;}";
    s += L".scripts-list::-webkit-scrollbar-thumb:hover{background:#333;}";
    s += L".script-item{height:26px;display:flex;align-items:center;gap:7px;padding:0 12px;";
    s += L"cursor:pointer;color:#808080;font-size:11px;transition:all 0.1s;user-select:none;white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}";
    s += L".script-item:hover{background:rgba(255,255,255,0.03);color:#aaa;}";
    s += L".script-item:active{background:rgba(255,255,255,0.05);}";
    s += L".script-item svg{width:12px;height:12px;stroke:#707070;fill:none;stroke-width:1.5;stroke-linecap:round;flex-shrink:0;}";
    s += L".script-item:hover svg{stroke:#999;}";
    s += L".script-item-name{overflow:hidden;text-overflow:ellipsis;}";
    s += L".scripts-empty{padding:12px;font-size:11px;color:#555;text-align:center;line-height:1.6;}";

    return s;
}

std::wstring StyleBuilder::ToolbarStyles() {
    std::wstring s;

    s += L".toolbar{height:36px;background:var(--bg2);border-top:1px solid var(--border);";
    s += L"display:flex;align-items:center;padding:0 8px;flex-shrink:0;}";
    s += L".tb-l{display:flex;align-items:center;gap:2px;}";
    s += L".btn{height:22px;padding:0 10px;background:transparent;border:1px solid transparent;";
    s += L"color:#a0a0a0;font-size:11px;font-weight:400;cursor:pointer;font-family:inherit;";
    s += L"letter-spacing:0.3px;text-transform:uppercase;display:flex;align-items:center;gap:5px;";
    s += L"transition:all 0.1s ease;user-select:none;}";
    s += L".btn:hover{background:var(--acc2);color:#d0d0d0;border-color:var(--border2);}";
    s += L".btn:active{background:var(--bg);}";
    s += L".btn svg{width:11px;height:11px;stroke:currentColor;fill:none;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round;flex-shrink:0;}";
    s += L".sep{width:1px;height:14px;background:var(--border2);margin:0 4px;}";

    return s;
}

std::wstring StyleBuilder::EmptyStateStyles() {
    std::wstring s;

    s += L".empty-state{position:absolute;inset:0;display:flex;flex-direction:column;align-items:center;justify-content:center;";
    s += L"color:var(--fg3);gap:12px;z-index:0;}";
    s += L".empty-state.hidden{display:none;}";
    s += L".empty-icon svg{width:32px;height:32px;stroke:#555;fill:none;stroke-width:1.2;stroke-linecap:round;}";
    s += L".empty-text{font-size:12px;color:#666;text-align:center;line-height:1.6;}";
    s += L".empty-link{color:#888;cursor:pointer;text-decoration:underline;text-underline-offset:2px;transition:color 0.1s;}";
    s += L".empty-link:hover{color:#aaa;}";

    return s;
}