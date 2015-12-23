#include <Windows.h>
#include <stdio.h>
//#include <stdlib.h>
#include<time.h>
#include"4-1.h"

LPCSTR lpszClass = "Window Class";
HINSTANCE g_hInst;
WNDCLASS WndClass;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
bool touch_r(int rx,int ry,int mx, int my);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU2);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, "my first window program", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL,
		(HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
static int rad=20;
bool touch_r(int rx,int ry, int mx, int my)
{
	if(mx>=rx&&mx<=rx+150&&my>=ry&&my<=ry+30)
		return true;
	else
		return false;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int ex=150,ey=480;//원 좌표
	static int rx=100,ry=500;//사각형 좌표
	static int status=0;//원의 이동방향 0=오른쪽위 1=왼쪽위 2=오른쪽아래 3=왼쪽아래
	static int mx,my,dx,dr;
	static bool Drag=false;
	static HBRUSH hbr,old;
	static COLORREF color=RGB(255,0,0);
	static COLORREF color2=RGB(255,255,255);
	static int ddx;
	static int v=50;
	static bool game=false;
	static int base;
	static RECT ec;
	static RECT rc;
	switch(iMessage)
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_GAME_ON:
			ex=150;ey=480;rx=100;ry=500;status=0;v=50;
			game=true;
			SetTimer(hWnd,1,v,NULL);
			break;
		case ID_GAME_OFF:
			KillTimer(hWnd,1);
			game=false;
			break;
		case ID_SPEED_FAST:
			if(game)
			{
				v=1;
				KillTimer(hWnd,1);
				SetTimer(hWnd,1,v,NULL);
			}
			break;
		case ID_SPEED_MEDIUM:
			if(game)
			{
				v=20;
				KillTimer(hWnd,1);
				SetTimer(hWnd,1,v,NULL);
			}
			break;
		case ID_SPEED_SLOW:
			if(game)
			{
				v=50;
				KillTimer(hWnd,1);
				SetTimer(hWnd,1,v,NULL);
			}
			break;
		case ID_BALL_BIG:
			rad=30;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_BALL_SMALL:
			rad=20;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_COLOR_RED:
			color=RGB(255,0,0);
			break;
		case ID_COLOR_BLUE:
			color=RGB(0,0,255);
			break;
		case ID_COLOR_GREEN:
			color=RGB(0,255,0);
			break;
		case ID_GRID_ON:
			color2=RGB(0,0,0);
			InvalidateRect(hWnd,NULL,0);
			break;
		case ID_GRID_OFF:
			color2=RGB(255,255,255);
			InvalidateRect(hWnd,NULL,0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if(game)
		{
		mx=LOWORD(lParam);
		my=HIWORD(lParam);
		dx=mx;dr=rx;
		if(touch_r(rx,ry,mx,my))
			Drag=true;
		}
		break;
	case WM_LBUTTONUP:
		if(game)
		{
		Drag=false;
		}
		break;
	case WM_RBUTTONDOWN:
		if(game)
		{
		mx=LOWORD(lParam);
		my=HIWORD(lParam);
		dx=mx;dr=rx;
		if(touch_r(rx,ry,mx,my))
			Drag=true;
		}
		break;
	case WM_RBUTTONUP:
		if(game)
		{
		Drag=false;
		rx=dr;
		}
		break;
	case WM_CHAR:
		if(game)
		{
		if(wParam=='-')
		{
			++v;
			KillTimer(hWnd,1);
			SetTimer(hWnd,1,v,NULL);
		}
		if(wParam=='+')
		{
			--v;
			KillTimer(hWnd,1);
			SetTimer(hWnd,1,v,NULL);
		}
		}
		break;
	case WM_MOUSEMOVE:
		if(game)
		{
			if(Drag)
			{
				rx=LOWORD(lParam)-(dx-dr);
			}
			rc.top=ry-10;
			rc.left=0;
			rc.right=800;
			rc.bottom=ry+30+10;
			InvalidateRect(hWnd,&rc,0);
		}
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		hbr=CreateHatchBrush(HS_CROSS,color2);
		old=(HBRUSH)SelectObject(hdc,hbr);
		Rectangle(hdc,-1,-1,801,601);
		SelectObject(hdc,old);
		DeleteObject(hbr);
		hbr=CreateSolidBrush(color);
		old=(HBRUSH)SelectObject(hdc,hbr);
		Ellipse(hdc,ex-rad,ey-rad,ex+rad,ey+rad);	
		SelectObject(hdc,old);
		DeleteObject(hbr);
		Rectangle(hdc,rx,ry,rx+150,ry+30);

		if(ey>576){
			KillTimer(hWnd,1);
			TextOut(hdc,400,300,"Game Over",9);
			game=false;
		}
		EndPaint(hWnd,&ps);
		break;
	case WM_TIMER:
		if(status==0)//오른쪽 위
		{
			if(ex>778&&ey>20)
			{
				status=1;
			}
			else if(ex>20&&ey<20)
			{
				status=2;
			}
			else
			{
				ex+=2+rand()%3;
				ey-=4+rand()%3;
			}

		}
		else if(status==1)//왼쪽 위
		{
			if(ex<20&&ey>20)
			{
				status=0;
			}
			else if(ex>20&&ey<20)
			{
				status=3;
			}
			else
			{
				ex-=2+rand()%2;
				ey-=4+rand()%3;
			}
		}


		else if(status==2)//오른쪽 아래
		{
			if(touch_r(rx,ry,ex,ey+rad))
			{
				status=0;
			}
			else
			{
				if(ex<778&&ey>576)
				{
					status=0;
				}
				else if(ex>778&&ey<580)
				{
					status=3;
				}
				else
				{
					ex+=4+rand()%3;
					ey+=2+rand()%2;
				}
			}
		}
		else if(status==3)//왼쪽 아래
		{
			if(touch_r(rx,ry,ex,ey+rad))
			{
				status=1;
			}
			else
			{
				if(ex<778&&ey>576)
				{
					status=1;
				}
				else if(ex<21&&ey>21)
				{
					status=2;
				}
				else
				{
					ex-=2+rand()%4;
					ey+=4+rand()%2;
				}
			}
		}
		ec.top=ey-rad-10;
		ec.left=ex-rad-10;
		ec.bottom=ey+rad+10;
		ec.right=ex+rad+10;
		InvalidateRect(hWnd,&ec,1);
		break;
	}
	return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}