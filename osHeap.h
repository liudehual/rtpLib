#ifndef __HEAP_H__
#define __HEAP_H__
#include <stdio.h>
#include "common.h"
#include "error.h"
#include "log.h"
class OSHeapElem;
class OSHeap:public Object
{
    public:
    
        enum
        {
            kDefaultStartSize = 1024 //unsigned int
        };
        
        OSHeap(void *root,void *parent,unsigned int inStartSize = kDefaultStartSize);
        ~OSHeap() { if (fHeap != NULL) delete fHeap; }
        
        //ACCESSORS
        unsigned int      CurrentHeapSize() { return fFreeIndex - 1; }
        OSHeapElem* PeekMin() { if (CurrentHeapSize() > 0) return fHeap[1]; return NULL; }
        
        //MODIFIERS
        
        //These are the two primary operations supported by the heap
        //abstract data type. both run in log(n) time.
        void            Insert(OSHeapElem*  inElem);
        OSHeapElem*     ExtractMin() { return Extract(1); }
        //removes specified element from the heap
        OSHeapElem*     Remove(OSHeapElem* elem);
        
   
    private:
    
        OSHeapElem*     Extract(unsigned int index);
    
    
        OSHeapElem**    fHeap;
        unsigned int          fFreeIndex;
        unsigned int          fArraySize;
};

class OSHeapElem:public Object
{
    public:
        OSHeapElem(void *root,void *parent,void* enclosingObject = NULL)
            : Object(root,parent),fValue(0), fEnclosingObject(enclosingObject), fCurrentHeap(NULL) {}
        ~OSHeapElem() {}
        
        //This data structure emphasizes performance over extensibility
        //If it were properly object-oriented, the compare routine would
        //be virtual. However, to avoid the use of v-functions in this data
        //structure, I am assuming that the objects are compared using a 64 bit number.
        //
        void    SetValue(signed long long newValue) { fValue = newValue; }
        signed long long  GetValue()              { return fValue; }
        void*   GetEnclosingObject()    { return fEnclosingObject; }
		void	SetEnclosingObject(void* obj) { fEnclosingObject = obj; }
        unsigned short  IsMemberOfAnyHeap()     { return fCurrentHeap != NULL; }
        
    private:
    
        signed long long  fValue;
        void* fEnclosingObject;
        OSHeap* fCurrentHeap;
        
        friend class OSHeap;
};


#endif
