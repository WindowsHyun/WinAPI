LRESULT EditControl4Double::OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
   // 숫자가 아니면 씹기
   if(   (wParam>=' ' && wParam<='-') ||
      wParam=='/' ||
      (wParam>=':' && wParam<='~') ||
      wParam>=0xff)
   {
      return 0;
   }

   // . 입력 처리
   if(wParam=='.')
   {
      TCHAR temp[2048];
      GetWindowText(temp, 2048);

      // 기존 문자열에 . 이 없는 경우... 통과시킨다.
      if(_tcschr(temp, '.')==NULL)
      {
         bHandled = FALSE;
         return 0;
      }

      // 이미 기존 문자열에 . 이 포함되어 있다.
      DWORD start;
      DWORD end;
      int ret = ((DWORD)SNDMSG((m_hWnd), EM_GETSEL, (WPARAM)&start, (LPARAM)&end));

      int len = end-start;

      // 선택된 문자열이 있다..
      if(len)
      {
         TCHAR* p = temp + start;

         while(len)
         {
            // 선택된 문자열에 . 이 포함되어 있다.
            if(*p=='.')
            {
               bHandled = FALSE;
               return 0;
            }
            p++;
            len--;
         }
      }

      // 여기오면 씹는다.
      return 0;
   }

   // 통과
   bHandled = FALSE;
   return 0;
}