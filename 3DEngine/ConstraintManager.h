#ifndef __ConstraintManager__
#define __ConstraintManager__

#include "Config.h"
#include "Constraint.h"
#include "TL/List.h"

class ConstraintManager {
public:
	ConstraintManager();
	NEWDEL_DECL
	inline static ConstraintManager * GetConstraintManager()
	{
		return GlobalHeap::GetConstraintManager();
	}

	void Destroy()
	{
		
	}

	void AddConstraint(Constraint * pConstraint);
	void RemoveConstraint(Constraint * pConstraint);

	void OnTick();
	

protected:
	List mConstraints;
};

#endif //__ConstraintManager__
