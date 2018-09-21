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
		// 初始化RtpInstance类对象
		int initialization();
		virtual int entry();
		NormalScheduler *getNormalScheduler();

		// 获取RtpInstance Id
		long getRtpInstanceId();

		// 启动RtpInstance
		int startRtpInstance();
		
		//	lPort				本地端口(接收端口 udp 则为接收端口 tcp则为监听端口)
		//	transType			传输类型(0-udp 1-tcp)
		//	streamType			流类型(0-h264 1-G711  2-ps)
		//	rtcpTimeout 		rtcp 超时时间(rtcp作为连接心跳使用(udp)，当超过指定时间没有收到rtcp包时，默认对端已离开) 
		//	rtpRecvRtpData		RTP数据接收回调函数
		//	arg1				rtpRecvRtpData	 私有参数
		//	rtpRecvRTCPData 	RTCP数据接收回调函数
		//	arg2				rtpRecvRTCPData  私有参数  
		//	rtpRecvMediaData	音视频数据接收回调函数(该回调函数返回音视频原始数据)(该回调函数不能为空)
		//	arg3				rtpRecvMediaData 私有参数
		//	rtpException		SDK异常回调
		//	arg4				rtpException 私有参数
		//	返回值					0 成功 其他值失败

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

		// 推流参数
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

		// 接收流参数
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

		// rtp 实例 模式(推送流还是接收流)
		// 0-接收 1-推送 其他值-无效
		int fRtpInstanceModel;
		bool fClearStatus;
};
#endif

