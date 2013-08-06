#include "StdAfx.h"
#include "PhysicsExport.h"
#include "ModelExporter.h"



PhysicsExport::PhysicsExport(void)
{
}

PhysicsExport::~PhysicsExport(void)
{
}


void PhysicsExport::Export(string folder, string fileName)
{
	mSaver.SetFolder(folder);
	
	mFileName = fileName;
	mFolder = folder;

	ModelExporter & modelExporter = ModelExporter::GetExporter();
	modelExporter.ResetTimeline();

	MStatus stat;
/*
	MFnRenderLayer physicsLayer;

	MItDependencyNodes itRenderLayers(MFn::kRenderLayer,&stat);
	err_code(stat);

	while (!itRenderLayers.isDone())
	{		
		MFnRenderLayer renderLayer( itRenderLayers.item() );

		if (renderLayer.name() == "physics")
		{
			stat = physicsLayer.setObject( renderLayer.object() );
			err_code(stat);
			break;
		}

		stat = itRenderLayers.next();
		err_code(stat);
	}
*/

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
/*
		bool objInLayer = physicsLayer.inLayer(obj, &stat);
		//err_code(stat);	

		if (!objInLayer)
		{
			stat = itDep.next();
			err_code(stat);	
			continue;
		}
*/

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

		
		MPlug physics_plug = mesh.findPlug( "Physics" , &stat);
		if( stat != MS::kSuccess ) 
		{ 
			stat = itDep.next();
			continue;
		}

		int use_physics = 0;
		stat = physics_plug.getValue(use_physics);
		err_code(stat);

		if (!use_physics)
		{
			stat = itDep.next();
			continue;
		}

		PhysicsNode phyNode;


		MObject parentObj = mesh.parent(0,&stat);
		MFn::Type parentType = parentObj.apiType();		
		if ( modelExporter.CheckParentType( parentType) )
		{
			MFnDagNode parentDagNode(parentObj, &stat);
			MString transName = parentDagNode.name(&stat);

			phyNode.index  = modelExporter.mSceneTransformsTable[transName.asChar()];
		}
		else
		{
			err_stop("Bad parent of mesh");
		}

		MPlug move_plug = mesh.findPlug( "Move" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = move_plug.getValue(phyNode.move);
			err_code(stat);
		}
		else
		{
			err_stop(string("Move attribute not found in Mesh: ") + meshName.asChar() );
		}

		MPlug shape_plug = mesh.findPlug( "Shape" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = shape_plug.getValue(phyNode.shape);
			err_code(stat);
		}
		else
		{
			err_stop(string("Shape attribute not found in Mesh: ") + meshName.asChar() );
		}

		MPlug deactivation_plug = mesh.findPlug( "DeactivationTime" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = deactivation_plug.getValue(phyNode.deactivationTime);
			err_code(stat);
		}
		else
		{
			err_stop(string("DeactivationTime attribute not found in Mesh: ") + meshName.asChar() );
		}

		MPlug friction_plug = mesh.findPlug( "Friction" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = friction_plug.getValue(phyNode.friction);
			err_code(stat);
		}
		else
		{
			err_stop(string("Friction attribute not found in Mesh: ") + meshName.asChar() );
		}

		MPlug restitution_plug = mesh.findPlug( "Restitution" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = restitution_plug.getValue(phyNode.restitution);
			err_code(stat);
		}
		else
		{
			err_stop(string("Restitution attribute not found in Mesh: ") + meshName.asChar() );
		}

		MPlug mass_plug = mesh.findPlug( "Mass" , &stat);
		if( stat == MS::kSuccess ) 
		{ 
			stat = mass_plug.getValue(phyNode.mass);
			err_code(stat);
		}
		else
		{
			err_stop(string("Mass attribute not found in Mesh: ") + meshName.asChar() );
		}


		MBoundingBox bbox = mesh.boundingBox(&stat);	
		//bbox.transformUsing(mesh.transformationMatrix(&stat).inverse());
		err_code(stat);

		MPoint bbox_center(0,0,0);// = bbox.center();
	
		MPointArray vertexArray;
		stat = mesh.getPoints(vertexArray);
		err_code(stat);
		
		int numOfVtx = vertexArray.length();
		double min_x = 9999999;
		double min_y = 9999999;
		double min_z = 9999999;

		double max_x = -9999999;
		double max_y = -9999999;
		double max_z = -9999999;


		for (int n=0;n<numOfVtx;n++)
		{
			MPoint pnt = vertexArray[n];	
			if (pnt.x < min_x)
				min_x = pnt.x;
			
			if (pnt.y < min_y)
				min_y = pnt.y;

			if (pnt.z < min_z)
				min_z = pnt.z;

			if (pnt.x > max_x)
				max_x = pnt.x;
			
			if (pnt.y > max_y)
				max_y = pnt.y;

			if (pnt.z > max_z)
				max_z = pnt.z;


			bbox_center += pnt;

		}

		bbox_center.x /= (float)numOfVtx;
		bbox_center.y /= (float)numOfVtx;
		bbox_center.z /= (float)numOfVtx;

		double radius = 0.0f;
		for (int n=0;n<numOfVtx;n++)
		{
			MPoint vtx = vertexArray[n];			
			double len = (vtx - bbox_center).length();
			if (len>radius)
				radius = len;
		}


		
		phyNode.radius = (float)radius;
		phyNode.width = (float)(max_x - min_x);
		phyNode.height =(float)(max_y - min_y);
		phyNode.depth = (float)(max_z - min_z);

		mPhysicsNodes.push_back( phyNode );
	
		stat = itDep.next();
		err_code(stat);
	}

	WritePhysics();

}


void PhysicsExport::WritePhysics()
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	if (mPhysicsNodes.size() == 0)
        return;
    
	mSaver.CreateFile(mFileName + ".phy");
    
	mSaver.writeInt(mPhysicsNodes.size()); //!!!!!!!!!!!!!!!

	vector<PhysicsNode>::iterator phyIter = mPhysicsNodes.begin();
	while (phyIter != mPhysicsNodes.end())
	{
		PhysicsNode & phyNode = *phyIter;

		//Write index and parent
		mSaver.writeInt(phyNode.index); //!!!!!!!!!!!!!!!
		mSaver.writeInt(phyNode.move); //!!!!!!!!!!!!!!!
		mSaver.writeInt(phyNode.shape); //!!!!!!!!!!!!!!!
		mSaver.writeFloat32(phyNode.deactivationTime); //!!!!!!!!!!!!!!!
		mSaver.writeFloat32(phyNode.friction); //!!!!!!!!!!!!!!!
		mSaver.writeFloat32(phyNode.restitution); //!!!!!!!!!!!!!!!
		mSaver.writeFloat32(phyNode.mass); //!!!!!!!!!!!!!!!
		mSaver.writeFloat32(phyNode.radius); //!!!!!!!!!!!!!!!
		mSaver.writeFloat32(phyNode.width); //!!!!!!!!!!!!!!!
		mSaver.writeFloat32(phyNode.height); //!!!!!!!!!!!!!!!
		mSaver.writeFloat32(phyNode.depth); //!!!!!!!!!!!!!!!

		phyIter++;
	}


}




		

