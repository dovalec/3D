#include "ResourceLoader.h"
#include "Shell.h"
#include "Mesh.h"
#include "SkinMesh.h"
#include "Loader.h"
#include "SceneManager.h"

#include "Shaders/ShadersManager.h"

NEWDEL_IMPL(ResourceLoader)
NEWDEL_IMPL(VertInd)
NEWDEL_IMPL(SceneData)
NEWDEL_IMPL(RefSceneData)

ResourceLoader::ResourceLoader()
{

}

Texture * ResourceLoader::LoadRawTexture565(String fileName)
{
	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();
	static char textureFullPathName[256]={0};	
	sprintf(textureFullPathName,"%s/%s", folder, fileName.GetPtr());
	LOGI("Loading texture: %s", textureFullPathName);
	Loader loader;
	if (!loader.OpenFile(textureFullPathName))
	{
		LOGE("Failed to open texture file: %s", textureFullPathName);
		STOP;
		return NULL;
	}
	Texture * pTexture = new Texture();
	pTexture->GetImage().Allocate(256*256*2);
	pTexture->GetImage().PutValuesDirect((GLubyte*)loader.GetReaderPtr());
	pTexture->GetImage().SetWidth(256);
	pTexture->GetImage().SetHeight(256);
	pTexture->GetImage().SetImageFormat(ImageFormatRGB_565);
	return pTexture;
}

PVRTexture * ResourceLoader::LoadTextureAndGenerate(String fileName, bool gui)
{
	PVRTexture * pTexture = ResourceLoader::LoadTexture(fileName, gui);
	GLuint genTex;

	EPVRTError err = PVRTTextureLoadFromPointer( pTexture->GetImage().GetValues(), &genTex);
	if (err != PVR_SUCCESS)
	{
		LOGE("Failed to recreate texture.");
	}
	else
	{
		LOGI("Recreated texture.");
		pTexture->SetTextureObject(genTex); 
	}

	return pTexture;
}

PVRTexture * ResourceLoader::LoadTexture(String fileName, bool gui)
{
	GLuint genTex = 0;

	PVRTexture * pTexture = (PVRTexture*)mTexturesMap.GetByKey( fileName.GetPtr() );
	if (pTexture)
		return pTexture;
	
	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();
	const char * system = pShell->GetSystem();

	static char textureFullPathName[256]={0};
	if (gui)
		sprintf(textureFullPathName,"%sui/%s", folder, fileName.GetPtr());
	else
		sprintf(textureFullPathName,"%s%s/%s", folder, system, fileName.GetPtr());

	LOGI("Loading texture: %s", textureFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(textureFullPathName))
	{
		LOGE("Failed to open texture file: %s", textureFullPathName);
		STOP;
		return NULL;
	}
/*
	EPVRTError err = PVRTTextureLoadFromPointer( loader.GetReaderPtr(), &genTex);
	if (err != PVR_SUCCESS)
	{
		LOGE("Failed to load texture: %s", fileName.GetPtr());
	}
	else
		LOGI("Scene success: %s", fileName.GetPtr());
*/

	PVRTexture * pPvrTexture = new PVRTexture();
	pPvrTexture->GetImage().PutValuesDirect((GLubyte*)loader.GetReaderPtr());


	mTexturesMap.Add( fileName.GetPtr() , pPvrTexture );
	//Dont free - it will free textures!!!
	//loader.FreeData();

	ReInitTexture(pPvrTexture);
    if (gui)
    {
        pPvrTexture->mTextureFilterMode = TextureFilterModeLinear;
    }
    
	return pPvrTexture;
}

bool ResourceLoader::ReadTextures(Loader & loader, ModelData * pModelData)
{
	int lastIndex = -1;
	int numOfTextures = loader.ReadInt(); //!!!!!!!!!
	//pModelData->mTextures.resize(numOfTextures);

	for (int n=0;n<numOfTextures;n++)
	{		
		int index = loader.ReadInt(); //!!!!!!!!!
		if ((lastIndex + 1) != index)
			return false;
		lastIndex = index;

		String pvrTextureFileName = loader.ReadString(); //!!!!!!!!!
		PVRTexture * pTexture = LoadTexture(pvrTextureFileName + ".pvr", false);

		pTexture->mImage.mWidth = loader.ReadInt(); //!!!!!!!!!
		pTexture->mImage.mHeight = loader.ReadInt(); //!!!!!!!!!
		int hasAlpha = loader.ReadInt(); //!!!!!!!!!
		if (hasAlpha)
			pTexture->mImage.mHasAlpha = true;
		else
			pTexture->mImage.mHasAlpha = false;


		pTexture->mURepeat = loader.ReadFloat(); //!!!!!!!!!
		pTexture->mVRepeat = loader.ReadFloat(); //!!!!!!!!!

		if (pTexture->mURepeat > 1.0f)
		{
			pTexture->mTextureWrapMode_T = TextureWrapModeRepeat;
		}
		if (pTexture->mVRepeat > 1.0f)
		{
			pTexture->mTextureWrapMode_S = TextureWrapModeRepeat;
		}

		pModelData->mTextures.Add(pTexture);
	}

	return true;
}

bool ResourceLoader::ReadMaterials(Loader & loader, ModelData * pModelData)
{
	int lastIndex = -1;
	int numOfMaterials = loader.ReadInt(); //!!!!!!!!!
	//pModelData->mMaterials.resize(numOfMaterials);

	for (int n=0;n<numOfMaterials;n++)
	{
		MaterialData * pMaterialData = new MaterialData();
		pMaterialData->index = loader.ReadInt(); //!!!!!!!!!
		if ((lastIndex + 1) != pMaterialData->index)
			return false;
		lastIndex = pMaterialData->index;
		pMaterialData->textureIndex = loader.ReadInt(); //!!!!!!!!!
		pMaterialData->hasAlpha = loader.ReadInt(); //!!!!!!!!!

		pModelData->mMaterials.Add(pMaterialData);
	}
	return true;
}

