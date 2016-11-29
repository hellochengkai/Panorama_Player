
// Panorama_PlayerDlg.cpp : 实现文件
//

#include <afxpriv.h>
#include "stdafx.h"
#include "Panorama_Player.h"
#include "Panorama_PlayerDlg.h"
#include "afxdialogex.h"

#include "UDPMessage.h"
#include "VideoWindow.h"
#include "Th_Player.h"
#include "Dshow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPanorama_PlayerDlg * MainDlg  = NULL;
CPanorama_PlayerDlg* GetMainDlg()
{
	return MainDlg;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
void OpenConsole()  
{  
    AllocConsole();  
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);  
    int hCrt = _open_osfhandle((long)handle,_O_TEXT);  
    FILE * hf = _fdopen( hCrt, "w" );  
    *stdout = *hf;  
} 

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//	afx_msg void OnPaint();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_SIZE()
//	ON_WM_SYSCOMMAND()
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPanorama_PlayerDlg 对话框
CPanorama_PlayerDlg::CPanorama_PlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPanorama_PlayerDlg::IDD, pParent)
{
	OpenConsole();
	printf("OpenConsole\n");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPanorama_PlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON2, m_CtrLogButton);

	DDX_Control(pDX, IDC_BUTTON7, m_Win1Open);
	DDX_Control(pDX, IDC_BUTTON8, m_Win2Open);
	DDX_Control(pDX, IDC_BUTTON9, m_Win3Open);
	DDX_Control(pDX, IDC_BUTTON10, m_Win4Open);

	DDX_Control(pDX, IDC_BUTTON19, m_Win1ShowHide);
	DDX_Control(pDX, IDC_BUTTON20, m_Win2ShowHide);
	DDX_Control(pDX, IDC_BUTTON21, m_Win3ShowHide);
	DDX_Control(pDX, IDC_BUTTON22, m_Win4ShowHide);
	DDX_Control(pDX, IDC_BUTTON23, m_DShow_ShowHide);

	DDX_Control(pDX, IDC_BUTTON15, m_Win1SaveCfg);
	DDX_Control(pDX, IDC_BUTTON16, m_Win2SaveCfg);
	DDX_Control(pDX, IDC_BUTTON17, m_Win3SaveCfg);
	DDX_Control(pDX, IDC_BUTTON14, m_Win4SaveCfg);
	DDX_Control(pDX, IDC_BUTTON18, m_DShow_SaveCfg);

	DDX_Control(pDX, IDC_EDIT2, m_Win1X);
	DDX_Control(pDX, IDC_EDIT3, m_Win1Y);
	DDX_Control(pDX, IDC_EDIT4, m_Win1W);
	DDX_Control(pDX, IDC_EDIT5, m_Win1H);

	DDX_Control(pDX, IDC_EDIT6, m_Win2X);
	DDX_Control(pDX, IDC_EDIT7, m_Win2Y);
	DDX_Control(pDX, IDC_EDIT8, m_Win2W);
	DDX_Control(pDX, IDC_EDIT9, m_Win2H);

	DDX_Control(pDX, IDC_EDIT10, m_Win3X);
	DDX_Control(pDX, IDC_EDIT11, m_Win3Y);
	DDX_Control(pDX, IDC_EDIT12, m_Win3W);
	DDX_Control(pDX, IDC_EDIT13, m_Win3H);

	DDX_Control(pDX, IDC_EDIT14, m_Win4X);
	DDX_Control(pDX, IDC_EDIT15, m_Win4Y);
	DDX_Control(pDX, IDC_EDIT16, m_Win4W);
	DDX_Control(pDX, IDC_EDIT17, m_Win4H);

	DDX_Control(pDX, IDC_EDIT18, m_DShowX);
	DDX_Control(pDX, IDC_EDIT19, m_DShowY);
	DDX_Control(pDX, IDC_EDIT20, m_DShowW);
	DDX_Control(pDX, IDC_EDIT21, m_DShowH);
	DDX_Control(pDX, IDC_EDIT22, m_INFO_EDIT);

	
	DDX_Control(pDX, IDC_EDIT23, m_Video1_H_W);
	DDX_Control(pDX, IDC_EDIT24, m_Video2_H_W);
	DDX_Control(pDX, IDC_EDIT25, m_Video3_H_W);
	DDX_Control(pDX, IDC_EDIT26, m_Video4_H_W);
	DDX_Control(pDX, IDC_EDIT27, m_DShow_H_W);
}

BEGIN_MESSAGE_MAP(CPanorama_PlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CPanorama_PlayerDlg::OnBnClickedButton3)
//	ON_WM_SIZE()
ON_BN_CLICKED(IDC_BUTTON4, &CPanorama_PlayerDlg::OnBnClickedButton4)
//ON_MESSAGE(WM_TRAY_NOTIFICATION, &CPanorama_PlayerDlg::OnTrayNotification)
ON_MESSAGE(WM_TRAY_NOTIFICATION, &CPanorama_PlayerDlg::OnTrayNotification)
ON_WM_NCHITTEST()
ON_BN_CLICKED(IDC_BUTTON1, &CPanorama_PlayerDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &CPanorama_PlayerDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON5, &CPanorama_PlayerDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON7, &CPanorama_PlayerDlg::OnBnClickedButton7)
ON_BN_CLICKED(IDC_BUTTON19, &CPanorama_PlayerDlg::OnBnClickedButton19)
ON_BN_CLICKED(IDC_BUTTON20, &CPanorama_PlayerDlg::OnBnClickedButton20)
ON_BN_CLICKED(IDC_BUTTON21, &CPanorama_PlayerDlg::OnBnClickedButton21)
ON_BN_CLICKED(IDC_BUTTON22, &CPanorama_PlayerDlg::OnBnClickedButton22)
ON_BN_CLICKED(IDC_BUTTON23, &CPanorama_PlayerDlg::OnBnClickedButton23)
ON_BN_CLICKED(IDC_BUTTON8, &CPanorama_PlayerDlg::OnBnClickedButton8)
ON_BN_CLICKED(IDC_BUTTON9, &CPanorama_PlayerDlg::OnBnClickedButton9)
ON_BN_CLICKED(IDC_BUTTON10, &CPanorama_PlayerDlg::OnBnClickedButton10)
ON_BN_CLICKED(IDC_BUTTON15, &CPanorama_PlayerDlg::OnBnClickedButton15)
ON_BN_CLICKED(IDC_BUTTON16, &CPanorama_PlayerDlg::OnBnClickedButton16)
ON_BN_CLICKED(IDC_BUTTON17, &CPanorama_PlayerDlg::OnBnClickedButton17)
ON_BN_CLICKED(IDC_BUTTON14, &CPanorama_PlayerDlg::OnBnClickedButton14)
ON_BN_CLICKED(IDC_BUTTON18, &CPanorama_PlayerDlg::OnBnClickedButton18)
END_MESSAGE_MAP()



void CPanorama_PlayerDlg::InitVideoUrlList(CString& strDirPath)
{
   VideoUrlListNum = 0;
   CFileFind FileFinder;  
   int Ret = 0;
    if (FileFinder.FindFile(strDirPath + "*"))  
    {  
		//int i = 0;
		while(FileFinder.FindNextFile() != 0){
			CString strFileName  = FileFinder.GetFileName();
			if(strFileName != "." && strFileName != "..")
			{
				USES_CONVERSION;
				LPSTR file_name = T2A(strFileName);
				int fileNum = 0;
				if(sscanf(file_name,"%d.",&fileNum) > 0 && fileNum > 0){
					CString strFile_Path = strDirPath + strFileName;  
					USES_CONVERSION;
					LPSTR file_path = T2A(strFile_Path);
					strcpy(VideoUrlList[VideoUrlListNum],file_path);
					VideoUrlListNum++;
				}
			}
		}
		CString strFileName  = FileFinder.GetFileName();  
		if(strFileName != "." && strFileName != "..")
		{
			USES_CONVERSION;
			LPSTR file_name = T2A(strFileName);
			int fileNum = 0;
			if(sscanf(file_name,"%d.",&fileNum) > 0 && fileNum > 0){
				CString strFile_Path = strDirPath + strFileName;  
				USES_CONVERSION;
				LPSTR file_path = T2A(strFile_Path);
				strcpy(VideoUrlList[VideoUrlListNum],file_path);
				VideoUrlListNum++;
			}
		}
    } 
    FileFinder.Close();  
}


int CPanorama_PlayerDlg::TCPServerInit(int Port)
{
	int  listenfd = 0;
	int Ret = -1;
	struct  sockaddr_in server;
	int opt =SO_REUSEADDR;
	
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Creating  socket failed.");
		goto EXIT;
	}
	setsockopt(listenfd,SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
	
	memset(&server,0,sizeof(server));
	server.sin_family=AF_INET;
	//server.sin_port=htons(FusionSyncPort);
	server.sin_port=htons(Port);
	
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
		perror("Binderror.");
		goto EXIT;
	}  
	
	#define  BACKLOG 10

	if(listen(listenfd,BACKLOG)== -1){  /* calls listen() */
		perror("listen()error\n");
		goto EXIT;
	}
	Ret = listenfd;
EXIT:
	if(Ret == -1){
		if(listenfd){
			closesocket(listenfd);
		}
	}
	return Ret;
}

