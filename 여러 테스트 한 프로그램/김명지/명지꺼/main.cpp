#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define BSIZE 20

LRESULT CALLBACK WndProc( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LPCTSTR lpszClass = "Window Class";
HINSTANCE g_hInst;

BOOL Check( int cX, int cY, int rX, int rY )
{	// rX-300, rY+495, rX+150, rY+545
	if( rY+545-50 <= cY+BSIZE && cX-BSIZE <= rX-300 && cX+BSIZE >= rX+150 )
		return TRUE;
	else 
		return FALSE;
}

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
	static int rX, rY, cX, cY;
	static int mX, mY, speed;
	static int temp;
	static RECT window;
	static BOOL selec, save;
	static char Debug[50];

	switch( iMsg )
	{

	case WM_CREATE :
		GetClientRect( hwnd, &window );
		speed = 70;
		SetTimer( hwnd, 1, speed, NULL );
		rX = 450, rY = 25;
		cX = 380, cY = 500;
		selec = save = FALSE;
		break;

	case WM_TIMER:
		switch( wParam ) {
		case 1 :
			cX -= 10;
			cY -= 10;
			temp = 1;
			if( cX-BSIZE <= window.left ) {
				KillTimer( hwnd, 1 );
 				SetTimer( hwnd, 2, speed, NULL );
			}else if( cY-BSIZE <= window.top ) {
				KillTimer( hwnd, 1 );
				KillTimer( hwnd, 3 );
				KillTimer( hwnd, 4 );
				SetTimer( hwnd, 2, speed, NULL );
			}

			break;
		case 2:
			cX += 10;
			cY -= 10;
			temp = 2;
			if( cY-BSIZE <= window.top ) {
				KillTimer( hwnd, 1 );
				KillTimer( hwnd, 2 );
				KillTimer( hwnd, 4 );
				SetTimer( hwnd, 3, speed, NULL );
			}
			break;
		case 3 :
			cX += 10;
			cY += 10;
			temp = 3;
			if( cX+BSIZE >= window.right ) {
				KillTimer( hwnd, 1 );
				KillTimer( hwnd, 2 );
				KillTimer( hwnd, 3 );
				SetTimer( hwnd, 4, speed, NULL );
			}
			if( Check( cX, cY, rX, rY ) ) {
				KillTimer( hwnd, 3 );
				SetTimer( hwnd, 1, speed, NULL );
			}
			break;
		case 4:
			cX -= 10;
			cY += 10;
			temp = 4;
			if( cY+BSIZE >= rY+495 && cX-BSIZE <= rX-300 && cX+BSIZE >= rX+150 ) {
				KillTimer( hwnd, 4 );
				SetTimer( hwnd, 1, speed, NULL );
			}
			break;
		}
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_CHAR :	
		if( wParam == '=' ) {
			KillTimer( hwnd, temp );
			speed -=20;
			SetTimer( hwnd, temp, speed, NULL );
		}
		else if( wParam == '-' ) {
			KillTimer( hwnd, temp );
			speed += 20;
			SetTimer( hwnd, temp, speed, NULL );
		}
		break;

	case WM_LBUTTONDOWN :
		mX = LOWORD( lParam );
		mY = HIWORD( lParam );

		if( mX >= rX-300 && mX <= rX+150 && mY >= rY+495 && mY <= rY+545 )
			selec = TRUE;
		InvalidateRect (hwnd, NULL, TRUE );
		break;

	case WM_LBUTTONUP :
		selec = FALSE;
		break;

	case WM_RBUTTONDOWN :
		mX = LOWORD( lParam );
		mY = HIWORD( lParam );

		if( mX >= rX-300 && mX <= rX+150 && mY >= rY+495 && mY <= rY+545 )
			selec = TRUE;
		InvalidateRect (hwnd, NULL, TRUE );
		break;

	case WM_RBUTTONUP :
		selec = FALSE;
		save = FALSE;
		rX = 450, rY = 25;
		break;

	case WM_MOUSEMOVE :
		mX = LOWORD( lParam );
		mY = HIWORD( lParam );

		if( selec )
			rX = mX;

		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_PAINT :
		hdc = BeginPaint( hwnd, &ps );
		Rectangle( hdc, rX-300, rY+495, rX+150, rY+545 );
		Ellipse( hdc, cX-BSIZE, cY-BSIZE, cX+BSIZE, cY+BSIZE ); //왼쪽, 탑 , 오른쪽, 아래

		sprintf(Debug, "오른쪽 : %d, 아래 : %d // 오른쪽 : %d, 아래 : %d", cX+BSIZE,cY+BSIZE, rX+150, rY+545);
		TextOut(hdc, 0,510, Debug, strlen(Debug));

		EndPaint( hwnd, &ps );
		break;

	case WM_DESTROY :
		PostQuitMessage(0);
		KillTimer( hwnd, 1 );
		KillTimer( hwnd, 2 );
		KillTimer( hwnd, 3 );
		KillTimer( hwnd, 4 );
		break;
	}

	return ( DefWindowProc(hwnd, iMsg, wParam, lParam) );
}