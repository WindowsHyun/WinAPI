#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmd){
	HWND hWnd;
	MSG message;
	g_hInst=hInstance;
	WNDCLASS WndClass;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_HAND);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=g_hInst;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=TEXT("MyWnd");
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_VREDRAW|CS_HREDRAW;

	RegisterClass(&WndClass);

	hWnd=CreateWindow(TEXT("MyWnd"),TEXT("¹¹"),WS_OVERLAPPEDWINDOW|WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,800,600,NULL,NULL,g_hInst,0);

	while(GetMessage(&message,0,0,0)){
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rt;
	static POINT point[6][3];
	static int type[6];
	static COLORREF color[6];
	static int index=0;


	switch(iMessage){
	case WM_CREATE:
		GetClientRect(hWnd,&rt);
		return 0;
	case WM_KEYDOWN:
		switch(wParam){
		case 'T':
			point[index%6][0].x=rand()%rt.right;
			point[index%6][0].y=rand()%rt.bottom;

			point[index%6][1].x=rand()%rt.right;
			point[index%6][1].y=rand()%rt.bottom;

			point[index%6][2].x=rand()%rt.right;
			point[index%6][2].y=rand()%rt.bottom;

			color[index%6]=RGB(rand()%255,rand()%255,rand()%255);

			type[index%6]=1;
			index++;
			break;
		case 'R': case 'E':
			point[index%6][0].x=rand()%rt.right;
			point[index%6][0].y=rand()%rt.bottom;

			point[index%6][1].x=rand()%rt.right;
			point[index%6][1].y=rand()%rt.bottom;

			color[index%6]=RGB(rand()%255,rand()%255,rand()%255);

			if(wParam=='R'){
				type[index%6]=2;
			}
			else{
				type[index%6]=3;
			}
			index++;
			break;
		}
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		HBRUSH hBrush,oldBrush;
		for(int i=0;i<(index>6?6:index);++i){
			hBrush=CreateSolidBrush(color[i]);
			oldBrush=(HBRUSH)SelectObject(hdc,hBrush);
			switch(type[i]){
			case 1:
				Polygon(hdc,point[i],3);
				break;
			case 2:
				Rectangle(hdc,point[i][0].x,point[i][0].y,point[i][1].x,point[i][1].y);
				break;
			case 3:
				Ellipse(hdc,point[i][0].x,point[i][0].y,point[i][1].x,point[i][1].y);
				break;
			}
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd,iMessage,wParam,lParam);
}