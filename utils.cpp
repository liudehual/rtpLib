#include "utils.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

long long milliSeconds()
{
    struct timeval t;
    int theErr = ::gettimeofday(&t, NULL);

    long long curTime;
    curTime = t.tv_sec;
    curTime *= 1000;                // sec -> msec
    curTime += t.tv_usec / 1000;    // usec -> msec

    return curTime;
}
long getObjectId(void *objectPtr)
{
	char pointBuf[32]={'\0'};
	sprintf(pointBuf,"%u",objectPtr);
	return atoi(pointBuf);
}
unsigned int stringIpToUInt32(char * ip)
{
	if(ip==NULL){
		return 0;
	}
	return ntohl(::inet_addr(ip));
}
void UInt32ToStringIp(unsigned int uIntIp, char * strIp)
{
	if(strIp==NULL){
		return;
	}
	unsigned char* p = (unsigned char *)&uIntIp;
	sprintf(strIp, "%u.%u.%u.%u", p[3]&0xff,p[2]&0xff,p[1]&0xff,p[0]&0xff);
}

