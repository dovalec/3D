#pragma once
#include "Saver.h"

struct Circle
{
	float x;
	float y;
	float z;

	float qx;
	float qy;
	float qz;
	float qw;

	float sx;
	float sy;
	float sz;

	string name;
	string refName;
	float radius;
};

class LocatorExport
{

private:
	LocatorExport(void);
	~LocatorExport(void);

public:

	static LocatorExport & GetExporter()
	{
		static LocatorExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	void WriteCircles(vector<Circle> & circlesVec);

	Saver & GetSaver()
	{
		return mSaver;
	}

	string mFileName;
	string mFolder;


private:
	Saver mSaver;

};
