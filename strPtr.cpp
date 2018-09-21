#include <ctype.h>
#include "strPtr.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char       StrPtr::sCaseInsensitiveMask[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, //0-9 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, //10-19 
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, //20-29
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, //30-39 
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, //40-49
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, //50-59
    60, 61, 62, 63, 64, 97, 98, 99, 100, 101, //60-69 //stop on every character except a letter
    102, 103, 104, 105, 106, 107, 108, 109, 110, 111, //70-79
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, //80-89
    122, 91, 92, 93, 94, 95, 96, 97, 98, 99, //90-99
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, //100-109
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, //110-119
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129 //120-129
};

unsigned char StrPtr::sNonPrintChars[] =
{
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, //0-9     // stop
    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, //10-19    //'\r' & '\n' are not stop conditions
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //20-29
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, //30-39   
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //40-49
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //50-59
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //60-69  
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //70-79
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //80-89
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //90-99
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //100-109
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //110-119
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //120-129
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //130-139
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //140-149
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //150-159
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //160-169
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, //170-179
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //180-189
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //190-199
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //200-209
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //210-219
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //220-229
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //230-239
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //240-249
    1, 1, 1, 1, 1, 1             //250-255
};

char* StrPtr::GetAsCString() const
{
    // convert to a "NEW'd" zero terminated char array
    // caler is reponsible for the newly allocated memory
    char *theString = new char[Len+1];
    
    if ( Ptr && Len > 0 )
        ::memcpy( theString, Ptr, Len );
    
    theString[Len] = 0;
    
    return theString;
}


unsigned short StrPtr::Equal(const StrPtr &compare) const
{
    if (NULL == compare.Ptr && NULL == Ptr )
        return true;
        
        if ((NULL == compare.Ptr) || (NULL == Ptr))
        return false;

    if ((compare.Len == Len) && (memcmp(compare.Ptr, Ptr, Len) == 0))
        return true;
    else
        return false;
}

unsigned short StrPtr::Equal(const char* compare) const
{   
    if (NULL == compare && NULL == Ptr )
        return true;
        
    if ((NULL == compare) || (NULL == Ptr))
        return false;
        
    if ((::strlen(compare) == Len) && (memcmp(compare, Ptr, Len) == 0))
        return true;
    else
        return false;
}



unsigned short StrPtr::NumEqualIgnoreCase(const char* compare, const unsigned int len) const
{
    // compare thru the first "len: bytes
    
    if (len <= Len)
    {
        for (unsigned int x = 0; x < len; x++)
            if (sCaseInsensitiveMask[Ptr[x]] != sCaseInsensitiveMask[compare[x]])
                return false;
        return true;
    }
    return false;
}

unsigned short StrPtr::EqualIgnoreCase(const char* compare, const unsigned int len) const
{
    if (len == Len)
    {
        for (unsigned int x = 0; x < len; x++)
            if (sCaseInsensitiveMask[Ptr[x]] != sCaseInsensitiveMask[compare[x]])
                return false;
        return true;
    }
    return false;
}

char *StrPtr::FindStringCase(char *queryCharStr, StrPtr *resultStr, unsigned short caseSensitive) const
{
    // Be careful about exiting this method from the middle. This routine deletes allocated memory at the end.
    // 

    if (resultStr)
        resultStr->Set(NULL,0);

    if (NULL == queryCharStr) return NULL;
    if (NULL == Ptr) return NULL;
    if (0 == Len) return NULL;
    

    StrPtr queryStr(queryCharStr);
    char *editSource = NULL;
    char *resultChar = NULL;
    char lastSourceChar = Ptr[Len -1];
    
    if (lastSourceChar != 0) // need to modify for termination. 
    {   editSource = new char[Len + 1]; // Ptr could be a static string so make a copy
        ::memcpy( editSource, Ptr, Len );
        editSource[Len] = 0; // this won't work on static strings so we are modifing a new string here
    }

    char *queryString = queryCharStr;       
    char *dupSourceString = NULL;
    char *dupQueryString = NULL;
    char *sourceString = Ptr;
    unsigned int foundLen = 0;
    
    if (editSource != NULL) // a copy of the source ptr and len 0 terminated
        sourceString = editSource;
    
    if (!caseSensitive)
    {   dupSourceString = ::strdup(sourceString);
        dupQueryString = ::strdup(queryCharStr);                
        if (dupSourceString && dupQueryString) 
        {   sourceString = StrPtr(dupSourceString).ToUpper();
            queryString = StrPtr(dupQueryString).ToUpper();
            resultChar = ::strstr(sourceString,queryString);
			
			::free(dupSourceString);
			::free(dupQueryString);
        }
    }
    else
    {   resultChar = ::strstr(sourceString,queryString);        
    }
    
    if (resultChar != NULL) // get the start offset
    {   foundLen = resultChar - sourceString;
        resultChar = Ptr + foundLen;  // return a pointer in the source buffer
        if (resultChar > (Ptr + Len)) // make sure it is in the buffer
            resultChar = NULL;
    }
    
    if (editSource != NULL)  
        delete [] editSource;
    
    if (resultStr != NULL && resultChar != NULL)
        resultStr->Set(resultChar,queryStr.Len);
    

    return resultChar;
}


