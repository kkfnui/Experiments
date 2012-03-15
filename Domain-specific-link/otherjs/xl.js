/*
	thunder network
*/
var xl = 
{
    CreateMenu: function(url) 
    {    		
  		var self = this;
        var li = document.createElement("li");
      	li.id = "downlaodbythunder";
        li.appendChild(document.createTextNode("使用迅雷下载"));
        li.xl_url = url;
        li.xl_index = 0;
        xl_chrome_menu.appendChild(li);
    	self.RegisterMenuEvent(li, url);
		
        li = document.createElement("li");
      	li.id = "downlaodbythunder";
        li.appendChild(document.createTextNode("在新标签中打开链接"));
        li.xl_url = url;
        li.xl_index = 1;
        xl_chrome_menu.appendChild(li); 
		self.RegisterMenuEvent(li, url);
	},
    RegisterMenuEvent: function (ele)
    {
        addEvent(ele, "mouseup", function (e) 
			{
			  if (e.button == 0) 
			  {
				if( this.xl_index == 0 )
				{
					chrome.extension.sendRequest({link: this.xl_url, index:0});
				}
				else if( this.xl_index == 1 )
				{
					chrome.extension.sendRequest({link: this.xl_url, index:1});
				}
				HideXLMenu();
			  }
			}
        );
        addEvent(ele, "mouseover", function (e) 
			{
			  if (e.button == 0) 
			  {
				e.srcElement.style.backgroundColor = "#ffff00";
			  }
			}
        );
        addEvent(ele, "mouseout", function (e) 
			{
			  if (e.button == 0) 
			  {
				e.srcElement.style.backgroundColor = "#D4D0C8";
			  }
			}
        );
	}
};

//资源列表
var ExtList = new Object();
ExtList["rar"] = 1;
ExtList["zip"] = 1;
ExtList["tar"] = 1;
ExtList["iso"] = 1;
ExtList["exe"] = 1;
ExtList["dll"] = 1;
ExtList["rmvb"] = 1;
ExtList["rm"] = 1;
ExtList["avi"] = 1;
ExtList["mp3"] = 1;
ExtList["wma"] = 1;
ExtList["wmv"] = 1;
ExtList["wmp"] = 1;
ExtList["mpg"] = 1;
ExtList["mpeg"] = 1;
ExtList["mov"] = 1;
ExtList["3gp"] = 1;
ExtList["mdf"] = 1;
ExtList["chm"] = 1;
ExtList["torrent"] = 1;

//资源类型扩展名
var expArray = ["http\\:\\/\\/.+\\/.+\\.(.+)",
				"ftp\\:\\/\\/.+\\/.+\\.(.+)",
				"https\\:\\/\\/.+\\/.+\\.(.+)",
				"thunder\\:\\/\\/.+",
				"ed2k\\:\\/\\/.+"
				];
var expSina = "http\\:\\/\\/.*sina.com.+\\/d_load\\.php\\?";
var expPConline = "http\\:\\/\\/.*pconline.com.+\\/filedown\\.jsp\\?";
var expZol = "http\\:\\/\\/.*zol.com.+\\/down\\.php\\?";
var expCrsky = "http\\:\\/\\/.*\\/.+\\?down_url=(.+\\:\\/\\/.+)";
var expRefix = "http\\:\\/\\/.*\\/.+\\?uri=(.+\\:\\/\\/.+)";
var expGougou= "http\\:\\/\\/.*gougou.+\\/.+&url=(.+)";
var expReToArray = ["http\\:\\/\\/.+\\/.+\\?uri=(.+\\:\\/\\/.+)",
					"http\\:\\/\\/.+\\/.+\\?uri=(.+\\:\\/\\/.+)"
					];

// 创建独立ID的计数器
addEvent.guid = 1;

/*
	隐藏菜单
*/
function HideXLMenu() 
{
	var child = document.getElementById("downlaodbythunder");
	xl_chrome_menu.removeChild(child);
    xl_chrome_menu.style.display = "none";
}

function GetScroll()
{
    var de = document.documentElement,
    y = self.pageYOffset || (de && de.scrollTop) || document.body.scrollTop,
    x = self.pageXOffset || (de && de.scrollLeft) || document.body.scrollLeft;
    return { "left": x, "top": y };
}

