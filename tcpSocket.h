#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__
#include "socket.h"
class TCPSocket:public Socket
{
	public:
		TCPSocket(void *root,void *parent);
	    virtual	~TCPSocket();
		int connect(unsigned int inRemoteAddr,unsigned short inRemotePort);
		int connect1(unsigned int ip,unsigned short port);
		int send(const char* inData, const unsigned int inLength, unsigned int* outLengthSent);
		int read(void *buffer, const unsigned int length, unsigned int *outRecvLenP);
		int setConnected();
	private:
	    struct sockaddr_in  fRemoteAddr;

		int fConnected;
};
#endif

