#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hlnst;
LPCTSTR lpszClass=TEXT("TextOut");



HWND hMDlg;
HWND hWndMain;
BOOL CALLBACK AboutDlgProc( HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    
    static int radio = 1;
    static int check = 0;
    static BOOL flag = TRUE;
    static int cnt;
    static RECT rect;
    
    HDC hdc;
    PAINTSTRUCT ps;
    HWND hButton;
    switch(iMessage){
    case WM_INITDIALOG:
        rect.left = 20;
        rect.top = 20;
        rect.right = 40;
        rect.bottom = 40;
        CheckRadioButton (hDlg,IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
        SetTimer(hDlg, 0, 100, NULL);
        break;
     
    case WM_TIMER:
        switch(wParam){
        case 0:
            rect.left+=20;
            rect.right+=20;

            if(rect.right >= 380){
                KillTimer(hDlg,0);
                SetTimer(hDlg,1,100,NULL);
            }

            InvalidateRect(hDlg,NULL,TRUE);
            break;
        case 1:
            
            rect.top +=20;
            rect.bottom += 20;
            KillTimer(hDlg,1);
            SetTimer(hDlg,2,100,NULL);
            InvalidateRect(hDlg,NULL,TRUE);
            break;

        case 2:
            rect.left-=20;
            rect.right-=20;

            if(rect.left <= 20){
                KillTimer(hDlg,2);
                SetTimer(hDlg,3,100,NULL);
            }

            InvalidateRect(hDlg,NULL,TRUE);
            break;
        
        case 3:
            
            rect.top +=20;
            rect.bottom += 20;
            KillTimer(hDlg,3);
            SetTimer(hDlg,0,100,NULL);
            InvalidateRect(hDlg,NULL,TRUE);
            break;
        

        case 4:
            rect.top -=5;
            rect.bottom -= 5;
            ++cnt;
            if(cnt == 4){
                KillTimer(hDlg,4);
                SetTimer(hDlg,5,50,NULL);
                cnt = 0;
            }
            InvalidateRect(hDlg,NULL,TRUE);
            break;
         case 5:
            rect.top +=5;
            rect.bottom += 5;
            ++cnt;
            if(cnt == 4){
                KillTimer(hDlg,5);
                cnt = 0;
            }
            InvalidateRect(hDlg,NULL,TRUE);
            break;
        }



        return 0;

    case WM_COMMAND:
        switch(LOWORD(wParam)){
        case IDOK: 
            SetTimer(hDlg,4,50,NULL);
            
            break;
        case IDC_BUTTON2: 
            SetTimer(hDlg, 0, 100, NULL);
            KillTimer(hDlg,2);
            break;
        case IDC_BUTTON1: 
            SetTimer(hDlg, 2, 100, NULL);
            KillTimer(hDlg,0);
            break;
        case IDC_RADIO1:
            radio = 1;
            InvalidateRect(hDlg,NULL,TRUE);
            break;
        case IDC_RADIO2:
            radio = 0;
            InvalidateRect(hDlg,NULL,TRUE);
            break;
        case IDC_CHECK1:
            if(check==1){
                check = 0;
                break;
            }
            check = 1;
            InvalidateRect(hDlg,NULL,TRUE);
            break;
        case IDCANCEL:
            DestroyWindow(hMDlg);
            hMDlg = NULL;
            break;
        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hDlg, &ps);
       
        if(radio == 1){
            Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
        }
        else if(radio == 0){
            Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        }

        if(check == 1){
            for(int i =0; i<19; ++i){
                MoveToEx(hdc, i*20,0,NULL);
                LineTo(hdc,i*20,240);
            }
            for(int i =0; i<12; ++i){
                MoveToEx(hdc, 0,i*20,NULL);
                LineTo(hdc,380,i*20);
            }
        }

        
        EndPaint(hDlg, &ps);
        return 0;
   

    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;

	MSG Message;
	WNDCLASSEX WndClass; // 구조체의 정의
	g_hlnst=hInstance;

	WndClass.cbSize=sizeof(WndClass);
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH); // CreateSolidBrush(RGB(0,0,255)); CreateHatchBrush(HS_DIAGCROSS,RGB(255,0,0));
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION); // IDI_APPLICATION, IDI_ASTERISK, IDI_ERROR, IDI_EXCLAMATION, IDI_QUESTION
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW; 
	WndClass.hIconSm=LoadIcon (NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,800,600,NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	//UpdateWindow(hWnd);

	while(GetMessage(&Message,NULL,0,0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static BOOL flag = TRUE;
    static RECT rect;
    switch(iMessage){
    case WM_CREATE:
        hWndMain = hWnd;
        rect.left = 700;
        rect.top = 50;
        rect.right = 770;
        rect.bottom = 120;
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)){
        case ID_40001:
            if(!IsWindow(hMDlg)){
                hMDlg = CreateDialog(g_hlnst, MAKEINTRESOURCE(IDD_DIALOG1),hWnd,AboutDlgProc);
                ShowWindow(hMDlg, SW_SHOW);
            }
        break;
        }

        return 0;


    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        
        EndPaint(hWnd, &ps);
        return 0;

    case WM_LBUTTONDOWN:
       
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}