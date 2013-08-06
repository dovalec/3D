#include "GotoTargetNode.h"
#include "../AI/Collision.h"
#include "../Shell.h"

NEWDEL_IMPL(GotoTargetNode)
GotoTargetNode::GotoTargetNode()
{
	mWalkDistance = GLOBAL_SCALE*0.1f;
	mMinDist = GLOBAL_SCALE*0.5f;
}

GotoTargetNode::~GotoTargetNode()
{

}