bool ResourceLoader::ReadTransforms(Loader & loader, ModelData * pModelData)
{	
	int lastIndex = -1;
	int numOfTransforms = loader.ReadInt(); //!!!!!!!!!
	//pModelData->mTransforms.resize(numOfTransforms);

	
	for (int n=0;n<numOfTransforms;n++)
	{
		Node * pNode = new Node();
		
		int index = loader.ReadInt(); //!!!!!!!!!
		if ((lastIndex + 1) != index)
			return false;
		lastIndex = index;

		pNode->mParentIndex = loader.ReadInt(); //!!!!!!!!!
		pNode->SetName( loader.ReadString() ); //!!!!!!!!!
		
		float tx = loader.ReadFloat(); //!!!!!!!!!
		float ty = loader.ReadFloat(); //!!!!!!!!!
		float tz = loader.ReadFloat(); //!!!!!!!!!

		float px = loader.ReadFloat(); //!!!!!!!!!
		float py = loader.ReadFloat(); //!!!!!!!!!
		float pz = loader.ReadFloat(); //!!!!!!!!!
		
		float qx = loader.ReadFloat(); //!!!!!!!!!
		float qy = loader.ReadFloat(); //!!!!!!!!!
		float qz = loader.ReadFloat(); //!!!!!!!!!
		float qw = loader.ReadFloat(); //!!!!!!!!!

		float sx = loader.ReadFloat(); //!!!!!!!!!
		float sy = loader.ReadFloat(); //!!!!!!!!!
		float sz = loader.ReadFloat(); //!!!!!!!!!
		
		pNode->SetTranslation(tx, ty, tz);
		pNode->SetPivot(px, py, pz);
		pNode->SetRotation(qx, qy, qz, qw);
		pNode->SetScale(sx, sy, sz);
		
		pModelData->mTransforms.Add(pNode);

		/*
		static char txt[128]={0};
		sprintf(txt,"%s: %f, %f, %f, %f\n",pNode->GetName().GetPtr(), quat.x, quat.y, quat.z , quat.w);
		OutputDebugString(txt);
		*/

	}

	ListIterator transIter( pModelData->mTransforms );
	while (Node * pNode = (Node*)transIter.GetPtr() )
	{
		if (pNode->mParentIndex == -1 )
		{
			pModelData->GetRoot()->AddChild(pNode);
		}
		else
		{
			Node * pParentNode = (Node*)pModelData->mTransforms.GetArray(pNode->mParentIndex);
			pParentNode->AddChild(pNode);			
		}		
	}

	return true;
}
bool ResourceLoader::ReadMeshes(Loader & loader, ModelData * pModelData, bool forceReload)
{
	int lastIndex = -1;
	int numOfMeshes = loader.ReadInt(); //!!!!!!!!!
	for (int n=0;n<numOfMeshes;n++)
	{
		int index = loader.ReadInt(); //!!!!!!!!!
		int layer = loader.ReadInt(); //!!!!!!!!!
		float radius = loader.ReadFloat(); //!!!!!!!!!
		int shaderType = loader.ReadInt(); //!!!!!!!!!
		int alphaMode = loader.ReadInt(); //!!!!!!!!!
		int displayMode = loader.ReadInt(); //!!!!!!!!!

		int use_depth = loader.ReadInt(); //!!!!!!!!!
		int use_back_face_culling = loader.ReadInt(); //!!!!!!!!!
		int use_occlusion_culling = loader.ReadInt(); //!!!!!!!!!

		if ((lastIndex + 1) != index)
			return false;
		lastIndex = index;

		int transformIndex = loader.ReadInt(); //!!!!!!!!!
		Node * pParentNode = (Node*)pModelData->mTransforms.GetArray(transformIndex);

		int materialIndex = loader.ReadInt(); //!!!!!!!!!
			
		MaterialData * pMaterialData = (MaterialData*)pModelData->mMaterials.GetArray(materialIndex);
		int textureIndex = pMaterialData->textureIndex;
	
		ShadersManager &  shaderManager = ShadersManager::GetShadersManager();

		int verticesCount = loader.ReadInt(); //!!!!!!!!!
		int useSkin = loader.ReadInt(); //!!!!!!!!!
		
		Mesh * pMesh = NULL;
		SkinMesh * pSkinMesh = NULL;

		if (useSkin)
		{
			pSkinMesh = new SkinMesh();
			pMesh = (Mesh*)pSkinMesh;
			//~~~~~~ Overwrite exporter shader type:

			//shaderType = GLSL_SHADER_SKIN_NO_LIGHT;

			int skeletonRoot = loader.ReadInt(); //!!!!!!!!!
			Node * pSkeleton = (Node*)pModelData->mTransforms.GetArray(skeletonRoot);
			//pModelData->GetRoot()->RemoveChild(pSkeleton);
			//pSkinMesh->SetSkeleton(pSkeleton);

		}
		else
		{
			pMesh = new Mesh();
		}
	
		pMesh->SetLayer(layer);
		pMesh->SetRadius(radius);
		pMesh->SetDisplayMode( (Mesh::DisplayMode) displayMode);

		pMesh->SetUseDepth( (bool)use_depth );
		pMesh->SetUseBackFaceCulling( (bool)use_back_face_culling );
		//pMesh->SetUseOcclusionCulling(  (bool)use_occlusion_culling );

		pParentNode->AddChild(pMesh);
		pMesh->SetModelData(pModelData);
		
		Appearance & appearance = pMesh->GetAppearance();
		if (textureIndex != -1) {
			Texture * pTexture = (Texture*)pModelData->mTextures.GetArray(textureIndex);
			appearance.SetTexture(pTexture);
		}
		appearance.SetShader(shaderManager.GetShader(shaderType));
		/*if (pMaterialData->hasAlpha)
		{
			appearance.mCompositingMode.mBlendingMode = BlendingModeAlpha;
		}
		*/

		//Replace:Transparency:Additive:Modulate:ModulateX2		
		appearance.mCompositingMode.mBlendingMode = (BlendingMode)alphaMode;
		

		int verticesDataSize = 0;

		Vertices * pVertices = NULL;
		VerticesSkin * pVerticesSkin = NULL;
		Indices * pIndices = NULL;

		static char key[128]={0};
		sprintf(key,"%s_%d",pModelData->GetFileName().GetPtr(),n);

		bool createVertInd = true; 
		VertInd * vi = (VertInd*)mVertIndMap.GetByKey( key );

		if  ( forceReload == true || vi == NULL )
		{
			pIndices = new Indices();
			if (useSkin)
			{
				pVerticesSkin = new VerticesSkin();
				pVertices = (Vertices*)pVerticesSkin;
				verticesDataSize = sizeof(VertexSkin) * verticesCount;
				pVertices->Allocate(sizeof(VertexSkin), verticesCount);
				pVertices->PutValues((GLvoid*)loader.GetReaderPtr());
				loader.Progress(verticesDataSize);
	
			}
			else
			{
				pVertices = new Vertices();
				verticesDataSize = sizeof(Vertex) * verticesCount;
				pVertices->Allocate(sizeof(Vertex), verticesCount);
				pVertices->PutValues((GLvoid*)loader.GetReaderPtr());
				loader.Progress(verticesDataSize);
			}
			vi = new VertInd();
			vi->pVertices = pVertices;
			vi->pIndices = pIndices;
			
			if (!forceReload)
				mVertIndMap.Add( key , vi);

			mVertIndList.Add( vi);

		}
		else
		{
			createVertInd = false;
			if (useSkin)
			{	
				verticesDataSize = sizeof(VertexSkin) * verticesCount;
				loader.Progress(verticesDataSize);

				pVertices = vi->pVertices;
				pVerticesSkin = (VerticesSkin*)pVertices;
			}
			else
			{			
				verticesDataSize = sizeof(Vertex) * verticesCount;
				loader.Progress(verticesDataSize);

				pVertices = vi->pVertices;
			}
			pIndices = vi->pIndices;
		}
		
		
		pIndices->mSubMeshCount = loader.ReadInt(); //!!!!!!!!!
		if (pIndices->mSubMeshCount >= MAX_SUB_MESH)
		{
			LOGE("Too many sub meshes %d !!!", pIndices->mSubMeshCount);
			STOP;
			return false;
		}

		int totalIndices = 0;
		for (int subMeshIndex = 0; subMeshIndex<pIndices->mSubMeshCount ; subMeshIndex++)
		{
			pIndices->mSubMeshSizes[subMeshIndex] = loader.ReadInt(); //!!!!!!!!!!!!!!!
			totalIndices+=pIndices->mSubMeshSizes[subMeshIndex];
		}

		if (createVertInd)
			pIndices->Allocate(totalIndices);

		int place = 0;

		for (int subMeshIndex = 0; subMeshIndex<pIndices->mSubMeshCount ; subMeshIndex++)
		{
			int indicesCount = pIndices->mSubMeshSizes[subMeshIndex];
			int indcDataSize = sizeof(unsigned short) * indicesCount;

			//this is good for skin and regular mesh
			if (createVertInd)
				pIndices->PutValuesPartial((GLushort*)loader.GetReaderPtr(), place, indcDataSize);
			loader.Progress(indcDataSize);
			if (indicesCount%2) //eii zugi
				loader.Progress(2);

			place += indcDataSize;
		}

		if (createVertInd)
		{
			ReInitVerticesIndices(vi);
		}
		
		pIndices->mTriangleMode = TrianglesModeTrianglesStrip;
		pMesh->SetVertices(pVertices);
		pMesh->SetIndices(pIndices);

		if (useSkin)
		{
			int againSubMashCount = loader.ReadInt(); //!!!!!!!!! just for checking stream ...
			if (againSubMashCount != pIndices->mSubMeshCount)
			{
				LOGE( "Error loading skined mesh - againSubMashCount != pIndices->mSubMeshCount");
				STOP;
				return false;
			}

			pSkinMesh->AllocateSkinTransforms();
			for (int subMeshIndex = 0; subMeshIndex<pIndices->mSubMeshCount ; subMeshIndex++)
			{
				SkinTransforms * pSkinTransForm = pSkinMesh->GetSkinTransform(subMeshIndex);

				int numTransforms = loader.ReadInt(); //!!!!!!!!!
				pSkinTransForm->mNumTransforms = numTransforms;

				for (int transCount = 0 ; transCount < numTransforms; transCount++)
				{
					int transformIndex = loader.ReadInt(); //!!!!!!!!!
					pSkinTransForm->mTransformIds[transCount] = transformIndex;
					Node * pBone = (Node*)pModelData->mTransforms.GetArray(transformIndex);
					pBone->UpdateInverseWorldTransform();
				}
			}
		}

		pModelData->mMeshes.Add(pMesh);
	}

	return true;
}
bool ResourceLoader::ReadCameras(Loader & loader, ModelData * pModelData)
{
	int lastIndex = -1;
	int numOfCameras = loader.ReadInt(); //!!!!!!!!!
	for (int n=0;n<numOfCameras;n++)
	{
		int index = loader.ReadInt(); //!!!!!!!!!
		if ((lastIndex + 1) != index)
			return false;
		lastIndex = index;
		int transformIndex = loader.ReadInt(); //!!!!!!!!!
		Node * pParentNode = (Node*)pModelData->mTransforms.GetArray(transformIndex);
		String name = loader.ReadString(); //!!!!!!!!!
		float aspectRatio = loader.ReadFloat(); //!!!!!!!!!
		float nearClippingPlane = loader.ReadFloat(); //!!!!!!!!!
		float farClippingPlane = loader.ReadFloat(); //!!!!!!!!!
		float horizontalFieldOfView = loader.ReadFloat(); //!!!!!!!!!
		float verticalFieldOfView = loader.ReadFloat(); //!!!!!!!!
		Camera * pCamera = new Camera();
		pCamera->SetAspectRatio(aspectRatio);
		pCamera->SetNearClippingPlane(nearClippingPlane);
		pCamera->SetFarClippingPlane(farClippingPlane);
		pCamera->SetHorizontalFieldOfView(horizontalFieldOfView);
		pCamera->SetVerticalFieldOfView(verticalFieldOfView);
		pParentNode->AddChild(pCamera);
		pModelData->mCameras.Add(pCamera);
	}
	return true;
}

