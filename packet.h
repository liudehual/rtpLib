#ifndef __PACKET_H__
#define __PACKET_H__
#include "common.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <algorithm>
class Task;
class Mutex;


using namespace std;

typedef struct PacketData
{
	PacketData()
	{
		clear();
	}
	void clear()
	{
		timeStamp=0;
		fSeq=0;
		fPayload=0;
		fSsrc=0;
		marker=-1;

		len=0;

	}
	unsigned char buf[MAX_RTP_PACKET_BUFFER_SIZE]; // ����������
	unsigned int len; // ����ʵ�ʳ���

	int timeStamp; // ʱ���
	unsigned short fSeq; // ���к�
	int fPayload; // ��������
	int fSsrc; // ssrc
	int marker; 

}RtpPacketData;

// ���ฺ��rtp/rtcp���ݵı��롢���룬����ȹ���
// ���õ����������������
class Packet:public Object
{
	public:
		Packet(void *root,void *parent);
		virtual ~Packet();
		virtual int writeData(char *pData,int len);
		virtual int decode(char *pData,int len)=0;
		virtual int encode()=0;
		virtual int flushRtpDataSocket(Task *tmpTask);
		virtual int flushRtcpDataSocket(Task *tmpTask);
	protected:
		unsigned char *pBuffer;  //�˴���ʱ���û�����
        unsigned pLen; // pBuffer����������ʵ���ݳ���

		// RTP encode
		// �ڴ��
		std::vector<RtpPacketData *> fRtpPacketDataArray;
		unsigned short fRtpSeq;

		// RTP decode
		std::vector<RtpPacketData *> fRtpPacketDataDecodeArray;

		//RTCP
		unsigned char *pRtcpBuffer;
		unsigned pRtcpLen;
};

class RtpPacket:public Packet
{
	public:
		RtpPacket(void *root,void *parent);
		virtual ~RtpPacket();

		int makeRtpHeader(char *pData, 
									int markerFlag,
									int payloadType,
									unsigned short cseq, 
									long long curpts, 
									unsigned int ssrc);
		
		int makeTcpRtpHeader(char *pData,unsigned short len,unsigned char channel);
		int makeRtpPacket();
		void merge(std::vector<RtpPacketData *>& array, int l1, int r1, int l2, int r2 );
		void MergeSort1(std::vector<RtpPacketData *>& array, int start, int end);

		virtual int writeData(char *pData,int len);

		int insertPakectAndSort(char *pBuf,int pLen);
		int insertPacketToQueue(char *pBuf,int pLen);
		int sortQueue();
		int achieveCachingRequirements();
		int fineTuning();
		int getFrameData(std::vector<RtpPacketData *> &frame);
		
	protected:
		Mutex *fMutex;
};

class H264RtpPacket:public RtpPacket
{
	public:
		H264RtpPacket(void *root,void *parent);
		virtual ~H264RtpPacket();
		virtual int decode(char *pData,int len);
		virtual int encode();
};
class G711RtpPacket:public RtpPacket
{
	public:
		G711RtpPacket(void *root,void *parent);
		virtual ~G711RtpPacket();
		virtual int decode(char *pData,int len);
		virtual int encode();
};
class PSRtpPacket:public RtpPacket
{
	public:
		PSRtpPacket(void *root,void *parent);
		virtual ~PSRtpPacket();
		
		// udp
		virtual int decode(char *pData,int len);
		// udp and tcp
		virtual int encode();

	private:
		
};


class RtcpPacket:public Packet
{
	public:
		RtcpPacket(void *root,void *parent);
		virtual ~RtcpPacket();
};
class SRRtcpPacket:public RtcpPacket
{
	public:
		SRRtcpPacket(void *root,void *parent);
		virtual ~SRRtcpPacket();
		int makeSRRtcpHeader(char *pData,		unsigned int ssrc);
		virtual int decode(char *pData,int len);
		virtual int encode();
};
class RRRtcpPacket:public RtcpPacket
{
	public:
		RRRtcpPacket(void *root,void *parent);
		virtual ~RRRtcpPacket();
		int makeRRRtcpHeader(char *pData,		unsigned int ssrc);
		virtual int decode(char *pData,int len);
		virtual int encode();
};
class SDESRtcpPacket:public RtcpPacket
{
	public:
		SDESRtcpPacket(void *root,void *parent);
		virtual ~SDESRtcpPacket();
		virtual int decode(char *pData,int len);
		virtual int encode();
};
class BYERtcpPacket:public RtcpPacket
{
	public:
		BYERtcpPacket(void *root,void *parent);
		virtual ~BYERtcpPacket();
		virtual int decode(char *pData,int len);
		virtual int encode();
};
class APPRtcpPacket:public RtcpPacket
{
	public:
		APPRtcpPacket(void *root,void *parent);
		virtual ~APPRtcpPacket();
		virtual int decode(char *pData,int len);
		virtual int encode();
};


#endif
