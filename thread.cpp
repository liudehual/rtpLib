#include "thread.h"
#include "error.h"
Thread::Thread(void *root,void *parent):Object(root,parent)
{

}
Thread::~Thread()
{
	pthread_cancel(fThread);
}
int Thread::initialization()
{

	/*初始化属性线程属性*/
  	pthread_attr_init(&fAttr);   
  	pthread_attr_setscope(&fAttr,PTHREAD_SCOPE_SYSTEM);   
  	pthread_attr_setdetachstate(&fAttr,PTHREAD_CREATE_DETACHED);   
	
	return OS_NO_ERROR;
}
int Thread::start()
{
	// 启动线程
	int ret=pthread_create(&fThread,&fAttr,Thread::threadEntry,this);
	if(ret==-1){
		return OS_CREATE_THREAD_ERROR;
	}
	return OS_NO_ERROR;
}

void *Thread::threadEntry(void *arg)
{
	// 线程执行体
	Thread *theThread=(Thread *)arg;
	theThread->entry();
}

Mutex::Mutex()
{
	pthread_mutex_init(&mutex,NULL);
}
Mutex::~Mutex()
{
	pthread_mutex_destroy(&mutex);
}
void Mutex::lock()
{
	pthread_mutex_lock(&mutex);
}
void Mutex::unLock()
{
	pthread_mutex_unlock(&mutex);
}




