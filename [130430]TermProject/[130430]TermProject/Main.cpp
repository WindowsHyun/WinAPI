//#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "User32.lib") // 투명 윈도우를 위한 부분
#pragma comment(lib, "winmm.lib") 

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <mmsystem.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;

#define isGameCloudY 66 //22로 하면 디폴트
#define isGameCloudX 5
#define isRadCloudJumpDistance 320
#define isRabitJumpDistance 13
//---------------------------------------------------------
#define WINVER 0x500 
#define _WIN32_WINNT 0x500 
#define SWP_NOMOVE 0x0002
//---------------------------------------------------------

// 밟은 구름은 및으로 내려가면서 없어진다
// 그외 다른색 구름은 빠르게 올라간다..!
// 일정시간이나 구름의 색이 바뀌게 된다..!

int isGameCloud[isGameCloudY][isGameCloudX];
int isCreateCloudNum =0, isCheck=0, isMoveCloud =5, isMoveCloudDistance=1, isRabitJump = isRabitJumpDistance;
int isScore = 0, isRealScore[7], isCloudColor =1;
int isGameState = 0, isGamePauseMode =0, isGameWhatTimer; // 0 = StartPage, 1 = Start, 2 = Multi-Play, 3 = GameOver
int isSuperMode =0, isStartTime =0, isEndTime =0, isSuperJump =0, isAlpha = 255;
int isRandCloudRGB =0, isRabbitJumpTimer = 35, isAlphaTimer =0, isFadeInOut =0;

int isTestMode =2;

int isWhatPlayUser = -1;
int isOnePlayUserScore = 0, isTwoPlayUserScore =0;
int isOnePlayUserTime = 0, isTwoPlayUserTime =0;

int isRabbitLR=0, isRabbitState=0, isJump=0; // Defalut Left = 0
int isX=370, isY=545;

HBITMAP isBackgroung, isCharacterLJump[3], isCharacterRJump[3], isMaskCharacterLJump[3];
HBITMAP isCharcterHighJump, isMaskCharcterHighJump, isMaskCharacterRJump[3];
HBITMAP hbmOld, hbmMem, hbmMemOld;
int mWidth, mHeight, bx, by, isBackGroundY;

