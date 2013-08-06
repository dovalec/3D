#include "StdAfx.h"
#include "CurveExport.h"

CurveExport::CurveExport(void)
{
}

CurveExport::~CurveExport(void)
{
}

void CurveExport::Export(string folder, string fileName)
{
	MStatus stat;

	mFolder = folder;
	mFileName = fileName;

	mSaver.SetFolder(mFolder);

	MItDependencyNodes itRenderLayers(MFn::kRenderLayer,&stat);
	err_code(stat);

	MFnRenderLayer curveLayer;
	while (!itRenderLayers.isDone())
	{		
		MFnRenderLayer renderLayer( itRenderLayers.item() );

		if (renderLayer.name() == "curve")
		{
			stat = curveLayer.setObject( renderLayer.object() );
			err_code(stat);
			break;
		}

		stat = itRenderLayers.next();
		err_code(stat);
	}

	MItDependencyNodes itDepCurve(MFn::kNurbsCurve,&stat);
	err_code(stat);

	vector<Curve> curves;

	while (!itDepCurve.isDone())
	{
		MObject obj = itDepCurve.item();
		MFnNurbsCurve curve(obj, &stat);
		err_code(stat);

		bool objInLayer = curveLayer.inLayer(obj, &stat);
		//err_code(stat);	

		if (!objInLayer)
		{
			stat = itDepCurve.next();
			err_code(stat);	
			continue;
		}

		// dont know Y ... just in case
		MString cmd = MString("reference -q -f ") + curve.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDepCurve.next();
			continue;
		}

		MObject parentObj = curve.parent(0, &stat);
		err_code(stat);

		Curve newCurve;
		newCurve.name = curve.name().asChar();

		double param = 0;
		while ( param < 1.0 )
		{
			MPoint point;
			stat = curve.getPointAtParam(param, point);
			err_code(stat);
			MVector tangent = curve.tangent(param);
			err_code(stat);
			stat = tangent.normalize();
			err_code(stat);

			CurvePoint curvePoint;
			curvePoint.x = (float)point.x;
			curvePoint.y = (float)point.y;
			curvePoint.z = (float)point.z;

			curvePoint.tx = (float)tangent.x;
			curvePoint.ty = (float)tangent.y;
			curvePoint.tz = (float)tangent.z;

			newCurve.curvePoints.push_back(curvePoint);
			param+=.01;

		}
		curves.push_back(newCurve);

		stat = itDepCurve.next();
		err_code(stat);
	}

	WriteCurves(curves);


}

void CurveExport::WriteCurves(vector<Curve> & curves)
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	int numOfCurves = curves.size();
	if (numOfCurves == 0)
        return;
    
	mSaver.CreateFile(mFileName + ".crv");

	mSaver.writeInt(numOfCurves);

	for(int c = 0 ; c < numOfCurves ; c++)
	{
		Curve & curve = curves[c];
		int numOfPoints = curve.curvePoints.size();

		mSaver.writeString(curve.name.c_str());
		mSaver.writeInt(numOfPoints);
		for( int n=0; n<numOfPoints ; n++)
		{
			CurvePoint & curvePoint = curve.curvePoints[n];
			mSaver.writeFloat32(GLOBAL_SCALE*curvePoint.x);
			mSaver.writeFloat32(GLOBAL_SCALE*curvePoint.y);
			mSaver.writeFloat32(GLOBAL_SCALE*curvePoint.z);
			mSaver.writeFloat32(curvePoint.tx);
			mSaver.writeFloat32(curvePoint.ty);
			mSaver.writeFloat32(curvePoint.tz);
		}
	}
}

