#include <windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	static char szClassName[] = "Hello";
	static char szTitle[] = "Window API Program1";	//타이틀바꾸기

	MSG msg;
	HWND hWnd;
	WNDCLASSEX wc;
	HACCEL hAcc;
	g_hInst = hInstance;


	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//배경색 지정
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);
	hWnd = CreateWindow(
		szClassName, szTitle,
		WS_OVERLAPPEDWINDOW,
		0, 0,//0, 0,		// x, y 좌표. 기본위치: CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,			// 윈도우 크기. 기본: CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL,
		hInstance,
		NULL);

	if(!hWnd) return (FALSE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hAcc=LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(hWnd, hAcc, &msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	HDC hdc, MemDC;
	PAINTSTRUCT ps;

	static HDC ImgDC;	//이미지 복사->ImgDC로 선택범위 안의 이미지를 옮김
			//->
	static HBITMAP Bitmap;	//비트맵 핸들
	static BITMAP Bitsz;	//비트맵 사이즈
	RECT winsz;				//윈도우 사이즈
	static int i=0, j=0;	
	static RECT mouse;		//마우스 지정 사각영역(복사할 부분)
	static POINT size, mPos;//복사 영역 사이즈, 마우스좌표
	static BOOL drag=0, imgcopy=0;//이미지 영역 지정o/x, 이미지 복사중~
	static BOOL imgtipe=0, img=0;//이미지 붙여넣기중~, 복사된 이미지 유무
	HPEN Pen, OldPen;


	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		mouse.left=LOWORD(lParam);
		mouse.top=HIWORD(lParam);
		drag=1;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		if(drag){
			mouse.right=LOWORD(lParam);
			mouse.bottom=HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		mPos.x=LOWORD(lParam);
		mPos.y=HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		mouse.right=LOWORD(lParam);
		mouse.bottom=HIWORD(lParam);
		if(mouse.right!=mouse.left&&mouse.top!=mouse.bottom){
			imgcopy=1;	//좌변-우변, 상변-하변이 일치하지 않으면(선택된 부분이 있으면)
						//이미지 복사 가능.
			size.x=mouse.right-mouse.left;	//복사한 이미지 가로 세로 사이즈
			size.y=mouse.bottom-mouse.top;
			drag=0;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case ID_CTRLC:
			if(imgcopy){
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else
				MessageBox(hWnd, "이미지가 이미 복사되었거나\n복사할 수 있는 이미지가 없습니다.", "Error!", MB_OK);
			break;
		case ID_CTRLV:
			if(imgcopy){
				imgtipe=1;
				imgcopy=0;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else
				MessageBox(hWnd, "복사해둔 이미지가 없습니다.", "Error!!", MB_OK);
			break;
		}
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		MemDC=CreateCompatibleDC(hdc);
		ImgDC=CreateCompatibleDC(hdc);
		SelectObject(MemDC, Bitmap);		
		GetClientRect(hWnd, &winsz);
		StretchBlt(hdc, 0, 0, winsz.right, winsz.bottom, MemDC,0, 0, Bitsz.bmWidth, Bitsz.bmHeight, SRCCOPY);
				//원본 이미지를 그림
		Pen=CreatePen(PS_DOT, 1, BLACK_PEN);
		OldPen=(HPEN)SelectObject(hdc, Pen);
		SetROP2(hdc, R2_NOTXORPEN);
		if(drag){
			Rectangle(hdc, mouse.left, mouse.top, mouse.right, mouse.bottom);
			Rectangle(hdc, mouse.left, mouse.top, mouse.right, mouse.bottom);
		}
		else{
			Rectangle(hdc, mouse.left, mouse.top, mouse.right, mouse.bottom);
		}
		if(imgcopy){
			BitBlt(ImgDC, 0, 0, size.x, size.y, hdc, mouse.left, mouse.top, SRCCOPY);
		}
		if(imgtipe){
			BitBlt(hdc, mPos.x, mPos.y, size.x, size.y, hdc, mouse.left, mouse.top, SRCCOPY);
			img=0;
			imgtipe=0;
		}

		SelectObject(hdc, OldPen);
		DeleteObject(Pen);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		SetTimer(hWnd, 1, 500, NULL);
		Bitmap=(HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(Bitmap, sizeof(BITMAP), &Bitsz);
		break;
	case WM_DESTROY:
		DeleteDC(ImgDC);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