void isGameFirstStart();
void isCloudMove();
void isRandCloud();
void isPauseGame(HWND);
void isFadeOut(HWND);
void isFadeIn(HWND);
void isOverGame(HWND);
void isGamePlay(HWND, int);
void isGameEnd(HWND);
void isDrawRabbit(HDC, HDC, BITMAP);
void isDrawMap(HDC, HDC, BITMAP, RECT, int);
void isDrawMask(HDC, HDC, HBITMAP, BITMAP, int, int); // mem0dc, mem1dc, 그려줄 그림, imageSize, 좌표x, 좌표y
void isDrawPAINT(HDC, HDC, HBITMAP, BITMAP, int, int); // mem0dc, mem1dc, 그려줄 그림, imageSize, 좌표x, 좌표y


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow){
	g_hInst=hInstance;
	HWND 	hwnd;
	MSG 		msg;
	WNDCLASS	WndClass; 
	WndClass.style 	= CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc	= WndProc;		
	WndClass.cbClsExtra	= 0;		
	WndClass.cbWndExtra	= 0;		
	WndClass.hInstance 	= hInstance;		
	WndClass.hIcon	= LoadIcon(NULL, "IDI_ICON1");
	WndClass.hCursor 	= LoadCursor(NULL, IDC_ARROW);	
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	
	WndClass.lpszMenuName	= NULL;		
	WndClass.lpszClassName 	= "CrageneRabbit";	
	RegisterClass(&WndClass);	
	hwnd = CreateWindow("CrageneRabbit", "Cragene Rabbit", WS_POPUP|WS_CAPTION,300, 0, 480, 750, NULL, NULL, hInstance, NULL );
	// 1366*768 내 최대화면 기본 게임 화면 480*800 크기 적당히 안맞으니 조절..!
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);	
		DispatchMessage(&msg);	
	} 
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam){
	HDC hdc,mem0dc ,mem1dc;
	static HBITMAP isPlay[2], isMultiPlay[2], isExit[2], isCloud, isCloudBlack, isPause, isRestart[2], isMenu[2];
	static HBITMAP isGameOver, isGetNumber[10], isScoreHbit, isCloudTransparent, isTotalTime, isPoint, isSecond;
	static HBITMAP isCloudOrange, isCloudRed, isMarskCloud, isMarskTransparentCloud, isMaskPause;
	static HBITMAP isMaskNumber, isMaskScore, isMaskTotalTime, isMaskPoint, isMaskSecond, isHelp;
	static HBITMAP isPlayUser[2], isContinue[2], isCloudPurple, isMenuMaskCloud, isMenuBigMaskCloud;
	static HBITMAP isHelpCloud[2], isRealPause;
	static BITMAP isImageSize;
	static PAINTSTRUCT ps;
	static HPEN hPen, oldPen;
	static RECT rt, isClickExit, isClickMultiPlay, isClickPlay, isRabbit, isJumpCloud, isClickRestart, isClickMenu, isClickHelp, isClickPause;
	static char debug[500];
	static int isGameoverY =0;
	static int isCloudMouseMove[6], isClickCloud=0;
	static int mx, my;
	switch(iMsg){

	case WM_LBUTTONDOWN:
		mx=LOWORD(lParam);
		my=HIWORD(lParam);

		if( isGameState == 0 ){
			if( mx >= isClickExit.left && mx <= isClickExit.right && my >= isClickExit.top && my<= isClickExit.bottom){
				//EXIT 버튼을 클릭
				SetTimer(hwnd,5,1,NULL);
			}else 	if( mx >= isClickMultiPlay.left && mx <= isClickMultiPlay.right && my >= isClickMultiPlay.top && my<= isClickMultiPlay.bottom){
				//MultiPlay 버튼을 클릭
				isGamePlay(hwnd, 2);
			}else 	if( mx >= isClickPlay.left && mx <= isClickPlay.right && my >= isClickPlay.top && my<= isClickPlay.bottom){
				//Play 버튼을 클릭
				isWhatPlayUser = -1;
				isGamePlay(hwnd, 1);
			}else 	if( mx >= isClickHelp.left && mx <= isClickHelp.right && my >= isClickHelp.top && my<= isClickHelp.bottom){
				//Help 버튼을 클릭
				isFadeOut(hwnd);
				isFadeInOut =1;
				isGameState = 2; 
			}
		}else if( isGameState == 3 ){
			if( mx >= isClickRestart.left && mx <= isClickRestart.right && my >= isClickRestart.top && my<= isClickRestart.bottom){
				// GameOver 이후 Restart 눌렀을경우..!
				if(isWhatPlayUser == 0){
					isWhatPlayUser =1;
					isGamePlay(hwnd, 1);
				}else if(isWhatPlayUser == 1){
					isWhatPlayUser =0;
					isGamePlay(hwnd, 2);
				}else{
					isWhatPlayUser = -1;
					isGamePlay(hwnd, 1);
				}
			}else 	if( mx >= isClickMenu.left && mx <= isClickMenu.right && my >= isClickMenu.top && my<= isClickMenu.bottom){
				// GameOver 이후 Menu 눌렀을경우..!
				if(isWhatPlayUser != 0){
					isFadeOut(hwnd);
					isFadeInOut =1;
					SetTimer(hwnd,1,10,NULL);
					SetTimer(hwnd,2,isRabbitJumpTimer,NULL);
					isJump=0;
					isX=370, isY=545;
					isWhatPlayUser = -1;
					isGameState=0;
				}
			}

		}else if( isGameState == 2 ){
			// 도움말에서 아무곳이나 클릭하면 빠져 나온다..!
			isFadeOut(hwnd);
			isFadeInOut =1;
			SetTimer(hwnd,1,10,NULL);
			SetTimer(hwnd,2,isRabbitJumpTimer,NULL);
			isJump=0;
			isX=370, isY=545;
			isGameState=0;
		}else if( isGameState == 1 ){
			//isClickPause.left = rt.right-88, isClickPause.right = rt.right, isClickPause.top = rt.bottom-40, isClickPause.bottom = rt.bottom+2;
			if( mx >= isClickPause.left && mx <= isClickPause.right && my >= isClickPause.top && my<= isClickPause.bottom){
				//일시정지를 클릭시
				isPauseGame(hwnd);
			}
		}
		break;

	case WM_MOUSEMOVE:
		mx=LOWORD(lParam);
		my=HIWORD(lParam);
		if( isGameState == 0 ){
			if( mx >= isClickExit.left && mx <= isClickExit.right && my >= isClickExit.top && my<= isClickExit.bottom){
				isCloudMouseMove[2] = 1;
			}else{
				isCloudMouseMove[2] = 0;
			}

			if( mx >= isClickMultiPlay.left && mx <= isClickMultiPlay.right && my >= isClickMultiPlay.top && my<= isClickMultiPlay.bottom){
				isCloudMouseMove[1] = 1;
			}else{
				isCloudMouseMove[1] = 0;
			}

			if( mx >= isClickPlay.left && mx <= isClickPlay.right && my >= isClickPlay.top && my<= isClickPlay.bottom){
				isCloudMouseMove[0] = 1;
			}else{
				isCloudMouseMove[0] = 0;
			}

			if( mx >= isClickHelp.left && mx <= isClickHelp.right && my >= isClickHelp.top && my<= isClickHelp.bottom){
				isCloudMouseMove[5] = 1;
			}else{
				isCloudMouseMove[5] = 0;
			}
		}else if( isGameState == 3 ){
			if( mx >= isClickRestart.left && mx <= isClickRestart.right && my >= isClickRestart.top && my<= isClickRestart.bottom){
				isCloudMouseMove[3] = 1;
			}else{
				isCloudMouseMove[3] = 0;
			}
			if( mx >= isClickMenu.left && mx <= isClickMenu.right && my >= isClickMenu.top && my<= isClickMenu.bottom){
				isCloudMouseMove[4] = 1;
			}else{
				isCloudMouseMove[4] = 0;
			}

		}
		break;

	case WM_KEYDOWN :
		if(wParam == VK_RIGHT){
			isRabbitLR =1;
		}else if(wParam == VK_LEFT){
			isRabbitLR=0;
		}
		break;

	case WM_CHAR:
		if(isGameState == 3){
			if(wParam == 'r' || wParam == 'R'){
				// GameOver 에서 재시작 키를 눌렀을 경우
				if(isWhatPlayUser == 0){
					isWhatPlayUser =1;
					isGamePlay(hwnd, 1);
				}else if(isWhatPlayUser == 1){
					isWhatPlayUser =0;
					isGamePlay(hwnd, 2);
				}else{
					isWhatPlayUser =-1;
					isGamePlay(hwnd, 1);
				}
			}else if( wParam == 'm' || wParam == 'M'){
				if(isWhatPlayUser != 0){
					isFadeOut(hwnd);
					isFadeInOut =1;
					SetTimer(hwnd,1,10,NULL);
					SetTimer(hwnd,2,isRabbitJumpTimer,NULL);
					isJump=0;
					isX=370, isY=545;
					isGameState=0;
				}
			}
		}else if(isGameState == 1){
			if(wParam == 'j' || wParam == 'J'){
				if( isSuperMode == 1){
					isSuperMode =0;
				}else{
					SetTimer(hwnd,2,isRabbitJumpTimer,NULL);
					KillTimer(hwnd,3);
					isSuperMode =1;
				}
			}else if( wParam == 'p' || wParam == 'P'){
				isPauseGame(hwnd);
			}
		}else if(isGameState == 0){
			if(wParam == 'p' || wParam == 'P'){
				//Play 게임모드로 변경합니다..!
				isWhatPlayUser = -1;
				isGamePlay(hwnd, 1);
			}else if(wParam == 'e' || wParam == 'E'){
				//EXIT 눌렀을 경우..!
				SetTimer(hwnd,5,1,NULL);
			}else if(wParam == 'm' || wParam == 'M'){
				//MultiPlay 게임모드로 변경합니다..!
				isGamePlay(hwnd, 2);
			}else if(wParam == 'c' || wParam == 'C'){
				//구름 색상 변경
				if(isTestMode >= 8){
					isTestMode=1;
				}else{
					isTestMode++;
				}
			}


		}
		break;

	case WM_TIMER:
		switch(wParam){

		case 1:
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE|SWP_SHOWWINDOW); // 최상위 윈도우 사용하기
			if( isGameState == 0 || isGameState == 3){
				isBackGroundY+=2;
			}
			if(isBackGroundY >= rt.bottom){
				isBackGroundY=0;
			}
			isRabbit.left = isX, isRabbit.top = isY, isRabbit.bottom = isY+73, isRabbit.right = isX+66;
			if( isGameState == 1 && isRabbit.top <= (rt.bottom/2)+50 ){
				isBackGroundY+=1;
				isCloudMove();
				if( isSuperMode != 1){
					isY+=2;
				}
			}

			if ( isSuperMode == 1){
				isBackGroundY+=5;
				isCloudMove();
				if( isSuperJump >= isRadCloudJumpDistance){
					isSuperMode=0;
					isSuperJump=0;
				}
				isSuperJump++;
			}

			if( isRabbit.bottom >= rt.bottom+100 && isGameState == 1){
				//게임 오버..!
				PlaySound(MAKEINTRESOURCE(IDR_WAVE4),g_hInst,SND_RESOURCE|SND_ASYNC);
				isFadeOut(hwnd);
				isFadeInOut =1;
				isOverGame(hwnd);
			}
			break;

		case 2:
			if( rt.bottom/2 <= isRabbit.bottom){
				isY-=10;
			}
			isJump++;
			if(isRabbitState != 2){
				isRabbitState++;
			}
			if(isJump>=isRabitJump && isSuperMode == 0){
				isJump=0;
				isRabbitState=0;
				isGameWhatTimer = 3;
				SetTimer(hwnd,3,isRabbitJumpTimer,NULL);
				KillTimer(hwnd,2);
			}

			isRabbit.left = isX, isRabbit.top = isY, isRabbit.bottom = isY+73, isRabbit.right = isX+66;
			break;

		case 3:
			if( isGameState != 0 ){
				isY+=12;
			}else{
				isY+=10;
			}
			isJump++;
			if(isRabbitState != 1){
				isRabbitState++;
			}
			if( isGameState == 0 || isGameState == 3 ){
				if(isJump>=isRabitJump){
					if( isGameState == 0 ){
						PlaySound(MAKEINTRESOURCE(IDR_WAVE5),g_hInst,SND_RESOURCE|SND_ASYNC);
					}
					isJump=0;
					isGameWhatTimer =2;
					isRabbitState=0;
					SetTimer(hwnd,2,isRabbitJumpTimer,NULL);
					KillTimer(hwnd,3);
				}
			}
			isRabbit.left = isX, isRabbit.top = isY, isRabbit.bottom = isY+73, isRabbit.right = isX+66;

			if( isGameState == 1 ){
				isBackGroundY+=2;
				for( int i=0; i<isGameCloudX; ++i){
					for(int j=0; j<isGameCloudY; ++j){
						if ( isGameCloud[j][i] == 1 || isGameCloud[j][i] == 2 || isGameCloud[j][i] == 4 || isGameCloud[j][i] == 5 || isGameCloud[j][i] == 6 || isGameCloud[j][i] == 7 ){

							bx = rt.right, by = rt.bottom;
							isRabbit.left = isX, isRabbit.top = isY, isRabbit.bottom = isY+73, isRabbit.right = isX+66;
							isJumpCloud.left = bx/isGameCloudX*i, isJumpCloud.top = by/isGameCloudY*j, isJumpCloud.right = (bx/isGameCloudX*i)+100, isJumpCloud.bottom = (by/isGameCloudY*j)+40;

							if( isRabbit.bottom <= by/isGameCloudY*(j+4) && isRabbit.bottom >= by/isGameCloudY*j && (bx/isGameCloudX*i)-30 <= isRabbit.left && (bx/isGameCloudX*i)+120 >= isRabbit.right ){
								PlaySound(MAKEINTRESOURCE(IDR_WAVE5),g_hInst,SND_RESOURCE|SND_ASYNC);
								Sleep(10);
								if( j !=0){
									if(isGameCloud[j][i] == 1){
										isGameCloud[j][i] = 2;
									}else if(isGameCloud[j][i] == 2){
										isGameCloud[j][i] = 3;
									}else if(isGameCloud[j][i] == 7){
										isAlphaTimer=0;
										SetTimer(hwnd,8,100,NULL);
										isGameCloud[j][i] = 3;
									}else if(isGameCloud[j][i] == 5){
										//Jump
										PlaySound(MAKEINTRESOURCE(IDR_WAVE6),g_hInst,SND_RESOURCE|SND_ASYNC);
										isGameCloud[j][i] = 3;
										isSuperJump =0;
										isSuperMode =1;
									}else if(isGameCloud[j][i] == 6){
										//GameOver..
										if ( isSuperMode != 1){
											isAlphaTimer =0;
											SetTimer(hwnd,10,1000,NULL);
											isGameCloud[j][i] = 3;
											//isOverGame(hwnd);
										}
									}

								}
								isScore++;
								isJump=0;
								isRabbitState=0;
								isClickCloud =1;
								KillTimer(hwnd,3);
								SetTimer(hwnd,2,isRabbitJumpTimer,NULL);
								break;
							}else{
								isClickCloud=0;
							}
						}


					}
				}
			}
			break;

		case 4:
			if(isRabbitLR == 1){
				isX+=3;
				if(isX>rt.right){
					isX=rt.left;
					isRabbitLR = 1;
				}
			}else{
				isX-=3;
				if(isX<rt.left){
					isX=rt.right;
					isRabbitLR = 0;
				}
			}
			isRabbit.left = isX, isRabbit.top = isY, isRabbit.bottom = isY+73, isRabbit.right = isX+66;
			break;

		case 5:
			if(isAlpha <= 0){
				isGameEnd(hwnd);
			}else{
				SetWindowLong ( hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED );
				SetLayeredWindowAttributes(hwnd, 0, isAlpha, LWA_ALPHA);
				KillTimer(hwnd,6);
				KillTimer(hwnd,7);
				isAlpha-=3;
			}
			break;

		case 6:
			if(isAlpha >= 255){
				isStartTime = GetTickCount();
				SetTimer(hwnd,1,10,NULL);
				SetTimer(hwnd,2,isRabbitJumpTimer,NULL);
				SetTimer(hwnd,4,10,NULL); 
				KillTimer(hwnd,6);
			}else{
				SetWindowLong ( hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED );
				SetLayeredWindowAttributes(hwnd, 0, isAlpha, LWA_ALPHA);
				isAlpha+=5;
			}
			break;

		case 7:
			if(isAlpha >= 255){
				KillTimer(hwnd,7);
			}else{
				SetWindowLong ( hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED );
				SetLayeredWindowAttributes(hwnd, 0, isAlpha, LWA_ALPHA);
				KillTimer(hwnd,6);
				KillTimer(hwnd,5);
				isAlpha+=3;
			}
			break;

		case 8:
			if( isGameState == 1 ){
				if(isAlphaTimer >= 100){
					isAlpha = 255;
					isAlphaTimer=0;
					SetWindowLong ( hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED );
					SetLayeredWindowAttributes(hwnd, 0, isAlpha, LWA_ALPHA);
					KillTimer(hwnd,8);
				}else{
					isAlpha = 100;
					SetWindowLong ( hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED );
					SetLayeredWindowAttributes(hwnd, 0, isAlpha, LWA_ALPHA);
					isAlphaTimer++;
				}
			}else{
				isAlpha = 255;
				isAlphaTimer=0;
				SetWindowLong ( hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED );
				SetLayeredWindowAttributes(hwnd, 0, isAlpha, LWA_ALPHA);
				KillTimer(hwnd,8);
			}
			break;

		case 9:
			for( int i=0; i<isGameCloudX; ++i){
				for(int j=0; j<isGameCloudY; ++j){

					if(isGameCloud[j][i] == 4){
						isGameCloud[j][i] = 8;
					}else if(isGameCloud[j][i] == 8){
						isGameCloud[j][i] = 4;
					}
				}
			}
			break;


		case 10:
			if(isAlphaTimer >= 10){
				KillTimer(hwnd,10);
			}else{
				for( int i=0; i<isGameCloudX; ++i){
					for(int j=0; j<isGameCloudY; ++j){
						if(isGameCloud[j][i] == 1){
							isGameCloud[j][i]=2;
						}
					}
				}
				isAlphaTimer++;
			}
			break;

		}

		InvalidateRgn(hwnd, NULL, FALSE);
		break;

	case WM_CREATE:
		GetClientRect(hwnd,&rt);
		SetTimer(hwnd,1,10,NULL);
		SetTimer(hwnd,2,isRabbitJumpTimer,NULL);
		isAlpha=0;
		SetWindowLong ( hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED );
		SetLayeredWindowAttributes(hwnd, 0, isAlpha, LWA_ALPHA);
		SetTimer(hwnd,7,1,NULL);

		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE|SWP_SHOWWINDOW); // 최상위 윈도우 사용하기

		isBackgroung = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2)); // 배경화면
		isCharacterLJump[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP3));
		isCharacterLJump[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP4));
		isCharacterLJump[2] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP5));
		isMaskCharacterLJump[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP42));
		isMaskCharacterLJump[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP43));
		isMaskCharacterLJump[2] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP44));
		isCharacterRJump[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP6));
		isCharacterRJump[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP7));
		isCharacterRJump[2] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP8));
		isMaskCharacterRJump[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP45));
		isMaskCharacterRJump[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP46));
		isMaskCharacterRJump[2] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP47));
		isPlay[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP9));
		isPlay[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP15));
		isMultiPlay[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP10));
		isMultiPlay[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP14));
		isExit[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP11));
		isExit[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP13));
		isCloud = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP12));
		isPause = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP16));
		isCloudBlack = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP17));
		isGameOver = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP18));
		isGetNumber[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP19));
		isGetNumber[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP20));
		isGetNumber[2] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP21));
		isGetNumber[3] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP22));
		isGetNumber[4] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP23));
		isGetNumber[5] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP24));
		isGetNumber[6] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP25));
		isGetNumber[7] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP26));
		isGetNumber[8] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP27));
		isGetNumber[9] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP28));
		isScoreHbit = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP29));
		isCloudTransparent = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP30));
		isTotalTime = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP31));
		isPoint = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP32));
		isSecond = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP33));
		isRestart[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP34));
		isRestart[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP35));
		isMenu[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP36));
		isMenu[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP37));
		isCloudOrange = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP38));
		isCloudRed = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP39));
		isMarskCloud = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP40));
		isMarskTransparentCloud = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP41));
		isMaskNumber = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP48));
		isMaskScore = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP49));
		isMaskTotalTime = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP50));
		isMaskPoint = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP51));
		isMaskSecond = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP52));
		isMaskPause = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP53));
		isCharcterHighJump = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP54));
		isMaskCharcterHighJump = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP55));
		isPlayUser[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP56));
		isPlayUser[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP57));
		isContinue[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP58));
		isContinue[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP59));
		isCloudPurple = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP60));
		isMenuMaskCloud = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP61));
		isMenuBigMaskCloud = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP62));
		isHelp = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP63));
		isHelpCloud[0] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP64));
		isHelpCloud[1] = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP65));
		isRealPause = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP66));
		isClickExit.left = 10, isClickExit.top = 415, isClickExit.bottom = 500, isClickExit.right = 350;
		isClickMultiPlay.left = 120, isClickMultiPlay.top = 245, isClickMultiPlay.bottom = 335, isClickMultiPlay.right = 464;
		isClickPlay.left = 10, isClickPlay.top = 85, isClickPlay.bottom = 175, isClickPlay.right = 350;
		isRabbit.left = 240, isRabbit.top = 535, isRabbit.bottom = 608, isRabbit.right = 305;
		isClickRestart.left =10, isClickRestart.top = 450, isClickRestart.bottom = 540, isClickRestart.right = 350;
		isClickMenu.left = 120, isClickMenu.top = 580, isClickMenu.bottom = 675, isClickMenu.right = 460;
		isClickHelp.left = 350, isClickHelp.right = 444, isClickHelp.top = 610, isClickHelp.bottom = 645;
		isClickPause.left = rt.right-88, isClickPause.right = rt.right, isClickPause.top = rt.bottom-40, isClickPause.bottom = rt.bottom+2;
		isCloudMouseMove[0] = 0, isCloudMouseMove[1] = 0, isCloudMouseMove[2] = 0;
		isCloudMouseMove[3] = 0, isCloudMouseMove[4] = 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		mem0dc = CreateCompatibleDC(hdc);//2
		hbmMem = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);//3
		hbmMemOld = (HBITMAP)SelectObject(mem0dc, hbmMem);//4
		mem1dc = CreateCompatibleDC(mem0dc);//5
		DeleteDC(mem1dc);//-5
		if( isGameState == 0 ){
			//-----------------------------------------------------------------------------------------------------------------------------------------------------
			//뒷배경 그리기
			isDrawMap( mem0dc, mem1dc, isImageSize, rt, 1);
			//-----------------------------------------------------------------------------------------------------------------------------------------------------
			// 토끼 구현
			isDrawRabbit(mem0dc, mem1dc, isImageSize);
			//-----------------------------------------------------------------------------------------------------------------------------------------------------
			isDrawMask(mem0dc, mem1dc, isMenuMaskCloud , isImageSize, 10, 80);
			// Play 버튼
			isDrawPAINT(mem0dc, mem1dc, isPlay[isCloudMouseMove[0]] , isImageSize, 10, 80);
			//-----------------------------------------------------------------------------------------------------------------------------------------------------
			isDrawMask(mem0dc, mem1dc, isMenuMaskCloud , isImageSize, 120, 240);
			//멀티플레이 버튼
			isDrawPAINT(mem0dc, mem1dc, isMultiPlay[isCloudMouseMove[1]] , isImageSize, 120, 240);
			//-----------------------------------------------------------------------------------------------------------------------------------------------------
			isDrawMask(mem0dc, mem1dc, isMenuMaskCloud , isImageSize, 10, 410);
			//Exit 버튼
			isDrawPAINT(mem0dc, mem1dc, isExit[isCloudMouseMove[2]] , isImageSize, 10, 410);
			//-----------------------------------------------------------------------------------------------------------------------------------------------------
			isDrawMask(mem0dc, mem1dc, isMarskCloud , isImageSize, 350, 610);
			// 작은구름
			isDrawPAINT(mem0dc, mem1dc, isHelpCloud[isCloudMouseMove[5]] , isImageSize, 350, 610);
			//-----------------------------------------------------------------------------------------------------------------------------------------------------
		}else if( isGameState == 1 ){
			// Play 모드에서의 게임 방식
			//----------------------------------------------------------------------------------------------------------------------------------------------------
			// 뒷배경 그리기
			isDrawMap( mem0dc, mem1dc, isImageSize, rt, 0);
			//----------------------------------------------------------------------------------------------------------------------------------------------------
			isDrawMask(mem0dc, mem1dc, isScoreHbit , isImageSize, 0, rt.bottom-40);
			// Score 입력 부분 isScoreHbit
			isDrawPAINT(mem0dc, mem1dc, isMaskScore , isImageSize, 0, rt.bottom-40);
			//----------------------------------------------------------------------------------------------------------------------------------------------------
			for(int i=0; i<7; ++i){
				isRealScore[i]=0;
			}

			int i=6, tempScore = isScore, isStartData=6;

			while(tempScore!=0){
				// 글자를 뒤집어서 배열에 넣는다..!
				isRealScore[i] = tempScore%10;
				i--;
				tempScore=tempScore/10;
			}
			for(int i=0; i<7; ++i){
				if(isRealScore[i] != 0){
					isStartData = i;
					break;
				}
			}

			for(int i=isStartData; i<7; ++i){
				isDrawMask(mem0dc, mem1dc, isGetNumber[isRealScore[i]] , isImageSize, 120+(i*17), rt.bottom-39);
				// 숫자 출력
				isDrawPAINT(mem0dc, mem1dc, isMaskNumber , isImageSize, 120+(i*17), rt.bottom-39);
			}

			if( isGamePauseMode == 0 ){
				//----------------------------------------------------------------------------------------------------------------------------------------------------
				// 토끼 그리기
				isDrawRabbit(mem0dc, mem1dc, isImageSize);
				//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				for( int i=0; i<isGameCloudX; ++i){
					for(int j=0; j<isGameCloudY; ++j){
						if ( isGameCloud[j][i] == 1 || isGameCloud[j][i] == 2 || isGameCloud[j][i] == 3 || isGameCloud[j][i] == 4 || isGameCloud[j][i] == 5 || isGameCloud[j][i] == 6 || isGameCloud[j][i] == 7 || isGameCloud[j][i] == 8 ){
							if ( isGameCloud[j][i] == 3 ||  isGameCloud[j][i] == 8 ){
								isDrawMask(mem0dc, mem1dc, isMarskTransparentCloud , isImageSize, bx/isGameCloudX*i, by/isGameCloudY*j);
							}else{
								isDrawMask(mem0dc, mem1dc, isMarskCloud , isImageSize, bx/isGameCloudX*i, by/isGameCloudY*j);
							}
							// 작은구름
							if ( isGameCloud[j][i] == 1 ){
								isDrawPAINT(mem0dc, mem1dc, isCloud , isImageSize, bx/isGameCloudX*i, by/isGameCloudY*j);
							}else if ( isGameCloud[j][i] == 2 ){
								isDrawPAINT(mem0dc, mem1dc, isCloudBlack , isImageSize, bx/isGameCloudX*i, by/isGameCloudY*j);
							}else if ( isGameCloud[j][i] == 3 ||  isGameCloud[j][i] == 8 ){
								isDrawPAINT(mem0dc, mem1dc, isCloudTransparent , isImageSize, bx/isGameCloudX*i, by/isGameCloudY*j);
							}else if ( isGameCloud[j][i] == 4 ){
								isDrawPAINT(mem0dc, mem1dc, isCloudOrange , isImageSize, bx/isGameCloudX*i, by/isGameCloudY*j);
							}else if ( isGameCloud[j][i] == 5 ){
								isDrawPAINT(mem0dc, mem1dc, isCloudRed , isImageSize, bx/isGameCloudX*i, by/isGameCloudY*j);
							}else if ( isGameCloud[j][i] == 6 ){
								isDrawPAINT(mem0dc, mem1dc, isCloudPurple , isImageSize, bx/isGameCloudX*i, by/isGameCloudY*j);
							}else if ( isGameCloud[j][i] == 7 ){
								isDrawPAINT(mem0dc, mem1dc, isCloudTransparent , isImageSize, bx/isGameCloudX*i, by/isGameCloudY*j);
							}
						}
					}
				}
				//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			}else{
				isDrawMask(mem0dc, mem1dc, isMenuBigMaskCloud , isImageSize, rt.right/10, rt.bottom/3);
				// 일시정지 그림 띄우기
				isDrawPAINT(mem0dc, mem1dc, isRealPause , isImageSize, rt.right/10, rt.bottom/3);
			}
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			isDrawMask(mem0dc, mem1dc, isPause , isImageSize, rt.right-88, rt.bottom-40);
			// 일시정지
			isDrawPAINT(mem0dc, mem1dc, isMaskPause , isImageSize, rt.right-88, rt.bottom-40);
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		}else if( isGameState == 2 ){
			mem1dc = CreateCompatibleDC(mem0dc);//5
			hbmOld = (HBITMAP)SelectObject(mem1dc, isHelp);//6
			StretchBlt(mem0dc, 0,0,rt.right,rt.bottom,mem1dc,0,0,470,700,SRCCOPY);
			SelectObject(mem1dc, hbmOld);//-6
			DeleteDC(mem1dc);//-5
		}else if( isGameState == 3 ){
			//GameOver..!
			//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			// 뒷배경 그리기
			isDrawMap( mem0dc, mem1dc, isImageSize, rt, 3);
			//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			if( isWhatPlayUser == -1 ){
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isMenuBigMaskCloud , isImageSize, rt.right/13, 30);
				// GameOver
				isDrawPAINT(mem0dc, mem1dc, isGameOver , isImageSize, rt.right/13, 30);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isScoreHbit , isImageSize, rt.right/4, 240);
				// Score
				isDrawPAINT(mem0dc, mem1dc, isMaskScore , isImageSize, rt.right/4, 240);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				for(int i=0; i<7; ++i){
					isRealScore[i]=0;
				}

				int i=6, tempScore = isScore, isStartData=6;

				while(tempScore!=0){
					// 글자를 뒤집어서 배열에 넣는다..!
					isRealScore[i] = tempScore%10;
					i--;
					tempScore=tempScore/10;
				}

				for(int i=0; i<7; ++i){
					if(isRealScore[i] != 0){
						isStartData = i;
						break;
					}
				}

				for(int i=isStartData; i<7; ++i){
					isDrawMask(mem0dc, mem1dc, isGetNumber[isRealScore[i]] , isImageSize, rt.right/4+120+(i*17), 240);
					// 숫자 출력
					isDrawPAINT(mem0dc, mem1dc, isMaskNumber , isImageSize, rt.right/4+120+(i*17), 240);
				}
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isPoint , isImageSize, rt.right/4+123+(7*17), 240);
				// 점
				isDrawPAINT(mem0dc, mem1dc, isMaskPoint , isImageSize, rt.right/4+123+(7*17), 240);
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isTotalTime , isImageSize, rt.right/26, 320);
				// TotalTime
				isDrawPAINT(mem0dc, mem1dc, isMaskTotalTime , isImageSize, rt.right/26, 320);
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				int isPlayTime = (isEndTime-isStartTime)/1000;

				for(int i=0; i<7; ++i){
					isRealScore[i]=0;
				}

				i=6, tempScore = isPlayTime, isStartData=6;
				while(tempScore!=0){
					// 글자를 뒤집어서 배열에 넣는다..!
					isRealScore[i] = tempScore%10;
					i--;
					tempScore=tempScore/10;
				}

				for(int i=0; i<7; ++i){
					if(isRealScore[i] != 0){
						isStartData = i;
						break;
					}
				}

				for(int i=isStartData; i<7; ++i){
					isDrawMask(mem0dc, mem1dc, isGetNumber[isRealScore[i]] , isImageSize, rt.right/4+120+(i*17), 320);
					//숫자 출력
					isDrawPAINT(mem0dc, mem1dc, isMaskNumber , isImageSize, rt.right/4+120+(i*17), 320);
				}
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isSecond , isImageSize, rt.right/4+123+(7*17), 321);
				//isMaskSecond
				isDrawPAINT(mem0dc, mem1dc, isMaskSecond , isImageSize, rt.right/4+123+(7*17), 321);
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isMenuMaskCloud , isImageSize, 10, 450);
				//Restart
				isDrawPAINT(mem0dc, mem1dc, isRestart[isCloudMouseMove[3]] , isImageSize, 10, 450);
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isMenuMaskCloud , isImageSize, 120, 580);
				//Menu
				isDrawPAINT(mem0dc, mem1dc, isMenu[isCloudMouseMove[4]] , isImageSize, 120, 580);
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			}else{ // 멀티플레이에서 게임 오버일경우~! ↓
				if(isWhatPlayUser == 0){
					isOnePlayUserScore = isScore;
					isOnePlayUserTime = (isEndTime-isStartTime)/1000;
				}else if(isWhatPlayUser == 1){
					isTwoPlayUserScore = isScore;
					isTwoPlayUserTime = (isEndTime-isStartTime)/1000;
				}
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isMenuMaskCloud , isImageSize, 10, 10);
				//User1
				isDrawPAINT(mem0dc, mem1dc, isPlayUser[0] , isImageSize, 10, 10);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isScoreHbit , isImageSize, rt.right/3, 120);
				//User1
				isDrawPAINT(mem0dc, mem1dc, isMaskScore, isImageSize, rt.right/3, 120);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				for(int i=0; i<7; ++i){
					isRealScore[i]=0;
				}

				int i=6, tempScore = isOnePlayUserScore, isStartData=6;

				while(tempScore!=0){
					// 글자를 뒤집어서 배열에 넣는다..!
					isRealScore[i] = tempScore%10;
					i--;
					tempScore=tempScore/10;
				}

				for(int i=0; i<7; ++i){
					if(isRealScore[i] != 0){
						isStartData = i;
						break;
					}
				}

				for(int i=isStartData; i<7; ++i){
					isDrawMask(mem0dc, mem1dc, isGetNumber[isRealScore[i]] , isImageSize, rt.right/3+120+(i*17), 120);
					//숫자 출력
					isDrawPAINT(mem0dc, mem1dc, isMaskNumber, isImageSize, rt.right/3+120+(i*17), 120);
				}
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isPoint , isImageSize, rt.right/3+123+(7*17), 120);
				//점
				isDrawPAINT(mem0dc, mem1dc, isMaskPoint, isImageSize, rt.right/3+123+(7*17), 120);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isTotalTime , isImageSize, (rt.right/9)+2, 160);
				// TotalTime
				isDrawPAINT(mem0dc, mem1dc, isMaskTotalTime, isImageSize, (rt.right/9)+2, 160);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				int isPlayTime = isOnePlayUserTime;

				for(int i=0; i<7; ++i){
					isRealScore[i]=0;
				}

				i=6, tempScore = isPlayTime, isStartData=6;
				while(tempScore!=0){
					// 글자를 뒤집어서 배열에 넣는다..!
					isRealScore[i] = tempScore%10;
					i--;
					tempScore=tempScore/10;
				}

				for(int i=0; i<7; ++i){
					if(isRealScore[i] != 0){
						isStartData = i;
						break;
					}
				}

				for(int i=isStartData; i<7; ++i){
					isDrawMask(mem0dc, mem1dc, isGetNumber[isRealScore[i]] , isImageSize, rt.right/3+120+(i*17), 160);
					//숫자 출력
					isDrawPAINT(mem0dc, mem1dc, isMaskNumber, isImageSize, rt.right/3+120+(i*17), 160);
				}
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isSecond , isImageSize, rt.right/3+123+(7*17), 160);
				//isMaskSecond
				isDrawPAINT(mem0dc, mem1dc, isMaskSecond, isImageSize, rt.right/3+123+(7*17), 160);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isMenuMaskCloud , isImageSize, rt.right-355, 225);
				//User2
				isDrawPAINT(mem0dc, mem1dc, isPlayUser[1], isImageSize, rt.right-355, 225);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isScoreHbit , isImageSize, (rt.right/4)-6, 340);
				// Score
				isDrawPAINT(mem0dc, mem1dc, isMaskScore, isImageSize, (rt.right/4)-6, 340);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				for(int i=0; i<7; ++i){
					isRealScore[i]=0;
				}

				i=6, tempScore = isTwoPlayUserScore, isStartData=6;

				while(tempScore!=0){
					// 글자를 뒤집어서 배열에 넣는다..!
					isRealScore[i] = tempScore%10;
					i--;
					tempScore=tempScore/10;
				}

				for(int i=0; i<7; ++i){
					if(isRealScore[i] != 0){
						isStartData = i;
						break;
					}
				}

				for(int i=isStartData; i<7; ++i){
					isDrawMask(mem0dc, mem1dc, isGetNumber[isRealScore[i]] , isImageSize, rt.right/4+120+(i*17), 340);
					//숫자 출력
					isDrawPAINT(mem0dc, mem1dc, isMaskNumber, isImageSize, rt.right/4+120+(i*17), 340);
				}
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isPoint, isImageSize, rt.right/4+123+(7*17), 340);
				//점
				isDrawPAINT(mem0dc, mem1dc, isMaskPoint, isImageSize, rt.right/4+123+(7*17), 340);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isTotalTime, isImageSize, 10, 380);
				// Score
				isDrawPAINT(mem0dc, mem1dc, isMaskScore, isImageSize, 10, 380);
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isPlayTime = isTwoPlayUserTime;

				for(int i=0; i<7; ++i){
					isRealScore[i]=0;
				}

				i=6, tempScore = isPlayTime, isStartData=6;
				while(tempScore!=0){
					// 글자를 뒤집어서 배열에 넣는다..!
					isRealScore[i] = tempScore%10;
					i--;
					tempScore=tempScore/10;
				}

				for(int i=0; i<7; ++i){
					if(isRealScore[i] != 0){
						isStartData = i;
						break;
					}
				}

				for(int i=isStartData; i<7; ++i){
					isDrawMask(mem0dc, mem1dc, isGetNumber[isRealScore[i]], isImageSize, rt.right/4+120+(i*17), 380);
					//숫자 출력
					isDrawPAINT(mem0dc, mem1dc, isMaskNumber, isImageSize, rt.right/4+120+(i*17), 380);
				}
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isSecond, isImageSize, rt.right/4+123+(7*17), 380);
				//isMaskSecond
				isDrawPAINT(mem0dc, mem1dc, isMaskSecond, isImageSize, rt.right/4+123+(7*17), 380);
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				isDrawMask(mem0dc, mem1dc, isMenuMaskCloud, isImageSize, 10, 450);
				//Restart
				if(isWhatPlayUser == 0){
					isDrawPAINT(mem0dc, mem1dc, isContinue[isCloudMouseMove[3]], isImageSize, 10, 450);
				}else{
					isDrawPAINT(mem0dc, mem1dc, isRestart[isCloudMouseMove[3]], isImageSize, 10, 450);
				}
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if(isWhatPlayUser == 1){
					isDrawMask(mem0dc, mem1dc, isMenuMaskCloud, isImageSize, 120, 580);
					//Menu isCloudTransparent
					isDrawPAINT(mem0dc, mem1dc, isMenu[isCloudMouseMove[4]], isImageSize, 120, 580);
				}
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			}
		}

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mem0dc, 0, 0, SRCCOPY);
		//-------------------------------------------------------------------------------------------------------------
		/*hPen = CreatePen(PS_DOT, 2, RGB(255,0,0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		for(int i=1; i<(isGameCloudX); ++i){
		MoveToEx(hdc, rt.right/(isGameCloudX)*i, rt.top,NULL);
		LineTo(hdc, rt.right/(isGameCloudX)*i,rt.bottom);
		}
		for(int i=0; i<=isGameCloudY; ++i){
		if(i ==0){
		MoveToEx(hdc, rt.left,0,NULL);
		LineTo(hdc, rt.right,0);
		}else{
		MoveToEx(hdc, rt.left,rt.bottom/(isGameCloudY)*i,NULL);
		LineTo(hdc, rt.right,rt.bottom/(isGameCloudY)*i);
		}
		}
		SelectObject(hdc,oldPen);
		DeleteObject(hPen);*/
		//-------------------------------------------------------------------------------------------------------------
		if( isFadeInOut == 1 ){
			isFadeIn(hwnd);
			isFadeInOut=0;
		}
		SelectObject(mem0dc, hbmMemOld); //-4
		DeleteObject(hbmMem); //-3
		DeleteObject(hbmMemOld); //-3
		DeleteDC(mem0dc); //-2
		DeleteDC(mem1dc); //-2
		DeleteDC(hdc); //-2
		EndPaint( hwnd, &ps );
		break;

	case WM_DESTROY:
		isGameEnd(hwnd);
		break;
	}
	return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}

