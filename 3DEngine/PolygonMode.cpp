#include "PolygonMode.h"

NEWDEL_IMPL(PolygonMode)
PolygonMode::PolygonMode() {

	mCullMode = CullModeBackFace;
	mWindingMode = WindingModeCounterClockWise;
}

void PolygonMode::Apply() {
	// DOV GL.glPolygonMode(GL10.GL_FRONT_AND_BACK, GL10.GL_FILL);
	// GL.glPolygonMode(1032, 6914);
	if (mCullMode == CullModeNone) {
		ERR( glDisable( GL_CULL_FACE) );
	} else {
		if (mCullMode == CullModeBackFace)
		{
			ERR( glCullFace( GL_BACK) );
		}
		else
		{
			ERR( glCullFace( GL_FRONT) );
		}

		ERR( glEnable( GL_CULL_FACE) );
	}

	
	if (mWindingMode == WindingModeClockWise)
	{
		ERR( glFrontFace( GL_CW) );
	}
	else
	{
		ERR( glFrontFace( GL_CCW) );
	}

}
