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
	HDC hdc,mem0dc ,mem1dc, mem2dc, mem3dc;
	static HBITMAP hBit1, hBit2, hBit3, hBit4, hBit5, hBit6, hBit7, hBit8, hBit9, hBit10, hBit11, hBit12, hBit13, hBit14, hBit15;
	static HBITMAP isMoveImage[14], isGirlImage[12], isBoyImage[4];
	PAINTSTRUCT ps ;
	static RECT rt;
	static bool isJump;
	static int isX=0, isY=0, isJumpData=0;
	static int isGX=0, isGY=50;
	static int move=0, isGirlMove=0;
	static int isChangeMode =0;
	static int isBackGroundX =0;
	int bx,by;
	switch(iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd,&rt);
		SetTimer(hwnd,3,100,NULL);
		hBit1 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

		isMoveImage[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		isMoveImage[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
		isMoveImage[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
		isMoveImage[3] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
		isMoveImage[4] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
		isMoveImage[5] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
		isMoveImage[6] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
		isMoveImage[7] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP10));
		isMoveImage[8] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP11));
		isMoveImage[9] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP12));
		isMoveImage[10] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP13));
		isMoveImage[11] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP14));
		isMoveImage[12] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP15));
		isMoveImage[13] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP16));

		isGirlImage[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP17));
		isGirlImage[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP18));
		isGirlImage[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP19));
		isGirlImage[3] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP20));
		isGirlImage[4] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP21));
		isGirlImage[5] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP22));
		isGirlImage[6] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP23));
		isGirlImage[7] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP24));
		isGirlImage[8] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP25));
		isGirlImage[9] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP26));
		isGirlImage[10] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP27));
		isGirlImage[11] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP28));

		isBoyImage[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP29));
		isBoyImage[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP30));
		isBoyImage[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP31));
		isBoyImage[3] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP32));
		break;
	case WM_KEYDOWN :

		if(wParam == VK_RIGHT){
			move++;
			if(isChangeMode ==0 ){
				if(move == 14){
					move = 0;
				}
			}else{
			if(move == 4){
					move = 0;
				}
			}
			isX+=10;
			InvalidateRect(hwnd, NULL, FALSE);
		}else if(wParam == VK_LEFT){
			move++;
			if(isChangeMode ==0 ){
				if(move == 14){
					move = 0;
				}
			}else{
				if(move == 4){
					move = 0;
				}
			}
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
		}else if(wParam == 'd' || wParam == 'D'){
			isGirlMove++;
			if(isGirlMove == 12){
				isGirlMove = 0;
			}
			isGX+=10;
			InvalidateRect(hwnd, NULL, FALSE);
		}else if(wParam == 'a' || wParam == 'A'){
			isGirlMove++;
			if(isGirlMove == 12){
				isGirlMove = 0;
			}
			isGX-=10;
			InvalidateRect(hwnd, NULL, FALSE);
		}else if(wParam == 'w' || wParam == 'W'){
			isGY-=10;
			InvalidateRect(hwnd, NULL, FALSE);
		}else if(wParam == 's' || wParam == 'S'){
			isGY+=10;
			InvalidateRect(hwnd, NULL, FALSE);
		}

		if(wParam == 'c' || wParam == 'C'){
			isChangeMode++;
			move=0;
			if(isChangeMode ==2){
				isChangeMode=0;
			}
			InvalidateRect(hwnd, NULL, TRUE);
			}



		break;
	case WM_TIMER:
		switch(wParam){

		case 1:
			if( isJump == TRUE ){
				isY-=1;
				isGY-=1;
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
				isGY+=1;
				isJumpData++;
				if(isJumpData >= 30){
					isJumpData=0;
					KillTimer(hwnd,2);
					isJump == FALSE; 
				}
			}
			break;

		case 3:
			isBackGroundX++;
			if(isBackGroundX >= rt.right){
				isBackGroundX=0;
			}
			break;

		}
		InvalidateRgn(hwnd, NULL, FALSE);
		break;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		mem1dc=CreateCompatibleDC(hdc);
		mem0dc=CreateCompatibleDC(hdc);
		mem2dc=CreateCompatibleDC(mem1dc);
		mem3dc=CreateCompatibleDC(mem2dc);

		SelectObject(mem0dc, hBit2);
		SelectObject(mem1dc, hBit1);
		if(isChangeMode ==0 ){
		SelectObject(mem2dc, isMoveImage[move]);
		}else{
			SelectObject(mem2dc, isBoyImage[move]);
		}
		SelectObject(mem3dc, isGirlImage[isGirlMove]);

		// SRCCOPY : 바탕색을 무시하고 그려라
		bx = rt.right, by = rt.bottom;
		StretchBlt(hdc,isBackGroundX-bx,0, bx, by,mem1dc,0,0,800,500,SRCCOPY);
		StretchBlt(hdc,isBackGroundX,0, bx, by,mem1dc,0,0,800,500,SRCCOPY);
		if(isChangeMode ==0 ){
		TransparentBlt(hdc, isX, isY, 34, 45, mem2dc, 0, 0,34, 45, RGB(255, 0, 0) );
		}else{
			TransparentBlt(hdc, isX, isY, 44, 45, mem2dc, 0, 0,44, 45, RGB(255, 0, 0) );
		}
		TransparentBlt(hdc, isGX, isGY, 37, 49, mem3dc, 0, 0,37, 39, RGB(255, 0, 0) );
		DeleteDC(mem0dc);
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