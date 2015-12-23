
#include <windows.h>
#include <time.h>
//#include <stdlib.h>

#include "resource.h"

const int TILE_STATE_CLOSE		=  0;
const int TILE_STATE_QUESTION	=  1;
const int TILE_STATE_FLAG		=  2;
const int TILE_STATE_OPEN		=  3;

const int TILE_STATE_FAIL_FLAG  =  4;
const int TILE_STATE_BOMB_MINE  =  5;
const int TILE_STATE_BOMB_QUESTION = 6;

const int TILE_IMAGE_WIDTH = 20;
const int TILE_IMAGE_HEIGHT = 20;

const int NUMBER_IMAGE_WIDTH = 14;
const int NUMBER_IMAGE_HEIGHT = 20;

const int EASY_MODE  = 1;
const int NORMAL_MODE = 2;
const int HARD_MODE = 3;

const int EASY_TILE_WIDTH = 9;
const int EASY_TILE_HEIGHT = 9;

const int NORMAL_TILE_WIDTH = 16;
const int NORMAL_TILE_HEIGHT = 16;

const int HARD_TILE_WIDTH = 30;
const int HARD_TILE_HEIGHT = 16;

const int EASY_MINE_COUNT  = 10;
const int NORMAL_MINE_COUNT = 40;
const int HARD_MINE_COUNT = 99;


const int GAME_READY = 1;
const int GAME_PLAY = 2;
const int GAME_OVER = 3;

// 현재 남아 있는 지뢰의 개수와 닫혀 있는 타일의 개수를 합한게, 스테이지 지뢰의 개수와 같다면 게임 클리어이다.
//  m_nRemainMine + m_nClosedTile == m_nStageMine . 클리어

int	  m_nRemainMine;   //현재 남아있는 지뢰의 개수.
int   m_nStageMine;	   //스테이지에 묻힌 지뢰의 개수
int	  m_nClosedTile;   //현재 닫혀 있는 타일의 개수.  


int	  m_tileAdjustMineCount[HARD_TILE_WIDTH * HARD_TILE_HEIGHT];
int	  m_tileMineState[HARD_TILE_WIDTH * HARD_TILE_HEIGHT];
bool  m_bMineExist[HARD_TILE_WIDTH * HARD_TILE_HEIGHT];

int	  m_iGameMode;
int   m_iGameState;

int   m_iCurrTileWidth;
int	  m_iCurrTileHeight;

float   m_fPlayingTimePerSec;

HINSTANCE g_hInst;
HWND hWnd , hStartButton;

int m_iBackgroundLeft;
int m_iBackgroundRight;
int m_iBackgroundTop;
int m_iBackgroundBottom;

bool m_bPressedLButton;	//현재 마우스 LButton이 눌렸는지 확인 하는 변수 
bool m_bPressedRButton; //현재 마우스 RButton이 눌렸는지 확인 하는 변수 

HBITMAP MemBit;	// 가상 메모리 비트맵 . 그릴 화면을 여기에서 그리고 난뒤에 WM_PAINT에서 이 메모리 비트맵을 그려낸다.

// 게임과 관련된 상태를 결정하는 함수들
void SetBackgroundSize( LPRECT ClientRect );
void SetGameState( int GameMode , LPRECT ClientRect );
void GenerateMine( int tileIndex );

//결과 값들을 비교해,게임의 결과물을 확인하는데 필요한 함수들
void CheckTileState( int indexRow, int indexColumn );
void CheckOpenTiles( int indexRow, int indexColumn );
void CheckMines( );
void ClearTiles( );

int GetAdjustMineCount( int indexRow , int indexColumn );
int GetAdjustFlagCount( int indexRow , int indexColumn );


