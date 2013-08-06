#pragma once

#include "Saver.h"
#include "TransformData.h"
#include "MeshData.h"
#include "MaterialData.h"
#include "TextureData.h"
#include "SkinExport.h"
#include "CameraExport.h"


class ModelExporter
{
public:
	ModelExporter(void);
	~ModelExporter(void);

	static ModelExporter & GetExporter()
	{
		static ModelExporter exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	void ResetTimeline();
	bool CheckParentType(MFn::Type type);
	bool CheckChildType(MFn::Type type);

	void WriteHeaders();

	Saver & GetSaver()
	{
		return mSaver;
	}

	string mFileName;
	string mFolder;

	void AddTransform(TransformData * pTransform);
	void AddMesh(MeshData * pMesh);
	void AddMaterial(MaterialData * pMaterial);
	

	vector<TransformData*> mSceneTransforms;
	map<string, int> mSceneTransformsTable;
	
	vector<MeshData*> mSceneMeshes;
	map<string, int> mSceneMeshTable;

	map<string,MeshSkinData> mMeshSkins;

	vector<MaterialData*> mSceneMaterials;
	map<string, int> mSceneMaterialsTable;

	vector<TextureData*> mSceneTextures;
	map<string, int> mSceneTexturesTable;

	vector<CameraData*> mSceneCameras;
	map<string, int> mSceneCameraTable;


	int mTransformCount;
	int mMeshCount;
	int mMaterialCount;
	int mTextureCount;
	int mCameraCount;

	string mSkeletonRoot;
	int mSkeletonRootIndex;



private:
	Saver mSaver;
};
