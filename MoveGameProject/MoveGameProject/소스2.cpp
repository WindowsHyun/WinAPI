if(susiarr[i].susi == 1)	//초밥종류 1이면
			{	
				if(isClick[i] == 1)
				{
					clicksusi[num] = susiarr[i].susi;// 0 번쨰에 저장하고
					click = susiarr[i].susi;		//가운데 그릴

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
					click = susiarr[i].susi;		//가운데 그릴

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
					click = susiarr[i].susi;		//가운데 그릴 

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
			if(isClick[i] == 1)
			{
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