//화면을 그려내는데 필요한 함수들
void DrawBitmap( HDC hdc, int x, int y , HBITMAP hBit );
void DrawNumber( HDC hdc, int x , int y , int drawNumber );
void PrintMineFinder( );



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int ShowCmd)
{
	MSG msg;

	LPSTR lpszClass = "MYButton";

	WNDCLASS wc;

	g_hInst = hInstance;

	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"WINDOW";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClass(&wc);

	hWnd = CreateWindow(L"WINDOW", L"WINDOW", WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, ShowCmd);

	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memDC;
	PAINTSTRUCT ps;
	int mouseX, mouseY , index , indexRow, indexColumm , leftX, TopY ;
	int nMatchMineCount,  buttonWidth, buttonHeight;
	RECT tempRect;
	
	HBITMAP hOldBmp;

//	HBITMAP tileFlag, tileClose, tileMine0 , tileMine1, tileMine2, tileMine3, tileMine4;
//	HBITMAP	tileMine5, tileMine6, tileMine7, tileMine8 , tileQuestion, tileMine;


	
	switch(message)
	{
	case WM_CREATE:
		
				
		GetClientRect( hWnd, &tempRect);

		SetGameState( HARD_MODE , &tempRect);

		buttonWidth = 100;
		buttonHeight = 30;
		leftX = tempRect.right / 2; // Center Position X
		TopY = 20;	// Center Position Y 	

		leftX -= ( buttonWidth / 2 );

		srand((int)time(NULL));

		hStartButton = CreateWindow( L"button" , L"Start" , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , leftX , TopY , buttonWidth , buttonHeight , hWnd , (HMENU)ID_START , g_hInst,NULL);

		m_bPressedLButton = false;
		m_bPressedRButton = false;

		SetTimer(hWnd,1, 500,NULL);

		PrintMineFinder();

		break;

	case WM_TIMER:

		if( m_iGameState == GAME_PLAY )
		{
			m_fPlayingTimePerSec += 0.5f;			

			if( m_fPlayingTimePerSec > 999.0f )
				m_fPlayingTimePerSec = 999.0f;

			PrintMineFinder();

			InvalidateRect(hWnd,NULL, FALSE);
		}
		break;

	case WM_COMMAND:
		
		switch( LOWORD(wParam) )
		{
		case ID_START:

			GetClientRect( hWnd, &tempRect);			
			SetGameState( m_iGameMode, &tempRect);
	
			PrintMineFinder();	

			InvalidateRect(hWnd,NULL, FALSE);
			break;
		case ID_GAME_NEWGAME:

			GetClientRect( hWnd, &tempRect);			
			SetGameState( m_iGameMode, &tempRect);

			PrintMineFinder();
			InvalidateRect(hWnd,NULL, FALSE);

			break;

		case ID_MODE_EASY:
	
			m_iGameMode = EASY_MODE;
			GetClientRect( hWnd, &tempRect);			
			SetGameState( m_iGameMode, &tempRect);

			PrintMineFinder();
			InvalidateRect(hWnd,NULL, TRUE);

			break;
		case ID_MODE_MIDDLE:

			m_iGameMode = NORMAL_MODE;
			GetClientRect( hWnd, &tempRect);			
			SetGameState( m_iGameMode, &tempRect);

			PrintMineFinder();
			InvalidateRect(hWnd,NULL, TRUE);

			break;
		case ID_MODE_HARD:

			m_iGameMode = HARD_MODE;
			GetClientRect( hWnd, &tempRect);
			SetGameState( m_iGameMode, &tempRect);

			PrintMineFinder();
			InvalidateRect(hWnd,NULL, TRUE);

			break;

		case ID_GAME_EXIT:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &tempRect);
		SetBackgroundSize( &tempRect );

		buttonWidth = 100;
		buttonHeight = 30;

		leftX = tempRect.right / 2; // Center Position X
		TopY = 20;	// Center Position Y 	

		leftX -= ( buttonWidth / 2 );

		MoveWindow( hStartButton , leftX , TopY , buttonWidth , buttonHeight , TRUE);

		PrintMineFinder( );
		InvalidateRect(hWnd,NULL, TRUE);

		break;

	case WM_PAINT:

		hdc = BeginPaint( hWnd, &ps);
		memDC = CreateCompatibleDC(hdc);	

		hOldBmp = (HBITMAP) SelectObject( memDC , MemBit);
		BitBlt( hdc , 0 , 0 , m_iBackgroundRight ,m_iBackgroundBottom, memDC ,0 ,0 , SRCCOPY);
		SelectObject( memDC , hOldBmp);
		DeleteDC( memDC );
		EndPaint( hWnd, &ps);
		return 0;		

	case WM_LBUTTONDOWN:
		
		m_bPressedLButton = true;

		if( m_bPressedRButton )
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);

			if( mouseX > m_iBackgroundLeft && mouseX < m_iBackgroundRight && mouseY > m_iBackgroundTop && mouseY < m_iBackgroundBottom )
			{
				indexColumm = ( mouseX - m_iBackgroundLeft ) / TILE_IMAGE_WIDTH;
				indexRow = ( mouseY - m_iBackgroundTop ) / TILE_IMAGE_HEIGHT;

				index = indexRow * m_iCurrTileWidth + indexColumm ;


				if( m_iGameState == GAME_PLAY )
				{				
					if( m_tileMineState[index] == TILE_STATE_OPEN )
					{						
						CheckOpenTiles( indexRow , indexColumm );
					}
				}

				PrintMineFinder();
				InvalidateRect(hWnd,NULL, FALSE);
			}
		}
		break;
	case WM_RBUTTONDOWN:
		m_bPressedRButton = true;

		if( m_bPressedLButton )
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);

			if( mouseX > m_iBackgroundLeft && mouseX < m_iBackgroundRight && mouseY > m_iBackgroundTop && mouseY < m_iBackgroundBottom )
			{
				indexColumm = ( mouseX - m_iBackgroundLeft ) / TILE_IMAGE_WIDTH;
				indexRow = ( mouseY - m_iBackgroundTop ) / TILE_IMAGE_HEIGHT;

				index = indexRow * m_iCurrTileWidth + indexColumm ;


				if( m_iGameState == GAME_PLAY )
				{				
					if( m_tileMineState[index] == TILE_STATE_OPEN )
					{						
						CheckOpenTiles( indexRow , indexColumm );
					}
				}

			
				PrintMineFinder();
				InvalidateRect(hWnd,NULL, FALSE);
			}
		}
		break;
				

	case WM_LBUTTONUP:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);

		if( mouseX > m_iBackgroundLeft && mouseX < m_iBackgroundRight && mouseY > m_iBackgroundTop && mouseY < m_iBackgroundBottom )
		{
			indexColumm = ( mouseX - m_iBackgroundLeft ) / TILE_IMAGE_WIDTH;
			indexRow = ( mouseY - m_iBackgroundTop ) / TILE_IMAGE_HEIGHT;

			index = indexRow * m_iCurrTileWidth + indexColumm ;
			

			switch( m_iGameState )
			{
			case GAME_READY:
				GenerateMine( index );

				CheckTileState( indexRow , indexColumm);

				m_iGameState = GAME_PLAY;
				//Timer 초기화 및 모드를 Play로 바꾼다.
				break;
			case GAME_PLAY:
				if( m_tileMineState[index] == TILE_STATE_CLOSE )
				{
					if( m_bMineExist[index] == true )
					{
						m_tileMineState[index] = TILE_STATE_BOMB_MINE;
						m_iGameState = GAME_OVER;
						MessageBox( hWnd, L"Game Over" , L"Game Over",MB_OK );

						ClearTiles( );
						
					}
					else 
					{
						CheckTileState( indexRow , indexColumm);
					}

				}

				/*if( m_nClosedTile + m_nRemainMine == m_nStageMine )
				{
					CheckMines();
				}
				*/
				if( m_nClosedTile == m_nRemainMine )
					CheckMines();

				break;				
			}
		}

		//임시로 마우스를 누르면 업데이트

		m_bPressedLButton = false;

		PrintMineFinder();
		InvalidateRect(hWnd,NULL, FALSE);

		break;

	case WM_RBUTTONUP:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);

		nMatchMineCount = 0;

		if( mouseX > m_iBackgroundLeft && mouseX < m_iBackgroundRight && mouseY > m_iBackgroundTop && mouseY < m_iBackgroundBottom )
		{
			indexColumm = ( mouseX - m_iBackgroundLeft ) / TILE_IMAGE_WIDTH;
			indexRow = ( mouseY - m_iBackgroundTop ) / TILE_IMAGE_HEIGHT;

			index = indexRow * m_iCurrTileWidth + indexColumm ;


			if( m_iGameState == GAME_PLAY )
			{
				if( m_tileMineState[index] == TILE_STATE_CLOSE )
				{
					m_tileMineState[index] = TILE_STATE_FLAG;
					m_nClosedTile--;
					m_nRemainMine--;
				}
				else if( m_tileMineState[index] == TILE_STATE_FLAG )
				{
					m_nRemainMine++;
					m_nClosedTile++;
					
					m_tileMineState[index] = TILE_STATE_QUESTION;					
				}
				else if( m_tileMineState[index] == TILE_STATE_QUESTION )
				{
					m_tileMineState[index] = TILE_STATE_CLOSE;
					
				}
			}			
		}

		m_bPressedLButton = false;

		PrintMineFinder();
		InvalidateRect(hWnd,NULL, FALSE);

		break;
			
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void SetGameState( int GameMode , LPRECT ClientRect)
{	
	int nAllTileCount = HARD_TILE_WIDTH * HARD_TILE_HEIGHT;

	m_iGameMode = GameMode;
	m_iGameState = GAME_READY;
	m_fPlayingTimePerSec = 0.0f;
	SetBackgroundSize( ClientRect );

	for( int i = 0 ; i < nAllTileCount; i++)
	{
		m_tileAdjustMineCount[i] = 0;
		m_tileMineState[i] = TILE_STATE_CLOSE;
		m_bMineExist[i] = false;
	}

}

