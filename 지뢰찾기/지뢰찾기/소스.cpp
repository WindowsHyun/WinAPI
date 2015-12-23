#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

enum{ None, None1, None2, None3, None4, None5, None6, None7, None8, Mine, Flag, NotMine, BombMine, NomalMine };

int isMineEasy[9][9];
int isMineNomal[16][16];
int isMineHard[30][16];

int isDrawEasy[9][9];
int isDrawNomal[16][16];
int isDrawHard[30][16];

int isGameMode = 1; //Easy = 0, Nomal = 1, Hard = 2
int isMineCount = 0;
int isMineGameWin = 0;
int isLineData = 1;

int isGameExit =0;
int isStartTime =0;
int isEndTime =0;
int isCountTwo =0;

void isGameReset(int, int, int);
void isDrawReset();
void isGameOver();
void isClick(int, int);
void isGameWin();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	HACCEL hAcc;
	MSG msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = "Minesweeper2012180004";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Minesweeper2012180004", "Minesweeper", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	hAcc=LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage (&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(hwnd,hAcc,&msg)) // 단축키 -> 메뉴 ID로 인식
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
	}

	while(GetMessage(&msg, NULL, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	switch(iMsg){
		HDC hdc,mem0dc ,mem1dc, mem2dc, mem3dc, mem4dc, mem5dc, isZE[9], mem6dc, mem7dc, mem8dc;
		static HBITMAP isCloseMine, isMine, isZeroToEight[9], isFlag, isNotMine, isSearchMine, isStart;
		HBITMAP hbmOld, hbmMem, hbmMemOld;
		static HBITMAP isMoveImage[14], isGirlImage[11], isBoyImage[4];
		PAINTSTRUCT ps ;
		HFONT hFont, OldFont;
		static RECT rt, isStartButton;
		static HMENU hMenu, hSubMenu;
		static int bx, by, isX, isY, mx, my;
		static char debug[500];

	case WM_CREATE:
		GetClientRect(hwnd,&rt);
		SetTimer(hwnd,1,1000,NULL);
		isCloseMine = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		isMine = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		isZeroToEight[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		isZeroToEight[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
		isZeroToEight[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
		isZeroToEight[3] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
		isZeroToEight[4] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
		isZeroToEight[5] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
		isZeroToEight[6] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
		isZeroToEight[7] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP10));
		isZeroToEight[8] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP11));
		isFlag = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP12));
		isNotMine = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP13));
		isSearchMine = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP14));
		isStart = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP15));
		isStartButton.left = rt.right/3+60, isStartButton.right = rt.right/3+60+120, isStartButton.top = 5, isStartButton.bottom = 43;
		//StretchBlt(hdc,rt.right/3+60,5,120, 43,mem6dc,0,0,120,43,SRCCOPY);
		isGameMode =0;
		isDrawReset();
		isGameReset(9, 9, 10);
		break;

	case WM_TIMER:
		switch(wParam){
		case 1:
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		}
		break;

	case WM_RBUTTONDOWN:
		mx=LOWORD(lParam);
		my=HIWORD(lParam);

		if( isGameMode == 0){
			bx = rt.right, by = rt.bottom-51;
			isLineData =1;
		}else if( isGameMode == 1){
			bx = rt.right, by = rt.bottom-60;
			isLineData=2;
		}else if( isGameMode == 2){
			bx = rt.right, by = rt.bottom-90;
			isLineData=3;
		}

		if( isGameMode == 0){
			isX = 9, isY = 9;
		}else if( isGameMode == 1){
			isX = 16, isY = 16;
		}else if( isGameMode == 2){
			isX = 30, isY = 16;
		}

		for(int i=0; i<isX; ++i){
			for(int j=isLineData; j<isY+isLineData; ++j){
				if(bx/isX*i<= mx && by/isY*j<= my && bx/isX*(i+1) >= mx && by/isY*(j+1) >= my ){

					if( isGameMode == 0){
						if(isDrawEasy[i][j-isLineData] != 1){
							if(isDrawEasy[i][j-isLineData] == Flag ){
								isDrawEasy[i][j-isLineData]  = 0;
							}else{
								isDrawEasy[i][j-isLineData] = Flag;
							}
						}
						InvalidateRgn(hwnd, NULL, TRUE);
					}else if( isGameMode == 1){
						if(isDrawNomal[i][j-isLineData] != 1){
							if(isDrawNomal[i][j-isLineData] == Flag ){
								isDrawNomal[i][j-isLineData]  = 0;
							}else{
								isDrawNomal[i][j-isLineData] = Flag;
							}
						}
						InvalidateRgn(hwnd, NULL, TRUE);

					}else if( isGameMode == 2){
						if(isDrawHard[i][j-isLineData] != 1){
							if(isDrawHard[i][j-isLineData] == Flag ){
								isDrawHard[i][j-isLineData]  = 0;
							}else{
								isDrawHard[i][j-isLineData] = Flag;
							}
						}
						InvalidateRgn(hwnd, NULL, TRUE);
					}

					break;
				}

			}
		}

		isGameWin();
		break;

	case WM_LBUTTONDOWN:
		mx=LOWORD(lParam);
		my=HIWORD(lParam);

		if( mx >= isStartButton.left && mx <= isStartButton.right && my >= isStartButton.top && my<= isStartButton.bottom){
			SetTimer(hwnd,1,1000,NULL);
			isDrawReset();
			if( isGameMode == 0){
				isGameReset(9, 9, 10);
			}else if( isGameMode == 1){
				isGameReset(16, 16, 20);
			}else if( isGameMode == 2){
				isGameReset(30, 16, 30);
			}
			InvalidateRgn(hwnd, NULL, TRUE);
		}

		if( isGameMode == 0){
			bx = rt.right, by = rt.bottom-51;
			isLineData =1;
		}else if( isGameMode == 1){
			bx = rt.right, by = rt.bottom-60;
			isLineData=2;
		}else if( isGameMode == 2){
			bx = rt.right, by = rt.bottom-90;
			isLineData=3;
		}

		if( isGameMode == 0){
			isX = 9, isY = 9;
		}else if( isGameMode == 1){
			isX = 16, isY = 16;
		}else if( isGameMode == 2){
			isX = 30, isY = 16;
		}

		for(int i=0; i<isX; ++i){
			for(int j=isLineData; j<isY+isLineData; ++j){
				if(bx/isX*i<= mx && by/isY*j<= my && bx/isX*(i+1) >= mx && by/isY*(j+1) >= my ){

					if( isGameMode == 0){
						if( isMineEasy[i][j-isLineData] == Mine ){
							isDrawEasy[i][j-isLineData]  = BombMine;
							isGameOver();
							wsprintf(debug,"Game Over..!");
							MessageBox(hwnd,debug,"",CB_OKAY);
							InvalidateRgn(hwnd, NULL, TRUE);
						}else{
							isClick( i,  j-isLineData);
							InvalidateRgn(hwnd, NULL, TRUE);
						}
					}else if( isGameMode == 1){
						if( isMineNomal[i][j-isLineData] == Mine ){
							isDrawNomal[i][j-isLineData]  = BombMine;
							isGameOver();
							wsprintf(debug,"Game Over..!");
							MessageBox(hwnd,debug,"",CB_OKAY);
							InvalidateRgn(hwnd, NULL, TRUE);
						}else{
							isClick( i,  j-isLineData);
							InvalidateRgn(hwnd, NULL, TRUE);
						}
					}else if( isGameMode == 2){
						if( isMineHard[i][j-isLineData] == Mine ){
							isDrawHard[i][j-isLineData]  = BombMine;
							isGameOver();
							wsprintf(debug,"Game Over..!");
							MessageBox(hwnd,debug,"",CB_OKAY);
							InvalidateRgn(hwnd, NULL, TRUE);
						}else{
							isClick( i,  j-isLineData);
							InvalidateRgn(hwnd, NULL, TRUE);
						}
					}

					break;
				}

			}
		}

		isGameWin();
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem0dc = CreateCompatibleDC(hdc);//2
		SelectObject(mem0dc, isCloseMine);

		mem1dc = CreateCompatibleDC(hdc);//2
		SelectObject(mem1dc, isMine);

		mem2dc = CreateCompatibleDC(hdc);//2
		SelectObject(mem2dc, isZeroToEight[0]);

		isZE[1] = CreateCompatibleDC(hdc);//2
		SelectObject(isZE[1], isZeroToEight[1]);

		isZE[2] = CreateCompatibleDC(hdc);//2
		SelectObject(isZE[2], isZeroToEight[2]);

		isZE[3] = CreateCompatibleDC(hdc);//2
		SelectObject(isZE[3], isZeroToEight[3]);

		isZE[4] = CreateCompatibleDC(hdc);//2
		SelectObject(isZE[4], isZeroToEight[4]);

		mem3dc = CreateCompatibleDC(hdc);//2
		SelectObject(mem3dc, isFlag);

		mem4dc = CreateCompatibleDC(hdc);//2
		SelectObject(mem4dc, isNotMine);

		mem5dc = CreateCompatibleDC(hdc);//2
		SelectObject(mem5dc, isSearchMine);

		mem6dc = CreateCompatibleDC(hdc);//2
		SelectObject(mem6dc, isStart);

		StretchBlt(hdc,rt.right/3+60,5,120, 43,mem6dc,0,0,120,43,SRCCOPY);

		isCountTwo=0;

		if( isGameMode == 0){
			isX = 9, isY = 9;
		}else if( isGameMode == 1){
			isX = 16, isY = 16;
		}else if( isGameMode == 2){
			isX = 30, isY = 16;
		}

		for(int i=0; i<isX; ++i){
			for(int j=0; j<isY; ++j){
				if( isGameMode == 0){
					if( isMineEasy[i][j] == Mine && isDrawEasy[i][j] == Flag){
						isCountTwo++;
					}
				}else if( isGameMode == 1){
					if( isMineNomal[i][j] == Mine && isDrawNomal[i][j] == Flag){
						isCountTwo++;
					}
				}else if( isGameMode == 2){
					if( isMineHard[i][j] == Mine && isDrawHard[i][j] == Flag){
						isCountTwo++;
					}
				}
			}
		}
		hFont=CreateFont(45,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,VARIABLE_PITCH | FF_ROMAN,TEXT("궁서"));
		OldFont=(HFONT)SelectObject(hdc,hFont);
		wsprintf(debug, "%d개", isMineCount-isCountTwo);
		SetBkColor(hdc,RGB(0,0,0));
		SetTextColor(hdc, RGB(255,255,255));
		TextOut(hdc, 30,5, debug, strlen(debug));
		SelectObject(hdc,OldFont);
		DeleteObject(hFont);

		hFont=CreateFont(45,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,VARIABLE_PITCH | FF_ROMAN,TEXT("궁서"));
		OldFont=(HFONT)SelectObject(hdc,hFont);
		isEndTime = GetTickCount();
		wsprintf(debug,"%d초", (isEndTime-isStartTime)/1000);
		SetBkColor(hdc,RGB(0,0,0));
		SetTextColor(hdc, RGB(255,255,255));
		TextOut(hdc, rt.right-100,5, debug, strlen(debug));
		SelectObject(hdc,OldFont);
		DeleteObject(hFont);

		if( isGameMode == 0){
			bx = rt.right, by = rt.bottom-51;
			isLineData =1;
		}else if( isGameMode == 1){
			bx = rt.right, by = rt.bottom-60;
			isLineData=2;
		}else if( isGameMode == 2){
			bx = rt.right, by = rt.bottom-90;
			isLineData=3;
		}

		for(int i=0; i<isX; ++i){
			for(int j=isLineData; j<isY+isLineData; ++j){
				if( isGameMode == 0){
					if( isDrawEasy[i][j-isLineData] == Flag ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem3dc,0,0,20,20,SRCCOPY);
					}else if( isDrawEasy[i][j-isLineData] == NotMine ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem4dc,0,0,20,20,SRCCOPY);
					}else if( isDrawEasy[i][j-isLineData] == BombMine ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem5dc,0,0,20,20,SRCCOPY);
					}else if( isDrawEasy[i][j-isLineData] == NomalMine ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem1dc,0,0,20,20,SRCCOPY);
					}else if( isMineEasy[i][j-isLineData] == 0 && isDrawEasy[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem2dc,0,0,20,20,SRCCOPY);
					}else if( isMineEasy[i][j-isLineData] == 1 && isDrawEasy[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[1],0,0,20,20,SRCCOPY);
					}else if( isMineEasy[i][j-isLineData] == 2 && isDrawEasy[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[2],0,0,20,20,SRCCOPY);
					}else  if( isMineEasy[i][j-isLineData] == 3 && isDrawEasy[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[3],0,0,20,20,SRCCOPY);
					}else  if( isMineEasy[i][j-isLineData] == 4 && isDrawEasy[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[4],0,0,20,20,SRCCOPY);
					}else{
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem0dc,0,0,20,20,SRCCOPY);
					}
				}
				if( isGameMode == 1){
					if( isDrawNomal[i][j-isLineData] == Flag ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem3dc,0,0,20,20,SRCCOPY);
					}else if( isDrawNomal[i][j-isLineData] == NotMine ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem4dc,0,0,20,20,SRCCOPY);
					}else if( isDrawNomal[i][j-isLineData] == BombMine ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem5dc,0,0,20,20,SRCCOPY);
					}else if( isDrawNomal[i][j-isLineData] == NomalMine ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem1dc,0,0,20,20,SRCCOPY);
					}else if( isMineNomal[i][j-isLineData] == 0 && isDrawNomal[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem2dc,0,0,20,20,SRCCOPY);
					}else if( isMineNomal[i][j-isLineData] == 1 && isDrawNomal[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[1],0,0,20,20,SRCCOPY);
					}else if( isMineNomal[i][j-isLineData] == 2 && isDrawNomal[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[2],0,0,20,20,SRCCOPY);
					}else  if( isMineNomal[i][j-isLineData] == 3 && isDrawNomal[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[3],0,0,20,20,SRCCOPY);
					}else  if( isMineNomal[i][j-isLineData] == 4 && isDrawNomal[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[4],0,0,20,20,SRCCOPY);
					}else{
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem0dc,0,0,20,20,SRCCOPY);
					}
				}
				if( isGameMode == 2){
					if( isDrawHard[i][j-isLineData] == Flag ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem3dc,0,0,20,20,SRCCOPY);
					}else if( isDrawHard[i][j-isLineData] == NotMine ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem4dc,0,0,20,20,SRCCOPY);
					}else  if( isDrawHard[i][j-isLineData] == BombMine ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem5dc,0,0,20,20,SRCCOPY);
					}else if( isDrawHard[i][j-isLineData] == NomalMine ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem1dc,0,0,20,20,SRCCOPY);
					}else if( isMineHard[i][j-isLineData] == 0 && isDrawHard[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem2dc,0,0,20,20,SRCCOPY);
					}else if( isMineHard[i][j-isLineData] == 1 && isDrawHard[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[1],0,0,20,20,SRCCOPY);
					}else if( isMineHard[i][j-isLineData] == 2 && isDrawHard[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[2],0,0,20,20,SRCCOPY);
					}else  if( isMineHard[i][j-isLineData] == 3 && isDrawHard[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[3],0,0,20,20,SRCCOPY);
					}else  if( isMineHard[i][j-isLineData] == 4 && isDrawHard[i][j-isLineData] == 1 ){
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,isZE[4],0,0,20,20,SRCCOPY);
					}else{
						StretchBlt(hdc,bx/isX*i,by/isY*j,bx/isX, by/isY,mem0dc,0,0,20,20,SRCCOPY);
					}
				}


			}
		}

		if( isMineGameWin == 1){
			KillTimer(hwnd,1);
			wsprintf(debug,"Game Win..!\n\n총 걸린 시간 : %d초", (isEndTime-isStartTime)/1000);
			MessageBox(hwnd,debug,"",CB_OKAY);
		}
		DeleteDC(mem0dc); //-2
		DeleteDC(mem1dc); //-2
		DeleteDC(mem2dc); //-2
		DeleteDC(mem3dc); //-2
		DeleteDC(mem4dc); //-2
		DeleteDC(mem5dc); //-2
		DeleteDC(mem6dc); //-2
		DeleteDC(isZE[0]); //-2
		DeleteDC(isZE[1]); //-2
		DeleteDC(isZE[2]); //-2
		DeleteDC(isZE[3]); //-2
		DeleteDC(isZE[4]); //-2
		DeleteDC(hdc); //-2
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_GAME_NEWGAME:
			isDrawReset();
			SetTimer(hwnd,1,1000,NULL);
			if( isGameMode == 0){
				isGameReset(9, 9, 10);
			}else if( isGameMode == 1){
				isGameReset(16, 16, 20);
			}else if( isGameMode == 2){
				isGameReset(30, 16, 30);
			}
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case ID_GAME_EXIT:
			if(isGameExit ==0){
				wsprintf(debug,"정말로 종료하시려면 한번더 EXIT 버튼을 눌러주세요..!");
				MessageBox(hwnd,debug,"",CB_OKAY);
				PostQuitMessage(0);
			}else{
				PostQuitMessage(0);
			}
			isGameExit++;
			break;

		case ID_LEVEL_EASY:
			SetTimer(hwnd,1,1000,NULL);
			isGameMode =0;
			isDrawReset();
			isGameReset(9, 9, 10);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;

		case ID_LEVEL_NOMAL:
			SetTimer(hwnd,1,1000,NULL);
			isGameMode=1;
			isDrawReset();
			isGameReset(16, 16, 20);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;

		case ID_LEVEL_HARD:
			SetTimer(hwnd,1,1000,NULL);
			isGameMode=2;
			isDrawReset();
			isGameReset(30, 16, 30);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;

		case ID_HINT_DOYOUHAVESEVERALMINESWEEPER:
			isCountTwo=0;
			for(int i=0; i<isX; ++i){
			for(int j=0; j<isY; ++j){
				if( isGameMode == 0){
					if( isMineEasy[i][j] == Mine && isDrawEasy[i][j] == Flag){
						isCountTwo++;
					}
				}else if( isGameMode == 1){
					if( isMineNomal[i][j] == Mine && isDrawNomal[i][j] == Flag){
						isCountTwo++;
					}
				}else if( isGameMode == 2){
					if( isMineHard[i][j] == Mine && isDrawHard[i][j] == Flag){
						isCountTwo++;
					}
				}
			}
		}

			wsprintf(debug,"총 지뢰의 갯수 : %d\n총 찾은 지뢰의 갯수 : %d\n남은 지뢰의 갯수 : %d",isMineCount, isCountTwo, isMineCount-isCountTwo);
			MessageBox(hwnd,debug,"",CB_OKAY);
			break;

		case ID_HINT_HOWMUCHTIMEDIDITGO:
			isEndTime = GetTickCount();
			wsprintf(debug,"소요 시간 : %d초", (isEndTime-isStartTime)/1000);
			MessageBox(hwnd,debug,"",CB_OKAY);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		//PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void isGameReset(int isWidth, int isHeight, int isMaxmine){
	srand((unsigned int)time(NULL));

	for(int i=0; i<isWidth; ++i){
		for(int j=0; j<isHeight; ++j){
			if(isGameMode == 0){
				isMineEasy[i][j] = 0;
			}else if (isGameMode == 1){
				isMineNomal[i][j] = 0;
			}else if(isGameMode == 2){
				isMineHard[i][j] = 0;
			}
		}
	}

	int isMine = 0, isMineCreate=10;
	for(int i=0; i<isWidth; ++i){
		for(int j=0; j<isHeight; ++j){
			if(isGameMode == 0){
				if(rand()%isMineCreate+1 == 1){
					if(isMine == isMaxmine){
						//isMineEasy[i][j] = 0;
					}else{
						isMineEasy[i][j] = Mine;
						isMineCount++;
						isMine++;
						//isMineCreate++;
						for(int k=-1; k<=1; ++k){
							for(int l=-1;l<=1; ++l){
								if(i+k <= -1 || i+k >= 9 || j+l <= -1 || j+l  >= 9){
								}else{
									if( isMineEasy[i+k][j+l] != Mine ){
										isMineEasy[i+k][j+l]++;
									}
								}
							}
						}

					}
				}else{
					//isMineEasy[i][j] = 0;
				}
			}else if(isGameMode == 1){
				if(rand()%isMineCreate+1 == 1){
					if(isMine == isMaxmine){
						//isMineNomal[i][j] = 0;
					}else{
						isMineNomal[i][j] = Mine;
						isMineCount++;
						isMine++;
						isMineCreate+=2;

						for(int k=-1; k<=1; ++k){
							for(int l=-1;l<=1; ++l){
								if(i+k <= -1 || i+k >= 16 || j+l <= -1 || j+l  >= 16){
								}else{
									if( isMineNomal[i+k][j+l] != Mine ){
										isMineNomal[i+k][j+l]++;
									}
								}
							}
						}

					}
				}else{
					//isMineNomal[i][j] = 0;
				}
			}else if(isGameMode ==2){
				if(rand()%13+1 == 1){
					if(isMine == isMaxmine){
						//isMineHard[i][j] = 0;
					}else{
						isMineHard[i][j] = Mine;
						isMineCount++;
						isMine++;
						isMineCreate+=2;

						for(int k=-1; k<=1; ++k){
							for(int l=-1;l<=1; ++l){
								if(i+k <= -1 || i+k >= 30 || j+l <= -1 || j+l  >= 16){
								}else{
									if( isMineHard[i+k][j+l] != Mine ){
										isMineHard[i+k][j+l]++;
									}
								}
							}
						}

					}
				}else{
					//isMineHard[i][j] = 0;
				}
			}
		}
	}
}

