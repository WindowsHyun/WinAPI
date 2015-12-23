#include<windows.h>
#include<stdio.h>
#include <math.h>
#define BSIZE 20 // 반지름
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
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR IpszClass=TEXT("연습문제3-1");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hbrBackground = CreateSolidBrush(RGB(255,255,255));
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static char Debug[50];
	static int count=0;
	static int startX, startY, oldX, oldY;
	static double ex=20,ey=20,mx,my;
	static BOOL Drag, Move;
	static int endX=0, endY=0;
	static double vx,vy;
	static int px1=0,py1=0,px2=0,py2=0;
	switch (iMessage)
	{
	case WM_CREATE :
		startX =0; oldX = 20; startY =0; oldY = 20; // 시작 좌표
		Drag = FALSE;
		return 0 ;

	case WM_LBUTTONDOWN : // 버튺을 누르면 드래그 동작 시작
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
			SetTimer(hWnd,1,10,NULL);
			count=0;
		}
		break;
	case WM_TIMER:
		if(count>=33)
			KillTimer(hWnd, 1);
		ex+=vx;
		ey+=vy;
		++count;

		InvalidateRect(hWnd,NULL,1);
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (Drag)
		{ // 흰 바탕
			//SetROP2(hdc, R2_XORPEN); // 펜의 XOR 연산
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN)); // 흰 펜
			// 흰 바탕 XOR 흰 펜 = 검은색 펜
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY); // 지우기 : 흰 바탕 XOR 검은 펜 = 흰 선
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY); // 그리기 : 흰 바탕 XOR 흰 펜 = 검은 선
			oldX = endX; oldY = endY; // 현 지점을 이젂 지점으로 설정
			px1=startX;px2=endX;
			py1=startY;py2=endY;
		}
		InvalidateRgn( hWnd, NULL, TRUE );
		ReleaseDC(hWnd, hdc);
		break;

	case WM_PAINT :
		hdc = BeginPaint (hWnd, &ps) ;

		hBrush=CreateSolidBrush(RGB(0,255,0));
		oldBrush=(HBRUSH)SelectObject(hdc,hBrush);
		Ellipse(hdc,ex-20,ey-20,ex+20,ey+20);
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);

		MoveToEx(hdc, px1, py1, NULL); // 이동하고 선으로 연결
		LineTo(hdc, px2, py2);

		sprintf(Debug, "ex : %f, eY : %f, count : %d", ex,ey,count);
		TextOut(hdc, 0,510, Debug, strlen(Debug));

		EndPaint (hWnd, &ps) ;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}