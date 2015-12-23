#include <Windows.h>
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
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hbrBackground = CreateSolidBrush(RGB(51,181,229));
	WndClass.lpszMenuName=NULL;
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name", "Window Title Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
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
	static RECT rt;
	static bool isJump;
	static int isX=0, isY=0, isJumpData=0;
	int bx,by;
	switch(iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd,&rt);
		//SetTimer(hwnd,1,70,NULL);
		hBit1 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		hBit2 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		break;
	case WM_KEYDOWN :

		if(wParam == VK_RIGHT){
			isX+=10;
			InvalidateRect(hwnd, NULL, FALSE);
		}else if(wParam == VK_LEFT){
			isX-=10;
			InvalidateRect(hwnd, NULL, FALSE);
		}else if(wParam == VK_UP){
			isY-=10;
			InvalidateRect(hwnd, NULL, FALSE);
		}else if(wParam == VK_DOWN){
			isY+=10;
			InvalidateRect(hwnd, NULL, FALSE);
		}else if(wParam == 'j' || wParam == 'J'){
			isJump=TRUE;
			isJumpData=0;
			SetTimer(hwnd, 1, 1, NULL);
		}
		//InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;
	case WM_TIMER:
		switch(wParam){

		case 1:
			if( isJump == TRUE ){
				isY-=1;
				isJumpData++;
				if(isJumpData >= 30){
					SetTimer(hwnd, 2, 1, NULL);
					isJumpData=0;
					KillTimer(hwnd,1);
				}
			}
			break;

		case 2:
			if( isJump == TRUE ){
				isY+=1;
				isJumpData++;
				if(isJumpData >= 30){
					isJumpData=0;
					KillTimer(hwnd,2);
					isJump == FALSE; 
				}
			}
			break;

		}
		InvalidateRgn(hwnd, NULL, FALSE);
		break;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		mem1dc=CreateCompatibleDC(hdc);
		mem2dc=CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBit1);
		SelectObject(mem2dc, hBit2);
		// SRCCOPY : 바탕색을 무시하고 그려라
		bx = rt.right, by = rt.bottom;
		StretchBlt(hdc,0,0, bx, by,mem1dc,0,0,800,500,SRCCOPY);
		TransparentBlt(hdc, isX, isY, 30, 36, mem2dc, 0, 0,30, 36, RGB(255, 0, 0) );
		DeleteDC(mem1dc);
		DeleteDC(mem2dc);
		break;

	case WM_DESTROY:
		KillTimer(hwnd,1);
		PostQuitMessage(0);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}