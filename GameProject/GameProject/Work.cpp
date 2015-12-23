#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "resource.h"
#define BSIZE 10 // 반지름
#define pieSize 50
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
BOOL Touch(int x1, int y1, int r1, int x2, int y2, int r2)
{
	if(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2))<(r1+r2)*(r1+r2))
		return TRUE;
	else
		return FALSE;
}
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;

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
	static RECT rt;
	static char Debug[500];
	static bool isGridOF, isGameStart, isDrawEat;
	static int isX=0, isY=0, isMouse=0, isMoveD=0, isColor=0, isSpeed = 100, isMouseBig = 50;
	static int isStartEat = 10, isEatX[30] = {0}, isEatY[30] = {0}, isEatColor[30] = {0};
	static COLORREF rgb[4], isEatColorRGB[3]; // 속도, 크기, 입

	switch (iMessage){

	case WM_COMMAND:
		srand((unsigned int)time(NULL));
		switch (LOWORD(wParam)) {
		case ID_GAME_RESTART:
			isX=0, isY=0, isMouseBig = 50, isSpeed=100, isColor=0, isMouse=0, isMoveD=0;
			break;
		case ID_EAT_10:
			isStartEat = 10, isDrawEat = TRUE;
			for(int i=0; i<isStartEat; ++i){
				isEatX[i] = rand()%rt.right-BSIZE, isEatY[i] = rand()%rt.bottom-BSIZE;
				isEatColor[i] = rand()%3+1;
			}
			break;
		case ID_EAT_20:
			isStartEat = 20, isDrawEat = TRUE;
			for(int i=0; i<isStartEat; ++i){
				isEatX[i] = rand()%rt.right-BSIZE, isEatY[i] = rand()%rt.bottom-BSIZE;
				isEatColor[i] = rand()%3+1;
			}
			break;
		case ID_EAT_30:
			isStartEat = 30, isDrawEat = TRUE;
			for(int i=0; i<isStartEat; ++i){
				isEatX[i] = rand()%rt.right-BSIZE, isEatY[i] = rand()%rt.bottom-BSIZE;
				isEatColor[i] = rand()%3+1;
			}
			break;
		case ID_COLOR_CYAN:
			isColor=1;
			break;
		case ID_COLOR_MAGENTA:
			isColor=2;
			break;
		case ID_COLOR_YELLOW:
			isColor=3;
			break;
		case ID_COLOR_RANDOM:
			rgb[4] = RGB(rand()%256,rand()%256,rand()%256);
			isColor=4;
			break;

		case ID_GAME_START:
			isGameStart=TRUE;
			isMoveD = 1;
			SetTimer(hwnd,1,isSpeed,NULL);
			break;
		case ID_GAME_END:
			isGameStart=FALSE;
			KillTimer(hwnd,1);
			break;
		case ID_GRID_ON:
			isGridOF=TRUE;
			break;

		case ID_GRID_OFF:
			isGridOF=FALSE;
			break;

		}
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_CREATE :
		GetClientRect(hwnd,&rt);
		rgb[0] = RGB(255,255,255);
		rgb[1] = RGB(0,255,255);
		rgb[2] = RGB(255,63,195);
		rgb[3] = RGB(255,255,0);
		rgb[4] = RGB(rand()%256,rand()%256,rand()%256);
		isEatColorRGB[0] = RGB(0,0,0);
		isEatColorRGB[1] = RGB(0,0,255); // 속도
		isEatColorRGB[2] = RGB(255,0,0); // 크기
		isEatColorRGB[3] = RGB(0,255,0); // ?
		break;

	case WM_KEYDOWN :
		if(isGameStart==TRUE){
			if(wParam == VK_RIGHT){
				isX+=10,isMouse+=1;
				isMoveD = 1;
			}else if(wParam == VK_LEFT){
				isX-=10,isMouse+=1;
				isMoveD =2;
			}else if(wParam == VK_UP){
				isY-=10,isMouse+=1;
				isMoveD =3;
			}else if(wParam == VK_DOWN){
				isY+=10,isMouse+=1;
				isMoveD =4;
			}
		}
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_TIMER:
		if(isGameStart==TRUE){
			if(isMoveD == 1){
				isX+=10,isMouse+=1;
			}else if(isMoveD == 2){
				isX-=10,isMouse+=1;
			}else if(isMoveD == 3){
				isY-=10,isMouse+=1;
			}else if(isMoveD == 4){
				isY+=10,isMouse+=1;
			}

			if(isX + pieSize-10 >= rt.right){
				isGameStart=FALSE, isDrawEat = FALSE;
				KillTimer(hwnd,1);
			}else if(isX+10  <= rt.left){
				isGameStart=FALSE, isDrawEat = FALSE;
				KillTimer(hwnd,1);
			}else if(isY  +10 <= rt.top){
				isGameStart=FALSE, isDrawEat = FALSE;
				KillTimer(hwnd,1);
			}else if(isY + pieSize - 10 >= rt.bottom){
				isGameStart=FALSE, isDrawEat = FALSE;
				KillTimer(hwnd,1);
			}

			if( isDrawEat == TRUE ){
				for(int i=0; i<isStartEat; ++i){
					if(Touch(isEatX[i], isEatY[i], BSIZE, isX, isY, isMouseBig)){

						if( isEatColor[i] == 1){
							isEatColor[i] = 0, isSpeed -= 20;
							if( isSpeed >= -1){
								KillTimer(hwnd,1);
								SetTimer(hwnd,1,isSpeed,NULL);
							}
						}else if( isEatColor[i] == 2){
							isEatColor[i] = 0, isMouseBig+=10;
						}else if( isEatColor[i] == 3){
							isEatColor[i] = 0, isSpeed += 10, isMouseBig-=5;
						}

					}
				}
				int isTotal =0;
				for(int i=0; i<isStartEat; ++i){
					if(isEatColor[i] == 0)
						++isTotal;
				}
				if(isTotal == isStartEat){
					isGameStart=FALSE, isDrawEat = FALSE;
					KillTimer(hwnd,1);
				}

			}



		}
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_PAINT :
		hdc = BeginPaint (hwnd, &ps) ;

		if(isGridOF==TRUE){
			hBrush=CreateHatchBrush(HS_CROSS,RGB(0,0,0));
			oldBrush=(HBRUSH)SelectObject(hdc,hBrush);
			Rectangle(hdc,rt.left, rt.top ,rt.right, rt.bottom );
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}

		hBrush = CreateSolidBrush(rgb[isColor]);
		oldBrush=(HBRUSH)SelectObject(hdc,hBrush);
		if(isMouse%2 == 0){
			if(isMoveD == 1)
				Pie(hdc, isX, isY, isX+isMouseBig, isY+isMouseBig, isX+100, isY, isX+200, isY+100);
			if(isMoveD == 2)
				Pie(hdc, isX, isY, isX+isMouseBig, isY+isMouseBig, isX-3600, isY+4000, isX-3600, isY-2000);
			if(isMoveD == 3)
				Pie(hdc, isX, isY, isX+isMouseBig, isY+isMouseBig, isX, isY, isX+60, isY-20);
			if(isMoveD == 4)
				Pie(hdc, isX, isY, isX+isMouseBig, isY+isMouseBig, isX+50, isY+50, isX-200, isY+290);
		}else{
			if(isMoveD == 1)
				Pie(hdc, isX, isY, isX+isMouseBig, isY+isMouseBig, isX+3600, isY, isX+3600, isY);
			if(isMoveD == 2)
				Pie(hdc, isX, isY, isX+isMouseBig, isY+isMouseBig, isX-3600, isY, isX-3600, isY);
			if(isMoveD == 3)
				Pie(hdc, isX, isY, isX+isMouseBig, isY+isMouseBig, isX+25, isY, isX+25, isY);
			if(isMoveD == 4)
				Pie(hdc, isX, isY, isX+isMouseBig, isY+isMouseBig, isX-3600, isY+502000, isX-3600, isY+502000);
		}
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);


		if( isDrawEat == TRUE ){
			for(int i=0; i<isStartEat; ++i){

				if(isEatColor[i] != 0){
					hBrush = CreateSolidBrush(isEatColorRGB[isEatColor[i]]);
					oldBrush=(HBRUSH)SelectObject(hdc,hBrush);
					Ellipse(hdc,isEatX[i]-BSIZE, isEatY[i]-BSIZE ,isEatX[i]+BSIZE, isEatY[i]+BSIZE );
					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}
			}
		}


		sprintf(Debug, "GridOn/OFF : %d, isX : %d, isY : %d, isSpeed : %d, rt.right : %d, rt.left : %d", isGridOF, isX, isY, isSpeed, rt.right, rt.left);
		TextOut(hdc, 0,510, Debug, strlen(Debug));

		EndPaint (hwnd, &ps) ;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd,iMessage,wParam,lParam));
}

















