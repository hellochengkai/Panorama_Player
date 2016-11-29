// UDPMessage.cpp : 实现文件
//

#include "stdafx.h"
#include "Panorama_PlayerDlg.h"
#include "UDPMessage.h"


// UDPMessage
int GetourIP_C(char * ourIP);

UDPMessage::UDPMessage()
{	
	memset(ourIP,0,INET_ADDRSTRLEN);
	memset(RecvfromIP,0,INET_ADDRSTRLEN);
	GetourIP_C(ourIP);
}

UDPMessage::~UDPMessage()
{
	memset(ourIP,0,INET_ADDRSTRLEN);
	memset(RecvfromIP,0,INET_ADDRSTRLEN);
}

int GetourIP_C(char * ourIP)
{
	int PORT = 7778;
	struct sockaddr_in user_addr,my_addr;
	int socket_fd;
//	char  so_broadcast;

	BOOL bBroadcast=TRUE;

	char buf[1024];

	socklen_t size;

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(PORT);
	my_addr.sin_addr.s_addr=inet_addr("255.255.255.255");

	user_addr.sin_family=AF_INET;
	user_addr.sin_port=htons(PORT);
	user_addr.sin_addr.s_addr=htonl(INADDR_ANY);

	if((socket_fd=(socket(AF_INET,SOCK_DGRAM,0)))==-1) {
		perror("socket");
		//exit(1);
	}
	setsockopt(socket_fd,SOL_SOCKET,SO_BROADCAST,(const char*)&bBroadcast,sizeof(BOOL ));//??socket发送的数据具有广播特性

	if((bind(socket_fd,(struct sockaddr *)&user_addr, sizeof(struct sockaddr)))==-1) {
		perror("bind");
		//exit(1);
	}

	strcpy_s(buf,"broadcast test");
	sendto(socket_fd,buf,strlen(buf),0,(struct sockaddr *)&my_addr,sizeof(my_addr));
	size=sizeof(user_addr);
	recvfrom(socket_fd,buf,1024,0,(struct sockaddr *)&user_addr,&size);

	//close(socket_fd);
	closesocket(socket_fd);
	printf("my_ip:%s\n",inet_ntoa(user_addr.sin_addr));
	memcpy(ourIP,inet_ntoa(user_addr.sin_addr),strlen(inet_ntoa(user_addr.sin_addr)));
	return 0;
}

char * UDPMessage::getourIP() const
{
	return (char * )ourIP;
}

//UDP MSG-----------------


int UDPMessage::UDPMessageSend(char *IP,int port,char *buf_in)
{	
	int Ret = -1;
	int sockfd;
	int acceptfd;
	int on = 1;
	int i = 0;

	/*my_printf("SendUDPMSG");
	for(i = 0;i < buf_len;i++){
		printf("%c",buf_in[i]);
	}
	printf("\n");
	*/
	

	GetMainDlg()->ShowTimeToEdit();
	GetMainDlg()->ShowLogToEdit("[SendTo:",strlen("[SendTo:"));
	if(IP == NULL){
		GetMainDlg()->ShowLogToEdit(RecvfromIP,strlen(RecvfromIP));
	}else{
		GetMainDlg()->ShowLogToEdit(IP,strlen(IP));
	}
	GetMainDlg()->ShowLogToEdit("]:",2);
	GetMainDlg()->ShowLogToEdit(buf_in,SendBufLen);
	GetMainDlg()->LogEditEnt();
	
	struct sockaddr_in broadcastaddr;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		printf("fail to socket");
		goto EXIT;
	}

	broadcastaddr.sin_family = AF_INET;

	if(IP == NULL){
		broadcastaddr.sin_addr.s_addr = inet_addr(RecvfromIP);
	}else{
		broadcastaddr.sin_addr.s_addr = inet_addr(IP);
	}
	//broadcastaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(port == 0)
		broadcastaddr.sin_port = htons(FusionSendMsgPort);
	else		
		broadcastaddr.sin_port = htons(port);

	if(sendto(sockfd, buf_in,SendBufLen, 0, (struct sockaddr *)&broadcastaddr, sizeof(broadcastaddr)) < 0)
	{
		perror("sendto");
		printf("fail to send");
		goto EXIT;
	}
