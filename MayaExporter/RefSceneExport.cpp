#include "StdAfx.h"
#include "RefSceneExport.h"
#include "Globals.h"

RefSceneExport::RefSceneExport(void)
{
}

RefSceneExport::~RefSceneExport(void)
{
}

void RefSceneExport::Export(string folder, string fileName)
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

		if (renderLayer.name() == "ref")
		{
			stat = locatorLayer.setObject( renderLayer.object() );
			err_code(stat);
			break;
		}

		stat = itRenderLayers.next();
		err_code(stat);
	}

	vector<RefNode> refVec;

	MItDependencyNodes itDepTransform(MFn::kTransform,&stat);
	err_code(stat);

	while (!itDepTransform.isDone())
	{
		MObject obj = itDepTransform.item();
		MFnTransform topNode(obj, &stat);
		err_code(stat);

		bool objInLayer = locatorLayer.inLayer(obj, &stat);
		//err_code(stat);	

		if (!objInLayer)
		{
			stat = itDepTransform.next();
			err_code(stat);	
			continue;
		}

		// filter none ref nodes
		MString cmd = MString("reference -q -f ") + topNode.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat != MS::kSuccess ) 
		{ 
			itDepTransform.next();
			continue;
		}

		//filter non top nodes
		MObject parentObj = topNode.parent(0, &stat);
		err_code(stat);

		if (parentObj.apiType() != MFn::kWorld)
		{
			itDepTransform.next();
			continue;
		}

		MQuaternion quat;
		stat = topNode.getRotation(quat);
		err_code(stat);

		MVector pos = topNode.getTranslation(MSpace::kTransform, &stat);
		err_code(stat);

		double scale[3]={1.0f};
		stat = topNode.getScale(scale);
		err_code(stat);


		MBoundingBox bbox = topNode.boundingBox(&stat);
		double bbox_w = bbox.width();
		double bbox_h = bbox.height();
		double bbox_d = bbox.depth();
		double bbox_b = sqrt(bbox_w*bbox_w+bbox_h*bbox_h+bbox_d*bbox_d);		
		float radius = (float)bbox_b * 0.5f;		


		RefNode myRefNode;

		myRefNode.refName = file_id.asChar();

		myRefNode.radius = radius;
		myRefNode.x = (float)pos.x;
		myRefNode.y = (float)pos.y;
		myRefNode.z = (float)pos.z;

		myRefNode.qx = -(float)quat.x;
		myRefNode.qy = -(float)quat.y;
		myRefNode.qz = -(float)quat.z;
		myRefNode.qw = (float)quat.w;

		myRefNode.sx = (float)scale[0];
		myRefNode.sy = (float)scale[1];
		myRefNode.sz = (float)scale[2];

		myRefNode.name = topNode.name().asChar();

	
		refVec.push_back(myRefNode);
		stat = itDepTransform.next();
		err_code(stat);
	}


	WriteRefNodes(refVec);
}

void RefSceneExport::WriteRefNodes(vector<RefNode> & refVec)
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	int numOfRefNodes = refVec.size();

	if (numOfRefNodes == 0)
        return;
    
	mSaver.CreateFile(mFileName + ".ref");

	mSaver.writeInt(numOfRefNodes);

	for(int n=0 ; n<numOfRefNodes ; n++)
	{
		RefNode& ref = refVec[n];
		
		int lastDot = ref.refName.rfind(".");
		int lastSlash = ref.refName.rfind("/");
		string refName = ref.refName.substr(lastSlash+1, lastDot - lastSlash - 1);

		mSaver.writeString(refName.c_str());
		
		mSaver.writeFloat32(GLOBAL_SCALE*ref.x);
		mSaver.writeFloat32(GLOBAL_SCALE*ref.y);
		mSaver.writeFloat32(GLOBAL_SCALE*ref.z);
		
		mSaver.writeFloat32(ref.qx);
		mSaver.writeFloat32(ref.qy);
		mSaver.writeFloat32(ref.qz);
		mSaver.writeFloat32(ref.qw);

		mSaver.writeFloat32(ref.sx);
		mSaver.writeFloat32(ref.sy);
		mSaver.writeFloat32(ref.sz);

		mSaver.writeFloat32(GLOBAL_SCALE*ref.radius);
	}

}