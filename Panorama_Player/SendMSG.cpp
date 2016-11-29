// SendMSG.cpp : 实现文件
//

#include "stdafx.h"
#include "Panorama_PlayerDlg.h"
#include "SendMSG.h"


// SendMSG

SendMSG::SendMSG()
{	
	send_buf_len = 48;
	
	send_buf = (char *)malloc(send_buf_len);

	if(send_buf){
		memset(send_buf,0,send_buf_len);
	}
}

SendMSG::~SendMSG()
{
	if(send_buf){
		free(send_buf);
		send_buf = NULL;
	}
}


// SendMSG 成员函数

int SendMSG::InitSocket(int Dest_port)
{

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(Dest_port);
    dest_addr.sin_addr.S_un.S_addr = inet_addr(INADDR_ANY);

	Sendsock = socket(AF_INET,SOCK_DGRAM, 0);

	return 0;
}
int SendMSG::SendMsg(const char *msg)
{
	if(!send_buf)
		return -1;
	memset(send_buf,0,send_buf_len);
	int i =0;
	for(i = 0;i < strlen(msg) * 2;i+=2){
		send_buf[i] = msg[i/2];
	}
	
	sendto(Sendsock,send_buf,send_buf_len,0,(SOCKADDR*)&dest_addr,sizeof(SOCKADDR));
	return 0;
}

int SendMSG::DeInitSocket()
{
	closesocket(Sendsock);
	return 0;
}