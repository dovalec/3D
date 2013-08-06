#include "StdAfx.h"
#include "TransformExport.h"
#include "ModelExporter.h"
#include "TransformData.h"
#include "Saver.h"
#include "Globals.h"

TransformExport::TransformExport(void)
{
}

TransformExport::~TransformExport(void)
{
}


void TransformExport::Export()
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();

	modelExporter.ResetTimeline();


	MStatus stat;
	MItDependencyNodes itDep(MFn::kTransform,&stat);


	while (!itDep.isDone()) 
	{
		MObject obj = itDep.item();
		MFnTransform transform(obj, &stat);
		err_code(stat);
		
		MString cmd = MString("reference -q -f ") + transform.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDep.next();
			continue;
		}

		MString transformName = transform.name(&stat);
		err_code(stat);
	
		unsigned int parentCount = transform.parentCount(&stat);
		err_code(stat);
		
		unsigned int childCount = transform.childCount(&stat);
		err_code(stat);

		bool doExport = false;
		if (childCount == 0)
		{
			doExport = true; // Tip JOINTS
		}
	
		for (unsigned int child = 0 ; child < childCount ; child++)
		{
			MObject childObj = transform.child(child,&stat);
			err_code(stat);

			MFn::Type childType = childObj.apiType();

			if ( modelExporter.CheckChildType(childType) )				
			{
				doExport = true;
				break;
			}
		}

		if (!doExport)
		{
			itDep.next();
			continue;
		}

		TransformData * pTransformData = new TransformData();
		pTransformData->name = transformName.asChar();

		MObject parentObj = transform.parent(0,&stat);
		MFn::Type parentType = parentObj.apiType();

		if ( modelExporter.CheckParentType(parentType) )
		{
			MFnDagNode parentDagNode(parentObj, &stat);
			MString parentName = parentDagNode.name(&stat);
		
			if (parentName.length() > 0)
				pTransformData->parentName = parentName.asChar();
		}

		if (parentType != MFn::kJoint && obj.apiType() == MFn::kJoint)
		{
			modelExporter.mSkeletonRoot = transform.name().asChar();
		}

		MVector translate = transform.getTranslation(MSpace::kTransform, &stat);
		err_code(stat);

		MVector pivot = transform.rotatePivotTranslation(MSpace::kTransform, &stat);
		err_code(stat);

		pTransformData->tx = (float)translate.x;
		pTransformData->ty = (float)translate.y;
		pTransformData->tz = (float)translate.z;

		pTransformData->px = (float)pivot.x;
		pTransformData->py = (float)pivot.y;
		pTransformData->pz = (float)pivot.z;

		//double rx,ry,rz,rw;
		MQuaternion quat;
		stat = transform.getRotation(quat);
		err_code(stat);

		if (transform.object().hasFn(MFn::kJoint))
		{
			err_code(stat);

			MFnIkJoint joint(transform.object(), &stat);
			err_code(stat);

			MQuaternion RO;
			MQuaternion R;
			MQuaternion JO;
			MQuaternion IS; // We dont have time for this.

			stat = joint.getScaleOrientation(RO);
			err_code(stat);
			stat = joint.getRotation(R); 
			err_code(stat);
			stat = joint.getOrientation(JO); 
			err_code(stat);
						
			quat = RO*R*JO;
		}

		//stat = transform.getRotationQuaternion(rx,ry,rz,rw, MSpace::kTransform);
		//err_code(stat);

		pTransformData->rx = -(float)quat.x;
		pTransformData->ry = -(float)quat.y;
		pTransformData->rz = -(float)quat.z;
		pTransformData->rw = (float)quat.w;

		double scale[3];
		stat = transform.getScale(scale);
		err_code(stat);

		pTransformData->sx = (float)scale[0];
		pTransformData->sy = (float)scale[1];
		pTransformData->sz = (float)scale[2];
		
		pTransformData->index = modelExporter.mTransformCount;
		modelExporter.mSceneTransforms.push_back(pTransformData);

	
		modelExporter.mSceneTransformsTable[transformName.asChar()] = modelExporter.mTransformCount;
		modelExporter.mTransformCount++;
		
		stat = itDep.next();
		err_code(stat);
	}

	
	vector<TransformData*>::iterator nodesIter = modelExporter.mSceneTransforms.begin();

	while (nodesIter != modelExporter.mSceneTransforms.end())
	{
		TransformData * pTransformData = *nodesIter;
		if (pTransformData->parentName.length() > 0)
			pTransformData->parent = modelExporter.mSceneTransformsTable[pTransformData->parentName];
		nodesIter++;
	}
	
	WriteTransforms();
}


void TransformExport::WriteTransforms()
{

	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	ModelExporter & modelExporter = ModelExporter::GetExporter();
	Saver & saver = modelExporter.GetSaver();

	//Write number of nodes
	saver.writeInt(modelExporter.mSceneTransforms.size());
	
	vector<TransformData*>::iterator nodesIter = modelExporter.mSceneTransforms.begin();
	while (nodesIter != modelExporter.mSceneTransforms.end())
	{
		TransformData * pTransformData = *nodesIter;

		//Write index and parent
		saver.writeInt(pTransformData->index);
		saver.writeInt(pTransformData->parent);
		saver.writeString(pTransformData->name.c_str());
		
		//Write floats
		saver.writeFloat32(GLOBAL_SCALE*pTransformData->tx);
		saver.writeFloat32(GLOBAL_SCALE*pTransformData->ty);
		saver.writeFloat32(GLOBAL_SCALE*pTransformData->tz);

		saver.writeFloat32(GLOBAL_SCALE*pTransformData->px);
		saver.writeFloat32(GLOBAL_SCALE*pTransformData->py);
		saver.writeFloat32(GLOBAL_SCALE*pTransformData->pz);

		saver.writeFloat32(pTransformData->rx);
		saver.writeFloat32(pTransformData->ry);
		saver.writeFloat32(pTransformData->rz);
		saver.writeFloat32(pTransformData->rw);

		saver.writeFloat32(pTransformData->sx);
		saver.writeFloat32(pTransformData->sy);
		saver.writeFloat32(pTransformData->sz);
		
		nodesIter++;
	}
}
