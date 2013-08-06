#include "StdAfx.h"
#include "GeometryExport.h"
#include "ModelExporter.h"
#include "PVRTTriStrip.h"


GeometryExport::GeometryExport(void)
{
}

GeometryExport::~GeometryExport(void)
{
}


void GeometryExport::Export()
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();

	modelExporter.ResetTimeline();

	MStatus stat;
	MItDependencyNodes itDep(MFn::kMesh,&stat);


	while (!itDep.isDone()) 
	{
		MObject obj = itDep.item();
		MFnMesh mesh(obj, &stat);
		err_code(stat);

		if ( mesh.isIntermediateObject() )
		{
			itDep.next();
			continue;
		}

		MString cmd = MString("reference -q -f ") + mesh.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDep.next();
			continue;
		}

		//stat = MGlobal::executeCommand(MString("polyTriangulate -ch 0 ") + mesh.name());
		//err_code(stat);

		MString meshName = mesh.name(&stat);
		err_code(stat);

		MItMeshPolygon itPoly(mesh.object(&stat));
		err_code(stat);

		int numTriangles = 0;
		stat = itPoly.numTriangles(numTriangles);
		err_code(stat);

		if ( numTriangles != 1 )
		{
			//err_stop(string("Quad mesh: ") + meshName.asChar() );
			itDep.next();
			continue;
		}

		
		
		err_info(string("MESH: ") + meshName.asChar());

		MeshData * pMeshData = new MeshData();
		pMeshData->name = meshName.asChar();

		pMeshData->materialIndex = GetShaderIndex(mesh);
		MObject parentObj = mesh.parent(0,&stat);
		MFn::Type parentType = parentObj.apiType();

		if ( modelExporter.CheckParentType( parentType) )
		{
			MFnDagNode parentDagNode(parentObj, &stat);
			MString transName = parentDagNode.name(&stat);

			pMeshData->transformIndex  = modelExporter.mSceneTransformsTable[transName.asChar()];
		}

		MPlug layer_plug = mesh.findPlug( "Layer" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = layer_plug.getValue(pMeshData->layer);
			err_code(stat);
		}
		else
		{
			err_stop(string("Layer attribute not found in Mesh: ") + pMeshData->name );
		}

		MPlug shader_plug = mesh.findPlug( "ShaderType" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = shader_plug.getValue(pMeshData->glsl_shader);
			err_code(stat);
		}
		else
		{
			err_stop(string("ShaderType attribute not found in Mesh: ") + pMeshData->name );
		}

		MPlug alpha_mode = mesh.findPlug( "AlphaMode" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = alpha_mode.getValue(pMeshData->alpha_mode);
			err_code(stat);
		}
		else
		{
			err_stop(string("AlphaMode attribute not found in Mesh: ") + pMeshData->name );
		}

		MPlug display_mode = mesh.findPlug( "DisplayMode" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = display_mode.getValue(pMeshData->display_mode);
			err_code(stat);
		}
		else
		{
			err_stop(string("DisplayMode attribute not found in Mesh: ") + pMeshData->name );
		}



		pMeshData->use_depth = 1;
		MPlug use_depth = mesh.findPlug( "Depth" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = use_depth.getValue(pMeshData->use_depth);
			err_code(stat);
		}
		
		pMeshData->use_back_face_culling = 1;
		MPlug use_back_face_culling = mesh.findPlug( "BackFaceCulling" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = use_back_face_culling.getValue(pMeshData->use_back_face_culling);
			err_code(stat);
		}

		pMeshData->use_occlusion_culling = 1;
		MPlug use_occlusion_culling = mesh.findPlug( "Occlusion" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = use_occlusion_culling.getValue(pMeshData->use_occlusion_culling);
			err_code(stat);
		}
		

		if (pMeshData->transformIndex == -1)
		{
			err_stop("Parent node not found");
		}

		
		MBoundingBox bbox = mesh.boundingBox(&stat);
	//	bbox.transformUsing(mesh.transformationMatrix(&stat));
		err_code(stat);

		MPoint bbox_center = bbox.center();
		double bbox_w = bbox.width();
		double bbox_h = bbox.height();
		double bbox_d = bbox.depth();

		double bbox_b = sqrt(bbox_w*bbox_w+bbox_h*bbox_h+bbox_d*bbox_d);
		bbox_center = bbox.center();

		pMeshData->radius = (float)bbox_b * 0.5f;
		
		MFloatPointArray points;
		stat = mesh.getPoints(points);
		err_code(stat);

		MFloatVectorArray normals;
		stat = mesh.getNormals(normals);
		err_code(stat);

		MFloatArray uArray;
		MFloatArray vArray;

		stat = mesh.getUVs(uArray, vArray);
		err_code(stat);

		vector<VertexSkin> vertexData;
		vector<int> indicesData;
		vector< vector<int> > indicesOfSubMesh;
		map< pair<int,int> ,int> conv;

		int count = 0;

		MeshSkinData meshSkinData;
		bool usingSkin = false;
		if ( modelExporter.mMeshSkins.find(pMeshData->name) != modelExporter.mMeshSkins.end() )
		{
			usingSkin = true;
			meshSkinData =  modelExporter.mMeshSkins[pMeshData->name];
		}

		if (usingSkin)
		{
			map<list<string>, vector<FaceInfo> >::iterator groupedFacesIter = meshSkinData.mGroupedFaces.begin();
			while(groupedFacesIter != meshSkinData.mGroupedFaces.end())
			{
				
				vector<FaceInfo> & faceInfoVec = groupedFacesIter->second;
				int numOfFaces = faceInfoVec.size();
				
				for (int f=0;f<numOfFaces;f++)
				{
					FaceInfo & faceInfo = faceInfoVec[f];

					if (conv.find( pair<int,int>(faceInfo.id,faceInfo.i0) ) == conv.end())
					{
						VertexSkin ver0;
						ver0.x = points[faceInfo.i0].x;
						ver0.y = points[faceInfo.i0].y;
						ver0.z = points[faceInfo.i0].z;

						ver0.nx = normals[faceInfo.n0].x;
						ver0.ny = normals[faceInfo.n0].y;
						ver0.nz = normals[faceInfo.n0].z;

						ver0.u = uArray[faceInfo.uv0];
						ver0.v = vArray[faceInfo.uv0];
	
						ver0.w0 = faceInfo.w0[0];
						ver0.w1 = faceInfo.w0[1];
						ver0.w2 = faceInfo.w0[2];
						ver0.w3 = faceInfo.w0[3];

						ver0.t0 = faceInfo.t0[0];
						ver0.t1 = faceInfo.t0[1];
						ver0.t2 = faceInfo.t0[2];
						ver0.t3 = faceInfo.t0[3];
					
						vertexData.push_back(ver0);
						conv[ pair<int,int>(faceInfo.id,faceInfo.i0) ] = count;
						pMeshData->mConvUV[ count ] = faceInfo.uv0;
						indicesData.push_back(count);
						count++;

					}
					else
					{
						int ind = conv[pair<int,int>(faceInfo.id,faceInfo.i0)];
						indicesData.push_back(ind);
					}

					if (conv.find( pair<int,int>(faceInfo.id,faceInfo.i1) ) == conv.end())
					{
						VertexSkin ver1;
						ver1.x = points[faceInfo.i1].x;
						ver1.y = points[faceInfo.i1].y;
						ver1.z = points[faceInfo.i1].z;


						ver1.nx = normals[faceInfo.n1].x;
						ver1.ny = normals[faceInfo.n1].y;
						ver1.nz = normals[faceInfo.n1].z;	

						ver1.u = uArray[faceInfo.uv1];
						ver1.v = vArray[faceInfo.uv1];
						
						
						ver1.w0 = faceInfo.w1[0];
						ver1.w1 = faceInfo.w1[1];
						ver1.w2 = faceInfo.w1[2];
						ver1.w3 = faceInfo.w1[3];

						ver1.t0 = faceInfo.t1[0];
						ver1.t1 = faceInfo.t1[1];
						ver1.t2 = faceInfo.t1[2];
						ver1.t3 = faceInfo.t1[3];
					
						vertexData.push_back(ver1);
						conv[pair<int,int>(faceInfo.id,faceInfo.i1)] = count;
						pMeshData->mConvUV[ count ] = faceInfo.uv1;
						indicesData.push_back(count);
						count++;
					}
					else
					{
						int ind = conv[pair<int,int>(faceInfo.id,faceInfo.i1)];
						indicesData.push_back(ind);
					}


					if (conv.find( pair<int,int>(faceInfo.id,faceInfo.i2) ) == conv.end())
					{
						VertexSkin ver2;

						ver2.x = points[faceInfo.i2].x;
						ver2.y = points[faceInfo.i2].y;
						ver2.z = points[faceInfo.i2].z;

						ver2.nx = normals[faceInfo.n2].x;
						ver2.ny = normals[faceInfo.n2].y;
						ver2.nz = normals[faceInfo.n2].z;

						ver2.u = uArray[faceInfo.uv2];
						ver2.v = vArray[faceInfo.uv2];
							
						ver2.w0 = faceInfo.w2[0];
						ver2.w1 = faceInfo.w2[1];
						ver2.w2 = faceInfo.w2[2];
						ver2.w3 = faceInfo.w2[3];

						ver2.t0 = faceInfo.t2[0];
						ver2.t1 = faceInfo.t2[1];
						ver2.t2 = faceInfo.t2[2];
						ver2.t3 = faceInfo.t2[3];
				
						vertexData.push_back(ver2);
						conv[pair<int,int>(faceInfo.id,faceInfo.i2)] = count;
						pMeshData->mConvUV[ count ] = faceInfo.uv2;
						indicesData.push_back(count);
						count++;
					}
					else
					{
						int ind = conv[pair<int,int>(faceInfo.id,faceInfo.i2)];
						indicesData.push_back(ind);
					}

				}

				indicesOfSubMesh.push_back(indicesData);
				indicesData.clear();
				groupedFacesIter++;
			}
		}
		//like else :
		if (usingSkin == false)
		{
			int id = 0;
			while ( !itPoly.isDone() )
			{

					int i0 = itPoly.vertexIndex(0, &stat); err_code(stat);
					int i1 = itPoly.vertexIndex(1, &stat); err_code(stat);
					int i2 = itPoly.vertexIndex(2, &stat); err_code(stat);

					int n0 = itPoly.normalIndex(0, &stat); err_code(stat);
					int n1 = itPoly.normalIndex(1, &stat); err_code(stat);
					int n2 = itPoly.normalIndex(2, &stat); err_code(stat);

					int uv0;
					int uv1;
					int uv2;

					stat = itPoly.getUVIndex(0,uv0); err_code(stat);
					stat = itPoly.getUVIndex(1,uv1); err_code(stat);
					stat = itPoly.getUVIndex(2,uv2); err_code(stat);

					if (conv.find( pair<int,int>(id,i0) ) == conv.end())
					{
						VertexSkin ver0;
						ver0.x = points[i0].x;
						ver0.y = points[i0].y;
						ver0.z = points[i0].z;

						ver0.nx = normals[n0].x;
						ver0.ny = normals[n0].y;
						ver0.nz = normals[n0].z;

						ver0.u = uArray[uv0];
						ver0.v = vArray[uv0];

						vertexData.push_back(ver0);
						conv[pair<int,int>(id,i0)] = count;
						pMeshData->mConvUV[ count ] = uv0;
						indicesData.push_back(count);
						count++;

					}
					else
					{
						int ind = conv[pair<int,int>(id,i0)];
						indicesData.push_back(ind);
					}

					if (conv.find(pair<int,int>(id,i1)) == conv.end())
					{
						VertexSkin ver1;
						ver1.x = points[i1].x;
						ver1.y = points[i1].y;
						ver1.z = points[i1].z;


						ver1.nx = normals[n1].x;
						ver1.ny = normals[n1].y;
						ver1.nz = normals[n1].z;	

						ver1.u = uArray[uv1];
						ver1.v = vArray[uv1];

						vertexData.push_back(ver1);
						conv[pair<int,int>(id,i1)] = count;
						pMeshData->mConvUV[ count ] = uv1;
						indicesData.push_back(count);
						count++;
					}
					else
					{
						int ind = conv[pair<int,int>(id,i1)];
						indicesData.push_back(ind);
					}


					if (conv.find(pair<int,int>(id,i2)) == conv.end())
					{
						VertexSkin ver2;

						ver2.x = points[i2].x;
						ver2.y = points[i2].y;
						ver2.z = points[i2].z;

						ver2.nx = normals[n2].x;
						ver2.ny = normals[n2].y;
						ver2.nz = normals[n2].z;

						ver2.u = uArray[uv2];
						ver2.v = vArray[uv2];

						vertexData.push_back(ver2);
						conv[pair<int,int>(id,i2)] = count;
						pMeshData->mConvUV[ count ] = uv2;
						indicesData.push_back(count);
						count++;
					}
					else
					{
						int ind = conv[pair<int,int>(id,i2)];
						indicesData.push_back(ind);
					}		

					itPoly.next();
					id++;
				}

			indicesOfSubMesh.push_back(indicesData);
		}

		itPoly.reset();

		int verticesCount = vertexData.size();
		pMeshData->pVertices = (VertexSkin*) malloc(sizeof(VertexSkin) * verticesCount);
		pMeshData->verticesCount = verticesCount;

		for (int n=0;n<verticesCount;n++)
		{
			pMeshData->pVertices[n] = vertexData[n];
		}

		int numOfSubMeshIndices = indicesOfSubMesh.size();
		for (int subMeshIndex = 0 ; subMeshIndex<numOfSubMeshIndices ; subMeshIndex++)
		{

			vector<int> & subIndicesData = indicesOfSubMesh[subMeshIndex];
			int indicesCount = subIndicesData.size();
			unsigned short * pIndices = (unsigned short*) malloc(sizeof(unsigned short) * indicesCount);
			pMeshData->mSubMeshIndices.push_back( pIndices );
			pMeshData->mSubMeshIndicesCount.push_back( indicesCount );

			for (int n=0;n<indicesCount;n++)
			{
				pIndices[n] = subIndicesData[n];
			}

			PVRTTriStripList( pIndices,  indicesCount/3);
		}

		pMeshData->index = modelExporter.mMeshCount;
		modelExporter.mSceneMeshes.push_back(pMeshData);

		modelExporter.mSceneMeshTable[meshName.asChar()] = modelExporter.mMeshCount;
		modelExporter.mMeshCount++;


		stat = itDep.next();
		err_code(stat);
	}

	WriteGeometries();
}


