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
#define VK_Q           0x51

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
	static CHAR str[1000][1000] ={0};
	static int count, tempCount, yPos;
	static int line, tempLine;

	switch(iMsg)
	{
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 5,15);
		ShowCaret(hwnd);
		count = 0;
		yPos=0;
		line =0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for(int i=0; i<=line; ++i){
			TextOut(hdc,0,size.cy*i,str[i],strlen(str[i]));
			GetTextExtentPoint(hdc,str[i],strlen(str[i]),&size);
		}
		SetCaretPos(size.cx,16*line); // 엔터치고 마지막줄인 Line 번째 줄로 가므로 0이되서 그냥 크기 16으로
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc=GetDC(hwnd);
		if(wParam == VK_BACK ){
			if(count <=0){
			}else{
				--count;
				str[line][count]=0;
			}
		}else if (wParam == VK_RETURN ){
			//Enter 는 의미 없는 코드이므로
		}else{

			if( wParam == VK_0 || wParam == VK_1 || wParam == VK_2 || wParam == VK_3 || wParam == VK_4 || wParam == VK_5 || wParam == VK_6 || wParam == VK_7 || wParam == VK_8 || wParam == VK_9 ){ 
				if(count !=0){
					count =0;
					++line;
					yPos = yPos +20;
				}
				str[line][count++] = wParam;
				str[line][count] = '\0';
				TextOut(hdc, 0,0, str[line], strlen(str[line]));
				count =0;
				++line;
				yPos = yPos +20;
				ReleaseDC(hwnd,hdc);
			}else{
				if( wParam == VK_Q || wParam == 113){
					PostQuitMessage(0);
				}else{
					str[line][count++] = wParam;
					str[line][count] = '\0';
					TextOut(hdc, 0,0, str[line], strlen(str[line]));
				}
			}

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