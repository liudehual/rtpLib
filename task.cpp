#include "task.h"
Task::Task(void *root,void *parent):Object(root,parent),fSocket(NULL)
{

}
Task::~Task()
{
	if(fSocket){
		delete fSocket;
	}
}
Socket *Task::getSocket()
{
	return fSocket;
}

