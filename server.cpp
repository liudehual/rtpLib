#include "server.h"
#include "error.h"
#include "rtpRtcpTask.h"
#include "rtpInstance.h"
#include "normalScheduler.h"
#include "rtpLib.h"
#include "tcpSocket.h"
Server::Server(void *root,void *parent):Object(root,parent),fTcpRtpTask(NULL)
{

}
Server::~Server()
{
			// trans_type=tcp
		if(fTcpRtpTask){
			delete fTcpRtpTask;
		}
		if(fListenTask){
			delete fListenTask;
		}

		if(fUdpRtpTask){
			delete fUdpRtpTask;
		}
		if(fUdpRtcpTask){
			delete fUdpRtcpTask;
		}

}
int Server::initialization()
{
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	if(theInstance->getRecvTransType()==TRANS_PROTOCOL_TCP){
		fListenTask=new TcpListenTask(getRoot(),this);
		
		int theError=OS_NO_ERROR;
		if((theError=fListenTask->initSocket())!=OS_NO_ERROR){
			return theError;
		}

	}else if(theInstance->getRecvTransType()==TRANS_PROTOCOL_UDP){
		// 解码并处理rtp数据包
		fUdpRtpTask=new UdpRtpTask(getRoot(),this);
		fUdpRtpTask->createDecodePacket();
		
		unsigned short rtpPort=0;
		if(theInstance->getRecvLPort()%2){
			rtpPort=theInstance->getRecvLPort()+1;
		}else{
			rtpPort=theInstance->getRecvLPort();
		}

		int theError=OS_NO_ERROR;
		// 绑定Rtp端口
		if((theError=fUdpRtpTask->bind(rtpPort))!=OS_NO_ERROR){
			return theError;
		}
		// 使fUdpRtpTask 具有接收功能
		if((theError=fUdpRtpTask->setRecvModel())!=OS_NO_ERROR){
			return theError;
		}

		// 解码并处理rtcp数据包
		fUdpRtcpTask=new UdpRtcpTask(getRoot(),this);
		
		// 绑定RTCP端口
		if((theError=fUdpRtcpTask->bind(rtpPort+1))!=OS_NO_ERROR){
			return theError;
		}
		// 使fUdpRtcpTask 具有接收功能
		if((theError=fUdpRtcpTask->setRecvModel())!=OS_NO_ERROR){
			return theError;
		}
		os_warn("init server succeed rtpPort=%d rtcpPort=%d \n",rtpPort,rtpPort+1);
	}		

	return OS_NO_ERROR;
}
int Server::createClientTask(int socket)
{	
	RtpInstance *theInstance=(RtpInstance *)getRoot();

	os_trace("recv stream type=%d \n",theInstance->getRecvStreamType());
	if(fTcpRtpTask){
		delete fTcpRtpTask;
		fTcpRtpTask=NULL;
	}
	fTcpRtpTask=new TcpRtpTask(getRoot(),this);
	fTcpRtpTask->createDecodePacket();	
	fTcpRtpTask->getSocket()->setSocket(socket);
	
	((TCPSocket *)fTcpRtpTask->getSocket())->setConnected();
	fTcpRtpTask->setRecvModel();

	return OS_NO_ERROR;
}
