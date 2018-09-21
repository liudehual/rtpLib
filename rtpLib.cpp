#include "rtpLib.h"
#include "error.h"
#include "sdkManager.h"
#include "common.h"
#include "log.h"
#include "rtpInstance.h"


static SdkManager *gManager=NULL;

// 返回值	0 成功 其他值失败
int rtpInit()
{
	if(gManager){
		return OS_REPEAT_INIT_ERROR;
	}
	gManager=new SdkManager();
	int theError=OS_NO_ERROR;
	if((theError=gManager->initialization())!=OS_NO_ERROR){
		return theError;
	}
	os_trace("create sdk manager succeed \n");
	return theError;
}
// fileName 日志文件名
// level 日志等级
int rtpSetLog(char *fileName,char *level)
{
	int theError=OS_NO_ERROR;
	if(!gManager){
		os_error("not init sdk \n");
		return OS_NOT_INIT_ERROR;
	}
	if((theError=gManager->setLog(fileName,level))!=OS_NO_ERROR){
		return theError;
	}
	return theError;
}

//  handler    rtp实例句柄
//  返回值 0成功   其他值失败
int rtpCreateRtpInstance(void **handler)
{
	if(!gManager){
		os_error("not init sdk \n");
		return OS_NOT_INIT_ERROR;
	}
	
	int theError=OS_NO_ERROR;
	if((theError=gManager->createRtpInstance(handler))!=OS_NO_ERROR){
		return theError;
	}
	
	return OS_NO_ERROR;
}

//  instanceHandler   	RTP实例句柄
//  lPort 				本地端口(接收端口 udp 则为接收端口 tcp则为监听端口)
//  transType 			传输类型(0-udp 1-tcp)
//	streamType			接收流类型(0-h264 1-G711 2-PS)	
//  rtcpTimeout			rtcp 超时时间(rtcp作为连接心跳使用，当超过指定时间没有收到rtcp包时，默认对端已离开) 
//  rtpRecvRtpData    	RTP数据接收回调函数
//  rtpRecvRTCPData   	RTCP数据接收回调函数
//  rtpRecvMediaData	音视频数据接收回调函数(该回调函数返回音视频原始数据)(该回调函数不能为空)
//  rtpException	    SDK异常回调
//  返回值		     0成功 其他值失败
int rtpSetRtpInstanceRecvModel(void *handler,
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
	if(!gManager){
		os_error("not init sdk \n");
		return OS_NOT_INIT_ERROR;
	}
	int theError=OS_NO_ERROR;
	if((theError=gManager->rtpSetRtpInstanceRecvModel(handler,
															lPort,
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
															arg4))!=OS_NO_ERROR){
		return theError;

	}
	return theError;

}

//	instanceHandler 	 RTP实例句柄
//	rIp 				 远端ip
//	rPort				 远端端口(udp 为发送端口 tcp为连接端口)
//	lPort				 本地绑定端口
//	transType			 传输类型(0-udp 1-tcp) 注:该接口在TCP模式下可能会有2-3秒延时
//	streamType			 流类型(0-h264 1-G711 2-PS)
//	rtcpTimeout 		 rtcp 超时时间 (rtcp作为连接心跳使用，当超过指定时间没有收到rtcp包时，默认对端已离开) 
//	rtpException		 SDK异常回调(该回调返回异常事件)
//	arg1				 rtpException 私有参数
//	rtpRecvRTCPData 	 RTCP数据接收回调函数
//	arg2				 rtpRecvRTCPData  私有参数	
//	返回值					 0 成功 其他值失败
int rtpSetRtpInstancePushModel(void *handler,
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
	if(!gManager){
		os_error("not init sdk \n");
		return OS_NOT_INIT_ERROR;
	}
	int theError=OS_NO_ERROR;
	if((theError=gManager->rtpSetRtpInstancePushModel(handler,
															rIp,
															rPort,
															lPort,
															transType,
															streamType,
															rtcpTimeout,
															re,
															arg1,
															recvRtcp,
															arg2))!=OS_NO_ERROR){
		return OS_NO_ERROR;
	}
	return theError;
}
int rtpStartRtpInstance(void *handler)
{
	if(!gManager){
		os_error("not init sdk \n");
		return OS_NOT_INIT_ERROR;
	}
	int theError=OS_NO_ERROR;
	if((theError=gManager->startRtpInstance(handler))!=OS_NO_ERROR){
		return OS_NO_ERROR;
	}
	return theError;
}

//  instanceHandler   	RTP实例句柄
//  ssrc 			 	RTP SSRC值
//  返回值	   		 0成功 其他值失败
int rtpSetRtpSSRC(void *handler,int ssrc)
{
	if(!gManager){
		os_error("not init sdk \n");
		return OS_NOT_INIT_ERROR;
	}
	int theError=OS_NO_ERROR;
	if((theError=gManager->rtpSetRtpSSRC(handler,ssrc))!=OS_NO_ERROR){
		return theError;
	}
	return theError;
}


//  instanceHandler   RTP实例句柄
//  inBuf			 输入缓冲区
//  intLen			 输入数据真实长度
//  返回值		 0 成功 其他值失败
int rtpSendData(void *handler,char *inBuf,int inLen)
{
	if(!gManager){
		os_error("not init sdk \n");
		return OS_NOT_INIT_ERROR;
	}
	
	int theError=OS_NO_ERROR;
	if((theError=gManager->rtpSendData(handler,inBuf,inLen))!=OS_NO_ERROR){
		return theError;
	}
	return theError;
}

//  清除RTP实例
//  instanceHandler  RTP实例句柄
//  返回值0 成功  其他值失败
int rtpClearRTPInstance(void **handler)
{
	if(!gManager){
		os_error("not init sdk \n");
		return OS_NOT_INIT_ERROR;
	}
	void *tmpHandler=*handler;
	*handler=NULL;
	int theError=OS_NO_ERROR;
	if((theError=gManager->rtpClearRTPInstance(tmpHandler))!=OS_NO_ERROR){
		return theError;
	}
	return theError;
}

//  根据错误码获取出错原因
//  errorCode 接口返回的错误码
char* rtpGetError(int errorCode)
{
	if(!gManager){
		os_error("not init sdk \n");
		return "sdk not init,can't get error information\n";
	}

	return "test error";
}

// 清理SDK
// 返回值	0 成功 其他值失败
int rtpCleanup()
{
	if(!gManager){
		return OS_NO_ERROR;
	}
	delete gManager;
	gManager=NULL;
	return OS_NO_ERROR;
}