void DrawBitmap( HDC hdc, int x, int y , HBITMAP hBit )
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int imageHeight, imageWidth;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject( MemDC , hBit);
	
	GetObject(hBit , sizeof(BITMAP), &bit);
	imageWidth = bit.bmWidth;
	imageHeight = bit.bmHeight;

	BitBlt(hdc, x, y,  imageWidth, imageHeight ,MemDC, 0,0,SRCCOPY );
	SelectObject(MemDC,OldBitmap);
	DeleteObject( OldBitmap);
	DeleteDC(MemDC);


}

void GenerateMine( int tileIndex )
{
	int nGenratedMine = 0;
	int tempIndex , tempMineCount , nAllTileCount;
	int indexRow , indexColumn;


	switch( m_iGameMode )
	{
	case EASY_MODE:
		nAllTileCount = EASY_TILE_WIDTH * EASY_TILE_HEIGHT;		
		break;
	case NORMAL_MODE:
		nAllTileCount = NORMAL_TILE_WIDTH * NORMAL_TILE_HEIGHT;		
		break;
	case HARD_MODE:
		nAllTileCount = HARD_TILE_WIDTH * HARD_TILE_HEIGHT;		
		break;
	}

	m_nClosedTile = nAllTileCount;

	while( nGenratedMine != m_nRemainMine )
	{
		tempIndex = rand() % nAllTileCount;

		if( tempIndex != tileIndex && !m_bMineExist[ tempIndex ] )
		{
			m_bMineExist[ tempIndex ] = true;
			nGenratedMine++;
		}
	}

	for( int i = 0 ; i < nAllTileCount ; i++ )
	{
		indexRow = i / m_iCurrTileWidth;
		indexColumn = i % m_iCurrTileWidth;

		tempMineCount = GetAdjustMineCount( indexRow , indexColumn );
	
		m_tileAdjustMineCount[i] = tempMineCount;	
		
	}

}