void isClick(int isMX, int isMY){
	int isX, isY;
	if( isGameMode == 0){
		isX = 9, isY = 9;
	}else if( isGameMode == 1){
		isX = 16, isY = 16;
	}else if( isGameMode == 2){
		isX = 30, isY = 16;
	}

	for(int k=-1; k<=1; ++k){
		for(int l=-1;l<=1; ++l){

			if(isMX+k <= -1 || isMX+k >= isX || isMY+l <= -1 || isMY+l  >= isY){
				//break;
			}else{
				//범위내에 도착시

				if( isGameMode == 0){
					if( isMineEasy[isMX+k][isMY+l] == 0 &&  isDrawEasy[isMX+k][isMY+l] == 0){
						isDrawEasy[isMX+k][isMY+l] = 1;
						isClick(isMX+k, isMY+l);
					}else{
						if( isMineEasy[isMX+k][isMY+l] != Mine &&  isDrawEasy[isMX+k][isMY+l] == 0){
							isDrawEasy[isMX+k][isMY+l] = 1;
						}
					}
				}

				if( isGameMode == 1){
					if( isMineNomal[isMX+k][isMY+l] == 0 &&  isDrawNomal[isMX+k][isMY+l] == 0){
						isDrawNomal[isMX+k][isMY+l] = 1;
						isClick(isMX+k, isMY+l);
					}else{
						if( isMineNomal[isMX+k][isMY+l] != Mine &&  isDrawNomal[isMX+k][isMY+l] == 0){
							isDrawNomal[isMX+k][isMY+l] = 1;
						}
					}
				}

				if( isGameMode == 2){
					if( isMineHard[isMX+k][isMY+l] == 0 &&  isDrawHard[isMX+k][isMY+l] == 0){
						isDrawHard[isMX+k][isMY+l] = 1;
						isClick(isMX+k, isMY+l);
					}else{
						if( isMineHard[isMX+k][isMY+l] != Mine){
							isDrawHard[isMX+k][isMY+l] = 1;
						}
					}
				}

			}
		}
	}
}

