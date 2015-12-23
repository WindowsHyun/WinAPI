#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg6_1Proc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;
static int isStartMove=0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hbrBackground = CreateSolidBrush(RGB(51,181,229));
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name", "Window Title Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	while(GetMessage(&msg, NULL, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	static HINSTANCE hInstance;
	switch(iMsg)
	{
	case WM_CREATE:
		hInstance = hInst;
		DialogBox (hInstance, MAKEINTRESOURCE(IDD_DIALOG1),hwnd, Dlg6_1Proc);
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN :

		break;

	case WM_PAINT :
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK Dlg6_1Proc (HWND hwnd, UINT iMsg,WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInstance;
	static float x, y;
	static char m, n;
	static float r;
	static double re;

	switch(iMsg){

	case WM_CREATE:
		hInstance = hInst;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case Mother_Start: 
			isStartMove=1;
			break;
		case Mother_Stop:
			isStartMove=0;
			break;
		case Plus:
			(float)x=GetDlgItemInt (hwnd, (float)Number1, NULL, TRUE);
			(float)y=GetDlgItemInt (hwnd, (float)Number2, NULL, TRUE);
			
			(float)SetDlgItemInt (hwnd, Result, (float)x+y, TRUE);
			break;
		case Minus:
			x=GetDlgItemInt (hwnd, Number1, NULL, TRUE);
			y=GetDlgItemInt (hwnd, Number2, NULL, TRUE);

			SetDlgItemInt (hwnd, Result, x-y, TRUE);
			break;
		case nanogi:
			x=GetDlgItemInt (hwnd, Number1, NULL, TRUE);
			m=GetDlgItem(hwnd,Number1);

			y=GetDlgItemInt (hwnd, Number2, NULL, TRUE);
			r=x/y;
			SetDlgItemInt (hwnd, Result, r, TRUE);
			break;
		case gobhgi:
			x=GetDlgItemInt (hwnd, Number1, NULL, TRUE);
			y=GetDlgItemInt (hwnd, Number2, NULL, TRUE);
			re=x*y;
			SetDlgItemInt (hwnd, Result, re, TRUE);
			break;
		case EXIT: 
			EndDialog(hwnd,0);
			break;
		}
		break;

	case WM_PAINT :

		break;


	}
	return 0;
}