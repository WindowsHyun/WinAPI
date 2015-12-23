#include <Windows.h>
#include <stdio.h>
#include<time.h>
#include"resource.h"

LPCSTR lpszClass = "Window Class";
HINSTANCE g_hInst;

BOOL checkFunction( int i, int j, int mX, int mY ) {
	if( i*780/6 <= mX && mX <= ( i+1 )*780/6 && j*540/3 <= mY && mY <= ( j+1 )*540/3 ) {
		return TRUE;
	} else return FALSE;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	//HACCEL hacc;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	//hacc=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, "my first window program", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL,
		(HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Message, 0, 0, 0)) {
		//if(!TranslateAccelerator(hWnd,hacc,&Message))
		//{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		//}
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc,memdc;
	PAINTSTRUCT ps;
	static HBITMAP hBit,oldBit;
	static HBITMAP cg;
	static RECT window;
	static RECT bitPoint, bitSize;
	static BOOL check3, check4, check5;
	int mX, mY;
	static char str[100];

	switch(iMessage)
	{
	case WM_CREATE:
		GetClientRect( hWnd, &window );
		cg = (HBITMAP)LoadBitmap( ( (LPCREATESTRUCT)lParam ) ->hInstance, MAKEINTRESOURCE( 101 ) );
		bitPoint.left = bitPoint.top = bitPoint.right = bitPoint.bottom = 0;
		bitSize.left = bitSize.top = bitSize.right = bitSize.bottom = 0;
		break;

	case WM_PAINT:
		hdc=BeginPaint( hWnd, &ps );
		memdc = CreateCompatibleDC( hdc );
		SelectObject( memdc, cg );

		StretchBlt( hdc, window.left, window.top, window.right/2, window.bottom, memdc, 0, 0, 645, 535, SRCCOPY );
		StretchBlt( hdc, bitPoint.left, bitPoint.top, bitPoint.right, bitPoint.bottom,
			memdc, bitSize.left, bitSize.top, bitSize.right, bitSize.bottom, SRCCOPY );

		if( check3 ) {
			for( int i = 0; i<5; ++i ) {
				MoveToEx( hdc, window.right*( i+1 )/6, window.top, NULL );
				LineTo( hdc, window.right*( i+1 )/6, window.bottom );
			} for( int i  = 0; i<2; ++i ) {
				MoveToEx( hdc, window.left, window.bottom*( i+1 )/3, NULL );
				LineTo( hdc, window.right, window.bottom*( i+1 )/3 );
			}
		} else if( check4 ) {
			for( int i = 0; i<7; ++i ) {
				MoveToEx( hdc, window.right*( i+1 )/8, window.top, NULL );
				LineTo( hdc, window.right*( i+1 )/8, window.bottom );
			} for( int i  = 0; i<3; ++i ) {
				MoveToEx( hdc, window.left, window.bottom*( i+1 )/4, NULL );
				LineTo( hdc, window.right, window.bottom*( i+1 )/4 );
			}
		} else if( check5 ) {
			for( int i = 0; i<9; ++i ) {
				MoveToEx( hdc, window.right*( i+1 )/10, window.top, NULL );
				LineTo( hdc, window.right*( i+1 )/10, window.bottom );
			} for( int i  = 0; i<4; ++i ) {
				MoveToEx( hdc, window.left, window.bottom*( i+1 )/5, NULL );
				LineTo( hdc, window.right, window.bottom*( i+1 )/5 );
			}
		}

		sprintf( str, "%d", bitSize.left );
		TextOut( hdc, 500, 300, str, strlen(str) );

		DeleteDC( memdc );
		EndPaint( hWnd, &ps );
		break;

	case WM_LBUTTONDOWN:
		mX = LOWORD( lParam );
		mY = HIWORD( lParam );

		if( window.left <= mX && mX <= window.right/2 ) {
			for( int i = 0; i<3; ++i ) {
				for( int j = 0; j<3; ++j ) {
					if ( checkFunction( i, j, mX, mY ) ) {
						bitSize.left = i*178;
						bitSize.top = j*215;
						bitSize.right = (i+1) * 178;
						bitSize.bottom = (j+1) * 215;
					} // 사진크기
				}
			}
		} else if( window.right/2 <= mX && mX <= window.right ) {
			for( int i = 3; i<=6; ++i ) {
				for( int j = 3; j<=6; ++j ) {
					if ( checkFunction( i, j, mX, mY ) ) {
						bitPoint.left = window.right*j/6;
						bitPoint.top = window.bottom*( i-3 )/3;
						bitPoint.right = window.right*( j+1 )/6;
						bitPoint.bottom = window.bottom*( i-2 )/3;
					}
				}
			}
		}

		//if( ( 0 <= mX && mX <= 133 ) &&
		//	( 0 <= mY && mY <= 200 ) ) {
		//		bitSize.left = 0;
		//		bitSize.top = 0;
		//		bitSize.right = 178;
		//		bitSize.bottom = 215;
		//} else if( ( 133 <= mX && mX <= 266 ) &&
		//	( 0 <= mY && mY <= 200 ) ) {
		//		bitSize.left = 175;
		//		bitSize.top = 0;
		//		bitSize.right = 356;
		//		bitSize.bottom = 215;
		//} else if( ( 266 <= mX && mX <= 399 ) &&
		//	( 0 <= mY && mY <= 200 ) ) {
		//		bitSize.left = 356;
		//		bitSize.top = 0;
		//		bitSize.right = 534;
		//		bitSize.bottom = 215;
		//} else if( ( 0 <= mX && mX <= 133 ) &&
		//	( 200 <= mY && mY <= 400 ) ) {
		//		bitSize.left = 0;
		//		bitSize.top = 215;
		//		bitSize.right = 175;
		//		bitSize.bottom = 430;
		//} else if( ( 133 <= mX && mX <= 266 ) &&
		//	( 200 <= mY && mY <= 400 ) ) {
		//		bitSize.left = 175;
		//		bitSize.top = 215;
		//		bitSize.right = 356;
		//		bitSize.bottom = 430;
		//} else if( ( 266 <= mX && mX <= 399 ) &&
		//	( 200 <= mY && mY <= 400 ) ) {
		//		bitSize.left = 356;
		//		bitSize.top = 215;
		//		bitSize.right = 534;
		//		bitSize.bottom = 430;
		//} else if( ( 0 <= mX && mX <= 133 ) &&
		//	( 400 <= mY && mY <= 600 ) ) {
		//		bitSize.left = 0;
		//		bitSize.top = 430;
		//		bitSize.right = 175;
		//		bitSize.bottom = 645;
		//} else if( ( 133 <= mX && mX <= 266 ) &&
		//	( 400 <= mY && mY <= 600 ) ) {
		//		bitSize.left = 175;
		//		bitSize.top = 430;
		//		bitSize.right = 356;
		//		bitSize.bottom = 645;
		//} else if( ( 266 <= mX && mX <= 399 ) &&
		//	( 400 <= mY && mY <= 600 ) ) {
		//		bitSize.left = 356;
		//		bitSize.top = 430;
		//		bitSize.right = 534;
		//		bitSize.bottom = 645;
		//		// 여기까지 선택 끝
		//} else if( ( 399 <= mX && mX <= 532 ) &&
		//	( 0 <= mY && mY <= 200 ) ) {
		//		bitPoint.left = 399;
		//		bitPoint.top = 0;
		//		bitPoint.right = 532;
		//		bitPoint.bottom = 200;
		//		InvalidateRgn( hWnd, NULL, FALSE );
		//} else if( ( 532 <= mX && mX <= 695 ) &&
		//	( 0 <= mY && mY <= 200 ) ) {
		//		bitPoint.left = 532;
		//		bitPoint.top = 0;
		//		bitPoint.right = 695;
		//		bitPoint.bottom = 200;
		//		InvalidateRgn( hWnd, NULL, FALSE );
		//} else if( ( 665 <= mX && mX <= 798 ) &&
		//	( 0 <= mY && mY <= 200 ) ) {
		//		bitPoint.left = 665;
		//		bitPoint.top = 0;
		//		bitPoint.right = 798;
		//		bitPoint.bottom = 200;
		//		InvalidateRgn( hWnd, NULL, FALSE );
		//}
		// 노가다
		break;

	case WM_LBUTTONUP:
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC( hWnd );
		break;

	//case WM_COMMAND:
	//	switch( LOWORD( wParam ) ) {
	//	case ID_DIVISION_3 :
	//		check3 = TRUE;
	//		check4 = FALSE;
	//		check5 = FALSE;
	//		break;
	//		
	//	case ID_DIVISION_4 :
	//		check3 = FALSE;
	//		check4 = TRUE;
	//		check5 = FALSE;
	//		break;

	//	case ID_DIVISION_5 :
	//		check3 = FALSE;
	//		check4 = FALSE;
	//		check5 = TRUE;
	//		break;
	//	}
	//	InvalidateRgn( hWnd, NULL, TRUE );
	//	break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	}
	return (DefWindowProc(hWnd,iMessage,wParam,lParam));
}