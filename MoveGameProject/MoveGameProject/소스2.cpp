if(susiarr[i].susi == 1)	//�ʹ����� 1�̸�
			{	
				if(isClick[i] == 1)
				{
					clicksusi[num] = susiarr[i].susi;// 0 ������ �����ϰ�
					click = susiarr[i].susi;		//��� �׸�

					hBrush=CreateSolidBrush(RGB(0,0,0));	//������
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					hBrush=CreateSolidBrush(RGB(0,55,200));	//�ϴû�
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					num++;	//++��Ų��

					

				}
				else
				{
					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;	

					hBrush=CreateSolidBrush(RGB(255,0,0));	//������
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}

			}
			else if(susiarr[i].susi ==2)//�ʹ�����2��
			{
				if(isClick[i] == 1)
				{
					clicksusi[num] = susiarr[i].susi;// 0 ������ �����ϰ�
					click = susiarr[i].susi;		//��� �׸�

					hBrush=CreateSolidBrush(RGB(0,0,0));	//������
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					hBrush=CreateSolidBrush(RGB(200,0,55));	//��ȫ��
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					num++;	//++��Ų��

				

				}

				else
				{

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;

					hBrush=CreateSolidBrush(RGB(0,255,0));	//�ʷϻ�
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}

			}
			else if(susiarr[i].susi ==3)//�ʹ� ���� 3�̸�
			{
				if(isClick[i] == 1)
				{
					clicksusi[num] = susiarr[i].susi;// 0 ������ �����ϰ�
					click = susiarr[i].susi;		//��� �׸� 

					hBrush=CreateSolidBrush(RGB(0,0,0));	//������
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					hBrush=CreateSolidBrush(RGB(65,190,0));	//�����
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);

					num++;	//++��Ų��

				

				}
				else
				{
					Rectangle(hdc, susirectP[susiarr[i].rectnum].left , susirectP[susiarr[i].rectnum].top , susirectP[susiarr[i].rectnum].right  , susirectP[susiarr[i].rectnum].bottom ) ;

					hBrush=CreateSolidBrush(RGB(0,0,255));	//�Ķ���
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc, susirectP[susiarr[i].rectnum].left+25 , susirectP[susiarr[i].rectnum].top+25 , susirectP[susiarr[i].rectnum].right-25  , susirectP[susiarr[i].rectnum].bottom-25 ) ;		

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}
	
			}




			

			/////////////////��� �׸���///////////////////////////////
			if(isClick[i] == 1)
			{
				if(click==1)
				{
					hBrush=CreateSolidBrush(RGB(255,0,0));	//������
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc,250,250,350,350) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}
				else if(click==2)
				{
					hBrush=CreateSolidBrush(RGB(0,255,0));	//�ʷϻ�
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc,250,250,350,350) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}
				else if(click ==3)
				{
					hBrush=CreateSolidBrush(RGB(0,0,255));	//�Ķ���
					oldBrush = (HBRUSH)SelectObject(hdc,hBrush);

					Rectangle(hdc,250,250,350,350) ;	

					SelectObject(hdc,oldBrush);
					DeleteObject(hBrush);
				}
			}