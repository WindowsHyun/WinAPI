#include <Windows.h>
#include <stdio.h>

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
	static int isLeft = 20, isRight = 40;
	static int isTop = 20, isBottom = 40;
	static BOOL GoStop;
	static int TLRB = 3;
	HBRUSH hBrush, oldBrush;
	static char Debug[50];

	switch(iMsg)
	{
	case WM_CREATE:

		break;
	case WM_KEYDOWN :
		if( wParam == VK_RETURN){
			if(GoStop==FALSE){
				SetTimer(hwnd,1,40,NULL);
				GoStop=TRUE;
			}else{
				KillTimer(hwnd,1);
				GoStop=FALSE;
			}
		}

		if(wParam == VK_RIGHT){
			TLRB =3;
		}else if(wParam == VK_LEFT){
			TLRB =2;
		}else if(wParam == VK_UP){
			TLRB =1;
		}else if(wParam == VK_DOWN){
			TLRB =4;
		}


		break;
	case WM_TIMER:

		if(isLeft <= 0 ){
			isLeft=10;
			isRight=30;
		}
		if(isLeft >= 760 ){
			isLeft=760;
			isRight=780;
		}
		if(isTop <= 0 ){
			isTop=10;
			isBottom=30;
		}
		if(isTop >= 545 ){
			isTop=545;
			isBottom=565;
		}
		if(TLRB ==3){
			isLeft+=10;
			isRight+=10;
		}else if(TLRB ==2){
			isLeft-=10;
			isRight-=10;
		}else if(TLRB ==1){
			isTop-=10;
			isBottom-=10;
		}else if(TLRB ==4){
			isTop+=10;
			isBottom+=10;
		}

		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수

		break;
	case WM_PAINT :
		hdc =BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(255,255,255));
		oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
		Ellipse(hdc,isLeft,isTop ,isRight,isBottom ); //왼쪽, 탑 , 오른쪽, 아래
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);
		sprintf(Debug, "isLeft : %d, isRight : %d isTop : %d, isRight : %d", isLeft, isRight, isTop, isBottom);
		TextOut(hdc, 0,510, Debug, strlen(Debug));
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hwnd,1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}