
// Panorama_Player.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPanorama_PlayerApp:
// �йش����ʵ�֣������ Panorama_Player.cpp
//

class CPanorama_PlayerApp : public CWinApp
{
public:
	CPanorama_PlayerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPanorama_PlayerApp theApp;