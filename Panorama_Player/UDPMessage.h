#pragma once

// UDPMessage ÃüÁîÄ¿±ê


#define  SendBufLen  48
#define  RecvBufLen  48

#define 	FusionSendMsgPort 21392
#define 	FusionRecvMsgPort 21391

class UDPMessage : public CSocket
{
public:
	UDPMessage();
	virtual ~UDPMessage();
	char * getourIP() const;
	int UDPMessageSend(char *IP,int port,char *buf_in);


	
	int RecvSockfd;
	char RecvfromIP[INET_ADDRSTRLEN];
	
	void UDPMessage::UDPMessageRecvDeinit();
	int UDPMessage::UDPMessageRecv(char **buf_out,char **fromIP);
	int UDPMessage::UDPMessageRecvInit(char *IP,int port,int timeout);

	//int UDPMessageRecv(char *IP,int port,char **buf_out,char **fromIP,int timeout);
private:
	char  ourIP[INET_ADDRSTRLEN];
};


