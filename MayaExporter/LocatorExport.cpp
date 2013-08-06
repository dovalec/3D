#include "StdAfx.h"
#include "LocatorExport.h"
#include "Globals.h"

LocatorExport::LocatorExport(void)
{
}

LocatorExport::~LocatorExport(void)
{
}

void LocatorExport::Export(string folder, string fileName)
{
	MStatus stat;

	mFolder = folder;
	mFileName = fileName;

    //err_info("Locator folder: " +mFolder);
    //err_info("Locator file: " +mFileName);
    
	mSaver.SetFolder(mFolder);

	MItDependencyNodes itRenderLayers(MFn::kRenderLayer,&stat);
	err_code(stat);

	MFnRenderLayer locatorLayer;
	while (!itRenderLayers.isDone())
	{		
		MFnRenderLayer renderLayer( itRenderLayers.item() );

		if (renderLayer.name() == "locator")
		{
			stat = locatorLayer.setObject( renderLayer.object() );
			err_code(stat);
			break;
		}

		stat = itRenderLayers.next();
		err_code(stat);
	}

	vector<Circle> circlesVec;

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

		double scale[3]={1.0f};
		stat = parent.getScale(scale);
		err_code(stat);


		MBoundingBox bbox = circle.boundingBox(&stat);
		double bbox_w = bbox.width();
		double bbox_h = bbox.height();
		double bbox_d = bbox.depth();
		double bbox_b = sqrt(bbox_w*bbox_w+bbox_h*bbox_h+bbox_d*bbox_d);		
		float radius = (float)bbox_b * 0.5f;		


		Circle myCircle;

		myCircle.radius = radius;
		myCircle.x = (float)pos.x;
		myCircle.y = (float)pos.y;
		myCircle.z = (float)pos.z;

		myCircle.qx = -(float)quat.x;
		myCircle.qy = -(float)quat.y;
		myCircle.qz = -(float)quat.z;
		myCircle.qw = (float)quat.w;

		myCircle.sx = (float)scale[0];
		myCircle.sy = (float)scale[1];
		myCircle.sz = (float)scale[2];

		myCircle.name = circle.name().asChar();

		
		int locatorChildCount = parent.childCount(&stat);
		err_code(stat);
		
		for (int n=0;n<locatorChildCount;n++)
		{
			MObject childObj = parent.child(n, &stat);
			MFnDependencyNode childDepNode(childObj, &stat);
			err_code(stat);

			MString childFile;
			MString cmd = MString("reference -q -f ") + childDepNode.name();
			stat = MGlobal::executeCommand( cmd, childFile );
			if( stat == MS::kSuccess ) 
			{ 
				myCircle.refName = childFile.asChar();
				err_info(myCircle.name + string(" - using ref: ") + myCircle.refName);
				break;
			}
		}

		circlesVec.push_back(myCircle);
		stat = itDepCircle.next();
		err_code(stat);
	}


	WriteCircles(circlesVec);
}

void LocatorExport::WriteCircles(vector<Circle> & circlesVec)
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	int numOfCircles = circlesVec.size();

	if (numOfCircles == 0)
        return;
    
	mSaver.CreateFile(mFileName + ".loc");
	mSaver.writeInt(numOfCircles);

	for(int n=0 ; n<numOfCircles ; n++)
	{
		Circle& circle = circlesVec[n];
		mSaver.writeString(circle.name.c_str());
		if (circle.refName.length() > 1)
		{
			int lastDot = circle.refName.rfind(".");
			int lastSlash = circle.refName.rfind("/");
			string refName = circle.refName.substr(lastSlash+1, lastDot - lastSlash - 1);

			mSaver.writeInt(1);
			mSaver.writeString(refName.c_str());
		}
		else
		{
			mSaver.writeInt(0);
		}

		mSaver.writeFloat32(GLOBAL_SCALE*circle.x);
		mSaver.writeFloat32(GLOBAL_SCALE*circle.y);
		mSaver.writeFloat32(GLOBAL_SCALE*circle.z);
		
		mSaver.writeFloat32(circle.qx);
		mSaver.writeFloat32(circle.qy);
		mSaver.writeFloat32(circle.qz);
		mSaver.writeFloat32(circle.qw);

		mSaver.writeFloat32(circle.sx);
		mSaver.writeFloat32(circle.sy);
		mSaver.writeFloat32(circle.sz);

		mSaver.writeFloat32(GLOBAL_SCALE*circle.radius);
	}

}