#include "ConstraintManager.h"

NEWDEL_IMPL(ConstraintManager)
ConstraintManager::ConstraintManager()
{

}

void ConstraintManager::AddConstraint(Constraint * pConstraint)
{
	mConstraints.Add(pConstraint);
}


void ConstraintManager::RemoveConstraint(Constraint * pConstraint)
{
	mConstraints.Remove(pConstraint);
}

void ConstraintManager::OnTick()
{
	ListIterator iter(mConstraints);
	while (Constraint * pConstraint = (Constraint*)iter.GetPtr())
	{
		pConstraint->OnTick();
	}
	
}
