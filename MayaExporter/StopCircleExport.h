#pragma once
#include "Saver.h"

struct StopCircle
{
	float x;
	float y;
	float z;
	float radius;
};

class StopCircleExport
{

private:
	StopCircleExport(void);
	~StopCircleExport(void);

public:

	static StopCircleExport & GetExporter()
	{
		static StopCircleExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	void WriteStopCircles(vector<StopCircle> & circlesVec);

	Saver & GetSaver()
	{
		return mSaver;
	}

	string mFileName;
	string mFolder;


private:
	Saver mSaver;

};
