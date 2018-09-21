												RTP����SDK������Ƽ��ӿ�
1. ��ʼ��SDK:

// ����ֵ	0 �ɹ� ����ֵʧ��
Int rtpInit();

2.����һ��RTPʵ��:

//  ����ֵ 0 �ɹ�������ֵʧ��
Int rtpCreateRtpInstance(long *handler);

3.����Rtpʵ��Ϊ������ģʽ��

//  instanceHandler   	RTPʵ�����
//  lPort 				���ض˿�(���ն˿� udp ��Ϊ���ն˿� tcp��Ϊ�����˿�)
//  transType 			��������(0-udp 1-tcp)
//  streamType 		������(0-h264 1-G711  2-ps)
//  rtcpTimeout			rtcp ��ʱʱ��(rtcp��Ϊ��������ʹ��(udp)��������ָ��ʱ��û���յ�rtcp��ʱ��Ĭ�϶Զ����뿪) 
//  rtpRecvRtpData    	RTP���ݽ��ջص�����
//  arg1			    rtpRecvRtpData   ˽�в���
//  rtpRecvRTCPData   	RTCP���ݽ��ջص�����
//  arg2				rtpRecvRTCPData  ˽�в���  
//  rtpRecvMediaData	����Ƶ���ݽ��ջص�����(�ûص�������������Ƶԭʼ����)(�ûص���������Ϊ��)
//  arg3				rtpRecvMediaData ˽�в���
//  rtpException	          SDK�쳣�ص�
//  arg4				rtpException ˽�в���
//  ����ֵ		     0�ɹ� ����ֵʧ��
Int rtpSetRtpInstanceRecvModel(long instanceHandler,unsigned short lPort,int transType,int streamType,rtcpTimeout rt,rtpRecvRtpData recvRtp,void *arg1,rtpRecvRTCPData recvRtcp,void *arg2,rtpRecvMediaData recvMediaData,void *arg3,rtpException re,void *arg4);
ע��һ��RtpInstance ֻ��Ϊ������ģʽ��������ģʽ

4.����Rtpʵ��Ϊ������ģʽ��

//  instanceHandler   	 RTPʵ�����
//  rIp 				 Զ��ip
//  rPort 				 Զ�˶˿�(udp Ϊ���Ͷ˿� tcpΪ���Ӷ˿�)
//  lPort				 ���ذ󶨶˿�
//  transType 			 ��������(0-udp 1-tcp)
//  streamType 		 ������(0-h264 1-G711 2-PS)
//  rtcpTimeout			 rtcp ��ʱʱ�� (rtcp��Ϊ��������ʹ�ã�������ָ��ʱ��û���յ�rtcp��ʱ��Ĭ�϶Զ����뿪) 
//  rtpException          SDK�쳣�ص�(�ûص������쳣�¼�)
//  arg1				 rtpException ˽�в���
//  rtpRecvRTCPData   	RTCP���ݽ��ջص�����
//  arg2				rtpRecvRTCPData  ˽�в���  


//  ����ֵ			0�ɹ� ����ֵʧ��
Int rtpSetRtpInstancePushModel(long instanceHandler,char *rIp,unsigned short rPort,unsigned short lPort,int transType,int streamType,int rtcpTimeout rt,rtpException re,void *arg1,rtpRecvRTCPData recvRtcp,void *arg2);
ע��һ��RtpInstance ֻ��Ϊ������ģʽ��������ģʽ


5.����SSRC�ֶ�
//  instanceHandler   	RTPʵ�����
//  ssrc 			 	RTP SSRCֵ
//  ����ֵ	   		 0�ɹ� ����ֵʧ��
Int rtpSetRtpSSRC(long instanceHandler,int ssrc);
ע���ýӿڿ��ظ�����

6.ͬʱ���Ͷ�·��Ƶ��Ƶ:
   
//  instanceHandler   RTPʵ�����
//  inBuf			 ���뻺����
//  intLen			 ����������ʵ����
//  ����ֵ		 0 �ɹ� ����ֵʧ��
Int rtpSendData(long instanceHandler,char *inBuf,int inLen);

7.��ָ���˿ڽ�������(��·)(ע��ص�������ʽ,Ϊ����SDK�ڲ�Ĭ�ϴ���):

//  RTP���ݻص�����
//  �ûص���������������
//  outBuf 	���ݻ�����(�û�������SDK����������,���Ƽ�������ֱ�Ӳ����û�����)
//  outLen	���ݳ���
//  ssrc		��RTP������SSRC
//  arg 		�û��Զ�������ָ�룬��ָ��SDK��������
//  ����ֵ	0�ɹ�(�ûص������ɹ�ʱ����������ط���0������SDK����ݷ���ֵ�Ĳ�ͬ,�����ݽ��ж��δ���) 
typedef int (*rtpRecvRTPData)(char *outBuf,int outLen,int ssrc,void *arg);

8.�����ߴ���RTCP����(��·)(ע��ص�������ʽ,Ϊ����SDK�ڲ�Ĭ�ϴ���):

//  RTCP�ص�����
//  �ûص���������������
//  outBuf 	���ݻ�����(�û�������SDK����������)
//  outLen	���ݳ���
//  ssrc      ��RTCP������SSRC
//  arg 		�û��Զ�������ָ�룬��ָ��SDK���ᴦ��
//  ����ֵ	0�ɹ�(�ûص������ɹ�ʱ����������ط���0������SDK����ݷ���ֵ�Ĳ�ͬ,�����ݽ��ж��δ���) 
typedef int (*rtpRecvRTCPData)(char *outBuf,int outLen,int ssrc,void *arg);

9.�����߻�ȡ����Ƶ����(��·)(�ûص���������Ϊ��)
//  ý�����ݻص�����
//  �ûص���������������
//  outBuf 	���ݻ�����(�û�������SDK����������,���Ƽ�������ֱ�Ӳ����û�����)
//  outLen	���ݳ���
//  ssrc		��ý����������SSRC
//	mediaType	ý���ʽ(0-h264 1-G711)
//  arg 		�û��Զ�������ָ�룬��ָ��SDK���ᴦ��
//  ����ֵ	0�ɹ�(�ûص������ɹ�ʱ����������ط���0������SDK����ݷ���ֵ�Ĳ�ͬ,�����ݽ��ж��δ���) 
typedef int (*rtpRecvMediaData)(char *outBuf,int outLen,int ssrc,int mediaType,void *arg);

10.�쳣�ص�:
//  SDK�ڲ��쳣�ص�(��ҪΪ����Ͽ��¼�)
//  exceptionType �쳣����
//  arg 		�û��Զ�������ָ�룬��ָ��SDK���ᴦ��
typedef int (*rtpException)(int exceptionType,void *arg);
11.�������ں��ʵ�ʱ���ͷ�ʵ����Դ:

//  instanceHandler  RTPʵ�����
//  ����ֵ0 �ɹ�  ����ֵʧ��
Int rtpClearRTPInstance(long instanceHandler);

12.��ȡSDK����ԭ��:
//  ���ݴ������ȡ����ԭ��
//  errorCode �ӿڷ��صĴ�����
char* rtpGetError(int errorCode);
ע���ýӿڳ��ڿ��ܲ���ʵ��

13.����SDK��־��Ϣ��

// fileName ��־�ļ���
// level ��־�ȼ�
Int rtpSetLog(char *fileName,char *level);

14.����SDK:
// ����SDK
// ����ֵ	0 �ɹ� ����ֵʧ��
Int rtpCleanup();


ע�����ĵ������ƣ��������б䶯����