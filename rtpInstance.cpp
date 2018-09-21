#include "rtpInstance.h"
#include "normalScheduler.h"
#include "utils.h"
#include "client.h"
#include "server.h"
#include "timer.h"
RtpInstance::RtpInstance(void *root,void *parent):
									Instance(root,parent),
									Thread(root,parent),
									fTransType(-1),
									fRtpInstanceModel(-1),
									fClearStatus(false),
									fClient(NULL)
{
	fId=getObjectId(this);

	fRecvLPort=0;
	fRecvTransType=-1;
	fRecvStreamType=-1;
	fRecvRtcpTimeout=0;
	fRecvCache=100*1024;
	fRecvRecvRtp=NULL;
	fRecvArg1=NULL;
	fRecvRecvRtcp=NULL;
	fRecvArg2=NULL;
	fRecvRecvMediaData=NULL;
	fRecvArg3=NULL;
	fRecvRe=NULL;
	fRecvArg4=NULL;
	fServer=NULL;


	fPushRIp=new char[32];;
	fPushRPort=0;
	fPushLPort=0;
	fPushTransType=-1;
	fPushStreamType=-1;
	os_warn("push stream type=%d \n",fPushStreamType);
	fPushRtcpTimeout=0;
	fPushRe=NULL;
	fPushArg1=NULL;
	fPushRecvRtcp=NULL;
	fPushArg2=NULL;	
	fClient=NULL;
	
}
RtpInstance::~RtpInstance()
{
	os_warn("RtpInstance Quit id=%lu \n",fId);
	// ���������
	delete fScheduler;

	// ����ͻ���
	if(fClient){
		delete fClient;
	}
	// ��������
	if(fServer){
		delete fServer;
	}


}
int RtpInstance::initialization()
{
	int theError=OS_NO_ERROR;
	fScheduler=new NormalScheduler(this,this);
	if((theError=fScheduler->initialization())!=OS_NO_ERROR){
		return theError;
	}
	return OS_NO_ERROR;
}
int RtpInstance::entry()
{
	for(;;){
		usleep(500*1000);
		if(getClearStatus()){
			// ����RtpInstance 
			fScheduler->breakLoop();
			// ����Ϊ��Чģʽ
			fRtpInstanceModel=-1;
			usleep(1000*1000);
			break;
			// ��ʱ��ɾ������
		}
	}
	delete this;
	return OS_NO_ERROR;
}

NormalScheduler *RtpInstance::getNormalScheduler()
{
	return fScheduler;
}
long RtpInstance::getRtpInstanceId()
{
	return fId;
}

int RtpInstance::startRtpInstance()
{
	return fScheduler->start();
}