void GeometryExport::WriteGeometries()
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	ModelExporter & modelExporter = ModelExporter::GetExporter();
	Saver & saver = modelExporter.GetSaver();

	//Write number of meshes
	saver.writeInt(modelExporter.mSceneMeshes.size()); //!!!!!!!!!!!!!!!

	vector<MeshData*>::iterator nodesIter = modelExporter.mSceneMeshes.begin();
	while (nodesIter != modelExporter.mSceneMeshes.end())
	{
		MeshData * pMeshData = *nodesIter;
		
		int textureIndex = modelExporter.mSceneMaterials[pMeshData->materialIndex]->textureIndex;
		TextureData * pTexture = modelExporter.mSceneTextures[textureIndex];

		
		//Write index and parent
		saver.writeInt(pMeshData->index); //!!!!!!!!!!!!!!!
		saver.writeInt(pMeshData->layer); //!!!!!!!!!!!!!!!
		saver.writeFloat32(GLOBAL_SCALE*pMeshData->radius); //!!!!!!!!!!!!!!!
		saver.writeInt(pMeshData->glsl_shader);
		saver.writeInt(pMeshData->alpha_mode);
		saver.writeInt(pMeshData->display_mode);
		saver.writeInt(pMeshData->use_depth);
		saver.writeInt(pMeshData->use_back_face_culling);
		saver.writeInt(pMeshData->use_occlusion_culling);

		saver.writeInt(pMeshData->transformIndex); //!!!!!!!!!!!!!!!
		saver.writeInt(pMeshData->materialIndex);//!!!!!!!!!!!!!!!
		saver.writeInt(pMeshData->verticesCount);//!!!!!!!!!!!!!!!

		bool usingSkin = false;
		if ( modelExporter.mMeshSkins.find(pMeshData->name) != modelExporter.mMeshSkins.end())
		{
			MeshSkinData & meshSkinData = modelExporter.mMeshSkins[pMeshData->name];
			saver.writeInt(1); // USING SKIN !!! //!!!!!!!!!!!!!!!
			saver.writeInt(modelExporter.mSkeletonRootIndex);
			usingSkin = true;
		}
		else
		{
			saver.writeInt(0); //NO SKIN !!!//!!!!!!!!!!!!!!!
			usingSkin = false;
		}

		for (int vertex = 0 ; vertex < pMeshData->verticesCount ; vertex ++)
		{
			VertexSkin * pVertex = pMeshData->pVertices + vertex;
			//Write floats
			saver.writeFloat32(GLOBAL_SCALE*pVertex->x);//!!!!!!!!!!!!!!!
			saver.writeFloat32(GLOBAL_SCALE*pVertex->y);//!!!!!!!!!!!!!!!
			saver.writeFloat32(GLOBAL_SCALE*pVertex->z);//!!!!!!!!!!!!!!!

			saver.writeFloat32(pVertex->nx);//!!!!!!!!!!!!!!!
			saver.writeFloat32(pVertex->ny);//!!!!!!!!!!!!!!!
			saver.writeFloat32(pVertex->nz);//!!!!!!!!!!!!!!!

			saver.writeFloat32(pVertex->u * pTexture->uRepeat);//!!!!!!!!!!!!!!!
			saver.writeFloat32(pVertex->v * pTexture->vRepeat);//!!!!!!!!!!!!!!!

			if (usingSkin)
			{
				saver.writeFloat32(pVertex->w0);//!!!!!!!!!!!!!!!
				saver.writeFloat32(pVertex->w1);//!!!!!!!!!!!!!!!
				saver.writeFloat32(pVertex->w2);//!!!!!!!!!!!!!!!
				saver.writeFloat32(pVertex->w3);//!!!!!!!!!!!!!!!

				float total = pVertex->w0 + pVertex->w1 +  pVertex->w2 + pVertex->w3;
				if (total > 1.0f )
				{
					char www[32]={0};
					sprintf(www,"%f", total);
					//err_print(www);
				}
				saver.writeChar(pVertex->t0);//!!!!!!!!!!!!!!!
				saver.writeChar(pVertex->t1);//!!!!!!!!!!!!!!!
				saver.writeChar(pVertex->t2);//!!!!!!!!!!!!!!!
				saver.writeChar(pVertex->t3);//!!!!!!!!!!!!!!!
			}
		}

		int numOfSubMesh = pMeshData->mSubMeshIndices.size();
		saver.writeInt(numOfSubMesh); //!!!!!!!!!!!!!!!


		for (int subMeshIndex = 0; subMeshIndex<numOfSubMesh ; subMeshIndex++)
		{
			int indicesCount = pMeshData->mSubMeshIndicesCount[subMeshIndex];
			saver.writeInt(indicesCount); //!!!!!!!!!!!!!!!
		}

		for (int subMeshIndex = 0; subMeshIndex<numOfSubMesh ; subMeshIndex++)
		{
			int indicesCount = pMeshData->mSubMeshIndicesCount[subMeshIndex];
			unsigned short * pIndices = pMeshData->mSubMeshIndices[subMeshIndex];
			for (int ind = 0 ; ind < indicesCount ; ind++)
			{
				saver.writeUShort(pIndices[ind]);//!!!!!!!!!!!!!!!
			}

			if (indicesCount%2) //eii zugi
			{
				saver.writeUShort(0); //!!!!!!!!!!!!!!! // should be implemented in reader also !!!
			}
		}

		if (usingSkin)
		{
			MeshSkinData & meshSkinData = modelExporter.mMeshSkins[pMeshData->name];
			map<list<string>, vector<FaceInfo> >::iterator  groupedFacesIter =  meshSkinData.mGroupedFaces.begin();
			saver.writeInt(meshSkinData.mGroupedFaces.size()); //!!!!!!!!!!!!!!! number of batches // just for comparing with numOfSubMesh

			while(groupedFacesIter != meshSkinData.mGroupedFaces.end())
			{
				list<string> transformList = groupedFacesIter->first;
				int numTransforms = transformList.size();
				saver.writeInt(numTransforms); //!!!!!!!!!!!!!!! number of Transforms
				
				list<string>::iterator transformListIter = transformList.begin();
				while (transformListIter!= transformList.end())
				{
					string & transformName = *transformListIter;
					int transformIndex = modelExporter.mSceneTransformsTable[transformName];
					saver.writeInt(transformIndex); //!!!!!!!!!!!!!!! Transform index
					transformListIter++;
				}

				groupedFacesIter++;
			}
		}

		nodesIter++;
	}
}




		

