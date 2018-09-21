#include "packet.h"
#include "log.h"
#include "rtpLib.h"
#include "task.h"
#include "tcpSocket.h"
#include "rtpRtcpTask.h"
#include "rtpInstance.h"
#include "thread.h"
Packet::Packet(void *root,void *parent):Object(root,parent),fRtpSeq(65000)
{

}
Packet::~Packet()
{

}

int Packet::writeData(char *pData,int len)
{
	if(!pData || len==0){
		os_error("data buffer is empty \n");
		return OS_FUNCTION_ARGS_ERROR;
	}
	if(len>DATA_MAX_BUFFER_SIZE-pLen){
		os_warn("ps data is too large,max=2M ,data will be truncated \n");
	}
	if(DATA_MAX_BUFFER_SIZE-pLen<=0){
		pLen=0;
	}
	if(len<=DATA_MAX_BUFFER_SIZE-pLen){
		memcpy(&pBuffer[pLen],pData,len);
		pLen+=len;
	}else{
		memcpy(&pBuffer[pLen],pData,DATA_MAX_BUFFER_SIZE-pLen);
		pLen+=DATA_MAX_BUFFER_SIZE-pLen;
	}
	return OS_NO_ERROR;

}
int Packet::flushRtpDataSocket(Task *tmpTask)
{
	RtpTask *theTask=(RtpTask *)tmpTask;
	unsigned int outLen=0;
	for(int i=0;i<fRtpPacketDataArray.size();i++){
		RtpPacketData *theData=fRtpPacketDataArray[i];
		if(theData->len==0){
			break;
		}
		unsigned short seq=ntohs(*(unsigned short *)&theData->buf[2]);
		if((theTask->sendData((char *)theData->buf,theData->len,&outLen))!=OS_NO_ERROR){
			break;
		}
		theData->len=0;
	}

	return OS_NO_ERROR;

}
int Packet::flushRtcpDataSocket(Task *tmpTask)
{
	UdpRtcpTask *theTask=(UdpRtcpTask *)tmpTask;
	unsigned int outLen=0;
	theTask->sendData((char *)pRtcpBuffer,pRtcpLen,&outLen);
	pRtcpLen=0;
	return OS_NO_ERROR;
}

RtpPacket::RtpPacket(void *root,void *parent):Packet(root,parent)
{
	fMutex=new Mutex();
	pBuffer=new unsigned char[DATA_MAX_BUFFER_SIZE];
	pLen=0;
	for(int i=0;i<MAX_RTP_PACKET_NUMBERS;i++){ // MAX_RTP_PACKET_NUMBERS
		RtpPacketData *theData=new RtpPacketData;
		fRtpPacketDataArray.push_back(theData);
	}
	fRtpPacketDataDecodeArray.clear();
}
RtpPacket::~RtpPacket()
{
	if(fMutex){
		delete fMutex;
	}
	if(pBuffer){
		delete[] pBuffer;
		pLen=0;
	}
	for(int i=0;i<MAX_RTP_PACKET_NUMBERS;i++){ // MAX_RTP_PACKET_NUMBERS
		RtpPacketData *theData=fRtpPacketDataArray[i];
		if(theData){
			delete[] theData->buf;
			theData->len=0;
		}
	}
}

