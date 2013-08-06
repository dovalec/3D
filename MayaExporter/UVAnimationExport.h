#pragma once

#include "Saver.h"

struct UVKey
{
public:
	float u;
	float v;
};

class UVAnimationExport
{
private:
	UVAnimationExport(void);
	~UVAnimationExport(void);

public:
	static UVAnimationExport & GetExporter()
	{
		static UVAnimationExport exporter;
		return exporter;
	}

	string mFileName;
	string mFolder;

	void Export(string folder, string fileName);
	void ExportAnimationCurve(string meshName, MFnAnimCurve & animCurve);
	void WriteAnimations();
	void SetTimeLinePos(MTime time);
	
	//map<[mesh name], map<[uvindex], map<[time],UVKey> >
	map<string, map<int, map<float,UVKey> > > mUVKeys;

};
