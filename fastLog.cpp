#include "fastLog.h"
#include "sdkManager.h"
#include "utils.h"

#define LOG_TAIL '\n'
#define LOG_TAIL_SIZE 1


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
    static const int Disabled = 6;
};

int srs_get_log_level(std::string level)
{
    if ("verbose" == level) {
        return LogLevel::Verbose;
    } else if ("info" == level) {
        return LogLevel::Info;
    } else if ("trace" == level) {
        return LogLevel::Trace;
    } else if ("warn" == level) {
        return LogLevel::Warn;
    } else if ("error" == level) {
        return LogLevel::Error;
    } else {
        return LogLevel::Disabled;
    }
}


FastLog::FastLog(void *root,void *parent):Object(root,parent),fd(-1),_level(LogLevel::Trace)
{
	log_data=new char[LOG_MAX_SIZE];
}
FastLog::~FastLog()
{
}
int FastLog::initialize()
{
	pthread_mutex_init(&mutex, NULL);
	SdkManager *theManager=(SdkManager *)this->getRoot();
	// 默认输出到system.log文件(推荐用户指定log文件，console 为输出到屏幕)
    log_to_file_tank = theManager->getLogTankFile();
	// 默认等级为"trace"
    _level = srs_get_log_level(theManager->getLogLevel());
	if(_level==LogLevel::Disabled){
		fprintf(stderr,"log level is not available \n");
		return LOG_LEVEL_NOT_AVAILABLE;
	}
	return OS_NO_ERROR;
}
int FastLog::verbose(char* tag, int context_id, const char* fmt, va_list ap)
{
	int theError=OS_NO_ERROR;
    if (_level > LogLevel::Verbose) {
        return theError;
    }

	pthread_mutex_lock(&mutex);
    int size = 0;
    if ((theError=generate_header(false, tag, context_id, "verbose", &size))!=OS_NO_ERROR) {
		pthread_mutex_unlock(&mutex);
		return theError;
    }
    
   // we reserved 1 bytes for the new line.
    size += vsnprintf(log_data + size, LOG_MAX_SIZE - size, fmt, ap);
	theError=write_log(fd, log_data, size, LogLevel::Verbose);
	pthread_mutex_unlock(&mutex);
	
	return theError; 

}
int FastLog::info(char* tag, int context_id, const char* fmt, va_list ap)
{
	int theError=OS_NO_ERROR;

    if (_level > LogLevel::Info) {
        return theError;
    }
    pthread_mutex_lock(&mutex);
    int size = 0;
    if ((theError=generate_header(false, tag, context_id, "debug", &size))!=OS_NO_ERROR) {
		pthread_mutex_unlock(&mutex);
		return theError;
    }
    
    // we reserved 1 bytes for the new line.
    size += vsnprintf(log_data + size, LOG_MAX_SIZE - size, fmt, ap);

	theError=write_log(fd, log_data, size, LogLevel::Info);
	pthread_mutex_unlock(&mutex);

	return theError;

}
int FastLog::trace(char* tag, int context_id, const char* fmt, va_list ap)
{
	int theError=OS_NO_ERROR;

    if (_level > LogLevel::Trace) {
        return theError;
    }

	pthread_mutex_lock(&mutex);
    int size = 0;
    if ((theError=generate_header(false, tag, context_id, "trace", &size))!=OS_NO_ERROR) {
		pthread_mutex_unlock(&mutex);
		return theError;
    }
	
    // we reserved 1 bytes for the new line.
    size += vsnprintf(log_data + size, LOG_MAX_SIZE - size, fmt, ap);

	theError=write_log(fd, log_data, size, LogLevel::Trace);
	pthread_mutex_unlock(&mutex);
	
	return theError;

}
int FastLog::warn(char* tag, int context_id, const char* fmt, va_list ap)
{
	int theError=OS_NO_ERROR;

    if (_level > LogLevel::Warn) {
        return theError;
    }
    pthread_mutex_lock(&mutex);
    int size = 0;
    if ((theError=generate_header(false, tag, context_id, "warn", &size))!=OS_NO_ERROR) {
		pthread_mutex_unlock(&mutex);
		return theError;
    }
    
    // we reserved 1 bytes for the new line.
    size += vsnprintf(log_data + size, LOG_MAX_SIZE - size, fmt, ap);
	theError=write_log(fd, log_data, size, LogLevel::Warn);
	pthread_mutex_unlock(&mutex);

	return theError;

}
int FastLog::error(char* tag, int context_id, const char* fmt, va_list ap)
{
	int theError=OS_NO_ERROR;

    if (_level > LogLevel::Error) {
        return theError;
    }

	pthread_mutex_lock(&mutex);
    int size = 0;
    if ((theError=generate_header(false, tag, context_id, "error", &size))!=OS_NO_ERROR) {
		pthread_mutex_unlock(&mutex);
		return theError;
    }
    // we reserved 1 bytes for the new line.
    size += vsnprintf(log_data + size, LOG_MAX_SIZE - size, fmt, ap);
	theError=write_log(fd, log_data, size, LogLevel::Error);
	pthread_mutex_unlock(&mutex);
	
	return theError;
}

