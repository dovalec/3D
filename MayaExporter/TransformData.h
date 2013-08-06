#pragma once


class TransformData
{

public:
	TransformData();
	~TransformData();


public:
	string name;
	string parentName;
	
	int index;
	int parent;

	float tx;
	float ty;
	float tz;

	float px;
	float py;
	float pz;

	float rx;
	float ry;
	float rz;
	float rw;

	float sx;
	float sy;
	float sz;
};