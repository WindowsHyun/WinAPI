#include <Windows.h>
#include <stdio.h>
#include <time.h>


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
	static SIZE size;
	HDC hdc;
	PAINTSTRUCT ps;
	static char str[1000] = "키보드 입력 연습문제";
	static int isX = rand()%800, isY = rand()%600; // static을 안쓰면 계속 초기화된다 (정적변수)
	static int count=0;
	switch(iMsg)
	{
	case WM_CREATE:
		break;

	case WM_CHAR:
		if (wParam != VK_BACK){
		hdc=GetDC(hwnd);
		str[count++] = wParam;
		str[count] = '\0';
		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		isX = rand()%800;
		isY = rand()%600;
		}	
		break;
	case WM_KEYDOWN:

		hdc=GetDC(hwnd);
		if (wParam == VK_UP){
			if(isY <=0){
			}else{
				isY -= 10;
			}
		}
		if (wParam == VK_LEFT){
			if(isX <=0){

			}

			}else{
				isX -= 10;
			}
		}
		if (wParam == VK_RIGHT){
			if(isX < 800-size.cx-10){
				isX += 10;
			}
		}
		if (wParam == VK_DOWN){
			if(isY < 600-size.cy-30){
				isY += 10;
			}
		}
		if (wParam == VK_BACK){
			count = 0;
			str[count] = '\0';
		}
		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//sprintf(str, "(%d, %d)", isX, isY);
		TextOut(hdc, isX, isY, str , lstrlen(str));
		GetTextExtentPoint(hdc,str,lstrlen(str),&size);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}