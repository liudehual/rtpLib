#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "error.h"


#define PS_HDR_LEN  		14  
#define SYS_HDR_LEN 		18  
#define PSM_HDR_LEN 		24  
#define PES_HDR_LEN 		19  
#define RTP_HDR_LEN 		12
#define RTCP_SR_HDR_LEN		8
#define RTCP_RR_HDR_LEN		8
#define RTP_TCP_HDR_LEN 	4
#define PS_SYS_MAP_SIZE 	30

#define RTP_MAX_PACKET_BUFF 1400

#define I_FRAME 	0	// idr frame
#define B_FRAME 	1   // b frame
#define P_FRAME 	2   // p frame

#define STREAM_H264 0 // h264 stream(video)
#define STREAM_G711 1 // g711 stream(audio)
#define STREAM_PS	2 // ps   stream

#define TRANS_RECV 	0 // start the server(接收流)
#define TRANS_PUSH 	1 // start the client(推送流)

#define TRANS_PROTOCOL_UDP	0 // udp
#define TRANS_PROTOCOL_TCP	1 // tcp


// rtp packet 
#define DATA_MAX_BUFFER_SIZE		3145728 // 3M
#define MAX_RTP_PACKET_BUFFER_SIZE 	1500	
#define MAX_RTP_PACKET_NUMBERS		DATA_MAX_BUFFER_SIZE/MAX_RTP_PACKET_BUFFER_SIZE+1 // 2000*1500=3M
#define MAX_RTP_PACKET_SIZE	    	1384 // rtp 包数据体长度

// rtcp packet
/*
	
类型 		缩写表示 								用途
200		SR（Sender Report）					发送端报告
201		RR（Receiver Report）					接收端报告
202		SDES（Source Description Items）		源点描述
203		BYE									结束传输
204		APP									特定应用
*/
#define SR_PACKET	200
#define RR_PACKET	201
#define SDES_PACKET	202
#define BYE_PACKET	203
#define APP_PACKET	204


class Object
{
	public:
		Object(void *root,void *parent):fRoot(root),fParent(parent){}
		virtual ~Object(){}
		void *getRoot(){return fRoot;}
		void *getParent(){return fParent;}
	private:
		void *fRoot;
		void *fParent;
};


#endif
