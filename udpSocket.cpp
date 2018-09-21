#include "udpSocket.h"
#include "error.h"
#include "log.h"
UDPSocket::UDPSocket(void *root,void *parent):Socket(root,parent)
{
	open(SOCK_DGRAM);
}
UDPSocket::~UDPSocket()
{
}

int UDPSocket::recvFrom(unsigned int* outRemoteAddr, unsigned short* outRemotePort,
                            void* ioBuffer, unsigned int inBufLen, unsigned int* outRecvLen) {
    
    socklen_t addrLen = sizeof(fMsgAddr);
    int theRecvLen = ::recvfrom(fSocket, (char*)ioBuffer, inBufLen, 0, (sockaddr*)&fMsgAddr, &addrLen);
    if (theRecvLen == -1){
		//os_error("recvFrom is error. errno is %d\n",errno);
		return SOCKET_CONNECTION_ERROR;
    }
    
    *outRemoteAddr = ntohl(fMsgAddr.sin_addr.s_addr);
    *outRemotePort = ntohs(fMsgAddr.sin_port);
    *outRecvLen = (unsigned int)theRecvLen;
    return SOCKET_CONNECTION_OK;        
}
bool UDPSocket::sendTo(unsigned int inRemoteAddr, unsigned short inRemotePort, void* inBuffer, unsigned int inLength)
{
    struct sockaddr_in  theRemoteAddr;
    theRemoteAddr.sin_family = AF_INET;
    theRemoteAddr.sin_port = htons(inRemotePort);
    theRemoteAddr.sin_addr.s_addr = htonl(inRemoteAddr);
	int theErr = ::sendto(fSocket, (char*)inBuffer, inLength, 0, (sockaddr*)&theRemoteAddr, sizeof(theRemoteAddr));

    if (theErr == -1){
    	os_error("SendTo is error. errno is %d\n",errno);
        return SOCKET_CONNECTION_ERROR;
    }
    return SOCKET_CONNECTION_OK;
}

