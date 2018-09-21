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


		// 初始化server
		// tcp 启动监听
		// udp 绑定rtp/rtcp端口
		int initialization();

		// tcp 模式下接受新连接并创建客户端任务
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
