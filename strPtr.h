#ifndef __STR_PTR_H__
#define __STR_PTR_H__
#include <string.h>
#include <ctype.h> 

#define StrPtrTESTING 0

class StrPtr
{
    public:

        //CONSTRUCTORS/DESTRUCTOR
        //These are so tiny they can all be inlined
        StrPtr() : Ptr(NULL), Len(0) {}
        StrPtr(char* sp) : Ptr(sp), Len(sp != NULL ? strlen(sp) : 0) {}
        StrPtr(char *sp, unsigned int len) : Ptr(sp), Len(len) {}
        virtual ~StrPtr(){}
        
        //OPERATORS:
        unsigned short Equal(const StrPtr &compare) const;
        unsigned short EqualIgnoreCase(const char* compare, const unsigned int len) const;
        unsigned short EqualIgnoreCase(const StrPtr &compare) const { return EqualIgnoreCase(compare.Ptr, compare.Len); }
        unsigned short Equal(const char* compare) const;
        unsigned short NumEqualIgnoreCase(const char* compare, const unsigned int len) const;
        
        void Delete() { delete [] Ptr; Ptr = NULL; Len = 0; }
        char *ToUpper() { for (unsigned int x = 0; x < Len ; x++) Ptr[x] = toupper (Ptr[x]); return Ptr;}
        
        char *FindStringCase(char *queryCharStr, StrPtr *resultStr, unsigned short caseSensitive) const;

        char *FindString(StrPtr *queryStr, StrPtr *outResultStr)              { return FindStringCase(queryStr->Ptr, outResultStr,true);    
                                                                                    }
        
        char *FindStringIgnoreCase(StrPtr *queryStr, StrPtr *outResultStr)    {
                                                                                        return FindStringCase(queryStr->Ptr, outResultStr,false); 
                                                                                    }

        char *FindString(StrPtr *queryStr)                                       {   return FindStringCase(queryStr->Ptr, NULL,true);    
                                                                                    }
        
        char *FindStringIgnoreCase(StrPtr *queryStr)                             {   return FindStringCase(queryStr->Ptr, NULL,false); 
                                                                                    }
                                                                                    
        char *FindString(char *queryCharStr)                                        { return FindStringCase(queryCharStr, NULL,true);   }
        char *FindStringIgnoreCase(char *queryCharStr)                              { return FindStringCase(queryCharStr, NULL,false);  }
        char *FindString(char *queryCharStr, StrPtr *outResultStr)               { return FindStringCase(queryCharStr, outResultStr,true);   }
        char *FindStringIgnoreCase(char *queryCharStr, StrPtr *outResultStr)     { return FindStringCase(queryCharStr, outResultStr,false);  }

        char *FindString(StrPtr &query, StrPtr *outResultStr)                 { return FindString( &query, outResultStr);             }
        char *FindStringIgnoreCase(StrPtr &query, StrPtr *outResultStr)       { return FindStringIgnoreCase( &query, outResultStr);   }
        char *FindString(StrPtr &query)                                          { return FindString( &query);           }
        char *FindStringIgnoreCase(StrPtr &query)                                { return FindStringIgnoreCase( &query); }
        
        StrPtr& operator=(const StrPtr& newStr) { Ptr = newStr.Ptr; Len = newStr.Len;
                                                        return *this; }
         char operator[](int i) { /*Assert(i<Len);i*/ return Ptr[i]; }
        void Set(char* inPtr, unsigned int inLen) { Ptr = inPtr; Len = inLen; }
        void Set(char* inPtr) { Ptr = inPtr; Len = (inPtr) ?  ::strlen(inPtr) : 0; }

        //This is a non-encapsulating interface. The class allows you to access its
        //data.
        char*       Ptr;
        unsigned int      Len;

        // convert to a "NEW'd" zero terminated char array
        char*   GetAsCString() const;
        void    PrintStr();
        void    PrintStr(char *appendStr);
        void    PrintStrEOL(char* stopStr = NULL, char *appendStr = NULL);
 
        //Utility function
        unsigned int    TrimTrailingWhitespace();
        unsigned int    TrimLeadingWhitespace();
   
        unsigned int  RemoveWhitespace();
        void  TrimWhitespace() { TrimLeadingWhitespace(); TrimTrailingWhitespace(); }


    private:

        static unsigned char    sCaseInsensitiveMask[];
        static unsigned char    sNonPrintChars[];
};


#endif
