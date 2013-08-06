#ifndef __MaterialData__
#define __MaterialData__

#include "Config.h"

class MaterialData
{
public:
	NEWDEL_DECL
	MaterialData(void);
	~MaterialData(void);
	
	int index;
	int textureIndex;
	unsigned int hasAlpha;
	

};


#endif //__MaterialData__