int RtpPacket::makeRtpHeader(char *pData, 
										int markerFlag, 
										int payloadType,
										unsigned short cseq, 
										long long curpts, 
										unsigned int ssrc)  
{  
    bits_buffer_s   bitsBuffer;  
    if (pData == NULL){  
        return OS_FUNCTION_ARGS_ERROR; 
	} 
    bitsBuffer.i_size = RTP_HDR_LEN;  
    bitsBuffer.i_data = 0;  
    bitsBuffer.i_mask = 0x80;  
    bitsBuffer.p_data = (unsigned char *)(pData);  
    memset(bitsBuffer.p_data,'\0', RTP_HDR_LEN);  
    bitsWrite(&bitsBuffer, 2, 2);    		   /* rtp version  */  
    bitsWrite(&bitsBuffer, 1, 0);              /* rtp padding  */  
    bitsWrite(&bitsBuffer, 1, 0);              /* rtp extension    */  
    bitsWrite(&bitsBuffer, 4, 0);              /* rtp CSRC count */  
    bitsWrite(&bitsBuffer, 1, (markerFlag));   /* rtp marker   */  
    bitsWrite(&bitsBuffer, 7, (payloadType));         	   /* rtp payload type*/  
    bitsWrite(&bitsBuffer, 16, (cseq));        /* rtp sequence      */  
    bitsWrite(&bitsBuffer, 32, (curpts));      /* rtp timestamp     */  
    bitsWrite(&bitsBuffer, 32, (ssrc));        /* rtp SSRC      */  
    return OS_NO_ERROR;  
}  
int RtpPacket::makeTcpRtpHeader(char *pData,unsigned short len,unsigned char channel)
{
	int theError=OS_NO_ERROR;
	bits_buffer_s   bitsBuffer;  
    if (pData == NULL){  
        return OS_FUNCTION_ARGS_ERROR; 
	} 
    bitsBuffer.i_size = RTP_TCP_HDR_LEN;  
    bitsBuffer.i_data = 0;  
    bitsBuffer.i_mask = 0x80;  
    bitsBuffer.p_data = (unsigned char *)(pData);  
    memset(bitsBuffer.p_data,'\0', RTP_TCP_HDR_LEN);  
    bitsWrite(&bitsBuffer, 8, 0x24);    		   /* magic number '$'  */  
    bitsWrite(&bitsBuffer, 8, channel);            /* channel rtp=0 rtcp=1  */  
    bitsWrite(&bitsBuffer, 16, len);               /* rtp packet len < mtu (advice len=1400)   */  

	return theError;
}
int RtpPacket::makeRtpPacket()
{
	int theError=OS_NO_ERROR;

	return theError;
}

/*该函数将数组下标范围[l1,r1]和[l2,r2]的有序序列合并成一个有序序列*/
void RtpPacket::merge(std::vector<RtpPacketData *>& array, int l1, int r1, int l2, int r2 ) {
	int i = l1;                                               //左半部分起始位置
	int j = l2;                                               //右半部分起始位置
	int n = (r1 - l1 + 1) + (r2 - l2 + 1);                    //要合并的元素个数
	std::vector<RtpPacketData *> temp(n);                                      //辅助数组
	int k = 0;	                                          //辅助数组其起始位置
	while(i <= r1 && j <= r2){                                //挑选两部分中最小的元素放入辅助数组中
		if (array[i]->fSeq <= array[j]->fSeq){
			temp[k++] = array[i++];
		}else{
			temp[k++] = array[j++];
		}
	}
	//如果还有剩余，直接放入到辅助数组中
	while(i <= r1){
		temp[k++] = array[i++];
	}
	while(j <= r2){
		temp[k++] = array[j++];
	}
	//更新原始数组元素
	for (int i = 0; i < n;i++){
		array[l1 + i] = temp[i];
	}
}
 
/*二路归并排序（迭代实现）*/
void RtpPacket::MergeSort1(std::vector<RtpPacketData *>& array, int start, int end)
{
	int n = array.size();
	if (start < end){
		// step为组内元素个数，step/2为左子区间元素个数
		for (int step = 2; step/2 <n; step *= 2) {
			// 每step个元素一组，组内前step/2和后step/2个元素进行合并
			for (int i = 0; i < n; i += step) {
				int mid = i + step / 2 - 1;					// 左子区间元素个数为step/2
				if(mid+1<n){							// 右子区间存在元素个数则合并
					// 左子区间为[i,mid]，右子区间为[mid+1, min(i+step-1, n-1)]
					merge(array, i, mid, mid + 1, min(i + step - 1, n-1));
				}
			}
		}
	}
}
int RtpPacket::insertPakectAndSort(char *pBuf,int pLen)
{

	//for(int i=0;i<fRtpPacketDataDecodeArray.size();i++){
	//	RtpPacketData *thePacket=fRtpPacketDataDecodeArray[i];
	//	os_error("fSeq=%d \n",thePacket->fSeq);
	//}
	return OS_NO_ERROR;
}

