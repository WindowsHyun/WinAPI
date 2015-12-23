#include <Windows.h>
#include <stdio.h>
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
	static CHAR str[10][100] ={0};
	static CHAR temp[10][100] ={0};
	static int count, tempCount, yPos;
	static int line, tempLine;

	OPENFILENAME OFN;
	char str[100], lpstrFile[100] = "";
	char filter[] = "Every File(*.*) \0*.*\0cpp File\0*.cpp;*.c\0";
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
			if ( line < 9){
				count =0;
				++line;
				yPos = yPos +20;
			}
		}else if (wParam == VK_ESCAPE){

			for(int i =0; i<10; ++i){
				for(int j=0; j<100; ++j){
					temp[i][j] = str[i][j] ;
					str[i][j] =0;
				}
			}
			tempLine = line;
			line = 0;
			tempCount = count;
			count = 0;
		}else if (wParam == VK_TAB){

			for(int i =0; i<10; ++i){
				for(int j=0; j<100; ++j){
					str[i][j] = temp[i][j] ;
				}
			}
			line= tempLine;
			count= tempCount;
		}else{

			if( count < 99 ){ // 맨끝자리가 null 들어가야하는데 = 해버리면 자리가 없어서 문자열 끝이사라짐
				str[line][count++] = wParam;
				str[line][count] = '\0';
				TextOut(hdc, 0,0, str[line], strlen(str[line]));
				ReleaseDC(hwnd,hdc);
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