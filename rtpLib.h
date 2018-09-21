#ifndef __RTP_LIB_H__
#define __RTP_LIB_H__
/*************************************************************
*
* 	Date:	2018/09/13
* author:	GCT
*
*
**************************************************************/
// log ��Ϣ��ϸ�̶�	verbose > info > trace > warn > error
#define G_LOG_LEVEL_VERBOSE 	"verbose"
#define G_LOG_LEVEL_INFO		"info"
#define G_LOG_LEVEL_TRACE 		"trace"
#define G_LOG_LEVEL_WARN		"warn"
#define G_LOG_LEVEL_ERROR		"error"

// ������
#define STREAM_H264_TYPE		0
#define STREAM_G711_TYPE		1
#define STREAM_PS_TYPE			2

// ����Э������
#define SOCKET_UDP_TYPE  		0
#define SOCKET_TCP_TYPE			1

//  RTP���ݻص�����
//  �ûص���������������
//  outBuf 	���ݻ�����(�û�������SDK����������,���Ƽ�������ֱ�Ӳ����û�����)
//  outLen	���ݳ���
//  ssrc		��RTP������SSRC
//  arg 		�û��Զ�������ָ�룬��ָ��SDK��������
//  ����ֵ	0�ɹ�(�ûص������ɹ�ʱ����������ط���0������SDK����ݷ���ֵ�Ĳ�ͬ,�����ݽ��ж��δ���) 
typedef int (*rtpRecvRtpData)(char *outBuf,int outLen,int ssrc,void *arg);

//  RTCP�ص�����
//  �ûص���������������
//  outBuf 	���ݻ�����(�û�������SDK����������)
//  outLen	���ݳ���
//  ssrc      ��RTCP������SSRC
//  arg 		�û��Զ�������ָ�룬��ָ��SDK���ᴦ��
//  ����ֵ	0�ɹ�(�ûص������ɹ�ʱ����������ط���0������SDK����ݷ���ֵ�Ĳ�ͬ,�����ݽ��ж��δ���) 
typedef int (*rtpRecvRtcpData)(char *outBuf,int outLen,int ssrc,void *arg);

//  ý�����ݻص�����
//  �ûص���������������
//  outBuf 	���ݻ�����(�û�������SDK����������,���Ƽ�������ֱ�Ӳ����û�����)
//  outLen	���ݳ���
//  ssrc		��ý����������SSRC
//	mediaType	ý���ʽ(0-h264 1-G711 2-ps)
//  arg 		�û��Զ�������ָ�룬��ָ��SDK���ᴦ��
//  ����ֵ	0�ɹ�(�ûص������ɹ�ʱ����������ط���0������SDK����ݷ���ֵ�Ĳ�ͬ,�����ݽ��ж��δ���) 
typedef int (*rtpRecvMediaData)(char *outBuf,int outLen,int ssrc,int mediaType,void *arg);

//  SDK�ڲ��쳣�ص�(��ҪΪ����Ͽ��¼�)
//  exceptionType �쳣����
//  arg 		�û��Զ�������ָ�룬��ָ��SDK���ᴦ��
typedef int (*rtpException)(int exceptionType,void *arg);

// ��ʼ��SDK(�ýӿڱ������ȱ�����)
// ����ֵ	0 �ɹ� ����ֵʧ��
int rtpInit();

// ����SDK��־(Ĭ��SDK���ӡ������̨����־�ȼ�Ϊtrace)
// fileName ��־�ļ���
// level ��־�ȼ�
// �û�û�е��øýӿڣ���Ĭ�ϴ�ӡ������̨���ȼ�Ϊtrace
int rtpSetLog(char *fileName,char *level);

//  ����Rtpʵ��
//  handler    rtpʵ����� ע:��������Ҫ��֤handler��һ���ԣ����������޷�Ԥ�ϵĺ��.
//  ����ֵ 0�ɹ�   ����ֵʧ��
int rtpCreateRtpInstance(void **handler);