void isDrawReset(){
	isMineCount = 0;
	isMineGameWin = 0;
	isStartTime = GetTickCount();
	for(int i=0; i<9; ++i){
		for(int j=0; j<9; ++j){
			isDrawEasy[i][j] = 0;
		}
	}
	for(int i=0; i<16; ++i){
		for(int j=0; j<16; ++j){
			isDrawNomal[i][j] = 0;
		}
	}
	for(int i=0; i<30; ++i){
		for(int j=0; j<16; ++j){
			isDrawHard[i][j] = 0;
		}
	}
}

void isGameOver(){
	int isX, isY;
	if( isGameMode == 0){
		isX = 9, isY = 9;
	}else if( isGameMode == 1){
		isX = 16, isY = 16;
	}else if( isGameMode == 2){
		isX = 30, isY = 16;
	}

	for(int i=0; i<isX; ++i){
		for(int j=0; j<isY; ++j){

			if( isGameMode == 0){
				if(isDrawEasy[i][j] == Flag && isMineEasy[i][j] != Mine ){
					isDrawEasy[i][j] = NotMine;
				}else if( isMineEasy[i][j] == Mine &&  isDrawEasy[i][j]  != BombMine){
					isDrawEasy[i][j] = NomalMine;
				}else if(isDrawEasy[i][j] == BombMine){

				}else{
					isDrawEasy[i][j] = 1;
				}
			}else if( isGameMode == 1){
				if(isDrawNomal[i][j] == Flag && isDrawNomal[i][j] != Mine ){
					isDrawNomal[i][j] = NotMine;
				}else if( isMineNomal[i][j] == Mine &&  isDrawNomal[i][j]  != BombMine){
					isDrawNomal[i][j] = NomalMine;
				}else if(isDrawNomal[i][j] == BombMine){

				}else{
					isDrawNomal[i][j] = 1;
				}

			}else if( isGameMode == 2){
				if(isDrawHard[i][j] == Flag && isDrawHard[i][j] != Mine ){
					isDrawHard[i][j] = NotMine;
				}else if( isMineHard[i][j] == Mine &&  isDrawHard[i][j]  != BombMine){
					isDrawHard[i][j] = NomalMine;
				}else if(isDrawHard[i][j] == BombMine){

				}else{
					isDrawHard[i][j] = 1;
				}
			}


		}
	}
}

