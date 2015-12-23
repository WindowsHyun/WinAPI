#include <Windows.h>
#include <stdio.h>
#include <math.h>
#define BSIZE 40	// ¹ÝÁö¸§

float LengthPts(int x1, int y1, int x2, int y2)
{
   return(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))); 
}

BOOL InCircle(int x, int y, int mx, int my)
{
   if(LengthPts(x, y, mx, my) < BSIZE) return TRUE;
   else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	HDC hdc;
	PAINTSTRUCT  ps;
	static RECT cir[2];
	static RECT rt;
	static int speed=10, xDrct[2]={1,1}, yDrct[2]={0,0}, bMove=0;


	switch(iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd,&rt);
		cir[0].left=40;cir[0].top=0;cir[0].right=80;cir[0].bottom=40;
		cir[1].left=0;cir[1].top=0;cir[1].right=40;cir[1].bottom=40;
		SetTimer(hwnd,1,10,NULL);
		return 0;
	case WM_TIMER:
		switch(wParam){
		case 1:
			{
				static int yPos[2]={0,0};
				for(int i=0;i<2;++i){
					if(bMove){
						OffsetRect(&cir[i],speed*xDrct[i],speed*yDrct[i]);
						yPos[i]+=speed*yDrct[i];
						if(cir[i].right>rt.right){
							OffsetRect(&cir[i],rt.right-cir[i].right,yDrct[i]);
							xDrct[i]=0;
							yDrct[i]=1;
						}
						else if(cir[i].left<rt.left){
							OffsetRect(&cir[i],rt.left-cir[i].left,yDrct[i]);
							xDrct[i]=0;
							yDrct[i]=1;
						}
						if(yPos[i]>=40){
							yPos[i]=0;
							yDrct[i]=0;
							if(cir[i].right>=rt.right)
								xDrct[i]=-1;
							else if(cir[i].left<=rt.left)
								xDrct[i]=1;
						}
						InvalidateRect(hwnd,NULL,TRUE);
					}
				}
			}
			break;
		case 2:
			{
				static int yPos[2]={0,0};
				if(bMove){
					for(int i=0;i<2;++i){
						OffsetRect(&cir[i],0,speed*-1);
						yPos[i]+=speed*-1;
					}
					if(yPos[0]<=-40 && yPos[1]<=-40){
						KillTimer(hwnd,2);
						SetTimer(hwnd,3,50,NULL);
						yPos[0]=0;yPos[1]=0;
					}
					InvalidateRect(hwnd,NULL,TRUE);
				}
			}
			break;
		case 3:
			{
				static int yPos[2]={0,0};
				if(bMove){
					for(int i=0;i<2;++i){
						OffsetRect(&cir[i],0,speed);
						yPos[i]+=speed;
					}
					if(yPos[0]>=40 && yPos[1]>=40){
						KillTimer(hwnd,3);
						SetTimer(hwnd,1,10,NULL);
						yPos[0]=0;yPos[1]=0;
					}
					InvalidateRect(hwnd,NULL,TRUE);
				}
			}
			break;
		}
		return 0;
	case WM_KEYDOWN:
		switch(wParam){
		case VK_RETURN:
			bMove=!bMove;
			break;
		case VK_SPACE:
			if(bMove){
				KillTimer(hwnd,1);
				SetTimer(hwnd,2,50,NULL);
			}
			break;
		case VK_RIGHT:
			bMove=0;
			break;

		case VK_LEFT:
			bMove=2;
			break;

		case VK_UP:
			bMove=3;
			break;

		case VK_DOWN:
\
			break;
		}
		return 0;
	case WM_PAINT :
		hdc =BeginPaint(hwnd, &ps);
		for(int i=0;i<2;++i){
			Ellipse(hdc,cir[i].left,cir[i].top,cir[i].right,cir[i].bottom);
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}