#include "ChasePlayerNode.h"
#include "../AI/Collision.h"
#include "../Shell.h"

NEWDEL_IMPL(ChasePlayerNode)
ChasePlayerNode::ChasePlayerNode()
{
	mMaxDist = GLOBAL_SCALE*24.0f;
	mMinDist = GLOBAL_SCALE*8.0f;
	mWalkDistance = GLOBAL_SCALE*0.1f;
}

ChasePlayerNode::~ChasePlayerNode()
{

}




