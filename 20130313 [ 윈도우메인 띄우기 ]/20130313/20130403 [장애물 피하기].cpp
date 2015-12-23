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
	//int cnt; //몇번째 도형인가
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
	static obj obj[5];	// 도형 5개, 0~4
	static bool isSelectNum[5]; //몇번째 도형이 눌려지고 있는가. 눌린 번호 제외 모두 false
	static int	SelectNum;		   //몇번째 도형을 선택했나 받아오기. 
	static int	drawCnt;  //도형을 몇개 그렸나.
	static int	ShapeNum[5]; //그리는 도형이 뭔가요
	static int	number;	  //몇개그렸는지를 이용해 for문돌리려고 만든 변수
	static bool  temp[5];			//임시로 잠깐 그릴껀가 안그릴껀가
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
			if (temp[i])	//번호 선택하지 않은 것만 그려주기!!!
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
			if (isSelectNum[i])	//번호선택했을때 그려주는거(그거 하나만)
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

			ShapeNum[SelectNum]  = 0; // 삭제된 도형을 1,2,3 이 아닌 0으로 만들어 버린다.

			//drawCnt = SelectNum; // 삭제된 도형부터 만들어야 하니깐 drawcnt를 선택한 번호로 설정.
			//drawCnt--;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_CHAR:
		if (wParam == 'E' || wParam == 'e'){		//800,600

			////////////////////////////////////////////////////////////////////////////////
			// for문으로 도형 0부터 number까지 돌리면서
			// 삭제시 도형을 0으로 만들었으므로, 해당 0이 된 도형을 찾는다.
			// 찾을경우 해당 번호를 그리는 용도로 만든다.
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
			// for문으로 도형 0부터 number까지 돌리면서
			// 삭제시 도형을 0으로 만들었으므로, 해당 0이 된 도형을 찾는다.
			// 찾을경우 해당 번호를 그리는 용도로 만든다.
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
			// for문으로 도형 0부터 number까지 돌리면서
			// 삭제시 도형을 0으로 만들었으므로, 해당 0이 된 도형을 찾는다.
			// 찾을경우 해당 번호를 그리는 용도로 만든다.
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
		InvalidateRect(hWnd, NULL, TRUE); // 강제로 WM_PAINT 실행, TRUE :
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/*
<키보드 명령에 따라 그림 그리기>
1. E : 원을 임의의 위치에 (o)
2. T : 삼각형				(o)
3. R : 사각형				(o)
4. 색은 랜덤한 색상으로	(o)
5. 숫자 키보드: 그려진 순서대로 도형이 선택된다(1. 첫번째 그려진 도형, 2. 두번째 그려진 도형 등)	(o)
- 선택된 도형은 표시된다.
6. 화살표 키보드: 선택된 도형이 화살표에 따라 움직인다.	(o)
7. Del: 선택된 도형이 삭제된다.
8. 최대 5개를 그리고, 5개가 넘으면 첫 번째 도형이 삭제되고 6번째 도형이 나타난다
(화면에는 최대 5개의 도형이 그려져있다)
*/