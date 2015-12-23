#include<Windows.h>
#include<windowsx.h>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"resource.h"

#define ScreenWidth 800
#define ScreenHeight 600

static int selection, selection2;
COLORREF color[8];

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK DlgProc( HWND, UINT, WPARAM, LPARAM );
HINSTANCE g_hInst;

int WINAPI WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow){

	HWND hWnd;
	MSG msg;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	HACCEL hAcc;

	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH);
	WndClass.lpszClassName = "WndClass";
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd =CreateWindow( "WndClass",
						"kch",
						WS_OVERLAPPEDWINDOW,
						0,
						0,
						ScreenWidth,
						ScreenHeight,
						NULL,
						(HMENU)NULL,
						hInstance,
						NULL);

	if(!hWnd)
		return(FALSE);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	//hAcc=LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while(GetMessage(&msg,NULL,0,0)){
		//if(!TranslateAccelerator(hWnd,hAcc,&msg)) // ����Ű -> �޴� ID�� �ν�
		//{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		//}
	}
	return msg.wParam;

}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam){

	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hBrush, oldBrush;
	HWND hDlg = NULL;

	static RECT rt = { 0, 0, 800,600 };
	static HINSTANCE hInstance;

	static POINT p;

	srand(time(NULL));

	switch(iMsg){
		case WM_CREATE:{
			GetClientRect(hWnd, &rt);
			hInstance = g_hInst;
			color[0] = RGB(255,0,0);
            color[1] = RGB(0,255,0);
            color[2] = RGB(0,0,255);
            color[3] = RGB(255,255,0);
            color[4] = RGB(0,255,255);
            color[5] = RGB(255,0,255);
            color[6] = RGB(12,51,43);
            color[7] = RGB(0,0,0);
			SetTimer(hWnd, 1, 100, NULL);
		}break;

			case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE); //��ȿȭ �Լ�
		break;

		case WM_COMMAND :{
			switch( LOWORD(wParam) ){
				case ID_40001:{
					if( !IsWindow(hDlg) ){
						hDlg = CreateDialog (hInstance, MAKEINTRESOURCE(IDD_DIALOG3),
							hWnd, DlgProc);
						ShowWindow( hDlg, SW_SHOW );
					}
				}
				break;
			}
		}break;

		case WM_PAINT:{
			hdc = BeginPaint(hWnd, &ps);

			hBrush = CreateSolidBrush(color[selection2]);
			oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
			Rectangle(hdc,rt.left, rt.top, rt.right, rt.bottom ); //����, ž , ������, �Ʒ�
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);


			EndPaint(hWnd, &ps);
		}break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc( hWnd, iMsg, wParam, lParam);
}

//TCHAR *color[] = {TEXT("����"), TEXT("�Ķ�"), TEXT("�ʷ�"), TEXT("����"), TEXT("���"), TEXT("����Ÿ"), TEXT("�þ�"), TEXT("����")};
char name6[100][30];
BOOL CALLBACK DlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam){
	
	PAINTSTRUCT ps;
	HDC hdc;

    
	static RECT rt;
	static POINT p,mp;
    static int cnt = 0;
	static HWND hList, hButton, hCombo, hEdit1, hEdit2;
	static char name[200], name1[20], name2[20], name3[20];
	

	switch(iMsg){
		case WM_INITDIALOG:{	// ��ȭ���� �ʱ�ȭ
			GetClientRect( hDlg, &rt );

            
			

			 //������� �ڽ� ���� & ��Ȱ��ȭ
            hList = GetDlgItem(hDlg, IDC_LIST1);
			hCombo = GetDlgItem( hDlg, IDC_COMBO1 );
			for( int i = 0; i< 8; ++i ){
				wsprintf( name, "%d", i );
				SendMessage( hList, LB_ADDSTRING, 0, (LPARAM)name);
			}
			

			// ȸ�� ��� ����Ʈ �ڽ� �ڵ�
			
		}break;

		case WM_COMMAND:{
			switch (LOWORD(wParam)) {

				case IDCANCEL: {
					PostQuitMessage(0);
				}break;

				case IDOK:{
                    GetDlgItemText( hDlg, IDC_EDIT1, name1, 20 ); // �̸� ���ڿ� ȹ��
                    
                    if (strcmp(name1, "")){ // �̸��� ��� ������, �� ������ �O����
                        SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)name1);
                        strcpy(name6[cnt], name1);
                        ++cnt;
                    }
				}break;

				
				case IDC_COMBO1:{
					if (HIWORD(wParam) == CBN_SELCHANGE) 
						selection = SendMessage(hCombo, CB_GETCURSEL,0,0);					
				}break;

				case IDC_BUTTON1:{

					
                    InvalidateRect( hDlg, NULL, TRUE );
					
							 
				}break;

				case IDC_LIST1:{
					if (HIWORD(wParam) == LBN_SELCHANGE)
						selection2 = SendMessage(hList, LB_GETCURSEL,0,0);		   
				}break;

				case IDC_BUTTON2:{
                   
                    InvalidateRect( hDlg, NULL, TRUE );
					
				}break;

			}
			//InvalidateRect( hDlg, NULL, TRUE );
		}break;

		case WM_PAINT:{
			hdc = BeginPaint( hDlg, &ps );	
            SetBkColor (hdc, color[selection2]);
            TextOut(hdc, 20,20,name6[selection],lstrlen(name6[selection]));

			EndPaint( hDlg, &ps );
		}break;
	}
	return 0;
}