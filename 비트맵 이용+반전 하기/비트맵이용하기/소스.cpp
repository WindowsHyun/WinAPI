#include <Windows.h>
#include "resource.h"
#include "resource1.h"

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
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hbrBackground = CreateSolidBrush(RGB(51,181,229));
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name", "Window Title Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	hAcc=LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage (&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(hwnd,hAcc,&msg)) // 단축키 -> 메뉴 ID로 인식
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
	}

	while(GetMessage(&msg, NULL, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	HDC hdc, memdc ;
	PAINTSTRUCT ps ;
	static RECT rt, isNemo;
	int bx,by, sx,sy;
	static int mx, my;
	static int startX, startY,endX, endY, oldX, oldY;
	static bool Drag, Draw, PasteComplete;
	static int isSx=0, isSy=0;
	static HMENU hMenu, hSubMenu;
	static int isNum = 0, isGetNum=3;
	static HBITMAP hBitmap;
	static HBRUSH hbr,old;
	static HPEN hpen,oldp;
	switch(iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd,&rt);
		hBitmap = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		hMenu = GetMenu(hwnd);
		hSubMenu = GetSubMenu(hMenu, 1);
		EnableMenuItem(hMenu, ID_REVERSE_STOP, MF_GRAYED);
		break;
	case WM_LBUTTONUP:
		Drag=FALSE;
		Draw=TRUE;
		startX=LOWORD(lParam);
		startY=HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		Drag=TRUE;
		Draw=FALSE;
		startX=LOWORD(lParam);
		startY=HIWORD(lParam);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);

		if (Drag){
			SetROP2(hdc, R2_WHITE); // 펜의 XOR 연산
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			oldX = endX; oldY = endY; // 현 지점을 이젂 지점으로 설정
			InvalidateRect(hwnd,NULL,1);
		ReleaseDC(hwnd, hdc);
		}
		
		break;
	case WM_TIMER:
		hdc=GetDC(hwnd);

		//hdc = BeginPaint(hwnd, &ps);
		if( -1 == isNum){
			memdc=CreateCompatibleDC(hdc);
			SelectObject(memdc, hBitmap);
			// SRCCOPY : 바탕색을 무시하고 그려라
			bx = rt.right, by = rt.bottom;
			StretchBlt(hdc,0,0, bx, by,memdc,0,0,400,280,SRCCOPY);
			DeleteDC(memdc);
		}

		for(int i=0; i<isGetNum; ++i){
			for(int j=0; j<isGetNum; ++j){
				if(isSx == i && isSy == j){
					memdc=CreateCompatibleDC(hdc);
					SelectObject(memdc, hBitmap);
					bx = (rt.right/isGetNum), by = (rt.bottom/isGetNum);
					sx = rt.right/isGetNum*i, sy = rt.bottom/isGetNum*j;
					StretchBlt(hdc,sx,sy, bx, by,memdc,100*i,70*j,100,70,NOTSRCCOPY);
					//BitBlt(hdc,sx,sy, bx, by,memdc,0,0,NOTSRCCOPY);
					DeleteDC(memdc);
				}
			}
		}
		isSy += 1;
		if(isSy >= 4){
			isSx+=1, isSy=0;
		}
		if(isSx >= 4){
			isSx=0, isSy=0;
		}
		EndPaint(hwnd, &ps);
		InvalidateRgn( hwnd, NULL, FALSE );
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

			if( 0 == isNum){
				memdc=CreateCompatibleDC(hdc);
				SelectObject(memdc, hBitmap);
				// SRCCOPY : 바탕색을 무시하고 그려라
				bx = rt.right, by = rt.bottom;
				StretchBlt(hdc,0,0, bx, by,memdc,0,0,400,280,SRCCOPY);
				DeleteDC(memdc);

			}else if( 1 == isNum){

				for(int i=0; i<isGetNum; ++i){
					for(int j=0; j<isGetNum; ++j){
						memdc=CreateCompatibleDC(hdc);
						SelectObject(memdc, hBitmap);
						bx = (rt.right/isGetNum), by = (rt.bottom/isGetNum);
						sx = rt.right/isGetNum*i, sy = rt.bottom/isGetNum*j;
						StretchBlt(hdc,sx,sy, bx, by,memdc,0,0,400,280,SRCCOPY);
						DeleteDC(memdc);

					}
				}
			}
		
			if(PasteComplete){
				memdc=CreateCompatibleDC(hdc);
				SelectObject(memdc, hBitmap);
				// SRCCOPY : 바탕색을 무시하고 그려라
				bx = rt.right, by = rt.bottom;
				StretchBlt(hdc,0,0, endX-startX, endY-startY,memdc, 50,50,400,280,SRCCOPY);
				DeleteDC(memdc);
			}
		hbr=CreateHatchBrush(HS_DIAGCROSS,RGB(0,0,0));
		old=(HBRUSH)SelectObject(hdc,hbr);
		isNemo.left = startX, isNemo.right = endX;

		//FrameRect(hdc,startX, startY ,endX, endY );
		SelectObject(hdc,old);
		DeleteObject(hbr);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_SCREEN_FULLSCREEN:
			isNum = 0 ;
			InvalidateRgn( hwnd, NULL, TRUE );
			break;
		case ID_SCREEN_3:
			isNum = 1 ;
			isGetNum=3;
			InvalidateRgn( hwnd, NULL, TRUE );
			break;
		case ID_SCREEN_4:
			isNum = 1;
			isGetNum=4;
			InvalidateRgn( hwnd, NULL, TRUE );
			break;
		case ID_SCREEN_5:
			isNum = 1;
			isGetNum=5;
			InvalidateRgn( hwnd, NULL, TRUE );
			break;
		case ID_REVERSE_START:
			hMenu = GetMenu(hwnd);
			hSubMenu = GetSubMenu(hMenu, 1);
			isNum = -1;
			SetTimer(hwnd, 1, 1000, NULL);
			isGetNum=4;
			EnableMenuItem(hMenu, ID_REVERSE_START, MF_GRAYED);
			EnableMenuItem(hMenu, ID_SCREEN_FULLSCREEN, MF_GRAYED);
			EnableMenuItem(hMenu, ID_SCREEN_3, MF_GRAYED);
			EnableMenuItem(hMenu, ID_SCREEN_4, MF_GRAYED);
			EnableMenuItem(hMenu, ID_SCREEN_5, MF_GRAYED);
			EnableMenuItem(hMenu, ID_REVERSE_STOP, MF_ENABLED);
			break;
		case ID_REVERSE_STOP:
			isNum = 0;
			KillTimer(	hwnd,1);
			isGetNum=3;
			EnableMenuItem(hMenu, ID_REVERSE_START, MF_ENABLED);
			EnableMenuItem(hMenu, ID_REVERSE_STOP, MF_GRAYED);
			EnableMenuItem(hMenu, ID_SCREEN_FULLSCREEN, MF_ENABLED);
			EnableMenuItem(hMenu, ID_SCREEN_3, MF_ENABLED);
			EnableMenuItem(hMenu, ID_SCREEN_4, MF_ENABLED);
			EnableMenuItem(hMenu, ID_SCREEN_5, MF_ENABLED);
			break;
		case ID_COPYPASTE_COPY:
			MessageBox(hwnd,"복사준비..!","Help",MB_OK);
			break;
		case ID_COPYPASTE_PASTE:
			PasteComplete = TRUE;
			MessageBox(hwnd,"복사완료..!","Help",MB_OK);
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