#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")									// TransparentBlt 사용하기 위해 라이브러리 호출..


#define isGameWidth 4														// 가로
#define isGameLength 4														// 세로
#define isGameTotalData 2048												// 게임 마지막 숫자

int isGameTotal = 2048;														// 게임 마지막 숫자
int isGameData[isGameWidth][isGameLength];						// 게임판 설정하기
int isGameDataCopy[isGameWidth][isGameLength];				// 게임판 복사하기
int isWhatKey;																		// 4 = Left, 8 = UP, 6 = Right, 2 = Down
int isTempNum;																	// 그냥 swap 임시 변수
int isRealMove = 0;																// 실제로 움직였는지 확인 ( 0 = 안움직임, 1 = 움직임 )
int isScore = 0;																		// 실제 점수
int isGamePause = 0;															// 게임 일시정지 ( 0 = 진행, 1 = 정지 )
int isLastScore = 0;																// 게임 정지후 최종 점수
int isMx, isMy;																		// 제스처 사용을 위한 마우스 좌표
int  isClickX, isClickY, isTempX, isTempY;								// 제처처 사용을 위한 위치 좌표
int isTargetScore = 2048, isHighNum = 2;								// 목표 점수는 2048, 현재 최대 점수는 처음이니깐 2
int isNewGameAnswer;															// 메시지 박스 확인취소 체크하는 부분

typedef struct isGameDataXY{
	int bx;
	int by;
}isGameDataXY;																	// 게임 위치 저장


bool isGameTheEnd();															// False는 Game Not End, True 는 Game End

void is4CheckData(int, HWND);												// 방향 체크후 동작 & HWND추가 이유는 isTheEndGame 에서 hwnd를 사용하기위해
void isRandNumber();															// 랜덤으로 숫자 뿌려주기
void isCopyMapData();															// 맵 변동이 되었는지 확인하기 위한 복사
void isCompareData();															// 맵 변동이 되었는지 확인하는 체크 툴
void isMapData(HDC, HDC, HBITMAP, int, int);						// 배열의 숫자를 맵으로 그려주는 부분
void isNewGame();																// 새로운 게임을 시작할때 호출하는 함수
void isTheEndGame(HWND);															// 목표점수에 도달했는지 확인후, 도달했을경우 메시지창 띄우기

