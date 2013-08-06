#include "CompositingMode.h"

NEWDEL_IMPL(CompositingMode)
CompositingMode::CompositingMode() {

	mBlendingMode = BlendingModeReplace;
	mAlphaThreshold = 0.0f;
	mAlphaTestMode = AlphaTestModeAlways;

	mEnableDepth = true;
	mDepthTest = true;
	mDepthWrite = true;
	mColorWrite = true; 
	mAlphaWrite = true;

	mPolygonOffset.mFactor = 0.0f;
	mPolygonOffset.mUnits = 0.75f;
}

void CompositingMode::Apply() {
/*
	if (mEnableDepth && mBlendingMode == BlendingModeReplace) {
		ERR( glEnable( GL_DEPTH_TEST) );
		//ERR( glDepthMask(mDepthWrite) );

		if (mDepthTest)
		{
			ERR( glDepthFunc( GL_LEQUAL) );
		}
		else
		{
			ERR( glDepthFunc( GL_ALWAYS) );
		}
	} else {
		ERR( glDisable( GL_DEPTH_TEST) );
	}

*/
/*
	ERR( glColorMask(mColorWrite, mColorWrite, mColorWrite, mAlphaWrite) );
*/

/*
#ifdef GLES_1_0
	if (mAlphaThreshold == 0.0F)
	{
		GLES 2.0 glDisable( GL_ALPHA_TEST);
	}
	else {
		GLES 2.0 
		glEnable(GL_ALPHA_TEST);

		if (mAlphaTestMode == AlphaTestModeAlways)
			glAlphaFunc(GL_ALWAYS, mAlphaThreshold);
		else if (mAlphaTestMode == AlphaTestModeNever)
			glAlphaFunc(GL_NEVER, mAlphaThreshold);
		else if (mAlphaTestMode == AlphaTestModeLess)
			glAlphaFunc(GL_LESS, mAlphaThreshold);
		else if (mAlphaTestMode == AlphaTestModeEqual)
			glAlphaFunc(GL_EQUAL, mAlphaThreshold);
		else if (mAlphaTestMode == AlphaTestModeLessEqual)
			glAlphaFunc(GL_LEQUAL, mAlphaThreshold);
		else if (mAlphaTestMode == AlphaTestModeGreater)
			glAlphaFunc(GL_GREATER, mAlphaThreshold);
		else if (mAlphaTestMode == AlphaTestModeNotEqual)
			glAlphaFunc(GL_NOTEQUAL, mAlphaThreshold);
		else if (mAlphaTestMode == AlphaTestModeGreaterEqual)
			glAlphaFunc(GL_GEQUAL, mAlphaThreshold);
		
	}
#endif //GLES_1_0
*/

	if (mBlendingMode == BlendingModeReplace) {
		ERR( glDisable( GL_BLEND) );
	} else {
		if (mBlendingMode == BlendingModeAlphaAdd)
		{
			ERR( glBlendFunc(GL_SRC_ALPHA, GL_ONE) );
		}
		else if (mBlendingMode == BlendingModeAlpha)
		{
			ERR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
		}
		else if (mBlendingMode == BlendingModeModulate)
		{
			ERR( glBlendFunc(GL_DST_COLOR, GL_ZERO) );
		}
		else if (mBlendingMode == BlendingModeModulateX2)
		{
			ERR( glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR) );
		}
		ERR( glEnable( GL_BLEND) );
	}


	
	ERR( glPolygonOffset(mPolygonOffset.mFactor, mPolygonOffset.mUnits) );
	if (mPolygonOffset.mFactor != 0.0F || mPolygonOffset.mUnits != 0.0F)
	{
		ERR( glEnable( GL_POLYGON_OFFSET_FILL) ); 
	}
	else
	{
		ERR( glDisable(GL_POLYGON_OFFSET_FILL) );
	}
	
}
