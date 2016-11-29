#pragma once

extern "C"
{
#include "SDL.h"
#include "SDL_thread.h"
}

// VideoWindow �Ի���

class VideoWindow : public CDialogEx
{
	DECLARE_DYNAMIC(VideoWindow)

public:
	VideoWindow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~VideoWindow();
	HDC hDC; 
// �Ի�������
	enum { IDD = IDD_DIALOG1 };
	SDL_Window * screen;
	int windowIsCreate;
	int DisPlaynum;//��ǰ��Ļ�ĸ���
	RECT DisPlayRect[3];//ÿ����Ļ�ĳߴ�
	int WindowShowType;
	void CreateScreen();
	void DestroyScreen();
	SDL_Window * GetScreen();
	void getScreenRect(CRect *rect);
	void setWindowType(int type);//0���ڵ�һ��С����ʾ 1:�ڶ���ȫ����ʾ���еڶ����Ļ��� 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnStnClickedStatic2();
	CStatic m_ShowPic;
//	virtual BOOL OnInitDialog();
};
