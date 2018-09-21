#ifndef __UDP_SOCKET_H__
#define __UDP_SOCKET_H__
#include "socket.h"
class UDPSocket:public Socket
{
	public:
		UDPSocket(void *root,void *parent);
		virtual ~UDPSocket();
		
    	int recvFrom(unsigned int* outRemoteAddr, unsigned short* outRemotePort,void* ioBuffer, unsigned int inBufLen, unsigned int* outRecvLen);
		bool sendTo(unsigned int inRemoteAddr, unsigned short inRemotePort, void* inBuffer, unsigned int inLength);

	private:
		struct sockaddr_in  fMsgAddr;

	 	char udpRemoteIp[32];
	  	unsigned short udpRemotePort;

	 	char udpLocalIp[32];
	  	unsigned short udpLocalPort;

};
#endif

