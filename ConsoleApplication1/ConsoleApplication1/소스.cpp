#include <Windows.h>
#include <stdio.h>
//#include <stdlib.h>
#include<time.h>
#include"resource.h"
LPCSTR lpszClass = "Window Class";
HINSTANCE g_hInst;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	HACCEL hacc;
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
	hacc=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, "my first window program", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL,
		(HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Message, 0, 0, 0)) {
		if(!TranslateAccelerator(hWnd,hacc,&Message))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc,hMemdc;
	PAINTSTRUCT ps;
	static HBITMAP hbit,oldbit;
	static BITMAP bmp;
	static RECT rt;
	static int line=4;
	//static bool status[4][4];
	static int x=0,y=0;
	static int mx,my,startX,startY,endX,endY;
	static int CopyX,CopyY,wid,hei;
	static bool Drag=false;
	static bool copy=false;
	switch(iMessage)
	{
	case WM_CREATE:
		GetClientRect(hWnd,&rt);
		SetTimer(hWnd,1,500,NULL);
		hbit=(HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,MAKEINTRESOURCE(IDB_BITMAP1));
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hbit);
		StretchBlt(hdc,0,0,rt.right,rt.bottom,hMemdc,0,0,400,280,SRCCOPY);
		DeleteDC(hMemdc);
		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
		//hdc=GetDC(hWnd);
		//SetROP2(hdc,R2_XORPEN);
		//SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
		//MoveToEx(hdc,startX,startY,NULL);
		//LineTo(hdc,startX,my);//지우기
		//MoveToEx(hdc,startX,startY,NULL);
		//LineTo(hdc,mx,startY);//지우기
		//MoveToEx(hdc,mx,startY,NULL);
		//LineTo(hdc,mx,my);//지우기
		//MoveToEx(hdc,startX,my,NULL);
		//LineTo(hdc,mx,my);//지우기
		//DeleteDC(hdc);
		startX=mx=LOWORD(lParam);
		startY=my=HIWORD(lParam);
		if(!copy)
			Drag=true;
		break;
	case WM_LBUTTONUP:
		Drag=false;
		break;
	case WM_MOUSEMOVE:
		hdc=GetDC(hWnd);
		SetROP2(hdc,R2_XORPEN);
		SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
		if(Drag)
		{
			endX=mx;endY=my;
			mx=LOWORD(lParam);
			my=HIWORD(lParam);
			MoveToEx(hdc,startX,startY,NULL);
			LineTo(hdc,startX,endY);//지우기
			MoveToEx(hdc,startX,startY,NULL); //왼쪽
			LineTo(hdc,startX,my);//그리기

			MoveToEx(hdc,startX,startY,NULL);
			LineTo(hdc,endX,startY);//지우기
			MoveToEx(hdc,startX,startY,NULL); //위쪽
			LineTo(hdc,mx,startY);//그리기

			MoveToEx(hdc,endX,startY,NULL);
			LineTo(hdc,endX,endY);//지우기
			MoveToEx(hdc,mx,startY,NULL);    //오른쪽
			LineTo(hdc,mx,my);//그리기

			MoveToEx(hdc,startX,endY,NULL);
			LineTo(hdc,endX,endY);//지우기
			MoveToEx(hdc,startX,my,NULL);    //아래쪽
			LineTo(hdc,mx,my);//그리기
		}
		DeleteDC(hdc);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_MENU_COPY:
			CopyX=startX;
			CopyY=startY;
			wid=mx-startX;
			hei=my-startY;
			hdc=GetDC(hWnd);
			SetROP2(hdc,R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
			MoveToEx(hdc,startX,startY,NULL);
			LineTo(hdc,startX,my);//지우기
			MoveToEx(hdc,startX,startY,NULL);
			LineTo(hdc,mx,startY);//지우기
			MoveToEx(hdc,mx,startY,NULL);
			LineTo(hdc,mx,my);//지우기
			MoveToEx(hdc,startX,my,NULL);
			LineTo(hdc,mx,my);//지우기
			DeleteDC(hdc);
			//InvalidateRect(hWnd,NULL,1);
			copy=true;
			break;
		case ID_MENU_PASTE:
			hdc=GetDC(hWnd);
			hMemdc=CreateCompatibleDC(hdc);
			SelectObject(hMemdc,hbit);
			StretchBlt(hdc,mx,my,wid,hei,hMemdc,CopyX,CopyY,wid,hei,SRCCOPY);
			DeleteDC(hMemdc);
			DeleteDC(hdc);
			copy=false;
			break;
		case ID_MENU_NEW:
			InvalidateRect(hWnd,NULL,true);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}