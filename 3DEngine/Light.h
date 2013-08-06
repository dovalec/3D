#ifndef __Light__
#define __Light__


#include "Config.h"

#include "Node.h"
#include "LightMode.h"
#include "Color.h"


class Light : Node
{
public:
	NEWDEL_DECL	
	void Apply();

};

#endif //__Light__

