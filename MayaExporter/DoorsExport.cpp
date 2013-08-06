#include "StdAfx.h"
#include "DoorsExport.h"
#include "ModelExporter.h"



DoorsExport::DoorsExport(void)
{
}

DoorsExport::~DoorsExport(void)
{
}


void DoorsExport::Export(string folder, string fileName)
{
	mSaver.SetFolder(folder);
	
	mFileName = fileName;
	mFolder = folder;

	ModelExporter & modelExporter = ModelExporter::GetExporter();
	modelExporter.ResetTimeline();

	MStatus stat;
	MFnRenderLayer doorsLayer;

	MItDependencyNodes itRenderLayers(MFn::kRenderLayer,&stat);
	err_code(stat);

	while (!itRenderLayers.isDone())
	{		
		MFnRenderLayer renderLayer( itRenderLayers.item() );

		if (renderLayer.name() == "doors")
		{
			stat = doorsLayer.setObject( renderLayer.object() );
			err_code(stat);
			break;
		}

		stat = itRenderLayers.next();
		err_code(stat);
	}

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

		bool objInLayer = doorsLayer.inLayer(obj, &stat);
		//err_code(stat);	

		if (!objInLayer)
		{
			stat = itDep.next();
			err_code(stat);	
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

		MString meshName = mesh.name(&stat);
		err_code(stat);


		vector<DoorEdge> doorEdges;
		int numOfQuads = mesh.numPolygons(&stat);
		for (int n=0;n<numOfQuads;n++)
		{
			MIntArray indices;
			mesh.getPolygonVertices(n,indices);
			if (indices.length() != 4)
			{
				err_stop(string("Doors mesh not quad: ") + meshName.asChar() );
				itDep.next();
				continue;
			}

			DoorEdge doorEdge;

			MPoint p0;
			MPoint p1;
			MPoint p2;
			MPoint p3;

			MVector polyNormal;
			stat = mesh.getPolygonNormal(n, polyNormal);
			err_code(stat);

			polyNormal.normalize();

			stat = mesh.getPoint(indices[0], p0);
			err_code(stat);
			stat = mesh.getPoint(indices[1], p1);
			err_code(stat);
			stat = mesh.getPoint(indices[2], p2);
			err_code(stat);
			stat = mesh.getPoint(indices[3], p3);
			err_code(stat);


			doorEdge.x1 = (float)p0.x;
			doorEdge.z1 = (float)p0.z;
			doorEdge.x2 = (float)p3.x;
			doorEdge.z2 = (float)p3.z;
			if (doorEdge.x1 == doorEdge.x2)
			{
				doorEdge.x1 = (float)p2.x;
				doorEdge.z1 = (float)p2.z;
			}

			doorEdge.nx = (float)polyNormal.x;
			doorEdge.ny = (float)polyNormal.y;
			doorEdge.nz = (float)polyNormal.z;
/*
			MVector v0(p0);
			MVector v1(p1);
			MVector v2(p2);
			MVector v3(p3);


			MVector cross = (v2 - v0) ^ (v1 - v0);
			stat = cross.normalize();
			err_code(stat);

			door.a = (float)cross.x;
			door.b = (float)cross.y;
			door.c = (float)cross.z;
			door.d = (float)-(door.a * v0.x + door.b * v0.y + door.c * v0.z);
*/

			doorEdges.push_back(doorEdge);

		}

		mDoors[meshName.asChar()] = doorEdges;
	
		stat = itDep.next();
		err_code(stat);
	}

	WriteDoors();

}


void DoorsExport::WriteDoors()
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	if (mDoors.size() == 0)
        return;
    
    mSaver.CreateFile(mFileName + ".dor");

	//Write number of meshes
	mSaver.writeInt(mDoors.size()); //!!!!!!!!!!!!!!!

	map<string , vector<DoorEdge> >::iterator doorIter = mDoors.begin();
	
	while( doorIter != mDoors.end())
	{
		mSaver.writeString(doorIter->first.c_str());//!!!!!!!!!!!!!!!

		vector<DoorEdge> & doorEdges = doorIter->second;
		mSaver.writeInt(doorEdges.size()); //!!!!!!!!!!!!!!!

		vector<DoorEdge>::iterator doorEdgeIter = doorEdges.begin();
		while (doorEdgeIter != doorEdges.end())
		{
			DoorEdge & door = *doorEdgeIter;

			//Write index and parent
			mSaver.writeFloat32(GLOBAL_SCALE*door.x1); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(GLOBAL_SCALE*door.z1); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(GLOBAL_SCALE*door.x2); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(GLOBAL_SCALE*door.z2); //!!!!!!!!!!!!!!!

			mSaver.writeFloat32(door.nx); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(door.ny); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(door.nz); //!!!!!!!!!!!!!!!

			doorEdgeIter++;
		}

		doorIter++;
	}
}




		