void isGameWin(){
	int isX, isY;
	int isCount=0, isCountTwo=0;
	if( isGameMode == 0){
		isX = 9, isY = 9;
	}else if( isGameMode == 1){
		isX = 16, isY = 16;
	}else if( isGameMode == 2){
		isX = 30, isY = 16;
	}

	for(int i=0; i<isX; ++i){
		for(int j=0; j<isY; ++j){
			if( isGameMode == 0){
				if(isDrawEasy[i][j] != 0){
					isCount++;
				}
			}else if( isGameMode == 1){
				if(isDrawNomal[i][j] != 0){
					isCount++;
				}
			}else if( isGameMode == 2){
				if(isDrawHard[i][j] != 0){
					isCount++;
				}
			}
		}
	}

	if( isCount == isX*isY ){

		for(int i=0; i<isX; ++i){
			for(int j=0; j<isY; ++j){
				if( isGameMode == 0){
					if( isMineEasy[i][j] == Mine && isDrawEasy[i][j] == Flag){
						isCountTwo++;
					}
				}else if( isGameMode == 1){
					if( isMineNomal[i][j] == Mine && isDrawNomal[i][j] == Flag){
						isCountTwo++;
					}
				}else if( isGameMode == 2){
					if( isMineHard[i][j] == Mine && isDrawHard[i][j] == Flag){
						isCountTwo++;
					}
				}
			}
		}

		if(isMineCount == isCountTwo){
			isMineGameWin =1;
			isEndTime = GetTickCount();
		}
	}
}