unsigned int StrPtr::RemoveWhitespace()
{
    if (Ptr == NULL || Len == 0)
        return 0;

    char *EndPtr = Ptr + Len; // one past last char
    char *destPtr = Ptr;
    char *srcPtr = Ptr;
    
    Len = 0;
    while (srcPtr < EndPtr)
    {
        
        if (*srcPtr != ' ' && *srcPtr != '\t')
        {    
            if (srcPtr != destPtr)
               *destPtr = *srcPtr;

             destPtr++;
             Len ++;
        }
        srcPtr ++;
    }

    return Len;
}

unsigned int StrPtr::TrimLeadingWhitespace()
{
    if (Ptr == NULL || Len == 0)
        return 0;

    char *EndPtr = Ptr + Len; //one past last char

    while (Ptr < EndPtr)
    {
        if (*Ptr != ' ' && *Ptr != '\t')
            break;
        
        Ptr += 1;
        Len -= 1; 
    }

    return Len;
}

unsigned int StrPtr::TrimTrailingWhitespace()
{
    if (Ptr == NULL || Len == 0)
        return 0;

    char *theCharPtr = Ptr + (Len - 1); // last char

    while (theCharPtr >= Ptr)
    {
        if (*theCharPtr != ' ' && *theCharPtr != '\t')
            break;
        
        theCharPtr -= 1;
        Len -= 1; 
    }
    
    return Len;
}

void StrPtr::PrintStr()
{
    char *thestr = GetAsCString();
    
    unsigned int i = 0;
    for (; i < Len; i ++) 
    { 
       if (StrPtr::sNonPrintChars[Ptr[i]]) 
       {   thestr[i] = 0;
           break;
       }
       
    } 
       
    if (thestr != NULL)
    {   
        delete thestr;
    }   
}

void StrPtr::PrintStr(char *appendStr)
{
    StrPtr::PrintStr();
    if (appendStr != NULL)
    	return;
}

void StrPtr::PrintStrEOL(char* stopStr, char *appendStr)
{
           
 
    char *thestr = GetAsCString();
    
    int i = 0;
    for (; i < (int) Len; i ++) 
    { 
       if (StrPtr::sNonPrintChars[Ptr[i]]) 
       {   thestr[i] = 0;
           break;
       }
       
    } 

    for (i = 0; thestr[i] != 0 ; i ++) 
    { 
       if (thestr[i] == '%' && thestr[i+1] != '%' ) 
       {   thestr[i] = '$';
       }       
    } 

    int stopLen = 0;
    if (stopStr != NULL)
        stopLen = ::strlen(stopStr);

    if (stopLen > 0 && stopLen <= i)
    {
        char* stopPtr = ::strstr(thestr, stopStr);
        if (stopPtr != NULL)
        {   stopPtr +=  stopLen;
           *stopPtr = 0;
           i = stopPtr - thestr;
        }
    }

    char * theStrLine = thestr;
    char * nextLine = NULL;
    char * theChar = NULL;
    static char cr[]="\\r";
    static char lf[]="\\n\n";
    int tempLen = i;
    for (i = 0; i < tempLen; i ++) 
    {   
        if (theStrLine[i] == '\r')
        {   theChar = cr;
            theStrLine[i] = 0;
            nextLine = &theStrLine[i+1];
        }
        else if (theStrLine[i] == '\n')
        {   theChar = lf;
            theStrLine[i] = 0;
            nextLine = &theStrLine[i+1];
        }
        
        if (nextLine != NULL)
        { 
            theStrLine = nextLine;
            nextLine = NULL;  
            tempLen -= (i+1);
            i = -1;
        }
    }
    delete thestr;

    if (appendStr != NULL)
			return;   
}

