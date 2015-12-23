
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
}Board;	// 게임 판

typedef struct Block
{
	COLORREF color;
	bool draw_state;	//눈속용 처음에는 true고 판 위에 올라갔을땐 false로
	int    full[5][5];	//1일때 그리기
	RECT rect[5][5];	//5*5의 위치 기억
	bool selected;		//이 도형이 선택 되었는가
}Block;

Board board[12][12]; //12*12의 판
Block block[3];	//3가지 블럭
int isMemoryData[4]; // 3가지 블럭이 실제 BlockData에 뭐가 들어갔는지.
int shape[3];	//3가지 모양
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

void Init_Block3()	//블럭 3개 초기화 함수
{
	for (int i = 0; i < 3; ++i)
	{
		block[i].draw_state = true;
		shape[i] = rand() % 18;
		isMemoryData[i] = shape[i];
		isMemoryData[3] = 19;
		block[i].color = color[shape[i]];
		memcpy(block[i].full, Block_Data[shape[i]], sizeof(block[i].full)); //block[모양n][5][5]에 미리 초기화해뒀던 Block_Data[모양n]을 저장해준다.
	}

	for (int i = 0; i < 3; ++i)		//블럭 3개의 위치(RECT)값 초기화
	{
		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 5; ++x)
			{
				block[i].rect[y][x].left = 600 + 40 * x;						//블럭 3개의 x 위치는 600 
				block[i].rect[y][x].top = i * 200 + 100 + 40 * y;		//블럭 3개의 y 위치  100 + 40*y / 300 + 40*y / 500 + 40*y
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
	static RECT rt = { 0, 0, 1200, 800 };	// 테두리영역을 뺀 흰 영역의 크기를 잡음
	HBRUSH usebrush, oldbrush;


	static bool draw_block[3];
	static int blockup_cnt;
	static int rx, ry; //보드 클릭한곳의 좌표 기억하기
	int answer;
	static int movecnt;	//블럭을 움직인 횟수
	static int timecnt;
	static int boardNum;
	static int selectShape;
	static bool mouseclick;
	static bool boardclick;
	static bool blockclick;
	static char ch[256];
	static int mx, my; //마우스 좌표

	switch (iMessage){
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		srand((unsigned)time(NULL));
		Init_BlockColor();
		Init_Block3();
		Init_Board();
		rx = ry = 0;
		boardclick = blockclick = false;
		selectShape = 30;		//임시로
		boardNum = 1;
		blockup_cnt = movecnt = 0;
		//SetTimer(hWnd, 1, 1000 , NULL); //1000당 1초
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		//======================================================
		//판 그리기
		if (boardNum == 1)		//12*12 판
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
		else if (boardNum == 2)		// ㅁ모양
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
		else if (boardNum == 3)		// ㄱ모양
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
		//3개의 블록 그리기
		for (int i = 0; i < 3; ++i)
		{
			usebrush = CreateSolidBrush(block[i].color);
			oldbrush = (HBRUSH)SelectObject(hdc, usebrush);
			for (int y = 0; y < 5; ++y)
			{
				for (int x = 0; x < 5; ++x)
				{
					//if (block[i].draw_state == false) continue;

					if (block[i].full[y][x] == 1 && block[i].draw_state == true)				// block[모양n][y][x]가 1인 곳만 사각형 그려주기
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
		wsprintf(ch, "1번블럭(클릭) : %d, 2번블럭(클릭) : %d, 3번블럭(클릭) : %d\n 보드클릭 : %d, 블럭클릭: %d \n rx, ry: (%d,%d)\n 현재선택블럭번호: %d\n %d초\n1번 도형 : %d, 2번도형 : %d, 3번도형 : %d, %d"
			, block[0].selected, block[1].selected, block[2].selected, boardclick, blockclick, rx, ry, selectShape + 1, timecnt, isMemoryData[0], isMemoryData[1], isMemoryData[2], isMemoryData[3]);
		DrawText(hdc, ch, -1, &rt, DT_BOTTOM | DT_RIGHT);

		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);


		if (blockclick == false){		//블럭 클릭이 False 일때 보드도 클릭이 안되어 있을경우에는 자동으로 보드 클릭도 False
			boardclick = false;
		}
		//======================================================
		// 3개의 블록 클릭확인 1번째도형일땐 2,3은 안되게..
		for (int i = 0; i < 3; ++i)
		{
			for (int y = 0; y < 5; ++y)
			{
				for (int x = 0; x < 5; ++x)
				{
					RECT rt = block[i].rect[y][x];
					if (block[i].full[y][x] == 1 && rt.left <= mx && mx < rt.right && rt.top <= my && my < rt.bottom)
					{
						blockclick = true;		//이제 블록 클릭했으니 판선택할수 있게끔
						boardclick = false;		//블록을 클릭할경우 보드는 클릭이 안되어 있으므로 false 반환
						selectShape = shape[i];	//몇번 도형 선택했는지 알수 있게
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
					ry = x; rx = y;		// y열 x행을 눌렀는지 y,x값 저장을 해줌
				}
			}
		}

		//======================================================	
		// shapeNum = 실제 도형의 번호, isSelectShapeNum = 화면에 나와있는 3개의 도형중 몇번째 도형인가(1,2,3)
		if (blockclick == true && boardclick == true)		// 무조건 for문을 돌릴수 없으니 블록이 클릭 되어 있을때만
		{
			// 도형을 그리기 이전에 먼저 충돌체크를 확인한다.
			int isRealCheck = 0;

			for (int x = 0; x < 5; ++x)						//블럭.[5][5]랑  판의[5][5]만 비교.   전체와는 비교하지 않는다.
			{
				for (int y = 0; y < 5; ++y)
				{
					if (Block_Data[selectShape][x][y] == true && board[rx + x][ry + y].state == true)
					{		// 해당 블록의 5*5와 실제 판의 데이터를 확인하여 둘다 그려져 있다면 충돌하는것 이므로 isRealCheck를 증가한다.
						isRealCheck++;
					}

					if (Block_Data[selectShape][x][y] == true && ry + y >= 12)
					{		// 배열 초과 부분 잡아주는 방식 블럭 데이터가 살아있는데 그 데이터가 배열을 넘어가게 될경우 isRealCheck를 증가한다.
						isRealCheck++;
					}

					if (Block_Data[selectShape][x][y] == true && rx + x >= 12)
					{		// 배열 초과 부분 잡아주는 방식 블럭 데이터가 살아있는데 그 데이터가 배열을 넘어가게 될경우 isRealCheck를 증가한다.
						isRealCheck++;
					}

				}
			}
			/*---------------------------------------------------------------- 전체 판에서 확인해보잣ㅅㅅㅅ*/
			bool isCheckAllDataBoard = false; // False 일경우 자리가 있다 True 일경우 자리가 없다.

			for (int i = 0; i < 3; ++i){
				// block 데이터 확인
				if (block[i].draw_state == false){
					++i; //이미 그려진 도형을 체크할 필요는 없으므로 i를 ++한다.
				}

				for (int isRx = 0; isRx < 12; ++isRx){
					for (int isRy = 0; isRy < 12; ++isRy){
						//블록 데이터 = i , 실제 클릭한것처럼 확인은 isRx, isRy

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

								// i의 블럭 데이터가 board나 배열이 초과날경우 isAllCheck를 증가시켜서 놓을수 없다고 알려준다.
								// isAllCheck가 0이 아닐경우 그자리는 놓을수 없는자리.
								// 그러므로 isAllCheck가 하나라도 0일경우 그자리는 무조건 놓을수 있는 자리가 된다.


							} // for (int y = 0; y < 5; ++y){
						} // for (int x = 0; x < 5; ++x){

						if (isAllCheck == 0){
							// isAllCheck가 하나라도 0이 있으면 어떤 도형이든 그자리는 들어갈수 있으므로 자리가 없는게 아니다.
							isCheckAllDataBoard = false;
							goto isEscape;
						}else{
							isCheckAllDataBoard = true;
						}// if (isAllCheck == 0){

					} // for (int isRy = 0; isRy < 12; ++isRy){
				} // for (int isRx = 0; isRx < 12; ++isRx){

			} // for (int i = 0; i < 3; ++i){

		

			if (isCheckAllDataBoard == true){
				answer = MessageBox(hWnd, "놓을 곳이 없네요", "알림", MB_OK);
				if (answer == IDOK)
					//PostQuitMessage(0);
					break;
			
		} // if (isCheckAllDataBoard == true){


			/*----------------------------------------------------------------*/
		isEscape:

			if (isRealCheck == 0){			//위에서 충돌체크를 해서 충돌이 안생긴 경우만 그려주게한다
				blockclick = false;
				boardclick = false;
				blockup_cnt++;
				for (int x = 0; x < 5; ++x){
					for (int y = 0; y < 5; ++y){

						if (Block_Data[selectShape][x][y] == true){		//실제 데이터 블럭을 읽어온다. 선택된 도형의 진짜!!도형번호를
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

				//================================================== 줄 다 찼을 경우 없어지게 해준다
				bool isonelinecheck_X = true;
				bool isonelinecheck_Y = true;
				int ischecklineCnt_X = 0;		//한줄에 몇개가 1로 되어있는지 개수 체크용
				int ischecklineCnt_Y = 0;
				int iswhatlineNum_X = 0;	//몇번째 줄인지 줄 번호(가로줄 삭제용)
				int iswhatlineNum_Y = 0;	//몇번째 줄인지 줄 번호(세로줄 삭제용) 

				for (int y = 0; y < 12; ++y)			//가로줄 삭제
				{
					isonelinecheck_X = false;
					for (int x = 0; x < 12; ++x)
					{
						if (board[y][x].state == 1)
						{
							ischecklineCnt_X++;
						}
					}
					//한줄을 싹 검사해보고 난 후에
					if (ischecklineCnt_X == 12)	//그 줄이 꽉찼으면
					{
						iswhatlineNum_X = y; //그 라인이 몇번째 줄인가를 알려준다.
						isonelinecheck_X = true;
					}

					if (isonelinecheck_X)
					{
						for (int x = 0; x < 12; ++x)	// 그 줄을 싹 지워버리기 위해서 만든 for문
						{
							board[iswhatlineNum_X][x].state = 0;
						}
					}
					ischecklineCnt_X = 0; //한줄 검사끝났으니까 다음 줄 검사하기 위해서 	
				}
				/*-----------------------------------------------------------------*/
				for (int y = 0; y < 12; ++y)			//세로줄 삭제
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
				if (blockup_cnt > 2)		//블럭 3개를 다 올려줬을경우 새로운 블럭3개를 꺼내준다
				{
					Init_Block3();
					blockup_cnt = 0;
				}
			}
			//movecnt++;	//블럭을 한번 옮겼다!
			//if (movecnt >= MAX_MOVE)	//만약 블럭 옮긴 횟수가 n번을 넘어간다면
			//{
			//	answer = MessageBox(hWnd, "이동횟수초과", "알림", MB_OK);
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
			if (timecnt > MAX_TIME)	//5초가 지났을때
			{
				KillTimer(hWnd, 1);
				answer = MessageBox(hWnd, "TIME OVER", "알림", MB_OK);
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