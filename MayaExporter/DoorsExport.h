#pragma once

#include "Saver.h"

struct DoorEdge {
	float x1;
	float z1;
	float x2;
	float z2;

	float nx;
	float ny;
	float nz;
};

class DoorsExport
{
private:
	DoorsExport(void);
	~DoorsExport(void);

public:
	static DoorsExport & GetExporter()
	{
		static DoorsExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	
	void WriteDoors( );

	Saver mSaver;
	string mFileName;
	string mFolder;

	map<string , vector<DoorEdge> > mDoors;

};
