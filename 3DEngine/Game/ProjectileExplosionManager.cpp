#include "ProjectileExplosionManager.h"

NEWDEL_IMPL(ProjectileExplosionManager)
ProjectileExplosionManager::ProjectileExplosionManager()
{
	mNumProjectileExplosionNodes = 0;
	mNumEnabledProjectileExplosions = 0;
}

ProjectileExplosionManager::~ProjectileExplosionManager()
{
	for (int n=0;n<mNumProjectileExplosionNodes;n++)
	{
		delete mProjectileExplosionNodes[n];
	}
}

void ProjectileExplosionManager::AddProjectileExplosionNode(ProjectileExplosionNode * pProjectileExplosionNode)
{
	if (mNumProjectileExplosionNodes == MAX_NUM_PROJECTILE_EXPLOSIONS)
	{
		LOGE("mNumProjectileExplosionNodes == MAX_NUM_PROJECTILE_EXPLOSIONS");
		STOP;
	}

	mProjectileExplosionNodes[mNumProjectileExplosionNodes] = pProjectileExplosionNode;
	mNumProjectileExplosionNodes++;
}


ProjectileExplosionNode * ProjectileExplosionManager::EnableProjectileExplosionNode()
{
	if (mNumEnabledProjectileExplosions == mNumProjectileExplosionNodes)
		return NULL;

	ProjectileExplosionNode * pProjectileExplosionNode = mProjectileExplosionNodes[mNumEnabledProjectileExplosions];
	if (pProjectileExplosionNode->IsEnabled())
		return NULL;

	pProjectileExplosionNode->Enable();
	mNumEnabledProjectileExplosions++;
	return pProjectileExplosionNode;
}

void ProjectileExplosionManager::DisableProjectileExplosionNode(int index)
{
	mProjectileExplosionNodes[index]->Disable();
	mNumEnabledProjectileExplosions--;
	ProjectileExplosionNode *pTmp = mProjectileExplosionNodes[mNumEnabledProjectileExplosions];
	mProjectileExplosionNodes[mNumEnabledProjectileExplosions] = mProjectileExplosionNodes[index];
	mProjectileExplosionNodes[index] = pTmp;
}


void ProjectileExplosionManager::RemoveProjectileExplosionNode(ProjectileExplosionNode * pProjectileExplosionNode)
{
	for (int n=0;n<mNumProjectileExplosionNodes;n++)
	{
		if (mProjectileExplosionNodes[n] == pProjectileExplosionNode)
		{
			mNumProjectileExplosionNodes--;
			mProjectileExplosionNodes[n] = mProjectileExplosionNodes[mNumProjectileExplosionNodes];
			break;
		}
	}
}

void ProjectileExplosionManager::RemoveProjectileExplosionNode(int index)
{
	mNumProjectileExplosionNodes--;
	mProjectileExplosionNodes[index] = mProjectileExplosionNodes[mNumProjectileExplosionNodes];
}

void ProjectileExplosionManager::OnTick()
{
	for (int n=0;n<mNumEnabledProjectileExplosions;n++)
	{
		mProjectileExplosionNodes[n]->OnTick();
		if (mProjectileExplosionNodes[n]->GetCurrentState() == ProjectileExplosionNode::state_finished)
		{
			DisableProjectileExplosionNode(n);
		}
	}
}

