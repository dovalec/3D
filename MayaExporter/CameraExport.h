#ifndef __CameraData_h__
#define __CameraData_h__

#include "stdafx.h"

class CameraData
{
public:
	CameraData(void);
	virtual ~CameraData(void);

public:
	string name;

	int index;
	int transformIndex;

	float aspectRatio;
	float nearClippingPlane;
	float farClippingPlane;
	float horizontalFieldOfView;
	float verticalFieldOfView;
	
};

class CameraExport
{
public:
	CameraExport(void);
	~CameraExport(void);

	void Export();
	
	void WriteCameras();

};


#endif //__CameraData_h__