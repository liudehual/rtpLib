#include "common.h"
#include "multiplexPs.h"
MultiplexPs::MultiplexPs(void *root,void *parent):Object(root,parent),fFrameRate(25)
{
	 struct	timeval tv;
	 gettimeofday(&tv,NULL);
	 fTimeStamp=tv.tv_sec*1000+tv.tv_usec/1000;
}
MultiplexPs::~MultiplexPs()
{

}
/*** 
 *@remark:   ps头的封装,里面的具体数据的填写已经占位，可以参考标准 
 *@param :   pData  [in] 填充ps头数据的地址 
 *           s64Src [in] 时间戳 
 *@return:   0 success, others failed 
*/  
int MultiplexPs::gb28181MakePsHeader(char *pData, unsigned long long s64Scr)  
{  
	int theError=OS_NO_ERROR;
    unsigned long long lScrExt = (s64Scr) % 100;      
    // s64Scr = s64Scr / 100;  
    // 这里除以100是由于sdp协议返回的video的频率是90000，帧率是25帧/s，所以每次递增的量是3600,  
    // 所以实际你应该根据你自己编码里的时间戳来处理以保证时间戳的增量为3600即可，  
    // 如果这里不对的话，就可能导致卡顿现象了  
    bits_buffer_s   bitsBuffer;  
    bitsBuffer.i_size = PS_HDR_LEN;   
    bitsBuffer.i_data = 0;  
    bitsBuffer.i_mask = 0x80; // 二进制：10000000 这里是为了后面对一个字节的每一位进行操作，避免大小端夸字节字序错乱  
    bitsBuffer.p_data = (unsigned char *)(pData);  
    memset((char *)bitsBuffer.p_data,'\0', PS_HDR_LEN);  
    bitsWrite(&bitsBuffer, 32, 0x000001BA);            /*start codes*/  
    bitsWrite(&bitsBuffer, 2,  1);                     /*marker bits '01b'*/  
    bitsWrite(&bitsBuffer, 3,  (s64Scr>>30)&0x07);     /*System clock [32..30]*/  
    bitsWrite(&bitsBuffer, 1,  1);                     /*marker bit*/  
    bitsWrite(&bitsBuffer, 15, (s64Scr>>15)&0x7FFF);   /*System clock [29..15]*/  
    bitsWrite(&bitsBuffer, 1,  1);                     /*marker bit*/  
    bitsWrite(&bitsBuffer, 15, s64Scr&0x7fff);         /*System clock [29..15]*/  
    bitsWrite(&bitsBuffer, 1,  1);                     /*marker bit*/  
    bitsWrite(&bitsBuffer, 9,  s64Scr&0x01ff);         /*System clock [14..0]*/  
    bitsWrite(&bitsBuffer, 1,  1);                     /*marker bit*/  
    bitsWrite(&bitsBuffer, 22, (255)&0x3fffff);        /*bit rate(n units of 50 bytes per second.)*/  
    bitsWrite(&bitsBuffer, 2,  3);                     /*marker bits '11'*/  
    bitsWrite(&bitsBuffer, 5,  0x1f);                  /*reserved(reserved for future use)*/  
    bitsWrite(&bitsBuffer, 3,  0);                     /*stuffing length*/  
    return theError;  
}  

