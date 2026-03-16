#include "ScriptBuilder.h"

std::wstring ScriptBuilder::Build() {
    std::wstring script;

    script += CoreFunctions();
    script += StatusFunctions();
    script += ScriptListFunctions();
    script += EventBindings();
    script += MonacoSetup();
    script += EditorFactory();
    script += TabManagement();
    script += SettingsSetup();
    script += ResizeHandling();

    return script;
}

std::wstring ScriptBuilder::CoreFunctions() {
    std::wstring s;

    s += L"function send(m){window.chrome.webview.postMessage(m);}";

    s += L"var viewEditor=document.getElementById('view-editor');";
    s += L"var viewSettings=document.getElementById('view-settings');";
    s += L"var currentFontSize=13;";
    s += L"var editors={},tabNames={},activeTab=null,usedIds={};";
    s += L"var monacoReady=false;";
    s += L"var DEF='warn(\"Synz here!\")\\n';";

    s += L"function showEditor(){";
    s += L"viewSettings.classList.remove('active');";
    s += L"viewEditor.classList.add('active');";
    s += L"var ed=getActive();";
    s += L"if(ed)setTimeout(function(){ed.layout();ed.focus();},50);}";

    s += L"function showSettings(){";
    s += L"viewEditor.classList.remove('active');";
    s += L"viewSettings.classList.add('active');}";

    s += L"function getNextId(){for(var i=1;;i++){if(!usedIds[i])return i;}}";

    s += L"function updateEmpty(){";
    s += L"var has=Object.keys(editors).length>0;";
    s += L"document.getElementById('empty-state').classList.toggle('hidden',has);}";

    s += L"function updateFontSizeUI(){";
    s += L"var slider=document.getElementById('sp-fontsize');";
    s += L"var label=document.getElementById('sp-fontsize-val');";
    s += L"if(slider)slider.value=currentFontSize;";
    s += L"if(label)label.textContent=currentFontSize;}";

    s += L"function getActive(){";
    s += L"return(activeTab&&editors[activeTab])?editors[activeTab]:null;}";

    s += L"function applyToAll(fn){";
    s += L"Object.keys(editors).forEach(function(k){fn(editors[k]);});}";

    return s;
}

std::wstring ScriptBuilder::StatusFunctions() {
    std::wstring s;

    s += L"function addConsoleOutput(msg,type){";
    s += L"console.log('[SynZ]['+type+'] '+msg);}";

    return s;
}

std::wstring ScriptBuilder::ScriptListFunctions() {
    std::wstring s;

    s += L"function populateScripts(list){";
    s += L"var container=document.getElementById('scripts-list');";
    s += L"while(container.firstChild){container.removeChild(container.firstChild);}";
    s += L"if(!list||list.length===0){";
    s += L"var emptyDiv=document.createElement('div');";
    s += L"emptyDiv.className='scripts-empty';";
    s += L"emptyDiv.innerHTML='No scripts found.<br>Place .lua files in<br>the Scripts folder.';";
    s += L"container.appendChild(emptyDiv);return;}";
    s += L"list.forEach(function(name){";
    s += L"var item=document.createElement('div');";
    s += L"item.className='script-item';";
    s += L"var svg=document.createElementNS('http://www.w3.org/2000/svg','svg');";
    s += L"svg.setAttribute('viewBox','0 0 24 24');";
    s += L"var p1=document.createElementNS('http://www.w3.org/2000/svg','path');";
    s += L"p1.setAttribute('d','M13 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V9z');";
    s += L"var p2=document.createElementNS('http://www.w3.org/2000/svg','polyline');";
    s += L"p2.setAttribute('points','13 2 13 9 20 9');";
    s += L"svg.appendChild(p1);svg.appendChild(p2);";
    s += L"var span=document.createElement('span');";
    s += L"span.className='script-item-name';";
    s += L"span.textContent=name;";
    s += L"item.appendChild(svg);item.appendChild(span);";
    s += L"item.addEventListener('click',function(){send('load-script:'+name);});";
    s += L"container.appendChild(item);});}";

    return s;
}

