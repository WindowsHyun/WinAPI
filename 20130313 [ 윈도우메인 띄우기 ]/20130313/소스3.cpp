#include <time.h>
#include <stdio.h>
#include <Windows.h>

#define SPEED 10
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Hello World");

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

						 hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
							 0, 0, 800, 800,
							 NULL, (HMENU)NULL, hInstance, NULL);

						 ShowWindow(hWnd, nCmdShow);

						 while (GetMessage(&Message, NULL, 0, 0)){
							 TranslateMessage(&Message);
							 DispatchMessage(&Message);
						 }

						 return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT img, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hbrush, oldbrush;
	static RECT rt = { 0, 0, 800, 800 };		//윈도우클라이언트창 좌표
	static RECT rect[3];			//사각형 3개
	static RECT temprt[3];		//초기 좌표 저장용
	static int rtsize[3];			//사각형 크기조절
	static bool draw[3];
	static int speed[3];			//사각형 움직이는 속도\
	// 눈으로 볼수있는 디버그 제작!
	static char Debug[50];
	static char Debug2[50];
	static char Debug3[50];

	switch (img) {

	case WM_KEYDOWN :

		if(wParam == VK_RIGHT){
			SetTimer(hWnd, 1, 250, NULL);
		}
		if(wParam == VK_LEFT){
			KillTimer(hWnd, 1);
		}

		break;

	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		srand((unsigned)time(NULL));
		SetTimer(hWnd, 1, 100, NULL);
		//SetTimer(hWnd, 2, 40, NULL);
		//SetTimer(hWnd, 3, 70, NULL);

		rect[0].top = rand() % 100;
		rect[1].top = rand() % 100 + 200;
		rect[2].top = rand() % 100 + 400;


		for (int i = 0; i < 3; ++i)
		{
			rect[i].left = rand() % 400;
			rtsize[i] = rand() % 100 + 50;
			rect[i].right = rect[i].left + rtsize[i];
			rect[i].bottom = rect[i].top + rtsize[i];

			temprt[i].left = rect[i].right;
			temprt[i].top = rect[i].top;
			temprt[i].right = temprt[i].left + rtsize[i];
			temprt[i].bottom = temprt[i].top + rtsize[i];

			draw[i] = false;
		}
		break;

	case WM_TIMER:
		switch (wParam){
		case 1:

			if (rect[0].left >= rt.right){ // 파란색 정사각형이 오른쪽 끝으로 모두 이동했을때!
				draw[0] = false;
				rect[0].left = temprt[0].left;
				rect[0].top = temprt[0].top;
				rect[0].right = temprt[0].right;
				rect[0].bottom = temprt[0].bottom;
				//↑보라색이 처음부터 이동을 하고있으니, 해당 보라색 위치를 전부다 가져온다.
			}


			if (rect[0].right >= rt.right) //파란색 정사각형이 화면오른쪽보다 넘어갔을경우
			{
				draw[0] = true;
			}

			if (draw[0]){

				rect[0].left += 10;
				rect[0].right += 10;
				temprt[0].left += 10;
				temprt[0].right += 10;

				if (temprt[0].left >= rt.left){ // 보라색 정사각형이 화면 왼쪽보다 클경우
					draw[0] = false;
					temprt[0].right = 0;
					temprt[0].left = temprt[0].right - rtsize[0];
				}

			}else{
				rect[0].left += 10;
				rect[0].right += 10;
				temprt[0].left += 10;
				temprt[0].right += 10;

				/*temprt[0].left = rect[0].right;
				temprt[0].top = rect[0].top;
				temprt[0].right = temprt[0].left + rtsize[0];
				temprt[0].bottom = temprt[0].top + rtsize[0];*/
			}
			break;

		case 2:

			break;

		case 3:

			break;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 3; ++i)
		{


			if (draw[i])

			{
				hbrush = CreateSolidBrush(RGB(0, 0, 255));
				oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
				Rectangle(hdc, temprt[i].left, temprt[i].top, temprt[i].right, temprt[i].bottom);
				SelectObject(hdc, oldbrush);
				DeleteObject(hbrush);



			}else

				hbrush = CreateSolidBrush(RGB(0, 0, 255));
			oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
			Rectangle(hdc, rect[i].left, rect[i].top, rect[i].right, rect[i].bottom);
			SelectObject(hdc, oldbrush);
			DeleteObject(hbrush);

		}


		sprintf(Debug, "원본 : isLeft : %d, isRight : %d isTop : %d, isRight : %d", rect[0].left, rect[0].right, rect[0].top, rect[0].bottom);
		TextOut(hdc, 0,700, Debug, strlen(Debug));

		sprintf(Debug2, "복사본 : isLeft : %d, isRight : %d isTop : %d, isRight : %d", temprt[0].left, temprt[0].right, temprt[0].top, temprt[0].bottom);
		TextOut(hdc, 0,720, Debug2, strlen(Debug2));

		sprintf(Debug3, "isLeft : %d, isRight : %d isTop : %d, isRight : %d", rt.left, rt.right, rt.top, rt.bottom);
		TextOut(hdc, 0,740, Debug3, strlen(Debug3));

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, img, wParam, lParam));
}

/*
신호등을 사용하여 이동하는 도형 그리기

화면을 y값으로 3등분하고 각 등분에 임의의 크기의 사각형을 그린다. 각 도형의 이동 속도는 다르다.

- 위 사각형 = 왼쪽에서 오른쪽으로 이동
- 중간 사각형 = 오른쪽에서 왼쪽으로 이동.
- 아래 사각형 = 왼쪽에서 오른쪽으로 이동

신호등 기능

- 파랑색을 마우스로 클릭 = 이동 시작
- 노랑색을 마우스로 클릭 = 3초 후 정지
- 빨강색을 마우스로 클릭 = 바로 정지

이동 속도는 각각 다르게 설정

왼쪽 또는 오른쪽 가장자리에 도달하면 반대편 가장자리에서 나타난다. 이때 화면에 안그려지는
일부분의 사각형을 반대편에 그리도록 한다.
*/