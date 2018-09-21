#ifndef __RTP_INSTANCE_H__
#define __RTP_INSTANCE_H__
#include "instance.h"
#include "rtpLib.h"
#include "thread.h"
class Server;
class Client;
class NormalScheduler;
class Timer;
class RtpInstance:public Thread,public Instance
{
	public:
		RtpInstance(void *root,void *parent);
		virtual ~RtpInstance();
		// ��ʼ��RtpInstance�����
		int initialization();
		virtual int entry();
		NormalScheduler *getNormalScheduler();

		// ��ȡRtpInstance Id
		long getRtpInstanceId();

		// ����RtpInstance
		int startRtpInstance();
		
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

		int rtpSetRtpInstanceRecvModel(			unsigned short lPort,
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

		int rtpSetRtpInstancePushModel(			char *rIp,
        											unsigned short rPort,
        											unsigned short lPort,
        											int transType,
        											int streamType,
        											int rtcpTimeout,
        											rtpException re,
        											void *arg1,
        											rtpRecvRtcpData recvRtcp,
        											void *arg2);
		int rtpSetRtpSSRC(int ssrc);
		int rtpGetRtpSSRC();
		int rtpSendData(char *inBuf,int inLen);

		char *getPushRIp();
		unsigned short getPushRPort();
		unsigned short getPushLPort();
		int getPushTransType();
		int getPushStreamType();
		int getPushRtcpTimeout();
		rtpException getPushRe();
		void *getPushArg1();
		rtpRecvRtcpData getPushRecvRtcp();
		void *getPushArg2();

		unsigned short getRecvLPort();
		int getRecvTransType();
		int getRecvStreamType();
		int getRecvRtcpTimeout();
		int getRecvCache();
		rtpRecvRtpData getRecvRecvRtp();
		void *getRecvArg1();
		rtpRecvRtcpData getRecvRecvRtcp();
		void *getRecvArg2();
		rtpRecvMediaData getRecvRecvMediaData();
		void *getRecvArg3();
		rtpException getRecvRe();
		void *getRecvArg4();

		int getRtpInstanceModel();
		int setClearStatus();
		int getClearStatus();
		int handleExecption(int type);
	private:
		Server *fServer;
		Client *fClient;
		NormalScheduler *fScheduler;
		
		int fTransType;	// 0-tcp 1-udp
		long fId;
		int fSsrc;

		// ��������
		char *fPushRIp;
		unsigned short fPushRPort;
		unsigned short fPushLPort;
		int fPushTransType;
		int fPushStreamType;
		int fPushRtcpTimeout;
		rtpException fPushRe;
		void *fPushArg1;
		rtpRecvRtcpData fPushRecvRtcp;
		void *fPushArg2;

		// ����������
		unsigned short fRecvLPort;
		int fRecvTransType;
		int fRecvStreamType;
		int fRecvRtcpTimeout;
		int fRecvCache;
		rtpRecvRtpData fRecvRecvRtp;
		void *fRecvArg1;
		rtpRecvRtcpData fRecvRecvRtcp;
		void *fRecvArg2;
		rtpRecvMediaData fRecvRecvMediaData;
		void *fRecvArg3;
		rtpException fRecvRe;
		void *fRecvArg4;

		// rtp ʵ�� ģʽ(���������ǽ�����)
		// 0-���� 1-���� ����ֵ-��Ч
		int fRtpInstanceModel;
		bool fClearStatus;
};
#endif

