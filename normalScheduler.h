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

// 调度类
class NormalScheduler:public Thread
{
	public:
		NormalScheduler(void *root,void *parent);
		virtual ~NormalScheduler();
		
		// 初始化调度类
		int initialization();

		//处理超时事件
		int processOvertimeEvent();

		//实现线程执行接口
		virtual int entry();

		//添加读事件
		int addReadEvent(Task *tmpTask);

		//添加写事件
		int addWriteEvent(Task *tmpTask);

		//添加定时器
		int addTimer(Timer *tmpTimer);

		int breakLoop();
	protected:

		// 删除定时器
		int delEvent(Task *tmpTask);
	private:
		int fEpollFd;	
		OSHeap fHeap;

		bool fLoop;
		
};

#endif