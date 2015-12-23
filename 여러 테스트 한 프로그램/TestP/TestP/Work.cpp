#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define isMakeEatData 16 //데이터 생성을 할때 총 데이터수
#define isChangeData isMakeEatData-1 // 회전할때 쓰기 위한 부분
#define drawNemo 90

BOOL InEatCheck(int x, int xX, int y, int yY, int mx, int my){ // 초밥안에 마우스가 들어가 있을경우를 체크하는 부분
	if((x <= mx && xX >= mx) && (y <= my && yY >= my)){
		return TRUE;
	}else{ 
		return FALSE;
	}
}

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
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name", "Window Title Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	while(GetMessage(&msg, NULL, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM  lParam)
{
	HDC hdc;
	PAINTSTRUCT  ps;
	static int mx, my;
	static RECT rt;
	static BOOL isGameStart, isStartDebug;
	static char Debug[500], Debug2[5000], isMsgBox[500];
	static int isEatData[isMakeEatData], isClick[isMakeEatData]={0}, isCheckClick[isMakeEatData]={0}, tempData, tempClick, tempCheckClick, WhatClick=0;
	static int isWhatMoney=0, isPlusMoney=0;
	static int isEatDatax[isMakeEatData], isEatDataxX[isMakeEatData], isEatDatay[isMakeEatData], isEatDatayY[isMakeEatData];
	static int ColorR[isMakeEatData], ColorG[isMakeEatData], ColorB[isMakeEatData], tempR, tempG, tempB;
	static int ClickColorR[10], ClickColorG[10], ClickColorB[10], isType[10];
	static COLORREF isEatColor[isMakeEatData], tempColor; // xor 연산 공부하자..!
	HBRUSH hBrush, oldBrush;
	switch(iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd,&rt);
		srand((unsigned int)time(NULL));
		for(int i=0; i<isMakeEatData;++i){ // 초밥을 만들기 위한 부분
			isEatData[i] = rand()%3;
			isEatColor[i] = RGB(rand()%256,rand()%256,rand()%256);
			ColorR[i] = rand()%256, ColorG[i] = rand()%256, ColorB[i] = rand()%256;
		}
		break;

	case WM_TIMER: // 초밥을 회전하기 위한 부분..!
		for(int i=0; i<isMakeEatData; ++i){
			if( i == 0 ){
				tempData = isEatData[0], tempClick = isClick[0]; //tempColor = isEatColor[0];
				tempCheckClick = isCheckClick[0];
				tempR = ColorR[0], tempG = ColorG[0], tempB = ColorB[0];
			}
			if( i == isChangeData ){
				isEatData[isChangeData] = tempData, isClick[isChangeData] = tempClick; // isEatColor[15] = tempColor;
				isCheckClick[isChangeData] = tempCheckClick;
				ColorR[isChangeData] = tempR, ColorG[isChangeData] = tempG, ColorB[isChangeData] = tempB;
			}else{
				isEatData[i] = isEatData[i+1], isClick[i] = isClick[i+1];
				isCheckClick[i] = isCheckClick[i+1];
				ColorR[i] = ColorR[i+1], ColorG[i] = ColorG[i+1], ColorB[i] = ColorB[i+1];
			}
		}
		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;

	case WM_LBUTTONDOWN :
		mx = LOWORD( lParam );
		my = HIWORD( lParam );
		if( isGameStart == TRUE){
			for(int i=0; i<isMakeEatData; ++i){
				if( InEatCheck(isEatDatax[i], isEatDataxX[i], isEatDatay[i],isEatDatayY[i], mx,  my) == TRUE){
					if( 9 < WhatClick || isClick[i] ==1){
						if(isClick[i] ==1){
							MessageBox(hwnd,"이미 선택하신 음식입니다..!","Sorry ㅠㅠ",MB_OK);
						}else{
							MessageBox(hwnd,"죄송합니다... 손님에게 더이상 음식을 팔지 못하겠습니다..!","Sorry ㅠㅠ",MB_OK);
						}
					}else{
						for(int i=0; i<isMakeEatData; ++i){
							isClick[i] = 0;
						}

						if( isEatData[i] == 1 ){
							isWhatMoney+=1000;
							isPlusMoney+=100;
						}else if( isEatData[i] == 2 ){
							isWhatMoney+=4000;
							isPlusMoney+=400;
						}else if( isEatData[i] == 3 ){
							isWhatMoney+=2000;
							isPlusMoney+=200;
						}

						ClickColorR[WhatClick] = ColorR[i], ClickColorG[WhatClick] = ColorG[i], ClickColorB[WhatClick] = ColorB[i];
						//ColorR[i] = 255-ColorR[i], ColorG[i] = 255-ColorG[i], ColorB[i] = 255-ColorB[i];
						isClick[i] = 1, isCheckClick[i] = 1;
						isType[WhatClick] = isEatData[i];
						WhatClick+=1;
					}
				}
			}
		}
		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;

	case WM_CHAR ://
		hdc=GetDC(hwnd);
		if (wParam == 's' || wParam == 'S' ){
			SetTimer(hwnd, 1, 1000, NULL);
			isGameStart = TRUE;
		}else if (wParam == 't' || wParam == 'T' ){
			KillTimer(hwnd,1);
			isGameStart = FALSE;
		}else if (wParam == 'd' || wParam == 'D' ){
			if(isStartDebug==FALSE){
				isStartDebug =TRUE;
			}else{
				isStartDebug =FALSE;
			}
		}else if (wParam == 'q' || wParam == 'Q' ){
			if( MessageBox(hwnd,"정말로 종료하시겠습니까..!?","EXIT",MB_YESNO) == IDYES){
				if( MessageBox(hwnd,"정말 종료 하고 싶으신건가요..!?","EXIT",MB_YESNO) == IDYES){
					if( MessageBox(hwnd,"종료 하시려면 No(아니오)버튼을 눌러주세요..!?","EXIT",MB_YESNO) == IDNO){
						if( MessageBox(hwnd,"종료 하기 싫은데 정말로 종료하실껀가요..!?","EXIT",MB_YESNO) == IDYES){
							MessageBox(hwnd,"감사합니다 다음에 또 와주시길바랍니다~!!!","Thank U",MB_OK);
							KillTimer(hwnd,1);
							PostQuitMessage(0);
						}
					}
				}
			}
		}else if (wParam == 'c' || wParam == 'C' ){
			//isMsgBox
			sprintf(isMsgBox, "지금까지 구매하신 초밥의 가격(V.A.T 포함)은 : %d 원 입니다..!\n\n계산을 하시겠습니까..!?",isWhatMoney+isPlusMoney,isPlusMoney );
			if( MessageBox(hwnd,isMsgBox,"Give Me Money~~!!!",MB_YESNO) == IDYES){
				//Yes 버튼을 클릭시
				srand((unsigned int)time(NULL));
				for(int i=0; i<isMakeEatData;++i){ 
					isEatData[i] = rand()%3;
					isEatColor[i] = RGB(rand()%256,rand()%256,rand()%256);
					ColorR[i] = rand()%256, ColorG[i] = rand()%256, ColorB[i] = rand()%256;
					isClick[i]=0, isCheckClick[i]=0, WhatClick=0, isWhatMoney=0, isGameStart = FALSE;
				}
				for(int i=0; i<10; ++i){
					ClickColorR[i] =0, ClickColorG[i] = 0, ClickColorB[i] = 0, isType[i] =0;
				}

				MessageBox(hwnd,"감사합니다 다음에 또 와주시길바랍니다~!!!","Thank U",MB_OK);
			}else{

			}
		}else if (wParam == 'r' || wParam == 'R' ){
			sprintf(isMsgBox, "구매하시려던 초밥 %d개를 다시 놓으시고 새로 고르시겠습니까..!?",WhatClick);
			if( MessageBox(hwnd,isMsgBox,"ㅠㅠ",MB_YESNO) == IDYES){
				srand((unsigned int)time(NULL));
				for(int i=0; i<isMakeEatData;++i){ 
					isEatData[i] = rand()%3;
					//isEatColor[i] = RGB(rand()%256,rand()%256,rand()%256);
					ColorR[i] = rand()%256, ColorG[i] = rand()%256, ColorB[i] = rand()%256;
					isClick[i]=0, isCheckClick[i]=0, WhatClick=0, isWhatMoney=0, isGameStart = FALSE;
				}
				for(int i=0; i<10; ++i){
					ClickColorR[i] =0, ClickColorG[i] = 0, ClickColorB[i] = 0, isType[i] =0;
				}
			}
		}

		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수
		break;

	case WM_PAINT :
		hdc =BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(255,255,255));
		oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
		for(int j=0 ; j<5; ++j){
			for(int i=0; i<5; ++i){
				Rectangle(hdc,50+(i*drawNemo),50+(j*drawNemo) ,140+(i*drawNemo), 140+(j*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
			}
		}
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);

		for(int i=0; i <isMakeEatData; ++i){ // 전체 데이터중 현재 클릭된것을 찾아서 해당 클릭된것을 정가운데 위치에 띄워주는 부분..!
			if(isClick[i] == 1){
				hBrush = CreateSolidBrush(RGB(ClickColorR[WhatClick-1], ClickColorG[WhatClick-1], ClickColorB[WhatClick-1]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				if(isEatData[i] == 0){
					Rectangle(hdc,250 ,250, 300, 300 ); //왼쪽, 탑 , 오른쪽, 아래
				}else if(isEatData[i] == 1){
					Ellipse(hdc,250 ,250, 300, 300 ); //왼쪽, 탑 , 오른쪽, 아래
				}else if(isEatData[i] == 2){
					Rectangle(hdc,260 ,250, 290, 300 ); //왼쪽, 탑 , 오른쪽, 아래
				}
				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);
			}

			if(isCheckClick[i] == 1){ // 전체데이터중 클릭이 되어있을경우 HS_DIAGCROSS 사용하여 클릭된것처럼 보이게 설정
				hBrush=CreateHatchBrush(HS_DIAGCROSS,RGB(0,0,0));
				oldBrush=(HBRUSH)SelectObject(hdc,hBrush);

				int tempi;
				if( isGameStart == TRUE){
					tempi = i-1;
				}else{
					tempi = i;
				}

				if(isEatData[tempi] == 2){
					Rectangle(hdc,isEatDatax[i]-30, isEatDatay[i]-20 ,isEatDataxX[i]+30, isEatDatayY[i]+20 );
				}else{
					Rectangle(hdc,isEatDatax[i]-20, isEatDatay[i]-20 ,isEatDataxX[i]+20, isEatDatayY[i]+20 );
				}
				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);
			}
		}
		// 직선 -----
		for(int i=0; i<5; ++i){
			if(isEatData[i] == 0){
				hBrush = CreateSolidBrush(RGB(ColorR[i], ColorG[i], ColorB[i]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,70+(i*drawNemo),70 ,120+(i*drawNemo), 120 ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i] = 70+(i*drawNemo), isEatDataxX[i] = 120+(i*drawNemo);
				isEatDatay[i] = 70, isEatDatayY[i] = 120;
			}else if(isEatData[i] == 1){
				hBrush = CreateSolidBrush(RGB(ColorR[i], ColorG[i], ColorB[i]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Ellipse(hdc,70+(i*drawNemo),70 ,120+(i*drawNemo), 120 ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i] = 70+(i*drawNemo), isEatDataxX[i] = 120+(i*drawNemo);
				isEatDatay[i] = 70, isEatDatayY[i] = 120;
			}else if(isEatData[i] == 2){
				hBrush = CreateSolidBrush(RGB(ColorR[i], ColorG[i], ColorB[i]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,80+(i*drawNemo),70 ,110+(i*drawNemo), 120 ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i] = 80+(i*drawNemo), isEatDataxX[i] = 110+(i*drawNemo);
				isEatDatay[i] = 70, isEatDatayY[i] = 120;
			}
		}
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);
		// 세로 -*4
		for(int i=1; i<5; ++i){
			if(isEatData[i+4] == 0){
				hBrush = CreateSolidBrush(RGB(ColorR[i+4], ColorG[i+4], ColorB[i+4]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,70+(360),70+(i*drawNemo) ,120+(360), 120+(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+4] = 70+(360), isEatDataxX[i+4] = 120+(360);
				isEatDatay[i+4] = 70+(i*drawNemo), isEatDatayY[i+4] = 120+(i*drawNemo);
			}else if(isEatData[i+4] == 1){
				hBrush = CreateSolidBrush(RGB(ColorR[i+4], ColorG[i+4], ColorB[i+4]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Ellipse(hdc,70+(360),70+(i*drawNemo) ,120+(360), 120+(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+4] = 70+(360), isEatDataxX[i+4] = 120+(360);
				isEatDatay[i+4] = 70+(i*drawNemo), isEatDatayY[i+4] = 120+(i*drawNemo);
			}else if(isEatData[i+4] == 2){
				hBrush = CreateSolidBrush(RGB(ColorR[i+4], ColorG[i+4], ColorB[i+4]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,80+(360),70+(i*drawNemo) ,110+(360), 120+(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+4] = 80+(360), isEatDataxX[i+4] = 110+(360);
				isEatDatay[i+4] = 70+(i*drawNemo), isEatDatayY[i+4] = 120+(i*drawNemo);
			}
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}
		
		for(int i=1; i<5; ++i){
			if(isEatData[i+8] == 0){
				hBrush = CreateSolidBrush(RGB(ColorR[i+8], ColorG[i+8], ColorB[i+8]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,70+(360)-(i*drawNemo),70+(360) ,120+(360)-(i*drawNemo), 120+(360) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+8] = 70+(360)-(i*drawNemo), isEatDataxX[i+8] = 120+(360)-(i*drawNemo);
				isEatDatay[i+8] = 70+(360), isEatDatayY[i+8] = 120+(360);
			}else if(isEatData[i+8] == 1){
				hBrush = CreateSolidBrush(RGB(ColorR[i+8], ColorG[i+8], ColorB[i+8]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Ellipse(hdc,70+(360)-(i*drawNemo),70+(360) ,120+(360)-(i*drawNemo), 120+(360) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+8] = 70+(360)-(i*drawNemo), isEatDataxX[i+8] = 120+(360)-(i*drawNemo);
				isEatDatay[i+8] = 70+(360), isEatDatayY[i+8] = 120+(360);
			}else if(isEatData[i+8] == 2){
				hBrush = CreateSolidBrush(RGB(ColorR[i+8], ColorG[i+8], ColorB[i+8]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,80+(360)-(i*drawNemo),70+(360) ,110+(360)-(i*drawNemo), 120+(360) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+8] = 80+(360)-(i*drawNemo), isEatDataxX[i+8] = 110+(360)-(i*drawNemo);
				isEatDatay[i+8] = 70+(360), isEatDatayY[i+8] = 120+(360);
			}
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}

		for(int i=1; i<4; ++i){
			if(isEatData[i+12] == 0){
				hBrush = CreateSolidBrush(RGB(ColorR[i+12], ColorG[i+12], ColorB[i+12]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,70+(360)-(360),70+(360)-(i*drawNemo) ,120+(360)-(360), 120+(360)-(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+12] = 70+(360)-(360),70+(360)-(i*drawNemo), isEatDataxX[i+12] = 120+(360)-(360);
				isEatDatay[i+12] = 70+(360)-(i*drawNemo), isEatDatayY[i+12] = 120+(360)-(i*drawNemo);
			}else if(isEatData[i+12] == 1){
				hBrush = CreateSolidBrush(RGB(ColorR[i+12], ColorG[i+12], ColorB[i+12]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Ellipse(hdc,70+(360)-(360),70+(360)-(i*drawNemo) ,120+(360)-(360), 120+(360)-(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+12] = 70+(360)-(360),70+(360)-(i*drawNemo), isEatDataxX[i+12] = 120+(360)-(360);
				isEatDatay[i+12] = 70+(360)-(i*drawNemo), isEatDatayY[i+12] = 120+(360)-(i*drawNemo);
			}else if(isEatData[i+12] == 2){
				hBrush = CreateSolidBrush(RGB(ColorR[i+12], ColorG[i+12], ColorB[i+12]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,80+(360)-(360),70+(360)-(i*drawNemo) ,110+(360)-(360), 120+(360)-(i*drawNemo) ); //왼쪽, 탑 , 오른쪽, 아래
				isEatDatax[i+12] = 80+(360)-(360),70+(360)-(i*drawNemo), isEatDataxX[i+12] = 110+(360)-(360);
				isEatDatay[i+12] = 70+(360)-(i*drawNemo), isEatDatayY[i+12] = 120+(360)-(i*drawNemo);
			}
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}

		sprintf(Debug, "시작 : S, 정지 : T, 계산 : C, 재시작 : R, 종료 : Q, 디버그 : D");
		TextOut(hdc, (rt.right/2)-(strlen(Debug)*3)-20, 10, Debug, strlen(Debug));

		sprintf(Debug, "지금까지 구매한 금액 : %d", isWhatMoney);
		TextOut(hdc, (rt.right/2)-(strlen(Debug)*3)-20, 30, Debug, strlen(Debug));

		if( isStartDebug == TRUE ) { // 디버깅 눈에 띄게 보기 편하게 하기윈한 부분
			for(int i=0; i<isMakeEatData; ++i){
				sprintf(Debug2, "%d", isEatDatax[i]);
				TextOut(hdc, 531,65+(i*20), Debug2, strlen(Debug2));
				sprintf(Debug2, "%d", isEatDatay[i]);
				TextOut(hdc, 562,65+(i*20), Debug2, strlen(Debug2));
				sprintf(Debug2, "%d", isEatDataxX[i]);
				TextOut(hdc, 592,65+(i*20), Debug2, strlen(Debug2));
				sprintf(Debug2, "%d", isEatDatayY[i]);
				TextOut(hdc, 622,65+(i*20), Debug2, strlen(Debug2));
				sprintf(Debug2, "%d", isEatData[i]);
				TextOut(hdc, 652,65+(i*20), Debug2, strlen(Debug2));
				sprintf(Debug2, "%d", isClick[i]);
				TextOut(hdc, 668,65+(i*20), Debug2, strlen(Debug2));
				sprintf(Debug2, "%d", isCheckClick[i]);
				TextOut(hdc, 682,65+(i*20), Debug2, strlen(Debug2));
			}

			sprintf(Debug2, "mx : %d, my : %d", mx, my);
			TextOut(hdc, 531,385, Debug2, strlen(Debug2));
			sprintf(Debug2, "isGameStart : %d, isStartDebug : %d", isGameStart, isStartDebug);
			TextOut(hdc, 531,405, Debug2, strlen(Debug2));
		}

		for(int i=0; i<WhatClick; ++i){ //클릭한 횟수까지의 데이터를 옆에다가 띄워주기
			if(5==i){
				break;
			}
			hBrush = CreateSolidBrush(RGB(255,255,255));
			oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
			Rectangle(hdc,530,50+(i*drawNemo),630,140+(i*drawNemo)); //왼쪽, 탑 , 오른쪽, 아래
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(ClickColorR[i],ClickColorG[i],ClickColorB[i]));
			oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
			if( isType[i] == 0){
				Rectangle(hdc,530+25,50+(i*drawNemo)+20,630-25,140+(i*drawNemo)-20); //왼쪽, 탑 , 오른쪽, 아래
			}else if( isType[i] == 1){
				Ellipse(hdc,530+25,50+(i*drawNemo)+20,630-25,140+(i*drawNemo)-20); //왼쪽, 탑 , 오른쪽, 아래
			}else if( isType[i] == 2){
				Rectangle(hdc,530+35,50+(i*drawNemo)+20,630-35,140+(i*drawNemo)-20); //왼쪽, 탑 , 오른쪽, 아래
			}
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}

		if(5<=WhatClick){ // 5개 이상일경우에는 옆에다가 띄워주기 위한 부분
			for(int i=5; i<WhatClick; ++i){
				hBrush = CreateSolidBrush(RGB(255,255,255));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,630,50+((i-5)*drawNemo),730,140+((i-5)*drawNemo)); //왼쪽, 탑 , 오른쪽, 아래
				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush(RGB(ClickColorR[i],ClickColorG[i],ClickColorB[i]));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				if( isType[i] == 0){
					Rectangle(hdc,630+25,50+((i-5)*drawNemo)+20,730-25,140+((i-5)*drawNemo)-20); //왼쪽, 탑 , 오른쪽, 아래
				}else if( isType[i] == 1){
					Ellipse(hdc,630+25,50+((i-5)*drawNemo)+20,730-25,140+((i-5)*drawNemo)-20); //왼쪽, 탑 , 오른쪽, 아래
				}else if( isType[i] == 2){
					Rectangle(hdc,630+35,50+((i-5)*drawNemo)+20,730-35,140+((i-5)*drawNemo)-20); //왼쪽, 탑 , 오른쪽, 아래
				}
				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);
			}
		}

		EndPaint(hwnd, &ps);
		break;


	case WM_DESTROY:
		KillTimer(hwnd,1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}



/*
하나씩 움직이게 할때 수동..!
tempData = isEatData[0], tempColor = isEatColor[0];
isEatData[0] = isEatData[1], isEatColor[0] = isEatColor[1];
isEatData[1] = isEatData[2], isEatColor[1] = isEatColor[2];
isEatData[2] = isEatData[3], isEatColor[2] = isEatColor[3];
isEatData[3] = isEatData[4], isEatColor[3] = isEatColor[4];
isEatData[4] = isEatData[5], isEatColor[4] = isEatColor[5];
isEatData[5] = isEatData[6], isEatColor[5] = isEatColor[6];
isEatData[6] = isEatData[7], isEatColor[6] = isEatColor[7];

isEatData[7] = isEatData[8], isEatColor[7] = isEatColor[8];
isEatData[8] = isEatData[9], isEatColor[8] = isEatColor[9];
isEatData[9] = isEatData[10], isEatColor[9] = isEatColor[10];

isEatData[10] = isEatData[11], isEatColor[10] = isEatColor[11];
isEatData[11] = isEatData[12], isEatColor[11] = isEatColor[12];
isEatData[12] = isEatData[13], isEatColor[12] = isEatColor[13];

isEatData[13] = isEatData[14], isEatColor[13] = isEatColor[14];
isEatData[14] = isEatData[15], isEatColor[14] = isEatColor[15];
isEatData[15] = tempData, isEatColor[15] = tempColor;
*/