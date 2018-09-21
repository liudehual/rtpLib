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

		//初始化SdkManager
		int initialization();

		// 获取log对象
		FastLog *getFastLog();

		// log 文件名，console 输出到控制台,其他输出到文件
		char *getLogTankFile();

		// 获取log等级
		char *getLogLevel();

		// 设置log 文件名，默认为console(控制台)
		int setLogFileName(char *fileName);

		// 设置Log文件等级
		int setLogLevel(char *levelName);
		
		// 设置log文件名和等级
		int setLog(char *logFileName,char *logLevel);
		
		// 创建RtpInstance
		int createRtpInstance(void **handler);
		
		// 启动RtpInstance
		int startRtpInstance(void *handler);
		//	instanceHandler 	RTP实例句柄
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
		
		// 设置SSRC
		int rtpSetRtpSSRC(void *handler,int ssrc);

		
		// 发送数据
		int rtpSendData(void *handler,char *inBuf,int inLen);
		
		// 清理RtpInstance
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
