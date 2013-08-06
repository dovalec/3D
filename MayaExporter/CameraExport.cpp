#include "StdAfx.h"
#include "CameraExport.h"
#include "ModelExporter.h"
#include "PVRTTriStrip.h"


CameraData::CameraData(void)
{
	index = -1;
	transformIndex = -1;
	
}

CameraData::~CameraData(void)
{
	
}

//////////////////////////////////////


CameraExport::CameraExport(void)
{
}

CameraExport::~CameraExport(void)
{
}


void CameraExport::Export()
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();

	modelExporter.ResetTimeline();

	MStatus stat;
	MItDependencyNodes itDep(MFn::kCamera,&stat);


	while (!itDep.isDone()) 
	{
		MObject obj = itDep.item();
		MFnCamera camera(obj, &stat);
		err_code(stat);

		if ( camera.isIntermediateObject() )
		{
			itDep.next();
			continue;
		}

		MString cmd = MString("reference -q -f ") + camera.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDep.next();
			continue;
		}

		MString cameraName = camera.name(&stat);
		err_code(stat);

		if (cameraName == "perspShape" || 
			cameraName == "topShape" ||
			cameraName == "frontShape" ||
			cameraName == "sideShape" )
		{
			itDep.next();
			continue;
		}
		err_info(string("CAMERA: ") + cameraName.asChar());

		CameraData * pCameraData = new CameraData();
		pCameraData->name = cameraName.asChar();

		pCameraData->aspectRatio = (float)camera.aspectRatio(&stat);
		err_code(stat);
		pCameraData->nearClippingPlane =  (float)camera.nearClippingPlane(&stat);
		err_code(stat);
		pCameraData->farClippingPlane = (float)camera.farClippingPlane(&stat);
		err_code(stat);
		pCameraData->horizontalFieldOfView = (float)camera.horizontalFieldOfView(&stat);
		err_code(stat);
		pCameraData->verticalFieldOfView = (float)camera.verticalFieldOfView(&stat);
		err_code(stat);

	
		MObject parentObj = camera.parent(0,&stat);
		MFn::Type parentType = parentObj.apiType();

		if ( modelExporter.CheckParentType( parentType) )
		{
			MFnDagNode parentDagNode(parentObj, &stat);
			MString transName = parentDagNode.name(&stat);

			pCameraData->transformIndex  = modelExporter.mSceneTransformsTable[transName.asChar()];
		}

		
		pCameraData->index = modelExporter.mCameraCount;
		modelExporter.mSceneCameras.push_back(pCameraData);

		modelExporter.mSceneCameraTable[cameraName.asChar()] = modelExporter.mCameraCount;
		modelExporter.mCameraCount++;


		stat = itDep.next();
		err_code(stat);
	}

	WriteCameras();
}


void CameraExport::WriteCameras()
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();
	Saver & saver = modelExporter.GetSaver();

	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	//Write number of cameras
	saver.writeInt(modelExporter.mSceneCameras.size()); //!!!!!!!!!!!!!!!

	vector<CameraData*>::iterator nodesIter = modelExporter.mSceneCameras.begin();
	while (nodesIter != modelExporter.mSceneCameras.end())
	{
		CameraData * pCameraData = *nodesIter;
				
		//Write index and parent
		saver.writeInt(pCameraData->index); //!!!!!!!!!!!!!!!
		saver.writeInt(pCameraData->transformIndex); //!!!!!!!!!!!!!!!
		saver.writeString(pCameraData->name.c_str()); //!!!!!!!!!!!!!!!
		saver.writeFloat32(pCameraData->aspectRatio); //!!!!!!!!!!!!!!!
		saver.writeFloat32(GLOBAL_SCALE*pCameraData->nearClippingPlane); //!!!!!!!!!!!!!!!
		saver.writeFloat32(GLOBAL_SCALE*pCameraData->farClippingPlane); //!!!!!!!!!!!!!!!
		saver.writeFloat32(pCameraData->horizontalFieldOfView); //!!!!!!!!!!!!!!!
		saver.writeFloat32(pCameraData->verticalFieldOfView); //!!!!!!!!!!!!!!!
		
		nodesIter++;
	}
}
