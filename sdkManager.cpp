#include <string.h>
#include "sdkManager.h"
#include "fastLog.h"
#include "log.h"
#include "rtpInstance.h"
#include "utils.h"

SdkManager::SdkManager():Object(this,this)
{
	memset(fLogTankFile,'\0',sizeof(fLogTankFile));
	strcpy(fLogTankFile,"console");
	
	memset(fLogLevel,'\0',sizeof(fLogLevel));
	strcpy(fLogLevel,"trace");

	fRtpInstanceContainer.clear();
}
SdkManager::~SdkManager()
{
	
}
int SdkManager::initialization()
{
	int theError=OS_NO_ERROR;

	fFastLog=new FastLog(this,this);
	if((theError=initializationLog(this))!=OS_NO_ERROR){
		return theError;
	}
	
	return theError;
}

FastLog *SdkManager::getFastLog()
{
	return fFastLog;
}
char *SdkManager::getLogTankFile()
{
	return fLogTankFile;
}
char *SdkManager::getLogLevel()
{
	return fLogLevel;
}
int SdkManager::setLogFileName(char *fileName)
{
	if(!fileName 
			|| strlen(fileName)==0){
		fprintf(stderr,"log file name error \n");
		return LOG_FILE_NAME_ERROR;
	}
	int theError=OS_NO_ERROR;
	memset(fLogTankFile,'\0',sizeof(fLogTankFile));
	strcpy(fLogTankFile,fileName);
	return theError;
}
int SdkManager::setLogLevel(char *levelName)
{
	if(!levelName 
			|| strlen(levelName)==0){
		fprintf(stderr,"log level error \n");
		return LOG_LEVEL_ERROR;
	}
	memset(fLogLevel,'\0',sizeof(fLogLevel));
	strcpy(fLogLevel,levelName);
	return OS_NO_ERROR;
}
int SdkManager::setLog(char *logFileName,char *logLevel)
{
	int theError=OS_NO_ERROR;

	// init log
	if((theError=setLogFileName(logFileName)) != OS_NO_ERROR){
		return theError;
	}
	if((theError=setLogLevel(logLevel)) != OS_NO_ERROR){
		return theError;
	}
	
	if((theError=initializationLog(this)) != OS_NO_ERROR){
		return theError;
	}
	
	return theError;
	
}
int SdkManager::createRtpInstance(void **handler)
{
	RtpInstance *theInstance=new RtpInstance(this,this);
	theInstance->initialization();
	theInstance->start();
	*handler=theInstance;
	return OS_NO_ERROR;
}
int SdkManager::startRtpInstance(void * handler)
{
	if(!handler){
		return OS_HANDLER_NOT_INIT;
	}
	RtpInstance *theInstance=(RtpInstance *)handler;
	return theInstance->startRtpInstance();
}

//  instanceHandler   	RTPʵ�����
//  lPort 				���ض˿�(���ն˿� udp ��Ϊ���ն˿� tcp��Ϊ�����˿�)
//  transType 			��������(0-udp 1-tcp)
//  streamType 		    ������(0-h264 1-G711    2-ps)
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
int SdkManager::rtpSetRtpInstanceRecvModel(void *handler,
														unsigned short lPort,
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
	int theError=OS_NO_ERROR;
	if(!handler){
		return OS_HANDLER_NOT_INIT;
	}

	RtpInstance *theInstance=(RtpInstance *)handler;
	if((theError=theInstance->rtpSetRtpInstanceRecvModel(lPort, 
														transType, 
														streamType,
														rtcpTimeout,
														recvCache,
														recvRtp, 
														arg1,
														recvRtcp,
														arg2,
														recvMediaData,
														arg3,
														re,
														arg4)) != OS_NO_ERROR){
		return theError;

	}
	return theError;
}
														
//	instanceHandler 	 RTPʵ�����
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
int SdkManager::rtpSetRtpInstancePushModel(void *handler,
														char *rIp,
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
	int theError=OS_NO_ERROR;
	if(!handler){
		return OS_HANDLER_NOT_INIT;
	}

	RtpInstance *theInstance=(RtpInstance *)handler;	
	if((theError=theInstance->rtpSetRtpInstancePushModel(rIp, 
															rPort, 
															lPort, 
															transType, 
															streamType,
															rtcpTimeout,
															re,
															arg1,
															recvRtcp,
															arg2))!=OS_NO_ERROR){
		return theError;
	}
	return theError;
}
														
int SdkManager::rtpSetRtpSSRC(void *handler,int ssrc)
{
	if(!handler){
		return OS_HANDLER_NOT_INIT;
	}
	int theError=OS_NO_ERROR;

	RtpInstance *theInstance=(RtpInstance *)handler;	
	theInstance->rtpSetRtpSSRC(ssrc);
	return theError;
}

int SdkManager::rtpSendData(void *handler,char *inBuf,int inLen)
{
	if(!handler){
		return OS_HANDLER_NOT_INIT;
	}

	int theError=OS_NO_ERROR;
	RtpInstance *theInstance=(RtpInstance *)handler;	
	if((theError=theInstance->rtpSendData(inBuf,inLen))!=OS_NO_ERROR){
		return theError;
	}
	return theError;
}

int SdkManager::rtpClearRTPInstance(void *handler)
{
	if(!handler){
		return OS_HANDLER_NOT_INIT;
	}

	RtpInstance *theInstance=(RtpInstance *)handler;
	theInstance->setClearStatus();
	
	return OS_NO_ERROR;
}


