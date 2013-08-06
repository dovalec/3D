#include "StdAfx.h"
#include "BotLauncherExport.h"

BotLauncherExport::BotLauncherExport(void)
{
}

BotLauncherExport::~BotLauncherExport(void)
{
}

void BotLauncherExport::Export(string folder, string fileName)
{
	MStatus stat;

	mFolder = folder;
	mFileName = fileName;

	mSaver.SetFolder(mFolder);


	MItDependencyNodes itRenderLayers(MFn::kRenderLayer,&stat);
	err_code(stat);

	MFnRenderLayer botLauncherLayer;
	while (!itRenderLayers.isDone())
	{		
		MFnRenderLayer renderLayer( itRenderLayers.item() );

		if (renderLayer.name() == "bot_launcher")
		{
			stat = botLauncherLayer.setObject( renderLayer.object() );
			err_code(stat);
			break;
		}

		stat = itRenderLayers.next();
		err_code(stat);
	}

	vector<BotLaunchePoint> botLaunchersVec;

	MItDependencyNodes itDepBotLaunchePoint(MFn::kNurbsSurface,&stat);
	err_code(stat);

	while (!itDepBotLaunchePoint.isDone())
	{
		MObject obj = itDepBotLaunchePoint.item();
		MFnNurbsSurface nurbsSurface(obj, &stat);
		err_code(stat);

		bool objInLayer = botLauncherLayer.inLayer(obj, &stat);
		//err_code(stat);	

		if (!objInLayer)
		{
			stat = itDepBotLaunchePoint.next();
			err_code(stat);	
			continue;
		}

		// dont know Y ... just in case
		MString cmd = MString("reference -q -f ") + nurbsSurface.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDepBotLaunchePoint.next();
			continue;
		}

		MBoundingBox bbox = nurbsSurface.boundingBox(&stat);
		double bbox_w = bbox.width();
		double bbox_h = bbox.height();
		double bbox_d = bbox.depth();
		double bbox_b = sqrt(bbox_w*bbox_w+bbox_h*bbox_h+bbox_d*bbox_d);		
		float radius = (float)bbox_b * 0.5f;		

		MObject parentObj = nurbsSurface.parent(0, &stat);
		err_code(stat);

		MFnTransform parent(parentObj, &stat);
		err_code(stat);



		MVector pos = parent.getTranslation(MSpace::kTransform, &stat);
		err_code(stat);


		BotLaunchePoint myBotLaunchePoint;
		myBotLaunchePoint.x = (float)pos.x;
		myBotLaunchePoint.y = (float)pos.y;
		myBotLaunchePoint.z = (float)pos.z;
		myBotLaunchePoint.radius = radius;
		myBotLaunchePoint.name = nurbsSurface.name().asChar();


		botLaunchersVec.push_back(myBotLaunchePoint);
		stat = itDepBotLaunchePoint.next();
		err_code(stat);
	}


	WriteBotLaunchePoints(botLaunchersVec);
}

void BotLauncherExport::WriteBotLaunchePoints(vector<BotLaunchePoint> & botLaunchersVec)
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	int numOfBotLaunchePoints = botLaunchersVec.size();
	if (numOfBotLaunchePoints == 0)
        return;
    
	mSaver.CreateFile(mFileName + ".blp");

	mSaver.writeInt(numOfBotLaunchePoints);

	for(int n=0 ; n<numOfBotLaunchePoints ; n++)
	{
		BotLaunchePoint& launcherPoint = botLaunchersVec[n];
		mSaver.writeString(launcherPoint.name.c_str());
		mSaver.writeFloat32(GLOBAL_SCALE*launcherPoint.x);
		mSaver.writeFloat32(GLOBAL_SCALE*launcherPoint.y);
		mSaver.writeFloat32(GLOBAL_SCALE*launcherPoint.z);
		mSaver.writeFloat32(GLOBAL_SCALE*launcherPoint.radius);
	}

}