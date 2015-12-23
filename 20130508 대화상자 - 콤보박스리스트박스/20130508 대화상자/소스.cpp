
#include <windows.h>
#include <stdio.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgEx6_8Proc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam);
HINSTANCE hInst;
HWND hList, hCombo;
int Sex;

int isBGcolor=0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	hwnd;
	MSG 	msg;
	WNDCLASS	WndClass;   
	hInst = hInstance;
	WndClass.style 	= CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc	= WndProc;		
	WndClass.cbClsExtra	= 0;		
	WndClass.cbWndExtra	= 0;		
	WndClass.hInstance 	= hInstance;		
	WndClass.hIcon	= LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor 	= LoadCursor(NULL, IDC_ARROW);	
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	
	//WndClass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);		
	WndClass.lpszMenuName	= NULL;
	WndClass.lpszClassName 	= "Window Class Name";	
	RegisterClass(&WndClass);	
	hwnd = CreateWindow(	"Window Class Name",
		"6-5",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,	
		CW_USEDEFAULT,	
		CW_USEDEFAULT,	
		CW_USEDEFAULT,	
		NULL,	
		NULL,	
		hInstance,	
		NULL	 
		);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	
		DispatchMessage(&msg);	
	}   
	return (int)msg.wParam;
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)     
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rt;
	HWND hDlg = NULL;
	HBRUSH hBrush, oldBrush;
	switch (iMsg) 
	{
	case WM_CREATE: 
		SetTimer(hwnd,1,1,NULL);
		GetClientRect(hwnd,&rt);
		hDlg=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG1),hwnd,DlgEx6_8Proc);
		ShowWindow(hDlg,SW_SHOW);
		hList = GetDlgItem(hDlg, IDC_LIST1);
		SendMessage (hList,LB_ADDSTRING,0,(LPARAM)"���");
		SendMessage (hList,LB_ADDSTRING,0,(LPARAM)"����");
		SendMessage (hList,LB_ADDSTRING,0,(LPARAM)"�Ķ�");
		SendMessage (hList,LB_ADDSTRING,0,(LPARAM)"���");
		hList = GetDlgItem(hDlg, IDC_LIST2);
		SendMessage (hList,LB_ADDSTRING,0,(LPARAM)"0");
		SendMessage (hList,LB_ADDSTRING,0,(LPARAM)"1");
		SendMessage (hList,LB_ADDSTRING,0,(LPARAM)"3");
		SendMessage (hList,LB_ADDSTRING,0,(LPARAM)"2");
		//IDC_LIST2
		break;

		//InvalidateRect(hwnd, NULL, TRUE); //��ȿȭ �Լ�

	case WM_TIMER:
		//InvalidateRect(hwnd, NULL, TRUE); //��ȿȭ �Լ�
		break;


	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		if( isBGcolor == 0 ){
			hBrush = CreateSolidBrush(RGB(255,255,255));
			oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
			Rectangle(hdc,rt.left, rt.top, rt.right, rt.bottom ); //����, ž , ������, �Ʒ�
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}else if( isBGcolor == 1 ){
			hBrush = CreateSolidBrush(RGB(255,0,0));
			oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
			Rectangle(hdc,rt.left, rt.top, rt.right, rt.bottom ); //����, ž , ������, �Ʒ�
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}else if( isBGcolor == 2 ){
			hBrush = CreateSolidBrush(RGB(0,255,0));
			oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
			Rectangle(hdc,rt.left, rt.top, rt.right, rt.bottom ); //����, ž , ������, �Ʒ�
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}else if( isBGcolor == 3 ){
			hBrush = CreateSolidBrush(RGB(0,0,255));
			oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
			Rectangle(hdc,rt.left, rt.top, rt.right, rt.bottom ); //����, ž , ������, �Ʒ�
			SelectObject(hdc,oldBrush);
			DeleteObject(hBrush);
		}


		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage (0) ;
		break;
	} 
	return DefWindowProc (hwnd, iMsg, wParam, lParam); 
}
//void InitDlg(HWND hDlg)
//{
//	int i;
//	//static char birthY[10];
//	static char birthY[500];
//	hList = GetDlgItem(hDlg, IDC_LIST);
//	hCombo = GetDlgItem(hDlg, IDC_COMBO_YEAR);
//	SetDlgItemText(hDlg, IDC_EDIT_NAME, "");
//	SetDlgItemText(hDlg, IDC_EDIT_PHONENUM, "");
//	CheckRadioButton(hDlg, IDC_RADIO_F, IDC_RADIO_M, IDC_RADIO_F);
//	
//	Sex = 0;
//	for(int i = 1970; i<2010; i++)
//	{
//		sprintf(birthY, "%d", i);
//		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"1970");
//	}
//	return;
//}
//void NewMember(HWND hDlg)
//{
//	SetDlgItemText(hDlg, IDC_EDIT_NAME, "");
//	SetDlgItemText(hDlg, IDC_EDIT_PHONENUM, "");
//	CheckRadioButton(hDlg, IDC_RADIO_F, IDC_RADIO_M, IDC_RADIO_F);
//	Sex = 0;
//	SendMessage(hCombo, CB_SETCURSEL,0,0);
//	return;
//}
//void Joining(HWND hDlg)
//{
//	char name[30], phone[30], birthY[10], output[200];
//	int Year;
//	char gender[][30] = {"����","����"};
//	GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
//	if (strcmp(name, "")) 
//	{
//		GetDlgItemText(hDlg, IDC_EDIT_PHONENUM, phone, 20);
//		Year = SendMessage(hCombo, CB_GETCURSEL,0,0);
//		sprintf(birthY, "%d", 1970+Year);
//		sprintf(output,"�̸� : %s,  ��ȭ��ȣ : %s,  ���� : %s,  ����⵵ : %s", name, phone, gender[Sex], birthY);
//		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)output);
//	}
//}
BOOL CALLBACK DlgEx6_8Proc(HWND hDlg,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	static int selection;
	static int nana;
	static char name[500];
	switch(iMsg)
	{
		/*case WM_INITDIALOG:
		InitDlg(hDlg);
		return 1;*/
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO1); // ȸ�����
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
			GetDlgItemText (hDlg, IDC_EDIT1, name, 20); // �̸� ���ڿ� ȹ��
			if (strcmp(name, "")) // �̸��� ��� ������, �� ������ �O����
				SendMessage (hCombo, IDC_COMBO1, 0, (LPARAM)name);
			break;

		case IDC_BUTTON2:
			hList = GetDlgItem(hDlg, IDC_LIST1);
			selection = SendMessage (hList, LB_GETCURSEL,0,0);
			hList = GetDlgItem(hDlg, IDC_LIST2);
			SendMessage(hList,LB_GETTEXT,selection,nana);

			//0 ���
			//1 ����
			//2 �Ķ�
			//3 ���

			if( nana == 1){
				isBGcolor = 1;
			}else if( nana == 2){
				isBGcolor = 3;
			}else if( nana == 0){
				isBGcolor = 2;
			}else if( nana == 3){
				isBGcolor = 0;
			}

			break;

		case IDC_BUTTON_DELETE:
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			return 0;
		case IDC_LIST:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = (int)SendMessage(hList, LB_GETCURSEL,0,0);
			break;
		case IDCLOSE:
			DestroyWindow(hDlg);
			hDlg=NULL;
			return 0;
		case EXIT:
			DestroyWindow(hDlg);
			hDlg=NULL;
			return 0;
		}
		break;
	}
	return 0;
}