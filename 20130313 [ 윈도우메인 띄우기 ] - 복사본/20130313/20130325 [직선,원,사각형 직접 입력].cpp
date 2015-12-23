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
	static int Realnum[5]; // ���� ���ڰ� �� ��Ʈ
	static int count; // ���ڼ��� ���� ī����
	static char RealNumber[50]; // �ؽ�Ʈ �ڽ�ó�� �����ֱ� ���� ĳ����
	static char Debug[50]; // �ټ��� ī���ͼ��� Ȯ���ϱ� ���� ����� ĳ����
	static int line = 0; // ó�� �ټ��� 0�ٺ��� �̹Ƿ� 0
	static CHAR str[5][5] = {0}; // ���������� �Է¹޴� �κ�
	POINT prevpos;
	HBRUSH hBrush, oldBrush;
	switch(iMsg)
	{
	case WM_CREATE:

		break;
	case WM_PAINT :
		hdc =BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(rand()%255,rand()%255,rand()%255));
		oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
		// ���� =1, ���� =2, �簢�� =3
		if(Realnum[0] == 1){
			MoveToEx(hdc,Realnum[1],Realnum[2], &prevpos ); //����, ž , ������, �Ʒ�
			LineTo(hdc, Realnum[3],Realnum[4]);
		}else if(Realnum[0] == 2){
			Ellipse(hdc,Realnum[1],Realnum[2] ,Realnum[3],Realnum[4] ); //����, ž , ������, �Ʒ�
		}else if(Realnum[0] ==3){
			Rectangle(hdc,Realnum[1],Realnum[2] ,Realnum[3],Realnum[4] ); //����, ž , ������, �Ʒ�
		}
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);
		sprintf(Debug, "LINE : %d, COUNT : %d, RealNum : %d, %d, %d, %d, %d", line, count, Realnum[0], Realnum[1], Realnum[2],Realnum[3], Realnum[4]);
		TextOut(hdc, 0,510, Debug, strlen(Debug));
		TextOut(hdc, 0,530, "-----------------------------------------------------------------------------------------------------------", strlen("-----------------------------------------------------------------------------------------------------------"));
		sprintf(RealNumber, "%s %s %s %s %s", str[0], str[1],str[2], str[3], str[4]);
		TextOut(hdc, 0,550, RealNumber, strlen(RealNumber));

		EndPaint(hwnd, &ps);
		return 0;

	case WM_CHAR :
		hdc=GetDC(hwnd);
		if(line >= 5){
			//���̻� ���� ���ϰ� �Ѵ�.
			if (wParam == VK_RETURN ){
				line =0;
				count =0;
				for(int i=0; i<5; ++i){
					str[i][0] = 0;
				}
			}
		}else{
			if(wParam == ' '){
				Realnum[line] = atoi(str[line]);
				count = 0;
				++line;
			}else if(wParam == VK_BACK ){
				if(count <=0){
				}else{
					--count;
					str[line][count]=0;
				}
			}else if (wParam == VK_RETURN ){
				//����Ű ��ȿȭ
			}else{
				str[line][count++] = wParam;
				str[line][count] = '\0';
			}
		}
		ReleaseDC(hwnd,hdc);
		InvalidateRect(hwnd, NULL, TRUE); //��ȿȭ �Լ�
		break;



	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}