void isGameFirstStart(){
	int randX =0;
	int isReData[2];
	//srand(GetTickCount()%rand()%1000);
	srand((unsigned int)time(NULL));

	isReData[0] = -1;
	isReData[1] = -1;
	isMoveCloud =5, isMoveCloudDistance=1, isRabitJump = isRabitJumpDistance, isCheck =0, isCreateCloudNum =0;

	for(int i=0; i<isGameCloudY; ++i){
		for(int j=0; j<isGameCloudX; ++j){
			isGameCloud[i][j] = 0;
		}
	}

	for(int i=0; i<isGameCloudY; i++){
		isCreateCloudNum++;


		if(isCreateCloudNum == isMoveCloud){
			isCreateCloudNum =0;
			randX = rand()%isGameCloudX;
			if(isReData[1] == randX){
				while(1){
					if(isReData[1] != randX && randX <= isGameCloudX){
						break;
					}
					//Sleep(10);
					srand(GetTickCount()%rand()%2+isCheck);
					randX = rand()%isGameCloudX;
				}
				isGameCloud[i][randX] = 1;
				isReData[0] = i;
				isReData[1] = randX;
			}else{
				isGameCloud[i][randX] = 1;
				isReData[0] = i;
				isReData[1] = randX;
			}
		}
	}

	for(int i=0; i<isGameCloudX; ++i){
		//isGameCloud[64][i] = 2;
		isGameCloud[64][i] = isTestMode;
	}
}

