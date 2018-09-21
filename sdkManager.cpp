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

//  instanceHandler   	RTP实例句柄
//  lPort 				本地端口(接收端口 udp 则为接收端口 tcp则为监听端口)
//  transType 			传输类型(0-udp 1-tcp)
//  streamType 		    流类型(0-h264 1-G711    2-ps)
//  rtcpTimeout			rtcp 超时时间(rtcp作为连接心跳使用(udp)，当超过指定时间没有收到rtcp包时，默认对端已离开) 
//  rtpRecvRtpData    	RTP数据接收回调函数
//  arg1			    rtpRecvRtpData   私有参数
//  rtpRecvRTCPData   	RTCP数据接收回调函数
//  arg2				rtpRecvRTCPData  私有参数  
//  rtpRecvMediaData	音视频数据接收回调函数(该回调函数返回音视频原始数据)(该回调函数不能为空)
//  arg3				rtpRecvMediaData 私有参数
//  rtpException	    SDK异常回调
//  arg4				rtpException 私有参数
//  返回值		            0 成功 其他值失败
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
														
//	instanceHandler 	 RTP实例句柄
//	rIp 				 远端ip
//	rPort				 远端端口(udp 为发送端口 tcp为连接端口)
//	lPort				 本地绑定端口
//	transType			 传输类型(0-udp 1-tcp)
//	streamType			 流类型(0-h264 1-G711)
//	rtcpTimeout 		 rtcp 超时时间 (rtcp作为连接心跳使用，当超过指定时间没有收到rtcp包时，默认对端已离开) 
//	rtpException		 SDK异常回调(该回调返回异常事件)
//	arg1				 rtpException 私有参数
//	rtpRecvRTCPData 	 RTCP数据接收回调函数
//	arg2				 rtpRecvRTCPData  私有参数	
//	返回值					 0 成功 其他值失败
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


