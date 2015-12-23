#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"

double LengthPts(int x1, int y1, int x2, int y2){
	return(sqrt((float)((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))));
}

bool InCircle( int x, int y, int mx, int my, int Size){
	if( LengthPts(x,y,mx,my) <Size) return TRUE;
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	hwnd;
	MSG 		msg;
	WNDCLASS	WndClass; 

	WndClass.style 	= CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc	= WndProc;		
	WndClass.cbClsExtra	= 0;		
	WndClass.cbWndExtra	= 0;		
	WndClass.hInstance 	= hInstance;		
	WndClass.hIcon	= LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor 	= LoadCursor(NULL, IDC_ARROW);	
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	
	WndClass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);		
	WndClass.lpszClassName 	= "Window Class Name";	
	RegisterClass(&WndClass);	

	hwnd = CreateWindow(	"Window Class Name", "Window Programming", WS_OVERLAPPEDWINDOW,
		100, 100, 800, 600, NULL, NULL,	hInstance, NULL );
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	
		DispatchMessage(&msg);	
	}   
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps; 
	static RECT rt, isCar[4], isMan[2], isRGB[3];
	static char Debug[1000];
	static int mx, my, isStartRGB = 0, isStartTimer =0;
	static COLORREF RGB[3];
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	switch(iMsg){
	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		isCar[0].bottom = 407, isCar[0].left = 8, isCar[0].right = 447, isCar[0].top = 60;
		isCar[1].bottom = 457, isCar[1].left = 8, isCar[1].right = 497, isCar[1].top = 60;
		isCar[2].bottom = 600, isCar[2].left = 470, isCar[2].right = 640, isCar[2].top = 532;
		isCar[3].bottom = 650, isCar[3].left = 470, isCar[3].right = 690, isCar[3].top = 532;

		isMan[0].bottom = 367, isMan[0].left = 239, isMan[0].right = 391, isMan[0].top = 261;
		isMan[1].bottom = 709, isMan[1].left = 299, isMan[1].right = 733, isMan[1].top = 321;

		isRGB[0].bottom = 47, isRGB[0].left = 44, isRGB[0].right = 111, isRGB[0].top = 105;
		isRGB[1].bottom = 47, isRGB[1].left = 114, isRGB[1].right = 111, isRGB[1].top = 175;
		isRGB[2].bottom = 47, isRGB[2].left = 184, isRGB[2].right = 111, isRGB[2].top = 245;

		RGB[0] = RGB(150,0,0);
		RGB[1] = RGB(150,150,0);
		RGB[2] = RGB(0,0,150);
		break;

	case WM_TIMER:
		switch(wParam){

		case 1:
			for(int i=0; i<2; ++i){
				isCar[i].left +=10, isCar[i].top += 10;
				if( isCar[i].top >= rt.bottom){
					isCar[i].left = 8, isCar[i].top = 60;
				}
			}

			if( isStartRGB == 2){
				//x: 411  y: 221
				if( isCar[0].top >= 221 ){
					if(isCar[0].top <= 240){
						KillTimer(hwnd,1);
					}else{
					}
				}
			}
			break;

		case 2:
			for(int i=2; i<4; ++i){
				isCar[i].left -=20, isCar[i].top -= 20;
				if( isCar[i].left <= rt.top){
					isCar[i].left = 470, isCar[i].top = 532;
				}
			}

			//x: 609  y: 335

			if( isStartRGB == 2){
				//x: 411  y: 221
				if( isCar[2].left <= 330){
					if(isCar[2].left >= 320){
						KillTimer(hwnd,2);
					}else{
					}
				}
			}
			break;

		case 3:
			if(isStartTimer == 4){
				isStartTimer=0;
				isStartRGB=2;
				RGB[0] = RGB(150,0,0);
				RGB[1] = RGB(150,150,0);
				RGB[2] = RGB(0,0,255);
				SetTimer(hwnd, 4, 70, NULL);
				SetTimer(hwnd, 5, 70, NULL);
			}
			isStartTimer+=1;
			break;

		case 4:
			//isMan[0].bottom = 367, isMan[0].left = 239, isMan[0].right = 391, isMan[0].top = 261;
			//isMan[1].bottom = 709, isMan[1].left = 299, isMan[1].right = 733, isMan[1].top = 321;
			if( isStartRGB == 2){
				isMan[0].bottom +=10, isMan[0].right +=10;
			}

			if(isMan[0].bottom >= 709){
				isMan[0].bottom = 367, isMan[0].left = 239, isMan[0].right = 391, isMan[0].top = 261;
			}
			break;

		case 5:
			if( isStartRGB == 2){
				isMan[1].bottom -=10, isMan[1].right -=10;
			}

			if(isMan[1].bottom <= 367){
				isMan[1].bottom = 709, isMan[1].left = 299, isMan[1].right = 733, isMan[1].top = 321;
			}
			break;

		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		for(int i=0; i<3; ++i){
			if(InCircle(isRGB[i].bottom,isRGB[i].left, mx, my, 40)){
				if(i ==0){
					RGB[0] = RGB(255,0,0);
					RGB[1] = RGB(150,150,0);
					RGB[2] = RGB(0,0,150);
					isStartRGB=0;
					SetTimer(hwnd, 1, 70, NULL);
					SetTimer(hwnd, 2, 70, NULL);
					isMan[0].bottom = 367, isMan[0].left = 239, isMan[0].right = 391, isMan[0].top = 261;
					isMan[1].bottom = 709, isMan[1].left = 299, isMan[1].right = 733, isMan[1].top = 321;
				}
				if(i ==1){
					RGB[0] = RGB(150,0,0);
					RGB[1] = RGB(255,255,0);
					RGB[2] = RGB(0,0,150);
					isStartRGB=1;
					SetTimer(hwnd, 3, 1000, NULL);
				}
				if(i ==2){
					RGB[0] = RGB(150,0,0);
					RGB[1] = RGB(150,150,0);
					RGB[2] = RGB(0,0,255);
					isStartRGB=2;
					SetTimer(hwnd, 4, 70, NULL);
					SetTimer(hwnd, 5, 70, NULL);
				}
			}
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;


	case WM_MOUSEMOVE:
		/*
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		InvalidateRgn(hwnd, NULL, TRUE);
		*/
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		Rectangle(hdc, 402, rt.top, 705,rt.bottom);

		for(int i=0; i<3; ++i){
			hBrush = CreateSolidBrush(RGB[i]);
			oldBrush = (HBRUSH) SelectObject(hdc,hBrush);
			Ellipse(hdc, isRGB[i].bottom, isRGB[i].left, isRGB[i].right,isRGB[i].top);
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}


		for(int i =0; i<4; ++i){
			Rectangle(hdc, isCar[i].bottom ,isCar[i].left, isCar[i].right,isCar[i].top);
		}

		//구분선
		hPen = CreatePen(PS_DOT, 4, RGB(255,255,0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		MoveToEx(hdc, 508, rt.top,NULL);
		LineTo(hdc, 508,rt.bottom);
		MoveToEx(hdc, 590, rt.top,NULL);
		LineTo(hdc, 590,rt.bottom);
		SelectObject(hdc,oldPen);
		DeleteObject(hPen);

		// 횡단보도
		hPen = CreatePen(PS_DOT, 2, RGB(255,255,0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		MoveToEx(hdc, 402, 232,NULL);
		LineTo(hdc, 700,232);
		MoveToEx(hdc, 401, 332,NULL);
		LineTo(hdc, 700,332);
		SelectObject(hdc,oldPen);
		DeleteObject(hPen);

		for(int i=0; i<2; ++i){
			Ellipse(hdc, isMan[i].bottom, isMan[i].left,isMan[i].right,isMan[i].top);
		}

		sprintf(Debug, "x : %d, y : %d", mx, my);
		TextOut(hdc, 0, 510, Debug, strlen(Debug));
		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case ID_MENU_START:
			SetTimer(hwnd, 1, 70, NULL);
			SetTimer(hwnd, 2, 70, NULL);
			RGB[0] = RGB(255,0,0);
			RGB[1] = RGB(150,150,0);
			RGB[2] = RGB(0,0,150);
			isStartRGB=0;
			break;
		case ID_MENU_STOP:
			KillTimer(hwnd,1);
			KillTimer(hwnd,2);
			KillTimer(hwnd,3);
			KillTimer(hwnd,4);
			KillTimer(hwnd,5);
			break;
		case ID_MENU_EXIT:
			PostQuitMessage(0);
			break;
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}