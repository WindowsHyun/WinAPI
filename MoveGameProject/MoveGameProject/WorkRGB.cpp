#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>



BOOL InEatCheck(int x, int xX, int y, int yY, int mx, int my){
	if((x <= mx && xX >= mx) && (y <= my && yY >= my)){
		return TRUE;
	}else{ 
		return FALSE;
	}
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
	static int rnadom, drawNemo = 90, mx, my;
	static RECT rt[4];
	static char Debug[500], Debug2[5000];
	static int isEatData[16], isClick[16]={0}, tempData;
	static int isEatDatax[16], isEatDataxX[16], isEatDatay[16], isEatDatayY[16];
	static int ColorR[16], ColorG[16], ColorB[16];
	static COLORREF isEatColor[16], tempColor; // xor 연산 공부하자..!
	HBRUSH hBrush, oldBrush;
	switch(iMsg)
	{
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		for(int i=0; i<16;++i){ 
			isEatData[i] = rand()%3;
			isEatColor[i] = RGB(rand()%256,rand()%256,rand()%256);
			ColorR[i] = rand()%256, ColorG[i] = rand()%256, ColorB[i] = rand()%256;
		}
		SetTimer(hwnd, 1, 1000, NULL);
		break;

	case WM_TIMER:
		for(int i=0; i<16; ++i){
			if( i == 0 ){
				tempData = isEatData[0], tempColor = isEatColor[0];
			}
			if( i == 15 ){
				isEatData[15] = tempData, isEatColor[15] = tempColor;
			}else{
				isEatData[i] = isEatData[i+1];
				isEatColor[i] = isEatColor[i+1];
			}
		}

		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;

	case WM_LBUTTONDOWN :
		mx = LOWORD( lParam );
		my = HIWORD( lParam );

		for(int i=0; i<16; ++i){
			if( InEatCheck(isEatDatax[i], isEatDataxX[i], isEatDatay[i],isEatDatayY[i], mx,  my) == TRUE){
				//isEatColor[i] = RGB(isEatColor[i]);
			}

		}
		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;

	case WM_CHAR :
		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;

	case WM_PAINT :
		hdc =BeginPaint(hwnd, &ps);

		hBrush = CreateSolidBrush(RGB(255,255,255));
		oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
		for(int j=0 ; j<5; ++j){
			for(int i=0; i<5; ++i){
				Rectangle(hdc,50+(i*drawNemo),50+(j*drawNemo) ,140+(i*drawNemo), 140+(j*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
			}
		}
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);
		// 직선 -----
		for(int i=0; i<5; ++i){
			if(isEatData[i] == 0){
				if(isClick[i] == 0){
					hBrush = CreateSolidBrush(isEatColor[i]);
				}else{
					hBrush = CreateSolidBrush(RGB(0,0,0));
				}
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,70+(i*drawNemo),70 ,120+(i*drawNemo), 120 ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i] = 70+(i*drawNemo), isEatDataxX[i] = 120+(i*drawNemo);
				isEatDatay[i] = 70, isEatDatayY[i] = 120;
			}else if(isEatData[i] == 1){
				hBrush = CreateSolidBrush(isEatColor[i]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Ellipse(hdc,70+(i*drawNemo),70 ,120+(i*drawNemo), 120 ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i] = 70+(i*drawNemo), isEatDataxX[i] = 120+(i*drawNemo);
				isEatDatay[i] = 70, isEatDatayY[i] = 120;
			}else if(isEatData[i] == 2){
				hBrush = CreateSolidBrush(isEatColor[i]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,80+(i*drawNemo),70 ,110+(i*drawNemo), 120 ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i] = 80+(i*drawNemo), isEatDataxX[i] = 110+(i*drawNemo);
				isEatDatay[i] = 70, isEatDatayY[i] = 120;
			}
		}
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);
		// 세로 -*4
		for(int i=1; i<5; ++i){
			if(isEatData[i+4] == 0){
				hBrush = CreateSolidBrush(isEatColor[i+4]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,70+(360),70+(i*drawNemo) ,120+(360), 120+(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+4] = 70+(360), isEatDataxX[i+4] = 120+(360);
				isEatDatay[i+4] = 70+(i*drawNemo), isEatDatayY[i+4] = 120+(i*drawNemo);
			}else if(isEatData[i+4] == 1){
				hBrush = CreateSolidBrush(isEatColor[i+4]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Ellipse(hdc,70+(360),70+(i*drawNemo) ,120+(360), 120+(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+4] = 70+(360), isEatDataxX[i+4] = 120+(360);
				isEatDatay[i+4] = 70+(i*drawNemo), isEatDatayY[i+4] = 120+(i*drawNemo);
			}else if(isEatData[i+4] == 2){
				hBrush = CreateSolidBrush(isEatColor[i+4]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,80+(360),70+(i*drawNemo) ,110+(360), 120+(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+4] = 80+(360), isEatDataxX[i+4] = 110+(360);
				isEatDatay[i+4] = 70+(i*drawNemo), isEatDatayY[i+4] = 120+(i*drawNemo);
			}
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}

		for(int i=1; i<5; ++i){
			if(isEatData[i+8] == 0){
				hBrush = CreateSolidBrush(isEatColor[i+8]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,70+(360)-(i*drawNemo),70+(360) ,120+(360)-(i*drawNemo), 120+(360) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+8] = 70+(360)-(i*drawNemo), isEatDataxX[i+8] = 120+(360)-(i*drawNemo);
				isEatDatay[i+8] = 70+(360), isEatDatayY[i+8] = 120+(360);
			}else if(isEatData[i+8] == 1){
				hBrush = CreateSolidBrush(isEatColor[i+8]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Ellipse(hdc,70+(360)-(i*drawNemo),70+(360) ,120+(360)-(i*drawNemo), 120+(360) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+8] = 70+(360)-(i*drawNemo), isEatDataxX[i+8] = 120+(360)-(i*drawNemo);
				isEatDatay[i+8] = 70+(360), isEatDatayY[i+8] = 120+(360);
			}else if(isEatData[i+8] == 2){
				hBrush = CreateSolidBrush(isEatColor[i+8]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,80+(360)-(i*drawNemo),70+(360) ,110+(360)-(i*drawNemo), 120+(360) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+8] = 80+(360)-(i*drawNemo), isEatDataxX[i+8] = 110+(360)-(i*drawNemo);
				isEatDatay[i+8] = 70+(360), isEatDatayY[i+8] = 120+(360);
			}
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}

		for(int i=1; i<4; ++i){
			if(isEatData[i+12] == 0){
				hBrush = CreateSolidBrush(isEatColor[i+12]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,70+(360)-(360),70+(360)-(i*drawNemo) ,120+(360)-(360), 120+(360)-(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+12] = 70+(360)-(360),70+(360)-(i*drawNemo), isEatDataxX[i+12] = 120+(360)-(360);
				isEatDatay[i+12] = 70+(360)-(i*drawNemo), isEatDatayY[i+12] = 120+(360)-(i*drawNemo);
			}else if(isEatData[i+12] == 1){
				hBrush = CreateSolidBrush(isEatColor[i+12]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Ellipse(hdc,70+(360)-(360),70+(360)-(i*drawNemo) ,120+(360)-(360), 120+(360)-(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+12] = 70+(360)-(360),70+(360)-(i*drawNemo), isEatDataxX[i+12] = 120+(360)-(360);
				isEatDatay[i+12] = 70+(360)-(i*drawNemo), isEatDatayY[i+12] = 120+(360)-(i*drawNemo);
			}else if(isEatData[i+12] == 2){
				hBrush = CreateSolidBrush(isEatColor[i+12]);
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,80+(360)-(360),70+(360)-(i*drawNemo) ,110+(360)-(360), 120+(360)-(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+12] = 80+(360)-(360),70+(360)-(i*drawNemo), isEatDataxX[i+12] = 110+(360)-(360);
				isEatDatay[i+12] = 70+(360)-(i*drawNemo), isEatDatayY[i+12] = 120+(360)-(i*drawNemo);
			}
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}

		sprintf(Debug, "mx : %d, my : %d", mx, my);
		TextOut(hdc, 0,510, Debug, strlen(Debug));

		for(int i=0; i<16; ++i){
			sprintf(Debug2, "%d", isEatDatax[i]);
			TextOut(hdc, 531,65+(i*20), Debug2, strlen(Debug2));
		}
		for(int i=0; i<16; ++i){
			sprintf(Debug2, "%d", isEatDataxX[i]);
			TextOut(hdc, 562,65+(i*20), Debug2, strlen(Debug2));
		}
		for(int i=0; i<16; ++i){
			sprintf(Debug2, "%d", isEatDatay[i]);
			TextOut(hdc, 592,65+(i*20), Debug2, strlen(Debug2));
		}
		for(int i=0; i<16; ++i){
			sprintf(Debug2, "%d", isEatDatayY[i]);
			TextOut(hdc, 622,65+(i*20), Debug2, strlen(Debug2));
		}

		EndPaint(hwnd, &ps);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}



/*
하나씩 움직이게 할때 수동..!
tempData = isEatData[0], tempColor = isEatColor[0];
isEatData[0] = isEatData[1], isEatColor[0] = isEatColor[1];
isEatData[1] = isEatData[2], isEatColor[1] = isEatColor[2];
isEatData[2] = isEatData[3], isEatColor[2] = isEatColor[3];
isEatData[3] = isEatData[4], isEatColor[3] = isEatColor[4];
isEatData[4] = isEatData[5], isEatColor[4] = isEatColor[5];
isEatData[5] = isEatData[6], isEatColor[5] = isEatColor[6];
isEatData[6] = isEatData[7], isEatColor[6] = isEatColor[7];

isEatData[7] = isEatData[8], isEatColor[7] = isEatColor[8];
isEatData[8] = isEatData[9], isEatColor[8] = isEatColor[9];
isEatData[9] = isEatData[10], isEatColor[9] = isEatColor[10];

isEatData[10] = isEatData[11], isEatColor[10] = isEatColor[11];
isEatData[11] = isEatData[12], isEatColor[11] = isEatColor[12];
isEatData[12] = isEatData[13], isEatColor[12] = isEatColor[13];

isEatData[13] = isEatData[14], isEatColor[13] = isEatColor[14];
isEatData[14] = isEatData[15], isEatColor[14] = isEatColor[15];
isEatData[15] = tempData, isEatColor[15] = tempColor;
*/