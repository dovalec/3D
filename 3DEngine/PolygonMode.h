#ifndef __PolygonMode__
#define __PolygonMode__


#include "Config.h"

#include "CullMode.h"
#include "WindingMode.h"


class PolygonMode
{
public:
	NEWDEL_DECL
	CullMode mCullMode;
	WindingMode mWindingMode;
	
	PolygonMode();

	virtual void Apply();
};

#endif //__PolygonMode__