int FastLog::generate_header(bool error, const char* tag, int context_id, const char* level_name, int* header_size)
{
    // clock time
    timeval tv;
    if (gettimeofday(&tv, NULL) == -1){
        return LOG_GET_TIME_OF_DAY_ERROR;
    }
    
    // to calendar time
    struct tm* tm;
    if (utc=="on"){
        if((tm = gmtime(&tv.tv_sec)) == NULL){
            return LOG_CONVERT_CALENDAR_TIME_ERROR;
        }
    }else{
        if((tm = localtime(&tv.tv_sec)) == NULL){
            return LOG_CONVERT_CALENDAR_TIME_ERROR;
        }
    }
    // write log header
    int log_header_size = -1;
    
    if (error){
        if (tag){
            log_header_size = snprintf(log_data, LOG_MAX_SIZE, 
                "[%d-%02d-%02d %02d:%02d:%02d.%03d][%s][%s][%d][%d][%d] [RTP SDK] ", 
                1900 + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec / 1000), 
                level_name, tag, getpid(), context_id, errno);
        }else{
            log_header_size = snprintf(log_data, LOG_MAX_SIZE, 
                "[%d-%02d-%02d %02d:%02d:%02d.%03d][%s][%d][%d][%d] [RTP SDK] ", 
                1900 + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec / 1000), 
                level_name, getpid(), context_id, errno);
        }
    }else{
        if (tag){
            log_header_size = snprintf(log_data, LOG_MAX_SIZE, 
                "[%d-%02d-%02d %02d:%02d:%02d.%03d][%s][%s][%d][%d] [RTP SDK] ", 
                1900 + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec / 1000), 
                level_name, tag, getpid(), context_id);
        }else{
            log_header_size = snprintf(log_data, LOG_MAX_SIZE, 
                "[%d-%02d-%02d %02d:%02d:%02d.%03d][%s][%d][%d] ", 
                1900 + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec / 1000), 
                level_name, getpid(), context_id);
		}
    }

	
    if(log_header_size == -1){
        return LOG_GET_HEADER_ERROR;
    }
    // write the header size.
    *header_size = my_min(LOG_MAX_SIZE - 1, log_header_size);
    return OS_NO_ERROR;
}
int FastLog::write_log(int& fd, char* str_log, int size, int level)
{
	   int theError=OS_NO_ERROR;
	   // ensure the tail and EOF of string
	   // LOG_TAIL_SIZE for the TAIL char.
	   // 1 for the last char(0).
	   size = my_min(LOG_MAX_SIZE - 1 - LOG_TAIL_SIZE, size);
	   
	   // add some to the end of char.
	   // str_log[size++] = LOG_TAIL;
	   // if not to file, to console and return.
	   if (log_to_file_tank){
		   // if is error msg, then print color msg.
		   // \033[31m : red text code in shell
		   // \033[32m : green text code in shell
		   // \033[33m : yellow text code in shell
		   // \033[0m : normal text code
		   if (level <= LogLevel::Trace){
			   printf("%.*s", size, str_log);
		   } else if (level == LogLevel::Warn){
			   printf("\033[33m%.*s\033[0m", size, str_log);
		   } else{
			   printf("\033[31m%.*s\033[0m", size, str_log);
		   }
		   fflush(stdout);
	
		   return OS_NO_ERROR;
	   }
	   
	   // open log file. if specified
	   if (fd < 0){
			if((theError=open_log_file())!=OS_NO_ERROR){
				return theError;
			}
	   }
	   
	   // write log to file.
	   if (fd > 0){
		   ::write(fd, str_log, size);
	   }

	return OS_NO_ERROR;

}
int FastLog::open_log_file()
{
	SdkManager *theManager=(SdkManager *)this->getRoot();
	std::string filename = theManager->getLogTankFile();
	// In Fact,We don't care about file name empty or not
	if (filename.empty()){
		fprintf(stderr,"file name is empty\n");
		return LOG_FILE_NAME_ERROR;
	}	
	// Open Log File (System Api)
	fd = ::open(filename.c_str(), O_RDWR | O_APPEND);
	if(fd == -1 && errno == ENOENT){
		fd = open(filename.c_str(), 
					O_RDWR | O_CREAT | O_TRUNC, 
					S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if(fd=-1){
			return LOG_CREATE_LOG_FILE_ERROR;
		}
	}
	// OK
	return OS_NO_ERROR;
}


