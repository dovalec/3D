#ifndef __ShaderEnvMap__
#define __ShaderEnvMap__

#include "../Config.h"
#include "../Matrix.h"

#include "ShaderWithLight.h"

class ShaderEnvMap : public Shader {
protected:
	GLuint myModelWorld;
	GLuint myModelViewIT;
	GLuint myEyePosModel;
	GLuint myCubeReflection;
	GLuint my2DMap;
	GLuint myCubeMap;

public:
	NEWDEL_DECL
	ShaderEnvMap();
	virtual bool Init();
	void UseProgram();

	GLuint GetBindModelWorld() { return myModelWorld; }
	GLuint GetBindEyePosModel() { return myEyePosModel; }
	GLuint GetCubeReflection() { return myCubeReflection; }

};

#endif //__ShaderEnvMap__
