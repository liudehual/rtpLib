#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "netCommon.h"
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

class Socket:public Object
{
	protected:
		Socket(void *root,void *parent);
	public:
		virtual ~Socket();
		int getSocketFD(){return fSocket;}
		int open(int theType);
		void InitNonBlocking(int inFileDesc);	
		
		unsigned getBufferSize(int bufOptName,int socket); 
		unsigned getSendBufferSize(); 
		unsigned getReceiveBufferSize(); 

		unsigned setBufferTo(int bufOptName,int socket, unsigned requestedSize);
		unsigned setSendBufferTo(unsigned requestedSize); 
		unsigned setReceiveBufferTo(unsigned requestedSize); 

		unsigned increaseBufferTo(int bufOptName,int socket, unsigned requestedSize); 
		unsigned increaseSendBufferTo(unsigned requestedSize);
		unsigned increaseReceiveBufferTo(unsigned requestedSize);

		bool makeSocketNonBlocking(int sock);
		bool makeSocketBlocking(int sock); 

		int bind(unsigned int addr, unsigned short port);
		int listen(int listenQueueSize);
		unsigned int strIpToUInt32(char * ip);
		void UInt32ToStrIp(unsigned int uIntIp,char *strIp);

		char *getRemoteStrIp();
		char *getLocalStrIp();
		
		unsigned short getRemotePort();
		unsigned short getLocalPort();

		void setRemoteStrIp(char *ip);
		void setLocalStrIp(char *ip);
		void setRemotePort(unsigned short port);
		void setLocalPort(unsigned short port);
		void setSocket(int socket);
	protected:
		int fSocket;
		struct sockaddr_in  fLocalAddr;

	 	char remoteIp[32];
	  	unsigned short remotePort;

	 	char localIp[32];
	  	unsigned short localPort;
};


#endif