/*
 * 이전 Tile의 상태가 Close일 경우, Open을 했을 시에 실행 되는 함수. 
 * 때문에, 해당 위치의 Mine 수를 가지고 판단만 하면 된다.
 */
void CheckTileState( int indexRow, int indexColumn )
{
	int nAllTileCount , iPrevTileState;
	int  index;

	switch( m_iGameMode )
	{
	case EASY_MODE:		
		nAllTileCount = EASY_TILE_WIDTH * EASY_TILE_HEIGHT;
		break;
	case NORMAL_MODE:		
		nAllTileCount = NORMAL_TILE_WIDTH * NORMAL_TILE_HEIGHT;
		break;
	case HARD_MODE:		
		nAllTileCount = HARD_TILE_WIDTH * HARD_TILE_HEIGHT;
		break;
	}

	index = ( indexRow * m_iCurrTileWidth ) + indexColumn;

	iPrevTileState = m_tileMineState[index];

	if( !( iPrevTileState == TILE_STATE_CLOSE || iPrevTileState == TILE_STATE_QUESTION ))
		return;

	if( indexRow < 0 || indexRow >= m_iCurrTileHeight || indexColumn < 0 || indexColumn >= m_iCurrTileWidth )
	{
		return;
	}	

	m_tileMineState[index] = TILE_STATE_OPEN;
	m_nClosedTile--;

	if( m_bMineExist[index] )
	{
		m_iGameState = GAME_OVER;

		if( iPrevTileState == TILE_STATE_QUESTION )
		{
			m_tileMineState[index] = TILE_STATE_BOMB_QUESTION;
		}
		else
		{
			m_tileMineState[index] = TILE_STATE_BOMB_MINE;
		}
		

		ClearTiles( );

		MessageBox( hWnd, L"Mission Failed" , L"Game Over",MB_OK );

		return;
	}


	if( GetAdjustMineCount( indexRow, indexColumn ) > 0 )
		return;	

	CheckTileState( indexRow - 1 , indexColumn - 1 );
	CheckTileState( indexRow - 1 , indexColumn );
	CheckTileState( indexRow - 1 , indexColumn + 1 );
	CheckTileState( indexRow , indexColumn - 1 );
	CheckTileState( indexRow , indexColumn + 1 );
	CheckTileState( indexRow + 1 , indexColumn - 1 );
	CheckTileState( indexRow + 1 , indexColumn  );
	CheckTileState( indexRow + 1 , indexColumn + 1 );
}