std::wstring ScriptBuilder::EventBindings() {
    std::wstring s;

    s += L"document.getElementById('btn-settings').addEventListener('click',function(){";
    s += L"if(viewSettings.classList.contains('active')){showEditor();}";
    s += L"else{showSettings();}});";

    s += L"document.getElementById('btn-back').addEventListener('click',function(){showEditor();});";
    s += L"document.getElementById('btn-min').addEventListener('click',function(){send('wc-min');});";
    s += L"document.getElementById('btn-max').addEventListener('click',function(){send('wc-max');});";
    s += L"document.getElementById('btn-close').addEventListener('click',function(){send('wc-close');});";
    s += L"document.getElementById('btn-new-tab').addEventListener('click',function(){newTab();});";

    s += L"document.getElementById('btn-run').addEventListener('click',function(){";
    s += L"var ed=getActive();if(ed)send('exec:'+ed.getValue());});";

    s += L"document.getElementById('btn-clear').addEventListener('click',function(){";
    s += L"var ed=getActive();if(ed){ed.setValue('');ed.focus();}});";

    s += L"document.getElementById('btn-open').addEventListener('click',function(){send('open');});";

    s += L"document.getElementById('btn-save').addEventListener('click',function(){";
    s += L"var ed=getActive();if(ed)send('savedata:'+ed.getValue());});";

    s += L"document.getElementById('empty-open').addEventListener('click',function(){newTab();});";

    s += L"document.getElementById('btn-refresh-scripts').addEventListener('click',function(){";
    s += L"var btn=this;";
    s += L"if(btn.classList.contains('spinning'))return;";
    s += L"btn.classList.add('spinning');";
    s += L"send('refresh-scripts');";
    s += L"var svg=btn.querySelector('svg');";
    s += L"if(svg){svg.addEventListener('animationend',function handler(){";
    s += L"btn.classList.remove('spinning');";
    s += L"svg.removeEventListener('animationend',handler);";
    s += L"});}";
    s += L"});";

    s += L"var dragReg=document.getElementById('drag-region');";
    s += L"dragReg.addEventListener('mousedown',function(e){";
    s += L"if(e.button!==0)return;e.preventDefault();";
    s += L"send('drag-start:'+e.screenX+':'+e.screenY);});";
    s += L"dragReg.addEventListener('dblclick',function(){send('wc-max');});";

    return s;
}

std::wstring ScriptBuilder::MonacoSetup() {
    std::wstring s;

    s += L"var sc=document.createElement('script');";
    s += L"sc.src='https://cdnjs.cloudflare.com/ajax/libs/monaco-editor/0.45.0/min/vs/loader.min.js';";
    s += L"sc.onload=function(){";
    s += L"require.config({paths:{vs:'https://cdnjs.cloudflare.com/ajax/libs/monaco-editor/0.45.0/min/vs'}});";
    s += L"require(['vs/editor/editor.main'],function(){";
    s += L"monacoReady=true;";
    s += L"setupMonaco();";
    s += L"newTab();";
    s += L"setupSettings();";
    s += L"send('refresh-scripts');";
    s += L"});};";
    s += L"document.head.appendChild(sc);";

    s += L"function setupMonaco(){";
    s += MonacoTheme();
    s += LuaLanguage();
    s += LuaCompletions();
    s += L"}";

    return s;
}

