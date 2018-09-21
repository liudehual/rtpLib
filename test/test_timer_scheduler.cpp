#include "timer.h"
#include "normalScheduler.h"
int main()
{
	TestTimer *theTimer1=new TestTimer(NULL,NULL,1000);
	theTimer1->setContent("timer1");
	TestTimer *theTimer2=new TestTimer(NULL,NULL,3000);
	theTimer2->setContent("timer2");
	NormalScheduler *theScheduler=new NormalScheduler(NULL,NULL);
	theScheduler->initialization();
	theScheduler->addTimer(theTimer1);
	theScheduler->addTimer(theTimer2);
	theScheduler->start();

	for(;;){
		sleep(1);
	}
	return 0;
}
