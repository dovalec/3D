#include "StdAfx.h"
#include "UVAnimationExport.h"
#include "ModelExporter.h"

UVAnimationExport::UVAnimationExport(void)
{
}

UVAnimationExport::~UVAnimationExport(void)
{
}


void UVAnimationExport::Export(string folder, string fileName)
{
	MStatus stat;
	MTime time(0, MTime::kFilm);
	stat = MGlobal::executeCommand(MString("currentTime -edit 0;"));
	err_code(stat);

	mFolder = folder;
	mFileName = fileName;

	MItDependencyNodes itDep(MFn::kMesh,&stat);


	map<string, int> curvesChecked;

	while (!itDep.isDone()) 
	{
		MObject obj = itDep.item();
		MFnDependencyNode depNode(obj, &stat);
		err_code(stat);


		MString cmd = MString("reference -q -f ") + depNode.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDep.next();
			continue;
		}


		MString meshName = depNode.name(&stat);
		err_code(stat); 

		MPlugArray plugArr;
		stat = depNode.getConnections(plugArr);
		err_code(stat);

		int numPlugConn = plugArr.length();
		for (int n=0;n<numPlugConn;n++)
		{
			MPlug plug = plugArr[n];
			MString plugName = plug.name(&stat);
			err_code(stat);

			const char * plugNameStr = plugName.asChar();
		
			MPlugArray connections;

			// get an array of connections to this attribute
			plug.connectedTo( connections, true, false, &stat );
			err_code(stat);

			int numPlugconn2 = connections.length();

			for(int i=0; i < numPlugconn2; i++) 
			{
				// get a reference to the node 
				MObject connected = connections[i].node(&stat);
				err_code(stat);

				MFn::Type connType = connected.apiType();
				if (connType == MFn::kPolyTweakUV)
				{
					MFnDependencyNode polyTweakUVDep(connected, &stat);
					MPlugArray ployTweakPlugArr;
					stat = polyTweakUVDep.getConnections(ployTweakPlugArr);
					err_code(stat);

					int numPolyPlugs = ployTweakPlugArr.length();
					for (int p=0;p<numPolyPlugs;p++)
					{
						MPlug plugPlyTweakConn = ployTweakPlugArr[p];
						MPlugArray ployTweakConnections;

						// get an array of connections to this attribute
						plugPlyTweakConn.connectedTo( ployTweakConnections, true, false, &stat );
						err_code(stat);

						int numPolyTweakConn = ployTweakConnections.length();
						for(int t=0; t < numPolyTweakConn; t++) 
						{
							MObject animConnected = ployTweakConnections[t].node(&stat);
							err_code(stat);

							// if it's an animation curve output it
							if(animConnected.hasFn(MFn::kAnimCurve)) 
							{
								MString infoStr = ployTweakConnections[t].info(&stat);			
								err_code(stat); 

								MFnAnimCurve animCurve(animConnected, &stat);
								err_code(stat); 
							
								MString curveName = animCurve.name(&stat).asChar();
								err_code(stat); 

								if (curvesChecked.find(curveName.asChar()) == curvesChecked.end())
								{
									ExportAnimationCurve( meshName.asChar(), animCurve );
									curvesChecked[curveName.asChar()] = 1;
								}
								

							}
						}

					}
				}
			}

		}
		stat = itDep.next();
		err_code(stat);
	}

	WriteAnimations();
}

