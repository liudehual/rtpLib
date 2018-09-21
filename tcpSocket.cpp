#include "tcpSocket.h"
#include "error.h"
#include "log.h"
TCPSocket::TCPSocket(void *root,void *parent):Socket(root,parent),fConnected(0)
{
	open(SOCK_STREAM);
}
TCPSocket::~TCPSocket()
{
}

int TCPSocket::send(const char* inData, const unsigned int inLength, unsigned int* outLengthSent)
{
	if(!fConnected){
		return SOCKET_CONNECTION_ERROR;
	}
    int err;
    do {
       err = ::send(fSocket, inData, inLength, 0);//flags??
    } while((err == -1) && (errno == EINTR));
    if(err == -1){
        //Are there any errors that can happen if the client is connected?
        //Yes... EAGAIN. Means the socket is now flow-controleld
        int theErr = errno;
        if (theErr != EAGAIN){
			os_error("connect is reset.errno is %d\n",errno);
			fConnected=0;
			return SOCKET_CONNECTION_ERROR;
        }else{
			return SOCKET_CONNECTION_OK;
		}
    }
    *outLengthSent = err;
    return SOCKET_CONNECTION_OK;
	
}
int TCPSocket::read(void *buffer, const unsigned int length, unsigned int *outRecvLenP)
{
	if(!fConnected){
		return SOCKET_CONNECTION_ERROR;
	}

    int theRecvLen;
    do{
       theRecvLen = ::recv(fSocket, (char*)buffer, length, 0);//flags??
    }while((theRecvLen == -1) && (errno == EINTR));

    if(theRecvLen == -1){
        // Are there any errors that can happen if the client is connected?
        // Yes... EAGAIN. Means the socket is now flow-controleld
        int theErr =errno;
        if (theErr != EAGAIN){
			os_error("connect is wrong while read. errno is %d\n",errno);
			fConnected=0;
			return SOCKET_CONNECTION_ERROR;
        }else{
			return SOCKET_CONNECTION_OK;
		}
    }else if (theRecvLen == 0){
       os_error("connect is reset while read. errno is %d\n",errno);
	   fConnected=0;
       return SOCKET_CONNECTION_ERROR;
    }
    *outRecvLenP = (unsigned int)theRecvLen;
    return SOCKET_CONNECTION_OK;
}

int  TCPSocket::connect(unsigned int inRemoteAddr,unsigned short inRemotePort)
{
    ::memset(&fRemoteAddr, 0, sizeof(fRemoteAddr));
    fRemoteAddr.sin_family = AF_INET;        /* host byte order */
    fRemoteAddr.sin_port = htons(inRemotePort); /* short, network byte order */
    fRemoteAddr.sin_addr.s_addr = htonl(inRemoteAddr);
    int err = ::connect(fSocket,(sockaddr *)&fRemoteAddr,sizeof(fRemoteAddr));
    if (err == -1){
        fRemoteAddr.sin_port = 0;
        fRemoteAddr.sin_addr.s_addr = 0;
        return SOCKET_CONNECT_SERVER_FAILED;
    }
    fConnected=1;
    return err;

}
int TCPSocket::connect1(unsigned int ip,unsigned short port)
{
    ::memset(&fRemoteAddr, 0, sizeof(fRemoteAddr));
    fRemoteAddr.sin_family = AF_INET;        /* host byte order */
    fRemoteAddr.sin_port = htons(port); /* short, network byte order */
    fRemoteAddr.sin_addr.s_addr = htonl(ip);

    // we are not allow our program like dead while connect to server
    makeSocketNonBlocking(fSocket);

    // don't forget to error check the connect()!
    // socket is noblock
    int err = ::connect(fSocket,(sockaddr *)&fRemoteAddr,sizeof(fRemoteAddr));

    // -1 means we should check the connection state
    if (err == -1){
        if (errno == EINPROGRESS){ // EINPROGRESS means connection is in progress, normally the socket connecting timeout is 75s. after the socket fd is ready to read.
            // means the connecting of the socket fd is established.
            int err;
            int len = sizeof(int);
            fd_set wds;
            struct timeval tm;
            tm.tv_sec = 2;
            tm.tv_usec = 0;
            FD_ZERO(&wds);
            FD_SET(fSocket, &wds);
            if (select(fSocket + 1, NULL, &wds, NULL, &tm) > 0){  // ">0" means sockfd ready to read, "=0" means timeout cause retrun, "<0" means error.
                int retcode = getsockopt(fSocket, SOL_SOCKET, SO_ERROR, &err, (socklen_t *)&len);
                if (retcode == -1 || err != 0){// we get error state,means this connection failed.return error
                    close(fSocket);
					os_error("connect server failed \n");
                    return SOCKET_CONNECT_SERVER_FAILED;
                }
            }else{
            	os_error("connect server failed \n");
                // timeout or error happened, connect failed,return error
                close(fSocket);
                return SOCKET_CONNECT_SERVER_FAILED;
            }
        }else{
            // connect failed,return error
            close(fSocket);
            fRemoteAddr.sin_port = 0;
            fRemoteAddr.sin_addr.s_addr = 0;
            os_error("connect server failed \n");
            return SOCKET_CONNECT_SERVER_FAILED;
        }
    }
    // make socket block
    // makeSocketBlocking(fSocket);
	fConnected=1;
    return OS_NO_ERROR;
}
int TCPSocket::setConnected()
{
	fConnected=1;
	return OS_NO_ERROR;
}

