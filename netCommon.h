#ifndef __NET_COMMON_H__
#define __NET_COMMON_H__

#include <stdarg.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <ctype.h>

#define closeSocket close

#ifndef SOCKLEN_T
#define SOCKLEN_T int
#endif

#endif
