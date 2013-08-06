#ifndef __FollowPlayerNode__
#define __FollowPlayerNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../WiresData.h"
#include "MoveToTarget.h"

class FollowPlayerNode : public MoveToTarget {
public:
	NEWDEL_DECL
	FollowPlayerNode();
	virtual ~FollowPlayerNode();

	virtual void CalcNodePos();
	

};

#endif //__FollowPlayerNode__