void isCloudMove(){
	int isNext =0;
	int isArrT[5];
	int randX =0;
	int isReData=0;

	srand((unsigned int)time(NULL));

	for( int i=isGameCloudY-1; i>-1; --i){
		isNext = i;
		isNext -=1;

		if( i != 0 && isNext != -1 ){

			for(int j=0; j<isGameCloudX; ++j){
				isArrT[j] = isGameCloud[isNext][j];
			}
			for(int j=0; j<isGameCloudX; ++j){
				isGameCloud[i][j] = isArrT[j];
			}

		}else{
			for(int j=0; j<isGameCloudX; ++j){
				isGameCloud[0][randX] = 0;
			}

			//-----------------------------------------------------------------------------------

			randX = rand()%isGameCloudX;
			isCreateCloudNum++;
			isCheck++;
			isScore++;
			//isScore=isMoveCloudDistance;
			if( isCheck >= 250*isMoveCloudDistance){
				if( isRabitJump <= 40){
					isRabitJump+=2;
				}
				isMoveCloud++;
				isMoveCloudDistance++;
			}
			if(isCreateCloudNum == isMoveCloud){
				isCreateCloudNum =0;
				//Sleep(10);
				srand(isCheck%GetTickCount()+rand()%isCheck);
				randX = rand()%isGameCloudX;
				int isCount =0;
				if(isReData == randX){
					while(1){
						if(isReData != randX || isCount>=5 ){
							break;
						}
						//Sleep(10);
						srand(isCheck%GetTickCount()+rand()%isCheck);
						randX = rand()%isGameCloudX;
						isCount++;
					}
					if( rand()%10 == 3){
						randX = 0;
					}
					isRandCloud();
					isGameCloud[1][randX] = isCloudColor;
					isReData = randX;
				}else{
					if( rand()%10 == 4){
						randX = 0;
					}
					isRandCloud();
					isGameCloud[1][randX] = isCloudColor;
					isReData = randX;
				}
			}
			//-----------------------------------------------------------------------------------
		}

	}

}

