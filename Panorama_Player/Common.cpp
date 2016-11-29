#include <stdarg.h>
#include <stdio.h>
#include <afxinet.h>

#include "stdafx.h"
#include "Common.h"
#include "resource.h"

char * CString_to_charX(CString str)
{
	return (char *)str.GetBuffer(0);
}
void OutputLog(char * msg)
{
	if(!msg){
		return;
	}
	CString log_msg = (CString)msg;
    try
    {
        //�����ļ��Ĵ򿪲���
        CStdioFile outFile((LPCTSTR)_T(LOG_FILE), CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
        CString msLine;
        CTime tt = CTime::GetCurrentTime();

        //��ΪLog�ļ�������Ҫ��ÿ��Log��ʱ�����ʱ���ʽ�����ɶ��壬
        //����ĸ�ʽ�磺2010-June-10 Thursday, 15:58:12
        msLine = tt.Format("[%Y-%B-%d %A, %H:%M:%S] ") + log_msg + (CString)"\n";
        
        //���ļ�ĩβ�����¼�¼
        outFile.SeekToEnd();
        outFile.WriteString((msLine));
        outFile.Close();
    }
    catch(CFileException *fx)
    {
        fx->Delete();
    }
}
/*
INT HTTPDownLoadFile(const CString strUrl,const CString strSavePath)
{
	//��鴫�����������
	if (strUrl.IsEmpty())
		return -5;
	if (strSavePath.IsEmpty())
		return -6;

	unsigned short nPort;		//���ڱ���Ŀ��HTTP����˿�
	CString strServer, strObject;	//strServer���ڱ����������ַ��strObject���ڱ����ļ���������
	DWORD dwServiceType,dwRet;		//dwServiceType���ڱ���������ͣ�dwRet���ڱ����ύGET���󷵻ص�״̬��

	//����URL����ȡ��Ϣ
	if(!AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
	{
		//����ʧ�ܣ���Url����ȷ
		return -1;
	}
	//�����������Ӷ���HTTP���Ӷ���ָ������ڸ����ӵ�HttpFile�ļ�����ָ�룬ע��delete
	CInternetSession intsess;
	CHttpFile *pHtFile = NULL;
	//������������
	CHttpConnection *pHtCon = intsess.GetHttpConnection(strServer,nPort);
	if(pHtCon == NULL)
	{
		//������������ʧ��
		intsess.Close();
		return -2;
	}
	//����GET����
	pHtFile = pHtCon->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject);
	if(pHtFile == NULL)
	{
		//����GET����ʧ��
		intsess.Close();
		delete pHtCon;pHtCon = NULL;
		return -3;
	}
	//�ύ����
	pHtFile->SendRequest();
	//��ȡ���������ص�״̬��
	pHtFile->QueryInfoStatusCode(dwRet);
	if (dwRet != HTTP_STATUS_OK)
	{
		//����������������
		intsess.Close();
		delete pHtCon;pHtCon = NULL;
		delete pHtFile;pHtFile = NULL;
		return -4;
	}
	//��ȡ�ļ���С
	UINT nFileLen = (UINT)pHtFile->GetLength();
	DWORD dwRead = 1;		//���ڱ�ʶ���˶��٣�Ϊ1��Ϊ�˽���ѭ��
	//����������
	CHAR *szBuffer = new CHAR[nFileLen+1];
	TRY 
	{
		//�����ļ�
		CFile PicFile(strSavePath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);
		while(dwRead>0)
		{  
			//��ջ�����
			memset(szBuffer,0,(size_t)(nFileLen+1));
			//��ȡ��������
			dwRead = pHtFile->Read(szBuffer,nFileLen); 
			//д�뵽�ļ�
			PicFile.Write(szBuffer,dwRead);
		}
		//�ر��ļ�
		PicFile.Close();
		//�ͷ��ڴ�
		delete []szBuffer;
		delete pHtFile;
		delete pHtCon;
		//�ر���������
		intsess.Close();
	}
	CATCH(CFileException,e)
	{
		//�ͷ��ڴ�
		delete []szBuffer;
		delete pHtFile;
		delete pHtCon;
		//�ر���������
		intsess.Close();
		return -7;			//��д�ļ��쳣
	}
	END_CATCH
		return 0;
}
*/
/*

void CBootFusionPlayerDlg::OnBnClickedButton3()
{
	CString url = (CString)"http://www.mplayerhq.hu/MPlayer/releases/MPlayer-1.3.0.tar.xz";
	CString path = (CString)"D:\\FVid\\MPlayer-1.3.0.tar.xz";
	bool a = HTTPDownLoadFile(url,path);
	if(a == false){
		printf("http down error\n");
	}
}
*/