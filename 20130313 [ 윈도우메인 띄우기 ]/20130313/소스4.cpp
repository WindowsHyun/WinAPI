
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define MAX_TIME 5
#define MAX_MOVE 10
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Hello World");

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		0, 0, 1200, 800,
		NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, NULL, 0, 0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

int Block_Data[18][5][5] =
{
	{//1
		{ 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//2
		{ 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//3
		{ 1, 1, 1, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//4
		{ 1, 1, 1, 1, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//5
		{ 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//6
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//7
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//8
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//9
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
	},
	{//10
		{ 1, 1, 0, 0, 0 },
		{ 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//11
		{ 1, 1, 1, 0, 0 },
		{ 1, 1, 1, 0, 0 },
		{ 1, 1, 1, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//12
		{ 1, 0, 0, 0, 0 },
		{ 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//13
		{ 1, 1, 0, 0, 0 },
		{ 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//14
		{ 1, 1, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//15
		{ 1, 1, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//16
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 1, 1, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//17
		{ 1, 1, 1, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{//18
		{ 1, 1, 0, 0, 0 },
		{ 0, 1, 1, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	}
};

typedef struct Board
{
	int bx;
	int by;
	int state;
	COLORREF color;
}Board;	// ���� ��

typedef struct Block
{
	COLORREF color;
	bool draw_state;	//���ӿ� ó������ true�� �� ���� �ö����� false��
	int    full[5][5];	//1�϶� �׸���
	RECT rect[5][5];	//5*5�� ��ġ ���
	bool selected;		//�� ������ ���� �Ǿ��°�
}Block;

Board board[12][12]; //12*12�� ��
Block block[3];	//3���� ��
int isMemoryData[4]; // 3���� ���� ���� BlockData�� ���� ������.
int shape[3];	//3���� ���
COLORREF color[18];

void Init_BlockColor()
{
	color[0] = RGB(243, 75, 103);
	color[1] = RGB(174, 75, 243);
	color[2] = RGB(236, 153, 188);
	color[3] = RGB(153, 213, 236);
	color[4] = RGB(153, 236, 182);
	color[5] = RGB(75, 217, 243);
	color[6] = RGB(75, 126, 243);
	color[7] = RGB(243, 75, 229);
	color[8] = RGB(184, 153, 236);
	color[9] = RGB(75, 243, 142);
	color[10] = RGB(243, 146, 75);
	color[11] = RGB(241, 243, 75);
	color[12] = RGB(236, 164, 153);
	color[13] = RGB(178, 106, 106);
	color[14] = RGB(223, 236, 153);
	color[15] = RGB(194, 243, 75);
	color[16] = RGB(243, 182, 75);
	color[17] = RGB(236, 211, 153);
}

void Init_Block3()	//�� 3�� �ʱ�ȭ �Լ�
{
	for (int i = 0; i < 3; ++i)
	{
		block[i].draw_state = true;
		shape[i] = rand() % 18;
		isMemoryData[i] = shape[i];
		isMemoryData[3] = 19;
		block[i].color = color[shape[i]];
		memcpy(block[i].full, Block_Data[shape[i]], sizeof(block[i].full)); //block[���n][5][5]�� �̸� �ʱ�ȭ�ص״� Block_Data[���n]�� �������ش�.
	}

	for (int i = 0; i < 3; ++i)		//�� 3���� ��ġ(RECT)�� �ʱ�ȭ
	{
		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 5; ++x)
			{
				block[i].rect[y][x].left = 600 + 40 * x;						//�� 3���� x ��ġ�� 600 
				block[i].rect[y][x].top = i * 200 + 100 + 40 * y;		//�� 3���� y ��ġ  100 + 40*y / 300 + 40*y / 500 + 40*y
				block[i].rect[y][x].right = block[i].rect[y][x].left + 40;
				block[i].rect[y][x].bottom = block[i].rect[y][x].top + 40;
			}
		}
	}
}

void Init_Board()
{
	for (int y = 0; y < 12; ++y)
	{
		for (int x = 0; x < 12; ++x)
		{
			board[y][x].bx = x * 40;
			board[y][x].by = y * 40;
			board[y][x].state = false;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rt = { 0, 0, 1200, 800 };	// �׵θ������� �� �� ������ ũ�⸦ ����
	HBRUSH usebrush, oldbrush;


	static bool draw_block[3];
	static int blockup_cnt;
	static int rx, ry; //���� Ŭ���Ѱ��� ��ǥ ����ϱ�
	int answer;
	static int movecnt;	//���� ������ Ƚ��
	static int timecnt;
	static int boardNum;
	static int selectShape;
	static bool mouseclick;
	static bool boardclick;
	static bool blockclick;
	static char ch[256];
	static int mx, my; //���콺 ��ǥ

	switch (iMessage){
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		srand((unsigned)time(NULL));
		Init_BlockColor();
		Init_Block3();
		Init_Board();
		rx = ry = 0;
		boardclick = blockclick = false;
		selectShape = 30;		//�ӽ÷�
		boardNum = 1;
		blockup_cnt = movecnt = 0;
		//SetTimer(hWnd, 1, 1000 , NULL); //1000�� 1��
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		//======================================================
		//�� �׸���
		if (boardNum == 1)		//12*12 ��
		{
			for (int y = 0; y < 12; ++y)
			{
				for (int x = 0; x < 12; ++x)
				{
					if (board[y][x].state == true)
					{
						usebrush = CreateSolidBrush(board[y][x].color);
						oldbrush = (HBRUSH)SelectObject(hdc, usebrush);
						Rectangle(hdc, board[y][x].bx, board[y][x].by, board[y][x].bx + 40, board[y][x].by + 40);
						SelectObject(hdc, oldbrush);
						DeleteObject(usebrush);
					}
					else
					{
						usebrush = CreateSolidBrush(RGB(255, 255, 255));
						oldbrush = (HBRUSH)SelectObject(hdc, usebrush);
						Rectangle(hdc, board[y][x].bx, board[y][x].by, board[y][x].bx + 40, board[y][x].by + 40);
						SelectObject(hdc, oldbrush);
						DeleteObject(usebrush);
					}
				}

			}
		}
		else if (boardNum == 2)		// �����
		{
			for (int y = 0; y < 12; ++y)
			{
				for (int x = 0; x < 12; ++x)
				{
					if (board[y][x].state == true && !(x>2 && x< 9 && y>2 && y<9))
					{
						usebrush = CreateSolidBrush(board[y][x].color);
						oldbrush = (HBRUSH)SelectObject(hdc, usebrush);
						Rectangle(hdc, board[y][x].bx, board[y][x].by, board[y][x].bx + 40, board[y][x].by + 40);
						SelectObject(hdc, oldbrush);
						DeleteObject(usebrush);
					}
					else if (board[y][x].state == false && !(x>2 && x< 9 && y>2 && y < 9))
					{
						usebrush = CreateSolidBrush(RGB(255, 255, 255));
						oldbrush = (HBRUSH)SelectObject(hdc, usebrush);
						Rectangle(hdc, board[y][x].bx, board[y][x].by, board[y][x].bx + 40, board[y][x].by + 40);
						SelectObject(hdc, oldbrush);
						DeleteObject(usebrush);
					}
				}
			}
		}
		else if (boardNum == 3)		// �����
		{
			for (int y = 0; y < 12; ++y)
			{
				for (int x = 0; x < 12; ++x)
				{
					if (board[y][x].state == true && !(x >= 0 && x<8 && y>3 && y <= 11))
					{
						usebrush = CreateSolidBrush(board[y][x].color);
						oldbrush = (HBRUSH)SelectObject(hdc, usebrush);
						Rectangle(hdc, board[y][x].bx, board[y][x].by, board[y][x].bx + 40, board[y][x].by + 40);
					}
					else if (board[y][x].state == false && !(x >= 0 && x<8 && y>3 && y <= 11))
					{
						usebrush = CreateSolidBrush(RGB(255, 255, 255));
						oldbrush = (HBRUSH)SelectObject(hdc, usebrush);
						Rectangle(hdc, board[y][x].bx, board[y][x].by, board[y][x].bx + 40, board[y][x].by + 40);
					}
				}
				SelectObject(hdc, oldbrush);
				DeleteObject(usebrush);
			}
		}
		//======================================================

		//======================================================
		//3���� ��� �׸���
		for (int i = 0; i < 3; ++i)
		{
			usebrush = CreateSolidBrush(block[i].color);
			oldbrush = (HBRUSH)SelectObject(hdc, usebrush);
			for (int y = 0; y < 5; ++y)
			{
				for (int x = 0; x < 5; ++x)
				{
					//if (block[i].draw_state == false) continue;

					if (block[i].full[y][x] == 1 && block[i].draw_state == true)				// block[���n][y][x]�� 1�� ���� �簢�� �׷��ֱ�
					{
						RECT rt = block[i].rect[y][x];
						Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
					}
				}
			}
			DeleteObject(SelectObject(hdc, oldbrush));
		}
		//======================================================


		//====================================================
		wsprintf(ch, "1����(Ŭ��) : %d, 2����(Ŭ��) : %d, 3����(Ŭ��) : %d\n ����Ŭ�� : %d, ��Ŭ��: %d \n rx, ry: (%d,%d)\n ���缱�ú���ȣ: %d\n %d��\n1�� ���� : %d, 2������ : %d, 3������ : %d, %d"
			, block[0].selected, block[1].selected, block[2].selected, boardclick, blockclick, rx, ry, selectShape + 1, timecnt, isMemoryData[0], isMemoryData[1], isMemoryData[2], isMemoryData[3]);
		DrawText(hdc, ch, -1, &rt, DT_BOTTOM | DT_RIGHT);

		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);


		if (blockclick == false){		//�� Ŭ���� False �϶� ���嵵 Ŭ���� �ȵǾ� ������쿡�� �ڵ����� ���� Ŭ���� False
			boardclick = false;
		}
		//======================================================
		// 3���� ��� Ŭ��Ȯ�� 1��°�����϶� 2,3�� �ȵǰ�..
		for (int i = 0; i < 3; ++i)
		{
			for (int y = 0; y < 5; ++y)
			{
				for (int x = 0; x < 5; ++x)
				{
					RECT rt = block[i].rect[y][x];
					if (block[i].full[y][x] == 1 && rt.left <= mx && mx < rt.right && rt.top <= my && my < rt.bottom)
					{
						blockclick = true;		//���� ��� Ŭ�������� �Ǽ����Ҽ� �ְԲ�
						boardclick = false;		//����� Ŭ���Ұ�� ����� Ŭ���� �ȵǾ� �����Ƿ� false ��ȯ
						selectShape = shape[i];	//��� ���� �����ߴ��� �˼� �ְ�
						block[i].selected = true;
						if (i == 0)
						{
							block[1].selected = false;
							block[2].selected = false;
						}
						if (i == 1)
						{
							block[0].selected = false;
							block[2].selected = false;
						}
						if (i == 2)
						{
							block[0].selected = false;
							block[1].selected = false;
						}
					}
				}
			}
		}

		//======================================================
		for (int y = 0; y < 12; ++y)
		{
			for (int x = 0; x < 12; ++x)
			{
				if (board[y][x].bx < mx && mx < board[y][x].bx + 40 &&
					board[y][x].by < my && my < board[y][x].by + 40)
				{
					if (blockclick == true){
						blockclick = true;
					}
					boardclick = true;
					ry = x; rx = y;		// y�� x���� �������� y,x�� ������ ����
				}
			}
		}

		//======================================================	
		// shapeNum = ���� ������ ��ȣ, isSelectShapeNum = ȭ�鿡 �����ִ� 3���� ������ ���° �����ΰ�(1,2,3)
		if (blockclick == true && boardclick == true)		// ������ for���� ������ ������ ����� Ŭ�� �Ǿ� ��������
		{
			// ������ �׸��� ������ ���� �浹üũ�� Ȯ���Ѵ�.
			int isRealCheck = 0;

			for (int x = 0; x < 5; ++x)						//��.[5][5]��  ����[5][5]�� ��.   ��ü�ʹ� ������ �ʴ´�.
			{
				for (int y = 0; y < 5; ++y)
				{
					if (Block_Data[selectShape][x][y] == true && board[rx + x][ry + y].state == true)
					{		// �ش� ����� 5*5�� ���� ���� �����͸� Ȯ���Ͽ� �Ѵ� �׷��� �ִٸ� �浹�ϴ°� �̹Ƿ� isRealCheck�� �����Ѵ�.
						isRealCheck++;
					}

					if (Block_Data[selectShape][x][y] == true && ry + y >= 12)
					{		// �迭 �ʰ� �κ� ����ִ� ��� �� �����Ͱ� ����ִµ� �� �����Ͱ� �迭�� �Ѿ�� �ɰ�� isRealCheck�� �����Ѵ�.
						isRealCheck++;
					}

					if (Block_Data[selectShape][x][y] == true && rx + x >= 12)
					{		// �迭 �ʰ� �κ� ����ִ� ��� �� �����Ͱ� ����ִµ� �� �����Ͱ� �迭�� �Ѿ�� �ɰ�� isRealCheck�� �����Ѵ�.
						isRealCheck++;
					}

				}
			}
			/*---------------------------------------------------------------- ��ü �ǿ��� Ȯ���غ��㤵����*/
			bool isCheckAllDataBoard = false; // False �ϰ�� �ڸ��� �ִ� True �ϰ�� �ڸ��� ����.

			for (int i = 0; i < 3; ++i){
				// block ������ Ȯ��
				if (block[i].draw_state == false){
					++i; //�̹� �׷��� ������ üũ�� �ʿ�� �����Ƿ� i�� ++�Ѵ�.
				}

				for (int isRx = 0; isRx < 12; ++isRx){
					for (int isRy = 0; isRy < 12; ++isRy){
						//��� ������ = i , ���� Ŭ���Ѱ�ó�� Ȯ���� isRx, isRy

						int isAllCheck = 0;

						for (int x = 0; x < 5; ++x){
							for (int y = 0; y < 5; ++y){

								if (Block_Data[isMemoryData[i]][x][y] == true && board[isRx + x][isRy + y].state == true){
									isAllCheck++;
								}

								if (Block_Data[isMemoryData[i]][x][y] == true && isRy + y >= 12){
									isAllCheck++;
								}

								if (Block_Data[isMemoryData[i]][x][y] == true && isRx + x >= 12){
									isAllCheck++;
								}

								// i�� �� �����Ͱ� board�� �迭�� �ʰ������ isAllCheck�� �������Ѽ� ������ ���ٰ� �˷��ش�.
								// isAllCheck�� 0�� �ƴҰ�� ���ڸ��� ������ �����ڸ�.
								// �׷��Ƿ� isAllCheck�� �ϳ��� 0�ϰ�� ���ڸ��� ������ ������ �ִ� �ڸ��� �ȴ�.


							} // for (int y = 0; y < 5; ++y){
						} // for (int x = 0; x < 5; ++x){

						if (isAllCheck == 0){
							// isAllCheck�� �ϳ��� 0�� ������ � �����̵� ���ڸ��� ���� �����Ƿ� �ڸ��� ���°� �ƴϴ�.
							isCheckAllDataBoard = false;
							goto isEscape;
						}else{
							isCheckAllDataBoard = true;
						}// if (isAllCheck == 0){

					} // for (int isRy = 0; isRy < 12; ++isRy){
				} // for (int isRx = 0; isRx < 12; ++isRx){

			} // for (int i = 0; i < 3; ++i){

		

			if (isCheckAllDataBoard == true){
				answer = MessageBox(hWnd, "���� ���� ���׿�", "�˸�", MB_OK);
				if (answer == IDOK)
					//PostQuitMessage(0);
					break;
			
		} // if (isCheckAllDataBoard == true){


			/*----------------------------------------------------------------*/
		isEscape:

			if (isRealCheck == 0){			//������ �浹üũ�� �ؼ� �浹�� �Ȼ��� ��츸 �׷��ְ��Ѵ�
				blockclick = false;
				boardclick = false;
				blockup_cnt++;
				for (int x = 0; x < 5; ++x){
					for (int y = 0; y < 5; ++y){

						if (Block_Data[selectShape][x][y] == true){		//���� ������ ���� �о�´�. ���õ� ������ ��¥!!������ȣ��
							int isSelectShapeNum;
							for (int i = 0; i < 3; ++i){
								if (block[i].selected == true){
									isSelectShapeNum = i;
								}
							}
							board[rx + x][ry + y].state = true;
							board[rx + x][ry + y].color = block[isSelectShapeNum].color;
							block[isSelectShapeNum].draw_state = false;
						}
					}
				}

				//================================================== �� �� á�� ��� �������� ���ش�
				bool isonelinecheck_X = true;
				bool isonelinecheck_Y = true;
				int ischecklineCnt_X = 0;		//���ٿ� ��� 1�� �Ǿ��ִ��� ���� üũ��
				int ischecklineCnt_Y = 0;
				int iswhatlineNum_X = 0;	//���° ������ �� ��ȣ(������ ������)
				int iswhatlineNum_Y = 0;	//���° ������ �� ��ȣ(������ ������) 

				for (int y = 0; y < 12; ++y)			//������ ����
				{
					isonelinecheck_X = false;
					for (int x = 0; x < 12; ++x)
					{
						if (board[y][x].state == 1)
						{
							ischecklineCnt_X++;
						}
					}
					//������ �� �˻��غ��� �� �Ŀ�
					if (ischecklineCnt_X == 12)	//�� ���� ��á����
					{
						iswhatlineNum_X = y; //�� ������ ���° ���ΰ��� �˷��ش�.
						isonelinecheck_X = true;
					}

					if (isonelinecheck_X)
					{
						for (int x = 0; x < 12; ++x)	// �� ���� �� ���������� ���ؼ� ���� for��
						{
							board[iswhatlineNum_X][x].state = 0;
						}
					}
					ischecklineCnt_X = 0; //���� �˻糡�����ϱ� ���� �� �˻��ϱ� ���ؼ� 	
				}
				/*-----------------------------------------------------------------*/
				for (int y = 0; y < 12; ++y)			//������ ����
				{
					isonelinecheck_Y = false;
					for (int x = 0; x < 12; ++x)
					{
						if (board[x][y].state == 1)
						{
							ischecklineCnt_Y++;
						}
					}
					if (ischecklineCnt_Y == 12)
					{
						iswhatlineNum_Y = y;
						isonelinecheck_Y = true;
					}
					if (isonelinecheck_Y)
					{
						for (int x = 0; x < 12; ++x)
						{
							board[x][iswhatlineNum_Y].state = 0;
						}
					}
					ischecklineCnt_Y = 0;
				}
				//==================================================
				if (blockup_cnt > 2)		//�� 3���� �� �÷�������� ���ο� ��3���� �����ش�
				{
					Init_Block3();
					blockup_cnt = 0;
				}
			}
			//movecnt++;	//���� �ѹ� �Ű��!
			//if (movecnt >= MAX_MOVE)	//���� �� �ű� Ƚ���� n���� �Ѿ�ٸ�
			//{
			//	answer = MessageBox(hWnd, "�̵�Ƚ���ʰ�", "�˸�", MB_OK);
			//	if (answer == IDOK)
			//		PostQuitMessage(0);
			//}
		}
		//=====================================================
		InvalidateRgn(hWnd, NULL, TRUE);
		break;


	case WM_LBUTTONUP:
		//Init_Block3();
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'c':case 'C':
			for (int i = 0; i < 12; ++i){
				for (int j = 0; j < 11; ++j){
					board[i][j].state = true;
					board[i][j].color = RGB(153, 236, 182);
				}
			}



			for (int i = 9; i < 12; ++i){
				for (int j = 9; j < 12; ++j){
					board[i][j].state = false;
					board[i][j].color = RGB(255, 255, 255);
				}
			}

			for (int i = 0; i < 12; ++i){
				board[7][i].state = false;
				board[7][i].color = RGB(255, 255, 255);
			}

			break;

			//
		case 'q':case 'Q':
			Init_Board();
			boardNum = 1;
			break;
		case 'w':case 'W':
			Init_Board();
			for (int y = 0; y < 12; ++y)
			{
				for (int x = 0; x < 12; ++x)
				{
					if (2 < x && x < 9 && 2 < y && y < 9)
						board[y][x].state = 2;
				}
			}
			boardNum = 2;
			break;
		case 'e':case 'E':
			Init_Board();
			for (int y = 0; y < 12; ++y)
			{
				for (int x = 0; x < 12; ++x)
				{
					if (0 <= x && x < 8 && 3 < y && y <= 11)
						board[y][x].state = 2;
				}
			}
			boardNum = 3;
			break;
		case 'v':case 'V':
			Init_Block3();
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			++timecnt;
			if (timecnt > MAX_TIME)	//5�ʰ� ��������
			{
				KillTimer(hWnd, 1);
				answer = MessageBox(hWnd, "TIME OVER", "�˸�", MB_OK);
				if (answer == IDOK)
					PostQuitMessage(0);
				timecnt = 0;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	}
	return(0);
}