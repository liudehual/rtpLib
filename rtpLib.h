#ifndef __RTP_LIB_H__
#define __RTP_LIB_H__
/*************************************************************
*
* 	Date:	2018/09/13
* author:	GCT
*
*
**************************************************************/
// log 信息详细程度	verbose > info > trace > warn > error
#define G_LOG_LEVEL_VERBOSE 	"verbose"
#define G_LOG_LEVEL_INFO		"info"
#define G_LOG_LEVEL_TRACE 		"trace"
#define G_LOG_LEVEL_WARN		"warn"
#define G_LOG_LEVEL_ERROR		"error"

// 流类型
#define STREAM_H264_TYPE		0
#define STREAM_G711_TYPE		1
#define STREAM_PS_TYPE			2

// 网络协议类型
#define SOCKET_UDP_TYPE  		0
#define SOCKET_TCP_TYPE			1

//  RTP数据回调函数
//  该回调函数不允许阻塞
//  outBuf 	数据缓冲区(该缓冲区由SDK传给调用者,不推荐调用者直接操作该缓冲区)
//  outLen	数据长度
//  ssrc		该RTP所属的SSRC
//  arg 		用户自定义数据指针，该指针SDK不作处理
//  返回值	0成功(该回调函数成功时，调用者务必返回0，否则SDK会根据返回值的不同,对数据进行二次处理) 
typedef int (*rtpRecvRtpData)(char *outBuf,int outLen,int ssrc,void *arg);

//  RTCP回调函数
//  该回调函数不允许阻塞
//  outBuf 	数据缓冲区(该缓冲区由SDK传给调用者)
//  outLen	数据长度
//  ssrc      该RTCP所属的SSRC
//  arg 		用户自定义数据指针，该指针SDK不会处理
//  返回值	0成功(该回调函数成功时，调用者务必返回0，否则SDK会根据返回值的不同,对数据进行二次处理) 
typedef int (*rtpRecvRtcpData)(char *outBuf,int outLen,int ssrc,void *arg);

//  媒体数据回调函数
//  该回调函数不允许阻塞
//  outBuf 	数据缓冲区(该缓冲区由SDK传给调用者,不推荐调用者直接操作该缓冲区)
//  outLen	数据长度
//  ssrc		该媒体流所属的SSRC
//	mediaType	媒体格式(0-h264 1-G711 2-ps)
//  arg 		用户自定义数据指针，该指针SDK不会处理
//  返回值	0成功(该回调函数成功时，调用者务必返回0，否则SDK会根据返回值的不同,对数据进行二次处理) 
typedef int (*rtpRecvMediaData)(char *outBuf,int outLen,int ssrc,int mediaType,void *arg);

//  SDK内部异常回调(主要为网络断开事件)
//  exceptionType 异常类型
//  arg 		用户自定义数据指针，该指针SDK不会处理
typedef int (*rtpException)(int exceptionType,void *arg);

// 初始化SDK(该接口必须首先被调用)
// 返回值	0 成功 其他值失败
int rtpInit();

// 设置SDK日志(默认SDK会打印到控制台，日志等级为trace)
// fileName 日志文件名
// level 日志等级
// 用户没有调用该接口，则默认打印到控制台，等级为trace
int rtpSetLog(char *fileName,char *level);

//  创建Rtp实例
//  handler    rtp实例句柄 注:调用者需要保证handler的一致性，否则将引起无法预料的后果.
//  返回值 0成功   其他值失败
int rtpCreateRtpInstance(void **handler);

//  设置Rtp实例为接收流模式
//  instanceHandler   	RTP实例句柄
//  lPort 				本地端口(接收端口 udp 则为接收端口 tcp则为监听端口)
//  transType 			传输类型(0-udp 1-tcp)
//	streamType			接收的数据流类型(0-h264 1-G711 2-PS)
//  rtcpTimeout			rtcp 超时时间(rtcp作为连接心跳使用(udp)，当超过指定时间没有收到rtcp包时，默认对端已离开) 
//  recvCache			max=3*1024 min=100
//  rtpRecvRtpData    	RTP数据接收回调函数
//  arg1			    rtpRecvRtpData   私有参数
//  rtpRecvRTCPData   	RTCP数据接收回调函数
//  arg2				rtpRecvRTCPData  私有参数  
//  rtpRecvMediaData	音视频数据接收回调函数(该回调函数返回音视频原始数据)(该回调函数不能为空)
//  arg3				rtpRecvMediaData 私有参数
//  rtpException	    SDK异常回调
//  arg4				rtpException 私有参数
//  返回值		            0 成功 其他值失败
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
 
//  设置Rtp实例为推送流模式
//	instanceHandler 	 RTP实例句柄
//	rIp 				 远端ip
//	rPort				 远端端口(udp 为发送端口 tcp为连接端口)
//	lPort				 本地绑定端口
//	transType			 传输类型(0-udp 1-tcp)
//	rtcpTimeout 		 rtcp 超时时间 (rtcp作为连接心跳使用，当超过指定时间没有收到rtcp包时，默认对端已离开) 
//	rtpException		 SDK异常回调(该回调返回异常事件)
//	arg1				 rtpException 私有参数
//	rtpRecvRTCPData 	 RTCP数据接收回调函数
//	arg2				 rtpRecvRTCPData  私有参数  
//	返回值			         0 成功 其他值失败
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


//  设置rtp实例SSRC值
//  instanceHandler   	RTP实例句柄
//  ssrc 			 	RTP SSRC值
//  返回值	   		 0成功 其他值失败
int rtpSetRtpSSRC(void *handler,int ssrc);

//  启动Rtp实例
//  instanceHandler   	RTP实例句柄
int rtpStartRtpInstance(void *handler);


//  向指定Rtp实例写入数据(在Rtp推送流模式下可用)
//  instanceHandler   RTP实例句柄
//  inBuf			  输入缓冲区
//  intLen			  输入数据真实长度
//	
//  返回值		 0 成功 其他值失败
//  注:该接口目前只接受一帧完整的数据，不支持随意写入
//  注:调用rtpClearRTPInstance释放Rtp实例前请不要继续写入数据
int rtpSendData(void *handler,char *inBuf,int inLen);

//  清理指定Rtp实例
//  
//  instanceHandler  RTP实例句柄 注:释放后的 handler 在重新调用 rtpCreateRtpInstance 函数前不可再用
//  调用该接口前请不要继续做其他操作
//  返回值0 成功  其他值失败
int rtpClearRTPInstance(void **handler);

//  获取SDK错误码
//  根据错误码获取出错原因
//  errorCode 接口返回的错误码
char* rtpGetError(int errorCode);

// 清理SDK
// 返回值	0 成功 其他值失败
int rtpCleanup();

#endif
