#ifndef __SERVER_H__
#define __SERVER_H__
#include "common.h"
class TcpRtpTask;
class TcpListenTask;

class UdpRtpTask;
class UdpRtcpTask;
class Server:public Object
{
	public:
		Server(void *root,void *parent);
		virtual ~Server();


		// ��ʼ��server
		// tcp ��������
		// udp ��rtp/rtcp�˿�
		int initialization();

		// tcp ģʽ�½��������Ӳ������ͻ�������
		int createClientTask(int socket);
	private:
		// trans_type=tcp
		TcpRtpTask *fTcpRtpTask;
		TcpListenTask *fListenTask;

		// trans_type=udp
		UdpRtpTask *fUdpRtpTask;
		UdpRtcpTask *fUdpRtcpTask;
};

#endif
