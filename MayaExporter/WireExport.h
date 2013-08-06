#pragma once
#include "Saver.h"

struct WirePoint {
	float x;
	float z;

	float tx;
	float tz;
};

struct Wire
{
	string name;
	vector<WirePoint> wirePoints;
};

class WireExport
{

private:
	WireExport(void);
	~WireExport(void);

public:

	static WireExport & GetExporter()
	{
		static WireExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	void WriteWires(vector<Wire> & wires);

	Saver & GetSaver()
	{
		return mSaver;
	}

	string mFileName;
	string mFolder;


private:
	Saver mSaver;

};
