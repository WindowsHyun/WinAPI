#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define BSIZE 50

double LengthPts( int x1, int y1, int x2, int y2 ) {
	return( sqrt( (float)( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ) ) );
}

BOOL InCircle( int x, int y, int mx, int my ) {
	if( LengthPts( x, y, mx, my ) < BSIZE ) return TRUE;
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
	static int mouX, mouY, catX, catY;
	static BOOL go, push, magic;
	static char str[10] = "Áã";
	static char Debug[50];
	int mx, my;

	switch( iMsg ){
		srand((unsigned int)time(NULL));
	case WM_CREATE :
		catX = rand()%800, catY = rand()%600;
		go = push = FALSE;
		break;

	case WM_MOUSEMOVE :
		push = TRUE;
		mx = LOWORD( lParam );
		my = HIWORD( lParam );
		mouX = mx, mouY = my;
		SetTimer( hwnd, 1, 1, NULL );
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_LBUTTONDOWN :
		magic=TRUE;
		break;

	case WM_LBUTTONUP :
		push = FALSE;
		go = FALSE;
		magic = FALSE;
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_TIMER :
		if( push ) {
			if( catX > mouX )
				--catX;
			if( catY > mouY )
				--catY;
			if( catX < mouX )
				++catX;
			if( catY < mouY )
				++catY;
			if( catX == mouX && catY == mouY )
				KillTimer(hwnd, 1);
		}
		InvalidateRgn( hwnd, NULL, TRUE );

	case WM_PAINT :
		hdc = BeginPaint( hwnd, &ps );
		if( push ){
			if(magic==TRUE){
				for(int i=0; i<rand()%10000; ++i){
					TextOut( hdc, rand()%400+rand()%400, rand()%300+rand()%300, "Áã.", strlen( "Áã." ) );
					mouX = rand()%400+rand()%400;
					mouY = rand()%300+rand()%300;
				}
				TextOut( hdc, mouX, mouY, "Áã", strlen( "Áã" ) );
			}else{
				TextOut( hdc, mouX, mouY, "Áã", strlen( "Áã" ) );
			}
		}
		TextOut( hdc, catX, catY, "°í¾çÀÌ", strlen("°í¾çÀÌ") );


		sprintf(Debug, "catX : %d, catY : %d, mouX : %d, mouY : %d", catX, catY, mouX, mouY);
		TextOut(hdc, 0,510, Debug, strlen(Debug));
		EndPaint( hwnd, &ps );
		break;

	case WM_DESTROY :
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}

	return ( DefWindowProc(hwnd, iMsg, wParam, lParam) );
}