int CPanorama_PlayerDlg::TCPServerWaitAccept(int listenfd)
{
	int connectfd;
	struct  sockaddr_in client;

	int addrlen = sizeof(client);
	char client_IP[INET_ADDRSTRLEN] = "";
	
	while(1){
		if((connectfd = accept(listenfd,(struct sockaddr*)&client,&addrlen)) == INVALID_SOCKET) {
			perror("accept()error\n");
			Sleep(100);
			continue;
		}else{
			break;
		} 
	}
	DWORD nNetTimeout = 1000*1;//超时时长
	setsockopt(connectfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
	inet_ntop(AF_INET,&client.sin_addr,client_IP,INET_ADDRSTRLEN);
	
	ShowTimeToEdit();
	ShowLogToEdit("TCP Client LogIn:",strlen("TCP Client LogIn:"));
	ShowLogToEdit(client_IP,INET_ADDRSTRLEN);
	ShowLogToEdit("!",1);
	LogEditEnt();

	return connectfd;
}
void CPanorama_PlayerDlg::TCPServerCutConnect(int *connectfd)
{
	if(*connectfd){
		ShowTimeToEdit();
		ShowLogToEdit("TCP Client Exit!",strlen("TCP Client Exit!"));
		LogEditEnt();
		closesocket(*connectfd);
		*connectfd = 0;
	}
}

int CPanorama_PlayerDlg::TCPServerSend(int Connectfd,char * SendBuf,int Len)
{
	int Ret = 0;
	ShowTimeToEdit();
	ShowLogToEdit("TCPSend: ",strlen("TCPSend: "));
	ShowLogToEdit(SendBuf,Len);
	LogEditEnt();
	Ret = send(Connectfd,SendBuf,Len,0);
	return Ret;
}
int CPanorama_PlayerDlg::TCPServerRecv(int Connectfd,char * RecvBuf,int Len)
{
	int Ret = 0;
	Ret = recv(Connectfd,RecvBuf,Len,0);
	if(Ret > 0){
		ShowTimeToEdit();
		ShowLogToEdit("TCPRecv: ",strlen("TCPRecv: "));
		ShowLogToEdit(RecvBuf,Len);
		LogEditEnt();
	}
	return Ret;
}

void CPanorama_PlayerDlg::TCPServerDeinit(int *listenfd)
{
	if(*listenfd){
		closesocket(*listenfd);
		*listenfd = 0;
	}
}

int CPanorama_PlayerDlg::ChangeBGPlayerUrl(char * Url)
{
	BGPlayUrllock.Lock();
	AVFormatContext	*pCtx = NULL;
	int Ret = -1;
	if(NULL == Url){
		goto EXIT;
	}
	if((avformat_open_input(&pCtx,Url,NULL,NULL)!=0)|| (pCtx == NULL)){
		printf("Couldn't open input stream.\n");
		goto EXIT;
	}
	Ret = 0;
	
	memset(BGPlayUrl,0,1024);
	CurPlayNum = 0;
	memcpy(BGPlayUrl,Url,strlen(Url));
	sscanf(BGPlayUrl + strlen(FusionVideoDir),"%d.",&CurPlayNum);

EXIT:
	if(pCtx){
		avformat_close_input(&pCtx);
		pCtx = NULL;
	}
	BGPlayUrllock.Unlock();
	return Ret;
}

static int decode_interrupt_cb_count = 0;
static int decode_interrupt_cb_ChangeWinPlayerUrl(void *ctx)
{
	int Ret = 0;
	AVFormatContext * pCtx = (AVFormatContext *)(ctx);
/*	
	decode_interrupt_cb_count++;
	printf("decode_interrupt_cb_count == %d\n",decode_interrupt_cb_count);
	if(decode_interrupt_cb_count > 10){
		return 1;
	}
*/	
	int count = 0;
	while(count <= 10){
		count++;
		Sleep(1000);
		printf("ctx_flags == %d\n",pCtx->ctx_flags);
		if(pCtx->ctx_flags != 0){
			Ret = 0;
			break;
		}
	}
	return Ret;
}


int CPanorama_PlayerDlg::ChangeWinPlayerUrl(const char * Url,const int num)
{
	AVFormatContext *pCtx = avformat_alloc_context();
	int Ret = -1;
	if(NULL == Url){
		goto EXIT;
	}

	/*
	AVDictionary *opts = NULL;
	
	if(strstr(Url,"rtmp")||strstr(Url,"rtsp")){
        av_dict_set(&opts, "timeout", "6000", 0); // in secs
	}else if(strstr(Url,"http")){
        av_dict_set(&opts, "timeout", "6000", 0); // in ms
	}
	
	pCtx->flags |= AVFMT_FLAG_NONBLOCK;
	*/

	
	//pCtx->interrupt_callback.callback = decode_interrupt_cb_ChangeWinPlayerUrl;
	//pCtx->interrupt_callback.opaque = (void *)pCtx;
	

	if((avformat_open_input(&pCtx,Url,NULL,NULL)!=0)||(pCtx == NULL)){
		printf("Couldn't open input stream %s.\n",Url);
		goto EXIT;
	}
	Ret = 0;
	WinPlayUrllock[num].Lock();
	memset(WinPlayerUrl[num],0,1024);
	memcpy(WinPlayerUrl[num],Url,strlen(Url));
	WinPlayUrllock[num].Unlock();
EXIT:
	
	decode_interrupt_cb_count = 0;
	if(pCtx){
		avformat_close_input(&pCtx);
		pCtx = NULL;
	}
	return Ret;
}

char EmptyBuffer[1024] = {0};
UINT BGPlayThreadFun(LPVOID pParam)
{
	CPanorama_PlayerDlg * Dlg = (CPanorama_PlayerDlg *)pParam;
	while(1){
		Dlg->BGplayer.Stop_Player();
		Dlg->BGplayer.Close_Player();
		
		Dlg->BGPlayUrllock.Lock();
		if(memcmp(EmptyBuffer,Dlg->BGPlayUrl,1024) == 0){
			Dlg->BGPlayUrllock.Unlock();
			Sleep(1000);
			continue;
		}
		Dlg->BGPlayerWaitEndFlag = 1;
		Dlg->BGplayer.Open_Player(Dlg->BGPlayUrl);
		
		Dlg->BGPlayUrllock.Unlock();

		Dlg->BGplayer.Play_Player();
		Dlg->DisPlay_Player();
		//Sleep(3000);
		//if(0)
		while(Dlg->BGPlayerWaitEndFlag){
			if(Dlg->BGplayer.play_end)
				break;
				Sleep(100);
		}
	}
}

UINT DshowPlayerThreadFun(LPVOID pParam)
{
	CPanorama_PlayerDlg * Dlg = (CPanorama_PlayerDlg *)pParam;
	while(1){
		Dlg->DshowPlayer.Stop_Player();
		Dlg->DshowPlayer.Close_Player();
		Dlg->DshowPlayer.Open_Player(Dlg->DshowUrl);
		
		char defaultHW[1024] = {0};
		sprintf(defaultHW,"%d/%d",Dlg->DshowPlayer.pVidCodecCtx->width,Dlg->DshowPlayer.pVidCodecCtx->height);
		Dlg->m_DShow_H_W.SetWindowText((CString)defaultHW);
		Dlg->m_DShow_H_W.EnableWindow(FALSE);

		
		Dlg->DshowPlayer.Play_Player();
		Dlg->DisPlay_Player();
		while(1){
			if(Dlg->DshowPlayer.play_end)
				break;
				Sleep(100);
		}
	}
}

UINT WinPlayThreadFun(LPVOID pParam)
{
	int i = (int)pParam;
	
	CPanorama_PlayerDlg * Dlg = (CPanorama_PlayerDlg *)MainDlg;
	CEdit * editHW[] = {&Dlg->m_Video1_H_W,&Dlg->m_Video2_H_W,
						&Dlg->m_Video3_H_W,&Dlg->m_Video4_H_W};
	while(1){
		Dlg->WinPlayer[i].Stop_Player();
		Dlg->WinPlayer[i].Close_Player();
		
		Dlg->WinPlayUrllock[i].Lock();
		if(memcmp(EmptyBuffer,Dlg->WinPlayerUrl[i],1024) == 0){
			Dlg->WinPlayUrllock[i].Unlock();
			Sleep(1000);
			continue;
		}
		Dlg->WinPlayerWaitEndFlag[i] = 1;
		Dlg->WinPlayer[i].Open_Player(Dlg->WinPlayerUrl[i]);
		
		char defaultHW[1024] = {0};
		sprintf(defaultHW,"%d/%d",Dlg->WinPlayer[i].pVidCodecCtx->width,Dlg->WinPlayer[i].pVidCodecCtx->height);
		editHW[i]->SetWindowText((CString)defaultHW);
		editHW[i]->EnableWindow(FALSE);

		
		Dlg->WinPlayUrllock[i].Unlock();
		Dlg->WinPlayer[i].Play_Player();

		while(Dlg->WinPlayerWaitEndFlag[i]){
			if(Dlg->WinPlayer[i].play_end)
				break;
				Sleep(100);
		}
	}
}

int CPanorama_PlayerDlg::OpenDShow()
{
	if(DshowPlayer.PlayerIsPlay)
		return 0;
	
	DshowVideoName = NULL;
	DshowAudioName = NULL;

	int Ret = -1;
	if(getDshowVideoDevice() == 0){
		memset(DshowUrl,0,1024);
		if(DshowVideoName){
			if(strstr(DshowVideoName,"HDMI") != NULL){
				strcat(DshowUrl,"video=");
				strcat(DshowUrl,DshowVideoName);
				//strcat(DshowUrl,":audio=");
				//strcat(DshowUrl,DshowVideoName);
				//sprintf(DshowUrl,"video=%s:audio=%s",DshowVideoName,DshowVideoName);
			}else{
				if(DshowAudioName){
					strcat(DshowUrl,"video=");
					strcat(DshowUrl,DshowVideoName);
					//strcat(DshowUrl,":audio=");
					//strcat(DshowUrl,DshowAudioName);
					//sprintf(DshowUrl,"video=%s:audio=%s",DshowVideoName,DshowAudioName);
				}else{
					strcat(DshowUrl,"video=");
					strcat(DshowUrl,DshowVideoName);
					//sprintf(DshowUrl,"video=%s",DshowVideoName);
				}
			}
			printf("open dshow device %s \n",DshowUrl);
			Ret = DshowPlayer.Open_Player(DshowUrl);
			if(Ret == 0){
				char defaultHW[1024] = {0};
				sprintf(defaultHW,"%d/%d",DshowPlayer.pVidCodecCtx->width,DshowPlayer.pVidCodecCtx->height);
				m_DShow_H_W.SetWindowText((CString)defaultHW);
				m_DShow_H_W.EnableWindow(FALSE);

				DshowPlayer.Play_Player();
			}
		}else{
			printf("not find dshow device \n");
		}
	}
	return Ret;
}

int CPanorama_PlayerDlg::CloseDShow()
{
	int Ret = 0;
	Ret = DshowPlayer.Stop_Player();
	Ret |= DshowPlayer.Close_Player();
//	Ret |= DshowPlayer.Deinit_Player();
	return Ret;
}

UINT SyncPlayThreadFun(LPVOID pParam)
{
	CPanorama_PlayerDlg * Dlg = (CPanorama_PlayerDlg *)pParam;	
	//const char strSTBOnLine[] = "STBOnLine";
	const char strCurPlayNum[] = "CurPlayNum";
	const char strChangeCurPlayNum[] = "ChangeCurPlayNum";
	const char strAllPlayNum[] = "AllPlayNum";
	
	char sendBuf[1024] = "";

	int  listenfd, connectfd;
	
	listenfd = Dlg->TCPServerInit(SyncPlayPort);
	if(listenfd == -1)
		goto EXIT;
	
	while(1){
		char recv_buf[1024] = "";
		connectfd = Dlg->TCPServerWaitAccept(listenfd);
		int recvTimeOutNum = 0;
		while(1){
			int Ret = Dlg->TCPServerRecv(connectfd,recv_buf,sizeof(recv_buf));
			if(Ret > 0){
				//��ȡ��ǰ�������ŵı��
				if(strncmp(recv_buf,strCurPlayNum,strlen(strCurPlayNum)) == 0){
					memset(sendBuf,0,1024);
					sprintf(sendBuf,"%s:%d",strCurPlayNum,Dlg->CurPlayNum);
					Dlg->TCPServerSend(connectfd,sendBuf,strlen(sendBuf)+1);
					recvTimeOutNum = 0;
				}
				//��ȡ���еĲ��ű��
				if(strncmp(recv_buf,strAllPlayNum,strlen(strAllPlayNum)) == 0){
					memset(Dlg->VideoUrlList,0,1024*64);
					Dlg->InitVideoUrlList((CString)FusionVideoDir);
					memset(sendBuf,0,1024);
					sprintf(sendBuf,"TotalPlayNum:%d",Dlg->VideoUrlListNum);
					Dlg->TCPServerSend(connectfd,sendBuf,strlen(sendBuf));
					int i = 0;
					memset(sendBuf,0,1024);
					for(i = 0;i < Dlg->VideoUrlListNum +1;i++){
						int PlayNum = 0;
						if(sscanf(Dlg->VideoUrlList[i] + strlen(FusionVideoDir),"%d.",&PlayNum) > 0 && PlayNum > 0){
							sprintf(sendBuf,"%sPlayNum:%d",sendBuf,PlayNum);
						}
					}
					printf("sendBuf %s\n",sendBuf);
					Dlg->TCPServerSend(connectfd,sendBuf,strlen(sendBuf)+1);
					
					for(i = 0;i < Dlg->VideoUrlListNum +1;i++){
						if(Dlg->CurPlayNum == 0){
							if(Dlg->ChangeBGPlayerUrl(Dlg->VideoUrlList[0]) == 0){
								Dlg->BGPlayerWaitEndFlag = 0;
								break;
							}
						}
					}
					recvTimeOutNum = 0;
				}
				//�ı䵱ǰ�ı���
				if(strncmp(recv_buf,strChangeCurPlayNum,strlen(strChangeCurPlayNum)) == 0){
					int recvplaynum = 0;
					sscanf(recv_buf,"ChangeCurPlayNum:%d",&recvplaynum);
					int i = 0;
					for(i = 0;i < Dlg->VideoUrlListNum +1;i++){
						int PlayNum = 0;
						sscanf(Dlg->VideoUrlList[i] + strlen(FusionVideoDir),"%d.",&PlayNum);
						if(PlayNum == recvplaynum && recvplaynum != Dlg->CurPlayNum){
							int OldCurPlayNum = Dlg->CurPlayNum;
							Dlg->CurPlayNum = recvplaynum;
							if(Dlg->ChangeBGPlayerUrl(Dlg->VideoUrlList[i]) == 0){
								Dlg->BGPlayerWaitEndFlag = 0;
								Dlg->TCPServerSend(connectfd,"PlaySuccess",strlen("PlaySuccess")+1);
							}else{
								Dlg->CurPlayNum = OldCurPlayNum;
								Dlg->TCPServerSend(connectfd,"PlayFail",strlen("PlayFail")+1);
							}
							break;
						}
					}
					if(i == Dlg->VideoUrlListNum +1)
					{
						Dlg->TCPServerSend(connectfd,"PlayFail",strlen("PlayFail")+1);
					}
				}
				//����ر�HDMI_IN����ʾ
				if(strncmp(recv_buf,"HDMI_IN",strlen("HDMI_IN")) == 0){
					if(recv_buf,"HDMI_IN:Open",strlen("HDMI_IN:Open")){
				/*		if(Dlg->OpenDShow()==0){
							Dlg->TCPServerSend(connectfd,"HDMI_IN:Open(Success)",strlen("HDMI_IN:Open(Success)")+1);
						}else{
							Dlg->TCPServerSend(connectfd,"HDMI_IN:Open(Fail)",strlen("HDMI_IN:Open(Fail)")+1);
						}

						*/
						Dlg->DshowPlayer.ShowWindow();
						Dlg->TCPServerSend(connectfd,"HDMI_IN:Open(Success)",strlen("HDMI_IN:Open(Success)")+1);
					}
					if(strncmp(recv_buf,"HDMI_IN:Close",strlen("HDMI_IN:Close"))==0){
						/*if(Dlg->CloseDShow()==0){
							Dlg->TCPServerSend(connectfd,"HDMI_IN:Close(Success)",strlen("HDMI_IN:Close(Success)")+1);
						}else{
							Dlg->TCPServerSend(connectfd,"HDMI_IN:Close(Fail)",strlen("HDMI_IN:Close(Fail)")+1);
						}*/
						Dlg->DshowPlayer.HideWindow();
						Dlg->TCPServerSend(connectfd,"HDMI_IN:Close(Success)",strlen("HDMI_IN:Close(Success)")+1);
					}
				}	
				//����ر�С���ڵ���ʾ
				if(strncmp(recv_buf,"Win",strlen("Win")) == 0){
					int WinNum = 0;
					sscanf(recv_buf,"Win%d:",&WinNum);
					if(WinNum > 0 && WinNum <= WinPlayerNUM){
						WinNum = WinNum-1;
						int OpenisSuccess = 0;
						if(strstr(recv_buf,":Open")!=NULL){
							if(strstr(recv_buf,":Url")!=NULL){
								if(strlen(Dlg->WinPlayerUrl[WinNum]) == (strlen(strstr(recv_buf,"Url:")) - strlen("Url:"))
									&&(strncmp(Dlg->WinPlayerUrl[WinNum],strstr(recv_buf,"Url:")+strlen("Url:"),strlen(Dlg->WinPlayerUrl[WinNum])) == 0)){
									//�Ѿ��ڲ��������Ƶ
									OpenisSuccess = 1;
								}else{
									//�л���Ƶ
									//memset(Dlg->WinPlayerUrl[WinNum],0,1024);
									//memcpy(Dlg->WinPlayerUrl[WinNum],strstr(recv_buf,"Url:")+strlen("Url:"),strlen(strstr(recv_buf,"Url:")) - strlen("Url:"));
									if(Dlg->ChangeWinPlayerUrl(strstr(recv_buf,"Url:")+strlen("Url:"),WinNum) == 0){
										Dlg->WinPlayerWaitEndFlag[WinNum] = 0;
										OpenisSuccess = 1;
									}else{
										OpenisSuccess = 0;
									}
								}
							}else{
								OpenisSuccess = 1;
							}
							if(OpenisSuccess){
								memset(sendBuf,0,1024);
								sprintf(sendBuf,"Win%d:Open(Success)",WinNum+1);
								Dlg->TCPServerSend(connectfd,sendBuf,strlen(sendBuf));
							}else{
								memset(sendBuf,0,1024);
								sprintf(sendBuf,"Win%d:Open(Fail)",WinNum+1);
								Dlg->TCPServerSend(connectfd,sendBuf,strlen(sendBuf));
							}
							Dlg->WinPlayer[WinNum].ShowWindow();
						}else if(strstr(recv_buf,":Close")!=NULL){
							memset(sendBuf,0,1024);
							sprintf(sendBuf,"Win%d:Close(Success)",WinNum+1);
							Dlg->TCPServerSend(connectfd,sendBuf,strlen(sendBuf));
							Dlg->WinPlayer[WinNum].HideWindow();
						}
					}
				}
			}else if(Ret < 0){
				recvTimeOutNum++;
			}else if(Ret == 0){
				recvTimeOutNum = 0;
				break;
			}
			if(recvTimeOutNum > 10)
				break;
		}
		Dlg->TCPServerCutConnect(&connectfd);
	}
EXIT:
	Dlg->TCPServerDeinit(&listenfd);
	return 0;
}

UINT SyncPlayThreadFun_1(LPVOID pParam)
{
	CPanorama_PlayerDlg * Dlg = (CPanorama_PlayerDlg *)pParam;	
	//const char strSTBOnLine[] = "STBOnLine";
	const char strCurPlayNum[] = "CurPlayNum";
	const char strChangeCurPlayNum[] = "ChangeCurPlayNum";
	const char strAllPlayNum[] = "AllPlayNum";
	
	char sendBuf[1024] = "";

	int  listenfd, connectfd;
	
	listenfd = Dlg->TCPServerInit(SyncPlayPort);
	if(listenfd == -1)
		goto EXIT;
	
	while(1){
		char recv_buf[1024] = "";
		connectfd = Dlg->TCPServerWaitAccept(listenfd);
		int recvTimeOutNum = 0;
		while(1){
			int Ret = Dlg->TCPServerRecv(connectfd,recv_buf,sizeof(recv_buf));
			if(Ret > 0){
				if(strncmp(recv_buf,strCurPlayNum,strlen(strCurPlayNum)) == 0){
					memset(sendBuf,0,1024);
					sprintf(sendBuf,"%s:%d",strCurPlayNum,Dlg->CurPlayNum);
					Dlg->TCPServerSend(connectfd,sendBuf,strlen(sendBuf));
					recvTimeOutNum = 0;
				}
				if(strncmp(recv_buf,strAllPlayNum,strlen(strAllPlayNum)) == 0){

					memset(Dlg->VideoUrlList,0,1024*64);
					Dlg->InitVideoUrlList((CString)FusionVideoDir);
					memset(sendBuf,0,1024);
					sprintf(sendBuf,"TotalPlayNum:%d",Dlg->VideoUrlListNum);
					Dlg->TCPServerSend(connectfd,sendBuf,strlen(sendBuf));
					int i = 0;
					for(i = 0;i < Dlg->VideoUrlListNum +1;i++){
						int PlayNum = 0;
						if(sscanf(Dlg->VideoUrlList[i] + strlen(FusionVideoDir),"%d.",&PlayNum) > 0 && PlayNum > 0){
							memset(sendBuf,0,1024);
							sprintf(sendBuf,"PlayNum:%d",PlayNum);
							printf("%s  PlayNum == %d\n",Dlg->VideoUrlList[i],PlayNum);
							Dlg->TCPServerSend(connectfd,sendBuf,strlen(sendBuf));
						}
						Sleep(100);
					}
					
					for(i = 0;i < Dlg->VideoUrlListNum +1;i++){
						if(Dlg->CurPlayNum == 0){
							if(Dlg->ChangeBGPlayerUrl(Dlg->VideoUrlList[0]) == 0){
								Dlg->BGPlayerWaitEndFlag = 0;
								break;
							}
						}
					}
					recvTimeOutNum = 0;
				}
				
				if(strncmp(recv_buf,strChangeCurPlayNum,strlen(strChangeCurPlayNum)) == 0){
					int recvplaynum = 0;
					sscanf(recv_buf,"ChangeCurPlayNum:%d",&recvplaynum);
					int i = 0;
					for(i = 0;i < Dlg->VideoUrlListNum +1;i++){
						int PlayNum = 0;
						sscanf(Dlg->VideoUrlList[i] + strlen(FusionVideoDir),"%d.",&PlayNum);
						if(PlayNum == recvplaynum && recvplaynum != Dlg->CurPlayNum){
							int OldCurPlayNum = Dlg->CurPlayNum;
							Dlg->CurPlayNum = recvplaynum;
							if(Dlg->ChangeBGPlayerUrl(Dlg->VideoUrlList[i]) == 0){
								Dlg->BGPlayerWaitEndFlag = 0;
								Dlg->TCPServerSend(connectfd,"PlaySuccess",strlen("PlaySuccess"));
							}else{
								Dlg->CurPlayNum = OldCurPlayNum;
								Dlg->TCPServerSend(connectfd,"PlayFail",strlen("PlayFail"));
							}
							break;
						}
					}
					if(i == Dlg->VideoUrlListNum +1)
					{
						Dlg->TCPServerSend(connectfd,"PlayFail",strlen("PlayFail"));
					}
				}
			}else if(Ret < 0){
				recvTimeOutNum++;
			}else if(Ret == 0){
				recvTimeOutNum = 0;
				break;
			}
			if(recvTimeOutNum > 10)
				break;
		}
		Dlg->TCPServerCutConnect(&connectfd);
	}
EXIT:
	Dlg->TCPServerDeinit(&listenfd);
	return 0;
}

// CPanorama_PlayerDlg 消息处理程序

int AV_LOG_show  = 0;
void ffmpeg_log_callback(void *ptr, int level, const char *fmt, va_list vl)
{
	if(level <= AV_LOG_TRACE && AV_LOG_show)
		vfprintf(stdout, fmt, vl);
}

double i = 0;

Uint32 RenderPresent_callback(Uint32 interval, void *param)  
{
	SDL_Rect dstrect = {0,0,0,0};

	
	CPanorama_PlayerDlg * Dlg = (CPanorama_PlayerDlg *)param;
	SDL_RenderClear(Dlg->sdlRenderer);
	
	Dlg->BGplayer.UpdataUpdateTexture();
	Dlg->DshowPlayer.UpdataUpdateTexture();
	int i = 0;
	for(i = 0;i < WinPlayerNUM;i++){
		Dlg->WinPlayer[i].UpdataUpdateTexture();
	}
	//if(Dlg->sdlBGTexture){
	//	SDL_Rect rect = {(Dlg->VideoWin.DisPlayRect[0].right - Dlg->VideoWin.DisPlayRect[0].left)/4,0,Dlg->sdlBGSurface->w/2,Dlg->sdlBGSurface->h/2};
	//	SDL_RenderCopy(Dlg->sdlRenderer,Dlg->sdlBGTexture,0,&rect);
	//}
/*
	SDL_RenderCopy(Dlg->sdlRenderer,Dlg->myplayer.sdlTexture, 0,&Dlg->myplayer.TextureRect);
 
	SDL_RenderCopy(Dlg->sdlRenderer,Dlg->UsbCameraPlayer.sdlTexture, 0,&Dlg->UsbCameraPlayer.TextureRect);

	SDL_RenderCopy(Dlg->sdlRenderer,Dlg->NetPlayer.sdlTexture, 0,&Dlg->NetPlayer.TextureRect);
*/
	SDL_RenderPresent(Dlg->sdlRenderer);
	return interval;
}
int CPanorama_PlayerDlg::getDshowVideoDevice()
{
	//�����豸������Ƶ����	

	int Ret = 0;
	HRESULT hr;
	HRESULT hhr;
	int ret=0;
	int videoBusy=1;
	int audioBusy=1;

	CoInitialize(NULL);

	ICreateDevEnum* pSysDevEnum = NULL;

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pSysDevEnum);

	IEnumMoniker* pEnumCat ;

	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

	if(hr == S_OK)
	{
		IMoniker* pMoniker = NULL;
		IMoniker* pm1=NULL;
		ULONG cFetched;

		while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		{
			IPropertyBag* pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag);

			if(SUCCEEDED(hr))
			{

				VARIANT varName;
				varName.vt=VT_BSTR;

				VariantInit(&varName);

				hr = pPropBag->Read(L"FriendlyName", &varName, 0);

				USES_CONVERSION;
				LPSTR lpstrMsg = T2A((CString)varName.bstrVal);

				if(SUCCEEDED(hr))
				{
					if(DshowVideoName){
						free(DshowVideoName);
						DshowVideoName = NULL;
					}
					DshowVideoName = (char *)malloc(strlen(lpstrMsg)+1);
					memset(DshowVideoName,0,strlen(lpstrMsg)+1);
					memcpy(DshowVideoName,lpstrMsg,strlen(lpstrMsg));
 					printf("%s --> %s  %d\n",lpstrMsg,DshowVideoName,strlen(lpstrMsg));
					if(strstr(DshowVideoName,"HDMI") == NULL){
						Ret = 0;
						break;
						//pPropBag->Release();
						//pMoniker->Release();
						//return 0;
					}
				}

				pPropBag->Release();

			}
			pMoniker->Release();
		}
	}

	hr = pSysDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEnumCat, 0);

	if(hr == S_OK)
	{
		IMoniker* pMoniker = NULL;
		IMoniker* pm1=NULL;
		ULONG cFetched;

		while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		{
			IPropertyBag* pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag);

			if(SUCCEEDED(hr))
			{

				VARIANT varName;
				varName.vt=VT_BSTR;

				VariantInit(&varName);

				hr = pPropBag->Read(L"FriendlyName", &varName, 0);

				USES_CONVERSION;
				LPSTR lpstrMsg = T2A((CString)varName.bstrVal);

				if(SUCCEEDED(hr))
				{
					if(DshowAudioName){
						free(DshowAudioName);
						DshowAudioName = NULL;
					}
					DshowAudioName = (char *)malloc(strlen(lpstrMsg)+1);
					memset(DshowAudioName,0,strlen(lpstrMsg)+1);
					memcpy(DshowAudioName,lpstrMsg,strlen(lpstrMsg));
 					printf("%s --> %s  %d\n",lpstrMsg,DshowAudioName,strlen(lpstrMsg));
					break;
				}

				pPropBag->Release();

			}
			pMoniker->Release();
		}
	}

	
	return Ret;
}

