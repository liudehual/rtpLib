#include "timer.h"
#include "packet.h"
#include "client.h"
#include "rtpLib.h"
#include "rtpInstance.h"
#include "rtpRtcpTask.h"
#include "normalScheduler.h"
Timer::Timer(void *root,void *parent,int interval):Object(root,parent),
														fInterval(interval),
														fElem(root,this)
{
	fElem.SetEnclosingObject(this);
}
Timer::~Timer()
{
	
}
OSHeapElem *Timer::getHeapElem()
{
	return &fElem;
}
int Timer::getInterval()
{
	return fInterval;
}
int Timer::setInterval(int interval)
{
	fInterval=interval;
	return OS_NO_ERROR;
}
void Timer::start()
{
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	NormalScheduler *theScheduler=theInstance->getNormalScheduler();
	theScheduler->addTimer(this);
}

RtpSendTimer::RtpSendTimer(void *root,void *parent,int interval):Timer(root,parent,interval)
{
	
}
int RtpSendTimer::run()
{
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	// ��������Ч
	if(theInstance->getRtpInstanceModel()==TRANS_PUSH){
		RtpTask *theTask=(RtpTask *)this->getParent();
		Packet *thePacket=theTask->getRtpPacket();
		if(thePacket){
			thePacket->encode();
			thePacket->flushRtpDataSocket(theTask);
		}else{
			os_error("not find packet \n");
		}
	}else if(theInstance->getRtpInstanceModel()==TRANS_RECV){ // ������Ĭ�ϲ����κζ���������1����
		return 60000;
	}


	return 10;
}

RtcpSendTimer::RtcpSendTimer(void *root,void *parent,int interval)
												:Timer(root,parent,interval)
{


}
int RtcpSendTimer::run()
{
	// ����������˻��������ˣ������˷���SR,�����˷���RR
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	if(theInstance->getRtpInstanceModel()==TRANS_RECV){
		RRRtcpPacket thePacket(getRoot(),getParent());
		thePacket.encode();
		thePacket.flushRtcpDataSocket((Task *)getParent());
	}else if(theInstance->getRtpInstanceModel()==TRANS_PUSH){
		SRRtcpPacket thePacket(getRoot(),getParent());
		thePacket.encode();
		thePacket.flushRtcpDataSocket((Task *)getParent());
	}
	return 2000;
}
RtcpRecvTimer::RtcpRecvTimer(void *root,void *parent,int interval)
												:Timer(root,parent,interval)
{
	
	
}

int RtcpRecvTimer::run()
{
	// ������
	UdpRtcpTask *theRtcpTask=(UdpRtcpTask *)getParent();
	if(theRtcpTask->fRtpConnection){
		theRtcpTask->fRtpConnection=false;
		return 5000;
	}
	RtpInstance *theInstance=(RtpInstance *)getRoot();
	theInstance->handleExecption(0);

	os_error("[++++++++++++++++++++++++]connect is error \n");
	return 5000;
}