//  lPort 				���ض˿�(���ն˿� udp ��Ϊ���ն˿� tcp��Ϊ�����˿�)
//  transType 			��������(0-udp 1-tcp)
//  streamType 		    ������(0-h264 1-G711  2-ps)
//  rtcpTimeout			rtcp ��ʱʱ��(rtcp��Ϊ��������ʹ��(udp)��������ָ��ʱ��û���յ�rtcp��ʱ��Ĭ�϶Զ����뿪) 
//  rtpRecvRtpData    	RTP���ݽ��ջص�����
//  arg1			    rtpRecvRtpData   ˽�в���
//  rtpRecvRTCPData   	RTCP���ݽ��ջص�����
//  arg2				rtpRecvRTCPData  ˽�в���  
//  rtpRecvMediaData	����Ƶ���ݽ��ջص�����(�ûص�������������Ƶԭʼ����)(�ûص���������Ϊ��)
//  arg3				rtpRecvMediaData ˽�в���
//  rtpException	    SDK�쳣�ص�
//  arg4				rtpException ˽�в���
//  ����ֵ		            0 �ɹ� ����ֵʧ��
int RtpInstance::rtpSetRtpInstanceRecvModel(		unsigned short lPort,
											int transType,
											int streamType,
											int rtcpTimeout,
											int recvCache,
											rtpRecvRtpData recvRtp,
											void *arg1,
											rtpRecvRtcpData recvRtcp,
											void *arg2,
											rtpRecvMediaData recvMediaData,
											void *arg3,
											rtpException re,
											void *arg4)
{
	if(recvMediaData==NULL){
		os_error("recv media data call back funtion null \n");
		return OS_RECV_MEDIA_DATA_CB_EMPTY;
	}
	if(transType!=TRANS_PUSH 
			&& transType!=TRANS_RECV){
		os_error("transType error transType=%d \n",transType);
		return OS_FUNCTION_ARGS_ERROR;
	}
	if(streamType!=STREAM_G711_TYPE
		&& streamType!=STREAM_H264_TYPE
		&& streamType!=STREAM_PS_TYPE){
		os_error("transType error streamType=%d \n",streamType);
		return OS_FUNCTION_ARGS_ERROR;
	}
	if(rtcpTimeout==-1){
		os_error("transType error rtcpTimeout=%d \n",rtcpTimeout);
		return OS_FUNCTION_ARGS_ERROR;
	}
	if( re==NULL){
		os_error("rtpException and rtpException arg not allowed set NULL \n");
		return OS_FUNCTION_ARGS_ERROR;
	}
	fRecvLPort=lPort;
	fRecvTransType=transType;
	fRecvStreamType=streamType;
	fRecvRtcpTimeout=rtcpTimeout;
	if(recvCache>3*1024){ // max 3M
		fRecvCache=3*1024;
	}else if(recvCache<100){ // min 100K
		fRecvCache=100*1024;
	}else{
		fRecvCache=recvCache*1024; // ok
	}
	fRecvRecvRtp=recvRtp;
	fRecvArg1=arg1;
	fRecvRecvRtcp=recvRtcp;
	fRecvArg2=arg2;
	fRecvRecvMediaData=recvMediaData;
	fRecvArg3=arg3;
	fRecvRe=re;
	fRecvArg4=arg4;

	os_trace("recv model \n");

	int theError=OS_NO_ERROR;
	// ������ģʽ
	fRtpInstanceModel=TRANS_RECV;
	fServer=new Server(this,this);
	if((theError=fServer->initialization())!=OS_NO_ERROR){
		return theError;
	}
	
	return OS_NO_ERROR;
}
											
//	rIp 				 Զ��ip
//	rPort				 Զ�˶˿�(udp Ϊ���Ͷ˿� tcpΪ���Ӷ˿�)
//	lPort				 ���ذ󶨶˿�
//	transType			 ��������(0-udp 1-tcp)
//	streamType			 ������(0-h264 1-G711)
//	rtcpTimeout 		 rtcp ��ʱʱ�� (rtcp��Ϊ��������ʹ�ã�������ָ��ʱ��û���յ�rtcp��ʱ��Ĭ�϶Զ����뿪) 
//	rtpException		 SDK�쳣�ص�(�ûص������쳣�¼�)
//	arg1				 rtpException ˽�в���
//	rtpRecvRTCPData 	 RTCP���ݽ��ջص�����
//	arg2				 rtpRecvRTCPData  ˽�в���	
//	����ֵ					 0 �ɹ� ����ֵʧ��
int RtpInstance::rtpSetRtpInstancePushModel(		char *rIp,
                    									unsigned short rPort,
                    									unsigned short lPort,
                    									int transType,
                    									int streamType,
                    									int rtcpTimeout,
                    									rtpException re,
                    									void *arg1,
                    									rtpRecvRtcpData recvRtcp,
                    									void *arg2)
{
	if(rIp==NULL 
		|| strlen(rIp)==0
		|| rPort==0
		){
		os_error("some thing error while set rIp=%p rPort=%d lPort=%d \n",rIp,rPort);
		return OS_FUNCTION_ARGS_ERROR;
	}
	if(transType!=TRANS_PUSH 
			&& transType!=TRANS_RECV){
		os_error("transType error transType=%d \n",transType);
		return OS_FUNCTION_ARGS_ERROR;
	}
	if(streamType!=STREAM_G711_TYPE
		&& streamType!=STREAM_H264_TYPE
		&& streamType!=STREAM_PS_TYPE){
		os_error("transType error streamType=%d \n",streamType);
		return OS_FUNCTION_ARGS_ERROR;
	}
	if(rtcpTimeout==-1){
		os_error("transType error rtcpTimeout=%d \n",rtcpTimeout);
		return OS_FUNCTION_ARGS_ERROR;
	}
	if( re==NULL){
		os_error("rtpException and rtpException arg not allowed set NULL \n");
		return OS_FUNCTION_ARGS_ERROR;
	}

	strcpy(fPushRIp,rIp);
	fPushRPort=rPort;
	fPushLPort=lPort;
	fPushTransType=transType;
	fPushStreamType=streamType;
	os_warn("push stream type=%d \n",fPushStreamType);
	fPushRtcpTimeout=rtcpTimeout;
	fPushRe=re;
	fPushArg1=arg1;
	fPushRecvRtcp=recvRtcp;
	fPushArg2=arg2;	

	int theError=OS_NO_ERROR;
	os_warn("Push model \n");
	
	// �ƶ���ģʽ
	fRtpInstanceModel=TRANS_PUSH;
	fClient=new Client(this,this);
	if((theError=fClient->initialization())!=OS_NO_ERROR){
		return theError;
	}

	return OS_NO_ERROR;
}

