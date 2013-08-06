#include "BotLauncher.h"
#include "BotNode.h"

NEWDEL_IMPL(BotLauncher)
BotLauncher::BotLauncher()
{
	//mBotManager.SetBotLauncher(this);
}

BotLauncher::~BotLauncher()
{
	
}

void BotLauncher::SetPosition(PVRTVec3 pos)
{
	mPosition = pos;
}

void BotLauncher::SetPosition(float x, float y, float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
}

void BotLauncher::SetRadius(float radius)
{
	mRadius = radius;
}

void BotLauncher::SetName(String name)
{
	mName = name;
}


String BotLauncher::GetName()
{
	return mName;
}

void BotLauncher::Transform(PVRTMat4 mtx)
{
	PVRTVec4 vec1( mPosition.x, mPosition.y,  mPosition.z, 1);
	
	vec1 = mtx * vec1;
	
	mPosition.x = vec1.x;
	mPosition.y = vec1.y;
	mPosition.z = vec1.z;

}