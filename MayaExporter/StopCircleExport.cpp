#include "StdAfx.h"
#include "StopCircleExport.h"
#include "Globals.h"

StopCircleExport::StopCircleExport(void)
{
}

StopCircleExport::~StopCircleExport(void)
{
}

void StopCircleExport::Export(string folder, string fileName)
{
	MStatus stat;

	mFolder = folder;
	mFileName = fileName;

	mSaver.SetFolder(mFolder);

	MItDependencyNodes itRenderLayers(MFn::kRenderLayer,&stat);
	err_code(stat);

	MFnRenderLayer locatorLayer;
	while (!itRenderLayers.isDone())
	{		
		MFnRenderLayer renderLayer( itRenderLayers.item() );

		if (renderLayer.name() == "stop_circle")
		{
			stat = locatorLayer.setObject( renderLayer.object() );
			err_code(stat);
			break;
		}

		stat = itRenderLayers.next();
		err_code(stat);
	}

	vector<StopCircle> circlesVec;

	MItDependencyNodes itDepCircle(MFn::kNurbsCurve,&stat);
	err_code(stat);

	while (!itDepCircle.isDone())
	{
		MObject obj = itDepCircle.item();
		MFnNurbsCurve circle(obj, &stat);
		err_code(stat);

		bool objInLayer = locatorLayer.inLayer(obj, &stat);
		//err_code(stat);	

		if (!objInLayer)
		{
			stat = itDepCircle.next();
			err_code(stat);	
			continue;
		}

		// dont know Y ... just in case
		MString cmd = MString("reference -q -f ") + circle.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDepCircle.next();
			continue;
		}

		MObject parentObj = circle.parent(0, &stat);
		err_code(stat);

		MFnTransform parent(parentObj, &stat);
		err_code(stat);

		MQuaternion quat;
		stat = parent.getRotation(quat);
		err_code(stat);

		MVector pos = parent.getTranslation(MSpace::kTransform, &stat);
		err_code(stat);

		
		MBoundingBox bbox = circle.boundingBox(&stat);
		double bbox_w = bbox.width();
		double bbox_h = bbox.height();
		double bbox_d = bbox.depth();
		double bbox_b = sqrt(bbox_w*bbox_w+bbox_h*bbox_h+bbox_d*bbox_d);		
		float radius = (float)bbox_b * 0.5f;		

		StopCircle myCircle;

		myCircle.radius = radius;
		myCircle.x = (float)pos.x;
		myCircle.y = (float)pos.y;
		myCircle.z = (float)pos.z;

		
		circlesVec.push_back(myCircle);
		stat = itDepCircle.next();
		err_code(stat);
	}


	WriteStopCircles(circlesVec);
}

void StopCircleExport::WriteStopCircles(vector<StopCircle> & circlesVec)
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	int numOfCircles = circlesVec.size();
	if (numOfCircles == 0)
        return;
    
	mSaver.CreateFile(mFileName + ".stc");

	mSaver.writeInt(numOfCircles);

	for(int n=0 ; n<numOfCircles ; n++)
	{
		StopCircle& circle = circlesVec[n];
		
		mSaver.writeFloat32(GLOBAL_SCALE*circle.x);
		mSaver.writeFloat32(GLOBAL_SCALE*circle.y);
		mSaver.writeFloat32(GLOBAL_SCALE*circle.z);
		
		mSaver.writeFloat32(GLOBAL_SCALE*circle.radius);
	}

}