SceneData * ResourceLoader::LoadScene(const char * sceneName)
{
	SceneData * pSceneData = new SceneData();
	String scnName(sceneName);

	pSceneData->modelData = LoadModel( (scnName + ".mod").GetPtr() );
	pSceneData->locatorData = LoadLocators( (scnName + ".loc").GetPtr() );
	pSceneData->stopCircleData = LoadStopCircles( (scnName + ".loc").GetPtr() );
	pSceneData->botLauncherData = LoadBotLaunchers( (scnName + ".blp").GetPtr() );
	pSceneData->curvesData = LoadCurves( (scnName + ".crv").GetPtr() );
	pSceneData->wiresData = LoadWires( (scnName + ".wir").GetPtr() );
	pSceneData->pCollisionMeshData = LoadCollisionMesh( (scnName + ".clm").GetPtr() );
	pSceneData->pGraphMeshData = LoadGraphMesh( (scnName + ".grp").GetPtr() );
	pSceneData->doorsData = LoadDoors( (scnName + ".dor").GetPtr() );
	pSceneData->physicsData = LoadPhyiscs( (scnName + ".phy").GetPtr(), pSceneData->modelData );

	return pSceneData;
}

CurvesData * ResourceLoader::LoadCurves(const char * fileName)
{
	CurvesData * pCurvesData  = (CurvesData*)mCurvesMap.GetByKey(fileName);
	if (pCurvesData)
		return pCurvesData;

	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char curvesFullPathName[256]={0};
	sprintf(curvesFullPathName,"%s%s", folder, fileName);

	LOGI("Loading curves: %s", curvesFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(curvesFullPathName))
	{
		return NULL;
	}

	pCurvesData = new CurvesData();

	int numOfCurves = loader.ReadInt();  //!!!!!!!!!
	pCurvesData->AllocateCurves(numOfCurves);

	for (int n=0;n<numOfCurves;n++)
	{
		Curve * pCurve = pCurvesData->GetCurve(n);
		pCurve->name = loader.ReadString(); //!!!!!!!!!
		pCurve->numOfPoints = loader.ReadInt(); //!!!!!!!!!
		pCurve->curvePoints = new CurvePoint[pCurve->numOfPoints];
		int curvePointsDataSize = pCurve->numOfPoints*sizeof(CurvePoint);
		memcpy(pCurve->curvePoints, loader.GetReaderPtr(), curvePointsDataSize );
		loader.Progress(curvePointsDataSize);
		pCurvesData->mCurvesTable.Add( pCurve->name.GetPtr(), (void*)n);
	}

	mCurvesMap.Add(fileName , pCurvesData);

	loader.FreeData();
	return pCurvesData;
}