/*** 
 *@remark:   sys头的封装,里面的具体数据的填写已经占位，可以参考标准 
 *@param :   pData  [in] 填充ps头数据的地址 
 *@return:   0 success, others failed 
*/  
int MultiplexPs::gb28181MakeSysHeader(char *pData)  
{  
    int theError=OS_NO_ERROR;  
    bits_buffer_s   bitsBuffer;  
    bitsBuffer.i_size = SYS_HDR_LEN;  
    bitsBuffer.i_data = 0;  
    bitsBuffer.i_mask = 0x80;  
    bitsBuffer.p_data = (unsigned char *)(pData);  
    memset(bitsBuffer.p_data,'\0', SYS_HDR_LEN);  
    /*system header*/  
    bitsWrite(&bitsBuffer, 32, 0x000001BB);   /*start code*/  
    bitsWrite(&bitsBuffer, 16, SYS_HDR_LEN-6);/*header_length 表示次字节后面的长度，后面的相关头也是次意思*/  
    bitsWrite(&bitsBuffer, 1,  1);            /*marker_bit*/  
    bitsWrite(&bitsBuffer, 22, 50000);        /*rate_bound*/  
    bitsWrite(&bitsBuffer, 1,  1);            /*marker_bit*/  
    bitsWrite(&bitsBuffer, 6,  1);            /*audio_bound*/  
    bitsWrite(&bitsBuffer, 1,  0);            /*fixed_flag */  
    bitsWrite(&bitsBuffer, 1,  1);            /*CSPS_flag */  
    bitsWrite(&bitsBuffer, 1,  1);            /*system_audio_lock_flag*/  
    bitsWrite(&bitsBuffer, 1,  1);            /*system_video_lock_flag*/  
    bitsWrite(&bitsBuffer, 1,  1);            /*marker_bit*/  
    bitsWrite(&bitsBuffer, 5,  1);            /*video_bound*/  
    bitsWrite(&bitsBuffer, 1,  0);            /*dif from mpeg1*/  
    bitsWrite(&bitsBuffer, 7,  0x7F);         /*reserver*/  
    /*audio stream bound*/  
    bitsWrite(&bitsBuffer, 8,  0xC0);         /*stream_id*/  
    bitsWrite(&bitsBuffer, 2,  3);            /*marker_bit */  
    bitsWrite(&bitsBuffer, 1,  0);            /*PSTD_buffer_bound_scale*/  
    bitsWrite(&bitsBuffer, 13, 512);          /*PSTD_buffer_size_bound*/  
    /*video stream bound*/  
    bitsWrite(&bitsBuffer, 8,  0xE0);         /*stream_id*/  
    bitsWrite(&bitsBuffer, 2,  3);            /*marker_bit */  
    bitsWrite(&bitsBuffer, 1,  1);            /*PSTD_buffer_bound_scale*/  
    bitsWrite(&bitsBuffer, 13, 2048);         /*PSTD_buffer_size_bound*/  
    return theError;  
}  


/*** 
 *@remark:   psm头的封装,里面的具体数据的填写已经占位，可以参考标准 
 *@param :   pData  [in] 填充ps头数据的地址 
 *@return:   0 success, others failed 
*/  
int MultiplexPs::gb28181MakePsmHeader(char *pData)  
{  
    int theError=OS_NO_ERROR;  
    bits_buffer_s   bitsBuffer;  
    bitsBuffer.i_size = PSM_HDR_LEN;   
    bitsBuffer.i_data = 0;  
    bitsBuffer.i_mask = 0x80;  
    bitsBuffer.p_data = (unsigned char *)(pData);  
    memset(bitsBuffer.p_data,'\0', PSM_HDR_LEN);  
    bitsWrite(&bitsBuffer, 24,0x000001);   /*start code*/  
    bitsWrite(&bitsBuffer, 8, 0xBC);       /*map stream id*/  
    bitsWrite(&bitsBuffer, 16,18);         /*program stream map length*/   
    bitsWrite(&bitsBuffer, 1, 1);          /*current next indicator */  
    bitsWrite(&bitsBuffer, 2, 3);          /*reserved*/  
    bitsWrite(&bitsBuffer, 5, 0);          /*program stream map version*/  
    bitsWrite(&bitsBuffer, 7, 0x7F);       /*reserved */  
    bitsWrite(&bitsBuffer, 1, 1);          /*marker bit */  
    bitsWrite(&bitsBuffer, 16,0);          /*programe stream info length*/  
    bitsWrite(&bitsBuffer, 16, 8);         /*elementary stream map length  is*/  
    /*audio*/  
    bitsWrite(&bitsBuffer, 8, 0x90);       /*stream_type*/  
    bitsWrite(&bitsBuffer, 8, 0xC0);       /*elementary_stream_id*/  
    bitsWrite(&bitsBuffer, 16, 0);         /*elementary_stream_info_length is*/  
    /*video*/  
    bitsWrite(&bitsBuffer, 8, 0x1B);       /*stream_type*/  
    bitsWrite(&bitsBuffer, 8, 0xE0);       /*elementary_stream_id*/  
    bitsWrite(&bitsBuffer, 16, 0);         /*elementary_stream_info_length */  
    /*crc (2e b9 0f 3d)*/  
    bitsWrite(&bitsBuffer, 8, 0x45);       /*crc (24~31) bits*/  
    bitsWrite(&bitsBuffer, 8, 0xBD);       /*crc (16~23) bits*/  
    bitsWrite(&bitsBuffer, 8, 0xDC);       /*crc (8~15) bits*/  
    bitsWrite(&bitsBuffer, 8, 0xF4);       /*crc (0~7) bits*/  
    return theError;  
} 
int MultiplexPs::gb28181AddPes(char *frameBuf,int frameLen,char *pData,int len,int &outLen)
{
	int theError=OS_NO_ERROR;
	if(frameLen<=MAX_PES_LEN){
		gb28181MakePesHeader(pData,PS_STREAM_ID,frameLen,fTimeStamp,fTimeStamp);
		pData+=PES_HDR_LEN;
		outLen+=PES_HDR_LEN;
		
		memcpy(pData,frameBuf,frameLen);
		pData+=frameLen;
		outLen+=frameLen;
		
	}else{
		int pesNumber=frameLen/MAX_PES_LEN;
		int pesLeftDataLen=frameLen%MAX_PES_LEN;
		for(int i=0;i<pesNumber;i++){
			gb28181MakePesHeader(pData,PS_STREAM_ID,MAX_PES_LEN,fTimeStamp,fTimeStamp);
			pData+=PES_HDR_LEN;
			outLen+=PES_HDR_LEN;
			
			memcpy(pData,&frameBuf[i*MAX_PES_LEN],MAX_PES_LEN);
			pData+=MAX_PES_LEN;
			outLen+=MAX_PES_LEN;
		}
		gb28181MakePesHeader(pData,PS_STREAM_ID,pesLeftDataLen,fTimeStamp,fTimeStamp);
		pData+=PES_HDR_LEN;
		outLen+=PES_HDR_LEN;
		
		memcpy(pData,&frameBuf[pesNumber*MAX_PES_LEN],pesLeftDataLen);
		pData+=pesLeftDataLen;
		outLen+=pesLeftDataLen;
	}
			
	return theError; 

}


