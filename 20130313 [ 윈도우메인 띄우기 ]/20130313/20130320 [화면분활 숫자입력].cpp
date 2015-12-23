#include <Windows.h>

#define VK_0           0x30
#define VK_1           0x31
#define VK_2           0x32
#define VK_3           0x33
#define VK_4           0x34
#define VK_5           0x35
#define VK_6           0x36
#define VK_7           0x37
#define VK_8           0x38
#define VK_9           0x39

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
	hwnd = CreateWindow("Window Class Name", "Window ICS Background Title Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
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
	static SIZE size;
	static CHAR str0[100] ={0};
	static CHAR str1[100] ={0};
	static CHAR str2[100] ={0};
	static CHAR str3[100] ={0};
	static CHAR str4[100] ={0};
	static int count[5], tempCount, yPos;
	static int line, tempLine;

	switch(iMsg)
	{
	case WM_CREATE:
		//CreateCaret(hwnd, NULL, 5,15);
		//ShowCaret(hwnd);
		for(int i=0; i<5; ++i){
		count[i] = 0;
		}
		yPos=0;
		line =0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		/*for(int i=0; i<=line; ++i){
			TextOut(hdc,0,600/i,str0,strlen(str0));
			//GetTextExtentPoint(hdc,str0[i],strlen(str0[i]),&size);
		}*/
		TextOut(hdc,800/5*0,0,str0,strlen(str0));
		TextOut(hdc,800/5*1,0,str1,strlen(str1));
		TextOut(hdc,800/5*2,0,str2,strlen(str2));
		TextOut(hdc,800/5*3,0,str3,strlen(str3));
		TextOut(hdc,800/5*4,0,str4,strlen(str4));
		
		//SetCaretPos(size.cx,16*line); // 엔터치고 마지막줄인 Line 번째 줄로 가므로 0이되서 그냥 크기 16으로
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc=GetDC(hwnd);

			if( wParam == VK_0 || wParam == VK_5 ){ 
				str0[count[0]++] = wParam;
				str0[count[0]] = '\0';
				TextOut(hdc, 0,0, str0, strlen(str0));
				ReleaseDC(hwnd,hdc);
			}
			if( wParam == VK_1 || wParam == VK_6 ){ 
				str1[count[1]++] = wParam;
				str1[count[1]] = '\0';
				TextOut(hdc, 0,0, str1, strlen(str1));
				ReleaseDC(hwnd,hdc);
			}
			if( wParam == VK_2 || wParam == VK_7 ){ 
				str2[count[2]++] = wParam;
				str2[count[2]] = '\0';
				TextOut(hdc, 0,0, str2, strlen(str2));
				ReleaseDC(hwnd,hdc);
			}
			if( wParam == VK_3 || wParam == VK_8 ){ 
				str3[count[3]++] = wParam;
				str3[count[3]] = '\0';
				TextOut(hdc, 0,0, str3, strlen(str3));
				ReleaseDC(hwnd,hdc);
			}
			if( wParam == VK_4 || wParam == VK_9 ){ 
				str4[count[4]++] = wParam;
				str4[count[4]] = '\0';
				TextOut(hdc, 0,0, str4, strlen(str4));
				ReleaseDC(hwnd,hdc);
			}


		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;
	case WM_DESTROY:
		HideCaret(hwnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}