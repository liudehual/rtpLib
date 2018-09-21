#include "multiplexPs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct h264FRAME_INFO
{
	int framelen;
	int frametype;
};
void GenerateFrameIndex()
{

	struct h264FRAME_INFO frameinfo;
	FILE* fp_h264 = NULL;
	int ret = 0;

	fp_h264 = fopen("test.h264", "rb");
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
					&& ph264Buf[i+2] == 0x00 
					&& ph264Buf[i+3] == 0x01)){
			if ( frame_type >= 0 ){
				frame_count++;
				frameinfo.frametype = frame_type;
				frameinfo.framelen = i-last_pos;
				fwrite(&frameinfo, sizeof(frameinfo), 1, fp_index);
				
				last_pos = i;
			}
			fprintf(stderr,"frame_type=%d \n",(ph264Buf[i+4] & 0x1F));
			if ( (ph264Buf[i+4] & 0x1F) == 5){ //i֡
				frame_type = I_FRAME;
			}else if ( (ph264Buf[i+4] & 0x1F) == 1 ){ //p֡
				frame_type = P_FRAME;
			}else{
				frame_type = -1;
			}

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

void h264es2ps()
{
	struct h264FRAME_INFO frameinfo;
	//!es תps
	MultiplexPs muPs(NULL,NULL);
	FILE* fp_h264 = NULL;
	int ret = 0;

	char* pBuf= new char[1024*1024];
	fp_h264 = fopen("test.h264", "rb");
	FILE* fp_index = fopen("test.index", "rb");
	FILE* fp_ps = fopen("test.ps", "wb+");
	if(!fp_h264 
		|| !fp_ps){
		fprintf(stderr,"open file error \n");
		return;
	}
	int framecount = 0;
	while (1){
		ret = fread(&frameinfo, sizeof(frameinfo), 1, fp_index);
		if (ret <= 0){
			printf("read frame index over, total:%d\n", framecount);
			break;
		}
		framecount++;
		ret = fread(pBuf, 1, frameinfo.framelen, fp_h264);
		muPs.gb28181AddFrame((char*)pBuf, frameinfo.framelen,frameinfo.frametype);
	}

	fclose(fp_h264);
	fclose(fp_index);
	fclose(fp_ps);
	delete[] pBuf;
}

int main(int argc,char *argv[])
{
	GenerateFrameIndex();
	h264es2ps();
	return 0;
}