BOOL CPanorama_PlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	//SetEdit( (CEdit*) GetDlgItem(IDC_EDIT1));
	

	//---------------------------------------------------
	::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,
	                                    SWP_HIDEWINDOW|SWP_NOACTIVATE|SWP_NOSIZE);
	MainDlg = this;
	
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_EVENTS)){
		printf("SDL_Init  %s\n",SDL_GetError());
	}
	if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG)){
		printf("IMG_Init  %s\n",SDL_GetError());
	}
	av_register_all();
	avcodec_register_all();
	avformat_network_init();
	avdevice_register_all();
	av_log_set_callback(ffmpeg_log_callback);

	//ListDshowDevice();
	RenderPresentTimerID = -1;
	StopShowLog = 1;
	pLogEdit = (CEdit*) GetDlgItem(IDC_EDIT1);
	CenterWindow(GetDesktopWindow());
	ShowWindow(SW_MINIMIZE);
	
	VideoWin.Create((LPCTSTR)IDD_DIALOG1,this);
	VideoWin.setWindowType(0);
	VideoWin.CreateScreen();

	/*SDL_Window * win = SDL_CreateWindow("SDL Application",
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				(VideoWin.DisPlayRect[0].right - VideoWin.DisPlayRect[0].left),
				(VideoWin.DisPlayRect[0].bottom - VideoWin.DisPlayRect[0].top),
				SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	*/
	//SDL_GLContext glContext = SDL_GL_CreateContext(VideoWin.screen);
	//char * err = ;
	//printf("error  %s\n",SDL_GetError());
	
	sdlRenderer = SDL_CreateRenderer(VideoWin.screen, -1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

	sdlBGTexture = NULL;
	sdlBGSurface = NULL;
	sdlBGSurface = IMG_Load("BG.png");
	if(sdlBGSurface){
		//SDL_SetSurfaceBlendMode(sdlBGSurface,SDL_BLENDMODE_NONE);
		sdlBGTexture =SDL_CreateTextureFromSurface(sdlRenderer,sdlBGSurface);
		//sdlBGTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,sdlBGSurface->w/4,sdlBGSurface->h/4); 
	}
	RenderPresentTimerID = SDL_AddTimer((1000/30),RenderPresent_callback,(void *)this);

	
	memset(VideoUrlList,0,1024*64);
	InitVideoUrlList((CString)FusionVideoDir);

	BGPlayerWaitEndFlag = 1;
	memset(BGPlayUrl,0,1024);
	CurPlayNum = 2;
	if(ChangeBGPlayerUrl(VideoUrlList[CurPlayNum]) == 0){
		BGPlayerWaitEndFlag = 0;
	}
	
	BGplayer.Init_Player(0,sdlRenderer);
	if(0){
		BGplayer.Open_Player(NULL);
		BGplayer.Play_Player();
	}else{
	    BGPlayThread = ::AfxBeginThread(BGPlayThreadFun,this);
	}
	
#if 1
	DshowPlayer.TextureRect.x=GetPrivateProfileInt((CString)"DshowPlayer",(CString)"Rect.x",0,(CString)".//Config.ini");
	DshowPlayer.TextureRect.y=GetPrivateProfileInt((CString)"DshowPlayer",(CString)"Rect.y",0,(CString)".//Config.ini");
	DshowPlayer.TextureRect.h=GetPrivateProfileInt((CString)"DshowPlayer",(CString)"Rect.h",0,(CString)".//Config.ini");
	DshowPlayer.TextureRect.w=GetPrivateProfileInt((CString)"DshowPlayer",(CString)"Rect.w",0,(CString)".//Config.ini");

	DshowPlayer.Init_Player(1,sdlRenderer);
	OpenDShow();
#endif

	int i = 0;
	char * WinPlayerstr[] = {"Win1Player","Win2Player","Win3Player","Win4Player"};
	for(i = 0;i < WinPlayerNUM;i++){
		WinPlayer[i].TextureRect.x=GetPrivateProfileInt((CString)WinPlayerstr[i],(CString)"Rect.x",0,(CString)".//Config.ini");
		WinPlayer[i].TextureRect.y=GetPrivateProfileInt((CString)WinPlayerstr[i],(CString)"Rect.y",0,(CString)".//Config.ini");
		WinPlayer[i].TextureRect.h=GetPrivateProfileInt((CString)WinPlayerstr[i],(CString)"Rect.h",0,(CString)".//Config.ini");
		WinPlayer[i].TextureRect.w=GetPrivateProfileInt((CString)WinPlayerstr[i],(CString)"Rect.w",0,(CString)".//Config.ini");		
		WinPlayer[i].Init_Player(i+2,sdlRenderer);
		if(0){
			memset(WinPlayerUrl[i],0,1024);
			memcpy(WinPlayerUrl[i],"rtmp://live.hkstv.hk.lxdns.com/live/hks",strlen("rtmp://live.hkstv.hk.lxdns.com/live/hks"));
			WinPlayer[i].Open_Player(WinPlayerUrl[i]);
			WinPlayer[i].Play_Player();
		}else{
			memset(WinPlayerUrl[i],0,1024);
			if(i==0)
				memcpy(WinPlayerUrl[i],"D:\\FVid\\xxx1.mpg",strlen("D:\\FVid\\xxx1.mpg"));
			if(i==1)
				memcpy(WinPlayerUrl[i],"D:\\FVid\\xxx2.mpg",strlen("D:\\FVid\\xxx2.mpg"));
			if(i==2)
				memcpy(WinPlayerUrl[i],"D:\\FVid\\xxx3.mpg",strlen("D:\\FVid\\xxx3.mpg"));
			::AfxBeginThread(WinPlayThreadFun,(void *)i);
		}
	}
	pSyncPlayThread = ::AfxBeginThread(SyncPlayThreadFun,this);
		
	//StartFtpSer(FTP_SER_USERNAME,FTP_SER_PASSWORD,FTP_SER_PATH);
	pLogEdit = (CEdit*) GetDlgItem(IDC_EDIT1);

	TrayMessage(m_hWnd, NIM_ADD, NULL, L"");   
	TrayMessage(m_hWnd, NIM_MODIFY, m_hIcon, L"全景播放器"); 
	ShowWindow(SW_HIDE);
		
	//VideoWin.ModifyStyle(WS_CAPTION,0);

//-----------set button EDIT--------------------

	CString str ;
	if(DshowPlayer.PlayerIsPlay){
		str.Format(_T("%d"), DshowPlayer.TextureRect.x);
		m_DShowX.SetWindowText(str);

		str.Format(_T("%d"), DshowPlayer.TextureRect.y);
		m_DShowY.SetWindowText(str);

		str.Format(_T("%d"), DshowPlayer.TextureRect.h);
		m_DShowH.SetWindowText(str);

		str.Format(_T("%d"), DshowPlayer.TextureRect.w);
		m_DShowW.SetWindowText(str);
		m_DShow_H_W.EnableWindow(FALSE);
	}else{
		m_DShow_ShowHide.EnableWindow(FALSE);
		m_DShowX.EnableWindow(FALSE);
		m_DShowY.EnableWindow(FALSE);
		m_DShowH.EnableWindow(FALSE);
		m_DShowW.EnableWindow(FALSE);
		m_DShow_SaveCfg.EnableWindow(FALSE);
		m_DShow_H_W.EnableWindow(FALSE);
	}

	if(DshowPlayer.isShow){
		m_DShow_ShowHide.SetWindowText(_T("显示"));
	}else{
		m_DShow_ShowHide.SetWindowText(_T("隐藏"));
	}
	
	CButton * buttonShowHide[4] = {
		&m_Win1ShowHide,&m_Win2ShowHide,&m_Win3ShowHide,&m_Win4ShowHide
	};
	CButton * buttonSaveCfg[4] = {
		&m_Win1SaveCfg,&m_Win2SaveCfg,&m_Win3SaveCfg,&m_Win4SaveCfg
	};
	CEdit * EditX[4] = {
		&m_Win1X,&m_Win2X,&m_Win3X,&m_Win4X
	};
	CEdit * EditY[4] = {
		&m_Win1Y,&m_Win2Y,&m_Win3Y,&m_Win4Y
	};
	CEdit * EditH[4] = {
		&m_Win1H,&m_Win2H,&m_Win3H,&m_Win4H
	};
	CEdit * EditW[4] = {
		&m_Win1W,&m_Win2W,&m_Win3W,&m_Win4W
	};
	CEdit * DefultHW[] = {&m_Video1_H_W,&m_Video2_H_W,
						&m_Video3_H_W,&m_Video4_H_W};
	for(i = 0;i < 4;i++){
		if(i < WinPlayerNUM && WinPlayer[i].PlayerIsInit){
			if(!WinPlayer[i].isShow){
				buttonShowHide[i]->SetWindowText(_T("显示"));
			}else{
				buttonShowHide[i]->SetWindowText(_T("隐藏"));
			}
			
			buttonShowHide[i]->EnableWindow(TRUE);
			EditX[i]->EnableWindow(TRUE);
			EditY[i]->EnableWindow(TRUE);
			EditH[i]->EnableWindow(TRUE);
			EditW[i]->EnableWindow(TRUE);
			buttonSaveCfg[i]->EnableWindow(TRUE);
			
			str.Format(_T("%d"), WinPlayer[i].TextureRect.x);
			EditX[i]->SetWindowText(str);
			str.Format(_T("%d"), WinPlayer[i].TextureRect.y);
			EditY[i]->SetWindowText(str);
			str.Format(_T("%d"), WinPlayer[i].TextureRect.h);
			EditH[i]->SetWindowText(str);
			str.Format(_T("%d"), WinPlayer[i].TextureRect.w);
			EditW[i]->SetWindowText(str);
			
			DefultHW[i]->EnableWindow(FALSE);
		}else{
			buttonShowHide[i]->EnableWindow(FALSE);
			EditX[i]->EnableWindow(FALSE);
			EditY[i]->EnableWindow(FALSE);
			EditH[i]->EnableWindow(FALSE);
			EditW[i]->EnableWindow(FALSE);
			buttonSaveCfg[i]->EnableWindow(FALSE);
			
			DefultHW[i]->EnableWindow(FALSE);
		}
	}
	
	class UDPMessage myMSG;
	//CRect rect;
	//m_INFO_EDIT.SetWindowText((CString)"IP:"+(CString)myMSG.getourIP()
	//+ str.Format(_T("%d"),BGplayer.screen_h)+str.Format(_T("%d"),BGplayer.screen_w));
	char infobuffer[1024] = {0};
	sprintf(infobuffer,"IP:%s         宽度：%d  高度：%d",myMSG.getourIP(),BGplayer.screen_w,BGplayer.screen_h);
	m_INFO_EDIT.SetWindowText((CString)infobuffer);
	m_INFO_EDIT.EnableWindow(FALSE);
	return TRUE;
}
/*
void CPanorama_PlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
*/

