#include "rtpLib.h"
#include "error.h"
#include "sdkManager.h"
#include "common.h"
#include "log.h"
#include "rtpInstance.h"


static SdkManager *gManager=NULL;

// ����ֵ	0 �ɹ� ����ֵʧ��
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
// fileName ��־�ļ���
// level ��־�ȼ�
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

//  handler    rtpʵ�����
//  ����ֵ 0�ɹ�   ����ֵʧ��
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

//  instanceHandler   	RTPʵ�����
//  lPort 				���ض˿�(���ն˿� udp ��Ϊ���ն˿� tcp��Ϊ�����˿�)
//  transType 			��������(0-udp 1-tcp)
//	streamType			����������(0-h264 1-G711 2-PS)	
//  rtcpTimeout			rtcp ��ʱʱ��(rtcp��Ϊ��������ʹ�ã�������ָ��ʱ��û���յ�rtcp��ʱ��Ĭ�϶Զ����뿪) 
//  rtpRecvRtpData    	RTP���ݽ��ջص�����
//  rtpRecvRTCPData   	RTCP���ݽ��ջص�����
//  rtpRecvMediaData	����Ƶ���ݽ��ջص�����(�ûص�������������Ƶԭʼ����)(�ûص���������Ϊ��)
//  rtpException	    SDK�쳣�ص�
//  ����ֵ		     0�ɹ� ����ֵʧ��
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

//	instanceHandler 	 RTPʵ�����
//	rIp 				 Զ��ip
//	rPort				 Զ�˶˿�(udp Ϊ���Ͷ˿� tcpΪ���Ӷ˿�)
//	lPort				 ���ذ󶨶˿�
//	transType			 ��������(0-udp 1-tcp) ע:�ýӿ���TCPģʽ�¿��ܻ���2-3����ʱ
//	streamType			 ������(0-h264 1-G711 2-PS)
//	rtcpTimeout 		 rtcp ��ʱʱ�� (rtcp��Ϊ��������ʹ�ã�������ָ��ʱ��û���յ�rtcp��ʱ��Ĭ�϶Զ����뿪) 
//	rtpException		 SDK�쳣�ص�(�ûص������쳣�¼�)
//	arg1				 rtpException ˽�в���
//	rtpRecvRTCPData 	 RTCP���ݽ��ջص�����
//	arg2				 rtpRecvRTCPData  ˽�в���	
//	����ֵ					 0 �ɹ� ����ֵʧ��
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

//  instanceHandler   	RTPʵ�����
//  ssrc 			 	RTP SSRCֵ
//  ����ֵ	   		 0�ɹ� ����ֵʧ��
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


//  instanceHandler   RTPʵ�����
//  inBuf			 ���뻺����
//  intLen			 ����������ʵ����
//  ����ֵ		 0 �ɹ� ����ֵʧ��
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

//  ���RTPʵ��
//  instanceHandler  RTPʵ�����
//  ����ֵ0 �ɹ�  ����ֵʧ��
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

//  ���ݴ������ȡ����ԭ��
//  errorCode �ӿڷ��صĴ�����
char* rtpGetError(int errorCode)
{
	if(!gManager){
		os_error("not init sdk \n");
		return "sdk not init,can't get error information\n";
	}

	return "test error";
}

// ����SDK
// ����ֵ	0 �ɹ� ����ֵʧ��
int rtpCleanup()
{
	if(!gManager){
		return OS_NO_ERROR;
	}
	delete gManager;
	gManager=NULL;
	return OS_NO_ERROR;
}