int RtpPacket::insertPacketToQueue(char *pBuf,int pLen)
{
		RtpPacketData *thePacketData=NULL;
#if 1
		for(int i=0;i<fRtpPacketDataArray.size();i++){
			thePacketData=fRtpPacketDataArray[i];
			if(thePacketData->len==0){
				thePacketData->clear();
				break;
			}
		}
		if(!thePacketData){
			for(int i=0;i<fRtpPacketDataArray.size();i++){
				RtpPacketData * theData=fRtpPacketDataArray[i];
				theData->clear();
				
			}
			fRtpPacketDataDecodeArray.clear();
			return RTP_ARRAY_FULL_ERROR;
		}
#endif
		
		memcpy(thePacketData->buf,pBuf,pLen);
		thePacketData->len=pLen;
	
		thePacketData->fSsrc=ntohl(*((unsigned int *)&pBuf[8]));
		thePacketData->marker=((unsigned char)pBuf[1] & 0x80) >>7;
		thePacketData->fSeq=ntohs(*(unsigned short *)&pBuf[2]);
		
	
		fRtpPacketDataDecodeArray.push_back(thePacketData);

	return OS_NO_ERROR;
}
int RtpPacket::sortQueue()
{

	RtpInstance *theInstance=(RtpInstance *)getRoot();
	MergeSort1(fRtpPacketDataDecodeArray,0,fRtpPacketDataDecodeArray.size());

	return OS_NO_ERROR;
}

int RtpPacket::achieveCachingRequirements()
{
	int packetNumbers=fRtpPacketDataDecodeArray.size();
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	if(packetNumbers*MAX_RTP_PACKET_BUFFER_SIZE>theInstance->getRecvCache()){
		return 1;
	}
	return 0;
}
int RtpPacket::fineTuning()
{
	RtpPacketData *theData=fRtpPacketDataDecodeArray[0];
	int i=0;
	if(theData->fSeq==0){ // 启动循环检查
		unsigned oldSeq=theData->fSeq;
		for(;i<fRtpPacketDataDecodeArray.size();i++){
			RtpPacketData *theData1=fRtpPacketDataDecodeArray[i];
			if((theData1->fSeq-oldSeq)>=10000){ // 首尾相差10000个数据包(经验值???)
				break;
			}
			oldSeq=theData1->fSeq;
		}		

		if(i==fRtpPacketDataDecodeArray.size()-1){ // 序列号本来就是从0开始的,不用调
			return OS_NO_ERROR;
		}
		
		std::vector<RtpPacketData *> startQueue;
		std::vector<RtpPacketData *> endQueue;

		// 缓存前半部分数据包
		for(int j=0;j<i;j++){
			RtpPacketData *theData=fRtpPacketDataDecodeArray[j];
			startQueue.push_back(theData);
		}

		// 缓存后半部分数据包
		for(int k=i;k<fRtpPacketDataDecodeArray.size();k++){
			RtpPacketData *theData=fRtpPacketDataDecodeArray[k];
			endQueue.push_back(theData);
		}

		// 清空原来的数据队列
		fRtpPacketDataDecodeArray.clear();

		// 将后半部分数据包放入解码队列中
		for(int n=0;n<endQueue.size();n++){
			RtpPacketData *theData=endQueue[n];
			fRtpPacketDataDecodeArray.push_back(theData);
		}

		// 将前半部分数据包放入解码队列中
		for(int m=0;m<startQueue.size();m++){
			RtpPacketData *theData=startQueue[m];
			fRtpPacketDataDecodeArray.push_back(theData);
		}


	}
	

	return OS_NO_ERROR;
}

int RtpPacket::getFrameData(std::vector<RtpPacketData *> &frame)
{
	std::vector<RtpPacketData *> startQueue;
	int i=0;
	for(;i<fRtpPacketDataDecodeArray.size();i++){
		RtpPacketData *theData=fRtpPacketDataDecodeArray[i];
		frame.push_back(theData);
		if(theData->marker==1){
			break;
		}
	}
	if(i==fRtpPacketDataDecodeArray.size()-1){
		frame.clear();
		fRtpPacketDataDecodeArray.clear();
		return NOT_FIND_FRAME_ERROR;
	}
	for(int j=i+1;j<fRtpPacketDataDecodeArray.size();j++){
		RtpPacketData *theData=fRtpPacketDataDecodeArray[j];
		startQueue.push_back(theData);
	}
	fRtpPacketDataDecodeArray.clear();
	for(int m=0;m<startQueue.size();m++){
		RtpPacketData *theData=startQueue[m];
		fRtpPacketDataDecodeArray.push_back(theData);
	}
	return OS_NO_ERROR;
}

