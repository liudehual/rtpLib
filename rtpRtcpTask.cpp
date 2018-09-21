#include "rtpRtcpTask.h"
#include "rtpInstance.h"
#include "normalScheduler.h"
#include "server.h"
#include "packet.h"
#include "udpSocket.h"
#include "utils.h"
#include "timer.h"
TcpListenTask::TcpListenTask(void *root,void *parent):Task(root,parent)
{
	fSocket=new TCPSocket(root,this);
}
TcpListenTask::~TcpListenTask()
{
	os_warn("TcpListenTask Quit \n");
}
int TcpListenTask::initSocket()
{

	RtpInstance *theInstance=(RtpInstance *)this->getRoot();
	
	if((fSocket->bind(0,theInstance->getRecvLPort()))!=1){
		os_error("bind socket error port=%d \n",theInstance->getRecvLPort());
		return SOCKET_BIND_ERROR;
	}
	if((fSocket->listen(1024))!=1){
		os_error("listen socket error \n");
		return SOCKET_LISTEN_ERROR;
	}
	os_trace("create listen socket succeedd port=%d \n",theInstance->getRecvLPort());
	
	int theError=OS_NO_ERROR;
	NormalScheduler *theScheduler=theInstance->getNormalScheduler();
	if((theError=theScheduler->addReadEvent(this))!=OS_NO_ERROR){
		return theError;
	}

	return OS_NO_ERROR;
}

int TcpListenTask::run(){
	
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	 
	int newSocket = accept(fSocket->getSocketFD(),(struct sockaddr*)&client_addr,&length);
	if ( newSocket < 0){
		os_error("accept create new socket error \n");
		return 0;
	}
	os_trace("create client Task Ok \n");
	Server *theServer=(Server *)getParent();
	theServer->createClientTask(newSocket);
	return 0;
}

RtpTask::RtpTask(void *root,void *parent):Task(root,parent)
{

}
RtpTask::~RtpTask()
{
	if(fRtpPacket){
		delete fRtpPacket;	
	}

	if(fTimer){
		delete fTimer;
	}
}
int RtpTask::createDecodePacket()
{
	
	RtpInstance *theInstance=(RtpInstance *)getRoot();

	// RTP 解码
	if(theInstance->getRecvStreamType()==STREAM_PS_TYPE){
		fRtpPacket=new PSRtpPacket(getRoot(),this);
	}else if(theInstance->getRecvStreamType()==STREAM_H264_TYPE){
		fRtpPacket=new H264RtpPacket(getRoot(),this);
	}else if(theInstance->getRecvStreamType()==STREAM_G711_TYPE){
		fRtpPacket=new G711RtpPacket(getRoot(),this);
	}else{
		return OS_STREAM_TYPE_ERROR;
	}
	
	return OS_NO_ERROR;

}

int RtpTask::createEncodePacket()
{
	
	RtpInstance *theInstance=(RtpInstance *)getRoot();

	os_trace("STREAM  TYPE =%d \n",theInstance->getPushStreamType());
	// RTP 编解码
	if(theInstance->getPushStreamType()==STREAM_PS_TYPE){
		fRtpPacket=new PSRtpPacket(getRoot(),this);
	}else if(theInstance->getPushStreamType()==STREAM_H264_TYPE){
		fRtpPacket=new H264RtpPacket(getRoot(),this);
	}else if(theInstance->getPushStreamType()==STREAM_G711_TYPE){
		fRtpPacket=new G711RtpPacket(getRoot(),this);
	}else{
		return OS_STREAM_TYPE_ERROR;
	}
	
	return OS_NO_ERROR;

}

int RtpTask::handleRtpData(char *buf,int len)
{
	fRtpPacket->decode(buf,len);
	return OS_NO_ERROR;
}
Packet *RtpTask::getRtpPacket()
{
	return fRtpPacket;
}

