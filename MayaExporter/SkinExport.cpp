#include "StdAfx.h"
#include "SkinExport.h"
#include "ModelExporter.h"

SkinExport::SkinExport(void)
{
}

SkinExport::~SkinExport(void)
{
}

void SkinExport::Export()
{

	ModelExporter & modelExporter = ModelExporter::GetExporter();

	modelExporter.mSkeletonRootIndex = -1;
	if (modelExporter.mSceneTransformsTable.find(modelExporter.mSkeletonRoot) != modelExporter.mSceneTransformsTable.end())
		modelExporter.mSkeletonRootIndex = modelExporter.mSceneTransformsTable[modelExporter.mSkeletonRoot];

	modelExporter.ResetTimeline();

	MStatus stat;
	MItDependencyNodes itDep(MFn::kSkinClusterFilter,&stat);

	while (!itDep.isDone()) 
	{
		MObject obj = itDep.item();

		MFnSkinCluster fnSkin(obj,&stat);
		err_code(stat);

		MString cmd = MString("reference -q -f ") + fnSkin.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDep.next();
			continue;
		}

		// loop through the geometries affected by this cluster
		//

		int numGeoms = fnSkin.numOutputConnections(&stat);
		err_code(stat);

		for (int geomIndex = 0; geomIndex < numGeoms; geomIndex++)
		{
			unsigned int index = fnSkin.indexForOutputConnection(static_cast<unsigned int>(geomIndex),&stat);
			err_code(stat);

			// get the dag path of the geomIndex'th geometry
			//
			MDagPath skinPath;
			stat = fnSkin.getPathAtIndex(index,skinPath);
			err_code(stat);

			// iterate through the components of this geometry
			//
			MItGeometry gIter(skinPath,&stat);
			err_code(stat);

			// print out the path name of the skin, vertexCount & influenceCount
			//
			MObjectArray objArray;

			stat = fnSkin.getOutputGeometry(objArray);
			err_code(stat);

			if (objArray.length()!=1)
			{
				err_stop("Skin object without geometry attached");				
			}

			MObject geomObj = objArray[0];
			MFnMesh mesh(geomObj, &stat);
			err_code(stat);

			//stat = MGlobal::executeCommand(MString("polyTriangulate -ch 0 ") + mesh.name());
			//err_code(stat);

			MItMeshPolygon itPoly(mesh.object(&stat));
			err_code(stat);


			if ( modelExporter.mMeshSkins.find(mesh.name().asChar()) == modelExporter.mMeshSkins.end() )
			{
				MeshSkinData meshSkinData; 
				meshSkinData.meshName = mesh.name().asChar();
				modelExporter.mMeshSkins[meshSkinData.meshName] = meshSkinData;
			}

			MeshSkinData & meshSkinData = modelExporter.mMeshSkins[mesh.name().asChar()];

			MDagPathArray infs;
			MStatus stat;
			unsigned int nInfs = fnSkin.influenceObjects(infs, &stat);
			err_code(stat);

			if (infs.length() == 0)
			{				
				err_stop("Skin object does not influence joints");
			}


			int numVerts = gIter.count(&stat);
			err_code(stat);

			for ( int i=0; i < numVerts; i++ )
			{
				MObject comp = gIter.component(&stat);
				err_code(stat);

				// Get the weights for this vertex (one per influence object)
				//
				MFloatArray wts;

				unsigned int infCount;

				stat = fnSkin.getWeights(skinPath,comp,wts,infCount);
				err_code(stat);

				if ( infCount > 0 && gIter.index() == i && !gIter.isDone())
				{				
					int foundBigWeights = 0;
					for (unsigned int jj = 0; jj < infCount ; jj++ )
					{
						float w  = wts[jj];
						if(w >= 0.001f)
						{	
							foundBigWeights++;
							MDagPath jointDagPath = infs[jj];
							MObject jointObj = jointDagPath.node(&stat);
							err_code(stat);

							MFnTransform jointTransform(jointObj, &stat);
							err_code(stat);

							const char * jointName = jointTransform.name().asChar();

							if (meshSkinData.mIndicesJoints.find(i) == meshSkinData.mIndicesJoints.end())
							{
								JointWeights jointWeights;
								meshSkinData.mIndicesJoints[i] = jointWeights;
							}

							JointWeights & jointWeights = meshSkinData.mIndicesJoints[i];

							jointWeights.mWeights[jointName] = w;		
							if (0) //
							{
								static char txt[256] = {0};
								sprintf(txt, "%s - %f",jointName, w);
								err_print(txt);
							}
						}
					}

					JointWeights & jointWeights = meshSkinData.mIndicesJoints[i];
					while (jointWeights.mWeights.size() > 4)
					{
						float smalletsWeight = 1.0f;
						string smallestJoint;
						map<string, float>::iterator smallIter = jointWeights.mWeights.begin();
						while (smallIter!=jointWeights.mWeights.end())
						{
							if (smalletsWeight > smallIter->second)
							{
								smalletsWeight = smallIter->second;
								smallestJoint = smallIter->first;
							}
							smallIter++;
						}

						jointWeights.mWeights.erase(smallestJoint);

						float addWeight = smalletsWeight / 4.0f;
						smallIter = jointWeights.mWeights.begin();
						while (smallIter!=jointWeights.mWeights.end())
						{
							smallIter->second += addWeight;						
							smallIter++;
						}
					}


					stat = gIter.next();
					err_code(stat);

				}
			}	

			//////////////////////////////////////////////////////////////////////////////////////////

			
			//int count = itPoly.count(&stat);
			//err_code(stat);

			int id = 0;
			while ( !itPoly.isDone() )
			{
			
				int i0 = itPoly.vertexIndex(0, &stat); err_code(stat);
				int i1 = itPoly.vertexIndex(1, &stat); err_code(stat);
				int i2 = itPoly.vertexIndex(2, &stat); err_code(stat);

				list<string> jointsForFace;

				JointWeights & jointWeights0 = meshSkinData.mIndicesJoints[i0];
				JointWeights & jointWeights1 = meshSkinData.mIndicesJoints[i1];
				JointWeights & jointWeights2 = meshSkinData.mIndicesJoints[i2];

				map<string, float>::iterator weightsIter;
				weightsIter = jointWeights0.mWeights.begin();	
				int k;
				k=0;
				while (weightsIter!=jointWeights0.mWeights.end())
				{					
					jointsForFace.push_back( weightsIter->first );
					k++;
					weightsIter++;
				}

				k=0;
				weightsIter = jointWeights1.mWeights.begin();				
				while (weightsIter!=jointWeights1.mWeights.end())
				{					
					jointsForFace.push_back( weightsIter->first );
					k++;
					weightsIter++;
				}

				k=0;
				weightsIter = jointWeights2.mWeights.begin();				
				while (weightsIter!=jointWeights2.mWeights.end())
				{					
					jointsForFace.push_back( weightsIter->first );
					k++;
					weightsIter++;
				}

				jointsForFace.sort();
				jointsForFace.unique();
				if (jointsForFace.size() > MAX_JOINTS_PER_FACE)
				{
					static char txt[256]={0};
					sprintf(txt, "Number of joints influence face is bigger then %d: %d - in mesh %s", MAX_JOINTS_PER_FACE, jointsForFace.size(), meshSkinData.meshName.c_str());
					err_stop(txt);
				}

				if ( meshSkinData.mGroupedFaces.find(jointsForFace) == meshSkinData.mGroupedFaces.end())
				{
					vector<FaceInfo> facesVec;					
					meshSkinData.mGroupedFaces[jointsForFace] = facesVec;
				}				

				vector<FaceInfo> & facesVec = meshSkinData.mGroupedFaces[jointsForFace];

				FaceInfo face;
				face.i0 =  itPoly.vertexIndex(0, &stat); err_code(stat);
				face.i1 =  itPoly.vertexIndex(1, &stat); err_code(stat);
				face.i2 =  itPoly.vertexIndex(2, &stat); err_code(stat);

				face.n0 = itPoly.normalIndex(0, &stat); err_code(stat);
				face.n1 = itPoly.normalIndex(1, &stat); err_code(stat);
				face.n2 = itPoly.normalIndex(2, &stat); err_code(stat);

				stat = itPoly.getUVIndex(0,face.uv0); err_code(stat); 
				stat = itPoly.getUVIndex(1,face.uv1); err_code(stat); 
				stat = itPoly.getUVIndex(2,face.uv2); err_code(stat); 
				face.id = id;
		
				facesVec.push_back(face);
				itPoly.next();
				id++;

			}

			// Optimize several times:
			err_info("Starting optimization ...");
			map< list<string> , vector<FaceInfo> >::iterator groupedFacesIter = meshSkinData.mGroupedFaces.begin();
			while (groupedFacesIter != meshSkinData.mGroupedFaces.end())
			{

				if (groupedFacesIter->second.size() == 0)
				{
					groupedFacesIter++;
					continue;
				}

				bool breakAll = false;
				list<string>  toFind = groupedFacesIter->first;
				map<list<string>, vector<FaceInfo> >::iterator groupedFacesIter2 = meshSkinData.mGroupedFaces.begin();
				while (groupedFacesIter2 != meshSkinData.mGroupedFaces.end())
				{
					list<string> toCompare = groupedFacesIter2->first;

					if (toFind == toCompare || groupedFacesIter2->second.size() == 0)
					{
						groupedFacesIter2++;
						continue;
					}

					list<string> newJointList;
					vector<FaceInfo> newJointFaces;

					list<string>::iterator iter1 = toFind.begin();
					list<string>::iterator iter2 = toCompare.begin();
					while(iter1 != toFind.end())
					{
						newJointList.push_back(iter1->c_str());
						iter1++;
					}

					while(iter2 != toCompare.end())
					{
						newJointList.push_back(iter2->c_str());
						iter2++;
					}

					newJointList.sort();
					newJointList.unique();

					if (newJointList.size() <= MAX_JOINTS_PER_FACE)
					{
						int count1 = groupedFacesIter->second.size();
						for (int n=0;n<count1;n++)
						{
							FaceInfo & face = groupedFacesIter->second[n];
							newJointFaces.push_back( face );
						}

						int count2 = groupedFacesIter2->second.size();
						for (int n=0;n<count2;n++)
						{
							FaceInfo & face = groupedFacesIter2->second[n];
							newJointFaces.push_back( face );
						}

						groupedFacesIter->second.clear();
						groupedFacesIter2->second.clear();

						meshSkinData.mGroupedFaces.erase(groupedFacesIter);
						meshSkinData.mGroupedFaces.erase(groupedFacesIter2);
						
						meshSkinData.mGroupedFaces[newJointList] = newJointFaces;
						groupedFacesIter2 = meshSkinData.mGroupedFaces.begin();
						breakAll = true;
						err_progress();
						break;		
					}
					groupedFacesIter2++;
				}
				if (breakAll)
				{
					groupedFacesIter = meshSkinData.mGroupedFaces.begin();
				}
				else
				{
					groupedFacesIter++;
				}
			}



			groupedFacesIter = meshSkinData.mGroupedFaces.begin();
			while (groupedFacesIter != meshSkinData.mGroupedFaces.end())
			{
				int mergedListSize = groupedFacesIter->second.size();
				list<string> newJointList = groupedFacesIter->first;
				for (int n=0;n<mergedListSize;n++)
				{
					FaceInfo & face = groupedFacesIter->second[n];

					JointWeights & jointWeights0 = meshSkinData.mIndicesJoints[face.i0];
					JointWeights & jointWeights1 = meshSkinData.mIndicesJoints[face.i1];
					JointWeights & jointWeights2 = meshSkinData.mIndicesJoints[face.i2];

					vector<int> jointIndex0;
					vector<int> jointIndex1;
					vector<int> jointIndex2;

					vector<float> weights0;
					vector<float> weights1;
					vector<float> weights2;

					findJointsIndexInList(newJointList, jointWeights0, jointIndex0, weights0);
					findJointsIndexInList(newJointList, jointWeights1, jointIndex1, weights1);
					findJointsIndexInList(newJointList, jointWeights2, jointIndex2, weights2);

					for (size_t q=0;q<jointIndex0.size();q++)
					{
						face.t0[q] = (char)jointIndex0[q];
						face.w0[q] = weights0[q];
					}
					for (size_t q=0;q<jointIndex1.size();q++)
					{
						face.t1[q] = (char)jointIndex1[q];
						face.w1[q] = weights1[q];
					}
					for (size_t q=0;q<jointIndex2.size();q++)
					{
						face.t2[q] = (char)jointIndex2[q];					
						face.w2[q] = weights2[q];
					}
				}

				groupedFacesIter++;
			}

			
		}

		itDep.next();
	}

}

void SkinExport::findJointsIndexInList(list<string> & jointsForFace, JointWeights & jointWeights, vector<int> & jointsIndex, vector<float> & weights)
{

	map<string, float>::iterator weightsIter = jointWeights.mWeights.begin();
	while (weightsIter!=jointWeights.mWeights.end())
	{
		int count = 0;
		list<string>::iterator jointsForFaceIter = jointsForFace.begin();
		while (jointsForFaceIter!=jointsForFace.end())
		{

			if (jointsForFaceIter->compare(weightsIter->first) == 0)
			{

				jointsIndex.push_back(count);
				weights.push_back(weightsIter->second);
				break;
			}
			count++;
			jointsForFaceIter++;
		}

		weightsIter++;
	}
}
