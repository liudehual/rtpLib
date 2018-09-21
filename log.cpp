#include <stdarg.h>
#include "log.h"
#include "error.h"
#include "sdkManager.h"
#include "fastLog.h"




static SdkManager *sManager=NULL;

int initializationLog(void *arg)
{	int theError=OS_NO_ERROR;
	if(arg==NULL){
		return LOG_INIT_ERROR;
	}
	if(!sManager){
		sManager=(SdkManager *)arg;
	}
	if((theError=sManager->getFastLog()->initialize())!=OS_NO_ERROR){
		return theError;
	}
	return OS_NO_ERROR;
}

int info_out(char *tag,char *fmt, ...)
{
	int theError=OS_NO_ERROR;
	va_list ap;
	va_start(ap, fmt);
	if((theError=sManager->getFastLog()->info(tag,0,fmt,ap))!=OS_NO_ERROR){
		va_end(ap);
		return theError;
	}
	va_end(ap);
	return OS_NO_ERROR;
}
int error_out(char *tag,char *fmt, ...)
{
	int theError=OS_NO_ERROR;

	va_list ap;
	va_start(ap, fmt);
	if((theError=sManager->getFastLog()->error(tag,errno,fmt,ap))!=OS_NO_ERROR){
		va_end(ap);
		return theError;
	}
	va_end(ap);
	return OS_NO_ERROR;
}
int warn_out(char *tag,char *fmt, ...)
{
	int theError=OS_NO_ERROR;

	va_list ap;
	va_start(ap, fmt);
	if((theError=sManager->getFastLog()->warn(tag,0,fmt,ap))!=OS_NO_ERROR){
		va_end(ap);
		return theError;
	}
	va_end(ap);
	return OS_NO_ERROR;
}
int verbose_out(char *tag,char *fmt, ...)
{
	int theError=OS_NO_ERROR;

	va_list ap;
	va_start(ap, fmt);
	if((theError=sManager->getFastLog()->verbose(tag,0,fmt,ap))!=OS_NO_ERROR){
		va_end(ap);
		return theError;
	}
	va_end(ap);
	return OS_NO_ERROR;
}
int trace_out(char *tag,char *fmt, ...)
{

	int theError=OS_NO_ERROR;

	va_list ap;
	va_start(ap, fmt);
	if((theError=sManager->getFastLog()->trace(tag,0,fmt,ap))!=OS_NO_ERROR){
		va_end(ap);
		return theError;
	}
	va_end(ap);
	return OS_NO_ERROR;
}
int raw_data_out(char *fmt, ...)
{

	return OS_NO_ERROR;

}

