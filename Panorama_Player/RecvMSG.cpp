// RecvMSG.cpp : 实现文件
//

#include "stdafx.h"
#include "Panorama_PlayerDlg.h"
#include "RecvMSG.h"


// RecvMSG

RecvMSG::RecvMSG()
{
}

RecvMSG::~RecvMSG()
{
}

int RecvMSG::InitSocket(int Src_port)
{
    addrSrc.sin_family = AF_INET;
    addrSrc.sin_port = htons(Src_port);
	addrSrc.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//addrSrc.sin_addr.S_un.S_addr = inet_addr("10.0.4.151");
	Recvsock = socket(AF_INET,SOCK_DGRAM, 0);
	return 0;
}

CString RecvMSG::RecvMsg()
{
	CString retDate;
	int ret = 0;
	sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr);
	if(0){
		BOOL bBroadcast=TRUE;
		setsockopt(Recvsock,SOL_SOCKET,SO_BROADCAST,(const char*)&bBroadcast,sizeof(BOOL));
	}
	bind(Recvsock, (sockaddr *)&addrSrc, sizeof(addrSrc));
	char recvData[255] = {0};  
	//while(1){
		ret = recvfrom(Recvsock, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);
		if(ret > 0){
			recvData[ret] = 0x00;
			retDate = (CString)recvData;
		//	break;
		}
	//}
	return retDate;
}

int RecvMSG::DeInitSocket()
{
	closesocket(Recvsock);
	return 0;
}
// RecvMSG 成员函数
