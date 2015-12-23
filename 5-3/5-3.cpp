#include <windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	static char szClassName[] = "Hello";
	static char szTitle[] = "Window API Program1";	//Ÿ��Ʋ�ٲٱ�

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
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//���� ����
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);
	hWnd = CreateWindow(
		szClassName, szTitle,
		WS_OVERLAPPEDWINDOW,
		0, 0,//0, 0,		// x, y ��ǥ. �⺻��ġ: CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,			// ������ ũ��. �⺻: CW_USEDEFAULT, CW_USEDEFAULT,
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

	static HDC ImgDC;	//�̹��� ����->ImgDC�� ���ù��� ���� �̹����� �ű�
			//->
	static HBITMAP Bitmap;	//��Ʈ�� �ڵ�
	static BITMAP Bitsz;	//��Ʈ�� ������
	RECT winsz;				//������ ������
	static int i=0, j=0;	
	static RECT mouse;		//���콺 ���� �簢����(������ �κ�)
	static POINT size, mPos;//���� ���� ������, ���콺��ǥ
	static BOOL drag=0, imgcopy=0;//�̹��� ���� ����o/x, �̹��� ������~
	static BOOL imgtipe=0, img=0;//�̹��� �ٿ��ֱ���~, ����� �̹��� ����
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
			imgcopy=1;	//�º�-�캯, ��-�Ϻ��� ��ġ���� ������(���õ� �κ��� ������)
						//�̹��� ���� ����.
			size.x=mouse.right-mouse.left;	//������ �̹��� ���� ���� ������
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
				MessageBox(hWnd, "�̹����� �̹� ����Ǿ��ų�\n������ �� �ִ� �̹����� �����ϴ�.", "Error!", MB_OK);
			break;
		case ID_CTRLV:
			if(imgcopy){
				imgtipe=1;
				imgcopy=0;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else
				MessageBox(hWnd, "�����ص� �̹����� �����ϴ�.", "Error!!", MB_OK);
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
				//���� �̹����� �׸�
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