WiresData * ResourceLoader::LoadWires(const char * fileName)
{

	WiresData * pWiresData = (WiresData*)mWiresMap.GetByKey(fileName);
	if (pWiresData)
		return pWiresData;

	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char wiresFullPathName[256]={0};
	sprintf(wiresFullPathName,"%s%s", folder, fileName);

	LOGI("Loading wires: %s", wiresFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(wiresFullPathName))
	{
		return NULL;
	}

	pWiresData = new WiresData();

	int numOfWires = loader.ReadInt();  //!!!!!!!!!
	pWiresData->AllocateWires(numOfWires);

	for (int n=0;n<numOfWires;n++)
	{
		Wire * pWire = pWiresData->GetWire(n);
		pWire->name = loader.ReadString(); //!!!!!!!!!
		pWire->numOfPoints = loader.ReadInt(); //!!!!!!!!!
		pWire->wirePoints = new WirePoint[pWire->numOfPoints];
		int wirePointsDataSize = pWire->numOfPoints*sizeof(WirePoint);
		memcpy(pWire->wirePoints, loader.GetReaderPtr(), wirePointsDataSize );
		loader.Progress(wirePointsDataSize);
		pWiresData->mWiresTable.Add(pWire->name.GetPtr(), (void*)n);
	}

	mWiresMap.Add( fileName,  pWiresData);

	loader.FreeData();
	return pWiresData;
}


CollisionMeshData * ResourceLoader::LoadCollisionMesh(const char * fileName)
{
	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char collisionMeshFullPathName[256]={0};
	sprintf(collisionMeshFullPathName,"%s%s", folder, fileName);

	LOGI("Loading collisionMesh: %s", collisionMeshFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(collisionMeshFullPathName))
	{
		return NULL;
	}


	CollisionMeshData * pCollisionMeshData = new CollisionMeshData();
	int numOfCollisionMesh = loader.ReadInt();  //!!!!!!!!!!

	pCollisionMeshData->AllocateCollisionMesh(numOfCollisionMesh);

	for (int c=0;c<numOfCollisionMesh;c++)
	{
		String collisionName = loader.ReadString(); //!!!!!!!!!!
		float radius  = loader.ReadFloat(); //!!!!!!!!!!!
		PlanesData * pPlaneData = pCollisionMeshData->GetPlanesData(c);
		pPlaneData->SetRadius(radius);

		int numOfPlanes = loader.ReadInt();  //!!!!!!!!!!
		pPlaneData->AllocatePlanes(numOfPlanes);

		int planesDataSize = numOfPlanes*sizeof(Plane);
		memcpy(pPlaneData->GetPlane(0), loader.GetReaderPtr(), planesDataSize );
		loader.Progress(planesDataSize);

		pCollisionMeshData->mCollisionMeshTable.Add(collisionName.GetPtr(), (int*)c);
		
	}
	
	mCollisionMeshVec.Add( pCollisionMeshData );

	
	loader.FreeData();
	return pCollisionMeshData;
}