void isRandCloud(){
	// 1 = 일반구름, 2 = 회색구름, 3= 투명한구름, 4 = 주황색구름, 5= 빨간색 구름, 6 =보라색 구름, 7=검정색 구름, 8=투명한주황색구름
	//Sleep(10);
	int isRandColor =0;
	srand(isCheck%GetTickCount()+rand()%isCheck);
	if(isCheck >= 100){
		if(rand()%5 == 3){
			isCloudColor =2; 
		}else if(rand()%5 == 4){
			isRandColor = isRandCloudRGB%4;
			if(isRandColor == 1){
				isCloudColor = 6;
			}else if(isRandColor == 2){
				isCloudColor = 5;
			}else if(isRandColor == 3){
				isCloudColor = 4;
			}else{
				isCloudColor = 7;
			}
			isRandCloudRGB++;
		}else{
			isCloudColor=1;
		}
	}else if(isCheck >= 300){
		if(rand()%5 == 4){
			isCloudColor =2;
		}else if(rand()%5 == 2){
			isRandColor = isRandCloudRGB%4;
			if(isRandColor == 1){
				isCloudColor = 6;
			}else if(isRandColor == 2){
				isCloudColor = 5;
			}else if(isRandColor == 3){
				isCloudColor = 4;
			}else{
				isCloudColor = 7;
			}
			isRandCloudRGB++;
		}else{
			isCloudColor=1;
		}
	}else if(isCheck >= 500){
		if(rand()%4 == 2){
			isCloudColor =2;
		}else if(rand()%4 == 3){
			isRandColor = isRandCloudRGB%4;
			if(isRandColor == 1){
				isCloudColor = 6;
			}else if(isRandColor == 2){
				isCloudColor = 5;
			}else if(isRandColor == 3){
				isCloudColor = 4;
			}else{
				isCloudColor = 7;
			}
			isRandCloudRGB++;
		}else{
			isCloudColor=1;
		}
	}else if(isCheck >= 1000){
		if(rand()%3 == 1){
			isCloudColor =2;
		}else if(rand()%3 == 2){
			isRandColor = isRandCloudRGB%4;
			if(isRandColor == 1){
				isCloudColor = 6;
			}else if(isRandColor == 2){
				isCloudColor = 5;
			}else if(isRandColor == 3){
				isCloudColor = 4;
			}else{
				isCloudColor = 7;
			}
			isRandCloudRGB++;
		}else{
			isCloudColor=1;
		}
	}else if(isCheck >= 2000){
		if(rand()%2 == 1){
			isCloudColor =2;
		}else if(rand()%2 == 1){
			isRandColor = isRandCloudRGB%4;
			if(isRandColor == 1){
				isCloudColor = 6;
			}else if(isRandColor == 2){
				isCloudColor = 5;
			}else if(isRandColor == 3){
				isCloudColor = 4;
			}else{
				isCloudColor = 7;
			}
			isRandCloudRGB++;
		}else{
			isCloudColor=1;
		}
	}else{
		isCloudColor=1;
	}
}