std::wstring ScriptBuilder::MonacoTheme() {
    std::wstring s;

    s += L"monaco.editor.defineTheme('synz-dark',{base:'vs-dark',inherit:true,rules:[";
    s += L"{token:'',foreground:'ababab',background:'1e1e1e'},";
    s += L"{token:'comment',foreground:'484848',fontStyle:'italic'},";
    s += L"{token:'comment.doc',foreground:'5a7a5c',fontStyle:'italic'},";
    s += L"{token:'keyword',foreground:'c586c0'},";
    s += L"{token:'keyword.operator',foreground:'c8c8c8'},";
    s += L"{token:'string',foreground:'ce9178'},";
    s += L"{token:'string.escape',foreground:'d7ba7d'},";
    s += L"{token:'string.invalid',foreground:'f44747'},";
    s += L"{token:'number',foreground:'b5cea8'},";
    s += L"{token:'number.float',foreground:'b5cea8'},";
    s += L"{token:'number.hex',foreground:'b5cea8'},";
    s += L"{token:'delimiter',foreground:'555555'},";
    s += L"{token:'delimiter.curly',foreground:'777777'},";
    s += L"{token:'delimiter.square',foreground:'777777'},";
    s += L"{token:'delimiter.parenthesis',foreground:'777777'},";
    s += L"{token:'operator',foreground:'c8c8c8'},";
    s += L"{token:'identifier',foreground:'9cdcfe'},";
    s += L"{token:'type',foreground:'4ec9b0'},";
    s += L"{token:'global',foreground:'dcdcaa'},";
    s += L"{token:'predefined',foreground:'4fc1ff'},";
    s += L"{token:'variable.self',foreground:'569cd6'}";
    s += L"],colors:{";
    s += L"'editor.background':'#1e1e1e',";
    s += L"'editor.foreground':'#ababab',";
    s += L"'editorCursor.foreground':'#909090',";
    s += L"'editor.lineHighlightBackground':'#242424',";
    s += L"'editor.selectionBackground':'#383838',";
    s += L"'editor.inactiveSelectionBackground':'#2c2c2c',";
    s += L"'editorLineNumber.foreground':'#2e2e2e',";
    s += L"'editorLineNumber.activeForeground':'#585858',";
    s += L"'editorIndentGuide.background':'#282828',";
    s += L"'editorIndentGuide.activeBackground':'#363636',";
    s += L"'scrollbar.shadow':'#00000000',";
    s += L"'scrollbarSlider.background':'#282828',";
    s += L"'scrollbarSlider.hoverBackground':'#323232',";
    s += L"'scrollbarSlider.activeBackground':'#3c3c3c',";
    s += L"'editorOverviewRuler.border':'#00000000',";
    s += L"'minimap.background':'#1e1e1e',";
    s += L"'editorGutter.background':'#1e1e1e',";
    s += L"'editorWidget.background':'#1e1e1e',";
    s += L"'editorWidget.border':'#2e2e2e',";
    s += L"'editorSuggestWidget.background':'#1c1c1c',";
    s += L"'editorSuggestWidget.border':'#2a2a2a',";
    s += L"'editorSuggestWidget.selectedBackground':'#282828',";
    s += L"'input.background':'#232323',";
    s += L"'input.border':'#303030',";
    s += L"'list.hoverBackground':'#252525',";
    s += L"'list.activeSelectionBackground':'#2a2a2a',";
    s += L"'focusBorder':'#00000000'";
    s += L"}});";

    return s;
}

