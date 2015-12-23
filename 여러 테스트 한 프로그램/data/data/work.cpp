#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
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
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
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
	static OPENFILENAME OFN;
	OPENFILENAME SFN;
	char str[100], lpstrFile[100] = "", isData[1000] = "";
	char filter[] = "Visual Studio File\0*.cpp;*.c\0Every File(*.*) \0*.*\0";
	switch(iMsg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case ID_MENU_OPEN:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner=hwnd;
			OFN.lpstrFilter= filter;
			OFN.lpstrFile=lpstrFile;
			OFN.nMaxFile=100;
			OFN.lpstrInitialDir="."; // 초기 디렉토리
			if (GetOpenFileName(&OFN)!=0) {
				wsprintf(str,"%s\n파일을 여시겠습니까 ?",OFN.lpstrFile);
				strcpy(isData, OFN.lpstrFile);
				MessageBox(hwnd,str,"열기 선택",MB_OK);
			}
			break;
		case ID_MENU_SAVE:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
				SFN.hwndOwner=hwnd;
			SFN.lpstrFilter= filter;
			SFN.lpstrFile=lpstrFile;
			SFN.nMaxFile=100;
			SFN.lpstrInitialDir="."; // 초기 디렉토리
			if (GetSaveFileName(&SFN)!=0) {
				wsprintf(str,"%s\n파일에 저장하시겠습니까 ?",
					SFN.lpstrFile);
				MessageBox(hwnd,str,"저장하기 선택",MB_OK);
			}
			break;

		}
		InvalidateRect(hwnd,NULL,TRUE);
		break;
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 5,15);
		ShowCaret(hwnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc,10,10,isData,lstrlen(isData));

		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc=GetDC(hwnd);
		
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