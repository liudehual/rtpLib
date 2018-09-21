#ifndef __MULTIPLES_PS_H__
#define __MULTIPLES_PS_H__
#include "common.h"
#include "utils.h"
#define MAX_PES_LEN 	60000
#define PS_STREAM_ID 	0xE0
#define VIDEO_FREQUENCY 90000
#define MAX_VIDEO_LEN 	2097152

class MultiplexPs:public Object
{
	public:
		MultiplexPs(void *root,void *parent);
		virtual ~MultiplexPs();
		int gb28181AddFrame(char *buf,int len,int type);
	private:
		int gb28181MakePsHeader(char *pData, unsigned long long s64Scr);
		int gb28181MakeSysHeader(char *pData);
		int gb28181MakePsmHeader(char *pData);
		int gb28181AddPes(char *frameBuf,int frameLen,char *pData,int len,int &outLen);
		int gb28181MakePesHeader(char *pData, int streamId, int payloadLen, unsigned long long pts, unsigned long long dts);
	private:
		int gb28181AddIFrame(char *frameBuf,int frameLen,char *psBuf,int psLen,int &outLen);
		int gb28181AddBFrame(char *frameBuf,int frameLen,char *psBuf,int psLen,int &outLen);
		int gb28181AddPFrame(char *frameBuf,int frameLen,char *psBuf,int psLen,int &outLen);
	private:
		unsigned long long fTimeStamp;
		int fFrameRate;
		
};

#endif