int RtpPacket::writeData(char *pData,int len)
{
	if(pData==NULL
		|| len <= 0){
		os_error("data is error %s \n",__FUNCTION__);
		return WRITE_FRAME_DATA_ERROR;
	}

	#if 0
	os_warn("ps packet len=%d \n",len);	
	#endif
	
	MutexLocker theLocket(fMutex);
	Packet::writeData(pData,len);

	return OS_NO_ERROR;
}


H264RtpPacket::H264RtpPacket(void *root,void *parent):RtpPacket(root,parent)
{
	if(pBuffer){
		delete[] pBuffer;
		pBuffer=NULL;
	}
	pBuffer=new unsigned char[DATA_MAX_BUFFER_SIZE];
}
H264RtpPacket::~H264RtpPacket()
{
	

}
int H264RtpPacket::decode(char *pData,int len)
{
	int theError=OS_NO_ERROR;
	
	return theError;
}
int H264RtpPacket::encode()
{	
	MutexLocker theLocket(fMutex);

	if(pLen <= 4){
		os_error("data is error %s \n",__FUNCTION__);
		return WRITE_FRAME_DATA_ERROR;
	}
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	int transType=theInstance->getPushTransType();

	pLen-=4;
	unsigned char *dataBuf=&pBuffer[4];
	
	int packetNumber=pLen / MAX_RTP_PACKET_SIZE;
	int leftDataLen=pLen % MAX_RTP_PACKET_SIZE;

	unsigned char *pos=NULL;
	RtpPacketData *thePacket=NULL;
	for(int i=0;i<packetNumber;i++){

		thePacket=fRtpPacketDataArray[i];
		pos=thePacket->buf;
	
		if(transType==TRANS_PROTOCOL_TCP){
			makeTcpRtpHeader((char *)pos,MAX_RTP_PACKET_SIZE+12,0);
			pos+=4;			
		}
		
		if(!i){
			
			// start packet
			makeRtpHeader((char *)pos,
							0/*marker=0*/,
							98/*H264=98*/,
							fRtpSeq++,
							0/*rtp timestamp =not set*/,
							theInstance->rtpGetRtpSSRC()/*ssrc=0*/);
			pos+=12;

			*pos=(dataBuf[0] & 0xE0) | 28;
			pos+=1;
			
			*pos=(0x80 | (dataBuf[0] & 0x1F));
			pos+=1;
		}else{

			// middle packet
			makeRtpHeader((char *)pos,
							0/*marker=0*/,
							98/*H264=98*/,
							fRtpSeq++,
							0/*rtp timestamp =not set*/,
							theInstance->rtpGetRtpSSRC()/*ssrc=0*/);
			pos+=12;

			*pos=(dataBuf[0] & 0xE0) | 28;
			pos+=1;

			*pos=(dataBuf[0] &~ 0x60) | (dataBuf[0] & 0x1F);
			pos+=1;
		}
		
		memcpy((char *)pos,(char *)&dataBuf[i*(MAX_RTP_PACKET_SIZE)],MAX_RTP_PACKET_SIZE);
		thePacket->len=pos-thePacket->buf;
	}

	thePacket=fRtpPacketDataArray[packetNumber];
	pos=thePacket->buf;
	
	if(transType==TRANS_PROTOCOL_TCP){
		makeTcpRtpHeader((char *)pos,leftDataLen+12,0);
		pos+=4;			
	}
	
	// end packet
	makeRtpHeader((char *)pos,
					1/*marker=1*/,
					98/*H264=98*/,
					fRtpSeq++,
					0/*rtp timestamp =not set*/,
					theInstance->rtpGetRtpSSRC()/*ssrc=0*/);
	pos+=12;

	*pos=(dataBuf[0] & 0xE0) | 28;;
	pos+=1;

	*pos=(dataBuf[0] &~ 0x60) | (dataBuf[0] & 0x1F);
	pos+=1;

	memcpy((char *)pos,(char *)&dataBuf[packetNumber*(MAX_RTP_PACKET_SIZE)],leftDataLen);
	thePacket->len=pos-thePacket->buf;

	pLen=0;
	return OS_NO_ERROR;
}

