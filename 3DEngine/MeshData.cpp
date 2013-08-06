
#include "MeshData.h"

MeshData::MeshData(void)
{
	index = -1;
	transformIndex = -1;
	materialIndex = -1;

	pPolygons = NULL;
	polygonsCount = 0;

	pIndices = NULL;
	indicesCount = 0;
}

MeshData::~MeshData(void)
{
	if (pPolygons)
		free(pPolygons);

	if (pIndices)
		free(pIndices);
}
