#ifndef __SDK_MANAGER_H__
#define __SDK_MANAGER_H__
#include "common.h"
#include <map>
#include "rtpLib.h"

class FastLog;
class Instance;
class FastLog;
class SdkManager:public Object
{
	public:
		SdkManager();
		virtual ~SdkManager();

		//��ʼ��SdkManager
		int initialization();

		// ��ȡlog����
		FastLog *getFastLog();

		// log �ļ�����console ���������̨,����������ļ�
		char *getLogTankFile();

		// ��ȡlog�ȼ�
		char *getLogLevel();

		// ����log �ļ�����Ĭ��Ϊconsole(����̨)
		int setLogFileName(char *fileName);

		// ����Log�ļ��ȼ�
		int setLogLevel(char *levelName);
		
		// ����log�ļ����͵ȼ�
		int setLog(char *logFileName,char *logLevel);
		
		// ����RtpInstance
		int createRtpInstance(void **handler);
		
		// ����RtpInstance
		int startRtpInstance(void *handler);
		//	instanceHandler 	RTPʵ�����
		//	lPort				���ض˿�(���ն˿� udp ��Ϊ���ն˿� tcp��Ϊ�����˿�)
		//	transType			��������(0-udp 1-tcp)
		//	streamType			������(0-h264 1-G711  2-ps)
		//	rtcpTimeout 		rtcp ��ʱʱ��(rtcp��Ϊ��������ʹ��(udp)��������ָ��ʱ��û���յ�rtcp��ʱ��Ĭ�϶Զ����뿪) 
		//	rtpRecvRtpData		RTP���ݽ��ջص�����
		//	arg1				rtpRecvRtpData	 ˽�в���
		//	rtpRecvRTCPData 	RTCP���ݽ��ջص�����
		//	arg2				rtpRecvRTCPData  ˽�в���  
		//	rtpRecvMediaData	����Ƶ���ݽ��ջص�����(�ûص�������������Ƶԭʼ����)(�ûص���������Ϊ��)
		//	arg3				rtpRecvMediaData ˽�в���
		//	rtpException		SDK�쳣�ص�
		//	arg4				rtpException ˽�в���
		//	����ֵ					0 �ɹ� ����ֵʧ��
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
		
		// ����SSRC
		int rtpSetRtpSSRC(void *handler,int ssrc);

		
		// ��������
		int rtpSendData(void *handler,char *inBuf,int inLen);
		
		// ����RtpInstance
		int rtpClearRTPInstance(void *handler);
	private:
		// rtp instance
		std::map<long,Instance *> fRtpInstanceContainer;

		// log info
		FastLog *fFastLog;
		char fLogTankFile[256];
		char fLogLevel[16];
};
#endif
