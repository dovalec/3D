#pragma once
#include "Saver.h"

struct BotLaunchePoint
{
	float x;
	float y;
	float z;
	float radius;

	string name;
};

class BotLauncherExport
{

private:
	BotLauncherExport(void);
	~BotLauncherExport(void);

public:

	static BotLauncherExport & GetExporter()
	{
		static BotLauncherExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	void WriteBotLaunchePoints(vector<BotLaunchePoint> & circlesVec);

	Saver & GetSaver()
	{
		return mSaver;
	}

	string mFileName;
	string mFolder;


private:
	Saver mSaver;

};
