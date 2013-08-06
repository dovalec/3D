#include "StdAfx.h"
#include "MeshData.h"

MeshData::MeshData(void)
{
	layer = 0;
	glsl_shader = 0;
	alpha_mode = 0;
    display_mode = 0;
	use_depth = 1;
	use_back_face_culling = 1;
	use_occlusion_culling = 1;
    
	index = -1;
	transformIndex = -1;
	materialIndex = -1;
	
    
	pVertices = NULL;
	verticesCount = 0;
	radius = 0.0f;

}

MeshData::~MeshData(void)
{
	if (pVertices)
		free(pVertices);

	for (size_t n=0;n<mSubMeshIndices.size();n++)
	{
		free(mSubMeshIndices[n]);
	}
}