void isPauseGame(HWND hwnd){
	if( isGamePauseMode == 0 ){
		isGamePauseMode =1;
		KillTimer(hwnd,1);
		KillTimer(hwnd,2);
		KillTimer(hwnd,3);
		KillTimer(hwnd,4);
	}else{
		isGamePauseMode =0;
		if( isGameWhatTimer == 2){
			SetTimer(hwnd,1,10,NULL);
			SetTimer(hwnd,2,isRabbitJumpTimer,NULL);
			SetTimer(hwnd,4,10,NULL); 
			KillTimer(hwnd,6);
		}else if ( isGameWhatTimer == 3){
			SetTimer(hwnd,1,10,NULL);
			SetTimer(hwnd,3,isRabbitJumpTimer,NULL);
			SetTimer(hwnd,4,10,NULL); 
			KillTimer(hwnd,6);

		}
	}
}

void isFadeOut(HWND hwnd){
	while(1){
		if(isAlpha <= 0){
			break;
		}else{
			SetWindowLong ( hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED );
			SetLayeredWindowAttributes(hwnd, 0, isAlpha, LWA_ALPHA);
			isAlpha-=3;
			Sleep(5);
		}
	}
}

void isFadeIn(HWND hwnd){
	while(1){
		if(isAlpha >= 255){
			break;
		}else{
			SetWindowLong ( hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED );
			SetLayeredWindowAttributes(hwnd, 0, isAlpha, LWA_ALPHA);
			isAlpha+=3;
			Sleep(5);
		}
	}
}

