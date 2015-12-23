#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"
#include <atlconv.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("WriteFile");
#define limitData 100

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
	//WndClass.hbrBackground = CreateSolidBrush(RGB(51,181,229));
	WndClass.hbrBackground = CreateSolidBrush(RGB(255,255,255));
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

HWND hEdit;
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static SIZE size;
	static OPENFILENAME OFN;
	OPENFILENAME SFN;
	FILE *fPtr;
	HANDLE hFile;
	CHAR buf[32000]={0,};
	DWORD dwRead;
	DWORD dwWritten;
	static char filename[1000] = "";
	static int line=30, start =0;
	static char* buffer=(char*)calloc(10,sizeof(char));
	static char isTextData[1000];
	static char str[100], lpstrFile[1000] = "", isData[1000] = "";
	char filter[] = "Every File(*.*) \0*.*\0Visual Studio File\0*.cpp;*.c0";
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
			OFN.lpstrInitialDir="."; // �ʱ� ���丮
			if (GetOpenFileName(&OFN)!=0) {
				wsprintf(str,"%s\n������ ���ðڽ��ϱ� ?",OFN.lpstrFile);
				MessageBox(hwnd,str,"���� ����",MB_OK);

				USES_CONVERSION;
				strcpy(isData, T2A(OFN.lpstrFile));
				start = 1;
				line=30;

			}
			InvalidateRect(hwnd,NULL,TRUE);
			break;
		case ID_MENU_SAVE:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner=hwnd;
			SFN.lpstrFilter= filter;
			SFN.lpstrFile=lpstrFile;
			SFN.nMaxFile=100;
			SFN.lpstrInitialDir="."; // �ʱ� ���丮
			if (GetSaveFileName(&SFN)!=0) {
				wsprintf(str,"%s\n���Ͽ� �����Ͻðڽ��ϱ� ?",SFN.lpstrFile);
				MessageBox(hwnd,str,"�����ϱ� ����",MB_OK);

				USES_CONVERSION;
				strcpy(isData, T2A(OFN.lpstrFile));

				fPtr = fopen (isData, "wb" );
				fwrite (isTextData , 2 , lstrlen(isTextData) , fPtr );
				fclose (fPtr);
			}
			break;

		}

		break;
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 5,15);
		ShowCaret(hwnd);
		//hEdit = CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,10,10,400,300,hwnd,(HMENU)0,g_hInst,NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		if(start != 0){
			
			fPtr = fopen(isData, "r");
			fread(isTextData, limitData, 1, fPtr); 
			fclose(fPtr);
			
			hFile=CreateFile(isData,GENERIC_READ,0,NULL,
				OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFile!=INVALID_HANDLE_VALUE) {
				ReadFile(hFile,buf,1024,&dwRead,NULL);
				CloseHandle(hFile);
				MessageBox(hwnd,buf,"���� ���",MB_OK);
			}

			fPtr = fopen(isData, "r");
			while(fgets(buffer, limitData, fPtr))
			{
				TextOut(hdc,5,line,buffer,lstrlen(buffer));
				line+=20;
			}
			fclose(fPtr);

		}



		TextOut(hdc,5,0,isData,lstrlen(isData));
		
		wsprintf(buf, buf);
		TextOut(hdc,5,line,buf,lstrlen(buf));
		
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		HideCaret(hwnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}