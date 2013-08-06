#ifndef __ChasePlayerNode__
#define __ChasePlayerNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../WiresData.h"
#include "MoveToTarget.h"

class ChasePlayerNode : public MoveToTarget {
public:
	NEWDEL_DECL
	ChasePlayerNode();
	virtual ~ChasePlayerNode();
};

#endif //__ChasePlayerNode__
