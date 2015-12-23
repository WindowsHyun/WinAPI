#include <Windows.h>
#include <stdlib.h>

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
	static int rnadom;
	static RECT rt[4];
	static BOOL Red[4];
	static char* str[4] = {"UP", "LEFT", "DOWN", "RIGHT" };
	HBRUSH hBrush, oldBrush;
	switch(iMsg)
	{
	case WM_CREATE:
		rt[0].top = 200, rt[0].left = 50, rt[0].right = 150, rt[0].bottom = 400; //1번
		rt[1].top = 200, rt[1].left = 200, rt[1].right = 300, rt[1].bottom = 400;// 2번
		rt[2].top = 200, rt[2].left = 350, rt[2].right = 450, rt[2].bottom = 400;// 3번
		rt[3].top = 200, rt[3].left = 500, rt[3].right = 600, rt[3].bottom = 400; // right
		break;
	case WM_PAINT :

		hdc =BeginPaint(hwnd, &ps);
		for(int i=0; i<4; ++i){
			if(Red[i] == TRUE){
				if(i == 0 )
				hBrush = CreateSolidBrush(RGB(255,0,0));
				if(i == 1 )
				hBrush = CreateSolidBrush(RGB(0,255,0));
				if(i == 2 )
				hBrush = CreateSolidBrush(RGB(0,0,255));
				if(i == 3 )
				hBrush = CreateSolidBrush(RGB(rand()%255,rand()%255,rand()%255));
			}else{
				hBrush = CreateSolidBrush(RGB(255,255,255));
			}
			oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
			Ellipse(hdc,rt[i].left,rt[i].top ,rt[i].right,rt[i].bottom ); //왼쪽, 탑 , 오른쪽, 아래
			//TextOut(hdc, rt[i].left, rt[i].top, str[i] , lstrlen(str[i]));
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}
		EndPaint(hwnd, &ps);
		return 0;
	
	case WM_CHAR :
		if( wParam == 'r' || wParam == 'R'){
			if(Red[0] == TRUE){
				Red[0] = FALSE;
			}else{
				Red[0] = TRUE;
			}
		}else if( wParam == 'g' || wParam == 'G'){
			if(Red[1] == TRUE){
				Red[1] = FALSE;
			}else{
				Red[1] = TRUE;
			}
		}else if( wParam == 'b' || wParam == 'B'){
			if(Red[2] == TRUE){
				Red[2] = FALSE;
			}else{
				Red[2] = TRUE;
			}
		}else if( wParam == 'a' || wParam == 'A'){
			if(Red[3] == TRUE){
				Red[3] = FALSE;
			}else{
				Red[3] = TRUE;
			}
		}else{
			rnadom = rand()%3;
			if(Red[rnadom] == TRUE){
				Red[rnadom] = FALSE;
			}else{
				Red[rnadom] = TRUE;
			}

		}

		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;



	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}