std::wstring ScriptBuilder::LuaLanguage() {
    std::wstring s;

    s += L"monaco.languages.register({id:'lua'});";
    s += L"monaco.languages.setMonarchTokensProvider('lua',{";
    s += L"defaultToken:'',";
    s += L"keywords:['and','break','do','else','elseif','end','false','for','function','goto',";
    s += L"'if','in','local','nil','not','or','repeat','return','then','true','until','while','continue'],";
    s += L"globals:['game','workspace','script','Instance','Vector3','Vector2','CFrame','Color3',";
    s += L"'BrickColor','Enum','UDim2','UDim','Ray','Region3','TweenInfo','math','string','table',";
    s += L"'coroutine','task','pcall','xpcall','spawn','delay','wait','typeof','type','print','warn',";
    s += L"'error','pairs','ipairs','next','select','unpack','rawget','rawset','setmetatable',";
    s += L"'getmetatable','tonumber','tostring','require','tick','time','os','debug','bit32','utf8',";
    s += L"'loadstring','newproxy','_G','_ENV'],";
    s += L"brackets:[{open:'{',close:'}',token:'delimiter.curly'},";
    s += L"{open:'[',close:']',token:'delimiter.square'},";
    s += L"{open:'(',close:')',token:'delimiter.parenthesis'}],";
    s += L"tokenizer:{root:[";
    s += L"[/--\\[([=]*)\\[[\\s\\S]*?\\]\\1\\]/,'comment.doc'],";
    s += L"[/--.*$/,'comment'],";
    s += L"[/\\[([=]*)\\[[\\s\\S]*?\\]\\1\\]/,'string'],";
    s += L"[/\"([^\"\\\\]|\\\\.)*$/,'string.invalid'],";
    s += L"[/'([^'\\\\]|\\\\.)*$/,'string.invalid'],";
    s += L"[/\"/,'string','@sdq'],";
    s += L"[/'/,'string','@ssq'],";
    s += L"[/0[xX][0-9a-fA-F]+/,'number.hex'],";
    s += L"[/\\d*\\.\\d+([eE][\\-+]?\\d+)?/,'number.float'],";
    s += L"[/\\d+/,'number'],";
    s += L"[/self(?!\\w)/,'variable.self'],";
    s += L"[/[a-zA-Z_]\\w*/,{cases:{'@keywords':'keyword','@globals':'global','@default':'identifier'}}],";
    s += L"[/[{}()\\[\\]]/,'@brackets'],";
    s += L"[/\\.\\.|\\.\\.\\./,'operator'],";
    s += L"[/[=~<>]=?/,'operator'],";
    s += L"[/[+\\-*\\/%^#]/,'operator'],";
    s += L"[/[;,.]/,'delimiter'],";
    s += L"[/:/,'delimiter']],";
    s += L"sdq:[[/\\\\[abfnrtv\\\\\"'\\n]/,'string.escape'],";
    s += L"[/\\\\\\d{1,3}/,'string.escape'],";
    s += L"[/\\\\x[0-9a-fA-F]{2}/,'string.escape'],";
    s += L"[/[^\\\\\"]+/,'string'],";
    s += L"[/\"/,'string','@pop']],";
    s += L"ssq:[[/\\\\[abfnrtv\\\\\"'\\n]/,'string.escape'],";
    s += L"[/\\\\\\d{1,3}/,'string.escape'],";
    s += L"[/\\\\x[0-9a-fA-F]{2}/,'string.escape'],";
    s += L"[/[^\\\\']+/,'string'],";
    s += L"[/'/,'string','@pop']]}});";

    return s;
}

std::wstring ScriptBuilder::LuaCompletions() {
    std::wstring s;

    s += L"monaco.languages.registerCompletionItemProvider('lua',{";
    s += L"provideCompletionItems:function(model,pos){";
    s += L"var w=model.getWordUntilPosition(pos);";
    s += L"var r={startLineNumber:pos.lineNumber,endLineNumber:pos.lineNumber,";
    s += L"startColumn:w.startColumn,endColumn:w.endColumn};";
    s += L"var items=['game','workspace','script','print','warn','error','wait',";
    s += L"'task','task.wait','task.spawn','task.delay',";
    s += L"'Instance','Instance.new','Vector3','Vector3.new','CFrame','CFrame.new',";
    s += L"'Color3','Color3.fromRGB','UDim2','UDim2.new',";
    s += L"'FindFirstChild','FindFirstChildOfClass','WaitForChild','GetService',";
    s += L"'GetChildren','GetDescendants','Clone','Destroy','IsA','Connect',";
    s += L"'Fire','Invoke','Players','LocalPlayer','Character','Humanoid',";
    s += L"'HumanoidRootPart','Head','Parent','Name','ClassName',";
    s += L"'TweenService','ReplicatedStorage','RunService','UserInputService',";
    s += L"'pcall','xpcall','pairs','ipairs','setmetatable','getmetatable',";
    s += L"'tonumber','tostring',";
    s += L"'math.abs','math.floor','math.ceil','math.sqrt','math.random',";
    s += L"'math.max','math.min','math.pi','math.huge',";
    s += L"'string.sub','string.find','string.format','string.rep',";
    s += L"'string.upper','string.lower','string.byte','string.char',";
    s += L"'table.insert','table.remove','table.sort','table.concat',";
    s += L"'table.find','loadstring'];";
    s += L"return{suggestions:items.map(function(l){";
    s += L"return{label:l,kind:monaco.languages.CompletionItemKind.Function,";
    s += L"insertText:l,range:r};})};";
    s += L"}});";

    return s;
}

