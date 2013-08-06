#ifndef __CompositingMode__
#define __CompositingMode__

#include "Config.h"

#include "PolygonOffset.h"
#include "BlendingMode.h"
#include "AlphaTestMode.h"


class CompositingMode
{
public:
	NEWDEL_DECL
	PolygonOffset mPolygonOffset ;
	BlendingMode mBlendingMode;
	float mAlphaThreshold;
	AlphaTestMode mAlphaTestMode;

	bool mEnableDepth;
	bool mDepthTest;
	bool mDepthWrite;
	bool mColorWrite;
	bool mAlphaWrite;

	CompositingMode();

	virtual void Apply();

};

#endif //__CompositingMode__

