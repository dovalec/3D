#include "StdAfx.h"
#include "Transform.h"


Transform::Transform(void)
{
}

Transform::~Transform(void)
{
}

void Transform::Export()
{
	MStatus status;

	MTime time(0, MTime::kFilm);
	status = MGlobal::executeCommand(MString("currentTime -edit 0;"));
	err_code(status);

	status.error();

	vector<MDagPath> rootNodesDag;	
	MItDependencyNodes itDep(MFn::kTransform,&status);

	while (!itDep.isDone()) 
	{
	
		MObject obj = itDep.item();
		MFnTransform transform(obj, &status);
		err_code(status);

		unsigned int parentCount = transform.parentCount(&status);
		err_code(status);

		if (parentCount > 0)
		{
			MObject parentObj = transform.parent(0, &status);
			err_code(status);

			MFnTransform parentTransform(obj, &status);
			err_code(status);

			MString parentName = parentTransform.name(&status);
			//err_print(parentName);
		}
		

		if (parentCount == 0)
		{
			MDagPath dag = transform.dagPath(&status);
			err_code(status);

			rootNodesDag.push_back(dag);
		}

		itDep.next();
	}

	vector<MDagPath>::iterator iter = rootNodesDag.begin();
	while( iter != rootNodesDag.end() )
	{
		MDagPath dag = *iter;
		MObject obj = dag.node(&status);

		MFnTransform transform(obj, &status);
		err_code(status);

		WriteTransforms(transform);
	}

	

}


void Transform::WriteTransforms(MFnTransform & transform)
{
	MStatus status;
	MVector translate = transform.getTranslation(MSpace::kTransform, &status);
	err_code(status);

	double rx,ry,rz,rw;
	status = transform.getRotationQuaternion(rx,ry,rz,rw, MSpace::kTransform);
	err_code(status);

	
}