// -----------------------------------------------------------------------------------------------------------
// 위, 아래, 왼쪽, 오른쪽 숫자 움직이기
void isDownPlusNumber(int);
void isDownNumber(int);
void isUPPlusNumber(int);
void isUPNumber(int);
void isLeftPlusNumber(int);
void isLeftNumber(int);
void isRightPlusNumber(int);
void isRightNumber(int);
// -----------------------------------------------------------------------------------------------------------


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
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name", "WindowsHyun 2048", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam){
	static HDC hdc, mem0dc, mem1dc;
	PAINTSTRUCT ps;
	static HBITMAP isNumber[isGameTotalData];					// 2048개의 배열을 만드는 메모리 낭비중ㅋㅋㅋ
	static HBITMAP hbmOld, hbmMem, hbmMemOld;			// 더블버퍼링을 위하여!
	static char isDebugData[500];
	static int isXdata, isYdata;
	static isGameDataXY isGameDataXY[isGameWidth][isGameLength];
	static int isTempHighNum = 2;
	static RECT rt;
	switch (iMsg){




	case WM_COMMAND:
		srand((unsigned int)time(NULL));
		switch (LOWORD(wParam)) {

		case ID_GAME_NEWGAME:
			isNewGameAnswer = MessageBox(hwnd, "정말로 게임을 다시 시작하실껀가요?", "WindowsHyun 2048", MB_OKCANCEL);

			if (isNewGameAnswer == IDOK){
				isNewGame();
			}
			break;

		case ID_TARGETSCORE_256:
			isTargetScore = 256;
			break;

		case ID_TARGETSCORE_512:
			isTargetScore = 512;
			break;

		case ID_TARGETSCORE_1024:
			isTargetScore = 1024;
			break;

		case ID_TARGETSCORE_2048:
			isTargetScore = 2048;
			break;

		case ID_GAME_EXITGAME:
			isNewGameAnswer = MessageBox(hwnd, "정말로 게임을 종료하실껀가요?", "WindowsHyun 2048", MB_OKCANCEL);

			if (isNewGameAnswer == IDOK){
				PostQuitMessage(0);
			}
			break;


		}
		InvalidateRgn(hwnd, NULL, FALSE);
		break;


	case WM_KEYDOWN:
		if (wParam == VK_RIGHT){
			isWhatKey = 6, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (wParam == VK_LEFT){
			isWhatKey = 4, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (wParam == VK_UP){
			isWhatKey = 8, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (wParam == VK_DOWN){
			isWhatKey = 2, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (wParam == 'r' || wParam == 'R'){
			isGameData[0][0] = 2;		// [x] [y]
			isGameData[1][0] = 4;		// [x] [y]
			isGameData[2][0] = 8;		// [x] [y]
			isGameData[3][0] = 4;		// [x] [y]

			isGameData[0][1] = 4;		// [x] [y]
			isGameData[1][1] = 16;		// [x] [y]
			isGameData[2][1] = 2;		// [x] [y]
			isGameData[3][1] = 16;		// [x] [y]

			isGameData[0][2] = 2;		// [x] [y]
			isGameData[1][2] = 4;		// [x] [y]
			isGameData[2][2] = 16;		// [x] [y]
			isGameData[3][2] = 8;		// [x] [y]

			isGameData[0][3] = 2;		// [x] [y]
			isGameData[1][3] = 32;		// [x] [y]
			isGameData[2][3] = 4;		// [x] [y]
			isGameData[3][3] = 2;		// [x] [y]
		}
		else if (wParam == 't' || wParam == 'T'){
			isGameData[0][0] = 2;		// [x] [y]
			isGameData[1][0] = 4;		// [x] [y]
			isGameData[2][0] = 8;		// [x] [y]
			isGameData[3][0] = 4;		// [x] [y]

			isGameData[0][1] = 8;		// [x] [y]
			isGameData[1][1] = 16;		// [x] [y]
			isGameData[2][1] = 2;		// [x] [y]
			isGameData[3][1] = 16;		// [x] [y]

			isGameData[0][2] = 4;		// [x] [y]
			isGameData[1][2] = 8;		// [x] [y]
			isGameData[2][2] = 16;		// [x] [y]
			isGameData[3][2] = 8;		// [x] [y]

			isGameData[0][3] = 2;		// [x] [y]
			isGameData[1][3] = 32;		// [x] [y]
			isGameData[2][3] = 4;		// [x] [y]
			isGameData[3][3] = 2;		// [x] [y]
		}
		InvalidateRgn(hwnd, NULL, FALSE);
		break;

	case WM_LBUTTONUP:
		isMx = LOWORD(lParam);
		isMy = HIWORD(lParam);
		//여기서 계산을 해야함

		for (int y = 0; y < isGameLength; ++y){
			for (int x = 0; x < isGameWidth; ++x){
				if (isGameDataXY[y][x].bx < isMx && isMx < 10 + isGameDataXY[y][x].bx + 130 &&
					isGameDataXY[y][x].by < isMy && isMy <10 + isGameDataXY[y][x].by + 130){
					isClickY = y; isClickX = x;								// X, Y 좌표를 저장해준다.
				}
			}
		}

		if (isTempX > isClickX){
			//왼쪽
			isWhatKey = 4, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (isTempX < isClickX){
			//오른쪽
			isWhatKey = 6, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (isTempY > isClickY){
			//위
			isWhatKey = 8, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (isTempY < isClickY){
			//아래
			isWhatKey = 2, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}

		InvalidateRgn(hwnd, NULL, FALSE);
		break;


	case WM_MOUSEMOVE:
		isMx = LOWORD(lParam);
		isMy = HIWORD(lParam);

		for (int y = 0; y < isGameLength; ++y){
			for (int x = 0; x < isGameWidth; ++x){
				if (isGameDataXY[y][x].bx < isMx && isMx < 10 + isGameDataXY[y][x].bx + 130 &&
					isGameDataXY[y][x].by < isMy && isMy < 10 + isGameDataXY[y][x].by + 130){
					isClickY = y; isClickX = x;								// X, Y 좌표를 저장해준다.
				}
				else if (isMx < 10 || isMy > 520 || isMx >520){
					// 판 범위를 벗어나면 둘다 0으로
					isClickY = 0; isClickX = 0;
					isTempX = 0, isTempY = 0;
				}
			}
		}

		InvalidateRgn(hwnd, NULL, FALSE);
		break;

	case WM_LBUTTONDOWN:
		isMx = LOWORD(lParam);
		isMy = HIWORD(lParam);

		for (int y = 0; y < isGameLength; ++y){
			for (int x = 0; x < isGameWidth; ++x){


				if (isGameDataXY[y][x].bx < isMx && isMx < 10 + isGameDataXY[y][x].bx + 130 &&
					isGameDataXY[y][x].by < isMy && isMy < 10 + isGameDataXY[y][x].by + 130){
					isClickY = y; isClickX = x;								// X, Y 좌표를 저장해준다.
					isTempX = x, isTempY = y;							// 마우스가 움직이면 자동으로 X,Y 좌표도 바뀐다 그러므로 임시 위치로 저장
				}



			}
		}


		InvalidateRgn(hwnd, NULL, FALSE);
		break;


	case WM_CREATE:
		srand((unsigned int)time(NULL) + GetTickCount());

		GetClientRect(hwnd, &rt);

		isNumber[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));					// 숫자 없음
		isNumber[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2));					// 숫자 2
		isNumber[4] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP3));					// 숫자 4
		isNumber[8] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP4));					// 숫자 8
		isNumber[16] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP5));					// 숫자 16
		isNumber[32] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP6));					// 숫자 32
		isNumber[64] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP7));					// 숫자 64
		isNumber[128] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP8));				// 숫자 128
		isNumber[256] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP9));				// 숫자 256
		isNumber[512] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP10));				// 숫자 512
		isNumber[1024] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP11));			// 숫자 1024
		isNumber[2048] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP12));			// 숫자 2048


		isWhatKey = 0;		// 4 = Left, 8 = UP, 6 = Right, 2 = Down

		for (int i = 0; i < 2; ++i){
			isRandNumber(); //초기 블럭 2개 설정!
		}


		break;

	case WM_PAINT:
		srand((unsigned int)time(NULL));

		hdc = BeginPaint(hwnd, &ps);

		mem0dc = CreateCompatibleDC(hdc);//2
		hbmMem = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);//3
		SelectObject(mem0dc, hbmMem);
		PatBlt(mem0dc, rt.left, rt.top, rt.right, rt.bottom, WHITENESS);
		hbmMemOld = (HBITMAP)SelectObject(mem0dc, hbmMem);//4
		mem1dc = CreateCompatibleDC(mem0dc);//5



		for (int i = 0; i < isGameLength; ++i){
			for (int j = 0; j < isGameWidth; ++j){
				//isGameDataXY

				isGameDataXY[i][j].bx = 10 + j * 130;
				isGameDataXY[i][j].by = 10 + i * 130;

				isMapData(mem0dc, mem1dc, isNumber[isGameData[i][j]], 10 + (i * 130), 10 + (j * 130));
			}
		}
		//-----------------------------------------------------------------------------------------------------------------------------------------------
		// 최고 점수를 찾아라!
		isTempHighNum = 0;
		for (int i = 0; i < isGameLength; ++i){
			for (int j = 0; j < isGameWidth; ++j){
				if (isTempHighNum <= isGameData[i][j]){
					isTempHighNum = isGameData[i][j];
				}
			}
		}
		isHighNum = isTempHighNum;

		//-----------------------------------------------------------------------------------------------------------------------------------------------
		// 게임 데이터를 텍스트로 변환
		sprintf(isDebugData, "WhatKey : %d, isRealMove : %d, isScore = %d, isLastScore = %d, isMx = %d, isMy = %d, 현재 위치 좌표 = (%d, %d), 이전 위치 좌표 = (%d, %d)",
			isWhatKey, isRealMove, isScore, isLastScore, isMx, isMy, isClickX, isClickY, isTempX, isTempY);
		TextOut(mem0dc, 0, 530, isDebugData, strlen(isDebugData));

		sprintf(isDebugData, "목표 점수 : %d, 현재 최고 높은 숫자 : %d", isTargetScore, isHighNum);
		TextOut(mem0dc, 630, 10, isDebugData, strlen(isDebugData));

		for (int i = 0; i < isGameLength; ++i){
			for (int j = 0; j < isGameWidth; ++j){
				sprintf(isDebugData, "%d  ", isGameData[i][j]);
				TextOut(mem0dc, 600 + (i * 40), 300 + (j * 45), isDebugData, strlen(isDebugData));
			}
		}

		for (int i = 0; i < isGameLength; ++i){
			for (int j = 0; j < isGameWidth; ++j){
				sprintf(isDebugData, "[%d , %d]", 10 + (i * 130), 10 + (j * 130));
				TextOut(mem0dc, 600 + (i * 80), 100 + (j * 45), isDebugData, strlen(isDebugData));
			}
		}

		//-----------------------------------------------------------------------------------------------------------------------------------------------

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mem0dc, 0, 0, SRCCOPY);


		SelectObject(mem0dc, hbmMemOld); //-4
		DeleteObject(hbmMem); //-3
		DeleteObject(hbmMemOld); //-3
		DeleteDC(mem0dc); //-2
		DeleteDC(mem1dc); //-2
		DeleteDC(hdc); //-2
		EndPaint(hwnd, &ps);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}