TcpRtpTask::TcpRtpTask(void *root,void *parent):RtpTask(root,parent)
{
	fSocket=new TCPSocket(root,this);
	fSocket->setSendBufferTo(256*1024);
	fSocket->setReceiveBufferTo(512*1024);
	fLen=0;

	fTimer=new RtpSendTimer(getRoot(),this,20);
	fTimer->start();
}
TcpRtpTask::~TcpRtpTask()
{
	os_warn("TcpRtpTask Quit\n");
}
int TcpRtpTask::setRecvModel()
{

	RtpInstance *theInstance=(RtpInstance *)getRoot();
	
	NormalScheduler *theScheduler=theInstance->getNormalScheduler();
	os_trace("create client task succeed \n");
	int theError=OS_NO_ERROR;
	if((theError=theScheduler->addReadEvent(this))!=OS_NO_ERROR){
		return theError;
	}

	return OS_NO_ERROR;	
}

int TcpRtpTask::run()
{
	unsigned int outLen=0;
	if(((TCPSocket *)fSocket)->read(&fBuf[fLen],8196-fLen,&outLen)!=SOCKET_CONNECTION_OK){
		RtpInstance *theInstance=(RtpInstance *)getRoot();
		theInstance->handleExecption(0);
		return -1;
	}
	
	fLen+=outLen;
	
	if(fLen<4){
		return 0;
	}
	unsigned char magicNumber=fBuf[0];
	unsigned char channel=fBuf[1];
	unsigned short dataLen=ntohs(*((unsigned short *)&fBuf[2]));
	//os_error("magicNumber=%d channel=%d dataLen=%d \n",magicNumber,channel,dataLen);
	if(fLen-4>=dataLen){
		if(channel==0){ // rtp
			handleRtpData(&fBuf[4],dataLen);
		}else if(channel==1){ //rtcp
			// do nothing
			handleRtcpData(&fBuf[4],dataLen);
		}

		int validDataLen=fLen-(dataLen+4);
		memmove(fBuf,&fBuf[dataLen+4],validDataLen);
		fLen=validDataLen;
	}
	return 0;
}
int TcpRtpTask::handleRtcpData(char *buf,int len)
{
	
	return OS_NO_ERROR;
}


int TcpRtpTask::sendData(char *buf,int len,unsigned int *outLen)
{
	int sendLen=0;
	int theSocketError=SOCKET_CONNECTION_OK;
	for(;;){
		unsigned int flushLen=0;
		if((theSocketError=((TCPSocket *)fSocket)->send(&buf[sendLen],len-sendLen,&flushLen))
																	!=SOCKET_CONNECTION_OK){
			RtpInstance *theInstance=(RtpInstance *)getRoot();
			theInstance->handleExecption(0);
			return theSocketError;
		}
		sendLen+=flushLen;
		if(sendLen>=len){
			break;
		}
	}
	*outLen=sendLen;
	os_warn("outLen=%d\n",sendLen);
	return OS_NO_ERROR;
}
int TcpRtpTask::writeData(char *buf,int len)
{
	fRtpPacket->writeData(buf,len);
	return OS_NO_ERROR;
}

UdpRtpTask::UdpRtpTask(void *root,void *parent):RtpTask(root,parent)
{
	os_trace("create udp rtp Task succeed \n");

	fSocket=new UDPSocket(root,this);
	fSocket->setSendBufferTo(256*1024);
	fSocket->setReceiveBufferTo(512*1024);
	fTimer=new RtpSendTimer(getRoot(),this,20);
	fTimer->start();
}
UdpRtpTask::~UdpRtpTask()
{
	os_warn("UdpRtpTask Quit\n");

}
int UdpRtpTask::bind(unsigned short port)
{
	if((fSocket->bind(0,port))!=1){
		os_error("rtp Task bind error \n");
		return SOCKET_BIND_ERROR;
	}
}

int UdpRtpTask::setRecvModel()
{

	RtpInstance *theInstance=(RtpInstance *)getRoot();
	
	NormalScheduler *theScheduler=theInstance->getNormalScheduler();
	os_trace("create client task succeed \n");
	int theError=OS_NO_ERROR;
	if((theError=theScheduler->addReadEvent(this))!=OS_NO_ERROR){
		return theError;
	}

	return OS_NO_ERROR;	
}
int UdpRtpTask::writeData(char *buf,int len)
{
	fRtpPacket->writeData(buf,len);
	return OS_NO_ERROR;
}