int RtpInstance::rtpSetRtpSSRC(int ssrc)
{
	fSsrc=ssrc;											
	return OS_NO_ERROR;	
}
int RtpInstance::rtpGetRtpSSRC()
{
	return fSsrc;
}

int RtpInstance::rtpSendData(char *inBuf,int inLen)
{
	if(fRtpInstanceModel==-1){
		os_warn("not set sdk model\n");
		return OS_NOT_SET_SDK_MODEL;
	}		
	if(fRtpInstanceModel==TRANS_PUSH){
		fClient->sendData(inBuf,inLen);
	}else{
		os_error("trans protocol type error \n");
		return OS_TRANS_PROTOCOL_TYPE_ERROR;
	}
	return OS_NO_ERROR;
}

char *RtpInstance::getPushRIp()
{
	return fPushRIp;
}
unsigned short RtpInstance::getPushRPort()
{
	return fPushRPort;
}
unsigned short RtpInstance::getPushLPort()
{
	return fPushLPort;
}
int RtpInstance::getPushTransType()
{
	return fPushTransType;
}
int RtpInstance::getPushStreamType()
{
	return fPushStreamType;
}

int RtpInstance::getPushRtcpTimeout()
{
	return fPushRtcpTimeout;
}
rtpException RtpInstance::getPushRe()
{
	return fPushRe;
}
void *RtpInstance::getPushArg1()
{
	return fPushArg1;
}
rtpRecvRtcpData RtpInstance::getPushRecvRtcp()
{
	return fPushRecvRtcp;
}
void *RtpInstance::getPushArg2()
{
	return fPushArg2;
}
unsigned short RtpInstance::getRecvLPort()
{
	return fRecvLPort;
}
int RtpInstance::getRecvTransType()
{
	return fRecvTransType;
}
int RtpInstance::getRecvStreamType()
{

	return fRecvStreamType;
}

int RtpInstance::getRecvRtcpTimeout()
{
	return fRecvRtcpTimeout;
}
int RtpInstance::getRecvCache()
{
	return fRecvCache;
}

rtpRecvRtpData RtpInstance::getRecvRecvRtp()
{
	return fRecvRecvRtp;
}
void *RtpInstance::getRecvArg1()
{
	return fRecvArg1;
}
rtpRecvRtcpData RtpInstance::getRecvRecvRtcp()
{
	return fRecvRecvRtcp;
}
void *RtpInstance::getRecvArg2()
{
	return fRecvArg2;
}
rtpRecvMediaData RtpInstance::getRecvRecvMediaData()
{
	return fRecvRecvMediaData;
}
void *RtpInstance::getRecvArg3()
{
	return fRecvArg3;
}
rtpException RtpInstance::getRecvRe()
{
	return fRecvRe;
}
void *RtpInstance::getRecvArg4()
{
	return fRecvArg4;
}
int RtpInstance::getRtpInstanceModel()
{
	return fRtpInstanceModel;
}
int RtpInstance::setClearStatus()
{
	fClearStatus=true;
	return OS_NO_ERROR;
}
int RtpInstance::getClearStatus()
{
	return fClearStatus;
}

int RtpInstance::handleExecption(int type)
{
	if(fRtpInstanceModel==TRANS_RECV){
		if(fRecvRe){
			fRecvRe(type,fRecvArg4);
		}
	}else if(fRtpInstanceModel==TRANS_PUSH){
		os_error("===================== \n");
		if(fPushRe){
			fPushRe(type,fPushArg1);
		}	
	}
}

