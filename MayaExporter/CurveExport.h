#pragma once
#include "Saver.h"

struct CurvePoint {
	float x;
	float y;
	float z;

	float tx;
	float ty;
	float tz;

};

struct Curve
{
	string name;
	vector<CurvePoint> curvePoints;
};

class CurveExport
{

private:
	CurveExport(void);
	~CurveExport(void);

public:

	static CurveExport & GetExporter()
	{
		static CurveExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	void WriteCurves(vector<Curve> & curves);

	Saver & GetSaver()
	{
		return mSaver;
	}

	string mFileName;
	string mFolder;


private:
	Saver mSaver;

};