/*** 
 *@remark:   pes头的封装,里面的具体数据的填写已经占位，可以参考标准 
 *@param :   pData      [in] 填充ps头数据的地址 
 *           stream_id  [in] 码流类型 
 *           paylaod_len[in] 负载长度 
 *           pts        [in] 时间戳 
 *           dts        [in] 
 *@return:   0 success, others failed 
*/  
int MultiplexPs::gb28181MakePesHeader(char *pData, int streamId, int payloadLen, unsigned long long pts, unsigned long long dts)  
{  
    int theError=OS_NO_ERROR;
    bits_buffer_s   bitsBuffer;  
    bitsBuffer.i_size = PES_HDR_LEN;  
    bitsBuffer.i_data = 0;  
    bitsBuffer.i_mask = 0x80;  
    bitsBuffer.p_data = (unsigned char *)(pData);  
    memset(bitsBuffer.p_data,'\0', PES_HDR_LEN);  
    /*system header*/  
    bitsWrite(&bitsBuffer, 24,0x000001);  /*start code*/  
    bitsWrite(&bitsBuffer, 8, (streamId));   /*streamID*/  
    bitsWrite(&bitsBuffer, 16,(payloadLen)+13);  /*packet_len*/ //指出pes分组中数据长度和该字节后的长度和  
    bitsWrite(&bitsBuffer, 2, 2 );        /*'10'*/  
    bitsWrite(&bitsBuffer, 2, 0 );        /*scrambling_control*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*priority*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*data_alignment_indicator*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*copyright*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*original_or_copy*/  
    bitsWrite(&bitsBuffer, 1, 1 );        /*PTS_flag*/  
    bitsWrite(&bitsBuffer, 1, 1 );        /*DTS_flag*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*ESCR_flag*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*ES_rate_flag*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*DSM_trick_mode_flag*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*additional_copy_info_flag*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*PES_CRC_flag*/  
    bitsWrite(&bitsBuffer, 1, 0 );        /*PES_extension_flag*/  
    bitsWrite(&bitsBuffer, 8, 10);        /*header_data_length*/   

	// 指出包含在 PES 分组标题中的可选字段和任何填充字节所占用的总字节数。该字段之前  
    // 的字节指出了有无可选字段。  
      
    /*PTS,DTS*/   
    bitsWrite(&bitsBuffer, 4, 3 );                    /*'0011'*/  
    bitsWrite(&bitsBuffer, 3, ((pts)>>30)&0x07 );     /*PTS[32..30]*/  
    bitsWrite(&bitsBuffer, 1, 1 );  
    bitsWrite(&bitsBuffer, 15,((pts)>>15)&0x7FFF);    /*PTS[29..15]*/  
    bitsWrite(&bitsBuffer, 1, 1 );  
    bitsWrite(&bitsBuffer, 15,(pts)&0x7FFF);          /*PTS[14..0]*/  
    bitsWrite(&bitsBuffer, 1, 1 );  
    bitsWrite(&bitsBuffer, 4, 1 );                    /*'0001'*/  
    bitsWrite(&bitsBuffer, 3, ((dts)>>30)&0x07 );     /*DTS[32..30]*/  
    bitsWrite(&bitsBuffer, 1, 1 );  
    bitsWrite(&bitsBuffer, 15,((dts)>>15)&0x7FFF);    /*DTS[29..15]*/  
    bitsWrite(&bitsBuffer, 1, 1 );  
    bitsWrite(&bitsBuffer, 15,(dts)&0x7FFF);          /*DTS[14..0]*/  
    bitsWrite(&bitsBuffer, 1, 1 );  
    return theError;  
}  
int MultiplexPs::gb28181AddFrame(char *buf,int len,int type)
{
	int theError=OS_NO_ERROR;
	if(buf==NULL
		|| len==0){
		return OS_FUNCTION_ARGS_ERROR;
	}
	static char psBuf[MAX_VIDEO_LEN]={'\0'};
	int outLen=0;
	switch(type){
		case I_FRAME:{
			if((theError=gb28181AddIFrame(buf,len,psBuf,sizeof(psBuf),outLen))!=OS_NO_ERROR){
				return theError;
			}
			break;
		}
		case B_FRAME:{
			if((theError=gb28181AddBFrame(buf,len,psBuf,sizeof(psBuf),outLen))!=OS_NO_ERROR){
				return theError;
			}
			break;
		}
		case P_FRAME:{
			if((theError=gb28181AddPFrame(buf,len,psBuf,sizeof(psBuf),outLen))!=OS_NO_ERROR){
				return theError;
			}
			break;
		}
		default:{
			return OS_FRAME_TYPE_ERROR;
		}
	}

	// write ps file [test]
	fprintf(stderr, "frameLen=%d "
					"outLen=%d "
					"D-Value=%d\n",
					len,
					outLen,
					outLen-len);
	{
		#if 1
		static FILE *fp=NULL;
		if(!fp){
			fp=fopen("test.ps","wb");
		}
		if(fp){
			fwrite(psBuf,1,outLen,fp);
			fflush(fp);
		}
		#endif
	}

	return theError; 
}