void CPanorama_PlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX){
		CAboutDlg dlgAbout;   
		dlgAbout.DoModal(); 
	}	   
	//��Ӵ�����С��ʱ�Ĵ����¼�   
	else if (nID == SC_MINIMIZE){ 
		TrayMessage(m_hWnd, NIM_ADD, NULL, L"");   
		TrayMessage(m_hWnd, NIM_MODIFY, m_hIcon, L"全景播放器"); 	  
		ShowWindow(SW_HIDE);
	}  else  { 
		CDialogEx::OnSysCommand(nID, lParam); 
	}
} 

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPanorama_PlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPanorama_PlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#ifdef CFTPSERVER_ENABLE_EVENTS

void OnServerEvent( int Event )
{
	switch( Event )
	{
		case CFtpServer::START_LISTENING:
			printf("* Server is listening !");
			break;

		case CFtpServer::START_ACCEPTING:
			printf("* Server is accepting incoming connexions !");
			break;

		case CFtpServer::STOP_LISTENING:
			printf("* Server stopped listening !");
			break;

		case CFtpServer::STOP_ACCEPTING:
			printf("* Server stopped accepting incoming connexions !");
			break;
		case CFtpServer::MEM_ERROR:
			printf("* Warning, the CFtpServer class could not allocate memory !");
			break;
		case CFtpServer::THREAD_ERROR:
			printf("* Warning, the CFtpServer class could not create a thread !");
			break;
		case CFtpServer::ZLIB_VERSION_ERROR:
			printf("* Warning, the Zlib header version differs from the Zlib library version !");
			break;
		case CFtpServer::ZLIB_STREAM_ERROR:
			printf("* Warning, error during compressing/decompressing data !");
			break;
	}
}

