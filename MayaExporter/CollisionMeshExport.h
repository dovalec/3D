#pragma once

#include "Saver.h"

struct QuadPlane {
	float x1;
	float z1;
	float x2;
	float z2;

	float nx;
	float ny;
	float nz;
};

struct CollisionMesh {
	string name;
	float  radius;
	vector<QuadPlane> quadPlanes;
};


class CollisionMeshExport
{
private:
	CollisionMeshExport(void);
	~CollisionMeshExport(void);

public:
	static CollisionMeshExport & GetExporter()
	{
		static CollisionMeshExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	
	void WriteCollisionMesh( );


	vector<CollisionMesh> mCollMeshVec;

	Saver mSaver;
};
