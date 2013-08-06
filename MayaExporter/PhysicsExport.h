#pragma once

#include "Saver.h"

struct PhysicsNode {
	int index;
	int move;
	int shape;
	float deactivationTime;
	float friction;
	float restitution;
	float mass;
	float radius;
	float width;
	float height;
	float depth;

};

class PhysicsExport
{
private:
	PhysicsExport(void);
	~PhysicsExport(void);

public:
	static PhysicsExport & GetExporter()
	{
		static PhysicsExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	
	void WritePhysics( );

	Saver mSaver;
	string mFileName;
	string mFolder;

	 vector<PhysicsNode> mPhysicsNodes;

};
