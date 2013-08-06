#ifndef __GotoTargetNode__
#define __GotoTargetNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../WiresData.h"
#include "MoveToTarget.h"

class GotoTargetNode :public MoveToTarget {
public:
	NEWDEL_DECL
	GotoTargetNode();
	virtual ~GotoTargetNode();

};

#endif //__GotoTargetNode__
