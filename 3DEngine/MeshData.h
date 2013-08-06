#ifndef __MeshData__
#define __MeshData__

#include "Config.h"


class MeshData
{
public:
	MeshData(void);
	~MeshData(void);

public:
	string name;

	int index;
	int transformIndex;
	int materialIndex;

	Triangle * pPolygons;
	int polygonsCount;

	unsigned short * pIndices;
	int indicesCount;

};

#endif //__MeshData__
