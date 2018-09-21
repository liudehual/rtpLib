#ifndef __TIMER_H__
#define __TIMER_H__
#include "common.h"
#include "osHeap.h"
class RtpPacket;
class RtcpPacket;
class Timer:public Object
{
	public:
		Timer(void *root,void *parent,int interval);
		virtual ~Timer();
		virtual int run()=0;
		OSHeapElem *getHeapElem();
		int getInterval();
		int setInterval(int interval);
		void start();
		
	private:
		OSHeapElem fElem;
	protected:
		int fInterval;
};
class TestTimer:public Timer
{
	public:
		TestTimer(void *root,void *parent,int timeout):Timer(root,parent,timeout){}
		virtual ~TestTimer(){}
		int setContent(char *content){strcpy(contentBuf,content);}
		int run()
		{
			fprintf(stderr,"%s \n",contentBuf);
			return fInterval;
		}
	private:
		char contentBuf[128];
};

// RTP数据包发送定时器，超时发送RTP数据包
class RtpSendTimer:public Timer
{
	public:
		RtpSendTimer(void *root,void *parent,int interval);
		virtual ~RtpSendTimer(){}
		virtual int run();
};

// RTCP发送定时器，超时发送RTCP数据包，推流端发送SR,收流端发送RR
class RtcpSendTimer:public Timer
{	
	public:
		RtcpSendTimer(void *root,void *parent,int interval);
		virtual ~RtcpSendTimer(){}
		virtual int run();	
};

// RTCP接收定时器，超时检查是否收到数据包，没有则默认系统断开
class RtcpRecvTimer:public Timer
{	
	public:
		RtcpRecvTimer(void *root,void *parent,int interval);
		virtual ~RtcpRecvTimer(){}
		virtual int run();	
};

#endif