void OnUserEvent( int Event, CFtpServer::CUserEntry *pUser, void *pArg )
{	
	char debugLog[OutputLogLen] = {0};
	switch( Event )
	{
		case CFtpServer::NEW_USER:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog,"* A new user has been created:\r\n"
					"\tLogin: %s\r\n" "\tPassword: %s\r\n" "\tStart directory: %s",
				pUser->GetLogin(), pUser->GetPassword(), pUser->GetStartDirectory() );
			printf(debugLog);
			break;

		case CFtpServer::DELETE_USER:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog,"* \"%s\"user is being deleted: \r\n", pUser->GetLogin() );
			printf(debugLog);
			break;
	}
}

void OnClientEvent( int Event, CFtpServer::CClientEntry *pClient, void *pArg )
{
	char debugLog[OutputLogLen] = {0};
	switch( Event )
	{
		case CFtpServer::NEW_CLIENT:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog,"* A new client has been created:\r\n"
				"\tClient IP: [%s]\r\n\tServer IP: [%s]\r\n",
				inet_ntoa( *pClient->GetIP() ), inet_ntoa( *pClient->GetServerIP() ));
				printf(debugLog);
			break;

		case CFtpServer::DELETE_CLIENT:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog, "* A client is being deleted.\r\n" );
			printf(debugLog);
			break;

		case CFtpServer::CLIENT_AUTH:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog, "* A client has logged-in as \"%s\".\r\n", pClient->GetUser()->GetLogin() );
			printf(debugLog);
			break;

		case CFtpServer::CLIENT_SOFTWARE:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog,"* A client has proceed the CLNT FTP command: %s.\r\n", (char*) pArg );
			printf(debugLog);
			break;

		case CFtpServer::CLIENT_DISCONNECT:
			printf( "* A client has disconnected.\r\n" );
			break;

		case CFtpServer::CLIENT_UPLOAD:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog,"* A client logged-on as \"%s\" is uploading a file: \"%s\"\r\n",
				pClient->GetUser()->GetLogin(), (char*)pArg );
			printf(debugLog);
			break;

		case CFtpServer::CLIENT_DOWNLOAD:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog,"* A client logged-on as \"%s\" is downloading a file: \"%s\"\r\n",
				pClient->GetUser()->GetLogin(), (char*)pArg );
			printf(debugLog);
			break;

		case CFtpServer::CLIENT_LIST:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog, "* A client logged-on as \"%s\" is listing a directory: \"%s\"\r\n",
				pClient->GetUser()->GetLogin(), (char*)pArg );
			printf(debugLog);
			break;

		case CFtpServer::CLIENT_CHANGE_DIR:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog, "* A client logged-on as \"%s\" has changed its working directory:\r\n"
				"\tFull path: \"%s\"\r\n\tWorking directory: \"%s\"\r\n",
				pClient->GetUser()->GetLogin(), (char*)pArg, pClient->GetWorkingDirectory() );
			printf(debugLog);
			break;

		case CFtpServer::RECVD_CMD_LINE:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog, "* Received: %s (%s)>  %s\r\n",
				pClient->GetUser() ? pClient->GetUser()->GetLogin() : "(Not logged in)",
				inet_ntoa( *pClient->GetIP() ),
				(char*) pArg );
			printf(debugLog);
			break;
		
		case CFtpServer::SEND_REPLY:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog,"* Sent: %s (%s)>  %s\r\n",
				pClient->GetUser() ? pClient->GetUser()->GetLogin() : "(Not logged in)",
				inet_ntoa( *pClient->GetIP() ),
				(char*) pArg );
			printf(debugLog);
			break;

		case CFtpServer::TOO_MANY_PASS_TRIES:
			memset(debugLog,0,OutputLogLen);
			sprintf(debugLog,"* Too many pass tries for (%s)\r\n",
				inet_ntoa( *pClient->GetIP() ) );
			printf(debugLog);
			break;
	}
}

