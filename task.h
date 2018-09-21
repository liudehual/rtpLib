#ifndef __TASK_H__
#define __TASK_H__
#include "common.h"
#include "socket.h"
class Socket;
class Task:public Object
{
	public:
		Task(void *root,void *parent);
		virtual ~Task();
		virtual int run()=0;
		Socket *getSocket();
	protected:
		Socket *fSocket;
};
class TestTask:public Task
{
	public:
		TestTask(void *root,void *parent):Task(root,parent){}
		 ~TestTask(){}
		int run(){}
};
#endif