void isDownPlusNumber(int isLineData){
	for (int i = isGameLength - 1; i >= 0; --i){
		if (isGameData[isLineData][i] == isGameData[isLineData][i - 1] && i - 1 >= 0){

			if (isGameData[isLineData][i] + isGameData[isLineData][i - 1] != 0){
				isRealMove = 1;
			}

			isGameData[isLineData][i] = isGameData[isLineData][i] + isGameData[isLineData][i - 1];
			isScore = isScore + isGameData[isLineData][i - 1];
			isGameData[isLineData][i - 1] = 0;
			isDownNumber(isLineData);
		}
	}
}

void isDownNumber(int isLineData){
	for (int i = 0; i < isGameLength; ++i){
		for (int i = isGameLength - 1; i >= 0; --i){

			if (isGameData[isLineData][i] == 0 && i - 1 >= 0){
				isTempNum = isGameData[isLineData][i];
				isGameData[isLineData][i] = isGameData[isLineData][i - 1];
				isGameData[isLineData][i - 1] = isTempNum;
			}
		}
	}
}

void isUPPlusNumber(int isLineData){
	for (int i = 0; i < isGameLength; ++i){
		if (isGameData[isLineData][i] == isGameData[isLineData][i + 1] && i + 1 < 4){

			if (isGameData[isLineData][i] + isGameData[isLineData][i + 1] != 0){
				isRealMove = 1;
			}

			isGameData[isLineData][i] = isGameData[isLineData][i] + isGameData[isLineData][i + 1];
			isScore = isScore + isGameData[isLineData][i + 1];
			isGameData[isLineData][i + 1] = 0;
			isUPNumber(isLineData);
		}
	}
}