std::wstring ScriptBuilder::EditorFactory() {
    std::wstring s;

    s += L"function mkEditor(id,val){";
    s += L"var container=document.getElementById('editors');";
    s += L"var p=document.createElement('div');";
    s += L"p.className='ep';p.id='ep-'+id;";
    s += L"p.style.width=container.offsetWidth+'px';";
    s += L"p.style.height=container.offsetHeight+'px';";
    s += L"container.appendChild(p);";

    s += L"var cfg={";
    s += L"value:(val!=null)?val:DEF,";
    s += L"language:'lua',theme:'synz-dark',fontSize:currentFontSize,";
    s += L"fontFamily:\"'Cascadia Mono','Consolas','Courier New',monospace\",";
    s += L"fontLigatures:false,lineHeight:20,letterSpacing:0,";
    s += L"minimap:{enabled:false},scrollBeyondLastLine:false,";
    s += L"smoothScrolling:true,cursorBlinking:'smooth',cursorSmoothCaretAnimation:'on',";
    s += L"cursorStyle:'line',cursorWidth:1,";
    s += L"renderLineHighlight:'line',renderWhitespace:'none',";
    s += L"padding:{top:10,bottom:10},lineNumbers:'on',lineNumbersMinChars:4,";
    s += L"lineDecorationsWidth:12,glyphMargin:false,";
    s += L"folding:true,bracketPairColorization:{enabled:false},";
    s += L"guides:{indentation:true,bracketPairs:false},";
    s += L"suggest:{showKeywords:true,showSnippets:true,showFunctions:true},";
    s += L"tabSize:4,insertSpaces:true,wordWrap:'off',automaticLayout:false,";
    s += L"overviewRulerBorder:false,hideCursorInOverviewRuler:true,overviewRulerLanes:0,";
    s += L"scrollbar:{vertical:'auto',horizontal:'auto',";
    s += L"verticalScrollbarSize:6,horizontalScrollbarSize:6,useShadows:false}";
    s += L"};";

    s += L"var ed=monaco.editor.create(p,cfg);";

    s += L"ed.getDomNode().addEventListener('wheel',function(e){";
    s += L"if(e.ctrlKey){";
    s += L"e.preventDefault();e.stopPropagation();";
    s += L"if(e.deltaY<0){currentFontSize=Math.min(currentFontSize+1,24);}";
    s += L"else{currentFontSize=Math.max(currentFontSize-1,10);}";
    s += L"applyToAll(function(ed2){ed2.updateOptions({fontSize:currentFontSize});});";
    s += L"updateFontSizeUI();}";
    s += L"},{passive:false});";

    s += L"editors[id]=ed;";
    s += L"return ed;}";

    return s;
}

