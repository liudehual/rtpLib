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
// ����ͳ�����һЩRTP�ӿ�
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



// TCPģʽ�£�rtp/rtcp���ɸ��ദ��
// �������ͺͽ��չ���
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

// udpRtpTask �������ͺͽ��չ���
// �յ�RTP���ݰ��󽻸��ض�RtpPacket�������
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

// UdpRtcpTask ���з��ͺͽ��չ���
// �յ�RTCP���ݰ�����ʱ��������ֻ������ʹ��
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
