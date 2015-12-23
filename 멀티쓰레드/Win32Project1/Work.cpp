#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <process.h>
#define BSIZE 40	// 반지름

double LengthPts( int x1, int y1, int x2, int y2 ) {
	return( sqrt( (float)( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ) ) );
}

BOOL InCircle(int x, int y, int mx, int my)
{
	if(LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LPCTSTR lpszClass = "Window Class";
HINSTANCE g_hInst;

int x[100], y[100];
COLORREF rgb[100];
int isONE=0;

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

void ThreadProc(HWND hwnd, HBRUSH hBrush, HBRUSH oldBrush){
	HDC hdc;
	int i;
	srand((unsigned)time(0)); // 난수 발생을 위한 초기화
	hdc = GetDC(hwnd);
	SelectObject(hdc, CreateSolidBrush(RGB(rand()%256, rand()%256, rand()%256)));
	// 스레드마다 사각형 그릴때 사용할 자신의 색을 선택함(난수 이용)

	int num;
	i = isONE;
	num = rand()%500; // 스레드마다 자신의 숫자를 0~499 사이에서 선택

	//Rectangle(hdc, 0, 0, 20, num); // 선택한 숫자 크기의 사각형을 (0,0) 기준으로 그림

	x[i] = rand()%400+rand()%400, y[i] = rand()%300+rand()%300;
	rgb[i] =RGB(rand()%256,rand()%256,rand()%256);

	//Ellipse( hdc, (rand()%400)-BSIZE, (rand()%300)-BSIZE, (rand()%400)+BSIZE, (rand()%300)+BSIZE );

	hBrush = CreateSolidBrush( rgb[i]); 
	oldBrush = (HBRUSH) SelectObject( hdc, hBrush );
	Ellipse( hdc,x[i]-BSIZE, y[i]-BSIZE, x[i]+BSIZE, y[i]+BSIZE );
	SelectObject( hdc, oldBrush );
	DeleteObject( hBrush );
	isONE +=1;
	//x[i] = rand()%400+rand()%400, y[i] = rand()%300+rand()%300;

	ReleaseDC(hwnd, hdc);
	return;
}

#define THREAD_NUM 10


LRESULT CALLBACK WndProc( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam ){
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;

	static char Debug[50];
	static int isSetTimer=0;
	int mx, my;
	static HANDLE hThread[THREAD_NUM];
	static int count;
	int i;

	switch( iMsg )
	{
		srand((unsigned int)time(NULL));
	case WM_TIMER:
		if(isSetTimer >= 1 ){
			isSetTimer=0;
			for(int i=0; i<100; ++i){
				if(x[i] !=NULL){
						rgb[i] =RGB(rand()%256,rand()%256,rand()%256);
					//x[i] = rand()%400+rand()%400, y[i] = rand()%300+rand()%300;
				}
			}
		}else{
			++isSetTimer;
		}

		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_CREATE :
		SetTimer(hwnd,1,1000,NULL);
		for(int i=0; i<10; ++i){

		}
		return 0;

	case WM_LBUTTONDOWN :
		mx = LOWORD( lParam );
		my = HIWORD( lParam );
		/*for( int i = 0; i<10; ++i) {
		if( InCircle( x[i], y[i], mx, my ) ) {
		x[i] = rand()%700+rand()%100, y[i] = rand()%500+rand()%100;
		}
		}*/
		hThread[count] = (HANDLE)_beginthreadex (NULL, 0,(unsigned int(__stdcall *)(void *))ThreadProc,NULL, 0, NULL);
		// 스레드 함수에게 전달하는 인수는 없음
		Sleep(100);

		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_PAINT :
		hdc = BeginPaint( hwnd, &ps );
		for( int i = 0; i<100; ++i ){
			if(x[i] !=NULL){
				hBrush = CreateSolidBrush( rgb[i]); 
				oldBrush = (HBRUSH) SelectObject( hdc, hBrush );
				Ellipse( hdc,x[i]-BSIZE, y[i]-BSIZE, x[i]+BSIZE, y[i]+BSIZE );
				SelectObject( hdc, oldBrush );
				DeleteObject( hBrush );
			}
		}

		wsprintf(Debug, "isSetTimer : %ds", isSetTimer);
		TextOut(hdc, 0,510, Debug, strlen(Debug));
		EndPaint( hwnd, &ps );
		break;

	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	}

	return ( DefWindowProc(hwnd, iMsg, wParam, lParam) );
}