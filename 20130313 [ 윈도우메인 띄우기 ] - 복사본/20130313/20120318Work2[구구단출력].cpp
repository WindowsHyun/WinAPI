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
	hwnd = CreateWindow("Window Class Name", "Window Title Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 1000, NULL, NULL, hInstance, NULL);
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
	PAINTSTRUCT ps;
	RECT rt;
	char str[50];

	switch(iMsg)
	{

	case WM_CREATE:

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for(int i=0; i <8; ++i){
			for(int j=0; j<9; ++j){

				rt.top = 1000/9*j;
				rt.left = 1000/8*i;
				rt.bottom = (1000/9)*(j+1);
				rt.right = (1000/8)*(i+1);

				
				//TextOut(hdc, 800/5*i, 600/4*j, str , lstrlen(str));
				SetTextColor(hdc, RGB(rand()%255,rand()%255,rand()%255));
				SetBkColor(hdc, RGB(255-rand()%255, 255-rand()%255,255-rand()%255));

				sprintf(str, "%d x %d = %d", i+2, j+1,(i+2)*(j+1));

				DrawText(hdc,str,lstrlen(str),&rt, DT_CENTER |DT_SINGLELINE|DT_VCENTER);
				//DrawText(hdc,"권창현",lstrlen("권창현"),&rt, DT_CENTER | DT_VCENTER|DT_SINGLELINE);


			}
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}