#include <Windows.h>
#include <stdio.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	HACCEL hAcc;
	MSG msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//WndClass.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name", "Window Title Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 700, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	//hAcc=LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	//while (GetMessage (&msg, NULL, 0, 0))
	//{
	//	if(!TranslateAccelerator(hwnd,hAcc,&msg)) // 단축키 -> 메뉴 ID로 인식
	//	{
	//		TranslateMessage (&msg) ;
	//		DispatchMessage (&msg) ;
	//	}
	//}

	while(GetMessage(&msg, NULL, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	HDC hdc, mem1dc, mem2dc;
	static HBITMAP hBit1, hBit2, oldBit1, oldBit2;
	PAINTSTRUCT ps ;
	static RECT rt, isNemo[5][5], isTempNemo;
	static bool isJump;
	HPEN hPen, oldPen;
	static int isX=0, isY=0, isLineData=3, isNemoData=0;
	static char debug[500];
	int bx,by, isRealX, isRealY;
	int tempbx, tempby;
	static int mx, my;
	switch(iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd,&rt);
		hBit1 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		for(int i=0; i<5; ++i){
			for(int j=0; j<5;++j){
				isNemo[i][j].right = 0, isNemo[i][j].left = 0, isNemo[i][j].bottom = 0, isNemo[i][j].top =0;
			}
		}
		/*isRealX=rt.right/2;
		isRealY=rt.bottom;
		for(int i=0; i<isLineData; ++i){
		for(int j=0; j<isLineData; ++j){
		isNemo[isNemoData].left=isRealX/isLineData*i, isNemo[isNemoData].right=isRealX/isLineData*(i+1);
		isNemo[isNemoData].top=isRealY/isLineData*j, isNemo[isNemoData].bottom=isRealY/isLineData*(j+1);
		isNemoData++;
		}
		}
		wsprintf(debug,"top : %d, left : %d, right : %d, bottom : %d",isNemo[1].top,isNemo[1].left,isNemo[1].right,isNemo[1].bottom );
		MessageBox(hwnd,debug,"",CB_OKAY);*/
		break;
	case WM_KEYDOWN :

		break;
	case WM_TIMER:

		break;

	case WM_LBUTTONDOWN:
		mx=LOWORD(lParam);
		my=HIWORD(lParam);

		isRealX=rt.right/2;
		isRealY=rt.bottom;

		for(int i=0; i<isLineData; ++i){
			for(int j=0; j<isLineData; ++j){
				if(isRealX/isLineData*i<= mx && isRealY/isLineData*j<= my && isRealX/isLineData*(i+1) >= mx && isRealY/isLineData*(j+1) >= my ){

					/*isNemo[isNemoData].left=isRealX/isLineData*i, isNemo[isNemoData].right=isRealX/isLineData*(i+1);
					isNemo[isNemoData].top=isRealY/isLineData*j, isNemo[isNemoData].bottom=isRealY/isLineData*(j+1);*/
					isTempNemo.left =isRealX/isLineData*i, isTempNemo.right=isRealX/isLineData*(i+1);
					isTempNemo.top =isRealY/isLineData*j, isTempNemo.bottom =isRealY/isLineData*(j+1);

					wsprintf(debug,"현재위치 : (%d, %d)\n\nleft : %d, right : %d\ntop : %d, bottom : %d",i+1,j+1,isRealX/isLineData*i,isRealX/isLineData*(i+1),isRealY/isLineData*j,isRealY/isLineData*(j+1));
					MessageBox(hwnd,debug,"",CB_OKAY);
					break;
				}

			}
		}
		break;

	case WM_RBUTTONDOWN:
		mx=LOWORD(lParam);
		my=HIWORD(lParam);

		isRealX=rt.right/2;
		isRealY=rt.bottom;

		for(int i=isLineData; i<isLineData*2; ++i){
			for(int j=0; j<isLineData*2; ++j){
				if(isRealX/isLineData*i<= mx && isRealY/isLineData*j<= my && isRealX/isLineData*(i+1) >= mx && isRealY/isLineData*(j+1) >= my ){

					/*isTempNemo.left =isRealX/isLineData*(i+isLineData),isTempNemo.right=isRealX/isLineData*(i+1+isLineData);
					isTempNemo.top =isRealY/isLineData*j, isTempNemo.bottom =isRealY/isLineData*(j+1);*/

					isNemo[i-isLineData][j].left =isTempNemo.left, isNemo[i-isLineData][j].right =isTempNemo.right;
					isNemo[i-isLineData][j].top =isTempNemo.top, isNemo[i-isLineData][j].bottom =isTempNemo.bottom;

					wsprintf(debug,"현재위치 : (%d, %d)\n\nleft : %d, right : %d\ntop : %d, bottom : %d",i+1,j+1,isRealX/isLineData*i, isRealX/isLineData*(i+1), isRealY/isLineData*j, isRealY/isLineData*(j+1));
					MessageBox(hwnd,debug,"",CB_OKAY);
					break;
				}

			}
		}
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc=CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBit1);
		// SRCCOPY : 바탕색을 무시하고 그려라
		bx = rt.right, by = rt.bottom;
		StretchBlt(hdc,0,0, bx/2, by,mem1dc,0,0,800,500,SRCCOPY);

		isRealX=rt.right/2;
		isRealY=rt.bottom;

		for(int i=0; i<isLineData; ++i){
			for(int j=0; j<isLineData; ++j){

				if(isNemo[i][j].left == 0 && isNemo[i][j].right == 0 && isNemo[i][j].top ==0 && isNemo[i][j].bottom == 0){

				}else{

					//isTempNemo.left =isRealX/isLineData*i, isTempNemo.top =isRealY/isLineData*j;
					/*wsprintf(debug,"left : %d",isRealX/isLineData*(isLineData-i));
					MessageBox(hwnd,debug,"",CB_OKAY);*/
					//StretchBlt(hdc,0,0, bx/2, by,mem1dc,0,0,800,500,SRCCOPY);
					//bx = (rt.right/isGetNum), by = (rt.bottom/isGetNum);
					/*tempbx=abs(isNemo[i][j].right-isNemo[i][j].left);
					tempby=abs(isNemo[i][j].bottom-isNemo[i][j].top);*/
					tempbx=isRealX/isLineData;
					tempby=isRealY/isLineData;
					StretchBlt(hdc, isRealX/isLineData*(i+isLineData), isRealY/isLineData*j, tempbx, tempby, mem1dc, isNemo[i][j].left*1.7, isNemo[i][j].top/1.3, 800/isLineData, 500/isLineData, SRCCOPY);
				}
			}
		}
		
		DeleteDC(mem1dc);


		hPen = CreatePen(PS_DOT, 2, RGB(81,222,92));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		for(int i=1; i<(isLineData*2); ++i){
			MoveToEx(hdc, rt.right/(isLineData*2)*i, rt.top,NULL);
			LineTo(hdc, rt.right/(isLineData*2)*i,rt.bottom);
		}
		for(int i=0; i<=isLineData; ++i){
			if(i ==0){
				MoveToEx(hdc, rt.left,0,NULL);
				LineTo(hdc, rt.right,0);
			}else{
				MoveToEx(hdc, rt.left,rt.bottom/(isLineData)*i,NULL);
				LineTo(hdc, rt.right,rt.bottom/(isLineData)*i);
			}
		}
		SelectObject(hdc,oldPen);
		DeleteObject(hPen);
		/*wsprintf(debug,"left : %d, top : %d, right : %d, bottom : %d",rt.left, rt.top, rt.right, rt.bottom);
		MessageBox(hwnd,debug,"",CB_OKAY);*/
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_Menu:
			isLineData=3;
			for(int i=0; i<5; ++i){
			for(int j=0; j<5;++j){
				isNemo[i][j].right = 0, isNemo[i][j].left = 0, isNemo[i][j].bottom = 0, isNemo[i][j].top =0;
			}
		}
			InvalidateRgn( hwnd, NULL, TRUE );
			break;
		case ID_MENU_4:
			isLineData=4;
			for(int i=0; i<5; ++i){
			for(int j=0; j<5;++j){
				isNemo[i][j].right = 0, isNemo[i][j].left = 0, isNemo[i][j].bottom = 0, isNemo[i][j].top =0;
			}
		}
			InvalidateRgn( hwnd, NULL, TRUE );
			break;
		case ID_MENU_5:
			isLineData=5;
			for(int i=0; i<5; ++i){
			for(int j=0; j<5;++j){
				isNemo[i][j].right = 0, isNemo[i][j].left = 0, isNemo[i][j].bottom = 0, isNemo[i][j].top =0;
			}
		}
			InvalidateRgn( hwnd, NULL, TRUE );
			break;
		case ID_MENU_NEW:
			for(int i=0; i<5; ++i){
			for(int j=0; j<5;++j){
				isNemo[i][j].right = 0, isNemo[i][j].left = 0, isNemo[i][j].bottom = 0, isNemo[i][j].top =0;
			}
		}
			InvalidateRgn( hwnd, NULL, TRUE );
			break;
		}
		break;

	case WM_DESTROY:
		KillTimer(hwnd,1);
		PostQuitMessage(0);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}