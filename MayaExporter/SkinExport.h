#pragma once

class FaceInfo
{
public:
	FaceInfo()
	{

		i0 = i1 = i2 = 0;
		n0 = n1 = n2 = 0;
		uv0 = uv1 = uv2 = 0;


		w0[0] = w0[1] = w0[2] = w0[3] = 0;
		w1[0] = w1[1] = w1[2] = w1[3] = 0;
		w2[0] = w2[1] = w2[2] = w2[3] = 0;

		t0[0] = t0[1] = t0[2] = t0[3] = 0;
		t1[0] = t1[1] = t1[2] = t1[3] = 0;
		t2[0] = t2[1] = t2[2] = t2[3] = 0;
		id = -1;
	}

	int id;
	int i0;
	int i1;
	int i2;

	int n0;
	int n1;
	int n2;

	int uv0;
	int uv1;
	int uv2;

	float w0[4];
	float w1[4];
	float w2[4];

	char t0[4];
	char t1[4];
	char t2[4];


};

class JointWeights
{
public:
	map<string, float> mWeights;
};

class MeshSkinData {
public:
	MeshSkinData()
	{
		
	}
	string meshName;

	//map<string, JointWeights > mJointsWeights;
	map<int, JointWeights> mIndicesJoints;
	map<list<string>, vector<FaceInfo> > mGroupedFaces;
	
};

class SkinExport
{
public:
	SkinExport(void);
	~SkinExport(void);

	void Export();
	void findJointsIndexInList(list<string> & jointsForFace, JointWeights & jointWeights, vector<int> & jointsIndex, vector<float> & weights);
};