int UdpRtpTask::run()
{
	unsigned int remoteAddr=0;
	unsigned short remotePort=0;
	
	int theError=0;
	do{
		fLen=0;
		theError=((UDPSocket *)fSocket)->recvFrom(&remoteAddr,&remotePort,(void *)fBuf,(unsigned int)sizeof(fBuf),&fLen);
		if(fLen==0 || theError!=SOCKET_CONNECTION_OK){
			break;
		}
		handleRtpData(fBuf,fLen);
	}while(1);
	return 0;
}
int UdpRtpTask::sendData(char *buf,int len,unsigned int *flushLen)
{
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	char *strIp=theInstance->getPushRIp();
	unsigned int rIp=stringIpToUInt32(strIp);
	unsigned short rPort=theInstance->getPushRPort();
	#if 0
	unsigned short seq=ntohs(*(unsigned short *)&buf[2]);
	os_warn("rIp=%s rPort=%d seq=%d rtpPacketDataLen=%d\n",strIp,rPort,seq,len-12);
	#endif
	((UDPSocket *)fSocket)->sendTo(rIp,rPort,(void *)buf,(unsigned int)len);
	return OS_NO_ERROR;
}

UdpRtcpTask::UdpRtcpTask(void *root,void *parent):Task(root,parent),fRtpConnection(true)
{
	fSocket=new UDPSocket(root,this);
	fSendTimer=new RtcpSendTimer(getRoot(),this,2*1000);
	fSendTimer->start();
	fRecvTimer=new RtcpRecvTimer(getRoot(),this,6*1000);
	fRecvTimer->start();
}
UdpRtcpTask::~UdpRtcpTask()
{

	os_warn("UdpRtcpTask Quit\n");

	if(fSendTimer){
		delete fSendTimer;
	}

	if(fRecvTimer){
		delete fRecvTimer;
	}
}
int UdpRtcpTask::setRecvModel()
{
	RtpInstance *theInstance=(RtpInstance *)getRoot();

	NormalScheduler *theScheduler=theInstance->getNormalScheduler();
	os_trace("create client task succeed \n");
	int theError=OS_NO_ERROR;
	if((theError=theScheduler->addReadEvent(this))!=OS_NO_ERROR){
		return theError;
	}

	return OS_NO_ERROR;
}

int UdpRtcpTask::bind(unsigned short port)
{
	if((fSocket->bind(0,port))!=1){
		os_error("rtp Task bind error \n");
		return SOCKET_BIND_ERROR;
	}
}

int UdpRtcpTask::run()
{
	unsigned int remoteAddr=0;
	unsigned short remotePort=0;
	fLen=0;
	if(((UDPSocket *)fSocket)->recvFrom(&remoteAddr,&remotePort,(void *)fBuf,(unsigned int)sizeof(fBuf),&fLen)!=SOCKET_CONNECTION_OK){
		return -1;
	}
	if(fLen==0){
		return 0;
	}
	handleRtcpData(fBuf,fLen);
	return 0;
}
int UdpRtcpTask::handleRtcpData(char *buf,int len)
{
	// 暂时不做处理，只做心跳用
	this->fRtpConnection=true;

	unsigned char rtcpType=buf[2];
	switch(rtcpType){
		case SR_PACKET:{
			SRRtcpPacket thePacket(getRoot(),this);
			thePacket.decode(buf,len);
			break;
		}
		case RR_PACKET:{
			RRRtcpPacket thePacket(getRoot(),this);
			thePacket.decode(buf,len);
			break;
		}
		case SDES_PACKET:{
			SDESRtcpPacket thePacket(getRoot(),this);
			thePacket.decode(buf,len);
			break;
		}
		case BYE_PACKET:{
			BYERtcpPacket thePacket(getRoot(),this);
			thePacket.decode(buf, len);
			break;
		}
		case APP_PACKET:{
			APPRtcpPacket thePacket(getRoot(),this);
			thePacket.decode(buf,len);
			break;
		}
		default:{
			break;
		}
	}
	return OS_NO_ERROR;
}

int UdpRtcpTask::sendData(char *buf,int len,unsigned int *flushLen)
{
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	char *strIp=theInstance->getPushRIp();
	unsigned int rIp=stringIpToUInt32(strIp);
	unsigned short rPort=theInstance->getPushRPort()+1;
	((UDPSocket *)fSocket)->sendTo(rIp,rPort,(void *)buf,(unsigned int)len);
	return OS_NO_ERROR;	
}