G711RtpPacket::G711RtpPacket(void *root,void *parent):RtpPacket(root,parent)
{
	if(pBuffer){
		delete[] pBuffer;
		pBuffer=NULL;
	}
	pBuffer=new unsigned char[256];
}
G711RtpPacket::~G711RtpPacket()
{

}
int G711RtpPacket::decode(char *pData,int len)
{
	int theError=OS_NO_ERROR;

	return theError;
}
/*
	pcm：   脉冲编码调制，以8000hz的采样，13位存储。通过A率压缩为8位。
	sdp：   a=rtpmap:0 pcma/8000/1
	pcma:  a率的压缩方式，8000是采样频率，1是单声道。
	ptime: 指一个rtp包承载的数据播放的时间，单位ms，通话过程中G.711U的每个数据包的打包时长20ms。
		   对应程序中每个rtp包为160字节。(1byte*8000hz*0.02s)()=160B/S。
*/
int G711RtpPacket::encode()
{
	MutexLocker theLocket(fMutex);

	if(pLen!=160){
		os_error("data is error %d \n",pLen);
		return WRITE_FRAME_DATA_ERROR;
	}

	
	
	RtpPacketData *thePacket=fRtpPacketDataArray[0];
	unsigned char *pos=thePacket->buf;

	RtpInstance *theInstance=(RtpInstance *)getRoot();
	int transType=theInstance->getPushTransType();
	
	if(transType==TRANS_PROTOCOL_TCP){
		makeTcpRtpHeader((char *)pos,pLen+12,0);
		pos+=4;			
	}

	makeRtpHeader((char *)pos,1,8,fRtpSeq++,0,theInstance->rtpGetRtpSSRC());
	pos+=12;

	memcpy((char *)pos,(char *)pBuffer,pLen);
	pos+=pLen;
	
	thePacket->len=pos-thePacket->buf;
	
	pLen=0;
	
	return OS_NO_ERROR;
}