GraphMeshData * ResourceLoader::LoadGraphMesh(const char * fileName)
{
    Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char grpahMeshFullPathName[256]={0};
	sprintf(grpahMeshFullPathName,"%s%s", folder, fileName);

	LOGI("Loading graphsMesh: %s", grpahMeshFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(grpahMeshFullPathName))
	{
		return NULL;
	}


	GraphMeshData * pGraphMeshData = new GraphMeshData();
	int numOfGraphData = loader.ReadInt();  //!!!!!!!!!!

	pGraphMeshData->AllocateGraphMesh(numOfGraphData);

	for (int graphDataIndex=0; graphDataIndex < numOfGraphData; graphDataIndex++)
	{
		GraphData * pGraphData = pGraphMeshData->GetGraphData(graphDataIndex);
		String graphName = loader.ReadString(); //!!!!!!!!!!
		
		int numOfGraphNodes = loader.ReadInt();  //!!!!!!!!!!
		pGraphData->AllocateGraphNodes(numOfGraphNodes);

		for (int graphNodeIndex=0; graphNodeIndex < numOfGraphNodes ; graphNodeIndex++ )
		{
			GraphNode * pGraphNode = pGraphData->GetGraphNode(graphNodeIndex);
			pGraphNode->mID = loader.ReadUInt();  //!!!!!!!!!!
			pGraphNode->x = loader.ReadFloat();  //!!!!!!!!!!
			pGraphNode->y = loader.ReadFloat();  //!!!!!!!!!!
			pGraphNode->z = loader.ReadFloat();  //!!!!!!!!!!
			int numOfEdges = loader.ReadUInt( );  //!!!!!!!!!!

			pGraphNode->AllocateEdges(numOfEdges);
	
			
			for (int edgeCount = 0 ; edgeCount < numOfEdges ; edgeCount++)
			{
				int edgeIndex = loader.ReadUInt( );
				pGraphNode->GetEdges()[edgeCount] = pGraphData->GetGraphNode(edgeIndex);
			}
			//int edgesDataSize = numOfEdges*sizeof(int);
			//memcpy(pGraphNode->GetEdges(), loader.GetReaderPtr(), edgesDataSize );
			//loader.Progress(edgesDataSize);
		}

		pGraphMeshData->mGraphDataTable.Add(graphName.GetPtr(), (int*)graphDataIndex);
		
	}
	
	mGraphMeshVec.Add( pGraphMeshData );

	
	loader.FreeData();
	return pGraphMeshData;
}


DoorsData * ResourceLoader::LoadDoors(const char * fileName)
{
	DoorsData * pDoorsData  = (DoorsData*)mDoorsMap.FindByKey(fileName);
	if (pDoorsData)
		return pDoorsData;

	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char doorsFullPathName[256]={0};
	sprintf(doorsFullPathName,"%s%s", folder, fileName);

	LOGI("Loading doors: %s", doorsFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(doorsFullPathName))
	{
		return NULL;
	}


	pDoorsData = new DoorsData();

	int numOfDoors = loader.ReadInt();  //!!!!!!!!!
	pDoorsData->AllocateDoors(numOfDoors);

	for (int d = 0 ; d<numOfDoors ; d++)
	{

		Door * pDoor = pDoorsData->GetDoor(d);
		String doorName = loader.ReadString();
		pDoorsData->mDoorsTable.Add( doorName.GetPtr(), (void*)d);

		int numDoorEdges = loader.ReadInt();
		pDoor->AllocateDoorEdges(numDoorEdges);
		
		int edgesDataSize = numDoorEdges*sizeof(DoorEdge);
		memcpy(pDoor->GetDoorEdge(0), loader.GetReaderPtr(), edgesDataSize );
		loader.Progress(edgesDataSize);
		
	}

	mDoorsMap.Add(fileName, pDoorsData);

	loader.FreeData();
	return pDoorsData;
}

PhysicsData * ResourceLoader::LoadPhyiscs(const char * fileName, ModelData * pModelData)
{
	PhysicsData * pPhysicsData  = (PhysicsData*)mPhysicsMap.FindByKey(fileName);
	if (pPhysicsData)
		return pPhysicsData;

	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char phyFullPathName[256]={0};
	sprintf(phyFullPathName,"%s%s", folder, fileName);

	LOGI("Loading physics: %s", phyFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(phyFullPathName))
	{
		return NULL;
	}


	pPhysicsData = new PhysicsData();

	int numOfPhysics = loader.ReadInt();  //!!!!!!!!!
	pPhysicsData->AllocatePhysics(numOfPhysics);

	for (int d = 0 ; d<numOfPhysics ; d++)
	{
		PhysicsProp * pPhysicsProp = pPhysicsData->GetPhysicsProp(d);
		pPhysicsProp->index = loader.ReadInt();
		pPhysicsProp->node = (Node*)pModelData->mTransforms.GetArray( pPhysicsProp->index );
		pPhysicsProp->move = loader.ReadInt();
		pPhysicsProp->shape = loader.ReadInt();
		pPhysicsProp->deactivationTime = loader.ReadFloat();
		pPhysicsProp->friction = loader.ReadFloat();
		pPhysicsProp->restitution = loader.ReadFloat();
		pPhysicsProp->mass = loader.ReadFloat();
		pPhysicsProp->radius = loader.ReadFloat();
		pPhysicsProp->width = loader.ReadFloat();
		pPhysicsProp->height = loader.ReadFloat();
		pPhysicsProp->depth = loader.ReadFloat();
			
	}

	mPhysicsMap.Add(fileName, pPhysicsData);

	loader.FreeData();
	return pPhysicsData;
}


