#include "StdAfx.h"
#include <WinUser.h>
#include "MouseWheelFilter.h"

CMouseWheelFilter::CMouseWheelFilter(void)
{
}

CMouseWheelFilter::~CMouseWheelFilter(void)
{
}

BOOL CMouseWheelFilter::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_MOUSEWHEEL)  
	{  
		POINT pos;   
		GetCursorPos (&pos);   
		pMsg->hwnd = WindowFromPoint (pos);   
		pMsg->message = WM_MOUSEWHEEL;
	}  
	return FALSE;
}
