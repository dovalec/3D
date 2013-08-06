#include "StdAfx.h"
#include "ModelExporter.h"
#include "TransformExport.h"
#include "GeometryExport.h"
#include "MaterialExport.h"
#include "SkinExport.h"

#include "Saver.h"

ModelExporter::ModelExporter(void)
{
	mTransformCount = 0;
	mMeshCount = 0;
	mMaterialCount = 0;
	mTextureCount = 0;
	mCameraCount = 0;
}

ModelExporter::~ModelExporter(void)
{
	mSceneTransforms.clear();
	mSceneMeshes.clear();
	mSceneMaterials.clear();
	mSceneTextures.clear();
}

void ModelExporter::Export(string folder, string fileName)
{
	mFolder = folder;
	mFileName = fileName;

	mSaver.SetFolder(mFolder);
	mSaver.CreateFile(mFileName + ".mod");
	
	MaterialExport material;
	material.Export();

	TransformExport transform;
	transform.Export();

	SkinExport skin;
	skin.Export();

	GeometryExport geometry;
	geometry.Export();

	CameraExport camera;
	camera.Export();
 

	WriteHeaders();
}

void ModelExporter::WriteHeaders()
{
	vector<TransformData*>::iterator transIter =  mSceneTransforms.begin();
	vector<MeshData*>::iterator meshIter =  mSceneMeshes.begin();
	vector<CameraData*>::iterator cameraIter =  mSceneCameras.begin();

	Saver headerSaver;
	headerSaver.SetFolder( mFolder );

	headerSaver.CreateFile(string("info_") + mFileName + ".h");
	headerSaver.writeTxt(string("#ifndef __" + string("info_") + mFileName + "__\n").c_str());
	headerSaver.writeTxt(string("#define __" + string("info_") + mFileName + "__\n").c_str());
	headerSaver.writeTxt("\n\n");

	while (transIter != mSceneTransforms.end())
	{
		TransformData * pTransformData = *transIter;

		static char def[256]={0};
		sprintf(def,"#define %s_TRANS_%s %d\n" , mFileName.c_str(), pTransformData->name.c_str() , pTransformData->index);
		headerSaver.writeTxt(def);
		transIter++;
	}

	headerSaver.writeTxt("\n");

	while (meshIter != mSceneMeshes.end())
	{
		MeshData * pMeshData = *meshIter;

		static char def[256]={0};
		sprintf(def,"#define %s_MESH_%s %d\n" , mFileName.c_str(), pMeshData->name.c_str() , pMeshData->index);
		headerSaver.writeTxt(def);
		meshIter++;
	}

	headerSaver.writeTxt("\n");

	while (cameraIter != mSceneCameras.end())
	{
		CameraData * pCameraData = *cameraIter;

		static char def[256]={0};
		sprintf(def,"#define %s_CAMERA_%s %d\n" , mFileName.c_str(), pCameraData->name.c_str() , pCameraData->index);
		headerSaver.writeTxt(def);
		cameraIter++;
	}


	headerSaver.writeTxt("\n\n");
	headerSaver.writeTxt(string("#endif //__" + string("info_") + mFileName + "__\n").c_str());
}

void ModelExporter::AddTransform(TransformData * pTransform)
{
	
}

void ModelExporter::AddMesh(MeshData * pMesh)
{

}
void ModelExporter::AddMaterial(MaterialData * pMaterial)
{

}

void ModelExporter::ResetTimeline()
{
	MStatus stat;
	stat = MGlobal::executeCommand(MString("currentTime -edit 0;"));
	err_code(stat);

}

bool ModelExporter::CheckParentType(MFn::Type type)
{
	if (type == MFn::kTransform || 
		type == MFn::kJoint ||
		type == MFn::kCamera)
	{
		return true;
	}

	return false;
}

bool ModelExporter::CheckChildType(MFn::Type type)
{
	if ( type == MFn::kTransform ||
		type == MFn::kLocator ||
		type == MFn::kMesh || 
		type == MFn::kJoint || 
		type == MFn::kCamera ||
		type == MFn::kLight )
	{
		return true;
	}

	return false;
}