//  ����Rtpʵ��Ϊ������ģʽ
//  instanceHandler   	RTPʵ�����
//  lPort 				���ض˿�(���ն˿� udp ��Ϊ���ն˿� tcp��Ϊ�����˿�)
//  transType 			��������(0-udp 1-tcp)
//	streamType			���յ�����������(0-h264 1-G711 2-PS)
//  rtcpTimeout			rtcp ��ʱʱ��(rtcp��Ϊ��������ʹ��(udp)��������ָ��ʱ��û���յ�rtcp��ʱ��Ĭ�϶Զ����뿪) 
//  recvCache			max=3*1024 min=100
//  rtpRecvRtpData    	RTP���ݽ��ջص�����
//  arg1			    rtpRecvRtpData   ˽�в���
//  rtpRecvRTCPData   	RTCP���ݽ��ջص�����
//  arg2				rtpRecvRTCPData  ˽�в���  
//  rtpRecvMediaData	����Ƶ���ݽ��ջص�����(�ûص�������������Ƶԭʼ����)(�ûص���������Ϊ��)
//  arg3				rtpRecvMediaData ˽�в���
//  rtpException	    SDK�쳣�ص�
//  arg4				rtpException ˽�в���
//  ����ֵ		            0 �ɹ� ����ֵʧ��
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
											void *arg4);
 
//  ����Rtpʵ��Ϊ������ģʽ
//	instanceHandler 	 RTPʵ�����
//	rIp 				 Զ��ip
//	rPort				 Զ�˶˿�(udp Ϊ���Ͷ˿� tcpΪ���Ӷ˿�)
//	lPort				 ���ذ󶨶˿�
//	transType			 ��������(0-udp 1-tcp)
//	rtcpTimeout 		 rtcp ��ʱʱ�� (rtcp��Ϊ��������ʹ�ã�������ָ��ʱ��û���յ�rtcp��ʱ��Ĭ�϶Զ����뿪) 
//	rtpException		 SDK�쳣�ص�(�ûص������쳣�¼�)
//	arg1				 rtpException ˽�в���
//	rtpRecvRTCPData 	 RTCP���ݽ��ջص�����
//	arg2				 rtpRecvRTCPData  ˽�в���  
//	����ֵ			         0 �ɹ� ����ֵʧ��
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
												void *arg2);


//  ����rtpʵ��SSRCֵ
//  instanceHandler   	RTPʵ�����
//  ssrc 			 	RTP SSRCֵ
//  ����ֵ	   		 0�ɹ� ����ֵʧ��
int rtpSetRtpSSRC(void *handler,int ssrc);

//  ����Rtpʵ��
//  instanceHandler   	RTPʵ�����
int rtpStartRtpInstance(void *handler);


//  ��ָ��Rtpʵ��д������(��Rtp������ģʽ�¿���)
//  instanceHandler   RTPʵ�����
//  inBuf			  ���뻺����
//  intLen			  ����������ʵ����
//	
//  ����ֵ		 0 �ɹ� ����ֵʧ��
//  ע:�ýӿ�Ŀǰֻ����һ֡���������ݣ���֧������д��
//  ע:����rtpClearRTPInstance�ͷ�Rtpʵ��ǰ�벻Ҫ����д������
int rtpSendData(void *handler,char *inBuf,int inLen);

//  ����ָ��Rtpʵ��
//  
//  instanceHandler  RTPʵ����� ע:�ͷź�� handler �����µ��� rtpCreateRtpInstance ����ǰ��������
//  ���øýӿ�ǰ�벻Ҫ��������������
//  ����ֵ0 �ɹ�  ����ֵʧ��
int rtpClearRTPInstance(void **handler);

//  ��ȡSDK������
//  ���ݴ������ȡ����ԭ��
//  errorCode �ӿڷ��صĴ�����
char* rtpGetError(int errorCode);

// ����SDK
// ����ֵ	0 �ɹ� ����ֵʧ��
int rtpCleanup();

#endif
