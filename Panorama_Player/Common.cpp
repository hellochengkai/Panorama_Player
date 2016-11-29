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
        //设置文件的打开参数
        CStdioFile outFile((LPCTSTR)_T(LOG_FILE), CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
        CString msLine;
        CTime tt = CTime::GetCurrentTime();

        //作为Log文件，经常要给每条Log打时间戳，时间格式可自由定义，
        //这里的格式如：2010-June-10 Thursday, 15:58:12
        msLine = tt.Format("[%Y-%B-%d %A, %H:%M:%S] ") + log_msg + (CString)"\n";
        
        //在文件末尾插入新纪录
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
	//检查传入的两个参数
	if (strUrl.IsEmpty())
		return -5;
	if (strSavePath.IsEmpty())
		return -6;

	unsigned short nPort;		//用于保存目标HTTP服务端口
	CString strServer, strObject;	//strServer用于保存服务器地址，strObject用于保存文件对象名称
	DWORD dwServiceType,dwRet;		//dwServiceType用于保存服务类型，dwRet用于保存提交GET请求返回的状态号

	//解析URL，获取信息
	if(!AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
	{
		//解析失败，该Url不正确
		return -1;
	}
	//创建网络连接对象，HTTP连接对象指针和用于该连接的HttpFile文件对象指针，注意delete
	CInternetSession intsess;
	CHttpFile *pHtFile = NULL;
	//建立网络连接
	CHttpConnection *pHtCon = intsess.GetHttpConnection(strServer,nPort);
	if(pHtCon == NULL)
	{
		//建立网络连接失败
		intsess.Close();
		return -2;
	}
	//发起GET请求
	pHtFile = pHtCon->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject);
	if(pHtFile == NULL)
	{
		//发起GET请求失败
		intsess.Close();
		delete pHtCon;pHtCon = NULL;
		return -3;
	}
	//提交请求
	pHtFile->SendRequest();
	//获取服务器返回的状态号
	pHtFile->QueryInfoStatusCode(dwRet);
	if (dwRet != HTTP_STATUS_OK)
	{
		//服务器不接受请求
		intsess.Close();
		delete pHtCon;pHtCon = NULL;
		delete pHtFile;pHtFile = NULL;
		return -4;
	}
	//获取文件大小
	UINT nFileLen = (UINT)pHtFile->GetLength();
	DWORD dwRead = 1;		//用于标识读了多少，为1是为了进入循环
	//创建缓冲区
	CHAR *szBuffer = new CHAR[nFileLen+1];
	TRY 
	{
		//创建文件
		CFile PicFile(strSavePath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);
		while(dwRead>0)
		{  
			//清空缓冲区
			memset(szBuffer,0,(size_t)(nFileLen+1));
			//读取到缓冲区
			dwRead = pHtFile->Read(szBuffer,nFileLen); 
			//写入到文件
			PicFile.Write(szBuffer,dwRead);
		}
		//关闭文件
		PicFile.Close();
		//释放内存
		delete []szBuffer;
		delete pHtFile;
		delete pHtCon;
		//关闭网络连接
		intsess.Close();
	}
	CATCH(CFileException,e)
	{
		//释放内存
		delete []szBuffer;
		delete pHtFile;
		delete pHtCon;
		//关闭网络连接
		intsess.Close();
		return -7;			//读写文件异常
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