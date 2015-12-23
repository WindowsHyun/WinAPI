#include<Windows.h>
#include<windowsx.h>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"resource.h"

#define ScreenWidth 800
#define ScreenHeight 600

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
						"Kim",
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
		//if(!TranslateAccelerator(hWnd,hAcc,&msg)) // 단축키 -> 메뉴 ID로 인식
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

	HWND hDlg = NULL;

	static RECT rt = { 0, 0, 800,600 };
	static HINSTANCE hInstance;

	static POINT p;

	srand(time(NULL));

	switch(iMsg){
		case WM_CREATE:{
			GetClientRect(hWnd, &rt);
			hInstance = g_hInst;
		}break;

		case WM_COMMAND :{
			switch( LOWORD(wParam) ){
				case ID_40001:{
					if( !IsWindow(hDlg) ){
						hDlg = CreateDialog (hInstance, MAKEINTRESOURCE(IDD_DIALOG2),
							hWnd, DlgProc);
						ShowWindow( hDlg, SW_SHOW );
					}
				}
				break;
			}
		}break;

		case WM_PAINT:{
			hdc = BeginPaint(hWnd, &ps);
		
			EndPaint(hWnd, &ps);
		}break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc( hWnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam){
	
	PAINTSTRUCT ps;
	HDC hdc;

	static RECT rt;
	static int selection, Radio;
	static POINT p,mp;
	static HWND hList, hButton, hCombo, hEdit1, hEdit2, hRadio1, hRadio2;
	static char name[200], name1[20], name2[20], name3[20];
	char sex[][30] = {"남자", "여자"};

	switch(iMsg){
		case WM_INITDIALOG:{	// 대화상자 초기화
			GetClientRect( hDlg, &rt );

			// 가입버튼 비활성화
			hButton = GetDlgItem (hDlg, IDC_BUTTON1);
			EnableWindow (hButton, FALSE);

			// 성명 란 비활성화
			hEdit1 = GetDlgItem (hDlg, IDC_EDIT1);
			EnableWindow (hEdit1 , FALSE);

			// 전화번호 란 비활성화
			hEdit2 = GetDlgItem (hDlg, IDC_EDIT3);
			EnableWindow (hEdit2, FALSE);

			// 성별 란 비 활성화
			hRadio1 = GetDlgItem (hDlg, IDC_RADIO2);
			EnableWindow (hRadio1, FALSE);
			hRadio2 = GetDlgItem (hDlg, IDC_RADIO1);
			EnableWindow (hRadio2, FALSE);

			// 출생연도 박스 설정 & 비활성화
			hCombo = GetDlgItem( hDlg, IDC_COMBO1 );
			for( int i = 1990; i< 2014; ++i ){
				wsprintf( name, "%d", i );
				SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM)name);
			}
			//EnableWindow( hCombo, FALSE );

			// 회원 명단 리스트 박스 핸들
			hList = GetDlgItem(hDlg, IDC_LIST1);
		}break;

		case WM_COMMAND:{
			switch (LOWORD(wParam)) {

				case IDCANCEL: {
					PostQuitMessage(0);
				}break;

				case IDOK:{
					// 가입버튼 활성화
					hButton = GetDlgItem (hDlg, IDC_BUTTON1);
					EnableWindow (hButton, TRUE);		

					// 성명 란 활성화
					hEdit1 = GetDlgItem (hDlg, IDC_EDIT1);
					EnableWindow (hEdit1 , TRUE);

					// 전화번호 란 활성화
					hEdit2 = GetDlgItem (hDlg, IDC_EDIT3);
					EnableWindow (hEdit2, TRUE);

					// 성별 란 활성화
					hRadio1 = GetDlgItem (hDlg, IDC_RADIO2);
					EnableWindow (hRadio1, TRUE);
					hRadio2 = GetDlgItem (hDlg,IDC_RADIO1);
					EnableWindow (hRadio2, TRUE);

					// 출생연도 활성화
					hCombo = GetDlgItem( hDlg, IDC_COMBO1 );
					EnableWindow( hCombo, TRUE );
				}break;

				case IDC_RADIO2:{
					Radio = 0;			 
				}break;

				case IDC_RADIO1:{
					Radio = 1;			 
				}break;


				case IDC_COMBO1:{
					if (HIWORD(wParam) == CBN_SELCHANGE) 
						selection = SendMessage(hCombo, CB_GETCURSEL,0,0);					
				}break;

				case IDC_BUTTON1:{

					GetDlgItemText( hDlg, IDC_EDIT1, name1, 20 );
					GetDlgItemText( hDlg, IDC_EDIT3, name2, 20 );
					GetDlgItemText( hDlg, IDC_COMBO1, name3, 20 );

					wsprintf(name, "이름: %s, 전화번호: %s, 성별: %s, 출생연도: %s", name1, name2, sex[Radio], name3 );
					if (strcmp(name, ""))
						SendMessage(hList,LB_ADDSTRING, 0,(LPARAM)name);

					// 가입버튼 비활성화
					hButton = GetDlgItem (hDlg, IDC_BUTTON1);
					EnableWindow (hButton, FALSE);

					// 성명 란 비활성화
					hEdit1 = GetDlgItem (hDlg, IDC_EDIT1);
					EnableWindow (hEdit1 , FALSE);

					// 전화번호 란 비활성화
					hEdit2 = GetDlgItem (hDlg, IDC_EDIT3);
					EnableWindow (hEdit2, FALSE);

					// 성별 란 비 활성화
					hRadio1 = GetDlgItem (hDlg, IDC_RADIO2);
					EnableWindow (hRadio1, FALSE);
					hRadio2 = GetDlgItem (hDlg, IDC_RADIO1);
					EnableWindow (hRadio2, FALSE);

					// 출생연도 비활성화
					hCombo = GetDlgItem( hDlg, IDC_COMBO1 );
					EnableWindow( hCombo, FALSE );
							 
				}break;

				case IDC_LIST1:{
					if (HIWORD(wParam) == LBN_SELCHANGE)
						selection = SendMessage(hList, LB_GETCURSEL,0,0);		   
				}break;

				case IDC_BUTTON2:{
					SendMessage(hList, LB_DELETESTRING, selection, 0);
				}break;

			}
			InvalidateRect( hDlg, NULL, TRUE );
		}break;

		case WM_PAINT:{
			hdc = BeginPaint( hDlg, &ps );		


			EndPaint( hDlg, &ps );
		}break;
	}
	return 0;
}