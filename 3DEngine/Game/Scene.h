#ifndef __Scene__
#define __Scene__

#include "../Config.h"
#include "../Shell.h"
#include "../FaceMesh.h"
#include "../Camera.h"
#include "../CameraManager.h"
#include "../ResourceLoader.h"
#include "../PVRTexture.h"

#include "../Animation/AnimationPlayer.h"


class Scene
{
public:
	NEWDEL_DECL
	Scene(void);
	virtual ~Scene(void);

	virtual bool Load() = 0;
	virtual void Render() = 0;
};


#endif //__Scene__