std::wstring ScriptBuilder::TabManagement() {
    std::wstring s;

    s += L"function newTab(val,name){";
    s += L"if(!monacoReady)return;";
    s += L"var id=getNextId();";
    s += L"usedIds[id]=true;";
    s += L"tabNames[id]=name||('Script '+id);";

    s += L"var bar=document.getElementById('tabbar');";
    s += L"var t=document.createElement('div');";
    s += L"t.className='tab';t.id='tab-'+id;";
    s += L"t.setAttribute('draggable','true');";

    s += L"var nameSpan=document.createElement('span');";
    s += L"nameSpan.className='tab-name';";
    s += L"nameSpan.textContent=tabNames[id];";
    s += L"t.appendChild(nameSpan);";

    s += L"var xBtn=document.createElement('button');";
    s += L"xBtn.className='tab-x';";
    s += L"var xSvg=document.createElementNS('http://www.w3.org/2000/svg','svg');";
    s += L"xSvg.setAttribute('viewBox','0 0 8 8');";
    s += L"var l1=document.createElementNS('http://www.w3.org/2000/svg','line');";
    s += L"l1.setAttribute('x1','1');l1.setAttribute('y1','1');";
    s += L"l1.setAttribute('x2','7');l1.setAttribute('y2','7');";
    s += L"var l2=document.createElementNS('http://www.w3.org/2000/svg','line');";
    s += L"l2.setAttribute('x1','7');l2.setAttribute('y1','1');";
    s += L"l2.setAttribute('x2','1');l2.setAttribute('y2','7');";
    s += L"xSvg.appendChild(l1);xSvg.appendChild(l2);";
    s += L"xBtn.appendChild(xSvg);";

    s += L"(function(tid,tab,ns,xb){";
    s += L"tab.addEventListener('mousedown',function(e){";
    s += L"if(e.target===xb||xb.contains(e.target))return;";
    s += L"if(e.target.classList&&e.target.classList.contains('tab-rename'))return;";
    s += L"activateTab(tid);});";

    s += L"xb.addEventListener('mousedown',function(e){";
    s += L"e.stopPropagation();closeTab(tid);});";

    s += L"tab.addEventListener('dblclick',function(e){";
    s += L"if(e.target===xb||xb.contains(e.target))return;";
    s += L"startRename(tid);});";

    s += L"tab.addEventListener('dragstart',function(e){";
    s += L"e.dataTransfer.setData('text/plain',tid.toString());";
    s += L"e.dataTransfer.effectAllowed='move';";
    s += L"setTimeout(function(){tab.style.opacity='0.4';},0);});";

    s += L"tab.addEventListener('dragend',function(){tab.style.opacity='';});";

    s += L"tab.addEventListener('dragover',function(e){";
    s += L"e.preventDefault();e.dataTransfer.dropEffect='move';";
    s += L"tab.classList.add('drag-over');});";

    s += L"tab.addEventListener('dragleave',function(){";
    s += L"tab.classList.remove('drag-over');});";

    s += L"tab.addEventListener('drop',function(e){";
    s += L"e.preventDefault();tab.classList.remove('drag-over');";
    s += L"var fromId=parseInt(e.dataTransfer.getData('text/plain'));";
    s += L"if(fromId===tid)return;";
    s += L"var fromTab=document.getElementById('tab-'+fromId);";
    s += L"if(!fromTab)return;";
    s += L"var bar=document.getElementById('tabbar');";
    s += L"bar.insertBefore(fromTab,tab);});";

    s += L"})(id,t,nameSpan,xBtn);";

    s += L"t.appendChild(xBtn);";
    s += L"bar.insertBefore(t,bar.lastElementChild);";
    s += L"mkEditor(id,(val!==undefined)?val:null);";
    s += L"activateTab(id);";
    s += L"updateEmpty();}";

    s += L"function startRename(id){";
    s += L"var tab=document.getElementById('tab-'+id);";
    s += L"if(!tab)return;";
    s += L"var ns=tab.querySelector('.tab-name');";
    s += L"if(!ns)return;";
    s += L"var inp=document.createElement('input');";
    s += L"inp.className='tab-rename';";
    s += L"inp.value=tabNames[id];inp.maxLength=20;";
    s += L"ns.style.display='none';";
    s += L"tab.insertBefore(inp,ns);";
    s += L"inp.focus();inp.select();";

    s += L"function finish(){";
    s += L"var v=inp.value.trim();";
    s += L"if(v)tabNames[id]=v;";
    s += L"ns.textContent=tabNames[id];";
    s += L"ns.style.display='';";
    s += L"if(inp.parentNode)inp.remove();}";

    s += L"inp.addEventListener('blur',finish);";
    s += L"inp.addEventListener('keydown',function(e){";
    s += L"if(e.key==='Enter'){e.preventDefault();inp.blur();}";
    s += L"if(e.key==='Escape'){inp.value=tabNames[id];inp.blur();}";
    s += L"e.stopPropagation();});";
    s += L"inp.addEventListener('mousedown',function(e){e.stopPropagation();});}";

    s += L"function activateTab(id){";
    s += L"document.querySelectorAll('.tab').forEach(function(t){t.classList.remove('active');});";
    s += L"document.querySelectorAll('.ep').forEach(function(p){p.classList.remove('active');});";
    s += L"var t=document.getElementById('tab-'+id);";
    s += L"var p=document.getElementById('ep-'+id);";
    s += L"if(t)t.classList.add('active');";
    s += L"if(p)p.classList.add('active');";
    s += L"activeTab=id;";
    s += L"var container=document.getElementById('editors');";
    s += L"if(p){p.style.width=container.offsetWidth+'px';";
    s += L"p.style.height=container.offsetHeight+'px';}";
    s += L"if(editors[id]){";
    s += L"setTimeout(function(){";
    s += L"if(editors[id]){editors[id].layout();editors[id].focus();}";
    s += L"},50);}}";

    s += L"function closeTab(id){";
    s += L"var tab=document.getElementById('tab-'+id);";
    s += L"var pane=document.getElementById('ep-'+id);";
    s += L"if(tab)tab.remove();";
    s += L"if(pane)pane.remove();";
    s += L"if(editors[id]){editors[id].dispose();delete editors[id];}";
    s += L"delete tabNames[id];delete usedIds[id];";
    s += L"if(activeTab===id){";
    s += L"var rem=Object.keys(editors);";
    s += L"if(rem.length){activateTab(parseInt(rem[rem.length-1]));}";
    s += L"else{activeTab=null;}}";
    s += L"updateEmpty();}";

    return s;
}