void isOverGame(HWND hwnd){
	isEndTime = GetTickCount();
	isGameState = 3;
	isX=370, isY=545;
	KillTimer(hwnd,4);
	KillTimer(hwnd,9);
	KillTimer(hwnd,10);
}

void isGamePlay(HWND hwnd, int isWhatPlay){
	if(isAlpha >= 255){
		if( isWhatPlay == 1){
			//일반 플레이
		}else if( isWhatPlay == 2){
			// 멀티플레이
			isWhatPlayUser = 0;
			isOnePlayUserScore = 0, isTwoPlayUserScore =0;
			isOnePlayUserTime = 0, isTwoPlayUserTime =0;
		}
		isGameFirstStart();
		isGameState=1;
		isGamePauseMode =0;
		KillTimer(hwnd,1);
		KillTimer(hwnd,2);
		KillTimer(hwnd,3);
		isX=220, isY=575;
		isJump=0;
		isRabbitState=0;
		isMoveCloud =5, isMoveCloudDistance=1, isRabitJump = isRabitJumpDistance, isCheck =0, isScore = 0;
		isAlpha = 0;
		SetTimer(hwnd,6,1,NULL);
		SetTimer(hwnd,9,1000,NULL);
	}
}

void isGameEnd(HWND hwnd){
	KillTimer(hwnd,1);
	KillTimer(hwnd,2);
	KillTimer(hwnd,3);
	KillTimer(hwnd,4);
	KillTimer(hwnd,5);
	KillTimer(hwnd,6);
	KillTimer(hwnd,7);
	KillTimer(hwnd,8);
	KillTimer(hwnd,9);
	KillTimer(hwnd,10);
	PostQuitMessage(0);
}

