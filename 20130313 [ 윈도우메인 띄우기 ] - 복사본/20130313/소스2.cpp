#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define BSIZE 50

double LengthPts( double x1, double y1, double x2, double y2 ) {
	return( sqrt( (float)( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ) ) );
}

BOOL InCircle(double x, double y, double mx, double my)
{
	if(LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}

bool CircleInCircle( double x1, double y1, double x2, double y2,double Radius1, double Radius2)
{
	double  CharX = x1 - x2; // ��ü1�� ��ü2�� x����ŭ�� ����
	double  CharY = y1 - y2 ; //  ��ü1�� ��ü2�� y����ŭ�� ����

	double distance = sqrt(CharX*CharX + CharY * CharY);   
	// distance ��Ÿ����� ������ ���� c�� = a�� + b��

	if(distance <= Radius1 + Radius2) // �� ���հ��� �� ���� �������� �պ��� ũ��(+) ���浹, ���Ŵٸ� ���鿡 �浹 �۴ٸ�(-) �浹
		return TRUE; // �浹�ϸ�
	else 
		return FALSE; // �ƴϸ�
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
	static double rada[10];
	static double x[10], y[10]; // �� ����
	static double cX, cY;
	static int temp[10], cnt;
	static BOOL selec;
	int mx, my;

	switch( iMsg )
	{

	case WM_CREATE :
		srand( (unsigned) time( 0 ) );
		cnt = 0; // ī��Ʈ 0���;�
		cX = cY = 0;
		selec = FALSE;

		for( int i = 0; i<10; ++i ) {
			x[i] = rand()%800;
			y[i] = rand()%600;
			rada[i] = rand()%95+5;
			temp[i] = NULL;
		}
		for( int j = 0; j<10; ++j ) {
			x[j] = rand()%800;
			y[j] = rand()%600;
			rada[j] = rand()%95+5;
		}
		break;

	case WM_TIMER:
		break;

	case WM_LBUTTONDOWN :
		mx = LOWORD( lParam );
		my = HIWORD( lParam );
		if( InCircle( cX, cY, mx, my ) ) selec = TRUE;
		//InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_LBUTTONUP :
		selec = FALSE;
		break;

	case WM_MOUSEMOVE :
		mx = LOWORD( lParam );
		my = HIWORD( lParam );
		if( selec ) {
			cX = mx;
			cY = my;
			InvalidateRgn( hwnd, NULL, TRUE );
		}
		break;

	case WM_PAINT :
		hdc = BeginPaint( hwnd, &ps );
		for( int i = 0; i<10; ++i ) {
			if( CircleInCircle( cX, cY, x[i], y[i], BSIZE, rada[i] ) ) { // �浹 ���� ��
				hBrush = CreateSolidBrush( RGB( 255, 0, 0 ) ); // ������
				temp[i] = i;
				++cnt;
			}
			else
				hBrush = CreateSolidBrush( RGB( 0, 255, 0 ) ); // �� �浹 �ʷϻ�

			oldBrush = (HBRUSH) SelectObject( hdc, hBrush );
			Ellipse( hdc, x[i]-rada[i], y[i]-rada[i], x[i]+rada[i], y[i]+rada[i] );
			SelectObject( hdc, oldBrush );
		}

		Ellipse( hdc, cX-BSIZE, cY-BSIZE, cX+BSIZE, cY+BSIZE );
		
		DeleteObject( hBrush );
		EndPaint( hwnd, &ps );
		break;

	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	}

	return ( DefWindowProc(hwnd, iMsg, wParam, lParam) );
}