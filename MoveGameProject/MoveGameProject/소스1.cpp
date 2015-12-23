#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include "resource.h"

//초밥 긴큰네모빨강 1
//            초록 2
//            파랑 3

// 초밥을 모양 설정한후 그 좌표를 배열에 집어넣은다음 
// 그 좌표들을 타이머에 따라 계속 바뀌면서 돌려야함
// 초밥 선택되면 선택된것 저장 한후 가운데 출력 그리고 선택된 초밥 색깔 바뀌게
// 그리고 선택된 초밥 옆에다 출력


//그리고 여러가지 버튼들 만들어야함.



BOOL InRectangle(int x1, int y1, int x2, int y2, int mzx, int mzy)
{
	if( (mzx >= x1 && mzx <=x2) && (mzy >= y1 && mzy <= y2) )
	{
		return 1;
	} 
	else
	{
		return 0;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage, WPARAM wParam, LPARAM IParam);
HINSTANCE g_hInst;
//BOOL InvalidateRect;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName="sky";
	WndClass.lpszMenuName=NULL;//MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	//윈도우 클래스등록
	RegisterClass(&WndClass);
	//윈도우 생성
	hWnd = CreateWindow("sky","sky",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT, 1100, 800, NULL ,(HMENU)NULL, hInstance, NULL);
	//윈도우 출력
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow (hWnd);
	//이벤트 루프 처리
	while(GetMessage(&Message,0,0,0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage, WPARAM wParam, LPARAM IParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;


	static BOOL Selection ;	//선택하면

	static BOOL startSelect ;	//선택하면
	static BOOL restartSelect ;	//선택하면
	static BOOL stopSelect ;	//선택하면
	static BOOL sumSelect ;	//선택하면
	static BOOL quitSelect ;	//선택하면


	//	static BOOL Drag;			//드래그

	static RECT susirectP[16];	//rect 구조체 배열 이걸로 판		0부터 15까지네모는 돌리기 

	static RECT ClicksusiP[16];//rect 구조체 배열 이걸로 오른쪽 판


	static int isClick[16]= {0};
	static int isToClick[16] = {0};
	static RECT startButton;	//ㅅㅣ작버튼
	static RECT restartButton;	//재시작버튼
	static RECT stopButton;		//멈춤버튼
	static RECT sumButton;		//ㄱㅖ산버튼
	static RECT quitButton;		//끝버튼
	struct susistruct		//스시 구조체
	{
		int rectnum;	//susirect [16]개의 번호들지정 

		int susi;		//초밥종류
	};

	static susistruct susiarr[16];


	int i;

	static int startX,startY;	//시작 좌표


	int clicksusi[16]= {0};	//클릭된 초밥 저장하는배열
	int click = 0;	//클릭된 초밥 가운데 나타나게 하려고
	int num = 0;

	static int mx,my;		//마우스 좌표



	srand((unsigned)time(NULL));	//rand함수 쓰려고

	// 메세지 처리하기

	switch(iMessage)
	{
	case WM_CREATE :

		for(i = 0; i<16;++i)
		{
			susiarr[i].rectnum = i;		//초기화 그림그리려고 rectnum으로 네모 정해놨으니까 그대로 그림그리려고 칸수 정해놓기
			susiarr[i].susi = rand()%3 + 1 ;	//1부터 16까지 번호 붙이고 돌릴ㄸ ㅐ 차례차례 

		}
		//////////////////////초밥 클릭한거 오른쪽에 나오기//////////////////////////////////////
		ClicksusiP[0].left = 850;
		ClicksusiP[0].top= 50;
		ClicksusiP[0].right = 900;
		ClicksusiP[0].bottom = 100;

		ClicksusiP[1].left = 850;
		ClicksusiP[1].top= 100;
		ClicksusiP[1].right = 900;
		ClicksusiP[1].bottom = 150;

		ClicksusiP[2].left = 850;
		ClicksusiP[2].top= 150;
		ClicksusiP[2].right = 900;
		ClicksusiP[2].bottom = 200;

		ClicksusiP[3].left = 850;
		ClicksusiP[3].top= 200;
		ClicksusiP[3].right = 900;
		ClicksusiP[3].bottom = 250;

		ClicksusiP[4].left = 850;
		ClicksusiP[4].top=250 ;
		ClicksusiP[4].right =900 ;
		ClicksusiP[4].bottom = 300;

		ClicksusiP[5].left = 850;
		ClicksusiP[5].top= 300;
		ClicksusiP[5].right = 900;
		ClicksusiP[5].bottom = 350;

		ClicksusiP[6].left = 850;
		ClicksusiP[6].top=350 ;
		ClicksusiP[6].right = 900;
		ClicksusiP[6].bottom =400 ;

		ClicksusiP[7].left = 850;
		ClicksusiP[7].top=400 ;
		ClicksusiP[7].right = 900;
		ClicksusiP[7].bottom = 450;

		ClicksusiP[8].left = 800;
		ClicksusiP[8].top= 50;
		ClicksusiP[8].right = 850;
		ClicksusiP[8].bottom = 100;

		ClicksusiP[9].left = 800;
		ClicksusiP[9].top= 100;
		ClicksusiP[9].right = 850;
		ClicksusiP[9].bottom = 150;

		ClicksusiP[10].left =800 ;
		ClicksusiP[10].top=150 ;
		ClicksusiP[10].right = 850;
		ClicksusiP[10].bottom =200 ;

		ClicksusiP[11].left = 800;
		ClicksusiP[11].top= 200;
		ClicksusiP[11].right =850 ;
		ClicksusiP[11].bottom = 250;

		ClicksusiP[12].left = 800;
		ClicksusiP[12].top= 250;
		ClicksusiP[12].right =850 ;
		ClicksusiP[12].bottom = 300;

		ClicksusiP[13].left =800 ;
		ClicksusiP[13].top= 300;
		ClicksusiP[13].right = 850;
		ClicksusiP[13].bottom = 350;

		ClicksusiP[14].left = 800;
		ClicksusiP[14].top= 350;
		ClicksusiP[14].right = 850;
		ClicksusiP[14].bottom = 400;

		ClicksusiP[15].left =800 ;
		ClicksusiP[15].top= 400;
		ClicksusiP[15].right = 850;
		ClicksusiP[15].bottom = 450 ;

		////////////////
	
		
		////////////////////////////////////////////////////////
		/////////////////////////////////////////////초밥 판 ㅋㅋ/////////////////////
		susirectP[0].left = 50;
		susirectP[0].top = 50;
		susirectP[0].right = 150;
		susirectP[0].bottom = 150;

		susirectP[1].left = 150;
		susirectP[1].top = 50;
		susirectP[1].right = 250;
		susirectP[1].bottom = 150;

		susirectP[2].left = 250;
		susirectP[2].top = 50;
		susirectP[2].right = 350;
		susirectP[2].bottom = 150;

		susirectP[3].left = 350;
		susirectP[3].top = 50;
		susirectP[3].right = 450;
		susirectP[3].bottom = 150;

		susirectP[4].left = 450;
		susirectP[4].top = 50;
		susirectP[4].right = 550;
		susirectP[4].bottom = 150;

		susirectP[5].left = 450;
		susirectP[5].top = 150;
		susirectP[5].right = 550;
		susirectP[5].bottom = 250;

		susirectP[6].left = 450;
		susirectP[6].top = 250;
		susirectP[6].right = 550;
		susirectP[6].bottom = 350;

		susirectP[7].left = 450;
		susirectP[7].top = 350;
		susirectP[7].right = 550;
		susirectP[7].bottom = 450;

		susirectP[8].left = 450;
		susirectP[8].top = 450;
		susirectP[8].right = 550;
		susirectP[8].bottom = 550;

		susirectP[9].left = 350;
		susirectP[9].top = 450;
		susirectP[9].right = 450;
		susirectP[9].bottom = 550;

		susirectP[10].left = 250;
		susirectP[10].top = 450;
		susirectP[10].right = 350;
		susirectP[10].bottom = 550;

		susirectP[11].left = 150;
		susirectP[11].top = 450;
		susirectP[11].right = 250;
		susirectP[11].bottom = 550;

		susirectP[12].left = 50;
		susirectP[12].top = 450;
		susirectP[12].right = 150;
		susirectP[12].bottom = 550;

		susirectP[13].left = 50;
		susirectP[13].top = 350;
		susirectP[13].right = 150;
		susirectP[13].bottom = 450;

		susirectP[14].left = 50;
		susirectP[14].top = 250;
		susirectP[14].right = 150;
		susirectP[14].bottom = 350;

		susirectP[15].left = 50;
		susirectP[15].top = 150;
		susirectP[15].right = 150;
		susirectP[15].bottom = 250;
		///////////////////////////////////////////////////////초밥판 끝
		//ㅅㅣ작버튼

		startButton.left = 700;	
		startButton.top = 700;
		startButton.right = 750;
		startButton.bottom = 750;
		////////////////////////////////////////////////////////////////
		//////////////////////재시작//////////////////////
		restartButton.left = 760;
		restartButton.top = 700;
		restartButton.right = 810;
		restartButton.bottom = 750;

		///////////////////////////////////////////////
		stopButton.left= 820;
		stopButton.top = 700;
		stopButton.right = 870;
		stopButton.bottom = 750;
		//////////////////////////////////////////////////
		sumButton.left = 880;	
		sumButton.top = 700;
		sumButton.right = 930;	
		sumButton.bottom = 750;	
		/////////////////////////////////////////////////////
		quitButton.left = 940;	
		quitButton.top = 700;	
		quitButton.right = 990;	
		quitButton.bottom = 750;	
		///////////////////////////////////////////////////////

		//초기화
		Selection = FALSE;
		break;

	case WM_LBUTTONDOWN :		
		mx = LOWORD(IParam);
		my = HIWORD(IParam);

		for(i = 0; i< 16; ++i)
		{

			if( InRectangle( susirectP[susiarr[i].rectnum].left+25,susirectP[susiarr[i].rectnum].top+25,susirectP[susiarr[i].rectnum].right-25,susirectP[susiarr[i].rectnum].bottom-25,mx,my) )
			{
				isClick[i]  = 1;
				for(int i=0; i<16; ++i){
					isToClick[i] = 0;
				}
				isToClick[i] = 1;
				click = 0;
				InvalidateRgn(hWnd,NULL,TRUE);
				break;
			}

		}

		if( InRectangle( startButton.left,startButton.top ,startButton.right,startButton.bottom,mx,my) )
		{
			startSelect = TRUE;
			SetTimer(hWnd,1,1000,NULL);


		}

		if( InRectangle( restartButton.left,restartButton.top ,restartButton.right ,restartButton.bottom ,mx,my) )
		{
			restartSelect = TRUE;

			for(int i = 0; i<16;++i)
			{
				susiarr[i].rectnum = i;		//초기화 그림그리려고 rectnum으로 네모 정해놨으니까 그대로 그림그리려고 칸수 정해놓기
				susiarr[i].susi = rand()%3 + 1 ;	//1부터 16까지 번호 붙이고 돌릴ㄸ ㅐ 차례차례 
				clicksusi[i]= 0;	//클릭된 초밥 저장하는배열
				isClick[i] = 0;

			}
			click = 0;	//클릭된 초밥 가운데 나타나게 하려
			num = 0;
		}

		if( InRectangle( stopButton.left,stopButton.top ,stopButton.right ,stopButton.bottom ,mx,my) )
		{
			stopSelect = TRUE;
			KillTimer(hWnd,1);
		}

		if(InRectangle( sumButton.left,sumButton.top ,sumButton.right ,sumButton.bottom ,mx,my) )
		{
			sumSelect = TRUE;
		}

		if(InRectangle( quitButton.left,quitButton.top ,quitButton.right ,quitButton.bottom ,mx,my) )
		{
			quitSelect = TRUE;
			KillTimer(hWnd,1);
			PostQuitMessage(0);

		}



		InvalidateRgn(hWnd,NULL,TRUE);
		break;

	case WM_LBUTTONUP :		
		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		Rectangle(hdc,250,250,350,350);	//선택한 초밥 그릴곳

		Rectangle(hdc,startButton.left,startButton.top,startButton.right,startButton.bottom);
		TextOut(hdc,startButton.left+10,startButton.top+15,"시작",4);

		Rectangle(hdc,restartButton.left,restartButton.top,restartButton.right, restartButton.bottom);
		TextOut(hdc,restartButton.left+2,restartButton.top+15,"재시작",6);

		Rectangle(hdc,stopButton.left,stopButton.top,stopButton.right, stopButton.bottom);
		TextOut(hdc,stopButton.left+10,stopButton.top+15,"멈춤",4);

		Rectangle(hdc,sumButton.left,sumButton.top,sumButton.right, sumButton.bottom);
		TextOut(hdc,sumButton.left+10,sumButton.top+15,"계산",4);

		Rectangle(hdc,quitButton.left,quitButton.top,quitButton.right, quitButton.bottom);
		TextOut(hdc,quitButton.left+12,quitButton.top+15,"끝",2);

		/*for(int i; i<16; ++i)
		{
			Rectangle(hdc,)
		}
*/
		for(i = 0; i<16 ;++i)	//초밥 네모랑 초밥 그리기
		{

			if( num == 15){
				KillTimer(hWnd,1);
			}

			if(susiarr[i].susi == 1)	//초밥종류 1이면
			{	
				if(isClick[i] == 1)
				{
					clicksusi[num] = susiarr[i].susi;// 0 번쨰에 저장하고
					for(int i=0; i<16; ++i){
						if(isToClick[i] ==1){
							click = susiarr[i].susi;							//가운데 그릴
							break;
						}
					}

					hBrush=CreateSolidBrush(RGB(0,0,0));	//검정색
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					hBrush=CreateSolidBrush(RGB(0,55,200));	//하늘색
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					num++;	//++시킨당

				}
				else
				{
					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;	

					hBrush=CreateSolidBrush(RGB(255,0,0));	//빨강색
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}

			}
			else if(susiarr[i].susi ==2)//초밥종류2면
			{
				if(isClick[i] == 1)
				{
					clicksusi[num] = susiarr[i].susi;// 0 번쨰에 저장하고
					for(int i=0; i<16; ++i){
						if(isToClick[i] ==1){
							click = susiarr[i].susi;							//가운데 그릴
							break;
						}
					}

					hBrush=CreateSolidBrush(RGB(0,0,0));	//검정색
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					hBrush=CreateSolidBrush(RGB(200,0,55));	//분홍색
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					num++;	//++시킨당



				}

				else
				{

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;

					hBrush=CreateSolidBrush(RGB(0,255,0));	//초록색
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}

			}
			else if(susiarr[i].susi ==3)//초밥 종류 3이면
			{
				if(isClick[i] == 1)
				{
					clicksusi[num] = susiarr[i].susi;// 0 번쨰에 저장하고
					for(int i=0; i<16; ++i){
						if(isToClick[i] ==1){
							click = susiarr[i].susi;							//가운데 그릴
							break;
						}
					}

					hBrush=CreateSolidBrush(RGB(0,0,0));	//검정색
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					hBrush=CreateSolidBrush(RGB(65,190,0));	//노랑색
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					num++;	//++시킨당



				}
				else
				{
					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;

					hBrush=CreateSolidBrush(RGB(0,0,255));	//파랑색
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;		

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}

			}






			/////////////////가운데 그리기///////////////////////////////
			if(click==1)
			{
				hBrush=CreateSolidBrush(RGB(255,0,0));	//빨강색
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,250,250,350,350) ;	
				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);
			}
			else if(click==2)
			{
				hBrush=CreateSolidBrush(RGB(0,255,0));	//초록색
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

				Rectangle(hdc,250,250,350,350) ;	

				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);
			}
			else if(click ==3)
			{
				hBrush=CreateSolidBrush(RGB(0,0,255));	//파랑색
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

				Rectangle(hdc,250,250,350,350) ;	

				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);
			}
		}


		for(int i=0; i<num;++i )
		{
			if(clicksusi[i]==1)
			{
				hBrush=CreateSolidBrush(RGB(255,0,0));	
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

				Rectangle(hdc,ClicksusiP[i].left,ClicksusiP[i].top,ClicksusiP[i].right,ClicksusiP[i].bottom) ;	

				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);
			}
			else if(clicksusi[i]==2)
			{
				hBrush=CreateSolidBrush(RGB(0,255,0));
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

				Rectangle(hdc,ClicksusiP[i].left,ClicksusiP[i].top,ClicksusiP[i].right,ClicksusiP[i].bottom) ;	

				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);
			}
			else if(clicksusi[i]==3)
			{
				hBrush=CreateSolidBrush(RGB(0,0,255));	//파랑색
				oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

				Rectangle(hdc,ClicksusiP[i].left,ClicksusiP[i].top,ClicksusiP[i].right,ClicksusiP[i].bottom) ;	

				SelectObject(hdc,oldBrush);
				DeleteObject(hBrush);
			}
			
		}






		EndPaint(hWnd, &ps);


		break;

	case WM_TIMER :


		for(i = 0; i<16; ++i)	//계속 돌게
		{
			if(susiarr[i].rectnum ==15)	//15면 다시 첨부터
			{
				susiarr[i].rectnum = 0;
			}
			else
				susiarr[i].rectnum ++;

		}




		InvalidateRgn(hWnd,NULL,TRUE);
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
		break;

	}	//switch case문 끝


	return DefWindowProc(hWnd,iMessage,wParam,IParam);

	//나머지는 OS로

}


