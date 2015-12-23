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
	static char str[MAXLINE][11];	// 0~9까지 열줄, 한줄당 0~9 총 10글자
	static int line, cnt; // 줄, 줄당 글자 개수
	static bool ColCheck;	// isNumberChechk가 대신해서 사용하기 때문에 해당 코드는 필요없어짐.
	static bool isNumberCheck[MAXLINE]; // 숫자가 입력된 것을 확인하기 위한 배열 생성


	switch (iMessage){
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 3, 15);
		ShowCaret(hWnd);
		ColCheck = false;
		cnt = 0;
		line = 0; //몇번째 줄인지 체크
		break;

	case WM_CHAR:
		if (wParam == VK_BACK)	//백스페이스
		{
			ColCheck = false;
			cnt--;
			if (cnt == 0 && line != 0)
			{
				--line;
				cnt = strlen(str[line]);
			}
		}
		else if (wParam == VK_ESCAPE)		//esc누르면 전체 지우기
		{
			for (int i = 0; i < 11; ++i)
			{
				for (int j = 0; j < line + 1; ++j)
				{
					str[j][i] = NULL;
				}
			}

			///////////////////////////////////////////////////////////////////////////////// 
			// ESC 누를경우 전체 지우기를 활성화 해야 하므로 모든 배열을 False로 변경.
			for (int i = 0; i < MAXLINE-1; i++){		
				isNumberCheck[i] = false;
			}
			///////////////////////////////////////////////////////////////////////////////// 

			cnt = 0;
			line = 0;
		}
		else if (wParam == VK_RETURN)		//엔터
		{
			ColCheck = false;
			if (line < MAXLINE-1 )			// line+1이 기준점이기 때문에 line이 9면 10번째줄이라 엔터 금지
			{
				line++;			//라인을 넘기고 
				cnt = 0;			//맨앞에서 부터 쓸 수 있게
			}
		}
		else if (wParam >= 48 && wParam <= 57)			//숫자 입력시 빨간색으로 문자 출력 후 다음줄로 넘어감
		{
			ColCheck = true;

			if (cnt != 0 ){
				// 중간에 글자 입력후 숫자를 입력할시에 대한 처리.
				// ex) My name 1
				// 위와같이 입력될경우 마지막에 숫자가 입력이 되었으므로 빨간색으로 변경이 됨.
				// 그러므로 cnt 즉 글자가 1글자라도 입력이 되었을경우 강제로 줄을 내리고선 작업을 하게 만들어야함.
				cnt = 0;
				line++; //글자가 입력된 부분의 줄을 넘기기 위해 line ++
				isNumberCheck[line] = true; // line이 넘겨진 줄이 숫자가 입력 되었으므로 true로 반환
				str[line][cnt++] = wParam;  // line이 넘겨진 줄에 해당 글자를 입력.

				cnt = 0; // line 넘어간 이후니깐 아래와 같이 한줄 넘기는 작업을 진행.
				line++;

			}else{
				isNumberCheck[line] = true; //숫자가 입력 되었을시 해당 배열을 True 로 변경한다.
				// 기존의 경우 line을 먼저 ++해버리고 나서 str에 글자를 입력해서 숫자 입력후 한줄이 넘어감.
				// 글자나 숫자를 먼저 입력 받은후 라인을 증가 시켜 주어야지 정상적으로 출력
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

		if (wParam == 'Q' || wParam == 'q')		//Q,q를 입력시 프로그램 종료
			DestroyWindow(hWnd);

		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		///////////////////////////////////////////////////////////////////////////////// 

		for (int i = 0; i < line + 1; i++){		// for문으로 전체 라인을 출력.
			GetTextExtentPoint(hdc, str[i], strlen(str[i]), &size);

			if( isNumberCheck[i] == true){ // 만약 isNumberCheck[i] 번째가 True 일경우 ( 즉 숫자로 입력이 되었을경우)
				SetTextColor(hdc, RGB(255, 0, 0)); // 해당 글자의 색상을 빨간색으로 변경.
			}else{
				SetTextColor(hdc, RGB(0, 0, 0)); // 아닐 경우 해당 글자의 색상을 검정색으로 출력.
			}
			TextOut(hdc, 0, i * 20, str[i], strlen(str[i]));		//엔터 높이는 20
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

