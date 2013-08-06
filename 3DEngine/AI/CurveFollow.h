#ifndef __CurveFollow__
#define __CurveFollow__

#include "../Config.h"
#include "../CurvesData.h"
#include "../Node.h"



class CurveFollow
{
public:
	NEWDEL_DECL
	CurveFollow(void);
	~CurveFollow(void);

	void MoveNode(Curve * pCurve, Node * pNode);
	
	float GetDirectionAngle()
	{
		return mAngle;
	}

	PVRTVec3 & GetDirectionVector()
	{
		return mDirectionVec;
	}

	inline void Progress(Curve * pCurve, float add)
	{
		mCurrentPos+=add;
		if ( ceil(mCurrentPos) >= pCurve->numOfPoints )
		{
			mCurrentPos = 0.0f;
		}
	}

	void Reset();

private:
	float mCurrentPos;
	PVRTVec3 mWorldPos;
	int mIndexOnCurve;
	float mAngle;
	PVRTVec3 mDirectionVec;
	bool mNodeAtEnd;
};


#endif //__CurveFollow__