int GeometryExport::GetShaderIndex(MFnMesh & mesh)
{

	ModelExporter & modelExporter = ModelExporter::GetExporter();

	MObjectArray shadersObjsArray;
	MIntArray    polyMaterial;

	MStatus stat;
	stat = mesh.getConnectedShaders(0,shadersObjsArray,polyMaterial);
	err_code(stat);

	MString meshName = mesh.name(&stat);
	err_code(stat);


	// for each surface shader
	unsigned int shadersLength = shadersObjsArray.length();
	if (shadersLength > 1)
	{
		err_stop( string("Too many shaders for mesh") + meshName.asChar());
	}

	for(unsigned int k=0;k<shadersLength;k++)
	{
		// attachy function set...
		MFnDependencyNode fnShader(shadersObjsArray[k]);

		// get access to the surfaceShader attribute. This will be connected to
		// lambert , phong nodes etc.
		MPlug surfaceShader = fnShader.findPlug("surfaceShader",&stat);
		err_code(stat);
		MString shaderName = surfaceShader.name();

		MPlugArray materials;

		// get the material connected to the surface shader
		surfaceShader.connectedTo(materials,true,true,&stat);
		err_code(stat);

		int l=0; // Single material
		//For supportting multi materials :
		unsigned int matrialLength = materials.length();

		for(unsigned int l=0;l<matrialLength;l++)
		{
			MFnDependencyNode fnMat(materials[l].node());
			bool defaultNode = fnMat.isDefaultNode(&stat);
			err_code(stat);

			// Beware !!!! we are not using default shaders
			//if (defaultNode)
			//	continue;

			
			return modelExporter.mSceneMaterialsTable[ fnMat.name().asChar() ];


				}
			}

	return -1;
}