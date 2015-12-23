#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define BSIZE 20

//double LengthPts( double x1, double y1, double x2, double y2 ) {
//	return( sqrt( (float)( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ) ) );
//}
//
//BOOL InCircle( double x, double y, double mx, double my )
//{
//	if(LengthPts(x, y, mx, my) < BSIZE) return TRUE;
//	else return FALSE;
//}
//
//bool CircleInCircle( double x1, double y1, double x2, double y2,double Radius1, double Radius2)
//{
//	double  CharX = x1 - x2; // ��ü1�� ��ü2�� x����ŭ�� ����
//	double  CharY = y1 - y2 ; //  ��ü1�� ��ü2�� y����ŭ�� ����
//
//	double distance = sqrt(CharX*CharX + CharY * CharY);   
//	// distance ��Ÿ����� ������ ���� c�� = a�� + b��
//
//	if(distance <= Radius1 + Radius2) // �� ���հ��� �� ���� �������� �պ��� ũ��(+) ���浹, ���Ŵٸ� ���鿡 �浹 �۴ٸ�(-) �浹
//		return TRUE; // �浹�ϸ�
//	else 
//		return FALSE; // �ƴϸ�
//}

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
	//HBRUSH hBrush, oldBrush;
	static int x, y;
	static int mX, mY, speed;
	static int tempX, tempY;
	static RECT window;
	static BOOL selec, save;

	switch( iMsg )
	{

	case WM_CREATE :
	//		�ٴ� �¿�θ�
	//Ư��Ű�� ������ �Ʒ��� �� �̵� ��� �� �� ������� �ٰ� �̵��ϰ� �ȴ�.
	//// �� �� ����
	//�̵���� 1 : ���� ���콺�� �� ������ ���� ä�� �̵��ϸ� �ٵ� �� ��ġ�� �̵��ϰ�, ���콺 ��ư�� ������ �� �ڸ���
	//�ִ´�.
	//
	//�̵���� 2 : ���� ���콺�� �� ���� ���� ä�� �̵��ϸ� �ٵ� �� ��ġ�� �̵��ϰ� ���콺 ��ư�� ������ �ٰ� ������
	//��ġ�� �ٽ� �̵��Ѵ�.
	//
	//���� �ӵ��� ��/��
		GetClientRect( hwnd, &window ) ;
		x = 150, y = 500;
		selec = save = FALSE;
		break;

	case WM_TIMER:
		break;

	case WM_LBUTTONDOWN :
		mX = LOWORD( lParam );

		if( save == FALSE ) {
			tempX = mX;
			tempY = mY;
			save = TRUE;
		}
		//if( mX >= x && mX <= x+450 && mY >= y && mY <= y+200 )
			selec = TRUE;
		break;

	case WM_LBUTTONUP :
		mX = LOWORD( lParam );
		break;

	case WM_RBUTTONDOWN :
		mX = LOWORD( lParam );
		if( mX >= x && mX <= x+450 && mY >= y && mY <= y+200 )
			selec = TRUE;
		break;

	case WM_RBUTTONUP :
		x = 150, y = 500;
		break;

	case WM_MOUSEMOVE :
		if( selec )
			x += mX - tempX;
		InvalidateRgn( hwnd, NULL, TRUE );
		break;

	case WM_PAINT :
		hdc = BeginPaint( hwnd, &ps );
		Rectangle( hdc, x, y, x+450, y+200 );
		EndPaint( hwnd, &ps );
		break;

	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	}

	return ( DefWindowProc(hwnd, iMsg, wParam, lParam) );
}