BotLauncherData * ResourceLoader::LoadBotLaunchers(const char * fileName)
{
	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char launcherFullPathName[256]={0};
	sprintf(launcherFullPathName,"%s%s", folder, fileName);

	LOGI("Loading bot launchers: %s", launcherFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(launcherFullPathName))
	{
		return NULL;
	}

	BotLauncherData * pBotLauncherData = new BotLauncherData();

	int numOfBotLaunchers = loader.ReadInt(); //!!!!!!!!!
	pBotLauncherData->AllocateBotLaunchers(numOfBotLaunchers);
	for (int n=0;n<numOfBotLaunchers;n++)
	{
		BotLauncher & launcher = pBotLauncherData->mBotLaunchers[n];
		String launcherName = loader.ReadString(); //!!!!!!!!!
		float x = loader.ReadFloat(); //!!!!!!!!!
		float y = loader.ReadFloat(); //!!!!!!!!!
		float z = loader.ReadFloat(); //!!!!!!!!!
		float radius = loader.ReadFloat(); //!!!!!!!!!
		launcher.SetPosition(x,y,z);
		launcher.SetRadius(radius);
		launcher.SetName(launcherName);
		pBotLauncherData->mBotLaunchersTable.Add(launcherName.GetPtr(), (void*)n);

	}

	loader.FreeData();
	mBotLaunchersVec.Add(pBotLauncherData);
	return pBotLauncherData;

}

LocatorData * ResourceLoader::LoadLocators(const char * fileName)
{
	LocatorData * pLocatorData = (LocatorData*)mLocatorsMap.GetByKey(fileName);
	if (pLocatorData)
		return pLocatorData;

	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char locatorFullPathName[256]={0};
	sprintf(locatorFullPathName,"%s%s", folder, fileName);

	LOGI("Loading locators: %s", locatorFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(locatorFullPathName))
	{
		return NULL;
	}

	pLocatorData = new LocatorData();

	int numOfLocators = loader.ReadInt(); //!!!!!!!!!
	pLocatorData->AllocateLocators(numOfLocators);
	for (int n=0;n<numOfLocators;n++)
	{
		Locator & locator = pLocatorData->mLocators[n];
		String locatorName = loader.ReadString(); //!!!!!!!!!
		int hasRef = loader.ReadInt(); //!!!!!!!!!
		locator.hasRef = false;
		if (hasRef)
		{
			locator.hasRef = true;
			locator.refName = loader.ReadString(); //!!!!!!!!!
		}
		locator.x = loader.ReadFloat(); //!!!!!!!!!
		locator.y = loader.ReadFloat(); //!!!!!!!!!
		locator.z = loader.ReadFloat(); //!!!!!!!!!

		locator.qx = loader.ReadFloat(); //!!!!!!!!!
		locator.qy = loader.ReadFloat(); //!!!!!!!!!
		locator.qz = loader.ReadFloat(); //!!!!!!!!!
		locator.qw = loader.ReadFloat(); //!!!!!!!!!

		locator.sx = loader.ReadFloat(); //!!!!!!!!!
		locator.sy = loader.ReadFloat(); //!!!!!!!!!
		locator.sz = loader.ReadFloat(); //!!!!!!!!!

		locator.radius = loader.ReadFloat(); //!!!!!!!!!
		
		PVRTMATRIX transMtx;
		PVRTMatrixTranslation(transMtx, locator.x, locator.y, locator.z);

		PVRTQUATERNION quat;
		PVRTMATRIX rotMtx;
		quat.x = locator.qx;
		quat.y = locator.qy;
		quat.z = locator.qz;
		quat.w = locator.qw;
		PVRTMatrixRotationQuaternion(rotMtx, quat);

		PVRTMATRIX scaleMtx;
		PVRTMatrixScaling(scaleMtx, locator.sx, locator.sy , locator.sz);

		PVRTMATRIX tmpMtx;
		PVRTMatrixMultiply(tmpMtx, scaleMtx, rotMtx);
		PVRTMatrixMultiply(locator.matrix, tmpMtx, transMtx);

		pLocatorData->mLocatorsTable.Add(locatorName.GetPtr(), (void*)n);

	}

	loader.FreeData();
	mLocatorsMap.Add(fileName, pLocatorData);
	return pLocatorData;

}