void CheckOpenTiles( int indexRow, int indexColumn )
{
	int nAllTileCount;
	int  index, iCurrTileState;

	switch( m_iGameMode )
	{
	case EASY_MODE:		
		nAllTileCount = EASY_TILE_WIDTH * EASY_TILE_HEIGHT;
		break;
	case NORMAL_MODE:		
		nAllTileCount = NORMAL_TILE_WIDTH * NORMAL_TILE_HEIGHT;
		break;
	case HARD_MODE:		
		nAllTileCount = HARD_TILE_WIDTH * HARD_TILE_HEIGHT;
		break;
	}

	index = ( indexRow * m_iCurrTileWidth ) + indexColumn;

	if( m_tileMineState[index] != TILE_STATE_OPEN )
		return;

	if( indexRow < 0 || indexRow >= m_iCurrTileHeight || indexColumn < 0 || indexColumn >= m_iCurrTileWidth )
	{
		return;
	}

	if( GetAdjustFlagCount( indexRow, indexColumn ) != m_tileAdjustMineCount[index] )
		return;	

	//if( GetAdjustFlagCount( indexRow, indexColumn ) == 0 )
	//	return;	


	// leftTop
	if( indexColumn - 1 >= 0 && indexRow - 1 >= 0 )
	{
		iCurrTileState = m_tileMineState[ index - m_iCurrTileWidth - 1];

		if( iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION )
			CheckTileState( indexRow - 1 , indexColumn - 1 );
	}
	//middleTop
	if( indexColumn >= 0 && indexRow - 1 >= 0 )
	{
		iCurrTileState = m_tileMineState[ index - m_iCurrTileWidth ];

		if( iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION  )
			CheckTileState( indexRow - 1, indexColumn );
	}
	//rightTop
	if( indexColumn + 1 < m_iCurrTileWidth && indexRow - 1 >= 0 )
	{
		iCurrTileState = m_tileMineState[ index - m_iCurrTileWidth + 1];

		if( iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION )
			CheckTileState( indexRow - 1, indexColumn + 1 );
	}

	// leftMiddle	
	if( indexColumn - 1 >= 0 && indexRow >= 0 )
	{
		iCurrTileState = m_tileMineState[ index - 1];

		if( iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION )
			CheckTileState( indexRow, indexColumn - 1 );
	}

	// rightMiddle	
	if( indexColumn + 1 < m_iCurrTileWidth && indexRow >= 0 )
	{
		iCurrTileState = m_tileMineState[ index + 1]; 

		if( iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION )
			CheckTileState( indexRow, indexColumn + 1 );
	}
	// leftBottom	
	if( indexColumn - 1 >= 0 && indexRow + 1 < m_iCurrTileHeight )
	{
		iCurrTileState = m_tileMineState[ index + m_iCurrTileWidth - 1];

		if( iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION  )
			CheckTileState( indexRow + 1, indexColumn - 1 );
	}
	// MiddleBottom	
	if( indexColumn >= 0 && indexRow + 1 < m_iCurrTileHeight )
	{
		iCurrTileState = m_tileMineState[ index + m_iCurrTileWidth ];

		if( iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION  )
			CheckTileState( indexRow + 1, indexColumn );
	}
	// rightBottom
	if( indexColumn + 1 < m_iCurrTileWidth && indexRow  + 1 < m_iCurrTileHeight )
	{
		iCurrTileState = m_tileMineState[ index + m_iCurrTileWidth + 1];

		if( iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION )
			CheckTileState( indexRow + 1, indexColumn + 1 );
	}	
}


int GetAdjustMineCount( int indexRow , int indexColumn )
{
	int MineCount = 0;
	int index = ( indexRow * m_iCurrTileWidth ) + indexColumn;


	//leftTop
	if( indexColumn - 1 >= 0 && indexRow - 1 >= 0 )
	{
		if( m_bMineExist[ index - m_iCurrTileWidth - 1] )
			MineCount++;
	}
	//middleTop
	if( indexColumn >= 0 && indexRow - 1 >= 0 )
	{
		if( m_bMineExist[ index - m_iCurrTileWidth ] )
			MineCount++;
	}
	//rightTop
	if( indexColumn + 1 < m_iCurrTileWidth && indexRow - 1 >= 0 )
	{
		if( m_bMineExist[ index - m_iCurrTileWidth + 1] )
			MineCount++;
	}

	// leftMiddle	
	if( indexColumn - 1 >= 0 && indexRow >= 0 )
	{
		if( m_bMineExist[ index - 1] )
			MineCount++;
	}

	// rightMiddle	
	if( indexColumn + 1 < m_iCurrTileWidth && indexRow >= 0 )
	{
		if( m_bMineExist[ index + 1] )
			MineCount++;
	}
	// leftBottom	
	if( indexColumn - 1 >= 0 && indexRow + 1 < m_iCurrTileHeight )
	{
		if( m_bMineExist[ index + m_iCurrTileWidth - 1] )
			MineCount++;;
	}
	// MiddleBottom	
	if( indexColumn >= 0 && indexRow + 1 < m_iCurrTileHeight )
	{
		if( m_bMineExist[ index + m_iCurrTileWidth ] )
			MineCount++;
	}
	// rightBottom
	if( indexColumn + 1 < m_iCurrTileWidth && indexRow  + 1 < m_iCurrTileHeight )
	{
		if( m_bMineExist[ index + m_iCurrTileWidth + 1] )
			MineCount++;
	}

	return MineCount;
}