/*
	显示自定义菜单
*/
function ShowXLMenu(elem, e)
{
	var scroll = GetScroll();
    var lf = document.documentElement.clientWidth - e.clientX,
		tp = document.documentElement.clientHeight - e.clientY,
		dstop = scroll.top,
		dsleft = scroll.left;


    var setPosition = function() 
    {
        if (lf < elem.offsetWidth)
        {
            elem.style.left = dsleft + e.clientX  + "px";
            elem.style.left = dsleft + e.clientX  + elem.offsetWidth + "px";
        } 
        else
        {
            elem.style.left = elem.style.left = dsleft + e.clientX + "px";
        }
        if (tp < elem.offsetHeight) 
        {
            elem.style.top = dstop + e.clientY + "px";
            elem.style.top = dstop + e.clientY - elem.offsetHeight + "px";
        }
        else 
        {
        	//alert(elem.offsetWidth);alert(elem.offsetHeight);
            elem.style.top = elem.style.top = dstop + e.clientY + "px";
        }
        elem.style.display = "block";
    };
	
	setPosition();
    elem.style.display = "block";
    e.preventDefault();
}

/*
	添加事件
*/
function addEvent(element, type, handler) 
{
	if (element.addEventListener)
	{
		element.addEventListener(type, handler, false);
	} 
	else 
	{
		//为每一个事件处理函数赋予一个独立的ID
		//alert(handler.$$guid);
		if (!handler.$$guid) 
			handler.$$guid = addEvent.guid++;
		
		// 为元素建立一个事件类型的散列表
		if (!element.events) 
			element.events = {};
		
		// 为每对元素/事件建立一个事件处理函数的哈希表
		var handlers = element.events[type];
		if (!handlers)
		{
			handlers = element.events[type] = {};
			
			// 储存已有的事件处理函数(如果已存在一个)
			if (element["on" + type])
			{
				handlers[0] = element["on" + type];
			}
		}
		
		// 在哈希表中储存该事件处理函数
		handlers[handler.$$guid] = handler;
		
		// 赋予一个全局事件处理函数来处理所有工作
		element["on" + type] = handleEvent;
	}
};

/*
	右键响应函数
*/
function onContextMenu(e) 
{
	if( e.ctrlKey)
	{
		//xl.CreateMenu(this.href);
		//ShowXLMenu(xl_chrome_menu,e);
	}
}

/*
	链接左键点击响应函数
*/
function onLinkClick(e) 
{
	var theURL = PreParseURL(this.href);
	
	if( theURL != "undownload" )
	{
		chrome.extension.sendRequest({ name: "xl", link: theURL},function(resp) 
		{
			//alert(resp.res);
		}
		);
		e.preventDefault();
	}
}
/*
	document左键点击响应函数
*/
function onDocClick(e) 
{
		HideXLMenu();
}

/*
	对URL进行分类识别
*/
function PreParseURL(url)
{
		//alert(url);
		var bValidate = false;
		
		//gougou
		var re = new RegExp(expGougou, "i");
		reArray = re.exec(url);
		if( reArray != null && reArray.length == 2 )
		{
			return decodeURIComponent(reArray[1]);
		}
				
		//新浪
		re = new RegExp(expSina, "i");
		var reArray = re.exec(url);
		if( reArray != null && reArray.length == 1)
		{
			return url;
		}
		//太平洋
		var re = new RegExp(expPConline, "i");
		var reArray = re.exec(url);
		if( reArray != null && reArray.length == 1)
		{
			return url;
		}
		//中关村在线
		re = new RegExp(expZol, "i");
		reArray = re.exec(url);
		if( reArray != null && reArray.length == 1)
		{
			return url;
		}
		
		//非凡
		re = new RegExp(expCrsky, "i");
		reArray = re.exec(url);
		if( reArray != null && reArray.length == 2 )
		{
			return url;
		}
				
		//天空
		re = new RegExp(expRefix, "i");
		reArray = re.exec(url);
		if( reArray != null && reArray.length == 2 )
		{
			//alert(reArray[1]);
			bValidate = IsValidateURL(reArray[1]);
			if( bValidate = true )
			{
				return reArray[1];
			}
		}
		
		//alert(url);
		bValidate = IsValidateURL(url)
		if( bValidate == true )
		{
			//alert(url);
			return url;
		}
		else
		{
			//alert("undownload");
			return "undownload"
		}
}

/*
	检查URL是否为链接地址
*/
function IsValidateURL(url)
{

	for( var i in expArray )
	{
		var re = new RegExp(expArray[i], "i");
		var reArray = re.exec(url);
		
		if( reArray != null )
		{
			//alert(reArray[1]);
			if( reArray.length == 1 || (reArray.length == 2 && ExtList[reArray[1].toLowerCase()] == 1) )
			{
				return true;
			}
		}
	}
	return false;
}

/*
	添加document事件
*/
document.addEventListener("click", onDocClick, false);

/*
	对链接添加点击与右键事件。
*/
for( var i = 0; i <document.links.length; i++ )
{
	var link = document.links[i];
	link.addEventListener("click", onLinkClick, false);
}