// VideoWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "Panorama_Player.h"
#include "VideoWindow.h"
#include "afxdialogex.h"


// VideoWindow 对话框

IMPLEMENT_DYNAMIC(VideoWindow, CDialogEx)
	  
/*********************************************************
函数名：MonitorEnumProc（）
输入参数：HMONITOR hMonitor ---显示器句柄
          HDC hdcMonitor  ----显示器DC句柄
     LPRECT lprcMonitor-----
     LPARAM dwData-----EnumDisplayMonitors传来的数据
返回：bool
功能：若返回为真，EnumDisplayMonitors继续枚举，
      若返回为假，EnumDisplayMonitors停止枚举，从而获得显示器信息
       将显示器个数保存在DisPlaynum，分辨率信息保存在DisPlayRect
**********************************************************/
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
        HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{

	VideoWindow * videoWin = (VideoWindow *)dwData;
   static BOOL first = TRUE;   //标志

   //保存显示器信息
   MONITORINFO monitorinfo;         
   monitorinfo.cbSize = sizeof(MONITORINFO);

   //获得显示器信息，将信息保存到monitorinfo中
   GetMonitorInfo(hMonitor, &monitorinfo);

   //若检测到主屏
   if(monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
   {
    if(first)  //第一次检测到主屏
    {
     first = FALSE;
     videoWin->DisPlaynum = 1;

     //将显示器的分辨率信息保存到DisPlayRect
     videoWin->DisPlayRect[0] = monitorinfo.rcMonitor;
     return TRUE;  

    }
    else //第二次检测到主屏,说明所有的监视器都已经检测了一遍，故可以停止检测了
    {
     first = TRUE;    //标志复位
     return FALSE;    //结束检测
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
void VideoWindow::setWindowType(int type)//type：0：在第一屏小屏显示 1:第二屏全屏显示（有第二屏的话） 
{
	
	//获取当前屏幕的分辨率 和 屏幕个数
	//DisPlaynum  = 0;
	
	if(type == 1 && DisPlaynum == 1){
		type = 0;
	}
	WindowShowType = type;
	type = 0;
	if(type == 0){		
			//ModifyStyle(WS_CAPTION, 0); // 去掉标题栏
			ModifyStyleEx(WS_EX_DLGMODALFRAME, 0); // 去掉边框

			//ModifyStyle(0, WS_CAPTION); // 加上标题栏
			ModifyStyleEx(0, WS_EX_DLGMODALFRAME); // 加上边框

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
			ModifyStyle(WS_CAPTION, 0); // 去掉标题栏
			ModifyStyleEx(WS_EX_DLGMODALFRAME, 0); // 去掉边框
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


	// 移动picturebox到对话框的左上角。
	rcPicture.MoveToX(0);
	rcPicture.MoveToY(0);

	// pircurebox的长宽是对话框长宽的2分之1.
	rcPicture.right = rcClient.Width();
	rcPicture.bottom = rcClient.Height();
	// 移动picturebox.s

	GetDlgItem(IDC_STATIC2)->MoveWindow(rcPicture);
	//刷新窗口
	//Invalidate(TRUE);
}

BEGIN_MESSAGE_MAP(VideoWindow, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_STATIC2, &VideoWindow::OnStnClickedStatic2)
END_MESSAGE_MAP()


// VideoWindow 消息处理程序


void VideoWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	//if(screen && windowIsCreate)
	//	setWindowType(0);
	// TODO: 在此处添加消息处理程序代码
}


BOOL VideoWindow::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类
	windowIsCreate = 1;
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}


void VideoWindow::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

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
       //修改窗体背景颜色
       CRect rect;
       CPaintDC dc(this);
       GetClientRect(rect);
 
       //设置为白色背景
       dc.FillSolidRect(rect,RGB(255,255,255)); 
 
       CDialog::OnPaint();
    }
}


void VideoWindow::OnStnClickedStatic2()
{
	// TODO: 在此添加控件通知处理程序代码
}