int GetAdjustFlagCount( int indexRow , int indexColumn )
{
	int FlagCount = 0;
	int index = ( indexRow * m_iCurrTileWidth ) + indexColumn;

	//leftTop
	if( indexColumn - 1 >= 0 && indexRow - 1 >= 0 )
	{
		if( m_tileMineState[ index - m_iCurrTileWidth - 1] == TILE_STATE_FLAG )
			FlagCount++;
	}
	//middleTop
	if( indexColumn >= 0 && indexRow - 1 >= 0 )
	{
		if( m_tileMineState[ index - m_iCurrTileWidth ] == TILE_STATE_FLAG  )
			FlagCount++;
	}
	//rightTop
	if( indexColumn + 1 < m_iCurrTileWidth && indexRow - 1 >= 0 )
	{
		if( m_tileMineState[ index - m_iCurrTileWidth + 1] == TILE_STATE_FLAG  )
			FlagCount++;
	}

	// leftMiddle	
	if( indexColumn - 1 >= 0 && indexRow >= 0 )
	{
		if( m_tileMineState[ index - 1] == TILE_STATE_FLAG )
			FlagCount++;
	}

	// rightMiddle	
	if( indexColumn + 1 < m_iCurrTileWidth && indexRow >= 0 )
	{
		if( m_tileMineState[ index + 1] == TILE_STATE_FLAG )
			FlagCount++;
	}
	// leftBottom	
	if( indexColumn - 1 >= 0 && indexRow + 1 < m_iCurrTileHeight )
	{
		if( m_tileMineState[ index + m_iCurrTileWidth - 1] == TILE_STATE_FLAG  )
			FlagCount++;;
	}
	// MiddleBottom	
	if( indexColumn >= 0 && indexRow + 1 < m_iCurrTileHeight )
	{
		if( m_tileMineState[ index + m_iCurrTileWidth ] == TILE_STATE_FLAG  )
			FlagCount++;
	}
	// rightBottom
	if( indexColumn + 1 < m_iCurrTileWidth && indexRow  + 1 < m_iCurrTileHeight )
	{
		if( m_tileMineState[ index + m_iCurrTileWidth + 1] == TILE_STATE_FLAG )
			FlagCount++;
	}

	return FlagCount;
}


void DrawNumber( HDC hdc, int x , int y , int drawNumber )
{
	HBITMAP numBitmap;

	switch( drawNumber )
	{
	case 0:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER0 ));
		break;
	case 1:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER1 ));
		break;
	case 2:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER2 ));
		break;
	case 3:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER3 ));
		break;
	case 4:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER4 ));
		break;
	case 5:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER5 ));
		break;
	case 6:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER6 ));
		break;
	case 7:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER7 ));
		break;
	case 8:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER8 ));
		break;
	case 9:
		numBitmap = LoadBitmap( g_hInst, MAKEINTRESOURCE(IDB_NUMBER9 ));
		break;

	}

	DrawBitmap( hdc,x,y, numBitmap );

	DeleteObject( numBitmap );

}


void SetBackgroundSize( LPRECT ClientRect )
{
	int nImageWidthGap, nImageHeightGap;

	switch( m_iGameMode )
	{
	case EASY_MODE:
		nImageWidthGap = ( ( ClientRect->right - ClientRect->left ) - ( TILE_IMAGE_WIDTH * EASY_TILE_WIDTH ) ) / 2;
		nImageHeightGap = ( ( ClientRect->bottom - ClientRect->top ) - ( TILE_IMAGE_HEIGHT * EASY_TILE_HEIGHT ) ) / 2;

		m_nRemainMine = EASY_MINE_COUNT;
		
		m_iCurrTileWidth = EASY_TILE_WIDTH;
		m_iCurrTileHeight = EASY_TILE_HEIGHT;
		
		break;

	case NORMAL_MODE:
		nImageWidthGap = ( ( ClientRect->right - ClientRect->left ) - ( TILE_IMAGE_WIDTH * NORMAL_TILE_WIDTH ) ) / 2;
		nImageHeightGap = ( ( ClientRect->bottom - ClientRect->top ) - ( TILE_IMAGE_HEIGHT * NORMAL_TILE_HEIGHT ) ) / 2;

		m_nRemainMine = NORMAL_MINE_COUNT;

		m_iCurrTileWidth = NORMAL_TILE_WIDTH;
		m_iCurrTileHeight = NORMAL_TILE_HEIGHT;
		break;
	case HARD_MODE:
		nImageWidthGap = ( ( ClientRect->right - ClientRect->left ) - ( TILE_IMAGE_WIDTH * HARD_TILE_WIDTH ) ) / 2;
		nImageHeightGap = ( ( ClientRect->bottom - ClientRect->top ) - ( TILE_IMAGE_HEIGHT * HARD_TILE_HEIGHT ) ) / 2;

		m_nRemainMine = HARD_MINE_COUNT;

		m_iCurrTileWidth = HARD_TILE_WIDTH;
		m_iCurrTileHeight = HARD_TILE_HEIGHT;
		break;
	}

	m_nStageMine  = m_nRemainMine;

	m_iBackgroundLeft = nImageWidthGap + ClientRect->left;
	m_iBackgroundTop = nImageHeightGap + ClientRect->top;
	m_iBackgroundRight = ClientRect->right - nImageWidthGap;
	m_iBackgroundBottom = ClientRect->bottom - nImageHeightGap;	

}

