#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "common.h"
#include <pthread.h>
class Timer;
// 线程类
class Thread:public Object
{
	public:
		Thread(void *root,void *parent);
		virtual ~Thread();

		// 初始化线程类
		// 初始化线程属性
		int initialization();

		// 启动线程
		int start();
		
		// 线程执行体
		static void *threadEntry(void *arg);

		// 线程类抽象接口
		virtual int entry()=0;

	private:
		pthread_t fThread;
		pthread_attr_t fAttr;
};
// 锁类
class Mutex
{
	public:
		Mutex();
		~Mutex();
		void lock();
		void unLock();
	private:
		pthread_mutex_t mutex;
};

// 加锁器
class  MutexLocker
{
    public:

        MutexLocker(Mutex *inMutexP) : fMutex(inMutexP) { if (fMutex != NULL) fMutex->lock(); }
        ~MutexLocker() {  if (fMutex != NULL) fMutex->unLock(); }
        
        void lock()         { if (fMutex != NULL) fMutex->lock(); }
        void unLock()       { if (fMutex != NULL) fMutex->unLock(); }
        
    private:

        Mutex*    fMutex;
};

#endif
