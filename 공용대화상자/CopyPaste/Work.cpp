#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"
#include <atlconv.h>

#define isMaxData 20
#define isLimitData 10

BOOL InEatCheck(int x, int xX, int y, int yY, int mx, int my){
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HMENU hMenu, hSubMenu;
	static int startX[isMaxData], startY[isMaxData], oldX[isMaxData], oldY[isMaxData], isClick[isMaxData] = {0};
	static BOOL Drag[isMaxData], tempDrag;
	static int endX[isMaxData], endY[isMaxData];
	static int mode[isMaxData]={2};
	static int tempStartX, tempStartY, tempendX, tempendY, tempMode, tempRand;
	static int mx, my, isWhatStart=0, isSaveData=-1;
	static int isDataSave[6], isNumber=0;
	static char Debug[500];
	static COLORREF color[isMaxData]={RGB(0,0,0)}, tempColor;
	static HBRUSH hbr,old;
	static HPEN hpen,oldp;
	static bool border=false;
	static SIZE size;
	static OPENFILENAME OFN;
	OPENFILENAME SFN;
	FILE *fPtr;
	HANDLE hFile;
	CHAR buf[32000]={0,};
	DWORD dwRead;
	DWORD dwWritten;
	static char filename[1000] = "";
	static int line=30, start =0;
	static char* buffer=(char*)calloc(10,sizeof(char));
	static char isTextData[1000];
	static char str[100], lpstrFile[1000] = "", isData[1000] = "";
	char filter[] = "Every File(*.*) \0*.*\0Memo\0*.txt;*.c0";
	CHOOSECOLOR COLORC;
	static COLORREF tmp[16], colort;
	HBRUSH hBrush, OldBrush;


	switch(iMessage)
	{
	case WM_TIMER:

		break;

	case WM_CREATE:
		for(int i=0; i <isMaxData; ++i){
			endX[i]=startX[i]=oldX[i]=0;
			endY[i]=startY[i]=oldY[i]=0;
			Drag[i]=false;
			mode[i] = 2, color[i] = RGB(rand()%256,rand()%256,rand()%256);
		}
		hMenu = GetMenu(hWnd);
		hSubMenu = GetSubMenu(hMenu, 1);
		EnableMenuItem(hMenu, ID_MENU_PASTE, MF_GRAYED);
		EnableMenuItem(hMenu, ID_MENU_CANCLE, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_1, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_2, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_3, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_4, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_5, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_6, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_7, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_8, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_9, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SELECT_10, MF_GRAYED);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		if(isWhatStart >= isLimitData){
			if( -1 == isSaveData){
				mx = LOWORD( lParam );
				my = HIWORD( lParam );
				for(int i=0; i<isMaxData; ++i){
					if( InEatCheck(startX[i], endX[i], startY[i],endY[i], mx,  my) == TRUE){
						isSaveData=i;
						hbr=CreateHatchBrush(HS_DIAGCROSS,RGB(255,0,0));
						old=(HBRUSH)SelectObject(hdc,hbr);
						if( 1 == mode[i] ){
							Ellipse(hdc,startX[i], startY[i] ,endX[i], endY[i] );
						}else{
							Rectangle(hdc,startX[i], startY[i] ,endX[i], endY[i] );
						}
						SelectObject(hdc,old);
						DeleteObject(hbr);
					}
				}
			}
		}else{
			Drag[isWhatStart]=true;
			startX[isWhatStart]=LOWORD(lParam);
			startY[isWhatStart]=HIWORD(lParam);
			//endX = LOWORD(lParam);
			//endY = HIWORD(lParam);
			oldX[isWhatStart] = LOWORD(lParam);
			oldY[isWhatStart] = HIWORD(lParam);
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_LBUTTONUP:
		if(isWhatStart >= isLimitData){
			if(10 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_10, MF_ENABLED);
		}else{
			Drag[isWhatStart]=false;
			if(1 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_1, MF_ENABLED);
			if(2 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_2, MF_ENABLED);
			if(3 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_3, MF_ENABLED);
			if(4 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_4, MF_ENABLED);
			if(5 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_5, MF_ENABLED);
			if(6 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_6, MF_ENABLED);
			if(7 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_7, MF_ENABLED);
			if(8 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_8, MF_ENABLED);
			if(9 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_9, MF_ENABLED);
			if(10 == isWhatStart)
				EnableMenuItem(hMenu, ID_SELECT_10, MF_ENABLED);
			isWhatStart+=1;
		}
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if(isWhatStart >= isLimitData){

		}else{
			if (Drag[isWhatStart]){
				SetROP2(hdc, R2_WHITE); // 펜의 XOR 연산
				endX[isWhatStart] = LOWORD(lParam);
				endY[isWhatStart] = HIWORD(lParam);
				oldX[isWhatStart] = endX[isWhatStart]; oldY[isWhatStart] = endY[isWhatStart]; // 현 지점을 이젂 지점으로 설정
			}
			InvalidateRect(hWnd,NULL,1);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		hbr=CreateSolidBrush(color[isWhatStart]);
		old=(HBRUSH)SelectObject(hdc,hbr);

		if(0 == mode[isWhatStart]){
			if(border){
				hpen=CreatePen(PS_SOLID,4,color[isWhatStart]);
				oldp=(HPEN)SelectObject(hdc,hpen);
				MoveToEx(hdc, startX[isWhatStart], startY[isWhatStart], NULL);
				SelectObject(hdc,oldp);
				DeleteObject(hpen);
			}else
				MoveToEx(hdc, startX[isWhatStart], startY[isWhatStart], NULL);
			LineTo(hdc, endX[isWhatStart], endY[isWhatStart]); // 그리기 : 흰 바탕 XOR 흰 펜 = 검은 선
		}else if(1 == mode[isWhatStart]){
			if(border){
				hpen=CreatePen(PS_SOLID,4,color[isWhatStart]);
				oldp=(HPEN)SelectObject(hdc,hpen);
				Ellipse(hdc,startX[isWhatStart],startY[isWhatStart],endX[isWhatStart],endY[isWhatStart]);
				SelectObject(hdc,oldp);
				DeleteObject(hpen);
			}else
				Ellipse(hdc,startX[isWhatStart],startY[isWhatStart],endX[isWhatStart],endY[isWhatStart]);
		}else if(2 == mode[isWhatStart]){
			if(border){
				hpen=CreatePen(PS_SOLID,4,color[isWhatStart]);
				oldp=(HPEN)SelectObject(hdc,hpen);
				Rectangle(hdc,startX[isWhatStart],startY[isWhatStart],endX[isWhatStart],endY[isWhatStart]);
				SelectObject(hdc,oldp);
				DeleteObject(hpen);
			}else
				Rectangle(hdc,startX[isWhatStart],startY[isWhatStart],endX[isWhatStart],endY[isWhatStart]);
		}
		SelectObject(hdc,old);
		DeleteObject(hbr);

		for(int i=0; i<isWhatStart; ++i){
			if(0 == mode[i]){
				if(border){
					hpen=CreatePen(PS_SOLID,4,color[i]);
					oldp=(HPEN)SelectObject(hdc,hpen);
					MoveToEx(hdc, startX[i], startY[i], NULL);
					SelectObject(hdc,oldp);
					DeleteObject(hpen);
				}else
					hbr = CreateSolidBrush( color[i]); 
				old = (HBRUSH) SelectObject( hdc, hbr );
				MoveToEx(hdc, startX[i], startY[i], NULL);
				LineTo(hdc, endX[i], endY[i]); // 그리기 : 흰 바탕 XOR 흰 펜 = 검은 선
			}else if(1 == mode[i]){

				if(border){
					hpen=CreatePen(PS_SOLID,4,color[i]);
					oldp=(HPEN)SelectObject(hdc,hpen);
					Ellipse(hdc,startX[i],startY[i],endX[i],endY[i]);
					SelectObject(hdc,oldp);
					DeleteObject(hpen);
				}else
					hbr = CreateSolidBrush( color[i]); 
				old = (HBRUSH) SelectObject( hdc, hbr );
				Ellipse(hdc,startX[i],startY[i],endX[i],endY[i]);
			}else if(2 == mode[i]){

				if(border){
					hpen=CreatePen(PS_SOLID,4,color[i]);
					oldp=(HPEN)SelectObject(hdc,hpen);
					Rectangle(hdc,startX[i],startY[i],endX[i],endY[i]);
					SelectObject(hdc,oldp);
					DeleteObject(hpen);
				}else
					hbr = CreateSolidBrush( color[i]); 
				old = (HBRUSH) SelectObject( hdc, hbr );
				Rectangle(hdc,startX[i],startY[i],endX[i],endY[i]);

				if(1 == isClick[i] ){
					hbr=CreateHatchBrush(HS_DIAGCROSS,RGB(255,0,0));
					old=(HBRUSH)SelectObject(hdc,hbr);
						Rectangle(hdc,startX[i], startY[i] ,endX[i], endY[i] );
					SelectObject(hdc,old);
					DeleteObject(hbr);
				}


			}
			SelectObject(hdc,old);
			DeleteObject(hbr);
		}

		wsprintf(Debug, "isWhatStart : %d", isWhatStart);
		TextOut(hdc, 10,510, Debug, strlen(Debug));

		if(isWhatStart >= isLimitData){
			hMenu = GetMenu(hWnd);
			hSubMenu = GetSubMenu(hMenu, 1);
			EnableMenuItem(hMenu, ID_SHAPE_RECTANGLE, MF_GRAYED);
			EnableMenuItem(hMenu, ID_SHAPE_LINE, MF_GRAYED);
			EnableMenuItem(hMenu, ID_SHAPE_ELLIPSE, MF_GRAYED);
		}

		EndPaint(hWnd,&ps);
		break;
	case WM_COMMAND:
		hdc = GetDC(hWnd);
		switch(LOWORD(wParam))
		{
		case ID_SELECT_1:
			isSaveData = 0;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_SELECT_2:
			isSaveData = 1;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_SELECT_3:
			isSaveData = 2;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_SELECT_4:
			isSaveData = 3;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_SELECT_5:
			isSaveData = 4;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_SELECT_6:
			isSaveData = 5;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_SELECT_7:
			isSaveData = 6;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;		
		case ID_SELECT_8:
			isSaveData = 7;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_SELECT_9:
			isSaveData = 8;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_SELECT_10:
			isSaveData = 9;
			isClick[isSaveData] = 1;
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_RECOLOR_CLICK:
			if( -1 == isSaveData ){
			}else{
				for(int i=0;i<16;i++)
					tmp[i] = RGB(rand()%256,rand()%256,rand()%256);
				memset(&COLORC, 0, sizeof(CHOOSECOLOR));
				COLORC.lStructSize = sizeof(CHOOSECOLOR);
				COLORC.hwndOwner = hWnd;
				COLORC.lpCustColors = tmp;
				COLORC.Flags = CC_FULLOPEN;
				if(ChooseColor(&COLORC)!=0) {
					colort = COLORC.rgbResult;
					color[isSaveData]=colort;
					isSaveData=-1;
					InvalidateRgn(hWnd, NULL, TRUE);
				}
			}
			break;
		case ID_COLOR_CYAN:
			color[isWhatStart]=RGB(0,255,255);
			break;
		case ID_COLOR_YELLOW:
			color[isWhatStart]=RGB(255,255,0);
			break;
		case ID_COLOR_MAGENTA:
			color[isWhatStart]=RGB(255,0,255);
			break;
		case ID_COLOR_RANDOM:
			color[isWhatStart]=RGB(rand()%256,rand()%256,rand()%256);
			break;
		case ID_SHAPE_RECTANGLE:
			mode[isWhatStart]=2;
			break;
		case ID_SHAPE_LINE:
			mode[isWhatStart]=0;
			break;
		case ID_SHAPE_ELLIPSE:
			mode[isWhatStart]=1;
			break;
		case ID_MENU_SAVE:
//isTextData
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner=hWnd;
			SFN.lpstrFilter= filter;
			SFN.lpstrFile=lpstrFile;
			SFN.nMaxFile=100;
			SFN.lpstrInitialDir="."; // 초기 디렉토리
			if (GetSaveFileName(&SFN)!=0) {
				wsprintf(str,"%s\n파일에 저장하시겠습니까 ?",SFN.lpstrFile);
				MessageBox(hWnd,str,"저장하기 선택",MB_OK);

				USES_CONVERSION;
				strcpy(isData, T2A(OFN.lpstrFile));

				fPtr = fopen (isData, "wb" );
				fwrite (isTextData , 2 , lstrlen(isTextData) , fPtr );
				fclose (fPtr);
			}
			break;

		}
			break;
		case ID_MENU_OPEN:
//
			break;
		case ID_MENU_COPY:
			if( -1 == isSaveData ){
				MessageBox(hWnd,"도형을 10개 모두 만드신후, 클릭을 해주시길 바랍니다..!","Help",MB_OK);
			}else{
				hMenu = GetMenu(hWnd);
				hSubMenu = GetSubMenu(hMenu, 1);
				tempStartX = startX[isSaveData], tempStartY= startY[isSaveData];
				tempendX = endX[isSaveData], tempendY = endY[isSaveData], tempColor = color[isSaveData];
				tempMode = mode[isSaveData], tempRand = rand()%50+50, tempDrag = Drag[isSaveData];
				EnableMenuItem(hMenu, ID_MENU_PASTE, MF_ENABLED);
				EnableMenuItem(hMenu, ID_MENU_CANCLE, MF_ENABLED);
				MessageBox(hWnd,"복사준비 완료..!","Help",MB_OK);
			}
			break;
		case ID_MENU_PASTE:
			startX[isWhatStart] = tempStartX + tempRand, startY[isWhatStart] = tempStartY + tempRand;
			endX[isWhatStart] = tempendX + tempRand, endY[isWhatStart] = tempendY + tempRand;
			mode[isWhatStart] = tempMode, Drag[isWhatStart] =  tempDrag, color[isWhatStart] = tempColor;
			isWhatStart+=1;
			isSaveData = -1;
			hMenu = GetMenu(hWnd);
			hSubMenu = GetSubMenu(hMenu, 1);
			EnableMenuItem(hMenu, ID_MENU_PASTE, MF_GRAYED);
			EnableMenuItem(hMenu, ID_MENU_CANCLE, MF_GRAYED);
			for(int i=0; i<isMaxData; ++i){
				isClick[i]  =0;
			}
			MessageBox(hWnd,"복사 완료..!","Help",MB_OK);
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_MENU_CANCLE:
			isSaveData = -1;
			hMenu = GetMenu(hWnd);
			hSubMenu = GetSubMenu(hMenu, 1);
			EnableMenuItem(hMenu, ID_MENU_PASTE, MF_GRAYED);
			EnableMenuItem(hMenu, ID_MENU_CANCLE, MF_GRAYED);
			for(int i=0; i<isMaxData; ++i){
				isClick[i]  =0;
			}
			MessageBox(hWnd,"취소 완료..!","Help",MB_OK);
			InvalidateRect(hWnd,NULL,1);
			break;
		case ID_BORDER_ON:
			border=false;
			break;
		case ID_BORDER_OFF:
			border=true;
			break;
		case ID_HELP_HELP:
			MessageBox(hWnd,"-_____-@06.16","My Email",MB_OK);
			break;
		case ID_HELP_EXIT:
			PostQuitMessage(0);
			break;
		}
		ReleaseDC(hWnd, hdc);
		break;
	}
	return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}