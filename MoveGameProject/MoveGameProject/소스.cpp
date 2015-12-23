#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define FSIZE 30

double LengthPts( int x1, int y1, int x2, int y2 ) {
	return( sqrt( (float)( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ) ) );
}

BOOL InCircle( int x, int y, int mx, int my ) {
	if( LengthPts( x, y, mx, my ) < FSIZE ) return TRUE;
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
		1100,
		800,
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
	static RECT board[5][5]; // ÃÊ¹ä ÆÇ
	static POINT food[5][5]; // ÃÊ¹ä
	static int tempL[5][5]; // »ö ÃÊ±âÈ­
	static int tempR[16]; // ¼±ÅÃ ÃÊ¹ä »ö ÃÊ±âÈ­
	static COLORREF red, green, blue; // ÃÊ¹ä »ö
	static int redCnt, blueCnt, greenCnt; // ÃÊ¹ä °¹¼ö
	static POINT tempF[16];// ÃÊ¹ä ÀÓ½Ãº¯¼ö
	static int selecX, selecY;
	static char str[100];
	static int mX, mY; // ¸¶¿ì½º ÁÂÇ¥
	static int cnt; // ¼±ÅÃ ÃÊ¹ä °¹¼ö
	static BOOL selec; // ÃÊ¹ä¼±ÅÃ
	static BOOL selecF[5][5];

	switch( iMsg )
	{
	case WM_CREATE :
		srand( (unsigned) time( 0 ) );
		SetTimer( hwnd, 1, 700, NULL );
		selec = FALSE;
		selecX = 0; selecY = 0;
		cnt = 0;
		red = RGB( 255, 0 , 0 );
		green = RGB( 0, 255, 0 );
		blue = RGB( 0, 0, 255 );
		redCnt = blueCnt = greenCnt = 0;
		for( int i = 0; i<5; ++i )
			for( int j = 0; j<5; ++j ) {
				board[i][j].left = 100*( j+1 );
				board[i][j].top = 100*( i+1 );
				board[i][j].right = 100*( j+2 );
				board[i][j].bottom = 100*( i+2 );
				food[i][j].x = ( board[i][j].left + board[i][j].right )/2;
				food[i][j].y = ( board[i][j].top + board[i][j].bottom )/2;
				tempL[i][j] = rand()%3;
				if( tempL[i][j] == 1 && !( 1<= i && i <=3 && 1<=j && j <=3 ) )
						++redCnt;
				else if( tempL[i][j] == 2 && !( 1<= i && i <=3 && 1<=j && j <=3 ) )
						++blueCnt;
				else if( tempL[i][j] == 0 && !( 1<= i && i <=3 && 1<=j && j <=3 ) )
						++greenCnt;
			}
				break;

	case WM_LBUTTONDOWN :
		hdc = GetDC( hwnd );

		mX = LOWORD( lParam );
		mY = HIWORD( lParam );
		for( int i = 0; i<5; ++i )
			for( int j = 0; j<5; ++j )
				if( InCircle( food[i][j].x, food[i][j].y, mX, mY )
					&& !( 200 <= mX && mX <= 500 && 200 <= mY && mY <= 500 ) ) {
					selec = TRUE;
					selecF[i][j] = TRUE;
					tempF[cnt].x = 720+(80*selecX);
					tempF[cnt].y = 120+(80*selecY);
					tempR[cnt] = tempL[i][j]; // ¿À¸¥ÂÊ ¼±ÅÃÇÑ ÃÊ¹ä
					++cnt;
					++selecX;
					if( selecX == 4 ) {
							selecX = 0;
							++selecY;
					}

					tempL[2][2] = tempL[i][j]; // Áß¾Ó¿¡ °¥ ÃÊ¹ä

					if( tempL[2][2] == 1 )
						--redCnt;
					else if( tempL[2][2] == 2 )
						--blueCnt;
					else if( tempL[2][2] == 0 )
						--greenCnt;
				}

		if( redCnt <= 0 && blueCnt <= 0 && greenCnt <= 0 ) // °°Àº °É ¼±ÅÃÇØµµ ¾È ²¨ÁöÁö··
			KillTimer( hwnd, 1 );

		ReleaseDC( hwnd, hdc );
		InvalidateRgn( hwnd, NULL, FALSE );
		break;

	case WM_LBUTTONUP :
		break;

	case WM_TIMER :
		for( int i = 0; i<5; ++i ) {
			for( int j = 0; j<5; ++j ) {
				if( i == 0 && j == 0 ) { // À­ÂÊ
					for( int j = 0; j<5; ++j ) {
						board[i][j].left = board[i][j+1].left;
						board[i][j].top = board[i][j+1].top;
						board[i][j].right = board[i][j+1].right;
						board[i][j].bottom = board[i][j+1].bottom;
						food[i][j].x = food[i][j+1].x;
						food[i][j].y = food[i][j+1].y;
					}
				} else if( i == 0 && j == 4 ) { // ¿À¸¥ÂÊ
					for( int i = 0; i<5; ++i ) {
						board[i][j].left = board[i+1][j].left;
						board[i][j].top = board[i+1][j].top;
						board[i][j].right = board[i+1][j].right;
						board[i][j].bottom = board[i+1][j].bottom;
						food[i][j].x = food[i+1][j].x;
						food[i][j].y = food[i+1][j].y;
					}
				} else if( i == 4 && j == 4 ) { // ¾Æ·§ÂÊ
					for( int j = 4; 0<j; --j ) {
						board[i][j].left = board[i][j-1].left;
						board[i][j].top = board[i][j-1].top;
						board[i][j].right = board[i][j-1].right;
						board[i][j].bottom = board[i][j-1].bottom;
						food[i][j].x = food[i][j-1].x;
						food[i][j].y = food[i][j-1].y;
					}
				} else if( i == 4 && j == 0 ) { // ¿ÞÂÊÁÙ
					for( int i = 4; 0<i; --i ) {
						board[i][j].left = board[i-1][j].left;
						board[i][j].top = board[i-1][j].top;
						board[i][j].right = board[i-1][j].right;
						board[i][j].bottom = board[i-1][j].bottom;
						food[i][j].x = food[i-1][j].x;
						food[i][j].y = food[i-1][j].y;
					}
				}
			}
		}
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_MOUSEMOVE :
		//hdc = GetDC( hwnd );
		//ReleaseDC( hwnd, hdc );
		break;

	case WM_PAINT :
		hdc = BeginPaint( hwnd, &ps );

		for( int i = 0; i<5; ++i ) // ÃÊ¹äÆÇ
			for( int j = 0; j<5; ++j )
				Rectangle( hdc, board[i][j].left, board[i][j].top, board[i][j].right, board[i][j].bottom );

		for( int i = 0; i<4; ++i ) // ´©¸¥ ÃÊ¹äÆÇ
			for( int j = 0; j<4; ++j )
				Rectangle( hdc, 680+(j*80), 80+(i*80), 760+(j*80), 160+(i*80) );

		for( int i = 0; i<5; ++i ) { // ÃÊ¹äµé
			for( int j = 0; j<5; ++j ) {
				if( tempL[i][j] == 1 && !( 1 <= i && i <= 3 && 1<= j && j <= 3 ) ) {
					hBrush = CreateSolidBrush( red );
					oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
					if( selecF[i][j] ) break;
					else
						Ellipse( hdc, food[i][j].x-FSIZE, food[i][j].y-FSIZE, food[i][j].x+FSIZE, food[i][j].y+FSIZE );
				} else if( tempL[i][j] == 2 && !( 1 <= i && i <= 3 && 1<= j && j <= 3 ) ) {
					hBrush = CreateSolidBrush( blue );
					oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
					if( selecF[i][j] ) break;
					else
						Ellipse( hdc, food[i][j].x-FSIZE, food[i][j].y-FSIZE, food[i][j].x+FSIZE, food[i][j].y+FSIZE );
				} else if( tempL[i][j] == 0 && !( 1 <= i && i <= 3 && 1<= j && j <= 3 ) ) {
					hBrush = CreateSolidBrush( green );
					oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
					if( selecF[i][j] ) break;
					else
						Rectangle( hdc, food[i][j].x-FSIZE, food[i][j].y-FSIZE, food[i][j].x+FSIZE, food[i][j].y+FSIZE );
				}
			}
		}

		if( selec ) {
			hBrush = CreateSolidBrush( RGB( 255, 255, 255 ) );
			oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
			Rectangle( hdc, board[2][2].left, board[2][2].top, board[2][2].right, board[2][2].bottom );
			if( tempL[2][2] == 1 ) {
					hBrush = CreateSolidBrush( red );
					oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
					Ellipse( hdc, food[2][2].x-FSIZE, food[2][2].y-FSIZE, food[2][2].x+FSIZE, food[2][2].y+FSIZE );
				} else if( tempL[2][2] == 2 ) {
					hBrush = CreateSolidBrush( blue );
					oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
					Ellipse( hdc, food[2][2].x-FSIZE, food[2][2].y-FSIZE, food[2][2].x+FSIZE, food[2][2].y+FSIZE );
				} else if( tempL[2][2] == 0 ) {
					hBrush = CreateSolidBrush( green );
					oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
					Rectangle( hdc, food[2][2].x-FSIZE, food[2][2].y-FSIZE, food[2][2].x+FSIZE, food[2][2].y+FSIZE );
				}
		}

		for( int i = 0; i<cnt; ++i ) {
			if( tempR[i] == 1 ) {
					hBrush = CreateSolidBrush( red );
					oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
					Ellipse( hdc, tempF[i].x-FSIZE, tempF[i].y-FSIZE, tempF[i].x+FSIZE, tempF[i].y+FSIZE );
				} if( tempR[i] == 2 ) {
					hBrush = CreateSolidBrush( blue );
					oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
					Ellipse( hdc, tempF[i].x-FSIZE, tempF[i].y-FSIZE, tempF[i].x+FSIZE, tempF[i].y+FSIZE );
				} if( tempR[i] == 0 ) {
					hBrush = CreateSolidBrush( green );
					oldBrush = ( HBRUSH ) SelectObject( hdc, hBrush );
					Rectangle( hdc, tempF[i].x-FSIZE, tempF[i].y-FSIZE, tempF[i].x+FSIZE, tempF[i].y+FSIZE );
				}
		}

		sprintf( str, "selecX : %d / selecY : %d", 640+(80*selecX)-FSIZE, 120+(80*selecY)-FSIZE );
		TextOut( hdc, 300, 200, str, strlen(str) );

		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );

		EndPaint( hwnd, &ps );
		break;

	case WM_DESTROY :
		KillTimer( hwnd, 1 );
		PostQuitMessage( 0 );
		break;
	}

	return ( DefWindowProc(hwnd, iMsg, wParam, lParam) );
}