PSRtpPacket::PSRtpPacket(void *root,void *parent):RtpPacket(root,parent)
{
	if(pBuffer){
		delete[] pBuffer;
		pBuffer=NULL;
	}
	pBuffer=new unsigned char[DATA_MAX_BUFFER_SIZE];
	pLen=0;
}
PSRtpPacket::~PSRtpPacket()
{

}
int PSRtpPacket::decode(char *pData,int len)
{
	if(len<12){
		return OS_NO_ERROR;
	}
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	
	rtpRecvMediaData theCB=theInstance->getRecvRecvMediaData();
	void *arg=theInstance->getRecvArg3();


	int ssrc=ntohl(*((unsigned int *)&pData[8]));
	unsigned char marker=((unsigned char)pData[1] & 0x80) >>7;

	#if 1
	
	insertPacketToQueue(pData,len);
	if(!achieveCachingRequirements()){
		return OS_NO_ERROR;
	}
	sortQueue();
	fineTuning();
	std::vector<RtpPacketData *> dataQueue;
	int theError=OS_NO_ERROR;
	if((theError=getFrameData(dataQueue))!=OS_NO_ERROR){
		return theError;
	}

	#if 0
	for(int i=0;i<dataQueue.size();i++){
		RtpPacketData *theData=dataQueue[i];
		os_warn("packet len=%d \n",theData->len);
	}
	#endif
	
	for(int i=0;i<dataQueue.size();i++){
		RtpPacketData *theData=dataQueue[i];
		
		if(pLen>DATA_MAX_BUFFER_SIZE){
			os_warn("frame too large MAX=3M,drop \n");
			pLen=0;
		}	
		if(!theData || theData->len==0){
			continue;
		}
		#if 0
		os_error("pLen=%u packet len=%d \n",pLen,theData->len-12);

		#endif
		memcpy((char *)&pBuffer[pLen],(char *)&theData->buf[12],theData->len-12);
		pLen+=theData->len-12;
		
		theData->clear();
	}
	#endif

	
	if(theCB){
		theCB((char *)pBuffer,pLen,ssrc,2,arg);
	}
	#if 0
	os_error("pLen=%u \n",pLen);

	#endif
	pLen=0;
	dataQueue.clear();
	return OS_NO_ERROR;
	
}
int PSRtpPacket::encode()
{
	MutexLocker theLocket(fMutex);

	unsigned psPacketLen=pLen;
	
	if(psPacketLen==0){
		return OS_NOT_FIND_PS_PACKET;
	}	
	//os_trace("start encode ps frame \n");
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	int transType=theInstance->getPushTransType();

	int packetNumber=psPacketLen / MAX_RTP_PACKET_SIZE;
	int leftDataLen=psPacketLen % MAX_RTP_PACKET_SIZE;
	unsigned char *pos=NULL;

	RtpPacketData *thePacket=NULL;
	
	for(int i=0;i<packetNumber;i++){

		thePacket=fRtpPacketDataArray[i];
		pos=thePacket->buf;
	
		if(transType==TRANS_PROTOCOL_TCP){
			makeTcpRtpHeader((char *)pos,MAX_RTP_PACKET_SIZE+12,0);
			pos+=4;
		}
		fRtpSeq++;
		makeRtpHeader((char *)pos,0/*marker=0*/,96/*Ps=96*/,fRtpSeq,0/*rtp timestampe=not set*/,theInstance->rtpGetRtpSSRC()/*ssrc = 0*/);
		pos+=12;
		
		memcpy((char *)pos,(char *)&pBuffer[i*MAX_RTP_PACKET_SIZE],MAX_RTP_PACKET_SIZE);
		pos+=MAX_RTP_PACKET_SIZE;
		
		thePacket->len=pos-thePacket->buf;
		unsigned short seq=ntohs(*(unsigned short *)&thePacket->buf[2]);
		#if 0
		os_trace("-----------------------> packet data len=%d fRtpSeq =%d seq=%d i=%d \n",
																			thePacket->len,
																			fRtpSeq,
																			seq,
																			i);
		#endif
	}

	thePacket=fRtpPacketDataArray[packetNumber];
	pos=thePacket->buf;
	
	if(transType==TRANS_PROTOCOL_TCP){
		makeTcpRtpHeader((char *)pos,leftDataLen+12,0);
		pos+=4;
	}
	fRtpSeq++;
	makeRtpHeader((char *)pos,1,96,fRtpSeq,0,theInstance->rtpGetRtpSSRC());
	pos+=12;

	memcpy((char *)pos,(char *)&pBuffer[packetNumber*MAX_RTP_PACKET_SIZE],leftDataLen);
	pos+=leftDataLen;

	thePacket->len=pos-thePacket->buf;
	unsigned short seq=ntohs(*(unsigned short *)&thePacket->buf[2]);
	#if 0
	os_trace("-----------------------> packet data len=%d fRtpSeq =%d seq=%d packetNumber=%d\n",
																	thePacket->len,
																	fRtpSeq,
																	seq,
																	packetNumber);	
	#endif
	pLen=0;
	
	return OS_NO_ERROR;

}

RtcpPacket::RtcpPacket(void *root,void *parent):Packet(root,parent)
{
	pRtcpBuffer=new unsigned char[256*1024];
	pRtcpLen=0;
}
RtcpPacket::~RtcpPacket()
{

}

SRRtcpPacket::SRRtcpPacket(void *root,void *parent):RtcpPacket(root,parent)
{
	
}
SRRtcpPacket::~SRRtcpPacket()
{

}
int SRRtcpPacket::makeSRRtcpHeader(char *pData, 
										unsigned int ssrc)  
{   
	// 不完整
    bits_buffer_s   bitsBuffer;  
    if (pData == NULL){  
        return OS_FUNCTION_ARGS_ERROR; 
	} 
    bitsBuffer.i_size = RTCP_SR_HDR_LEN;  
    bitsBuffer.i_data = 0;  
    bitsBuffer.i_mask = 0x80;  
    bitsBuffer.p_data = (unsigned char *)(pData);  
    memset(bitsBuffer.p_data,'\0', RTCP_SR_HDR_LEN);  
    bitsWrite(&bitsBuffer, 2, 2);    		   	 /* rtcp version  */  
    bitsWrite(&bitsBuffer, 1, 0);              	 /* rtcp padding  */  
    bitsWrite(&bitsBuffer, 5, 0);              	 /* rtcp extension    */  
    bitsWrite(&bitsBuffer, 8, 200);              /* rtcp type SR=200    */  
    bitsWrite(&bitsBuffer, 16, 0);        		 /* rtcp sequence      */  
    bitsWrite(&bitsBuffer, 32, (ssrc));       	 /* rtcp SSRC      */  
    return OS_NO_ERROR;  
} 