void isUPNumber(int isLineData){
	for (int i = 0; i < isGameLength; ++i){
		for (int i = 0; i < isGameLength; ++i){

			if (isGameData[isLineData][i] == 0 && i + 1 < 4){
				isTempNum = isGameData[isLineData][i];
				isGameData[isLineData][i] = isGameData[isLineData][i + 1];
				isGameData[isLineData][i + 1] = isTempNum;
			}
		}
	}
}

void isLeftPlusNumber(int isLineData){
	for (int i = 0; i < isGameLength; ++i){
		if (isGameData[i][isLineData] == isGameData[i + 1][isLineData] && i + 1 < 4){

			if (isGameData[i][isLineData] + isGameData[i + 1][isLineData] != 0){
				isRealMove = 1;
			}

			isGameData[i][isLineData] = isGameData[i][isLineData] + isGameData[i + 1][isLineData];
			isScore = isScore + isGameData[i + 1][isLineData];
			isGameData[i + 1][isLineData] = 0;
			isLeftNumber(isLineData);
		}
	}
}

void isLeftNumber(int isLineData){
	for (int i = 0; i < isGameLength; ++i){
		for (int i = 0; i < isGameLength; ++i){

			if (isGameData[i][isLineData] == 0 && i + 1 < 4){
				isTempNum = isGameData[i][isLineData];
				isGameData[i][isLineData] = isGameData[i + 1][isLineData];
				isGameData[i + 1][isLineData] = isTempNum;
			}
		}
	}
}

