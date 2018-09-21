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

// RTP���ݰ����Ͷ�ʱ������ʱ����RTP���ݰ�
class RtpSendTimer:public Timer
{
	public:
		RtpSendTimer(void *root,void *parent,int interval);
		virtual ~RtpSendTimer(){}
		virtual int run();
};

// RTCP���Ͷ�ʱ������ʱ����RTCP���ݰ��������˷���SR,�����˷���RR
class RtcpSendTimer:public Timer
{	
	public:
		RtcpSendTimer(void *root,void *parent,int interval);
		virtual ~RtcpSendTimer(){}
		virtual int run();	
};

// RTCP���ն�ʱ������ʱ����Ƿ��յ����ݰ���û����Ĭ��ϵͳ�Ͽ�
class RtcpRecvTimer:public Timer
{	
	public:
		RtcpRecvTimer(void *root,void *parent,int interval);
		virtual ~RtcpRecvTimer(){}
		virtual int run();	
};

#endif