void PrintMineFinder( )
{	

	HDC hdc, memDC;
	
	HBITMAP tileFlag, tileClose, tileMine0 , tileMine1, tileMine2, tileMine3, tileMine4, tileMine5, tileMine6;
	HBITMAP	tileMine7, tileMine8 , tileQuestion, tileMine, failFlag, bombMine, bombQuestion, hOldBmp , numMinus;

	int index , indexRow, indexColumm, leftX, TopY, nAllTileCount ;

	

	if( MemBit )
		DeleteObject(MemBit); //이전에 그린 화면을 지워낸다.

	hdc = GetDC(hWnd);

	MemBit = CreateCompatibleBitmap( hdc, m_iBackgroundRight, m_iBackgroundBottom );
	memDC  = CreateCompatibleDC( hdc );
	hOldBmp = (HBITMAP)SelectObject( memDC, MemBit);

	PatBlt( memDC , 0 , 0 , m_iBackgroundRight , m_iBackgroundBottom , PATCOPY);

	//hdc = BeginPaint( hWnd, &ps);	

	tileFlag  = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_FLAG));
	tileClose = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_CLOSE));
	tileMine0 = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE0));
	tileMine1 = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE1));
	tileMine2 = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE2));
	tileMine3 = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE3));
	tileMine4 = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE4));
	tileMine5 = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE5));
	tileMine6 = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE6));
	tileMine7 = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE7));
	tileMine8 = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE8));
	tileQuestion = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_QUESTION));
	tileMine = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINE));
	failFlag = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_FAIL_FLAG));
	bombMine = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_BOMB_MINE));
	bombQuestion = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_BOMB_QUESTION));
	numMinus = LoadBitmap(g_hInst , MAKEINTRESOURCE(IDB_MINUS));

	switch( m_iGameMode)
	{
	case EASY_MODE:
		nAllTileCount = EASY_TILE_HEIGHT * EASY_TILE_WIDTH;

		break;
	case NORMAL_MODE:
		nAllTileCount = NORMAL_TILE_HEIGHT * NORMAL_TILE_WIDTH;			

		break;
	case HARD_MODE:
		nAllTileCount = HARD_TILE_HEIGHT * HARD_TILE_WIDTH;

		break;
	}

	index = ((int)m_fPlayingTimePerSec) % 10;

	leftX = m_iBackgroundLeft + ( NUMBER_IMAGE_WIDTH * 3 );
	TopY = m_iBackgroundTop - ( NUMBER_IMAGE_HEIGHT * 2 ) ;
	DrawNumber( memDC , leftX , TopY, index );

	index = ((int)m_fPlayingTimePerSec) / 10;

	if( index == 26 )
	{
		index = index;
	}

	if( index >= 10 )
	{
		index %= 10;
	}

	leftX -= NUMBER_IMAGE_WIDTH;		
	DrawNumber( memDC , leftX , TopY, index );

	index = ((int)m_fPlayingTimePerSec) / 100;

	leftX -= NUMBER_IMAGE_WIDTH;		
	DrawNumber( memDC , leftX , TopY, index );

	index = abs(m_nRemainMine) % 10;

	leftX = m_iBackgroundRight - ( NUMBER_IMAGE_WIDTH * 2 );
	TopY = m_iBackgroundTop - ( NUMBER_IMAGE_HEIGHT * 2 ) ;

	DrawNumber( memDC , leftX , TopY, index );

	index = abs(m_nRemainMine) / 10;

	leftX -= NUMBER_IMAGE_WIDTH;	

	DrawNumber( memDC , leftX , TopY, index );

	if( m_nRemainMine < 0 )
	{
		leftX -= NUMBER_IMAGE_WIDTH;	
		DrawBitmap(memDC, leftX , TopY, numMinus );

	}
	

	// 지나온 시간과 Mine 을 그려줘야한다.

	for ( int i = 0 ; i < nAllTileCount; i++ )
	{
		indexRow = i / m_iCurrTileWidth;
		indexColumm = i % m_iCurrTileWidth;

		leftX = m_iBackgroundLeft + (indexColumm * TILE_IMAGE_WIDTH );
		TopY = m_iBackgroundTop + ( indexRow * TILE_IMAGE_HEIGHT );

		switch( m_tileMineState[i] )
		{
		case TILE_STATE_CLOSE:
			DrawBitmap(memDC, leftX , TopY, tileClose);
			break;
		case TILE_STATE_QUESTION:
			DrawBitmap(memDC, leftX , TopY, tileQuestion);
			break;
		case TILE_STATE_FLAG:
			DrawBitmap(memDC, leftX , TopY, tileFlag);
			break;
		case TILE_STATE_BOMB_MINE:
			DrawBitmap(memDC, leftX , TopY, bombMine);
			break;
		case TILE_STATE_BOMB_QUESTION:
			DrawBitmap(memDC, leftX , TopY, bombQuestion);
			break;
		case TILE_STATE_FAIL_FLAG:
			DrawBitmap(memDC, leftX , TopY, failFlag);
			break;		
		case TILE_STATE_OPEN:
			switch( m_tileAdjustMineCount[i] )
			{
			case 0:
				DrawBitmap(memDC, leftX , TopY, tileMine0);
				break;
			case 1:
				DrawBitmap(memDC, leftX , TopY, tileMine1);
				break;
			case 2:
				DrawBitmap(memDC, leftX , TopY, tileMine2);
				break;
			case 3:
				DrawBitmap(memDC, leftX , TopY, tileMine3);
				break;
			case 4:
				DrawBitmap(memDC, leftX , TopY, tileMine4);
				break;
			case 5:
				DrawBitmap(memDC, leftX , TopY, tileMine5);
				break;
			case 6:
				DrawBitmap(memDC, leftX , TopY, tileMine6);
				break;
			case 7:
				DrawBitmap(memDC, leftX , TopY, tileMine7);
				break;
			case 8:
				DrawBitmap(memDC, leftX , TopY, tileMine8);
				break;
			}

			if( m_bMineExist[i] )
				DrawBitmap(memDC, leftX , TopY, tileMine);

			break;
		}
	}

	DeleteObject( tileFlag );
	DeleteObject( tileClose );
	DeleteObject( tileMine0 );
	DeleteObject( tileMine1 );
	DeleteObject( tileMine2 );
	DeleteObject( tileMine3 );
	DeleteObject( tileMine4 );
	DeleteObject( tileMine5 );
	DeleteObject( tileMine6 );
	DeleteObject( tileMine7 );
	DeleteObject( tileMine8 );
	DeleteObject( tileQuestion );
	DeleteObject( tileMine );		
	
	SelectObject( memDC, hOldBmp );
	DeleteObject(memDC);
	ReleaseDC( hWnd, hdc);

}


