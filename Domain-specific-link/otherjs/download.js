/**
 * js 函数集合
 *
 * @author kafeifei@verycd.com
 *
 * Copyright 2005 VeryCD Team and Kafeifei<@http://www.yb-cn.com/>
 * Some Rights Reserved
 *
 */

function dumpVariable(o) {
    var rstr = "<ul>";
    for (var n in o) {
        if (typeof o[n] == "string") {
            rstr += "<li><strong>" + n + "</strong> " + o[n] + "</li>";
        } else {
            dumpVariable(o[n]);
        }
    }
    rstr += "</ul>";
    return rstr;
}

function convertFilename(str) {
    //str = str.replace(' ', '%20');
    str = encodeURIComponent(str);
    return str;
}

/**
* 根据输入的byte单位，计算出最简洁的表示数据
* 例如1024 得 1k
*/
function humanReadableSize(f_size) {
    return getSmartSize(f_size);
}

function getSmartSize(f_size) {
    var funit, k_size, m_size, g_size, f_size;
    
    k_size = f_size / 1024;
    m_size = k_size / 1024;
    g_size = m_size / 1024;
    
    if (1024 > f_size) {
        funit = 'b';
        r_size = f_size;
    } else if (1024 > k_size) {
        funit = 'K';
        r_size = Math.round(k_size);
    } else if (1024 > m_size) {
        funit = 'M';
        r_size = Math.round(m_size * 10) / 10;
    } else {
        funit = 'G';
        r_size = Math.round(g_size * 10) / 10;
    }
    
    return '' + r_size + funit;
}

/**
* 设置默认选中的 selection
*/
function def_select(varname, val) {
    var tobj = $(varname);
    if (!tobj) {
        return;
    }
    for (var i = 0; i < tobj.options.length; i++) {
        if (tobj.options[i].value == val) {
            tobj.options[i].selected = 'selected';
        } else if ('n' == val) {
            tobj.options[i].selected = false;
        }
    }
}


/**
* isset 有些问题
*/
function isset(any) {
    if ('undefined' == typeof (any)) {
        return false;
    }
    return true;
}

/**
* rsspot
*/
function rsspot$sendRPCDone(ht_str) {
    document.write(ht_str);
}