RefSceneData * ResourceLoader::LoadRefScene(const char * fileName)
{
	
	RefSceneData * pScene = new RefSceneData();

	float sceneRadius = sqrtf( MM( 500*GLOBAL_SCALE) + MM( 500*GLOBAL_SCALE) );
	pScene->mRenderQuadTree.Allocate(4 ,sceneRadius);
	pScene->mCollisionQuadTree.Allocate(4 ,sceneRadius);
	pScene->mBotLauncherQuadTree.Allocate(4 , sceneRadius);

    Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();
    
	static char refSceneFullPathName[256]={0};
	sprintf(refSceneFullPathName,"%s%s", folder, fileName);

	LOGI("Loading ref scene: %s", refSceneFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(refSceneFullPathName))
	{
		return NULL;
	}

	
	int numOfRefs = loader.ReadInt(); //!!!!!!!!!	
	for (int refCount=0;refCount<numOfRefs;refCount++)
	{
		String refName = loader.ReadString(); //!!!!!!!!!
	
		float x = loader.ReadFloat(); //!!!!!!!!!
		float y = loader.ReadFloat(); //!!!!!!!!!
		float z = loader.ReadFloat(); //!!!!!!!!!

		float qx = loader.ReadFloat(); //!!!!!!!!!
		float qy = loader.ReadFloat(); //!!!!!!!!!
		float qz = loader.ReadFloat(); //!!!!!!!!!
		float qw = loader.ReadFloat(); //!!!!!!!!!

		float sx = loader.ReadFloat(); //!!!!!!!!!
		float sy = loader.ReadFloat(); //!!!!!!!!!
		float sz = loader.ReadFloat(); //!!!!!!!!!

		float radius = loader.ReadFloat(); //!!!!!!!!!
		
		ModelData * pModel = LoadModel((refName + ".mod").GetPtr(), false, false);
		if (pModel == NULL)
		{
			LOGE("Cant load model: %s", refName.GetPtr());
			STOP;
		}

		Node * pModelRoot = pModel->GetRoot();
		pModelRoot->SetRadius(radius);

		pModelRoot->SetTranslation(x,y,z);
		pModelRoot->SetRotation(qx,qy,qz,qw);
		pModelRoot->SetScale(sx,sy,sz);
		pModelRoot->Update();

		pScene->mRenderQuadTree.Add( pModelRoot, x , z , radius);

		PVRTMat4 rotMtx;
		PVRTQUATERNION rotQuat={qx, qy, qz, qw};
		PVRTMatrixRotationQuaternion(rotMtx, rotQuat);
		
		//////////////////////////////////////////////////////////////////////////////////////////////////

		CollisionMeshData  * pCollData = LoadCollisionMesh((refName + ".clm").GetPtr());
		if (pCollData)
		{
			int numOfCollisionMesh = pCollData->GetNumOfCollisionMesh();
			for (int colMeshCount=0;colMeshCount<numOfCollisionMesh;colMeshCount++)
			{
				PlanesData * pPlaneData = pCollData->GetPlanesData(colMeshCount);
				pPlaneData->Transform(pModelRoot->GetWorldMtx(), rotMtx);	

				PVRTVec3 worldsPos = pModelRoot->GetWorldTranslation();
				pScene->mCollisionQuadTree.Add( pPlaneData, worldsPos.x, worldsPos.z, sx * pPlaneData->GetRadius() );
			}
		}
		
		//////////////////////////////////////////////////////////////////////////////////////////////////

		BotLauncherData * pBotLauncherData = LoadBotLaunchers((refName + ".blp").GetPtr());
		if (pBotLauncherData)
		{
			int numBotLaunchers = pBotLauncherData->GetNumOfBotLaunchers();
			for (int botCount=0;botCount<numBotLaunchers;botCount++)
			{
				BotLauncher * pBotLauncher = pBotLauncherData->GetBotLauncher(botCount);
				pBotLauncher->Transform(pModelRoot->GetWorldMtx());

				PVRTVec3 worldsPos = pModelRoot->GetWorldTranslation();
				pScene->mBotLauncherQuadTree.Add( pBotLauncher, worldsPos.x, worldsPos.z, sx * pBotLauncher->GetRadius() );
			}
		}

	}

	loader.FreeData();
	


	//////////////////////////////////////////////////////////////////////////////////////////////////
	String grpFile(fileName);
	grpFile.InsertZeroAtChar('.');

	GraphMeshData  * pGraphMeshData = LoadGraphMesh( (grpFile + ".grp").GetPtr() );
	if (pGraphMeshData)
	{
		int numOfGraphData = pGraphMeshData->GetNumOfGraphData();
		pScene->mGraphQuadTree = new QuadTree[numOfGraphData];
		pScene->mNumOfGraphQuadTree = numOfGraphData;

		for (int graphDataCount=0; graphDataCount < numOfGraphData; graphDataCount++)
		{
			pScene->mGraphQuadTree[graphDataCount].Allocate(4 , sceneRadius );

			GraphData * pGraphData = pGraphMeshData->GetGraphData(graphDataCount);		
			int numOfGraphNodes = pGraphData->GetNumOfGraphNodes();
			for (int nodeCount = 0 ; nodeCount < numOfGraphNodes ; nodeCount++)
			{
				GraphNode * pGraphNode = pGraphData->GetGraphNode(nodeCount);
				pScene->mGraphQuadTree[graphDataCount].Add( pGraphNode, pGraphNode->x, pGraphNode->z, 0.0f );
			}				
		}
	}

	return pScene;

}


StopCircleData * ResourceLoader::LoadStopCircles(const char * fileName)
{
	StopCircleData * pStopCircleData = (StopCircleData*)mStopCirclesMap.GetByKey(fileName);
	if (pStopCircleData)
		return pStopCircleData;

	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char stopCircleFullPathName[256]={0};
	sprintf(stopCircleFullPathName,"%s%s", folder, fileName);

	LOGI("Loading stop circles: %s", stopCircleFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(stopCircleFullPathName))
	{
		return NULL;
	}

	pStopCircleData = new StopCircleData();

	int numOfStopCircles = loader.ReadInt(); //!!!!!!!!!
	pStopCircleData->AllocateStopCircles(numOfStopCircles);
	for (int n=0;n<numOfStopCircles;n++)
	{
		StopCircle & stopCircle = pStopCircleData->mStopCircles[n];
		
		stopCircle.x = loader.ReadFloat(); //!!!!!!!!!
		stopCircle.y = loader.ReadFloat(); //!!!!!!!!!
		stopCircle.z = loader.ReadFloat(); //!!!!!!!!!		
		stopCircle.radius = loader.ReadFloat(); //!!!!!!!!!		
	}

	loader.FreeData();
	mStopCirclesMap.Add(fileName, pStopCircleData);
	return pStopCircleData;

}


OverlayFace * ResourceLoader::LoadOverlayFace(String fileName, bool gui)
{
	ShadersManager & shadersManager = ShadersManager::GetShadersManager();

	OverlayFace * pFace = new OverlayFace();
	PVRTexture * pTexture = LoadTexture(fileName, gui);	
	if (pTexture == NULL)
	{
		STOP;
	}

	pFace->SetTexture(pTexture);
	pFace->SetShader( shadersManager.GetShader(GLSL_SHADER_NO_LIGHT) );

	Vertices * pVertices = pFace->GetVertices();
	Indices * pIndices = pFace->GetIndices();

	pVertices->ReInitDeviceResources();
	pIndices->ReInitDeviceResources();	

	mOverlayFaces.Add(pFace);
	return pFace;
}

void ResourceLoader::RemoveOverlayFace(OverlayFace * pOverlayFace)
{
	mOverlayFaces.Remove(pOverlayFace);
}

void ResourceLoader::FreeDeviceResourcesWithShaders()
{
	ShadersManager &  shaderManager = ShadersManager::GetShadersManager();
	shaderManager.FreeDeviceResources();

	FreeDeviceResources();
}
void ResourceLoader::FreeDeviceResources()
{
	MapIterator textureIter(mTexturesMap);
	while (PVRTexture * pTexture = (PVRTexture*)textureIter.GetPtr())
	{
		pTexture->FreeDeviceResources();
	}
	
	ListIterator vertIndIter(mVertIndList);
	while (VertInd * vi = (VertInd*)vertIndIter.GetPtr())
	{
		vi->pVertices->FreeDeviceResources();
		vi->pIndices->FreeDeviceResources();		
	}
	

	ListIterator overlayIter(mOverlayFaces);

	while ( OverlayFace * pFace = (OverlayFace*)overlayIter.GetPtr() )
	{		
		Vertices * pVertices = pFace->GetVertices();
		Indices * pIndices = pFace->GetIndices();
		
		pVertices->FreeDeviceResources();
		pIndices->FreeDeviceResources();			
	}

	mTexturesMap.RemoveAll();
	mVertIndMap.RemoveAll();
	mVertIndList.RemoveAll();
	mOverlayFaces.RemoveAll();
}

