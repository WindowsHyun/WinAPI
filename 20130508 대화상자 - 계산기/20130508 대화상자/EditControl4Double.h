LRESULT EditControl4Double::OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
   // ���ڰ� �ƴϸ� �ñ�
   if(   (wParam>=' ' && wParam<='-') ||
      wParam=='/' ||
      (wParam>=':' && wParam<='~') ||
      wParam>=0xff)
   {
      return 0;
   }

   // . �Է� ó��
   if(wParam=='.')
   {
      TCHAR temp[2048];
      GetWindowText(temp, 2048);

      // ���� ���ڿ��� . �� ���� ���... �����Ų��.
      if(_tcschr(temp, '.')==NULL)
      {
         bHandled = FALSE;
         return 0;
      }

      // �̹� ���� ���ڿ��� . �� ���ԵǾ� �ִ�.
      DWORD start;
      DWORD end;
      int ret = ((DWORD)SNDMSG((m_hWnd), EM_GETSEL, (WPARAM)&start, (LPARAM)&end));

      int len = end-start;

      // ���õ� ���ڿ��� �ִ�..
      if(len)
      {
         TCHAR* p = temp + start;

         while(len)
         {
            // ���õ� ���ڿ��� . �� ���ԵǾ� �ִ�.
            if(*p=='.')
            {
               bHandled = FALSE;
               return 0;
            }
            p++;
            len--;
         }
      }

      // ������� �ô´�.
      return 0;
   }

   // ���
   bHandled = FALSE;
   return 0;
}