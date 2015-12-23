#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("window program 2-10");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstnace,
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
						   0, 0, 800, 600,
						   NULL, (HMENU)NULL, hInstance, NULL);

					   ShowWindow(hWnd, nCmdShow);
					   UpdateWindow(hWnd);
					   while (GetMessage(&Message, NULL, 0, 0)){
						   TranslateMessage(&Message);
						   DispatchMessage(&Message);
					   }
					   return (int)Message.wParam;
}

typedef struct obj
{
	int posX;
	int posY;
	int size;
	bool select;
	//int cnt; //���° �����ΰ�
}obj;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rt;
	HBRUSH myBrush, oldBrush;
	HPEN myPen, oldPen;
	//COLORREF Color;

	static POINT point[5][6];
	static COLORREF Color[5];
	static obj obj[5];	// ���� 5��, 0~4
	static bool isSelectNum[5]; //���° ������ �������� �ִ°�. ���� ��ȣ ���� ��� false
	static int	SelectNum;		   //���° ������ �����߳� �޾ƿ���. 
	static int	drawCnt;  //������ � �׷ȳ�.
	static int	ShapeNum[5]; //�׸��� ������ ������
	static int	number;	  //��׷ȴ����� �̿��� for���������� ���� ����
	static bool  temp[5];			//�ӽ÷� ��� �׸����� �ȱ׸�����
	static int	a;
	static int	tempcnt;
	static bool	d;

	switch (iMessage){
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		srand((unsigned)time(NULL));
		drawCnt = 0;
		SelectNum = 0;
		for (int i = 0; i <= number; ++i)
		{
			Color[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
		}
		for (int i = 0; i < 5; ++i)
		{
			temp[i] = true;
			ShapeNum[i] = 0;
		}
		tempcnt = 0;
		a = 1;
		d = false;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i <= number; ++i)
		{
			if (temp[i])	//��ȣ �������� ���� �͸� �׷��ֱ�!!!
			{
				myBrush = CreateSolidBrush(Color[i]);
				oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
				if (ShapeNum[i] == 1)
					Ellipse(hdc, obj[i].posX, obj[i].posY, obj[i].posX + obj[i].size, obj[i].posY + obj[i].size);
				else if (ShapeNum[i] == 2)
					Polygon(hdc, point[i], 3);
				else if (ShapeNum[i] == 3)
					Rectangle(hdc, obj[i].posX, obj[i].posY, obj[i].posX + obj[i].size, obj[i].posY + obj[i].size);
			}
			if (isSelectNum[i])	//��ȣ���������� �׷��ִ°�(�װ� �ϳ���)
			{
				myBrush = CreateSolidBrush(RGB(255, 245, 149));
				oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
				if (ShapeNum[SelectNum] == 1)
					Ellipse(hdc, obj[SelectNum].posX, obj[SelectNum].posY, obj[SelectNum].posX + obj[SelectNum].size, obj[SelectNum].posY + obj[SelectNum].size);
				else if (ShapeNum[SelectNum] == 2)
					Polygon(hdc, point[SelectNum], 3);
				else if (ShapeNum[SelectNum] == 3)
					Rectangle(hdc, obj[SelectNum].posX, obj[SelectNum].posY, obj[SelectNum].posX + obj[SelectNum].size, obj[SelectNum].posY + obj[SelectNum].size);
			}
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_UP){
			if (isSelectNum[SelectNum] == true && ShapeNum[SelectNum] == 1 || ShapeNum[SelectNum] == 3)
				obj[SelectNum].posY -= 10;
			else if (isSelectNum[SelectNum] == true && ShapeNum[SelectNum] == 2)
			{
				point[SelectNum][0].y -= 10;
				point[SelectNum][1].y -= 10;
				point[SelectNum][2].y -= 10;
			}
		}
		else if (wParam == VK_DOWN){
			if (isSelectNum[SelectNum] == true && ShapeNum[SelectNum] == 1 || ShapeNum[SelectNum] == 3)
				obj[SelectNum].posY += 10;
			else if (isSelectNum[SelectNum] == true && ShapeNum[SelectNum] == 2)
			{
				point[SelectNum][0].y += 10;
				point[SelectNum][1].y += 10;
				point[SelectNum][2].y += 10;
			}
		}
		else if (wParam == VK_LEFT){
			if (isSelectNum[SelectNum] == true && ShapeNum[SelectNum] == 1 || ShapeNum[SelectNum] == 3)
				obj[SelectNum].posX -= 10;
			else if (isSelectNum[SelectNum] == true && ShapeNum[SelectNum] == 2)
			{
				point[SelectNum][0].x -= 10;
				point[SelectNum][1].x -= 10;
				point[SelectNum][2].x -= 10;
			}
		}
		else if (wParam == VK_RIGHT){
			if (isSelectNum[SelectNum] == true && ShapeNum[SelectNum] == 1 || ShapeNum[SelectNum] == 3)
				obj[SelectNum].posX += 10;
			else if (isSelectNum[SelectNum] == true && ShapeNum[SelectNum] == 2)
			{
				point[SelectNum][0].x += 10;
				point[SelectNum][1].x += 10;
				point[SelectNum][2].x += 10;
			}
		}
		else if (wParam == VK_DELETE)
		{
			isSelectNum[SelectNum] = false;
			temp[SelectNum] = false;
			obj[SelectNum].posX = 0;
			obj[SelectNum].posY = 0;
			obj[SelectNum].size = 0;

			ShapeNum[SelectNum]  = 0; // ������ ������ 1,2,3 �� �ƴ� 0���� ����� ������.

			//drawCnt = SelectNum; // ������ �������� ������ �ϴϱ� drawcnt�� ������ ��ȣ�� ����.
			//drawCnt--;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_CHAR:
		if (wParam == 'E' || wParam == 'e'){		//800,600

			////////////////////////////////////////////////////////////////////////////////
			// for������ ���� 0���� number���� �����鼭
			// ������ ������ 0���� ��������Ƿ�, �ش� 0�� �� ������ ã�´�.
			// ã����� �ش� ��ȣ�� �׸��� �뵵�� �����.
			for (int i = 0; i <= number; ++i){
				if(ShapeNum[i] == 0){
					drawCnt = i;
					i=5;
				}
			}
			////////////////////////////////////////////////////////////////////////////////


			temp[drawCnt] = true;
			ShapeNum[drawCnt] = 1;
			////////////////////////////////////////////////////////////////////////////////

			obj[drawCnt].posX = rand() % 600 + 1;
			obj[drawCnt].posY = rand() % 400 + 1;
			obj[drawCnt].size = 50;
			Color[drawCnt] = RGB(rand() % 256, rand() % 256, rand() % 256);

			drawCnt++;
			drawCnt = drawCnt % 5;

			number++;
			if (number > 5)
				number = 5;

		}else if (wParam == 'T' || wParam == 't'){

			////////////////////////////////////////////////////////////////////////////////
			// for������ ���� 0���� number���� �����鼭
			// ������ ������ 0���� ��������Ƿ�, �ش� 0�� �� ������ ã�´�.
			// ã����� �ش� ��ȣ�� �׸��� �뵵�� �����.
			for (int i = 0; i <= number; ++i){
				if(ShapeNum[i] == 0){
					drawCnt = i;
					i=5;
				}
			}
			////////////////////////////////////////////////////////////////////////////////

			temp[drawCnt] = true;
			ShapeNum[drawCnt] = 2;

			point[drawCnt][0].x = rand() % 600 + 50;
			point[drawCnt][0].y = rand() % 500 + 50;
			point[drawCnt][1].x = (rand() % 600 + 50) - (rand() % 30 + 10);
			point[drawCnt][1].y = (rand() % 500 + 50) - (rand() % 30 + 10);
			point[drawCnt][2].x = (rand() % 600 + 50) + (rand() % 30 + 10);
			point[drawCnt][2].y = (rand() % 500 + 50) + (rand() % 30 + 10);
			Color[drawCnt] = RGB(rand() % 256, rand() % 256, rand() % 256);
			drawCnt++;
			drawCnt = drawCnt % 5;
			number++;
			if (number > 5)
				number = 5;


		}else if (wParam == 'R' || wParam == 'r'){

			////////////////////////////////////////////////////////////////////////////////
			// for������ ���� 0���� number���� �����鼭
			// ������ ������ 0���� ��������Ƿ�, �ش� 0�� �� ������ ã�´�.
			// ã����� �ش� ��ȣ�� �׸��� �뵵�� �����.
			for (int i = 0; i <= number; ++i){
				if(ShapeNum[i] == 0){
					drawCnt = i;
					i=5;
				}
			}
			////////////////////////////////////////////////////////////////////////////////

			temp[drawCnt] = true;
			ShapeNum[drawCnt] = 3;

			obj[drawCnt].posX = rand() % 600 + 1;
			obj[drawCnt].posY = rand() % 400 + 1;
			obj[drawCnt].size = rand() % 200 + 50;
			Color[drawCnt] = RGB(rand() % 256, rand() % 256, rand() % 256);
			drawCnt++;
			drawCnt = drawCnt % 5;
			number++;
			if (number > 5)
				number = 5;


		}else if (wParam == '1'){
			SelectNum = 0;
			isSelectNum[0] = true;
			for (int i = 0; i < 5; ++i)
			{
				if (SelectNum != i)
					isSelectNum[i] = false;
			}
		}
		else if (wParam == '2')
		{
			SelectNum = 1;
			isSelectNum[1] = true;
			for (int i = 0; i < 5; ++i)
			{
				if (SelectNum != i)
					isSelectNum[i] = false;
			}

		}
		else if (wParam == '3')
		{
			SelectNum = 2;
			isSelectNum[2] = true;
			for (int i = 0; i < 5; ++i)
			{
				if (SelectNum != i)
					isSelectNum[i] = false;
			}

		}
		else if (wParam == '4')
		{
			SelectNum = 3;
			isSelectNum[3] = true;
			for (int i = 0; i < 5; ++i)
			{
				if (SelectNum != i)
					isSelectNum[i] = false;
			}

		}
		else if (wParam == '5')
		{
			SelectNum = 4;
			isSelectNum[4] = true;
			for (int i = 0; i < 5; ++i)
			{
				if (SelectNum != i)
					isSelectNum[i] = false;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE); // ������ WM_PAINT ����, TRUE :
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/*
<Ű���� ��ɿ� ���� �׸� �׸���>
1. E : ���� ������ ��ġ�� (o)
2. T : �ﰢ��				(o)
3. R : �簢��				(o)
4. ���� ������ ��������	(o)
5. ���� Ű����: �׷��� ������� ������ ���õȴ�(1. ù��° �׷��� ����, 2. �ι�° �׷��� ���� ��)	(o)
- ���õ� ������ ǥ�õȴ�.
6. ȭ��ǥ Ű����: ���õ� ������ ȭ��ǥ�� ���� �����δ�.	(o)
7. Del: ���õ� ������ �����ȴ�.
8. �ִ� 5���� �׸���, 5���� ������ ù ��° ������ �����ǰ� 6��° ������ ��Ÿ����
(ȭ�鿡�� �ִ� 5���� ������ �׷����ִ�)
*/