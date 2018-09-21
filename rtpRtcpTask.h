#ifndef __RTP_RTCP_TASK_H__
#define __RTP_RTCP_TASK_H__
#include "task.h"
#include "tcpSocket.h"
class Packet;
class Timer;
class TcpListenTask:public Task
{
	public:
		TcpListenTask(void *root,void *parent);
		virtual ~TcpListenTask();
		int initSocket();
		virtual int run();
};
// 定义和抽象了一些RTP接口
class RtpTask:public Task
{
	public:
		RtpTask(void *root,void *parent);
		virtual ~RtpTask();
		virtual int createDecodePacket(); 
		virtual int createEncodePacket();
		virtual int run()=0;
		virtual int handleRtpData(char *buf,int len);
		virtual int sendData(char *buf,int len,unsigned int *flushLen)=0;
		Packet *getRtpPacket();
	protected:
		Packet *fRtpPacket;
		Timer *fTimer;
};



// TCP模式下，rtp/rtcp均由该类处理
// 包含发送和接收功能
class TcpRtpTask:public RtpTask
{
	public:
		TcpRtpTask(void *root,void *parent);
		virtual ~TcpRtpTask();
		virtual int setRecvModel();
		virtual int run();
		int writeData(char *buf,int len);
		int handleRtcpData(char *buf,int len);
		int sendData(char *buf,int len,unsigned int *flushLen);
	private:		
		char fBuf[8196];
		int fLen;
		
};

// udpRtpTask 包含发送和接收功能
// 收到RTP数据包后交给特定RtpPacket类对象处理
class UdpRtpTask:public RtpTask
{
	public:
		UdpRtpTask(void *root,void *parent);
		virtual ~UdpRtpTask();
		virtual int setRecvModel(); 
		int bind(unsigned short port);
		int writeData(char *buf,int len);
		virtual int run();
		int sendData(char *buf,int len,unsigned int *flushLen);
	private:
		char fBuf[1500];
		unsigned int fLen;
};

// UdpRtcpTask 具有发送和接收功能
// 收到RTCP数据包后暂时不做处理，只做心跳使用
class UdpRtcpTask:public Task
{
	public:
		UdpRtcpTask(void *root,void *parent);
		virtual ~UdpRtcpTask();
		int setRecvModel(); 
		int bind(unsigned short port);
		virtual int run();
		virtual int handleRtcpData(char *buf,int len);
		int sendData(char *buf,int len,unsigned int *flushLen);
	public:
		bool fRtpConnection;
	private:
		char fBuf[1500];
		unsigned int fLen;
		Timer *fRecvTimer;
		Timer *fSendTimer;
};

#endif
