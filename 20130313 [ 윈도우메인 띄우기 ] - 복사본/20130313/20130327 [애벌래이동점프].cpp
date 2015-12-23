#include <Windows.h>
#include <stdio.h>

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

	static int isLeft2 = 0, isRight2 = 40;
	static int isTop2 = 0, isBottom2 = 40;
	static int isLeft = 40, isRight = 80;
	static int isTop = 0, isBottom = 40;

	static BOOL GoStop;
	static int downcount = 0;

	static int TLRB = 3;

	static int spped = 50;
	HBRUSH hBrush, oldBrush;
	static char Debug[50];
	static char Debug2[50];

	switch(iMsg)
	{
	case WM_CREATE:

		break;
	case WM_KEYDOWN :
		if( wParam == VK_RETURN){
			if(GoStop==FALSE){
				SetTimer(hwnd,1,spped,NULL);
				SetTimer(hwnd,2,spped,NULL);
				GoStop=TRUE;
			}else{
				KillTimer(hwnd,1);
				KillTimer(hwnd,2);
				GoStop=FALSE;
			}
		}

		if(GoStop==TRUE){
			if(wParam == VK_RIGHT){
				TLRB =3;
			}else if(wParam == VK_LEFT){
				TLRB =2;
			}else if(wParam == VK_UP){
				TLRB =1;
			}else if(wParam == VK_DOWN){
				TLRB =4;
			}else if(wParam == VK_SPACE){
				KillTimer(hwnd,1);
				KillTimer(hwnd,2);
				SetTimer(hwnd,3,spped,NULL);
				GoStop==false;
			}
		}
		break;
	case WM_CHAR:
		if(GoStop==TRUE){
			if(wParam == 's'){
				if(spped >= 10){
					spped-=10;
					KillTimer(hwnd,1);
					KillTimer(hwnd,2);
				}
				SetTimer(hwnd,1,spped,NULL);
				SetTimer(hwnd,2,spped+10,NULL);
			}else if(wParam == 'd'){
				spped+=10;
				KillTimer(hwnd,1);
				KillTimer(hwnd,2);
				SetTimer(hwnd,1,spped,NULL);
				SetTimer(hwnd,2,spped+10,NULL);
			}
		}
		break;

	case WM_TIMER:
		if (wParam == 1 ){
			if(isLeft <= 0 ){
				isLeft=0;
				isRight=40;
				TLRB =4;
			}
			if(isLeft >= 750 ){
				isLeft=750;
				isRight=790;
				TLRB =4;
			}
			if(TLRB ==3){
				isLeft+=10;
				isRight+=10;
			}else if(TLRB ==2){
				isLeft-=10;
				isRight-=10;
			}else if(TLRB ==1){
				isTop-=10;
				isBottom-=10;
			}else if(TLRB ==4){
				isTop+=10;
				isBottom+=10;
			}
			if(TLRB ==4){
				downcount+=1;
			}
			if(downcount >= 5 ){
				downcount=0;
				if(isLeft==750){
					isLeft=740;
					isRight=780;
					TLRB =2;
				}
				if(isLeft==0){
					isLeft=10;
					isRight=50;
					TLRB =3;
				}
			}
		}
		if (wParam == 2 ){
			if(TLRB ==3){
				isTop2=isTop;
				isBottom2=isBottom;
				isLeft2=isLeft-30;
				isRight2=isRight-30;
			}else if(TLRB ==2){
				isTop2=isTop;
				isBottom2=isBottom;
				isLeft2=isLeft+30;
				isRight2=isRight+30;
			}else if(TLRB ==1){
				isLeft2=isLeft;
				isRight2=isRight;
				isTop2=isTop+30;
				isBottom2=isBottom+30;
			}else if(TLRB ==4){
				isLeft2=isLeft;
				isRight2=isRight;
				isTop2=isTop-30;
				isBottom2=isBottom-30;
			}
		}

		if (wParam == 3 ){
			if(downcount >= 5){
				KillTimer(hwnd,3);
				SetTimer(hwnd,4,spped,NULL);
			}else{
				isTop-=10;
				isBottom-=10;
				isTop2-=10;
				isBottom2-=10;
				++downcount;
			}
		}

		if (wParam == 4 ){
			if(downcount <= 0){
				KillTimer(hwnd,4);
				SetTimer(hwnd,1,spped,NULL);
				SetTimer(hwnd,2,spped,NULL);
				GoStop==true;
			}else{
				isTop+=10;
				isBottom+=10;
				isTop2+=10;
				isBottom2+=10;
				--downcount;
			}
		}


		InvalidateRect(hwnd, NULL, TRUE); //무효화 함수

		break;
	case WM_PAINT :
		hdc =BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(0,0,255));
		oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
		Ellipse(hdc,isLeft,isTop ,isRight,isBottom ); //왼쪽, 탑 , 오른쪽, 아래
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);


		hBrush = CreateSolidBrush(RGB(255,0,0));
		oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
		Ellipse(hdc,isLeft2,isTop2 ,isRight2,isBottom2 ); //왼쪽, 탑 , 오른쪽, 아래
		SelectObject(hdc,oldBrush);
		DeleteObject(hBrush);


		sprintf(Debug, "isLeft : %d, isRight : %d isTop : %d, isRight : %d, downcount : %d, spped : %d", isLeft, isRight, isTop, isBottom, downcount,spped);
		TextOut(hdc, 0,510, Debug, strlen(Debug));
		sprintf(Debug2, "isLeft : %d, isRight : %d isTop : %d, isRight : %d, downcount : %d, TLRB : %d", isLeft2, isRight2, isTop2, isBottom2, downcount,TLRB);
		TextOut(hdc, 0,530, Debug2, strlen(Debug2));
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hwnd,1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}