EXIT:
	if(sockfd)
		closesocket(sockfd);
	return 0;
}



int UDPMessage::UDPMessageRecvInit(char *IP,int port,int timeout)
{
	int Ret = -1;
    struct timeval tv_out;
	int acceptfd;
    tv_out.tv_sec = timeout;
    tv_out.tv_usec = 0;
	struct sockaddr_in broadcastaddr;
	
	//char fromIPtemp[INET_ADDRSTRLEN] = "";


	if((RecvSockfd= socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		printf("fail to socket");
		goto EXIT;
	}
	
	broadcastaddr.sin_family = AF_INET;
	
	if(IP == NULL){
		broadcastaddr.sin_addr.s_addr = inet_addr(getourIP());
	}else{
		broadcastaddr.sin_addr.s_addr = inet_addr(IP);
	}

	//broadcastaddr.sin_addr.s_addr = inet_addr(INADDR_ANY);
	if(port == 0)
		broadcastaddr.sin_port = htons(FusionRecvMsgPort);
	else		
		broadcastaddr.sin_port = htons(port);
	
	setsockopt(RecvSockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv_out, sizeof(tv_out));

	if(bind(RecvSockfd, (struct sockaddr *)&broadcastaddr, sizeof(broadcastaddr)) < 0)
	{
		//printf("strerror:(%d)%s\n", errno,strerror(errno)); 
		printf("fail to bind\n");
		goto EXIT;
	}

	return 0;
	EXIT:
		
		return Ret;
}

int UDPMessage::UDPMessageRecv(char **buf_out,char **fromIP)
{
	int Ret = -1;
	struct sockaddr_in	fromaddr;
	
	char * recvBuf = NULL;
	recvBuf = (char *)malloc(RecvBufLen);
	if(recvBuf)
		memset(recvBuf,0,RecvBufLen);

	socklen_t fromaddr_len = sizeof(fromaddr);
	if(fromIP){
		if(*fromIP == NULL){
			*fromIP = (char *)malloc(INET_ADDRSTRLEN);
		}
		
		if(*fromIP){
			memset(*fromIP,0,INET_ADDRSTRLEN);
		}
	}
	if((Ret = recvfrom(RecvSockfd, recvBuf, RecvBufLen, 0, (struct sockaddr *)&fromaddr, &fromaddr_len)) < 0) 
	{
		//perror("recvfrom");
		//my_printf("fail to recvfrom");
		Ret = -1;
		goto EXIT;
	}
	
	inet_ntop(AF_INET,&fromaddr.sin_addr,RecvfromIP,INET_ADDRSTRLEN);
	
	if(fromIP){
		inet_ntop(AF_INET,&fromaddr.sin_addr,*fromIP,INET_ADDRSTRLEN);
		//printf("recvfrom IP %s\n",*fromIP);
	}
	/*
	int i = 0;
	printf("RecvUDPMSG(%d)",Ret);
	for(i = 0;i < Ret;i++){
		printf("%c",recvBuf[i]);
	}
	printf("\n");
	*/

	GetMainDlg()->ShowTimeToEdit();
	GetMainDlg()->ShowLogToEdit("[RecvFrom:",strlen("[RecvFrom:"));
	GetMainDlg()->ShowLogToEdit(RecvfromIP,strlen(RecvfromIP));
	GetMainDlg()->ShowLogToEdit("]:",2);
	GetMainDlg()->ShowLogToEdit(recvBuf,RecvBufLen);
	GetMainDlg()->LogEditEnt();

	*buf_out = recvBuf;
	
EXIT:
	if(Ret < 0){
		if(fromIP && *fromIP){
			free(*fromIP);
			*fromIP = NULL;
		}
	}
		
	return Ret;
}

void UDPMessage::UDPMessageRecvDeinit()
{
	if(RecvSockfd)
		closesocket(RecvSockfd);
}