#endif // #ifdef CFTPSERVER_ENABLE_EVENTS


int CPanorama_PlayerDlg::StartFtpSer(const char * username,const char * password,const char *tfpserPath)
{
	//#ifdef CFTPSERVER_ENABLE_EVENTS
		FtpServer.SetServerCallback( OnServerEvent );
		FtpServer.SetUserCallback(  OnUserEvent );
		FtpServer.SetClientCallback(  OnClientEvent );
	//#endif

	FtpServer.SetMaxPasswordTries( 3 );
	FtpServer.SetNoLoginTimeout( 45 ); // seconds
	FtpServer.SetNoTransferTimeout( 90 ); // seconds
	FtpServer.SetDataPortRange( 10, 900 ); // data TCP-Port range = [100-999]
	FtpServer.SetCheckPassDelay( 500 ); // milliseconds. Bruteforcing protection.
	

	if(FtpServer.IsListening()){
		return 0;
	}

	pUser = FtpServer.AddUser( username, password, tfpserPath);
	pAnonymousUser = FtpServer.AddUser("anonymous", NULL, tfpserPath );

	if( pAnonymousUser ) {
	
		OutputLog( "-Anonymous user successfully created." );
		pAnonymousUser->SetPrivileges( CFtpServer::LIST | CFtpServer::READFILE);
	} else{
		OutputLog( "-Can't create anonymous user." );
		StopFtpSer();
		return -1;
	}

	if(pUser){
		pUser->SetMaxNumberOfClient( 0 ); // Unlimited

		pUser->SetPrivileges(	CFtpServer::READFILE | CFtpServer::WRITEFILE |
								CFtpServer::LIST | CFtpServer::DELETEFILE | CFtpServer::CREATEDIR |
								CFtpServer::DELETEDIR );
			class UDPMessage myMSG;
			if( FtpServer.StartListening( inet_addr(myMSG.getourIP()), 21) ){
				if( FtpServer.StartAccepting()) {
					return 0;
				} else{
					StopFtpSer();
					return -1;
				}
			}else{
				StopFtpSer();
				return -1;
			}
	}else{
		StopFtpSer();
		return -1;
	}
}
int CPanorama_PlayerDlg::StopFtpSer()
{
	if(FtpServer.IsListening()){
		FtpServer.StopListening();
	}
	if(pUser){
		FtpServer.DeleteUser(pUser);
		pAnonymousUser = NULL;
	}

	if(pAnonymousUser){
		FtpServer.DeleteUser(pAnonymousUser);
		pAnonymousUser = NULL;
	}

	return 0;
}