int MultiplexPs::gb28181AddIFrame(char *frameBuf,int frameLen,char *psBuf,int psLen,int &outLen)
{
	int theError=OS_NO_ERROR;
	
	fTimeStamp+=VIDEO_FREQUENCY/fFrameRate;
	gb28181MakePsHeader(psBuf,fTimeStamp);
	outLen+=PS_HDR_LEN;
	gb28181MakeSysHeader(&psBuf[PS_HDR_LEN]);
	outLen+=SYS_HDR_LEN;
	gb28181MakePsmHeader(&psBuf[PS_HDR_LEN+SYS_HDR_LEN]);
	outLen+=PSM_HDR_LEN;
	gb28181AddPes(frameBuf,frameLen,&psBuf[PS_HDR_LEN+SYS_HDR_LEN+PSM_HDR_LEN],psLen-(PS_HDR_LEN+SYS_HDR_LEN+PSM_HDR_LEN),outLen);

	return theError; 
}
int MultiplexPs::gb28181AddBFrame(char *frameBuf,int frameLen,char *psBuf,int psLen,int &outLen)
{
	int theError=OS_NO_ERROR;
	
	fTimeStamp+=VIDEO_FREQUENCY/fFrameRate;
	gb28181MakePsHeader(psBuf,fTimeStamp);
	outLen+=PS_HDR_LEN;
	gb28181MakePsmHeader(&psBuf[PS_HDR_LEN]);
	outLen+=PSM_HDR_LEN;
	gb28181AddPes(frameBuf,frameLen,&psBuf[PS_HDR_LEN+PSM_HDR_LEN],psLen-(PS_HDR_LEN+PSM_HDR_LEN),outLen);
		
	return theError; 
}
int MultiplexPs::gb28181AddPFrame(char *frameBuf,int frameLen,char *psBuf,int psLen,int &outLen)
{
	int theError=OS_NO_ERROR;
	
	fTimeStamp+=VIDEO_FREQUENCY/fFrameRate;
	gb28181MakePsHeader(psBuf,fTimeStamp);
	outLen+=PS_HDR_LEN;
	gb28181MakePsmHeader(&psBuf[PS_HDR_LEN]);
	outLen+=PSM_HDR_LEN;
	gb28181AddPes(frameBuf,frameLen,&psBuf[PS_HDR_LEN+PSM_HDR_LEN],psLen-(PS_HDR_LEN+PSM_HDR_LEN),outLen);
		
	return theError; 
}