void isDrawRabbit(HDC mem0dc, HDC mem1dc, BITMAP isImageSize){
	if( isSuperMode == 1){
		mem1dc = CreateCompatibleDC(mem0dc);//5
		hbmOld = (HBITMAP)SelectObject(mem1dc, isMaskCharcterHighJump);//6
		GetObject (isMaskCharcterHighJump, sizeof(BITMAP), &isImageSize);
		mWidth = isImageSize.bmWidth;
		mHeight = isImageSize.bmHeight;
		BitBlt(mem0dc,isX,isY,mWidth,mHeight,mem1dc,0,0,SRCAND);
		SelectObject(mem1dc, hbmOld);//-6
		DeleteDC(mem1dc);//-5
		//토끼 점프 구현
		mem1dc = CreateCompatibleDC(mem0dc);//5
		hbmOld = (HBITMAP)SelectObject(mem1dc, isCharcterHighJump);//6
		GetObject (isCharcterHighJump, sizeof(BITMAP), &isImageSize);
		mWidth = isImageSize.bmWidth;
		mHeight = isImageSize.bmHeight;
		BitBlt(mem0dc,isX,isY,mWidth,mHeight,mem1dc,0,0,SRCPAINT);
		SelectObject(mem1dc, hbmOld);//-6
		DeleteDC(mem1dc);//-5

	}else{
		mem1dc = CreateCompatibleDC(mem0dc);//5
		if( isRabbitLR == 0){
			hbmOld = (HBITMAP)SelectObject(mem1dc, isMaskCharacterLJump[isRabbitState]);//6
			GetObject (isMaskCharacterLJump[isRabbitState], sizeof(BITMAP), &isImageSize);
		}else{
			hbmOld = (HBITMAP)SelectObject(mem1dc, isMaskCharacterRJump[isRabbitState]);//6
			GetObject (isMaskCharacterRJump[isRabbitState], sizeof(BITMAP), &isImageSize);
		}
		mWidth = isImageSize.bmWidth;
		mHeight = isImageSize.bmHeight;
		BitBlt(mem0dc,isX,isY,mWidth,mHeight,mem1dc,0,0,SRCAND);
		SelectObject(mem1dc, hbmOld);//-6
		DeleteDC(mem1dc);//-5
		//토끼 점프 구현
		mem1dc = CreateCompatibleDC(mem0dc);//5
		if( isRabbitLR == 0){
			hbmOld = (HBITMAP)SelectObject(mem1dc, isCharacterLJump[isRabbitState]);//6
			GetObject (isCharacterLJump[isRabbitState], sizeof(BITMAP), &isImageSize);
		}else{
			hbmOld = (HBITMAP)SelectObject(mem1dc, isCharacterRJump[isRabbitState]);//6
			GetObject (isCharacterRJump[isRabbitState], sizeof(BITMAP), &isImageSize);
		}
		mWidth = isImageSize.bmWidth;
		mHeight = isImageSize.bmHeight;
		BitBlt(mem0dc,isX,isY,mWidth,mHeight,mem1dc,0,0,SRCPAINT);
		SelectObject(mem1dc, hbmOld);//-6
		DeleteDC(mem1dc);//-5
	}
}

void isDrawMap(HDC mem0dc, HDC mem1dc, BITMAP isImageSize, RECT rt, int isDebugMode){
	mem1dc = CreateCompatibleDC(mem0dc);//5
	hbmOld = (HBITMAP)SelectObject(mem1dc, isBackgroung);//6
	bx = rt.right, by = rt.bottom;
	BitBlt(mem0dc,0,isBackGroundY-by+2,bx,by,mem1dc,0,0,SRCCOPY);
	BitBlt(mem0dc,0,isBackGroundY,bx,by,mem1dc,0,0,SRCCOPY);
	if( isDebugMode == 1){
		char debug[500];
		wsprintf(debug, "isX : %d, isY : %d, CloudColor : %d", isX, isY, isTestMode);
		TextOut(mem0dc, 0,0, debug, strlen(debug));
	}else if(isDebugMode == 2){
		char debug[500];
		for( int i=0; i<isGameCloudX; ++i){
			for(int j=0; j<isGameCloudY; ++j){
				sprintf(debug, "%d", isGameCloud[j][i]);
				TextOut(mem0dc, 20+(i*10),10+(j*10), debug, strlen(debug));
			}
		}
	}else if(isDebugMode == 3){
		char debug[500];
		sprintf(debug, "Check : %d, Score : %d, Time : %d", isCheck, isScore, (isEndTime-isStartTime)/1000);
		TextOut(mem0dc, 0,rt.bottom-40, debug, strlen(debug));
		sprintf(debug, "isMoveCloudDistance : %d, isRabitJump : %d", isMoveCloudDistance, isRabitJump);
		TextOut(mem0dc, 0,rt.bottom-20, debug, strlen(debug));
	}
	SelectObject(mem1dc, hbmOld);//-6
	DeleteDC(mem1dc);//-5
}

void isDrawMask(HDC mem0dc, HDC mem1dc, HBITMAP isDrawImage , BITMAP isImageSize, int isDrawX, int isDrawY){
	mem1dc = CreateCompatibleDC(mem0dc);//5
	hbmOld = (HBITMAP)SelectObject(mem1dc, isDrawImage);//6
	GetObject (isDrawImage, sizeof(BITMAP), &isImageSize);
	mWidth = isImageSize.bmWidth;
	mHeight = isImageSize.bmHeight;
	BitBlt(mem0dc, isDrawX, isDrawY,mWidth,mHeight,mem1dc,0,0,SRCAND);
	SelectObject(mem1dc, hbmOld);//-6
	DeleteDC(mem1dc);//-5
}

void isDrawPAINT(HDC mem0dc, HDC mem1dc, HBITMAP isDrawImage , BITMAP isImageSize, int isDrawX, int isDrawY){
	mem1dc = CreateCompatibleDC(mem0dc);//5
	hbmOld = (HBITMAP)SelectObject(mem1dc, isDrawImage);//6
	GetObject (isDrawImage, sizeof(BITMAP), &isImageSize);
	mWidth = isImageSize.bmWidth;
	mHeight = isImageSize.bmHeight;
	BitBlt(mem0dc, isDrawX, isDrawY, mWidth, mHeight, mem1dc, 0, 0, SRCPAINT);
	SelectObject(mem1dc, hbmOld);//-6
	DeleteDC(mem1dc);//-5
}