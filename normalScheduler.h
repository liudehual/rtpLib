#ifndef __NORMAL__SCHEDULER_H__
#define __NORMAL__SCHEDULER_H__
#include <sys/epoll.h>
#include <map>
#include "common.h"
#include "thread.h"
#include "osHeap.h"

#define MAX_FD_SIZE 1024
#define MIN_TIME_INTERVAL 10

#define add_event EPOLL_CTL_ADD
#define mod_event EPOLL_CTL_MOD
#define del_event EPOLL_CTL_DEL

class Task;

// ������
class NormalScheduler:public Thread
{
	public:
		NormalScheduler(void *root,void *parent);
		virtual ~NormalScheduler();
		
		// ��ʼ��������
		int initialization();

		//����ʱ�¼�
		int processOvertimeEvent();

		//ʵ���߳�ִ�нӿ�
		virtual int entry();

		//��Ӷ��¼�
		int addReadEvent(Task *tmpTask);

		//���д�¼�
		int addWriteEvent(Task *tmpTask);

		//��Ӷ�ʱ��
		int addTimer(Timer *tmpTimer);

		int breakLoop();
	protected:

		// ɾ����ʱ��
		int delEvent(Task *tmpTask);
	private:
		int fEpollFd;	
		OSHeap fHeap;

		bool fLoop;
		
};

#endif