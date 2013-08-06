#include "StdAfx.h"
#include "CollisionMeshExport.h"
#include "ModelExporter.h"



CollisionMeshExport::CollisionMeshExport(void)
{
}

CollisionMeshExport::~CollisionMeshExport(void)
{
}


void CollisionMeshExport::Export(string folder, string fileName)
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();
	modelExporter.ResetTimeline();

	MStatus stat;
	MFnRenderLayer collisionLayer;

	MItDependencyNodes itRenderLayers(MFn::kRenderLayer,&stat);
	err_code(stat);

	while (!itRenderLayers.isDone())
	{		
		MFnRenderLayer renderLayer( itRenderLayers.item() );

		if (renderLayer.name() == "collision_mesh")
		{
			stat = collisionLayer.setObject( renderLayer.object() );
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

		bool objInLayer = collisionLayer.inLayer(obj, &stat);
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

		MBoundingBox bbox = mesh.boundingBox(&stat);
		err_code(stat);

		MPoint bbox_center = bbox.center();
		double bbox_w = bbox.width();
		double bbox_h = bbox.height();
		double bbox_d = bbox.depth();

		double bbox_b = sqrt(bbox_w*bbox_w+bbox_h*bbox_h+bbox_d*bbox_d);
		bbox_center = bbox.center();

		
		CollisionMesh collMesh;
		collMesh.name = meshName.asChar();
		collMesh.radius = (float)bbox_b * 0.5f;

		int numOfQuads = mesh.numPolygons(&stat);
		for (int n=0;n<numOfQuads;n++)
		{
			MIntArray indices;
			mesh.getPolygonVertices(n,indices);
			if (indices.length() != 4)
			{
				err_stop(string("Collision mesh not quad: ") + meshName.asChar() );
				itDep.next();
				continue;
			}

			QuadPlane plane;
			
			MVector polyNormal;
			stat = mesh.getPolygonNormal(n, polyNormal);
			err_code(stat);

			polyNormal.normalize();

			MPoint pnt[4];
			for (int i=0;i<4;i++)
			{
				stat = mesh.getPoint(indices[i], pnt[i]);
				err_code(stat);
			}

			bool not_found = true;
			for (int i=1;i<4;i++)
			{
				if (pnt[0].y == pnt[i].y)
				{

					plane.x1 = (float)pnt[0].x;
					plane.z1 = (float)pnt[0].z;
					plane.x2 = (float)pnt[i].x;
					plane.z2 = (float)pnt[i].z;
					not_found = false;
					break;
				}
			}

			if (not_found)
			{
				err_stop("BAD COLLISION MESH!!!");
			}
			

			plane.nx = (float)polyNormal.x;
			plane.ny = (float)polyNormal.y;
			plane.nz = (float)polyNormal.z;
/*
			MVector v0(p0);
			MVector v1(p1);
			MVector v2(p2);
			MVector v3(p3);


			MVector cross = (v2 - v0) ^ (v1 - v0);
			stat = cross.normalize();
			err_code(stat);

			plane.a = (float)cross.x;
			plane.b = (float)cross.y;
			plane.c = (float)cross.z;
			plane.d = (float)-(plane.a * v0.x + plane.b * v0.y + plane.c * v0.z);
*/

			collMesh.quadPlanes.push_back(plane);

		}

		mCollMeshVec.push_back(collMesh);
		
		stat = itDep.next();
		err_code(stat);
	}

	if (mCollMeshVec.size())
	{		
		mSaver.SetFolder(folder);
		mSaver.CreateFile(fileName + ".clm");
		WriteCollisionMesh( );
	}
}


void CollisionMeshExport::WriteCollisionMesh( )
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	//Write number of meshes
	mSaver.writeInt(mCollMeshVec.size()); //!!!!!!!!!!!!!!!
	
	for (int c=0;c<mCollMeshVec.size();c++)
	{
		CollisionMesh & collMesh = mCollMeshVec[c];
		mSaver.writeString(collMesh.name.c_str()); //!!!!!!!!!!!!!!!
		mSaver.writeFloat32(GLOBAL_SCALE*collMesh.radius); //!!!!!!!!!!!!!!!

		vector<QuadPlane> & planes = collMesh.quadPlanes;
		//Write number of planes
		mSaver.writeInt(planes.size()); //!!!!!!!!!!!!!!!

		vector<QuadPlane>::iterator quadIter = planes.begin();
		while (quadIter != planes.end())
		{
			QuadPlane & plane = *quadIter;
			
			//Write index and parent
			mSaver.writeFloat32(GLOBAL_SCALE*plane.x1); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(GLOBAL_SCALE*plane.z1); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(GLOBAL_SCALE*plane.x2); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(GLOBAL_SCALE*plane.z2); //!!!!!!!!!!!!!!!

			mSaver.writeFloat32(plane.nx); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(plane.ny); //!!!!!!!!!!!!!!!
			mSaver.writeFloat32(plane.nz); //!!!!!!!!!!!!!!!

			quadIter++;
		}
	}
}




		

