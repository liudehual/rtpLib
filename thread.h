#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "common.h"
#include <pthread.h>
class Timer;
// �߳���
class Thread:public Object
{
	public:
		Thread(void *root,void *parent);
		virtual ~Thread();

		// ��ʼ���߳���
		// ��ʼ���߳�����
		int initialization();

		// �����߳�
		int start();
		
		// �߳�ִ����
		static void *threadEntry(void *arg);

		// �߳������ӿ�
		virtual int entry()=0;

	private:
		pthread_t fThread;
		pthread_attr_t fAttr;
};
// ����
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

// ������
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
