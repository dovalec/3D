#ifndef __OverlayFace__
#define __OverlayFace__

#include "Config.h"

#include "Mesh.h"
#include "Vertices.h"
#include "Indices.h"
#include "Appearance.h"

class OverlayFace : public Mesh
{
public:
	NEWDEL_DECL
	OverlayFace();
	virtual ~OverlayFace();

	virtual void Render();
	virtual void Apply();

};

#endif //__OverlayFace__
