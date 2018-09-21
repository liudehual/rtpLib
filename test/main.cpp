#include <stdio.h>
#include <stdlib.h>
#include "rtpLib.h"
#include <string.h>
#include <unistd.h> 

struct PSFRAME_INFO
{
	int framelen;
	int frametype;
};
void GenerateFrameIndex()
{

	struct PSFRAME_INFO frameinfo;
	FILE* fp_h264 = NULL;
	int ret = 0;

	fp_h264 = fopen("test-720p.ps", "rb");
	fseek(fp_h264, 0, SEEK_END);
	int size_h264=ftell(fp_h264);
	fprintf(stderr,"size_h264=%d \n",size_h264);
	fseek(fp_h264, 0, SEEK_SET);
	unsigned char* ph264Buf = new unsigned char[size_h264];
	ret = fread(ph264Buf, 1, size_h264, fp_h264);

	FILE* fp_index = fopen("test.index", "wb+");
	int last_pos = 0;
	int frame_count = 0;
	int frame_type = -1;
	for ( int i = 0 ; i < size_h264 - 3 ; i++){
		if ( (ph264Buf[i] == 0x00 
					&& ph264Buf[i+1] == 0x00 
					&& ph264Buf[i+2] == 0x01 
					&& ph264Buf[i+3] == 0xBA)){
			if ( frame_type >= 0 ){
				frame_count++;
				frameinfo.frametype = frame_type;
				frameinfo.framelen = i-last_pos;
				fwrite(&frameinfo, sizeof(frameinfo), 1, fp_index);
				
				last_pos = i;
			}
			frame_type = 1;
			continue;
		}
	}
	frame_count++;
	frameinfo.frametype = frame_type;
	frameinfo.framelen = size_h264-last_pos;
	fwrite(&frameinfo, sizeof(frameinfo), 1, fp_index);

	delete []ph264Buf;
	fclose(fp_h264);
	fclose(fp_index);

	printf("genrate frame index sucess, frame count[%d]!\n",frame_count);
}

void sendPs(void * handler)
{
	struct PSFRAME_INFO frameinfo;
	//!es תps

	FILE* fp_h264 = NULL;
	int ret = 0;

	char* pBuf= new char[1024*1024];
	fp_h264 = fopen("test-720p.ps", "rb");
	FILE* fp_index = fopen("test.index", "rb");
	FILE* fp_ps = fopen("test.ps", "wb+");
	if(!fp_h264 
		|| !fp_ps){
		fprintf(stderr,"open file error \n");
		return;
	}
	int framecount = 0;
	while (1){
		usleep(40*1000);
		
		ret = fread(&frameinfo, sizeof(frameinfo), 1, fp_index);
		if (ret <= 0){
			printf("read frame index over, total:%d\n", framecount);
			break;
		}
		fprintf(stderr,"send data len=%d framecount=%d \n",frameinfo.framelen,framecount);
		framecount++;
		ret = fread(pBuf, 1, frameinfo.framelen, fp_h264);
		rtpSendData(handler,pBuf,frameinfo.framelen);
		ret=fwrite(pBuf,1,frameinfo.framelen,fp_ps);
		fflush(fp_ps);
	}

	fclose(fp_h264);
	fclose(fp_index);
	fclose(fp_ps);
	delete[] pBuf;
}


int getMediaData(char *outBuf,int outLen,int ssrc,int mediaType,void *arg)
{

	static FILE *fp=NULL;
	if(!fp){
		fp=fopen("my_write.ps","wb+");
		if(!fp){
			return 0;
		}

	}
	fprintf(stderr,"buf len=%d \n",outLen);
	fwrite(outBuf,1,outLen,fp);
	fflush(fp);


	return 1;
}

int rtpPushExecption(int exceptionType,void *arg)
{
	fprintf(stderr,"client connection is error \n");
	return 1;
}

int rtpRecvExecption(int exceptionType,void *arg)
{
	fprintf(stderr,"server connection is error \n");
	return 1;
}

void usage()
{
	fprintf(stderr,"./main -c server_ip server_port \n");
	fprintf(stderr,"./main -s server_local_port \n");
}
int main(int argc,char *argv[])
{
	if(argc<3 || (strcmp(argv[1],"-c") && strcmp(argv[1],"-s"))){
		usage();
		return 0;
	}
	rtpInit();

	
	void *handler=NULL;
	rtpCreateRtpInstance(&handler);
	fprintf(stderr,"handler=%p \n",handler);
	if(!strcmp(argv[1],"-c")){
		rtpSetRtpInstancePushModel(handler,
										argv[2],
										atoi(argv[3]),
										0,
										SOCKET_UDP_TYPE,
										2,
										3,
										rtpPushExecption,
										NULL,
										NULL,
										NULL);

	}
	if(!strcmp(argv[1],"-s")){
		rtpSetRtpInstanceRecvModel(handler, 
								atoi(argv[2]),
								SOCKET_UDP_TYPE, 
								STREAM_PS_TYPE,
								3,
								300,
								NULL, 
								NULL, 
								NULL, 
								NULL,
								getMediaData,
								NULL,
								rtpRecvExecption,
								NULL
								);

	}
	
	rtpStartRtpInstance(handler);

	sleep(1);
	if(!strcmp(argv[1],"-c")){
		GenerateFrameIndex();
		sendPs(handler);
		rtpClearRTPInstance(&handler);
	}

	if(!strcmp(argv[1],"-s")){
		getchar();
		getchar();
		getchar();
		rtpClearRTPInstance(&handler);
	}
	for(;;){
		getchar();
	}
	fprintf(stderr,"hello world\n");
	return 0;
}