std::wstring ScriptBuilder::SettingsSetup() {
    std::wstring s;

    s += L"function setupSettings(){";

    s += L"var fs=document.getElementById('sp-fontsize');";
    s += L"var fsv=document.getElementById('sp-fontsize-val');";
    s += L"fs.addEventListener('input',function(){";
    s += L"currentFontSize=parseInt(fs.value);";
    s += L"fsv.textContent=currentFontSize;";
    s += L"applyToAll(function(ed){ed.updateOptions({fontSize:currentFontSize});});});";

    s += L"document.getElementById('sp-tabsize').addEventListener('change',function(){";
    s += L"var v=parseInt(this.value);";
    s += L"applyToAll(function(ed){ed.getModel().updateOptions({tabSize:v});});});";

    s += L"document.getElementById('sp-wordwrap').addEventListener('change',function(){";
    s += L"var v=this.value;";
    s += L"applyToAll(function(ed){ed.updateOptions({wordWrap:v});});});";

    s += L"document.getElementById('sp-linenums').addEventListener('change',function(){";
    s += L"var v=this.value;";
    s += L"applyToAll(function(ed){ed.updateOptions({lineNumbers:v});});});";

    s += L"document.getElementById('sp-minimap').addEventListener('change',function(){";
    s += L"var v=this.checked;";
    s += L"applyToAll(function(ed){ed.updateOptions({minimap:{enabled:v}});});});";

    s += L"document.getElementById('sp-smooth').addEventListener('change',function(){";
    s += L"var v=this.checked?'on':'off';";
    s += L"applyToAll(function(ed){ed.updateOptions({cursorSmoothCaretAnimation:v});});});";

    s += L"document.getElementById('sp-font').addEventListener('change',function(){";
    s += L"var v=this.value;";
    s += L"applyToAll(function(ed){ed.updateOptions({fontFamily:v});});});";

    s += L"}";

    return s;
}

std::wstring ScriptBuilder::ResizeHandling() {
    std::wstring s;

    s += L"window.addEventListener('resize',function(){";
    s += L"var container=document.getElementById('editors');";
    s += L"document.querySelectorAll('.ep').forEach(function(p){";
    s += L"p.style.width=container.offsetWidth+'px';";
    s += L"p.style.height=container.offsetHeight+'px';});";
    s += L"if(activeTab&&editors[activeTab]){editors[activeTab].layout();}});";

    s += L"new ResizeObserver(function(){";
    s += L"var container=document.getElementById('editors');";
    s += L"var w=container.offsetWidth,h=container.offsetHeight;";
    s += L"document.querySelectorAll('.ep').forEach(function(p){";
    s += L"p.style.width=w+'px';p.style.height=h+'px';});";
    s += L"if(activeTab&&editors[activeTab]){editors[activeTab].layout();}";
    s += L"}).observe(document.getElementById('editors'));";

    return s;
}