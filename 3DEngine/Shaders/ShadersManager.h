#ifndef __ShadersManager__
#define __ShadersManager__

#include "../Config.h"

#include "Shader.h"
#include "ShaderWithLight.h"
#include "ShaderWithTwoLights.h"
#include "ShaderWithSkin.h"
#include "ShaderWithLightAndSkin.h"
#include "ShaderWithLightPhong.h"
#include "ShaderEnvMap.h"

#define GLSL_SHADER_USING_LIGHT 0
#define GLSL_SHADER_NO_LIGHT 1
#define GLSL_SHADER_SKIN_USING_LIGHT 2
#define GLSL_SHADER_SKIN_NO_LIGHT 3
#define GLSL_SHADER_USING_LIGHT_PHONG 4
#define GLSL_SHADER_USING_LIGHT_REFLECTION 5
#define GLSL_SHADER_USING_LIGHT_GLASS 6

class ShadersManager {
public:
	NEWDEL_DECL
	ShadersManager();
	virtual ~ShadersManager();
	
	inline static ShadersManager & GetShadersManager()
	{
		return *GlobalSys::GetShadersManager();		
	}

	Shader * GetShader(int type);
	void Destroy();
	
	void FreeDeviceResources();
	void ReInitDeviceResources();

protected:
	Shader * mShadersTable[16];
};	

#endif //__ShadersManager__
