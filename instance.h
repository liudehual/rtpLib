#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include "common.h"
class Instance:public Object
{
	public:
		Instance(void *root,void *parent);
		virtual ~Instance();
};
#endif