void CPanorama_PlayerDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
//	char * file_path = NULL;	
	LPSTR file_path = NULL;
	CString m_sourceFile;
	int len = 0;

#if 1
	//VideoWin.setWindowSize(0,0);
	CString szFilter = _T("All Files (*.*)|*.*|avi Files (*.avi)|*.avi|rmvb Files (*.rmvb)|*.rmvb|3gp Files (*.3gp)|*.3gp|mp3 Files (*.mp3)|*.mp3|mp4 Files (*.mp4)|*.mp4|mpeg Files (*.ts)|*.ts|flv Files (*.flv)|*.flv|mov Files (*.mov)|*.mov||");
	CFileDialog dlg(TRUE,NULL ,NULL,OFN_PATHMUSTEXIST|OFN_HIDEREADONLY ,szFilter,NULL);
	if (IDOK == dlg.DoModal()){
		m_sourceFile = dlg.GetPathName();
		USES_CONVERSION;
		file_path = T2A(m_sourceFile);
	}else{
		return;
	}
	if(ChangeBGPlayerUrl(file_path) == 0){
//	if(ChangeSinglePlayUrl("rtmp://live.hkstv.hk.lxdns.com/live/hks") == 0){
		BGPlayerWaitEndFlag = 0;
	}
#else
	
	myplayer.Stop_Player();
	myplayer.Close_Player();
	//myplayer.Open_Player(file_path);

//	myplayer.Open_Player("F:/video/3tai.mp4");
//	myplayer.Open_Player("F:/video/4K.avi");
//	myplayer.Open_Player("F:/video/bigbuckbunny_480x272.h265");
	myplayer.Open_Player("F:/video/7024162.mpg");
	myplayer.Play_Player();
	myplayer.DisPlay_Player();
	
	while(0){
		if(myplayer.play_end)
			break;
			Sleep(1000);
	}
	Sleep(1000 * 2);
#endif	
}
void CPanorama_PlayerDlg::OnBnClickedButton4()
{
	VideoWin.ShowWindow(SW_SHOWNORMAL);;
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CPanorama_PlayerDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(RenderPresentTimerID >0){
		SDL_RemoveTimer(RenderPresentTimerID);
		RenderPresentTimerID = -1;
	}
	BGplayer.Stop_Player();
	BGplayer.Close_Player();
	BGplayer.Deinit_Player();
	
	DshowPlayer.Stop_Player();
	DshowPlayer.Close_Player();
	DshowPlayer.Deinit_Player();

	int i = 0;
	for(i = 0;i < WinPlayerNUM;i++){
		WinPlayer[i].Stop_Player();
		WinPlayer[i].Close_Player();
		WinPlayer[i].Deinit_Player();
	}
	
	if(sdlRenderer){
		SDL_DestroyRenderer(sdlRenderer);
		sdlRenderer = NULL;
	}
	VideoWin.DestroyScreen();

	avformat_network_deinit();
	IMG_Quit();
	SDL_Quit();
	return CDialogEx::DestroyWindow();
}

BOOL TrayMessage(HWND hWnd, DWORD dwMessage, HICON hIcon, LPCWSTR pszTip)
{
	BOOL bReturn; 
	NOTIFYICONDATA NofityData;
	NofityData.cbSize = (DWORD)sizeof(NOTIFYICONDATA); 
	NofityData.hWnd = hWnd; 
	NofityData.uID = IDR_MAINFRAME; 
	NofityData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;  
	NofityData.uCallbackMessage = WM_TRAY_NOTIFICATION;  
	NofityData.hIcon = hIcon; 
	lstrcpyn(NofityData.szTip, pszTip, sizeof(NofityData.szTip));  
	bReturn = Shell_NotifyIcon(dwMessage, &NofityData); 
	if (hIcon)   
		DestroyIcon(hIcon);  
	return bReturn; 
}

afx_msg LRESULT CPanorama_PlayerDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)  
	{	
		case WM_RBUTTONUP:	
		{	
			LPPOINT lpoint = new tagPOINT;	 
			::GetCursorPos(lpoint);   
			CMenu Menu;  
			Menu.CreatePopupMenu();  
			Menu.AppendMenuW(MF_STRING, WM_DESTROY, L"��������"); 
			Menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this); 
			SetForegroundWindow();	  
			HMENU hmenu = Menu.Detach(); 
			delete lpoint;	
		}
		break;	 
		case WM_LBUTTONDBLCLK:	
			{  
				ShowWindow(SW_SHOW);   
				TrayMessage(m_hWnd, NIM_DELETE, NULL, L"");   
			}
		break; 
	}  
	return 0; 
}

LRESULT CPanorama_PlayerDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int ret = CDialogEx::OnNcHitTest(point);
	if(HTTOP == ret||HTBOTTOM == ret||HTLEFT == ret || HTRIGHT == ret 
	|| HTBOTTOMLEFT == ret || HTBOTTOMRIGHT == ret || HTTOPLEFT == ret || HTTOPRIGHT == ret
	) 
        return HTCLIENT; 

	return ret;
}

int CPanorama_PlayerDlg::ShowTimeToEdit()
{
	ShowLoglock.Lock();
	if(StopShowLog){
		ShowLoglock.Unlock();
		return 0;
	}
	CTime  time = CTime::GetCurrentTime();
	CString timeStr = time.Format(_T("【%H:%M:%S】"));

	int l=pLogEdit->GetWindowTextLength();
    pLogEdit->SetSel(l,l,FALSE);
    pLogEdit->SetFocus();


	//pLogEdit->SetSel(-1,-1);
	pLogEdit->ReplaceSel(timeStr);
	ShowLoglock.Unlock();
	return 0;
}
int  CPanorama_PlayerDlg::ShowLogToEdit(char * str,int len)
{	
	ShowLoglock.Lock();
	if(StopShowLog){
		ShowLoglock.Unlock();
		return 0;
	}
	static int strShowNum = 0;
	if(strShowNum > 1024*1024*2){
		pLogEdit->SetWindowText( (CString)"");
		pLogEdit->Clear();
	}
	int i = 0;
	for(i = 0;i < len;i++){
		int l=pLogEdit->GetWindowTextLength();
		pLogEdit->SetSel(l,l,FALSE);
		pLogEdit->SetFocus();
		if(str[i] != 0){
			pLogEdit->ReplaceSel((CString)str[i]);
			strShowNum++;
		}
	}
	ShowLoglock.Unlock();
	return 0;
}
int  CPanorama_PlayerDlg::LogEditEnt()
{
	ShowLoglock.Lock();
	if(StopShowLog){
		ShowLoglock.Unlock();
		return 0;
	}
	int l=pLogEdit->GetWindowTextLength();
    pLogEdit->SetSel(l,l,FALSE);
    pLogEdit->SetFocus();
	pLogEdit->ReplaceSel((CString)"\r\n");
	ShowLoglock.Unlock();
	return 0;
}


void CPanorama_PlayerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	pLogEdit->SetWindowText( (CString)"");
	pLogEdit->Clear();
}


