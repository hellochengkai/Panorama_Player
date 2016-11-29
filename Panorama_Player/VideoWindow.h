#pragma once

extern "C"
{
#include "SDL.h"
#include "SDL_thread.h"
}

// VideoWindow 对话框

class VideoWindow : public CDialogEx
{
	DECLARE_DYNAMIC(VideoWindow)

public:
	VideoWindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~VideoWindow();
	HDC hDC; 
// 对话框数据
	enum { IDD = IDD_DIALOG1 };
	SDL_Window * screen;
	int windowIsCreate;
	int DisPlaynum;//当前屏幕的个数
	RECT DisPlayRect[3];//每个屏幕的尺寸
	int WindowShowType;
	void CreateScreen();
	void DestroyScreen();
	SDL_Window * GetScreen();
	void getScreenRect(CRect *rect);
	void setWindowType(int type);//0：在第一屏小屏显示 1:第二屏全屏显示（有第二屏的话） 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
