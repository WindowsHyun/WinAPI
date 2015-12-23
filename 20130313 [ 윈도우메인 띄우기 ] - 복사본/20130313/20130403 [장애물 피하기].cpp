#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define MAXLINE 10

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("window program 2-3");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstnace,
					 LPSTR lpszCmdParam, int nCmdShow){
						 HWND hWnd;
						 MSG Message;
						 WNDCLASS WndClass;
						 g_hInst = hInstance;

						 WndClass.cbClsExtra = 0;
						 WndClass.cbWndExtra = 0;
						 WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
						 WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
						 WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
						 WndClass.hInstance = hInstance;
						 WndClass.lpfnWndProc = WndProc;
						 WndClass.lpszClassName = lpszClass;
						 WndClass.lpszMenuName = NULL;
						 WndClass.style = CS_HREDRAW | CS_VREDRAW;
						 RegisterClass(&WndClass);

						 hWnd = CreateWindow(
							 lpszClass,
							 lpszClass,
							 WS_OVERLAPPEDWINDOW,
							 0, 0, 900, 800,
							 NULL, (HMENU)NULL, hInstance, NULL);

						 ShowWindow(hWnd, nCmdShow);

						 while (GetMessage(&Message, NULL, 0, 0)){
							 TranslateMessage(&Message);
							 DispatchMessage(&Message);
						 }

						 return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	COLORREF Color;
	static SIZE size;
	static char str[MAXLINE][11];	// 0~9���� ����, ���ٴ� 0~9 �� 10����
	static int line, cnt; // ��, �ٴ� ���� ����
	static bool ColCheck;	// isNumberChechk�� ����ؼ� ����ϱ� ������ �ش� �ڵ�� �ʿ������.
	static bool isNumberCheck[MAXLINE]; // ���ڰ� �Էµ� ���� Ȯ���ϱ� ���� �迭 ����


	switch (iMessage){
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 3, 15);
		ShowCaret(hWnd);
		ColCheck = false;
		cnt = 0;
		line = 0; //���° ������ üũ
		break;

	case WM_CHAR:
		if (wParam == VK_BACK)	//�齺���̽�
		{
			ColCheck = false;
			cnt--;
			if (cnt == 0 && line != 0)
			{
				--line;
				cnt = strlen(str[line]);
			}
		}
		else if (wParam == VK_ESCAPE)		//esc������ ��ü �����
		{
			for (int i = 0; i < 11; ++i)
			{
				for (int j = 0; j < line + 1; ++j)
				{
					str[j][i] = NULL;
				}
			}

			///////////////////////////////////////////////////////////////////////////////// 
			// ESC ������� ��ü ����⸦ Ȱ��ȭ �ؾ� �ϹǷ� ��� �迭�� False�� ����.
			for (int i = 0; i < MAXLINE-1; i++){		
				isNumberCheck[i] = false;
			}
			///////////////////////////////////////////////////////////////////////////////// 

			cnt = 0;
			line = 0;
		}
		else if (wParam == VK_RETURN)		//����
		{
			ColCheck = false;
			if (line < MAXLINE-1 )			// line+1�� �������̱� ������ line�� 9�� 10��°���̶� ���� ����
			{
				line++;			//������ �ѱ�� 
				cnt = 0;			//�Ǿտ��� ���� �� �� �ְ�
			}
		}
		else if (wParam >= 48 && wParam <= 57)			//���� �Է½� ���������� ���� ��� �� �����ٷ� �Ѿ
		{
			ColCheck = true;

			if (cnt != 0 ){
				// �߰��� ���� �Է��� ���ڸ� �Է��ҽÿ� ���� ó��.
				// ex) My name 1
				// ���Ͱ��� �Էµɰ�� �������� ���ڰ� �Է��� �Ǿ����Ƿ� ���������� ������ ��.
				// �׷��Ƿ� cnt �� ���ڰ� 1���ڶ� �Է��� �Ǿ������ ������ ���� ������ �۾��� �ϰ� ��������.
				cnt = 0;
				line++; //���ڰ� �Էµ� �κ��� ���� �ѱ�� ���� line ++
				isNumberCheck[line] = true; // line�� �Ѱ��� ���� ���ڰ� �Է� �Ǿ����Ƿ� true�� ��ȯ
				str[line][cnt++] = wParam;  // line�� �Ѱ��� �ٿ� �ش� ���ڸ� �Է�.

				cnt = 0; // line �Ѿ ���Ĵϱ� �Ʒ��� ���� ���� �ѱ�� �۾��� ����.
				line++;

			}else{
				isNumberCheck[line] = true; //���ڰ� �Է� �Ǿ����� �ش� �迭�� True �� �����Ѵ�.
				// ������ ��� line�� ���� ++�ع����� ���� str�� ���ڸ� �Է��ؼ� ���� �Է��� ������ �Ѿ.
				// ���ڳ� ���ڸ� ���� �Է� ������ ������ ���� ���� �־���� ���������� ���
				str[line][cnt++] = wParam; 
				cnt = 0;
				line++;
			}

			if (line > MAXLINE - 1) break;
		}
		else
		{
			ColCheck = false;
			if (cnt > MAXLINE - 1)
			{
				line++;
				cnt = 0;
			}
			ColCheck = false;
			str[line][cnt++] = wParam;
		}

		str[line][cnt] = '\0';

		if (wParam == 'Q' || wParam == 'q')		//Q,q�� �Է½� ���α׷� ����
			DestroyWindow(hWnd);

		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		///////////////////////////////////////////////////////////////////////////////// 

		for (int i = 0; i < line + 1; i++){		// for������ ��ü ������ ���.
			GetTextExtentPoint(hdc, str[i], strlen(str[i]), &size);

			if( isNumberCheck[i] == true){ // ���� isNumberCheck[i] ��°�� True �ϰ�� ( �� ���ڷ� �Է��� �Ǿ������)
				SetTextColor(hdc, RGB(255, 0, 0)); // �ش� ������ ������ ���������� ����.
			}else{
				SetTextColor(hdc, RGB(0, 0, 0)); // �ƴ� ��� �ش� ������ ������ ���������� ���.
			}
			TextOut(hdc, 0, i * 20, str[i], strlen(str[i]));		//���� ���̴� 20
			SetCaretPos(size.cx, i * 20);
		}

		///////////////////////////////////////////////////////////////////////////////// 
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

