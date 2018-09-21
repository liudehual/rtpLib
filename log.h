#ifndef __LOG_H__
#define __LOG_H__
#include <string>

int initializationLog(void *arg);

/* new log interface */
int info_out(char *tag,char *fmt, ...);
int error_out(char *tag,char *fmt, ...);
int warn_out(char *tag,char *fmt, ...);
int verbose_out(char *tag,char *fmt, ...);
int trace_out(char *tag,char *fmt, ...);
int raw_data_out(char *fmt, ...);

// donot print method
#define os_verbose(msg, ...) (void)verbose_out((char *)__FUNCTION__,msg, ##__VA_ARGS__)
#define os_info(msg, ...)    (void)info_out((char *)__FUNCTION__,msg, ##__VA_ARGS__)
#define os_trace(msg, ...)   (void)trace_out((char *)__FUNCTION__,msg, ##__VA_ARGS__)
#define os_warn(msg, ...)    (void)warn_out((char *)__FUNCTION__,msg, ##__VA_ARGS__)
#define os_error(msg, ...)   (void)error_out((char *)__FUNCTION__,msg, ##__VA_ARGS__)

// TODO: FIXME: add more verbose and info logs.
#if 0
    #undef os_verbose
    #define os_verbose(msg, ...) (void)0
#endif
#if 0
    #undef os_info
    #define os_info(msg, ...) (void)0
#endif
#if 0
    #undef os_trace
    #define os_trace(msg, ...) (void)0
#endif


#endif
