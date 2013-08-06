#ifndef __ResourceLoader__
#define __ResourceLoader__

#include "Config.h"
#include "PVRTexture.h"
#include "Node.h"
#include "Shaders/Shader.h"
#include "ModelData.h"
#include "LocatorData.h"
#include "BotLauncherData.h"
#include "CurvesData.h"
#include "WiresData.h"
#include "PlanesData.h"
#include "DoorsData.h"
#include "PhysicsData.h"
#include "QuadTree.h"
#include "Loader.h"
#include "OverlayFace.h"
#include "GraphData.h"

#include "TL/Map.h"

struct VertInd
{
public:
	NEWDEL_DECL
	VertInd()
	{
		pVertices = NULL;
		pIndices = NULL;
	}
	~VertInd()
	{
		delete pVertices;
		delete pIndices;
	}


	Vertices * pVertices;
	Indices * pIndices;
};

class SceneData
{
public:
	NEWDEL_DECL;

	ModelData * modelData;
	CurvesData * curvesData;
	WiresData * wiresData;
	CollisionMeshData * pCollisionMeshData;
	GraphMeshData * pGraphMeshData;
	DoorsData * doorsData;
	PhysicsData * physicsData;
	LocatorData * locatorData;
	StopCircleData * stopCircleData;
	BotLauncherData * botLauncherData;
};


class RefSceneData
{
public:
	NEWDEL_DECL;

	RefSceneData()
	{
		mGraphQuadTree = NULL;
		mNumOfGraphQuadTree = 0;
	}
	
	~RefSceneData()
	{
		if (mGraphQuadTree)
		{
			delete [] mGraphQuadTree;
		}
	}

	QuadTree mRenderQuadTree;
	QuadTree mCollisionQuadTree;
	QuadTree * mGraphQuadTree;
	QuadTree mBotLauncherQuadTree;
	int mNumOfGraphQuadTree;
/*
	ModelData * modelData;
	CurvesData * curvesData;
	WiresData * wiresData;
	CollisionMeshData * pCollisionMeshData;
	DoorsData * doorsData;
	PhysicsData * physicsData;
	LocatorData * locatorData;
	StopCircleData * stopCircleData;
	BotLauncherData * botLauncherData;
*/
};


class ResourceLoader
{
public:
	ResourceLoader();
	NEWDEL_DECL
	inline static ResourceLoader & GetResourceLoader()
	{
		return *GlobalHeap::GetResourceLoader();		
	}
	void ReInitDeviceResources();
	void ReInitDeviceResourcesWithShaders();
	void FreeDeviceResources();
	void FreeDeviceResourcesWithShaders();
	void Reset();

	SceneData * LoadScene(const char * sceneName);
	ModelData * LoadModel(const char * fileName, bool forceReload = false, bool addSceneManager = true);
	LocatorData * LoadLocators(const char * fileName);
	RefSceneData * LoadRefScene(const char * fileName);
	StopCircleData * LoadStopCircles(const char * fileName);
	BotLauncherData * LoadBotLaunchers(const char * fileName);
	CurvesData * LoadCurves(const char * fileName);
	WiresData * LoadWires(const char * fileName);
	CollisionMeshData * LoadCollisionMesh(const char * fileName);
	GraphMeshData * LoadGraphMesh(const char * fileName);
	DoorsData * LoadDoors(const char * fileName);
	PhysicsData * LoadPhyiscs(const char * fileName, ModelData * pModelData);

	bool ReadTextures(Loader & loader, ModelData * pModelData);
	bool ReadMaterials(Loader & loader, ModelData * pModelData);
	bool ReadTransforms(Loader & loader, ModelData * pModelData);
	bool ReadMeshes(Loader & loader, ModelData * pModelData, bool forceReload);
	bool ReadCameras(Loader & loader, ModelData * pModelData);

	OverlayFace * LoadOverlayFace(String fileName, bool gui);
	void RemoveOverlayFace(OverlayFace * pOverlayFace);

	Texture * LoadRawTexture565(String fileName);
	PVRTexture * LoadTexture(String fileName, bool gui);
	PVRTexture * LoadTextureAndGenerate(String fileName, bool gui);
	void ReInitTexture(PVRTexture * pTexture);
	void ReInitVerticesIndices(VertInd * pVerInd);
	
	
	
	void Destroy();

protected:
	Map mTexturesMap;	
	Map mVertIndMap;
	List mVertIndList;
	Map mLocatorsMap;
	Map mStopCirclesMap;
	List mBotLaunchersVec;
	Map mCurvesMap;
	Map mWiresMap;
	List mCollisionMeshVec;
	List mGraphMeshVec;
	Map mDoorsMap;
	Map mPhysicsMap;

	List mModelsVec;
	List mOverlayFaces;
};

#endif //__ResourceLoader__

