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
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static char Debug[5000];
	static RECT rt, isNemo, isTempNemo, isOne;
	static int mx,my, moveLeft=1, moveRight=1, moveTop=-1, moveBottom=-1;
	static int startMove;
	static BOOL isLeftMouse, isRightMouse;

	switch (iMessage){
	case WM_CREATE :
		GetClientRect(hwnd,&rt);
		SetTimer(hwnd,1,1,NULL);
		isNemo.left = rt.right/3, isNemo.right = rt.right/3+200;
		isNemo.bottom = rt.bottom-10, isNemo.top = rt.bottom-50;

		isOne.left = 393, isOne.right = 393-50;
		isOne.bottom = 473+50, isOne.top = 473;

		break;

	case WM_TIMER:

		isOne.left +=moveLeft, isOne.right +=moveRight;
		isOne.top +=moveTop, isOne.bottom +=moveBottom;

		if(rt.right-50 <= isOne.right){
			

			if(startMove==3){
			startMove=0;
			moveLeft=-1, moveRight=-1, moveTop=1, moveBottom=1;
			}else{
moveLeft=-1, moveRight=-1, moveTop=-1, moveBottom=-1;
			}


		}else if(rt.top == isOne.top){
			if( 1 == startMove){
				startMove=3;
				moveLeft=1, moveRight=1, moveTop=1, moveBottom=1;
			}else{
				startMove=2;
			moveLeft=-1, moveRight=-1, moveTop=1, moveBottom=1;
			}
		}else if(rt.left+50 == isOne.left){
			
			//startMove =1;
			moveLeft=1, moveRight=1, moveTop=-1, moveBottom=-1;
			if(startMove == 2){
				startMove =1;
				moveLeft=1, moveRight=1, moveTop=1, moveBottom=1;
			//moveLeft=-1, moveRight=-1, moveTop=1, moveBottom=1;
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

		hBrush = CreateSolidBrush(RGB(rand()%255,rand()%255,rand()%255));
		oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
		Ellipse(hdc,isOne.left,isOne.top ,isOne.right,isOne.bottom ); //왼쪽, 탑 , 오른쪽, 아래
		Rectangle(hdc,isNemo.left, isNemo.top ,isNemo.right, isNemo.bottom ); //왼쪽, 탑 , 오른쪽, 아래
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);
		sprintf(Debug, "mX : %d, mY : %d isOne.bottom : %d, isOne.top : %d, isOne.left : %d, isOne.right : %d, startMove : %d", mx,my, isOne.bottom, isOne.top, isOne.left, isOne.right, startMove);
		TextOut(hdc, 0,0, Debug, strlen(Debug));
		EndPaint (hwnd, &ps) ;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd,iMessage,wParam,lParam));
}