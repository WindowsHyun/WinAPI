#include<windows.h>
#include<stdio.h>
#include <math.h>
#include "resource.h"
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
//LPCTSTR IpszClass=TEXT("연습문제4-1");

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
	//WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
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
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static char Debug[50];
	static RECT rt, isNemo, isTempNemo, isOne;
	static int mx,my, moveLeft=1, moveRight=1, moveTop=-1, moveBottom=-1;
	static int startMove;
	static COLORREF RGB[3];
	static int isOneColor=0, isSpeed =1, isGridOF=0;
	static BOOL isLeftMouse, isRightMouse;

	switch (iMessage){

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_GAME_START:
			SetTimer(hwnd,1,1,NULL);
			break;
		case ID_GAME_END:
			KillTimer(hwnd,1);
			break;
		case ID_COLOR_RED:
			isOneColor=0;
			break;
		case ID_COLOR_BLUE:
			isOneColor=2;
			break;
		case ID_COLOR_GREEN:
			isOneColor=1;
			break;
		case ID_BALL_SMALL:
			isOne.left+=10;
			isOne.right+=20;
			isOne.bottom +=10, isOne.top +=20;
			break;
		case ID_BALL_BIG:
			isOne.left-=10;
			isOne.right-=20;
			isOne.bottom -=10, isOne.top -=20;
			break;
		case ID_SPEED_SLOW:
			SetTimer(hwnd,1,500,NULL);
			break;

		case ID_SPEED_MEDIUM:
			SetTimer(hwnd,1,100,NULL);
			break;

		case ID_SPEED_FAST:
			SetTimer(hwnd,1,1,NULL);
			break;

		case ID_GRID_ON:
			isGridOF=1;
			break;

			case ID_GRID_OFF:
			isGridOF=0;
			break;

		}
		break;
	case WM_CREATE :
		GetClientRect(hwnd,&rt);
		isNemo.left = rt.right/3, isNemo.right = rt.right/3+200;
		isNemo.bottom = rt.bottom-10, isNemo.top = rt.bottom-50;

		isOne.left = 393, isOne.right = 393-50;
		isOne.bottom = 473+50, isOne.top = 473;
		RGB[0] = RGB(255,0,0);
		RGB[1] = RGB(0,255,0);
		RGB[2] = RGB(0,0,255);
		break;

	case WM_TIMER:

		isOne.left +=moveLeft, isOne.right +=moveRight;
		isOne.top +=moveTop, isOne.bottom +=moveBottom;

		if(rt.right-50 <= isOne.right){
			moveLeft=-1, moveRight=-1, moveTop=-1, moveBottom=-1;
		}else if(rt.top == isOne.top){
			if( 1 == startMove){
				startMove=0;
				moveLeft=1, moveRight=1, moveTop=1, moveBottom=1;
			}else{
				startMove=2;
				moveLeft=-1, moveRight=-1, moveTop=1, moveBottom=1;
			}
		}else if(rt.left+50 == isOne.left){

			startMove =1;
			moveLeft=1, moveRight=1, moveTop=-1, moveBottom=-1;
			if(startMove == 2){
				startMove =0;
				moveLeft=-1, moveRight=-1, moveTop=1, moveBottom=1;
			}
		}else if(isNemo.top == isOne.bottom && isNemo.left <= isOne.left){
			moveLeft=-1, moveRight=-1, moveTop=-1, moveBottom=-1;
		}


		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_LBUTTONDOWN :
		isLeftMouse =TRUE;
		break;
	case WM_LBUTTONUP :
		isLeftMouse =FALSE;
		break;

	case WM_RBUTTONDOWN :
		hdc = GetDC(hwnd);

		isTempNemo.left = isNemo.left;
		isTempNemo.right = isNemo.right;

		isNemo.left = rt.left+mx-100;
		isNemo.right = rt.left+100+mx;
		isRightMouse =TRUE;
		InvalidateRgn( hwnd, NULL, TRUE );
		break;
	case WM_RBUTTONUP :
		hdc = GetDC(hwnd);
		isNemo.left = isTempNemo.left;
		isNemo.right = isTempNemo.right;

		isRightMouse =FALSE;
		InvalidateRgn( hwnd, NULL, TRUE );
		break;


	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		mx = LOWORD( lParam );
		my = HIWORD( lParam );


		if(isLeftMouse == TRUE){
			isNemo.left = rt.left+mx-100;
			isNemo.right = rt.left+100+mx;
		}

		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_PAINT :
		hdc = BeginPaint (hwnd, &ps) ;

		if(isGridOF == 1){
			hBrush=CreateHatchBrush(HS_CROSS,RGB(0,0,0));
			oldBrush=(HBRUSH)SelectObject(hdc,hBrush);

			Rectangle(hdc,rt.left, rt.top ,rt.right, rt.bottom );

			SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);
		}


		hBrush = CreateSolidBrush(RGB[isOneColor]);
		oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
		Ellipse(hdc,isOne.left,isOne.top ,isOne.right,isOne.bottom ); //왼쪽, 탑 , 오른쪽, 아래
		Rectangle(hdc,isNemo.left, isNemo.top ,isNemo.right, isNemo.bottom ); //왼쪽, 탑 , 오른쪽, 아래
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);

		

		sprintf(Debug, "mX : %d, mY : %d", mx,my);
		TextOut(hdc, 0,510, Debug, strlen(Debug));

		EndPaint (hwnd, &ps) ;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd,iMessage,wParam,lParam));
}