/**
* getElementsByAttribute
*/
document.getElementsByAttribute = function(attrN, attrV, multi) {
    attrV = attrV.replace(/\|/g, '\\|').replace(/\[/g, '\\[').replace(/\(/g, '\\(').replace(/\+/g, '\\+').replace(/\./g, '\\.').replace(/\*/g, '\\*').replace(/\?/g, '\\?').replace(/\//g, '\\/');
    var 
    multi = typeof multi != 'undefined' ? 
    multi : 
    false, 
    cIterate = document.getElementsByTagName('*'), 
    aResponse = [], 
    attr, 
    re = new RegExp(multi ? '\\b' + attrV + '\\b' : '^' + attrV + '$'), 
    i = 0, 
    elm;
    while ((elm = cIterate.item(i++))) {
        attr = elm.getAttributeNode(attrN);
        if (attr && 
        attr.specified && 
        re.test(attr.value)
        )
            aResponse.push(elm);
    }
    return aResponse;
}

var admin_mod_visit = false;
function admin_menu_visit() {
    var adm_entrys = document.getElementsByAttribute('class', 'admin-mod');
    if (null != adm_entrys) {
        for (var i = 0; i < adm_entrys.length; i++) {
            if (!admin_mod_visit) {
                adm_entrys[i].style.display = 'inline';
            } else {
                adm_entrys[i].style.display = 'none';
            }
        }
        admin_mod_visit = admin_mod_visit ? false : true;
    }
}

function toggle(obj) {
    var el = document.getElementById(obj);
    if (el.style.display != 'none') {
        el.style.display = 'none';
    } 
    else {
        el.style.display = 'block';
    }
}


var issubtitle = true;
function addSubtitle() {
    var subtitleDIV = document.getElementById('add_subtitle');
    if (null != subtitleDIV) {
        if (!issubtitle) {
            subtitleDIV.style.display = 'none';
        } else {
            subtitleDIV.style.display = 'block';
        
        }
        issubtitle = issubtitle ? false : true;
    }
}

function delconfirm() {
    return confirm('确定删除吗？没有进一步提示了！');
}

function vcg_confirm() {
    return confirm('确认操作吗？没有进一步提示了！');
}

//////// 文本框文字处理 ////////
function setSelectionRange(input, selectionStart, selectionEnd) {
    if (input.setSelectionRange) {
        input.focus();
        input.setSelectionRange(selectionStart, selectionEnd);
    } 
    else if (input.createTextRange) {
        var range = input.createTextRange();
        range.collapse(true);
        range.moveEnd('character', selectionEnd);
        range.moveStart('character', selectionStart);
        range.select();
    }
}
function setCaretToEnd(input) {
    setSelectionRange(input, input.value.length, input.value.length);
}
function setCaretToBegin(input) {
    setSelectionRange(input, 0, 0);
}
function setCaretToPos(input, pos) {
    setSelectionRange(input, pos, pos);
}
function selectString(input, string) {
    var match = new RegExp(string, "i").exec(input.value);
    if (match) {
        setSelectionRange(input, match.index, match.index + match
        [0].length);
    }
}
function replaceSelection(input, replaceString) {
    if (input.setSelectionRange) {
        var selectionStart = input.selectionStart;
        var selectionEnd = input.selectionEnd;
        input.value = input.value.substring(0, selectionStart) 
        + replaceString 
        + input.value.substring(selectionEnd);
        if (selectionStart != selectionEnd) // has there been a selection
            setSelectionRange(input, selectionStart, selectionStart + 
            replaceString.length);
        else // set caret
            setCaretToPos(input, selectionStart + replaceString.length);
    } 
    else if (document.selection) {
        var range = document.selection.createRange();
        if (range.parentElement() == input) {
            var isCollapsed = range.text == '';
            range.text = replaceString;
            if (!isCollapsed) { // there has been a selection
                //it appears range.select() should select the newly
                //inserted text but that fails with IE
                range.moveStart('character', -replaceString.length);
                range.select();
            }
        }
    }
}

///////////// 全局 /////////////
// Example:
// writeCookie("myCookie", "my name", 24);
// Stores the string "my name" in the cookie "myCookie" which expires after 24 hours.
function setCookie(name, value, hours) {
    var expire = "";
    if (hours != null) {
        expire = new Date((new Date()).getTime() + hours * 3600000);
        expire = "; expires=" + expire.toGMTString();
    }
    document.cookie = name + "=" + escape(value) + expire + ';domain=.verycd.com;path=/;';
}

// Example:
// alert( readCookie("myCookie") );
function readCookie(name) {
    var cookieValue = "";
    var search = name + "=";
    if (document.cookie.length > 0) {
        offset = document.cookie.indexOf(search);
        if (offset != -1) {
            offset += search.length;
            end = document.cookie.indexOf(";", offset);
            if (end == -1)
                end = document.cookie.length;
            cookieValue = unescape(document.cookie.substring(offset, end))
        }
    }
    return cookieValue;
}

// 同时兼容opera - accesskey
// 同时支持不推荐的ctrl+enter
function check_hotkey(event) {
    if (event.altKey && 83 == event.keyCode) {
        document.getElementById('submit-button').click();
    }
    if (event.ctrlKey && event.keyCode == 13) {
        document.getElementById('submit-button').click();
    }
}

function set_used_accesskey() {
    setCookie('used_accesskey', '1', 24 * 7);
}

function unset_used_accesskey() {
    setCookie('used_accesskey', '0');
}

function show_trackback(title, topicid) {
    var trackback_v = $('trackback-v-box');
    var tmp = '';
    trackback_v.style.display = 'block';
    tmp = '<script type="text\/javascript" id="rsspot-box" src="';
    tmp += '"><\/script>';
    trackback_v.innerHTML = tmp;
    $('rsspot-box').src = 'http:\/\/www.rsspot.com/js?u[]=http:\/\/beta.verycd.com\/groups\/bugs\/feed';
}

function changeInputerNotice(iid, limit, chars) {
    var noticePanel = $(iid + '_notice');
    var num = (limit - chars) / 2;

    //noticePanel.style.display = '';
    if (0 > num) {
        noticePanel.innerHTML = '<span style="color:red">您已经超出' + Math.abs(num) + '个中文</span>';
    } else {
        noticePanel.innerHTML = "您还可以输入" + num + "个中文";
    }
}

function showInputerNotice(iid) {
    var noticePanel = $(iid + '_notice');
    noticePanel.style.display = '';
}

function hideInputerNotice(iid) {
    var noticePanel = $(iid + '_notice');
    noticePanel.style.display = 'none';
}

function getChars(str) {
    var i = 0;
    var c = 0.0;
    var unicode = 0;
    var len = 0;
    
    if (str == null || str == "") {
        return 0;
    }
    len = str.length;
    for (i = 0; i < len; i++) {
        unicode = str.charCodeAt(i);
        if (unicode < 127) {
            c += 1;
        } else {
            c += 2;
        }
    }
    return c;
}

function sb_strlen(str) {
    return getChars(str);
}

function sb_substr(str, startp, endp) {
    var i = 0;
    c = 0;
    unicode = 0;
    rstr = '';
    var len = str.length;
    var sblen = sb_strlen(str);
    
    if (startp < 0) {
        startp = sblen + startp;
    }
    
    if (endp < 1) {
        endp = sblen + endp; // - ((str.charCodeAt(len-1) < 127) ? 1 : 2);
    }
    // 寻找起点
    for (i = 0; i < len; i++) {
        if (c >= startp) {
            break;
        }
        var unicode = str.charCodeAt(i);
        if (unicode < 127) {
            c += 1;
        } else {
            c += 2;
        }
    }

    // 开始取
    for (i = i; i < len; i++) {
        var unicode = str.charCodeAt(i);
        if (unicode < 127) {
            c += 1;
        } else {
            c += 2;
        }
        rstr += str.charAt(i);
        
        if (c >= endp) {
            break;
        }
    }
    
    return rstr;
}

function getShortFileName(filename) {
    short_filename = filename;
    if (sb_strlen(short_filename) > 61) {
        short_filename = sb_substr(short_filename, 0, 36) + ' ... ' + sb_substr(short_filename, -20);
    }
    return short_filename;
}

///////////// 旧版函数 //////////////
/**
 * FreeIPB eMule下载时使用
 */
function checkAll(str, checked) {
    var a = document.getElementsByName(str);
    var n = a.length;
    
    for (var i = 0; i < n; i++) {
        a[i].checked = checked;
    }
    em_size(str);
}

var check_link = ''; //永远传值
function download(str, i, first) {
    var a = document.getElementsByName(str);
    var n = a.length;
    var isautoDownSub = $('autoDownload');
    var isShowPopPage = false;
    var isContinueDown = false;
    var isGetEd2k = false;
    //判断用户浏览器语言
    if (navigator.userAgent.indexOf("MSIE") > 0) {
        var language = navigator.userLanguage;
    } else {
        var language = navigator.language;
    }
    try {
        var e = e ? e : window.event;
        if (window.event) {
            e.cancelBubble = true;
        } else {
            e.stopPropagation();
        }
    } catch (e) {
    }
    if (VeryCD.emule.installed()) {
        var ed2k_links = '';
        for (var i = i; i < n; i++) {
            if (a[i].checked) {
                if (ed2k_links == '') {
                    ed2k_links = a[i].value;
                } else {
                    ed2k_links += "\n" + a[i].value;
                }
                if (isautoDownSub && isautoDownSub.checked) {
                    isGetEd2k = true;
                    var link = getLanguage(str, i, isContinueDown, isGetEd2k);
                    if (link) {
                        if (ed2k_links == '') {
                            ed2k_links = link;
                        } else {
                            ed2k_links += "\n" + link;
                        }
                    }
                
                }
            }
        }
        VeryCD.emule.SendUrl(ed2k_links, 'dd', document.location);
        return;
    } else {
        if (navigator.userAgent.indexOf("MSIE") > 0) {
            isShowPopPage = true;
        }
    
    }
    
    if (!window.continueDown) {
        isContinueDown = true;
        //使用最旧的方法来批量新增下载
        for (var i = i; i < n; i++) {
            if (a[i].checked) {
                noWinToDown(a[i].value);
                
                if (isautoDownSub && isautoDownSub.checked) {
                    window.setTimeout("getLanguage('" + str + "', " + i + ", " + isContinueDown + "," + isGetEd2k + ")", 50);
                }
                if (first)
                    timeout = 4000;
                
                i++;
                window.setTimeout("do_download('" + str + "', " + i + ", " + isContinueDown + "," + isGetEd2k + ")", timeout);
                break;
            }
        }
    } else {
        if ('never' != getCookie('downPopup') && isShowPopPage == true) {
            check_link = ''; //清空
            for (var i = i; i < n; i++) {
                if (a[i].checked) {
                    if (check_link == '') {
                        check_link = a[i].value;
                    } else {
                        check_link += "," + a[i].value;
                    }
                    if (isautoDownSub && isautoDownSub.checked) {
                        isGetEd2k = true;
                        var link = getLanguage(str, i, isContinueDown, isGetEd2k);
                        if (link) {
                            if (check_link == '') {
                                check_link = link;
                            } else {
                                check_link += "," + link;
                            }
                        }
                    }
                    var k = i;
                    var current_link = a[k].nextSibling;
                    var multi_text = '';
                    var tmp_counter = 0;
                    var comma = '';
                    var right_link = '';
                    while (true) {
                        if (a[k].checked && current_link) { //如果是有效节点并且被选中
                            if (current_link.href) {
                                if (current_link.href.indexOf('ed2k') !== 0) {
                                    current_link = current_link.nextSibling; //获取ed2k的值
                                    continue;
                                }
                                if (tmp_counter > 7) { //收集超过若干个有效链接后，退出
                                    multi_text += '<br />…………';
                                    break;
                                }
                                var right_link = current_link;
                                tmp_counter++;
                                if (navigator.userAgent.toLowerCase().indexOf("msie") == -1) { //ie
                                    multi_text += comma + current_link.text;
                                } else {
                                    multi_text += comma + current_link.innerText;
                                }
                                comma = '<br />';
                            }
                            
                            current_link = current_link.nextSibling;
                        } else { //未被选中，或往下没有相邻节点了，那么切换到下个父节点
                            if (++k >= n) { //如果父节点也到底了，那么退出
                                break;
                            }
                            current_link = a[k].nextSibling;
                        }
                    }
                    
                    downPopup(event);
                
                }
            } //
        } else {
            for (var i = i; i < n; i++) {
                if (a[i].checked) {
                    continueDown(a[i].value);
                    if (isautoDownSub && isautoDownSub.checked) {
                        window.setTimeout("getLanguage('" + str + "', " + i + ", " + isContinueDown + ", " + isGetEd2k + ")", 50);
                    }
                    if (first)
                        timeout = 4000;
                    i++;
                    window.setTimeout("do_download('" + str + "', " + i + ", " + isContinueDown + ", " + isGetEd2k + ")", timeout);
                    break;
                }
            }
        }
    }
}


var is_chrome = navigator.userAgent.toLowerCase().indexOf('chrome') > -1;
function do_download(str, i, isContinueDown) {
    var a = document.getElementsByName(str);
    var n = a.length;
    var isGetEd2k = false;
    var isautoDownSub = $('autoDownload');
    for (var i = i; i < n; i++) {
        if (a[i].checked) {
            if (isContinueDown) {
                noWinToDown(a[i].value);
                if (isautoDownSub && isautoDownSub.checked) {
                    window.setTimeout("getLanguage('" + str + "', " + i + ", " + isContinueDown + ", " + isGetEd2k + ")", 50);
                }
            } else {
                continueDown(a[i].value);
                if (isautoDownSub && isautoDownSub.checked) {
                    window.setTimeout("getLanguage('" + str + "', " + i + ", " + isContinueDown + ", " + isGetEd2k + ")", 50);
                }
            }
            var timeout = 500;
            i++;
            window.setTimeout("do_download('" + str + "', " + i + ", " + isContinueDown + ")", timeout);
            break;
        }
    }
}

function getLanguage(str, i, isContinueDown, isGetEd2k) {
    var a = document.getElementsByName(str);
    //判断用户浏览器语言
    if (navigator.userAgent.indexOf("MSIE") > 0) {
        var language = navigator.userLanguage;
    } else {
        var language = navigator.language;
    }
    if (language.toLowerCase().indexOf('zh-cn') > -1) {
        var ed2k = a[i].getAttribute('subtitle_zh_cn');
    } else if (language.toLowerCase().indexOf('zh-tw') > -1) {
        var ed2k = a[i].getAttribute('subtitle_zh_tw');
    } else if (language.toLowerCase().indexOf('en-us') > -1) {
        var ed2k = a[i].getAttribute('subtitle_en_us');
    } else {
        var ed2k = a[i].getAttribute('subtitle_others');
    }
    if (ed2k && !isGetEd2k) {
        if (isContinueDown) {
            noWinToDown(ed2k);
        
        } else {
            continueDown(ed2k);
        }
    } else if (isGetEd2k) {
        return ed2k;
    }
}

function noWinToDown(link) {
    try {
        if ($('down_iframe')) {
            document.body.removeChild($('down_iframe'))
        }
        //link = encodeURI(link);
        
        var iframe_obj = document.createElement("iframe")
        
        document.body.appendChild(iframe_obj)
        iframe_obj.id = 'down_iframe'
        iframe_obj.height = "0"
        iframe_obj.width = "0"
        iframe_obj.display = "none"
        if (document.all) { // ie 特殊处理
            var fra = '<frameset><frame  src="' + link + '" /></frameset>';
            window.frames['down_iframe'].document.write(fra);
        } else {
            iframe_obj.src = link
        }
    } catch (e) {
        iframe_obj.src = link
    }
}

function copy(str) {
    var a = document.getElementsByName(str);
    var n = a.length;
    var isautoDownSub = $('autoDownload');
    var isContinueDown = false;
    var isGetEd2k = false;
    var ed2kcopy = "";
    for (var i = 0; i < n; i++) {
        if (a[i].checked) {
            ed2kcopy += a[i].value;
            ed2kcopy += "\r\n";
            if (isautoDownSub && isautoDownSub.checked) {
                isGetEd2k = true;
                var link = getLanguage(str, i, isContinueDown, isGetEd2k);
                if (link) {
                    ed2kcopy += link;
                    ed2kcopy += "\r\n";
                }
            }
        }
    }
    //copyToClipboard(ed2kcopy);
    VeryCD.events.copy(ed2kcopy);

}

function copyToClipboard(txt) {
    if (window.clipboardData) {
        window.clipboardData.clearData();
        window.clipboardData.setData("Text", txt);
    } else if (navigator.userAgent.indexOf("Opera") != -1) {
        window.location = txt;
    } else if (window.netscape) {
        try {
            netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
        } catch (e) {
            alert("被浏览器拒绝！\n请在浏览器地址栏输入'about:config'并回车\n然后将'signed.applets.codebase_principal_support'设置为'true'");
        }
        var clip = Components.classes['@mozilla.org/widget/clipboard;1'].createInstance(Components.interfaces.nsIClipboard);
        if (!clip)
            return;
        var trans = Components.classes['@mozilla.org/widget/transferable;1'].createInstance(Components.interfaces.nsITransferable);
        if (!trans)
            return;
        trans.addDataFlavor('text/unicode');
        var str = new Object();
        var len = new Object();
        var str = Components.classes["@mozilla.org/supports-string;1"].createInstance(Components.interfaces.nsISupportsString);
        var copytext = txt;
        str.data = copytext;
        trans.setTransferData("text/unicode", str, copytext.length * 2);
        var clipid = Components.interfaces.nsIClipboard;
        if (!clip)
            return false;
        clip.setData(trans, null, clipid.kGlobalClipboard);
    }
}

/*
function em_size(str) {
    var a = document.getElementsByName(str);
    var n = a.length;
    try {
        var input_checkall = document.getElementById("checkall_"+str);
        var size = 0;
        input_checkall.checked = true ;
        for (var i=0; i < n; i++) {
            if (a[i].checked) {
                var piecesArray = a[i].value.split( "|" );
                size += piecesArray[3]*1;
            } else {
                input_checkall.checked = false;
            }
        }
        test = document.getElementById("size_"+str);
        test.innerHTML = gen_size(size, 3, 1);
    } catch (e) {

    }
}
*/

// get all the seletected
function getEd2k(id) {
    var a = document.getElementsByName(id);
    var n = a.length;
    var isautoDownSub = document.getElementById('autoDownload');
    var isContinueDown = false;
    var isGetEd2k = false;
    var ed2kcopy = "";
    for (var i = 0; i < n; i++) {
        if (a[i].checked) {
            ed2kcopy += a[i].value;
            ed2kcopy += "\r\n";
            if (isautoDownSub && isautoDownSub.checked) {
                isGetEd2k = true;
                var link = getLanguage(id, i, isContinueDown, isGetEd2k);
                if (link) {
                    ed2kcopy += link;
                    ed2kcopy += "\r\n";
                }
            }
        }
    }
    return ed2kcopy;
}

// overwrite the old em_size function
function em_size(str) {
    var a = document.getElementsByName(str);
    var n = a.length;
    try {
        
        var size = 0;
        var input_checkall = document.getElementById("checkall_" + str);
        input_checkall.checked = true;
        for (var i = 0; i < n; i++) {
            if (a[i].checked) {
                var piecesArray = a[i].value.split("|");
                size += piecesArray[3] * 1;
            } else {
                input_checkall.checked = false;
            }
        }
        
        test = document.getElementById("size_" + str);
        test.innerHTML = gen_size(size, 3, 1);
        if (eval("window.document.ed2kcopy_" + str)) {
            eval("window.document.ed2kcopy_" + str).send2flash(getEd2k(str));
        }
    } catch (e) {
    //alert("e"+e.message)
    }
}

function changeUpdateFlash(inputId) {
    var browser = navigator.appName
    if (browser == "Microsoft Internet Explorer") { //ie
        $("updateflash" + inputId + "").innerHTML = "<input type=\'button\' value=\'复制选中的链接\' class=\'button\' onclick=\'copy(\"" + inputId + "\")\' />";
    } else {
        var fls = flashChecker();
        if (fls.f && parseInt(fls.v) >= 10) {
            $("updateflash" + inputId + "").innerHTML = "<object classid=\'clsid:d27cdb6e-ae6d-11cf-96b8-444553540000\' codebase=\'http:\/\/download.macromedia.com\/pub\/shockwave\/cabs\/flash\/swflash.cab#version=9,0,0,0\' width=\'136\' height=\'20\' id=\'ed2kcopy_" + inputId + "\' align=\'middle\' class=\'copyflash\' onload=\'em_size(\"" + inputId + "\");\' class=\'copyedflash\'><param name=\'allowScriptAccess\' value=\'always\' /><param name=\'allowFullScreen\' value=\'false\' /><param name=\'movie\' value=\'/cp2flash.swf?x=11\' /><param name=\'quality\' value=\'high\' /><param name=\'bgcolor\' value=\'#ffffff\' /><param name=\'wmode\' value=\'transparent\' /><param name=\'FlashVars\' value=\'flashID=" + inputId + "\'></param><embed src=\'/cp2flash.swf?x=11\' wmode=\'transparent\' quality=\'high\' bgcolor=\'#ffffff\' width=\'136\' height=\'20\' name=\'ed2kcopy_" + inputId + "\' align=\'middle\' allowScriptAccess=\'always\' allowFullScreen=\'false\' flashVars=\'flashID=" + inputId + "\' type=\'application/x-shockwave-flash\' pluginspage=\'http:\/\/www.macromedia.com\/go\/getflashplayer\' /></object>";
        } else {
            $("updateflash" + inputId + "").innerHTML = "<input type=\"button\" value=\"复制选中的链接\" class=\"button downall\" onclick=\"updateflash(event);\" />";
        }
    }
}

function gen_size(val, li, sepa) {
    sep = Math.pow(10, sepa); //小数点后的位数
    li = Math.pow(10, li); //开始截断的长度
    retval = val;
    unit = 'Bytes';
    if (val >= li * 1000000000) {
        val = Math.round(val / (1099511627776 / sep)) / sep;
        unit = 'TB';
    } else if (val >= li * 1000000) {
        val = Math.round(val / (1073741824 / sep)) / sep;
        unit = 'GB';
    } else if (val >= li * 1000) {
        val = Math.round(val / (1048576 / sep)) / sep;
        unit = 'MB';
    } else if (val >= li) {
        val = Math.round(val / (1024 / sep)) / sep;
        unit = 'KB';
    }
    return val + unit;
}

function popoupNotemsg(msg) {
    try {
        var deltaY;
        if (navigator.appName.match(/microsoft/i)) {
            deltaY = window.pageYOffset 
            || document.documentElement.scrollTop 
            || document.body.scrollTop 
            || 0;
        } else {
            deltaY = 0;
        }
        
        var wtly = $('LayerTop');
        var mask_ht = '';
        mask_ht += '<div id="notemsg" style="display:none;">';
        mask_ht += '  <p>';
        mask_ht += msg;
        mask_ht += '</p>';
        mask_ht += '</div>';
        wtly.innerHTML = mask_ht;
        var wtmx = $('notemsg');
        wtmx.style.top = 50 + deltaY + 'px';
        wtly.style.display = '';
    } catch (e) {
        //alert(msg);
        return false;
    }
    
    setTimeout("new Effect.Appear('notemsg')", 500);
    setTimeout("new Effect.Fade('notemsg')", 8000);
}

// mytrack
function getMyTrack(thisfgroup) {
    var mytrack_str = getCookie('mytrack');
    var mytrack_old = (null != mytrack_str && mytrack_str.isJSON()) ? mytrack_str.evalJSON() : [];
    //    alert(mytrack_str);
    if (mytrack_old.length < 1) {
        mytrack_old = null;
    } else {
        mytrack_old = mytrack_old.reverse();
    }
    
    var mytrack = []; // 我的历史列表
    var myidx = 0; // 我的历史列表索引号
    //    alert(thisfgroup);
    if (null != thisfgroup) {
        if (null != mytrack_old) {
            for (var i = 0; i < mytrack_old.length; i++) {
                if (mytrack_old[i][0] != thisfgroup[0]) {
                    mytrack[myidx] = mytrack_old[i];
                    myidx++;
                } else {
                //                    alert(mytrack_old[i][0] +'!='+ thisfgroup[0]);
                }
            }
        }
        mytrack[myidx] = thisfgroup;
        if (6 < mytrack.length) {
            var tmp = [];
            for (var i = 1; i < mytrack.length; i++) {
                tmp[i - 1] = mytrack[i];
            }
            mytrack = tmp;
        }
        mytrack = mytrack.reverse();
        expr = new Date();
        expr.setFullYear(expr.getFullYear() + 2);
        //        document.write((mytrack.toJSON()));
        VeryCD.cookie.set('mytrack', mytrack.toJSON(), expr, '/groups', '.verycd.com');
        return mytrack;
    } else {
        return (null !== mytrack_old) ? mytrack_old.reverse() : null;
    }
}

function refreshMyTrack(mytrack, icon_size) {
    if ('undefined' == typeof icon_size)
        var icon_size = 48;
    if (null == mytrack)
        return;
    var mytrack_div = $('mytracks');
    var mytrack_ht = '<dl><dt>我最近访问过的小组</dt>\n';
    for (var i = 0; i < mytrack.length; i++) {
        mytrack_ht += '<dd><a href="/groups/@g' + mytrack[i][0] + '/">';
        mytrack_ht += '<img alt="" src="http://img.verycd.com/vcgroup/gface/' + icon_size + '-' + icon_size + '/' + mytrack[i][0] + '.jpg">';
        mytrack_ht += '</a><br /><a href="/groups/@g' + mytrack[i][0] + '/">' + mytrack[i][1] + '</a></dd>\n';
    }
    mytrack_ht += '</dt></dl>';
    mytrack_div.innerHTML = mytrack_ht;
}
function mytrackIndex(mytrack, icon_size) {
    //for(var i in mytrack)alert(mytrack[i]);
    if ('undefined' == typeof icon_size)
        var icon_size = 48;
    if (null == mytrack)
        return;
    var vtrack_div = $('vtracks');
    var mytrack_ht = '<dl><dt style="color: #F3C518;">我最近访问过的小组</dt>\n';
    for (var i = 0; i < mytrack.length; i++) {
        mytrack_ht += '<dd><img alt="" src="http://img.verycd.com/vcgroup/gface/' + icon_size + '-' + icon_size + '/' + mytrack[i][0] + '.jpg"><a href="/groups/@g' + mytrack[i][0] + '/">' + mytrack[i][1] + '</a></dd>\n';
    }
    mytrack_ht += '</dt></dl>';
    vtrack_div.innerHTML = mytrack_ht;
}

function goToSubtitle() {

}

/**
 * add by Cofyc, for display switch
 */
function display_switch(class_name) {
    var objs = document.getElementsByAttribute('class', class_name);
    if (null != objs) {
        for (var i = 0; i < objs.length; i++) {
            if (objs[i].style.display == 'block') {
                objs[i].style.display = 'none';
            } else if (objs[i].style.display == 'none') {
                objs[i].style.display = 'block';
            }
        }
    }
}

/**
* 资源或主题详细内容中点击链接显示图片
**/
function LoadImg(id, url) {
    pageTracker._trackPageview('/stat/autoHiddenImage/');
    document.getElementById(id).innerHTML = '<img src="' + url + '" border="0" alt="IPB Image" name="post_img">';
    return false;
}

/**
* 检查flash版本
**/
function flashChecker() {
    var hasFlash = 0; //是否安装了flash
    var flashVersion = 0; //flash版本
    var isIE =  /*@cc_on!@*/0; //是否IE浏览器
    
    if (isIE) {
        var swf = new ActiveXObject('ShockwaveFlash.ShockwaveFlash');
        if (swf) {
            hasFlash = 1;
            VSwf = swf.GetVariable("$version");
            flashVersion = parseInt(VSwf.split(" ")[1].split(",")[0]);
        }
    } else {
        if (navigator.plugins && navigator.plugins.length > 0) {
            var swf = navigator.plugins["Shockwave Flash"];
            if (swf) {
                hasFlash = 1;
                var words = swf.description.split(" ");
                for (var i = 0; i < words.length; ++i) {
                    if (isNaN(parseInt(words[i])))
                        continue;
                    flashVersion = parseInt(words[i]);
                }
            }
        }
    }
    return {f: hasFlash,v: flashVersion};
}

/**
* 提示升级的js
**/
var flashplayerLink = "";
var myBox;
mypath_to_background_images = "http://v4.vcimg.com/images/glassbox/"
function updateflash(evt) {
    path_to_root_dir = "http://v4.vcimg.com/";
    myBox = new GlassBox();
    myBox.init('myBox', '460px', '170px', 'false', 'facebookstyle', true, false);
    myBox.lbo(false, 0.01);
    myBox.apos(mouseX(evt) + 32 + 'px', mouseY(evt) - 128 + 'px');
}

function closeBox() {
    $("myBox").style.display = "none";
}


/* 下载 flashplayer */

function downloadFlash() {
    if (navigator.appVersion.indexOf("Win") != -1) {
        if (navigator.userAgent.indexOf("MSIE") != -1) {
            flashplayerLink = "http://download.VeryCD.com/flashplayer/flashplayer10_winax.exe";
        } else if (navigator.userAgent.indexOf("Mozilla") != -1 || navigator.userAgent.indexOf("Opera") != -1) {
            flashplayerLink = "http://download.VeryCD.com/flashplayer/flashplayer10_win.exe";
        }
    } else if (navigator.appVersion.indexOf("Mac") != -1) {
        flashplayerLink = "http://download.verycd.com/flashplayer/flashplayer10_mac.dmg.zip";
    } else if (navigator.appVersion.indexOf("Linux") != -1) {
        flashplayerLink = "http://download.VeryCD.com/flashplayer/flashplayer_10_linux.tar.gz";
    } else {
        flashplayerLink = "http://get.adobe.com/flashplayer/";
    }
    window.open(flashplayerLink);
    $("myBox").style.display = "none";
}

/**
* 文本框光标定位
**/
function setCaretPosition(elemId, caretPos) {
    var elem = document.getElementById(elemId);
    if (elem != null && caretPos != 0) {
        if (elem.createTextRange) {
            var range = elem.createTextRange();
            range.move('character', caretPos);
            range.select();
        } 
        else {
            if (elem.selectionStart) {
                elem.setSelectionRange(caretPos, caretPos);
            }
            elem.focus();
        }
        elem.scrollTop = elem.scrollHeight;
    }
}
/* 收藏部分mini登录框的位置  */
function findPosX(obj) 
{
    var curleft = 0;
    if (obj.offsetParent)
        while (1) 
        {
            curleft += obj.offsetLeft;
            if (!obj.offsetParent)
                break;
            obj = obj.offsetParent;
        }
    else if (obj.x)
        curleft += obj.x;
    return curleft;
}

function findPosY(obj) 
{
    var curtop = 0;
    if (obj.offsetParent)
        while (1) 
        {
            curtop += obj.offsetTop;
            if (!obj.offsetParent)
                break;
            obj = obj.offsetParent;
        }
    else if (obj.y)
        curtop += obj.y;
    return curtop;
}

/* 获取鼠标位置 */
function mouseY(evt) {
    var _docHeight = window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight;
    if (evt.pageY) {
        /*
		if(evt.pageY-_docHeight<255){
			return evt.pageY - 140;
		}else{
			return evt.pageY;
		}*/
        return evt.pageY;
    
    } else if (evt.clientY) {
        var _ieY = evt.clientY + (document.documentElement.scrollTop ? 
        document.documentElement.scrollTop : 
        document.body.scrollTop);
        //if(evt.clientY-_docHeight>-30){
        //return _ieY - 140;
        //}else{
        return _ieY;
    //}
    
    } else
        return null;
}
function mouseX(evt) {
    if (evt.pageX)
        return evt.pageX;
    else if (evt.clientX)
        return evt.clientX + (document.documentElement.scrollLeft ? 
        document.documentElement.scrollLeft : 
        document.body.scrollLeft);
    else
        return null;
}


/*  全局弹出框  */
function veryCD_popup(dom, mask, w, ypos) {
    var tbody = document.getElementsByTagName("body")[0];
    tnode = document.createElement("div");
    tnode.id = "IEPromptBox";
    tbody.appendChild(tnode);
    eliteFwindow = $("IEPromptBox");
    tnode = document.createElement("div");
    tnode.id = "promptBlackout";
    tbody.appendChild(tnode);
    eliteBwindow = $("promptBlackout");
    eliteBwindow.style.opacity = ".9";
    eliteBwindow.style.position = "absolute";
    eliteBwindow.style.top = "0px";
    eliteBwindow.style.left = "0px";
    if (mask) {
        eliteBwindow.style.backgroundColor = "#999";
        eliteBwindow.style.filter = "alpha(opacity=90)";
        eliteBwindow.style.height = (document.body.offsetHeight < screen.height) ? screen.height + "px" : document.body.offsetHeight - 120 + "px";
    
    }
    //eliteBwindow.style.backgroundColor = "#999";
    //eliteBwindow.style.filter = "alpha(opacity=90)";
    //eliteBwindow.style.height = (document.body.offsetHeight < screen.height) ? screen.height + "px": document.body.offsetHeight - 120 + "px";
    eliteBwindow.style.display = "block";
    eliteBwindow.style.zIndex = "50";
    //eliteFwindow.style.border = "2px solid #B22222";
    eliteFwindow.style.backgroundColor = "#EFEFEF";
    eliteFwindow.style.position = "absolute";
    if (!w) {
        w = 400
    }
    eliteFwindow.style.width = w + "px";
    eliteFwindow.style.zIndex = "810";
    eliteBwindow.onclick = function() {
        eliteFwindow.style.display = "none";
        eliteBwindow.style.display = "none";
        $("iepromptfield").value = "";
    }
    var tmp = dom;
    //eliteBwindow.style.height = (document.body.offsetHeight < screen.height) ? screen.height + "px": document.body.offsetHeight+ "px";
    eliteBwindow.style.width = "100%";
    eliteBwindow.style.display = "block";
    eliteFwindow.innerHTML = tmp;
    eliteFwindow.style.top = ypos + "px" //parseInt(document.documentElement.scrollTop + (screen.height / 4)) + "px";
    eliteFwindow.style.left = parseInt((document.body.offsetWidth - w - 80) / 2) + "px";
    eliteFwindow.style.display = "block";
    $("iepromptfield").focus();
    return false;
}
function close_popup() {
    eliteFwindow.style.display = "none";
    eliteBwindow.style.display = "none";
    return false;
}
function insertContent(_link, _title) {
    $("page-top").insert('<div id="myBox" class="glassbox" style="display:none"><div id="updateFlash"><span style="font-sie:14px;padding-left:12px;">升级提示:</span></div><span style="margin-left:45px;margin-top:0px;text-align:center;">你的Flash版本过低，不支持使用复制选中的链接功能</span><br /><br /><a href="#" onclick="downloadFlash();return false;">立即下载升级Flash</a><br /><div id="boxBottom"><input type="button" class="button grayButton" id="closeBtn" onclick="closeBox()" value="取消" /></div></div></div>');
}
