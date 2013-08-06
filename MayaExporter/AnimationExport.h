#pragma once

#include "Saver.h"

struct NodeAnimationData {
public:

	NodeAnimationData()
	{
		string name;
		foundRotation = 0;
		foundTranslation = 0;
		foundScale = 0;
		foundVisibility = 0;
	}

	string name;
	int foundRotation;
	int foundTranslation;
	int foundScale;
	int foundVisibility;

};

struct RotateKey
{
	float rx;
	float ry;
	float rz;
	float rw;
};


struct TranslateKey
{
	float tx;
	float ty;
	float tz;
};


struct ScaleKey
{
	float sx;
	float sy;
	float sz;
};

struct AnimKeys {
	vector<RotateKey> rotateKeys;
	vector<TranslateKey> translateKey;
	vector<ScaleKey> scaleKey;
	vector<int> visibilityKey;
};

struct Clip {
	// transform name , keys
	map<string, AnimKeys> animKeys;
	float duration;
};



class AnimationExport
{
private:
	AnimationExport(void);
	~AnimationExport(void);

public:
	static AnimationExport & GetExporter()
	{
		static AnimationExport exporter;
		return exporter;
	}

	string mFileName;
	string mFolder;

	void Export(string folder, string fileName);
	void ExportCharacter(MFnCharacter & character);
	void DisableAddClips();
	void SetTimeLinePos(MTime time);
	void ExportTransformAnim(map<string, NodeAnimationData> & transformTable, MFnClip & clipObj);
	void WriteAnimations();

	map<string, Clip> mClips;

};
