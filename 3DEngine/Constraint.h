#ifndef __Constraint__
#define __Constraint__

#include "Config.h"
#include "Node.h"
#include "Camera.h"


class Constraint {//: public StopWireUser {
public:

	enum ConstraintType {
		CONSTRAINT_TYPE_MTX ,
		CONSTRAINT_TYPE_ROTATION ,
		CONSTRAINT_TYPE_TRANSLATION,
		CONSTRAINT_TYPE_ROTATION_TRANSLATION,		
		CONSTRAINT_TYPE_ROTATION_TRANSLATION_XZ,
		CONSTRAINT_TYPE_SCALE,
	};

	NEWDEL_DECL
	Constraint();
	virtual ~Constraint();


	void SetTargetNode(Node * pNode);
	inline Node * GetTargetNode()
	{
		return mTargetNode;
	}

	
	void SetNode(Node * pNode);
	inline Node * GetNode()
	{
		return mNode;
	}
	
	void SetConstraintType(ConstraintType type)
	{
		mConstraintType = type;
	}

	void SetConstraintShift(PVRTVec3 shift)
	{
		mConstraintShift = shift;
	}

	inline ConstraintType GetConstraintType()
	{
		return mConstraintType;
	}

	void OnTick();
	
protected:

	Node * mTargetNode;
	Node * mLookAtTargetNode;
	Node * mNode;
	ConstraintType mConstraintType;
	PVRTVec3 mConstraintShift;

};

#endif //__Constraint__