void UVAnimationExport::ExportAnimationCurve(string meshName, MFnAnimCurve & animCurve)
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();

	int meshDataIndex = modelExporter.mSceneMeshTable[meshName];
	MeshData * pMeshData = modelExporter.mSceneMeshes[meshDataIndex];

	MStatus stat;
	MString curveName = animCurve.name(&stat);
	err_code(stat);

	string curveNameStr = curveName.asChar();
	int first_ = curveNameStr.find_first_of("_");
	first_++;
	curveNameStr = curveNameStr.substr(first_ + string("uvTweak_").length(), curveNameStr.length() - first_ + string("uvTweak_").length() );

	bool isU;
	int posU = curveNameStr.find("TweakU");
	int posV = curveNameStr.find("TweakV");
	if (posU != -1)
	{
		isU = true;
	}
	else if( posV != -1)
	{
		isU = false;
	}
	else
	{
		err_stop("Bad UV curve name");
	}

	first_ = curveNameStr.find_first_of("_");
	string uvNumber = curveNameStr.substr(0,first_);
	int uvIndex = atoi(uvNumber.c_str());
	
	
	map< int ,int>::iterator convUVIter = pMeshData->mConvUV.begin();
	while (convUVIter !=  pMeshData->mConvUV.end())
	{
		if (convUVIter->second != uvIndex)
		{
			convUVIter++;
			continue;
		}


		if (mUVKeys.find(meshName) == mUVKeys.end())
		{
			map<float,UVKey> mapUV;
			map<int, map<float,UVKey> > mapInd;
			mapInd[convUVIter->first] = mapUV;

			mUVKeys[meshName] = mapInd;
		}
		
		if (mUVKeys[meshName].find(convUVIter->first) == mUVKeys[meshName].end())
		{
			map<int, map<float,UVKey> > & mapInd = mUVKeys[meshName];

			map<float,UVKey> mapUV;
			mapInd[convUVIter->first] = mapUV;
		}

		map<int, map<float,UVKey> > & mapInd = mUVKeys[meshName];
		map<float,UVKey> & keysMap = mapInd[convUVIter->first];


		int numKeys = animCurve.numKeys(&stat);
		for (int k=0;k<numKeys;k++)
		{
			float value = (float)animCurve.value(k, &stat);
			MTime time = animCurve.time(k, &stat);
			err_code(stat);

			float time_f = (float)time.value();

			if (isU)
				keysMap[time_f].u = value;
			else
				keysMap[time_f].v = value;
		
		}

		convUVIter++;
	}



}

void UVAnimationExport::WriteAnimations()
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();

	map<string, map<int, map<float,UVKey> > >::iterator geomIter = mUVKeys.begin();
	
	while(geomIter != mUVKeys.end())
	{
		string clipName = geomIter->first;
		map<int, map<float,UVKey> > verticesKeys = geomIter->second;

		Saver saver;
		saver.SetFolder(mFolder);
		saver.CreateFile(mFileName + "_" + clipName + ".uva");

		int meshIndex = modelExporter.mSceneMeshTable[geomIter->first];
		saver.writeInt(meshIndex);  //!!!!!!!!!!!!!!!!!

		int numVertices = verticesKeys.size();
		saver.writeInt(numVertices);  //!!!!!!!!!!!!!!!!!
		
		map<int, map<float,UVKey> >::iterator verticesIter = verticesKeys.begin();

		
		while (verticesIter != verticesKeys.end())
		{
			int vertexId = verticesIter->first;
			map<float,UVKey> & keysMap = verticesIter->second;

			int numKeys = keysMap.size();
			saver.writeInt(vertexId); //!!!!!!!!!!!!!!!!!
			saver.writeInt(numKeys); //!!!!!!!!!!!!!!!!!

			map<float,UVKey>::iterator keysIter = keysMap.begin();
			while(keysIter != keysMap.end())
			{
				UVKey uv = keysIter->second;
				saver.writeFloat32(uv.u); //!!!!!!!!!!!!!!!!!
				saver.writeFloat32(uv.v); //!!!!!!!!!!!!!!!!!
				keysIter++;
			}

			verticesIter++;
		}

		geomIter++;
	}
	
}

void UVAnimationExport::SetTimeLinePos(MTime time)
{
	MStatus stat;
	static char cmd[128] = {0};
	sprintf(cmd,"currentTime -edit %f;", (float)time.value());
	stat = MGlobal::executeCommand(MString(cmd));
	err_code(stat);
}