#include "client.h"
#include "rtpInstance.h"
#include "tcpSocket.h"
#include "error.h"
#include "rtpRtcpTask.h"
#include "normalScheduler.h"
#include "timer.h"
#include "packet.h"
#include "task.h"
Client::Client(void *root,void *parent):Object(root,parent)
{
	
}
Client::~Client()
{
	os_warn("Client Quit\n");

	if(fTcpRtpTask){
		delete fTcpRtpTask;
	}
	if(fUdpRtpTask){
		delete fUdpRtpTask;
	}
	if(fUdpRtcpTask){
		delete fUdpRtcpTask;
	}

}
int Client::initialization()
{
	RtpInstance *theInstance=(RtpInstance *)getRoot();

	// tcp model
	int theError=OS_NO_ERROR;

	if(theInstance->getPushTransType()==TRANS_PROTOCOL_TCP){
		fTcpRtpTask=new TcpRtpTask(getRoot(),this);
		fTcpRtpTask->createEncodePacket();	

		TCPSocket *theTcpSocket=(TCPSocket *)fTcpRtpTask->getSocket();
		int rIp=theTcpSocket->strIpToUInt32(theInstance->getPushRIp());
		os_trace("connect server server_ip=%s server_port=%d \n",
											theInstance->getPushRIp(),
											theInstance->getPushRPort());
	
		if((theError=theTcpSocket->connect1(rIp,theInstance->getPushRPort()))!=OS_NO_ERROR){
			return theError;
		}
		// 使fTcpRtcpTask 具有接收功能
		if((theError=fTcpRtpTask->setRecvModel())!=OS_NO_ERROR){
			return theError;
		}
	 }else if(theInstance->getPushTransType()==TRANS_PROTOCOL_UDP){

	 	// 解码并处理rtp数据包
		fUdpRtpTask=new UdpRtpTask(getRoot(),this);
		fUdpRtpTask->createEncodePacket();
		
		unsigned short rtpPort=0;
		if(theInstance->getPushLPort()%2){
			rtpPort=theInstance->getPushLPort()+1;
		}else{
			rtpPort=theInstance->getPushLPort();
		}

		int theError=OS_NO_ERROR;
		// 绑定Rtp端口
		if((theError=fUdpRtpTask->bind(rtpPort))!=OS_NO_ERROR){
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
	 }

	return OS_NO_ERROR;
}

int Client::sendData(char *buf,int len)
{	
	//os_warn("ps buf len=%d \n",len);
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	if(theInstance->getPushTransType()==TRANS_PROTOCOL_TCP){
		fTcpRtpTask->writeData(buf,len);
	}else if(theInstance->getPushTransType()==TRANS_PROTOCOL_UDP){
		fUdpRtpTask->writeData(buf,len);
	}else{
		return OS_TRANS_PROTOCOL_TYPE_ERROR;
	}
	return OS_NO_ERROR;
}
Task * Client::getTask()
{
	return fTcpRtpTask;
}
Task *Client::getUdpRtpTask()
{
	return fUdpRtpTask;
}
Task *Client::getUdpRtcpTask()
{
	return fUdpRtcpTask;
}