int SRRtcpPacket::decode(char *pData,int len)
{
	// here,we do nothing
	
	return OS_NO_ERROR;
}
int SRRtcpPacket::encode()
{
	// 只组装头部数据
	/*
		① 版本（V）：2位，标识RTP版本。

       ② 填充(P)：1位，如设置于此位，RTCP包结尾包含一些附加填充八位组，它们不属于控制信息。最后一个八位组填充表示应忽略多少个填充。有些加密算法需要填充，块大小固定。在组合RTCP包内，填充仅在最后一个包中需要，因为组合包加密成一个整体。

       ③ 接收报告计数(RC)：5位，包含在包内的接收报告块数目，0值为有效。

       ④ 包类型(PT)：8位，包含常数200标识此包为RTCP的SR包。

       ⑤ 长度：16位，32位字RTCP包长度的一半。

       ⑥ SSRC：32位，同步源标识。
	*/
	
	RtpInstance *theInStance=(RtpInstance *)getRoot();

	unsigned char *pos=pRtcpBuffer;
	makeSRRtcpHeader((char *)pRtcpBuffer,theInStance->rtpGetRtpSSRC());
	pos+=8;
	pRtcpLen=pos-pRtcpBuffer;
	
	return OS_NO_ERROR;
}
RRRtcpPacket::RRRtcpPacket(void *root,void *parent):RtcpPacket(root,parent)
{

}
RRRtcpPacket::~RRRtcpPacket()
{

}
int RRRtcpPacket::makeRRRtcpHeader(char *pData, 
										unsigned int ssrc)  
{  
    bits_buffer_s   bitsBuffer;  
    if (pData == NULL){  
        return OS_FUNCTION_ARGS_ERROR; 
	} 
    bitsBuffer.i_size = RTCP_RR_HDR_LEN;  
    bitsBuffer.i_data = 0;  
    bitsBuffer.i_mask = 0x80;  
    bitsBuffer.p_data = (unsigned char *)(pData);  
    memset(bitsBuffer.p_data,'\0', RTCP_RR_HDR_LEN);  
    bitsWrite(&bitsBuffer, 2, 2);    		   /* rtp version  */  
    bitsWrite(&bitsBuffer, 1, 0);              /* rtcp padding  */  
    bitsWrite(&bitsBuffer, 5, 0);              /* rtcp extension    */  
    bitsWrite(&bitsBuffer, 8, 201);              /* rtcp type SR=200    */  
    bitsWrite(&bitsBuffer, 16, 0);        /* rtcp sequence      */  
    bitsWrite(&bitsBuffer, 32, (ssrc));        /* rtcp SSRC      */  
    return OS_NO_ERROR;  
} 

int RRRtcpPacket::decode(char *pData,int len)
{

	return OS_NO_ERROR;
}
int RRRtcpPacket::encode()
{
	RtpInstance *theInStance=(RtpInstance *)getRoot();

	unsigned char *pos=pRtcpBuffer;
	makeRRRtcpHeader((char *)pRtcpBuffer,theInStance->rtpGetRtpSSRC());
	pos+=8;
	pRtcpLen=pos-pRtcpBuffer;
	return OS_NO_ERROR;
}


SDESRtcpPacket::SDESRtcpPacket(void *root,void *parent):RtcpPacket(root,parent)
{

}
SDESRtcpPacket::~SDESRtcpPacket()
{

}
int SDESRtcpPacket::decode(char *pData,int len)
{

	return OS_NO_ERROR;
}
int SDESRtcpPacket::encode()
{

	return OS_NO_ERROR;
}
BYERtcpPacket::BYERtcpPacket(void *root,void *parent):RtcpPacket(root,parent)
{

}
BYERtcpPacket::~BYERtcpPacket()
{

}
int BYERtcpPacket::decode(char *pData,int len)
{

	return OS_NO_ERROR;
}
int BYERtcpPacket::encode()
{

	return OS_NO_ERROR;
}

APPRtcpPacket::APPRtcpPacket(void *root,void *parent):RtcpPacket(root,parent)
{

}
APPRtcpPacket::~APPRtcpPacket()
{

}
int APPRtcpPacket::decode(char *pData,int len)
{

	return OS_NO_ERROR;
}
int APPRtcpPacket::encode()
{

	return OS_NO_ERROR;
}