void ResourceLoader::ReInitDeviceResourcesWithShaders()
{
	ShadersManager &  shaderManager = ShadersManager::GetShadersManager();
	shaderManager.ReInitDeviceResources();

//	ReInitDeviceResources();
}

void ResourceLoader::ReInitTexture(PVRTexture * pTexture)
{
	GLuint genTex;

	EPVRTError err = PVRTTextureLoadFromPointer( pTexture->GetImage().GetValues(), &genTex);
	if (err != PVR_SUCCESS)
	{
		LOGE("Failed to recreate texture.");
		STOP;
	}
	else
	{
		LOGI("Recreated texture.");
		pTexture->SetTextureObject(genTex); 
	}
}
void ResourceLoader::ReInitVerticesIndices(VertInd * pVerInd)
{	
	pVerInd->pVertices->ReInitDeviceResources();
	pVerInd->pIndices->ReInitDeviceResources();			
}

void ResourceLoader::ReInitDeviceResources()
{
	MapIterator textureIter(mTexturesMap);
	while (PVRTexture * pTexture = (PVRTexture*)textureIter.GetPtr())
	{
		GLuint genTex;

		EPVRTError err = PVRTTextureLoadFromPointer( pTexture->GetImage().GetValues(), &genTex);
		if (err != PVR_SUCCESS)
		{
			LOGE("Failed to recreate texture.");
			STOP;
		}
		else
		{
			LOGI("Recreated texture.");
			pTexture->SetTextureObject(genTex); 
		}
	}
	
	ListIterator vertIndIter(mVertIndList);
	while (VertInd * vi = (VertInd*)vertIndIter.GetPtr())
	{
		vi->pVertices->ReInitDeviceResources();
		vi->pIndices->ReInitDeviceResources();			

	}

/*
	int size = mModelsVec.size();
	for (int n=0; n < size ; n++)
	{
		ModelData * pModel = mModelsVec[n];

		int numOfMeshes = pModel->mMeshes.size();
		for (int n=0;n<numOfMeshes;n++)
		{
			Mesh * pMesh = pModel->mMeshes[n];
			Vertices * pVertices = pMesh->GetVertices();
			Indices * pIndices = pMesh->GetIndices();

			pVertices->ReInitDeviceResources();
			pIndices->ReInitDeviceResources();			
		}
	}
*/

	ListIterator overlayIter(mOverlayFaces);
	while ( OverlayFace * pFace = (OverlayFace*)overlayIter.GetPtr() )
	{
		Vertices * pVertices = pFace->GetVertices();
		Indices * pIndices = pFace->GetIndices();
		
		pVertices->ReInitDeviceResources();
		pIndices->ReInitDeviceResources();			
	}

	
}

ModelData * ResourceLoader::LoadModel(const char * fileName, bool forceReload, bool addSceneManager)
{
	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();
	
	static char modelFullPathName[256]={0};
	sprintf(modelFullPathName,"%s%s", folder, fileName);

	LOGI("Loading scene: %s", modelFullPathName);
	
	Loader loader;
	if (!loader.OpenFile(modelFullPathName))
	{
		return NULL;
	}

	ModelData * pModelData = new ModelData();
	pModelData->SetRoot( new Node() );
	pModelData->SetFileName(fileName);
	
	if (!ReadTextures(loader, pModelData))
	{
		LOGE("Error reading textures from file");
		STOP;
		return NULL;
	}

	if (!ReadMaterials(loader, pModelData))
	{
		LOGE("Error reading materials from file");
		STOP;
		return NULL;
	}

	if (!ReadTransforms(loader, pModelData))
	{
		LOGE("Error reading transforms from file");
		STOP;
		return NULL;
	}

	if (!ReadMeshes(loader, pModelData, forceReload))
	{
		LOGE("Error reading meshes from file");
		STOP;
		return NULL;
	}

	if (!ReadCameras(loader, pModelData))
	{
		LOGE("Error reading cameras from file");
		STOP;
		return NULL;
	}
	loader.FreeData();
	mModelsVec.Add(pModelData);

	pModelData->GetRoot()->UpdateAfterLoadingModel();

	if (addSceneManager)
	{
		SceneManager & sceneManager = SceneManager::GetSceneManager();
		sceneManager.AddRootNode(pModelData->GetRoot());
	}

	return pModelData;
}

void ResourceLoader::Reset()
{
	FreeDeviceResources();

	mTexturesMap.RemoveAll();
	mVertIndMap.RemoveAll();
	mVertIndList.RemoveAll();
	mModelsVec.RemoveAll();
	mLocatorsMap.RemoveAll();
	mStopCirclesMap.RemoveAll();
	mBotLaunchersVec.RemoveAll();
	mCurvesMap.RemoveAll();
	mCollisionMeshVec.RemoveAll();
	mWiresMap.RemoveAll();
	mDoorsMap.RemoveAll();
	mOverlayFaces.RemoveAll();
}

void ResourceLoader::Destroy()
{

	STOP;
/*
	mTexturesMap.DeleteAll();	
	mVertIndMap.DeleteAll();
	mVertIndList.DeleteAll();
	mLocatorsMap.DeleteAll();
	mStopCirclesMap.DeleteAll();
	mBotLaunchersVec.DeleteAll();
	mCurvesMap.DeleteAll();
	mWiresMap.DeleteAll();
	mCollisionMeshVec.DeleteAll();
	mDoorsMap.DeleteAll();

	mModelsVec.DeleteAll();
	mOverlayFaces.DeleteAll();
*/

}