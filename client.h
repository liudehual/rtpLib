#ifndef __CLIENT_H__
#define __CLIENT_H__
#include "common.h"
class RtpPacket;
class TcpRtpTask;
class UdpRtpTask;
class UdpRtcpTask;
class TcpClienSendTimer;
class Task;
class Client:public Object
{
	public:
		Client(void *root,void *parent);
		virtual ~Client();
		int initialization();
		int sendData(char *buf,int len);
		Task *getTask();
		Task *getUdpRtpTask();
		Task *getUdpRtcpTask();
	private:

		// trans_type=tcp
		TcpRtpTask *fTcpRtpTask;

		// trans_type=udp
		UdpRtpTask *fUdpRtpTask;
		UdpRtcpTask *fUdpRtcpTask;
};
#endif
