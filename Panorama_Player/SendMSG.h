#pragma once

// SendMSG ÃüÁîÄ¿±ê

class SendMSG : public CSocket
{
public:
	SendMSG();
	virtual ~SendMSG();

	SOCKET Sendsock;
	SOCKADDR_IN  dest_addr;
	int send_buf_len;
	char *send_buf;

	int InitSocket(int Dest_port);
	int SendMsg(const char * msg);
	int DeInitSocket();
};