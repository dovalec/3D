#pragma once
#include "Saver.h"

struct RefNode
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

class RefSceneExport
{

private:
	RefSceneExport(void);
	~RefSceneExport(void);

public:

	static RefSceneExport & GetExporter()
	{
		static RefSceneExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	void WriteRefNodes(vector<RefNode> & circlesVec);

	Saver & GetSaver()
	{
		return mSaver;
	}

	string mFileName;
	string mFolder;


private:
	Saver mSaver;

};
