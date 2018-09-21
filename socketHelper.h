#ifndef __SOCKET_HELPER_H__
#define __SOCKET_HELPER_H__

#include "netCommon.h"

#if defined(__WIN32__) || defined(_WIN32)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#endif

#include "strPtr.h"

#ifdef __solaris__
    #ifndef INADDR_NONE
        #define INADDR_NONE     0xffffffff      /* -1 return from inet_addr */
    #endif
#endif

class SocketHelper
{
    public:

        // Call initialize before using any socket functions.
        // (pass true for lookupDNSName if you want the hostname
        // looked up via DNS during initialization -- %%sfu)
        static void Initialize(unsigned short lookupDNSName = true);
        
        //static utility routines
        static unsigned short   IsMulticastIPAddr(unsigned int inAddress);
        static unsigned short   IsLocalIPAddr(unsigned int inAddress);

        //This function converts an integer IP address to a dotted-decimal string.
        //This function is NOT THREAD SAFE!!!
        static void ConvertAddrToString(const struct in_addr& theAddr, StrPtr* outAddr);
        
        // This function converts a dotted-decimal string IP address to a unsigned int
        static unsigned int ConvertStringToAddr(const char* inAddr);
        
        //You can get at all the IP addrs and DNS names on this machine this way
        static unsigned int       GetNumIPAddrs() { return sNumIPAddrs; }
        static inline unsigned int        GetIPAddr(unsigned int inAddrIndex);
        static inline StrPtr*    GetIPAddrStr(unsigned int inAddrIndex);
        static inline StrPtr*    GetDNSNameStr(unsigned int inDNSIndex);
            
    private:

        //Utility function used by Initialize
#if defined(__WIN32__) || defined(_WIN32)
#else
        static unsigned short IncrementIfReqIter(char** inIfReqIter, ifreq* ifr);
#endif
        //For storing relevent information about each IP interface
        struct IPAddrInfo
        {
            unsigned int      fIPAddr;
            StrPtr   fIPAddrStr;
            StrPtr   fDNSNameStr;
        };
        
        static IPAddrInfo*              sIPAddrInfoArray;
        static unsigned int                   sNumIPAddrs;
};

inline unsigned int   SocketHelper::GetIPAddr(unsigned int inAddrIndex)
{
    return sIPAddrInfoArray[inAddrIndex].fIPAddr;
}

inline StrPtr*   SocketHelper::GetIPAddrStr(unsigned int inAddrIndex)
{
    return &sIPAddrInfoArray[inAddrIndex].fIPAddrStr;
}

inline StrPtr*   SocketHelper::GetDNSNameStr(unsigned int inDNSIndex)
{
    return &sIPAddrInfoArray[inDNSIndex].fDNSNameStr;
}


#endif