// 열리지 않거나, 잘못 설정된 타일들을 보여준다.
void ClearTiles( )
{
	int nAllTileCount = m_iCurrTileWidth * m_iCurrTileHeight;

	for( int i = 0 ; i< nAllTileCount ; i++)
	{
		if( m_tileMineState[i] == TILE_STATE_CLOSE )
			m_tileMineState[i] = TILE_STATE_OPEN;
		else if( m_tileMineState[i] == TILE_STATE_FLAG && !m_bMineExist[i])
		{			
			m_tileMineState[i] = TILE_STATE_FAIL_FLAG;			
		}

	}

}

//현재 남아 있는 폭탄의 수와 close된 타일이 같다면 클리어 조건이 완성 된 것.
void CheckMines()
{
	int nAllTileCount = m_iCurrTileWidth * m_iCurrTileHeight;

	for( int i = 0 ; i< nAllTileCount ; i++)
	{
		if( m_tileMineState[i] == TILE_STATE_CLOSE )
		{
			m_tileMineState[i] = TILE_STATE_FLAG;
			m_nRemainMine--;
		}
	}

	for( int i = 0; i < nAllTileCount; i++)
	{
		if( m_bMineExist[i] )
		{
			if( m_tileMineState[i] != TILE_STATE_FLAG )
			{
				MessageBox( hWnd, L"Mission Failed" , L"Game Over",MB_OK );

				m_tileMineState[i] = TILE_STATE_BOMB_MINE;

				m_iGameState = GAME_OVER;
				ClearTiles();

				return;
			}
			
		}
	}

	MessageBox( hWnd, L"congratulation" , L"Victory",MB_OK );

	m_iGameState = GAME_OVER;

	ClearTiles();

}