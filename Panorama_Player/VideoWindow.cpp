// VideoWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Panorama_Player.h"
#include "VideoWindow.h"
#include "afxdialogex.h"


// VideoWindow �Ի���

IMPLEMENT_DYNAMIC(VideoWindow, CDialogEx)
	  
/*********************************************************
��������MonitorEnumProc����
���������HMONITOR hMonitor ---��ʾ�����
          HDC hdcMonitor  ----��ʾ��DC���
     LPRECT lprcMonitor-----
     LPARAM dwData-----EnumDisplayMonitors����������
���أ�bool
���ܣ�������Ϊ�棬EnumDisplayMonitors����ö�٣�
      ������Ϊ�٣�EnumDisplayMonitorsֹͣö�٣��Ӷ������ʾ����Ϣ
       ����ʾ������������DisPlaynum���ֱ�����Ϣ������DisPlayRect
**********************************************************/
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
        HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{

	VideoWindow * videoWin = (VideoWindow *)dwData;
   static BOOL first = TRUE;   //��־

   //������ʾ����Ϣ
   MONITORINFO monitorinfo;         
   monitorinfo.cbSize = sizeof(MONITORINFO);

   //�����ʾ����Ϣ������Ϣ���浽monitorinfo��
   GetMonitorInfo(hMonitor, &monitorinfo);

   //����⵽����
   if(monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
   {
    if(first)  //��һ�μ�⵽����
    {
     first = FALSE;
     videoWin->DisPlaynum = 1;

     //����ʾ���ķֱ�����Ϣ���浽DisPlayRect
     videoWin->DisPlayRect[0] = monitorinfo.rcMonitor;
     return TRUE;  

    }
    else //�ڶ��μ�⵽����,˵�����еļ��������Ѿ������һ�飬�ʿ���ֹͣ�����
    {
     first = TRUE;    //��־��λ
     return FALSE;    //�������
    }
   }

   videoWin->DisPlayRect[videoWin->DisPlaynum] = monitorinfo.rcMonitor;
   videoWin->DisPlaynum++;
   return TRUE;
}

VideoWindow::VideoWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(VideoWindow::IDD, pParent)
{
	windowIsCreate = 0;
	DisPlaynum = 0;
	screen = NULL;
	EnumDisplayMonitors(NULL,NULL,MonitorEnumProc,(LPARAM)(void*)this);
}

VideoWindow::~VideoWindow()
{
	if(screen){
		SDL_DestroyWindow(screen);
		screen = NULL;
	}
}

void VideoWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC2, m_ShowPic);
}

SDL_Window * VideoWindow::GetScreen()
{
	return screen;
}

void VideoWindow::CreateScreen()
{
	screen = SDL_CreateWindowFrom((void *)( GetDlgItem(IDC_STATIC2)->GetSafeHwnd()));
}
void VideoWindow::DestroyScreen()
{
  SDL_DestroyWindow(screen);
}


void VideoWindow::getScreenRect(CRect *rect)
{
	GetDlgItem(IDC_STATIC2)->GetWindowRect(*rect);
}
void VideoWindow::setWindowType(int type)//type��0���ڵ�һ��С����ʾ 1:�ڶ���ȫ����ʾ���еڶ����Ļ��� 
{
	
	//��ȡ��ǰ��Ļ�ķֱ��� �� ��Ļ����
	//DisPlaynum  = 0;
	
	if(type == 1 && DisPlaynum == 1){
		type = 0;
	}
	WindowShowType = type;
	type = 0;
	if(type == 0){		
			//ModifyStyle(WS_CAPTION, 0); // ȥ��������
			ModifyStyleEx(WS_EX_DLGMODALFRAME, 0); // ȥ���߿�

			//ModifyStyle(0, WS_CAPTION); // ���ϱ�����
			ModifyStyleEx(0, WS_EX_DLGMODALFRAME); // ���ϱ߿�

			/*SetWindowPos(NULL, 			
				DisPlayRect[0].right/2,
				100,
				(DisPlayRect[0].right - DisPlayRect[0].left)/2,
				(DisPlayRect[0].bottom - DisPlayRect[0].top)/2,
				SWP_SHOWWINDOW);*/
			SetWindowPos(NULL, 			
				//DisPlayRect[0].left + (DisPlayRect[0].right - DisPlayRect[0].left)/2,
				0,
				0,
				(DisPlayRect[0].right - DisPlayRect[0].left),
				(DisPlayRect[0].bottom - DisPlayRect[0].top),
				SWP_SHOWWINDOW);
	}else if(type == 1){
			ModifyStyle(WS_CAPTION, 0); // ȥ��������
			ModifyStyleEx(WS_EX_DLGMODALFRAME, 0); // ȥ���߿�
			SetWindowPos(NULL, 			
				DisPlayRect[0].right,
				//0,
				0,
				(DisPlayRect[1].right - DisPlayRect[1].left),
				(DisPlayRect[1].bottom - DisPlayRect[1].top),
				SWP_SHOWWINDOW);
	}

	CRect rcClient,rcPicture;
	GetWindowRect(&rcClient);


	// �ƶ�picturebox���Ի�������Ͻǡ�
	rcPicture.MoveToX(0);
	rcPicture.MoveToY(0);

	// pircurebox�ĳ����ǶԻ��򳤿��2��֮1.
	rcPicture.right = rcClient.Width();
	rcPicture.bottom = rcClient.Height();
	// �ƶ�picturebox.s

	GetDlgItem(IDC_STATIC2)->MoveWindow(rcPicture);
	//ˢ�´���
	//Invalidate(TRUE);
}

BEGIN_MESSAGE_MAP(VideoWindow, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_STATIC2, &VideoWindow::OnStnClickedStatic2)
END_MESSAGE_MAP()


// VideoWindow ��Ϣ�������


void VideoWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	//if(screen && windowIsCreate)
	//	setWindowType(0);
	// TODO: �ڴ˴������Ϣ����������
}


BOOL VideoWindow::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���
	windowIsCreate = 1;
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}


void VideoWindow::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
	ShowWindow(SW_HIDE);
}
void VideoWindow::OnPaint()
{
    if (IsIconic())
    {     
	
	}
    else
    {
       //�޸Ĵ��屳����ɫ
       CRect rect;
       CPaintDC dc(this);
       GetClientRect(rect);
 
       //����Ϊ��ɫ����
       dc.FillSolidRect(rect,RGB(255,255,255)); 
 
       CDialog::OnPaint();
    }
}


void VideoWindow::OnStnClickedStatic2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
