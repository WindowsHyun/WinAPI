#include <Windows.h>
#include <math.h>
#include "resource.h"
#define BSIZE 20 // ¹ÝÁö¸§
double LengthPts (int x1, int y1, int x2, int y2)
{
	double a=((x2-x1)*(x2-x1) +(y2-y1)*(y2-y1));
	return(sqrt(a));
}
BOOL InCircle (int x, int y, int mx, int my)
{
	if(LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}
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
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static HINSTANCE hInstance;
	static int x, y;
	static float r;
	static double re;
	static double ex=20,ey=20,mx,my;
	static BOOL Drag, Move;
	static int startX, startY, oldX, oldY;
	static double vx,vy;
	static int endX=0, endY=0;
	static int count=0, Radio=1;
	static int px1=0,py1=0,px2=0,py2=0;
	switch(iMsg){

	case WM_CREATE:
		hInstance = hInst;
		break;
	case WM_LBUTTONDOWN : // ¹öºjÀ» ´©¸£¸é µå·¡±× µ¿ÀÛ ½ÃÀÛ
		mx=LOWORD(lParam);
		my=HIWORD(lParam);
		if(InCircle(ex,ey,mx,my))
		{
			Drag = TRUE;
			startX=ex;
			startY=ey;

		}
		break;
	case WM_LBUTTONUP :
		if(Drag){
			Drag = FALSE;
			vx=(endX-ex)/33.000;
			vy=(endY-ey)/33.000;
			count=0;
		}
		break;

	case WM_MOUSEMOVE:
		hdc = BeginPaint (hwnd, &ps) ;
		if (Drag)
		{ // Èò ¹ÙÅÁ
			//SetROP2(hdc, R2_XORPEN); // ÆæÀÇ XOR ¿¬»ê
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN)); // Èò Ææ
			// Èò ¹ÙÅÁ XOR Èò Ææ = °ËÀº»ö Ææ
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY); // Áö¿ì±â : Èò ¹ÙÅÁ XOR °ËÀº Ææ = Èò ¼±
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY); // ±×¸®±â : Èò ¹ÙÅÁ XOR Èò Ææ = °ËÀº ¼±
			oldX = endX; oldY = endY; // Çö ÁöÁ¡À» ÀÌ † ÁöÁ¡À¸·Î ¼³Á¤
			px1=startX;px2=endX;
			py1=startY;py2=endY;
		}
		InvalidateRgn( hwnd, NULL, TRUE );
		ReleaseDC(hwnd, hdc);
		break;

	case WM_TIMER:
		if(count>=33)
			KillTimer(hwnd, 1);
		ex+=vx;
		ey+=vy;
		++count;

		InvalidateRect(hwnd,NULL,1);
		break;


	case WM_PAINT :
		hdc = BeginPaint (hwnd, &ps) ;


		hBrush=CreateSolidBrush(RGB(0,255,0));
		oldBrush=(HBRUSH)SelectObject(hdc,hBrush);
		if( Radio == 1){
			Ellipse(hdc,ex-20,ey-20,ex+20,ey+20);
		}else{
			Rectangle(hdc,ex-20,ey-20,ex+20,ey+20);
		}
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);

		MoveToEx(hdc, px1, py1, NULL); // ÀÌµ¿ÇÏ°í ¼±À¸·Î ¿¬°á
		LineTo(hdc, px2, py2);

		EndPaint (hwnd, &ps) ;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDC_RADIO1:
			Radio = 0;
			break;

		case IDC_RADIO2:
			Radio = 1;
			break;

		case IDC_BUTTON3:
			SetTimer(hwnd,1,100,NULL);
			break;

		case EXIT: 
			EndDialog(hwnd,0);
			break;
		}
		break;


	}
	return 0;
}