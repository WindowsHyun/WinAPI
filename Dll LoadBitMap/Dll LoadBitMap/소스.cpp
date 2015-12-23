#include <windows.h>
#include "resource.h"

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
	HDC hdc, mem1dc, mem2dc, mem3dc;
	static HBITMAP hBit1, hBit2, hBit3, oldBit1, oldBit2, hBitmap;
	PAINTSTRUCT ps ;
	HMODULE hRes = NULL;
	static RECT rt;
	static bool isJump[3];
	static int num = 0;
	static int isX=0, isY=0, isJumpData=0;
	int bx,by;
	switch(iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd,&rt);
		hRes = LoadLibrary("DllMake.dll"); // hRes NULL이 아니면 성공
		hBit1 = LoadBitmap (hRes, MAKEINTRESOURCE(IDB_BITMAP1));
		hBit2 = LoadBitmap (hRes, MAKEINTRESOURCE(IDB_BITMAP2));
		hBit3 = LoadBitmap (hRes, MAKEINTRESOURCE(IDB_BITMAP3));
		//hBit1 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		//hBit2 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		break;
	case WM_LBUTTONDOWN:
		if(num >= 3){
			isJump[0] =FALSE;
			isJump[1] =FALSE;
			isJump[2] =FALSE;
			num =0;
		}
		isJump[num] =TRUE;
		num++;

		InvalidateRgn(hwnd, NULL, TRUE);
		break;


	case WM_KEYDOWN :

		//InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);



		mem1dc=CreateCompatibleDC(hdc);
		mem2dc=CreateCompatibleDC(hdc);
		mem3dc=CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBit1);
		SelectObject(mem2dc, hBit2);
		SelectObject(mem3dc, hBit3);
		// SRCCOPY : 바탕색을 무시하고 그려라
		bx = rt.right, by = rt.bottom;
		if( isJump[0] == TRUE){
			StretchBlt(hdc,0,0, 66, 76,mem1dc,0,0,66,76,SRCCOPY);
		} 
		if( isJump[1] == TRUE){
			StretchBlt(hdc,0,80, 72, 70,mem2dc,0,0,72,70,SRCCOPY);
		}
		if( isJump[2] == TRUE){
			StretchBlt(hdc,0,160, 62, 76,mem3dc,0,0,62,76,SRCCOPY);
		}
		DeleteDC(mem1dc);
		DeleteDC(mem2dc);
		DeleteDC(mem3dc);
		break;

	case WM_DESTROY:
		KillTimer(hwnd,1);
		PostQuitMessage(0);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}