void isRightPlusNumber(int isLineData){
	for (int i = isGameLength - 1; i >= 0; --i){
		if (isGameData[i][isLineData] == isGameData[i - 1][isLineData] && i - 1 >= 0){

			if (isGameData[i][isLineData] + isGameData[i - 1][isLineData] != 0){
				isRealMove = 1;
			}

			isGameData[i][isLineData] = isGameData[i][isLineData] + isGameData[i - 1][isLineData];
			isScore = isScore + isGameData[i - 1][isLineData];
			isGameData[i - 1][isLineData] = 0;
			isRightNumber(isLineData);
		}
	}
}

void isRightNumber(int isLineData){
	for (int i = 0; i < isGameLength; ++i){
		for (int i = isGameLength - 1; i >= 0; --i){

			if (isGameData[i][isLineData] == 0 && i - 1 >= 0){
				isTempNum = isGameData[i][isLineData];
				isGameData[i][isLineData] = isGameData[i - 1][isLineData];
				isGameData[i - 1][isLineData] = isTempNum;
			}
		}
	}
}

void is4CheckData(int isWhatKey, HWND hwnd){
	// 4 = Left, 8 = UP, 6 = Right, 2 = Down
	if (isGameTheEnd() == false && isGamePause == 0){
		isCopyMapData();		// 게임 데이터 복사한다

		for (int i = 0; i < isGameLength; ++i){
			if (isWhatKey == 6){
				isRightNumber(i);
				isRightPlusNumber(i);
			}
			else if (isWhatKey == 4){
				isLeftNumber(i);
				isLeftPlusNumber(i);
			}
			else if (isWhatKey == 8){
				isUPNumber(i);
				isUPPlusNumber(i);
			}
			else if (isWhatKey == 2){
				isDownNumber(i);
				isDownPlusNumber(i);
			}
		}

		isCompareData();		// 게임 데이터 비교한다

		if (isRealMove == 1){
			isRandNumber();
		}
		isTheEndGame(hwnd);
	}
	else{
		// Game The End 일경우 ↓
		if (isGamePause == 0){
			isNewGameAnswer = MessageBox(hwnd, "Game Over..!\n\n더이상 움직일수 있는 숫자가 없습니다.\n게임을 재시작 하시겠습니까?", "WindowsHyun 2048", MB_OKCANCEL);
			if (isNewGameAnswer == IDOK){
				isNewGame();
			}
			else{
				MessageBox(hwnd, "게임이 정지 상태입니다.\n새로운 게임을 시작하시려면 ( Game - NewGame )을 클릭해주세요.", "WindowsHyun 2048", MB_OK);
				isLastScore = isScore;
				isScore = 0;
				isGamePause = 1;
			}
		}

	}
}

