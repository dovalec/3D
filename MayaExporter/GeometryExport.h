#pragma once

#include "SkinExport.h"

class GeometryExport
{
public:
	GeometryExport(void);
	~GeometryExport(void);

	void Export();
	
	int GetShaderIndex(MFnMesh & mesh);
	void WriteGeometries();

};