void CPanorama_PlayerDlg::OnBnClickedButton2()
{
	return;
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	if(StopShowLog == 1){
		m_CtrLogButton.SetWindowText(_T("暂停显示日志"));
		StopShowLog = 0;
		ShowLogToEdit("#Resume Show Log#",strlen("#Resume Show Log#"));
		LogEditEnt();
	}else{	
		m_CtrLogButton.SetWindowText(_T("开启显示日志"));
		ShowLogToEdit("#Pause Show Log#",strlen("#Pause Show Log#"));
		LogEditEnt();
		StopShowLog = 1;
	}
}


//��ʾ���Ŵ���
void CPanorama_PlayerDlg::DisPlay_Player(void)
{
	if(VideoWin){
		VideoWin.ShowWindow(SW_SHOWNORMAL);
	}
}
//���ز��Ŵ���
void CPanorama_PlayerDlg::Hide_Player(void)
{
	if(VideoWin)
		VideoWin.ShowWindow(SW_HIDE);
}


//INT_PTR CPanorama_PlayerDlg::DoModal()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	return CDialogEx::DoModal();
//}


void CPanorama_PlayerDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	::WritePrivateProfileString((CString)"Main",(CString)"Versions",(CString)"V1.0",(CString)".//Config.ini");
}



void CPanorama_PlayerDlg::WinOpenFun(int num)
{
	// TODO: 在此添加控件通知处理程序代码

	if(WinPlayerNUM <= num)
		return;

	LPSTR file_path = NULL;
	CString m_sourceFile;
	CString szFilter = _T("All Files (*.*)|*.*|avi Files (*.avi)|*.avi|rmvb Files (*.rmvb)|*.rmvb|3gp Files (*.3gp)|*.3gp|mp3 Files (*.mp3)|*.mp3|mp4 Files (*.mp4)|*.mp4|mpeg Files (*.ts)|*.ts|flv Files (*.flv)|*.flv|mov Files (*.mov)|*.mov||");
	CFileDialog dlg(TRUE,NULL ,NULL,OFN_PATHMUSTEXIST|OFN_HIDEREADONLY ,szFilter,NULL);
	if (IDOK == dlg.DoModal()){
		m_sourceFile = dlg.GetPathName();
		USES_CONVERSION;
		file_path = T2A(m_sourceFile);
	}else{
		return;
	}
	
	ChangeWinPlayerUrl(file_path,num);
	WinPlayerWaitEndFlag[num] = 0;

	if(!WinPlayer[num].isShow){
		WinShowHide(num);
	}
}

void CPanorama_PlayerDlg::OnBnClickedButton7()
{
	WinOpenFun(0);
}
void CPanorama_PlayerDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	WinOpenFun(1);
}

void CPanorama_PlayerDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	WinOpenFun(2);
}

void CPanorama_PlayerDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	WinOpenFun(3);
}

void CPanorama_PlayerDlg::WinShowHide(int num)
{
	if(WinPlayerNUM <= num)
		return;

	CButton * button = NULL;
	switch(num)
	{
		case 0:button = &m_Win1ShowHide;break;
		case 1:button = &m_Win2ShowHide;break;
		case 2:button = &m_Win3ShowHide;break;
		case 3:button = &m_Win4ShowHide;break;
		default:
			return;
	}

	if(WinPlayer[num].isShow){
		WinPlayer[num].HideWindow();
		button->SetWindowText(_T("显示"));
	}else{
		WinPlayer[num].ShowWindow();
		button->SetWindowText(_T("隐藏"));
	}
	
}

void CPanorama_PlayerDlg::OnBnClickedButton19()
{
	// TODO: 在此添加控件通知处理程序代码
	WinShowHide(0);
}

void CPanorama_PlayerDlg::OnBnClickedButton20()
{
	// TODO: 在此添加控件通知处理程序代码	
	
	WinShowHide(1);
}

void CPanorama_PlayerDlg::OnBnClickedButton21()
{
	// TODO: 在此添加控件通知处理程序代码

	WinShowHide(2);
}

void CPanorama_PlayerDlg::OnBnClickedButton22()
{
	// TODO: 在此添加控件通知处理程序代码

	WinShowHide(3);
}

void CPanorama_PlayerDlg::OnBnClickedButton23()
{
	// TODO: 在此添加控件通知处理程序代码
	if(DshowPlayer.isShow){
		DshowPlayer.HideWindow();
		m_DShow_ShowHide.SetWindowText(_T("显示"));
	}else{
		DshowPlayer.ShowWindow();
		m_DShow_ShowHide.SetWindowText(_T("隐藏"));
	}
}

void CPanorama_PlayerDlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	SDL_Rect rect = {0,0,0,0};

    m_Win1X.GetWindowText(str);
	rect.x= _ttoi(str);

    m_Win1Y.GetWindowText(str);
	rect.y= _ttoi(str);
	
	m_Win1H.GetWindowText(str);
	rect.h= _ttoi(str);
	
	m_Win1W.GetWindowText(str);
	rect.w= _ttoi(str);
	
	WinPlayer[0].ChargeShowRect(rect);
}

void CPanorama_PlayerDlg::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	SDL_Rect rect = {0,0,0,0};

    m_Win2X.GetWindowText(str);
	rect.x= _ttoi(str);

    m_Win2Y.GetWindowText(str);
	rect.y= _ttoi(str);
	
	m_Win2H.GetWindowText(str);
	rect.h= _ttoi(str);
	
	m_Win2W.GetWindowText(str);
	rect.w= _ttoi(str);
	
	WinPlayer[1].ChargeShowRect(rect);
}


void CPanorama_PlayerDlg::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	SDL_Rect rect = {0,0,0,0};

    m_Win3X.GetWindowText(str);
	rect.x= _ttoi(str);

    m_Win3Y.GetWindowText(str);
	rect.y= _ttoi(str);
	
	m_Win3H.GetWindowText(str);
	rect.h= _ttoi(str);
	
	m_Win3W.GetWindowText(str);
	rect.w= _ttoi(str);
	
	WinPlayer[2].ChargeShowRect(rect);
}


void CPanorama_PlayerDlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	SDL_Rect rect = {0,0,0,0};

    m_Win4X.GetWindowText(str);
	rect.x= _ttoi(str);

    m_Win4Y.GetWindowText(str);
	rect.y= _ttoi(str);
	
	m_Win4H.GetWindowText(str);
	rect.h= _ttoi(str);
	
	m_Win4W.GetWindowText(str);
	rect.w= _ttoi(str);
	
	WinPlayer[3].ChargeShowRect(rect);
}


void CPanorama_PlayerDlg::OnBnClickedButton18()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	SDL_Rect rect = {0,0,0,0};

    m_DShowX.GetWindowText(str);
	rect.x= _ttoi(str);

   m_DShowY.GetWindowText(str);
	rect.y= _ttoi(str);
	
	m_DShowH.GetWindowText(str);
	rect.h= _ttoi(str);
	
	m_DShowW.GetWindowText(str);
	rect.w= _ttoi(str);
	
	DshowPlayer.ChargeShowRect(rect);
}


BOOL CPanorama_PlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
		// TODO: 在此添加专用代码和/或调用基类
		//指定对话框只接受数字按键输入，其他符号输入无效
	
	//获取控件窗口指针
   /* if(	(GetFocus() == &m_Win1X ||GetFocus() == &m_Win1Y) && (pMsg->message == WM_CHAR)  )
    {
		//只允许输入数字和小数点“.”
		if((pMsg->wParam <= '9' && pMsg->wParam >= '0') || pMsg->wParam == '.') 
		{
			return 0;
		}
	}*/
	  if((WM_KEYDOWN == pMsg->message) &&1
		  /*||GetFocus() == &m_Win1X ||GetFocus() == &m_Win1Y||GetFocus() == &m_Win1W ||GetFocus() == &m_Win1H
		  ||GetFocus() == &m_Win2X ||GetFocus() == &m_Win2Y||GetFocus() == &m_Win2W ||GetFocus() == &m_Win2H
		  ||GetFocus() == &m_Win3X ||GetFocus() == &m_Win3Y||GetFocus() == &m_Win3W ||GetFocus() == &m_Win3H
		  ||GetFocus() == &m_Win4X ||GetFocus() == &m_Win4Y||GetFocus() == &m_Win4W ||GetFocus() == &m_Win4H
	  	  ||GetFocus() == &m_DShowX ||GetFocus() == &m_DShowY||GetFocus() == &m_DShowW ||GetFocus() == &m_DShowH
		  */
	  )
    {
        if((pMsg->wParam >= _T('0') && pMsg->wParam <= _T('9'))
            || ((pMsg->wParam >= VK_NUMPAD0) && (pMsg->wParam <= VK_NUMPAD9))
             || (_T('*') == pMsg->wParam) || (_T('#') == pMsg->wParam)
             || (VK_LEFT == pMsg->wParam) || (VK_RIGHT == pMsg->wParam)
             || (VK_UP == pMsg->wParam) || (VK_DOWN == pMsg->wParam)
             || (VK_DELETE == pMsg->wParam) || (VK_BACK == pMsg->wParam))
        {
            return CDialog::PreTranslateMessage(pMsg);
        }
        else
        {
            return TRUE;
        }
    }

	return CDialogEx::PreTranslateMessage(pMsg);
}

