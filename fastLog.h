#ifndef __FAST_LOG_H__
#define __FAST_LOG_H__
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>

#include "common.h"
#include "error.h"

#define LOG_MAX_SIZE 8196

class FastLog:public Object
{
public:
	FastLog(void *root,void *parent);
	~FastLog();
protected:
    // defined in LogLevel.
    // _level 值越大，显示的信息越多，默认trace
    /*
		class LogLevel
		{
			public:
    			// only used for very verbose debug, generally, 
    			// we compile without this level for high performance.
    			static const int Verbose = 1;
    			static const int Info = 2;
    			static const int Trace = 3;
    			static const int Warn = 4;
    			static const int Error = 5;
    			// specified the disabled level, no log, for utest.
    			static const int Disabled = -1;
		};
	*/ 
    int _level;
private:
    char* log_data;
    // log to file if specified log_file
    int fd;
    // whether log to file tank
    bool log_to_file_tank;
    // whether use utc time.
    std::string utc;

public:
    virtual int initialize();
    virtual int verbose(char* tag, int context_id, const char* fmt, va_list ap);
    virtual int info(char* tag, int context_id, const char* fmt, va_list ap);
    virtual int trace(char* tag, int context_id, const char* fmt, va_list ap);
    virtual int warn(char* tag, int context_id, const char* fmt, va_list ap);
    virtual int error(char* tag, int context_id, const char* fmt, va_list ap);

private:
    virtual int generate_header(bool error, const char* tag, int context_id, const char* level_name, int* header_size);
    virtual int write_log(int& fd, char* str_log, int size, int level);
    virtual int open_log_file();
private:
	pthread_mutex_t mutex;
};

#endif
