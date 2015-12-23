#include <Windows.h>
#include <stdio.h>
//#include <stdlib.h>
#include<time.h>
#include<math.h>

BOOL Touch(POINT p1, int r1, POINT p2, int r2);
double LengthPts (int x1, int y1, int x2, int y2)
{
	double a=((x2-x1)*(x2-x1) +(y2-y1)*(y2-y1));
	return(sqrt(a));
}
BOOL InCircle (int x, int y, int mx, int my, int rad)
{
	if(LengthPts(x, y, mx, my) < rad) return TRUE;
	else return FALSE;
}
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
	WndClass.lpszMenuName = NULL;
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
BOOL Touch(POINT p1, int r1, POINT p2, int r2)
{
	if(((p1.x-p2.x)*(p1.x-p2.x))+((p1.y-p2.y)*(p1.y-p2.y))<(r1+r2)*(r1+r2))
		return false;
	else
		return true;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int rad[30];//반지름
	static POINT p[30];//좌표
	static bool s[30];//상태
	static int ex=0,ey=0,mx,my;
	static bool DRAG=false;
	static HBRUSH hbr,old;
	static COLORREF color;
	static POINT eP;
	static bool status=true;
	static int dx=0,dy=0;
	switch(iMessage)
	{
	case WM_CREATE:
		//srand((unsigned)time(NULL));
		int j;
		for(int i=0;i<30;)
		{
			s[i]=false;
			rad[i]=rand()%60+5;
			p[i].x=rand()%500+50;
			p[i].y=rand()%300+50;
			for(j=0;j<i;++j)
			{
				if(!(Touch(p[i],rad[i],p[j],rad[j])))
					break;
			}
			if(j==i)
				++i;
		}
		
		break;
	case WM_LBUTTONDOWN:
		if(status)
		{
			mx=LOWORD(lParam);
			my=HIWORD(lParam);
			if(InCircle(ex,ey,mx,my,50))
			{
				DRAG=TRUE;
			}
		}
		break;
	case WM_LBUTTONUP:
		DRAG=false;
		break;
	case WM_MOUSEMOVE:
		hdc=GetDC(hWnd);
		if(DRAG){
			SetROP2(hdc, R2_WHITE); // 펜의 XOR 연산
			SelectObject(hdc, (HPEN)CreateSolidBrush(RGB(0,0,0)));
			ex=LOWORD(lParam);
			ey=HIWORD(lParam);
			Ellipse(hdc,dx-50,dy-50,dx+50,dy+50);
			//Ellipse(hdc,ex-50,ey-50,ex+50,ey+50);
			dx=ex;
			dy=ey;
		}
		InvalidateRect(hWnd,NULL,0);
		ReleaseDC(hWnd,hdc);
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		if(status)
		{
			eP.x=ex;
			eP.y=ey;
			Ellipse(hdc,ex-50,ey-50,ex+50,ey+50);
			for(int i=0;i<30;++i)
			{
				if(!Touch(eP,50,p[i],rad[i]))
					s[i]=true;
			}
			for(int i=0;i<30;++i)
			{
				if(s[i])
					color=RGB(255,0,0);
				else
					color=RGB(0,255,0);
				hbr=CreateSolidBrush(color);
				old=(HBRUSH)SelectObject(hdc,hbr);
				Ellipse(hdc,p[i].x-rad[i],p[i].y-rad[i],p[i].x+rad[i],p[i].y+rad[i]);
				SelectObject(hdc,old);
				DeleteObject(hbr);
			}
			if(ex>550&&ey>350)
			{
				status=false;
				DRAG=false;
			}
		}
		else
		{
			char str[100];
			int count=0;
			for(int i=0;i<30;++i)
			{
				if(s[i]==true)
					count++;
			}
			Ellipse(hdc,ex-50,ey-50,ex+50,ey+50);
			wsprintf(str,"%d개 공과 충돌했습니다.",count);
			TextOut(hdc,200,200,str,strlen(str));
		}
		EndPaint(hWnd,&ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}