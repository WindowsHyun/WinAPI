#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")									// TransparentBlt ����ϱ� ���� ���̺귯�� ȣ��..


#define isGameWidth 4														// ����
#define isGameLength 4														// ����
#define isGameTotalData 2048												// ���� ������ ����

int isGameTotal = 2048;														// ���� ������ ����
int isGameData[isGameWidth][isGameLength];						// ������ �����ϱ�
int isGameDataCopy[isGameWidth][isGameLength];				// ������ �����ϱ�
int isWhatKey;																		// 4 = Left, 8 = UP, 6 = Right, 2 = Down
int isTempNum;																	// �׳� swap �ӽ� ����
int isRealMove = 0;																// ������ ���������� Ȯ�� ( 0 = �ȿ�����, 1 = ������ )
int isScore = 0;																		// ���� ����
int isGamePause = 0;															// ���� �Ͻ����� ( 0 = ����, 1 = ���� )
int isLastScore = 0;																// ���� ������ ���� ����
int isMx, isMy;																		// ����ó ����� ���� ���콺 ��ǥ
int  isClickX, isClickY, isTempX, isTempY;								// ��óó ����� ���� ��ġ ��ǥ
int isTargetScore = 2048, isHighNum = 2;								// ��ǥ ������ 2048, ���� �ִ� ������ ó���̴ϱ� 2
int isNewGameAnswer;															// �޽��� �ڽ� Ȯ����� üũ�ϴ� �κ�

typedef struct isGameDataXY{
	int bx;
	int by;
}isGameDataXY;																	// ���� ��ġ ����


bool isGameTheEnd();															// False�� Game Not End, True �� Game End

void is4CheckData(int, HWND);												// ���� üũ�� ���� & HWND�߰� ������ isTheEndGame ���� hwnd�� ����ϱ�����
void isRandNumber();															// �������� ���� �ѷ��ֱ�
void isCopyMapData();															// �� ������ �Ǿ����� Ȯ���ϱ� ���� ����
void isCompareData();															// �� ������ �Ǿ����� Ȯ���ϴ� üũ ��
void isMapData(HDC, HDC, HBITMAP, int, int);						// �迭�� ���ڸ� ������ �׷��ִ� �κ�
void isNewGame();																// ���ο� ������ �����Ҷ� ȣ���ϴ� �Լ�
void isTheEndGame(HWND);															// ��ǥ������ �����ߴ��� Ȯ����, ����������� �޽���â ����

// -----------------------------------------------------------------------------------------------------------
// ��, �Ʒ�, ����, ������ ���� �����̱�
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
	static HBITMAP isNumber[isGameTotalData];					// 2048���� �迭�� ����� �޸� �����ߤ�����
	static HBITMAP hbmOld, hbmMem, hbmMemOld;			// ������۸��� ���Ͽ�!
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
			isNewGameAnswer = MessageBox(hwnd, "������ ������ �ٽ� �����Ͻǲ�����?", "WindowsHyun 2048", MB_OKCANCEL);

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
			isNewGameAnswer = MessageBox(hwnd, "������ ������ �����Ͻǲ�����?", "WindowsHyun 2048", MB_OKCANCEL);

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
		//���⼭ ����� �ؾ���

		for (int y = 0; y < isGameLength; ++y){
			for (int x = 0; x < isGameWidth; ++x){
				if (isGameDataXY[y][x].bx < isMx && isMx < 10 + isGameDataXY[y][x].bx + 130 &&
					isGameDataXY[y][x].by < isMy && isMy <10 + isGameDataXY[y][x].by + 130){
					isClickY = y; isClickX = x;								// X, Y ��ǥ�� �������ش�.
				}
			}
		}

		if (isTempX > isClickX){
			//����
			isWhatKey = 4, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (isTempX < isClickX){
			//������
			isWhatKey = 6, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (isTempY > isClickY){
			//��
			isWhatKey = 8, isRealMove = 0;
			is4CheckData(isWhatKey, hwnd);
		}
		else if (isTempY < isClickY){
			//�Ʒ�
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
					isClickY = y; isClickX = x;								// X, Y ��ǥ�� �������ش�.
				}
				else if (isMx < 10 || isMy > 520 || isMx >520){
					// �� ������ ����� �Ѵ� 0����
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
					isClickY = y; isClickX = x;								// X, Y ��ǥ�� �������ش�.
					isTempX = x, isTempY = y;							// ���콺�� �����̸� �ڵ����� X,Y ��ǥ�� �ٲ�� �׷��Ƿ� �ӽ� ��ġ�� ����
				}



			}
		}


		InvalidateRgn(hwnd, NULL, FALSE);
		break;


	case WM_CREATE:
		srand((unsigned int)time(NULL) + GetTickCount());

		GetClientRect(hwnd, &rt);

		isNumber[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));					// ���� ����
		isNumber[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2));					// ���� 2
		isNumber[4] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP3));					// ���� 4
		isNumber[8] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP4));					// ���� 8
		isNumber[16] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP5));					// ���� 16
		isNumber[32] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP6));					// ���� 32
		isNumber[64] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP7));					// ���� 64
		isNumber[128] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP8));				// ���� 128
		isNumber[256] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP9));				// ���� 256
		isNumber[512] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP10));				// ���� 512
		isNumber[1024] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP11));			// ���� 1024
		isNumber[2048] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP12));			// ���� 2048


		isWhatKey = 0;		// 4 = Left, 8 = UP, 6 = Right, 2 = Down

		for (int i = 0; i < 2; ++i){
			isRandNumber(); //�ʱ� �� 2�� ����!
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
		// �ְ� ������ ã�ƶ�!
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
		// ���� �����͸� �ؽ�Ʈ�� ��ȯ
		sprintf(isDebugData, "WhatKey : %d, isRealMove : %d, isScore = %d, isLastScore = %d, isMx = %d, isMy = %d, ���� ��ġ ��ǥ = (%d, %d), ���� ��ġ ��ǥ = (%d, %d)",
			isWhatKey, isRealMove, isScore, isLastScore, isMx, isMy, isClickX, isClickY, isTempX, isTempY);
		TextOut(mem0dc, 0, 530, isDebugData, strlen(isDebugData));

		sprintf(isDebugData, "��ǥ ���� : %d, ���� �ְ� ���� ���� : %d", isTargetScore, isHighNum);
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
		isCopyMapData();		// ���� ������ �����Ѵ�

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

		isCompareData();		// ���� ������ ���Ѵ�

		if (isRealMove == 1){
			isRandNumber();
		}
		isTheEndGame(hwnd);
	}
	else{
		// Game The End �ϰ�� ��
		if (isGamePause == 0){
			isNewGameAnswer = MessageBox(hwnd, "Game Over..!\n\n���̻� �����ϼ� �ִ� ���ڰ� �����ϴ�.\n������ ����� �Ͻðڽ��ϱ�?", "WindowsHyun 2048", MB_OKCANCEL);
			if (isNewGameAnswer == IDOK){
				isNewGame();
			}
			else{
				MessageBox(hwnd, "������ ���� �����Դϴ�.\n���ο� ������ �����Ͻ÷��� ( Game - NewGame )�� Ŭ�����ּ���.", "WindowsHyun 2048", MB_OK);
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
				// ��, �Ʒ� �ϰ��
				if (isGameData[i][j] == isGameData[i][j + 1] || isGameData[i][j] == isGameData[i][j - 1]){
					if (j + 1 <= 3 && j - 1 >= 0){
						isCheckData++;
					}
				}
			}

			if (isWhatKey == 4 || isWhatKey == 6){
				// ���� ������ �ϰ��
				if (isGameData[i][j] == isGameData[i + 1][j] || isGameData[i][j] == isGameData[i - 1][j]){
					if (i + 1 <= 3 && i - 1 >= 0){
						isCheckData++;
					}
				}
			}


		}
	}

	if (isCheckData != 0){
		return 0;	//False ���� ���� �ȳ���
	}
	else{
		return 1;	//True ���� ���� ������
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
		isRandNumber(); //�ʱ� �� 2�� ����!
	}
	isTargetScore = 2048, isHighNum = 2;
}

void isTheEndGame(HWND hwnd){
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	// �ش� ������ ������ ���!
	if (isHighNum >= isTargetScore){
		isHighNum = 2;
		isNewGameAnswer = MessageBox(hwnd, "�ش� ��ǥ�� �����Ͽ����ϴ�!\n������ ����� �Ͻðڽ��ϱ�?", "WindowsHyun 2048", MB_OKCANCEL);

		if (isNewGameAnswer == IDOK){
			isNewGame();
		}
		else{
			MessageBox(hwnd, "������ ���� �����Դϴ�.\n���ο� ������ �����Ͻ÷��� ( Game - NewGame )�� Ŭ�����ּ���.", "WindowsHyun 2048", MB_OK);
			isGamePause = 1;
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------
}