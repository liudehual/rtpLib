#include <errno.h>
#include "normalScheduler.h"
#include "task.h"
#include "utils.h"
#include "timer.h"
#include "log.h"
NormalScheduler::NormalScheduler(void *root,void *parent):
											Thread(root,parent),
											fEpollFd(-1),
											fHeap(root,this,1024),
											fLoop(true)
{

}
NormalScheduler::~NormalScheduler()
{
	os_warn("NormalScheduler Quit\n");
	// 调度器跳出循环
	breakLoop();

	// 清理所有的定时器
	for(;;){
		//find one timer
		Timer *theTimer=NULL;
		if((fHeap.PeekMin() != NULL)){	 
			theTimer=(Timer*)fHeap.ExtractMin()->GetEnclosingObject();
			fHeap.Remove(theTimer->getHeapElem());
		}else{ // not find,break the loop
			break;
		}
	}

	//usleep(100*1000);
}
int NormalScheduler::initialization()
{
	// 创建epoll fd
	fEpollFd=epoll_create(MAX_FD_SIZE); // create epoll socket fd
	if(fEpollFd == -1){
		os_error("epoll_create error \n");
		return OS_CREATE_EPOLL_ERROR;
	}
	
	// 初始化线程属性
	int theError=OS_NO_ERROR;
	if((theError=Thread::initialization())!=OS_NO_ERROR){
		os_error("init thread error \n");
		return theError;
	}
	return OS_NO_ERROR;
}

int NormalScheduler::addReadEvent(Task *tmpTask)
{
	struct epoll_event ev;
	ev.data.ptr = tmpTask;
	ev.events = EPOLLIN | EPOLLET;
	if(epoll_ctl(fEpollFd, add_event,tmpTask->getSocket()->getSocketFD(), &ev)==-1){
		os_error("epoll_ctl somethin is error while add event \n");
		return OS_EPOLL_ADD_EVENT_ERROR;
	}
	os_trace("add event ok socket_fd=%d \n",tmpTask->getSocket()->getSocketFD());
	return OS_NO_ERROR;
}

int NormalScheduler::addWriteEvent(Task *tmpTask)
{
	struct epoll_event ev;
	ev.data.ptr = tmpTask;
	ev.events = EPOLLOUT;
	if(epoll_ctl(fEpollFd, add_event,tmpTask->getSocket()->getSocketFD(), &ev)==-1){
		os_error("epoll_ctl somethin is error while add event \n");
		return OS_EPOLL_ADD_EVENT_ERROR;
	}
	return OS_NO_ERROR;

}

int NormalScheduler::delEvent(Task *tmpTask)
{
	// del event from epoll
	struct epoll_event ev;
	ev.data.ptr = NULL;
	ev.events = 0;
	if(epoll_ctl(fEpollFd, del_event,tmpTask->getSocket()->getSocketFD(), &ev)==-1){
		os_error("epoll_ctl somethin is error while del event \n");
		return OS_EPOLL_DEL_EVENT_ERROR;
	}
	return OS_NO_ERROR;
}
int NormalScheduler::addTimer(Timer *tmpTimer)
{
	os_trace("add timer succeed %p\n",tmpTimer);
	long long theCurrentTime = milliSeconds();	
	theCurrentTime+=tmpTimer->getInterval();
	tmpTimer->getHeapElem()->SetValue(theCurrentTime);
	fHeap.Insert(tmpTimer->getHeapElem());

	return OS_NO_ERROR;
}
int NormalScheduler::breakLoop()
{
	fLoop=false;
	return OS_NO_ERROR;
}


int NormalScheduler::processOvertimeEvent()
{

	for(;;){
		Timer *theTimer=NULL;

		// get current time
		long long theCurrentTime = milliSeconds();
	
		//find one timer
		if((fHeap.PeekMin() != NULL) 
				&& (fHeap.PeekMin()->GetValue() <= theCurrentTime)){	 
			theTimer=(Timer*)fHeap.ExtractMin()->GetEnclosingObject();
		}else{ // not find,break the loop
			break;
		}

		// run the timer
		int timeout=theTimer->run();

		// if timeout<=-1,we should kill this timer
		if(timeout <= -1){
			fHeap.Remove(theTimer->getHeapElem());
			//delete theTimer;
			continue;
		}
		// if timeout>=0 and timeout<10,we set timeout=10
		if(timeout < MIN_TIME_INTERVAL && timeout >= 0){
			timeout=MIN_TIME_INTERVAL;
		}
		// remove this timer from heap
		fHeap.Remove(theTimer->getHeapElem());

		// compute new time
		long long execTime = milliSeconds()+timeout;
		theTimer->getHeapElem()->SetValue(execTime);

		// insert this timer to the heap 
		fHeap.Insert(theTimer->getHeapElem());
	}

	return OS_NO_ERROR;
}

int NormalScheduler::entry()
{
	struct epoll_event revents[MAX_FD_SIZE] = {'\0'};
	for(;;){
		if(!fLoop){
			break;
		}
		// wait event happen
		int ret=epoll_wait(fEpollFd,revents,MAX_FD_SIZE,MIN_TIME_INTERVAL);
		if(ret==-1){
			if(errno==EINTR){
				continue;
			}else{
				os_error("epoll_wait something is wrong while waiting event ret=%d \n",ret);
				break;
			}
		}
		//continue;
		// process timer event
		processOvertimeEvent();

		// process read event,may be we don't care about write event
		for(int i=0;i<ret;i++){
			//os_trace("some event happend \n");
			if(revents[i].events & EPOLLIN){
				Task *theTask=(Task *)revents[i].data.ptr;
				if(!theTask){// will happen???
					continue;
				}
				int theStatus=theTask->run();
				if(theStatus==-1){
					delEvent(theTask);
					//delete theTask;
				}
			}else if(revents[i].events & EPOLLOUT){
				// do nothing 
			}
		}
	}
	// good choice,close epoll fd
	close(fEpollFd);
	return OS_NO_ERROR;
}

