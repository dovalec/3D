#ifndef __FaceMesh__
#define __FaceMesh__

#include "Config.h"

#include "Mesh.h"
#include "Vertices.h"
#include "Indices.h"
#include "Appearance.h"

class FaceMesh : public Mesh
{
public:
	NEWDEL_DECL
	FaceMesh();
	virtual void Render();
	virtual void Apply();
};

#endif //__FaceMesh__
