// Control_Task_Bar.cpp : main source file for Control_Task_Bar.exe
//

#include "stdafx.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HWND hWnd = FindWindow(L"Shell_TrayWnd", NULL);
	CWindow window;
	window.Attach(hWnd);
	DWORD dwStyle = window.GetStyle();
	if(dwStyle & WS_VISIBLE)
	{
		window.ShowWindow(SW_HIDE);
	}
	else
	{
		window.ShowWindow(SW_NORMAL);
	}
	int nRet = 0;
	return nRet;
}
