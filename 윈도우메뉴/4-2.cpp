#include <Windows.h>
#include <stdio.h>
//#include <stdlib.h>
#include<time.h>
#include"4-2.h"
LPCSTR lpszClass = "Window Class";
HINSTANCE g_hInst;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, "my first window program", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL,
		(HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int startX, startY, oldX, oldY;
	static BOOL Drag;
	static int endX, endY;
	static int mode=0;
	static COLORREF color=RGB(0,0,0);
	static HBRUSH hbr,old;
	static HPEN hpen,oldp;
	static bool border=false;
	switch(iMessage)
	{
	case WM_CREATE:
		endX=startX=oldX=0;
		endY=startY=oldY=0;

		Drag=false;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		Drag=true;
		startX=LOWORD(lParam);
		startY=HIWORD(lParam);
		//endX = LOWORD(lParam);
		//endY = HIWORD(lParam);
		oldX = LOWORD(lParam);
		oldY = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		Drag=false;
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (Drag)
		{ // »Ú πŸ≈¡
			
				SetROP2(hdc, R2_WHITE); // ∆Ê¿« XOR ø¨ªÍ
				//SelectObject(hdc, (HPEN)GetStockObject(BLACK_PEN)); // »Ú ∆Ê
				// »Ú πŸ≈¡ XOR »Ú ∆Ê = ∞À¿∫ªˆ ∆Ê
				endX = LOWORD(lParam);
				endY = HIWORD(lParam);

				//MoveToEx(hdc, startX, startY, NULL);
				//LineTo(hdc, oldX, oldY); // ¡ˆøÏ±‚ : »Ú πŸ≈¡ XOR ∞À¿∫ ∆Ê = »Ú º±

				oldX = endX; oldY = endY; // «ˆ ¡ˆ¡°¿ª ¿Ã†Ü ¡ˆ¡°¿∏∑Œ º≥¡§
		}
		InvalidateRect(hWnd,NULL,1);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		hbr=CreateSolidBrush(color);
		old=(HBRUSH)SelectObject(hdc,hbr);
		if(mode==0)
		{
			if(border){
				hpen=CreatePen(PS_SOLID,4,color);
				oldp=(HPEN)SelectObject(hdc,hpen);
				MoveToEx(hdc, startX, startY, NULL);
				SelectObject(hdc,oldp);
				DeleteObject(hpen);
			}
			else
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY); // ±◊∏Æ±‚ : »Ú πŸ≈¡ XOR »Ú ∆Ê = ∞À¿∫ º±
		}
		else if(mode==1)
		{
			if(border){
				hpen=CreatePen(PS_SOLID,4,color);
				oldp=(HPEN)SelectObject(hdc,hpen);
				Ellipse(hdc,startX,startY,endX,endY);
				SelectObject(hdc,oldp);
				DeleteObject(hpen);
			}
			else
			Ellipse(hdc,startX,startY,endX,endY);
		}
		else if(mode==2)
		{
			if(border){
				hpen=CreatePen(PS_SOLID,4,color);
				oldp=(HPEN)SelectObject(hdc,hpen);
				Rectangle(hdc,startX,startY,endX,endY);
				SelectObject(hdc,oldp);
				DeleteObject(hpen);
			}
			else
			Rectangle(hdc,startX,startY,endX,endY);
		}
		SelectObject(hdc,old);
		DeleteObject(hbr);
		EndPaint(hWnd,&ps);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_COLOR_CYAN:
			color=RGB(0,255,255);
			break;
		case ID_COLOR_YELLOW:
			color=RGB(255,255,0);
			break;
		case ID_COLOR_MAGENTA:
			color=RGB(255,0,255);
			break;
		case ID_COLOR_RANDOM:
			color=RGB(rand()%256,rand()%256,rand()%256);
			break;
		case ID_SHAPE_RECTANGLE:
			mode=2;
			break;
		case ID_SHAPE_LINE:
			mode=0;
			break;
		case ID_SHAPE_ELLIPSE:
			mode=1;
			break;
		case ID_BORDER_ON:
			border=false;
			break;
		case ID_BORDER_OFF:
			border=true;
			break;
		case ID_HELP_HELP:
			MessageBox(hWnd,"«¡∑Œ±◊∑•¿Ã∏ß","HELP",MB_OK);
			break;
		case ID_HELP_EXIT:
			PostQuitMessage(0);
			break;
		}
		break;
	}
	return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}