bool isGameTheEnd(){
	// 4 = Left, 8 = UP, 6 = Right, 2 = Down
	int isCheckData = 0;

	for (int i = 0; i < isGameLength; ++i){
		for (int j = 0; j < isGameWidth; ++j){
			if (isGameData[i][j] == 0){
				isCheckData++;
			}
		}
	}

	for (int i = 0; i < isGameLength; ++i){
		for (int j = 0; j < isGameWidth; ++j){


			if (isWhatKey == 8 || isWhatKey == 2){
				// 위, 아래 일경우
				if (isGameData[i][j] == isGameData[i][j + 1] || isGameData[i][j] == isGameData[i][j - 1]){
					if (j + 1 <= 3 && j - 1 >= 0){
						isCheckData++;
					}
				}
			}

			if (isWhatKey == 4 || isWhatKey == 6){
				// 왼쪽 오른쪽 일경우
				if (isGameData[i][j] == isGameData[i + 1][j] || isGameData[i][j] == isGameData[i - 1][j]){
					if (i + 1 <= 3 && i - 1 >= 0){
						isCheckData++;
					}
				}
			}


		}
	}

	if (isCheckData != 0){
		return 0;	//False 게임 아직 안끝남
	}
	else{
		return 1;	//True 게임 아직 진행중
	}

}

void isMapData(HDC hdc, HDC memdc, HBITMAP hBit1, int isX, int isY){
	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, hBit1);
	TransparentBlt(hdc, isX, isY, 120, 120, memdc, 0, 0, 120, 120, RGB(255, 0, 0));
	DeleteDC(memdc);
}

void isRandNumber(){
	int isXdata = 0, isYdata = 0;
isReDataTwo:
	srand((unsigned int)time(NULL) + isXdata + isYdata + GetTickCount());
	isXdata = rand() % isGameWidth;
	isYdata = rand() % isGameLength;

	if (isGameData[isXdata][isYdata] != 0){
		goto isReDataTwo;
	}
	else{
		isGameData[isXdata][isYdata] = 2;
	}

}

void isCopyMapData(){
	for (int i = 0; i < isGameLength; ++i){
		for (int j = 0; j < isGameWidth; ++j){
			isGameDataCopy[i][j] = isGameData[i][j];
		}
	}
}

void isCompareData(){
	for (int i = 0; i < isGameLength; ++i){
		for (int j = 0; j < isGameWidth; ++j){
			if (isGameDataCopy[i][j] != isGameData[i][j]){
				isRealMove = 1;
			}
		}
	}
}

void isNewGame(){
	isWhatKey = 0;		// 4 = Left, 8 = UP, 6 = Right, 2 = Down
	isGamePause = 0;
	for (int i = 0; i < isGameLength; ++i){
		for (int j = 0; j < isGameWidth; ++j){
			isGameData[i][j] = 0;
			isGameDataCopy[i][j] = 0;
		}
	}
	for (int i = 0; i < 2; ++i){
		isRandNumber(); //초기 블럭 2개 설정!
	}
	isTargetScore = 2048, isHighNum = 2;
}

void isTheEndGame(HWND hwnd){
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	// 해당 점수에 도달할 경우!
	if (isHighNum >= isTargetScore){
		isHighNum = 2;
		isNewGameAnswer = MessageBox(hwnd, "해당 목표에 도달하였습니다!\n게임을 재시작 하시겠습니까?", "WindowsHyun 2048", MB_OKCANCEL);

		if (isNewGameAnswer == IDOK){
			isNewGame();
		}
		else{
			MessageBox(hwnd, "게임이 정지 상태입니다.\n새로운 게임을 시작하시려면 ( Game - NewGame )을 클릭해주세요.", "WindowsHyun 2048", MB_OK);
			isGamePause = 1;
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------
}