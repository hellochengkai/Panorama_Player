
// Panorama_PlayerDlg.h : 头文件
//

#pragma once
#include "Resource.h"
#include "CFtpServer.h"
#include "Th_Player.h"

#pragma   comment(lib,"Strmiids.lib")     
#pragma   comment(lib,"Quartz.lib")


#define WM_TRAY_NOTIFICATION WM_USER+1 
BOOL TrayMessage(HWND hWnd, DWORD dwMessage, HICON hIcon, LPCWSTR pszTip); 

#define 	WinPlayerNUM 2


// CPanorama_PlayerDlg 对话框
class CPanorama_PlayerDlg : public CDialogEx
{
// 构造
public:
	CPanorama_PlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PANORAMA_PLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

//-----------------------------------------------------------------------
//自己加的

//#define FTP_SER_USERNAME "thunder_user"
//#define FTP_SER_PASSWORD "1qa@WS3ed"
//#define FTP_SER_PATH "F:\\work\ftpserver"
public:
	
#define		FusionVideoDir "D:\\FVid\\"

#define 	SyncPlayPort 21395

#define		FTP_SER_USERNAME "FusionUser"
#define		FTP_SER_PASSWORD "thunder1996"
#define		FTP_SER_PATH FusionVideoDir
	CFtpServer FtpServer;
	CFtpServer::CUserEntry *pUser;
	CFtpServer::CUserEntry *pAnonymousUser;
	int StartFtpSer(const char * username,const char * password,const char *tfpserPath);
	int StopFtpSer();

	
	Th_Player BGplayer;
	Th_Player DshowPlayer;
	//Th_Player NetPlayer;
	Th_Player WinPlayer[WinPlayerNUM];
	
	int VideoUrlListNum;
	char VideoUrlList[1024][64];
	void InitVideoUrlList(CString& strDirPath);
	int TCPServerInit(int Port);
	int TCPServerWaitAccept(int listenfd);
	void TCPServerCutConnect(int *connectfd);
	int TCPServerSend(int Connectfd,char * SendBuf,int Len);
	int TCPServerRecv(int Connectfd,char * RecvBuf,int Len);
	void TCPServerDeinit(int *listenfd);

	CWinThread*	pSyncPlayThread;
	
	int ChangeBGPlayerUrl(char * Url);
	
	int CurPlayNum;
	CCriticalSection BGPlayUrllock;
	CCriticalSection WinPlayUrllock[WinPlayerNUM];

	char BGPlayUrl[1024];
	//char NetPlayUrl[1024];
	char DshowUrl[1024]; 
	char WinPlayerUrl[WinPlayerNUM][1024];
	int BGPlayerWaitEndFlag;
	
	int WinPlayerWaitEndFlag[WinPlayerNUM];
	CWinThread*	BGPlayThread;
	int RenderPresentTimerID;
	/*
	int pRecvMsgThreadIsRun;
	int pHandleMsgThreadIsRun;
	
	
	int pRecvMsgThreadIsEnd;
	int pHandleMsgThreadIsEnd;
	CWinThread* pRecvMsgThread;
	CWinThread* pHandleMsgThread;
	*/
	//ShowLogToEdit LogEdit;
	
	//ShowLogToEdit LogEdit;

	CEdit* pLogEdit;
	CFont m_editFont;
	CCriticalSection ShowLoglock;

	int ShowTimeToEdit();
	int ShowLogToEdit(char * str,int len);
	int LogEditEnt();
	int StopShowLog;
	
	class VideoWindow VideoWin;//播放器显示窗口
	void DisPlay_Player(void);
	void Hide_Player(void);
	SDL_Renderer * sdlRenderer;
	int getDshowVideoDevice();
	char *DshowVideoName;
	char *DshowAudioName;
	SDL_Surface * sdlBGSurface;
	SDL_Texture * sdlBGTexture;
	int OpenDShow();
	int CloseDShow();
	
	int ChangeWinPlayerUrl(const char * Url,const int num);
	void WinOpenFun(int num);
	void WinShowHide(int num);
//-----------------------------------------------------------------------
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClicked();
	afx_msg void OnBnClickedButton3();
	virtual BOOL DestroyWindow();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton4();
protected:
//	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CButton m_CtrLogButton;
//	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
	CButton m_Win1ShowHide;
	CButton m_Win2ShowHide;
	CButton m_Win3ShowHide;
	CButton m_Win4ShowHide;
	CButton m_DShow_ShowHide;
	CButton m_Win1Open;
	CButton m_Win2Open;
	CButton m_Win3Open;
	CButton m_Win4Open;
	CButton m_Win1SaveCfg;
	CButton m_Win2SaveCfg;
	CButton m_Win3SaveCfg;
	CButton m_Win4SaveCfg;
	CButton m_DShow_SaveCfg;
	CEdit m_Win1X;
	CEdit m_Win1Y;
	CEdit m_Win1W;
	CEdit m_Win1H;
	CEdit m_Win2X;
	CEdit m_Win2Y;
	CEdit m_Win2W;
	CEdit m_Win2H;
	CEdit m_Win3X;
	CEdit m_Win3Y;
	CEdit m_Win3H;
	CEdit m_Win3W;
	CEdit m_Win4X;
	CEdit m_Win4Y;
	CEdit m_Win4W;
	CEdit m_Win4H;
	CEdit m_DShowX;
	CEdit m_DShowY;
	CEdit m_DShowW;
	CEdit m_DShowH;
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton15();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton18();
	CEdit m_INFO_EDIT;
	CEdit m_Video1_H_W;
	CEdit m_Video2_H_W;
	CEdit m_Video3_H_W;
	CEdit m_Video4_H_W;
	CEdit m_DShow_H_W;
};
CPanorama_PlayerDlg* GetMainDlg();
