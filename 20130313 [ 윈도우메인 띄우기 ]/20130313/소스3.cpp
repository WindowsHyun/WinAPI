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
	static RECT rt = { 0, 0, 800, 800 };		//������Ŭ���̾�Ʈâ ��ǥ
	static RECT rect[3];			//�簢�� 3��
	static RECT temprt[3];		//�ʱ� ��ǥ �����
	static int rtsize[3];			//�簢�� ũ������
	static bool draw[3];
	static int speed[3];			//�簢�� �����̴� �ӵ�\
	// ������ �����ִ� ����� ����!
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

			if (rect[0].left >= rt.right){ // �Ķ��� ���簢���� ������ ������ ��� �̵�������!
				draw[0] = false;
				rect[0].left = temprt[0].left;
				rect[0].top = temprt[0].top;
				rect[0].right = temprt[0].right;
				rect[0].bottom = temprt[0].bottom;
				//�躸����� ó������ �̵��� �ϰ�������, �ش� ����� ��ġ�� ���δ� �����´�.
			}


			if (rect[0].right >= rt.right) //�Ķ��� ���簢���� ȭ������ʺ��� �Ѿ�����
			{
				draw[0] = true;
			}

			if (draw[0]){

				rect[0].left += 10;
				rect[0].right += 10;
				temprt[0].left += 10;
				temprt[0].right += 10;

				if (temprt[0].left >= rt.left){ // ����� ���簢���� ȭ�� ���ʺ��� Ŭ���
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


		sprintf(Debug, "���� : isLeft : %d, isRight : %d isTop : %d, isRight : %d", rect[0].left, rect[0].right, rect[0].top, rect[0].bottom);
		TextOut(hdc, 0,700, Debug, strlen(Debug));

		sprintf(Debug2, "���纻 : isLeft : %d, isRight : %d isTop : %d, isRight : %d", temprt[0].left, temprt[0].right, temprt[0].top, temprt[0].bottom);
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
��ȣ���� ����Ͽ� �̵��ϴ� ���� �׸���

ȭ���� y������ 3����ϰ� �� ��п� ������ ũ���� �簢���� �׸���. �� ������ �̵� �ӵ��� �ٸ���.

- �� �簢�� = ���ʿ��� ���������� �̵�
- �߰� �簢�� = �����ʿ��� �������� �̵�.
- �Ʒ� �簢�� = ���ʿ��� ���������� �̵�

��ȣ�� ���

- �Ķ����� ���콺�� Ŭ�� = �̵� ����
- ������� ���콺�� Ŭ�� = 3�� �� ����
- �������� ���콺�� Ŭ�� = �ٷ� ����

�̵� �ӵ��� ���� �ٸ��� ����

���� �Ǵ� ������ �����ڸ��� �����ϸ� �ݴ��� �����ڸ����� ��Ÿ����. �̶� ȭ�鿡 �ȱ׷�����
�Ϻκ��� �簢���� �ݴ��� �׸����� �Ѵ�.
*/