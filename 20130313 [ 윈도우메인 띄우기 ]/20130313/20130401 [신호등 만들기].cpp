#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define BSIZE 40	// ������

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

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow )
{
	HWND  hwnd;
	MSG   msg;
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	WndClass.hbrBackground = ( HBRUSH )GetStockObject( WHITE_BRUSH );
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "my first window program";
	RegisterClass(&WndClass);

	hwnd = CreateWindow( "my first window program",
		"Window Title Name",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow( hwnd, nCmdShow );
	UpdateWindow( hwnd );

	while( GetMessage( &msg, NULL, 0, 0 ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT iMsg,
						 WPARAM wParam, LPARAM lParam )
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static int x[4], y[4];
	static BOOL isClick[4];
	static COLORREF rgb;
	int mx, my;

	switch( iMsg )
	{
	case WM_CREATE :
		x[0] = 50, y[0] = 200;
		x[1] = 250, y[1] = 200;
		x[2] = 450, y[2] = 200;
		x[3] = 650, y[3] = 200;
		for(int i=0; i<4; ++i)
			isClick[i] = FALSE;
		break;

	case WM_LBUTTONDOWN :
		mx = LOWORD( lParam );
		my = HIWORD( lParam );
		for( int i = 0; i<4; ++i) {
			if( InCircle( x[i], y[i], mx, my ) ) {
				if(isClick[i] == FALSE){
					isClick[i] = TRUE;
				}else{
					isClick[i] = FALSE;
				}
				if(isClick[3] ==TRUE && i==3){
					rgb =RGB(rand()%256,rand()%256,rand()%256);
				}
			}
		}
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_PAINT :
		hdc = BeginPaint( hwnd, &ps );
		for( int i = 0; i<4; ++i ){
			if( isClick[i] == TRUE ){
				if(i == 0 )
					hBrush = CreateSolidBrush( RGB( 255,0,0 ) );
				if(i == 1 )
					hBrush = CreateSolidBrush( RGB( 0,255,0 ) );
				if(i == 2 )
					hBrush = CreateSolidBrush( RGB ( 0, 0, 255 ) );
				if(i == 3 )
					hBrush = CreateSolidBrush( rgb );
			}else{
				hBrush = CreateSolidBrush( RGB(255, 255, 255) ); 
			}
			oldBrush = (HBRUSH) SelectObject( hdc, hBrush );
			Ellipse( hdc,x[i]-BSIZE, y[i]-BSIZE, x[i]+BSIZE, y[i]+BSIZE );
			SelectObject( hdc, oldBrush );
			DeleteObject( hBrush );
		}

		EndPaint( hwnd, &ps );
		break;

	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	}

	return ( DefWindowProc(hwnd, iMsg, wParam, lParam) );
}