#ifndef __ShaderWithTwoLights__
#define __ShaderWithTwoLights__

#include "../Config.h"
#include "../Matrix.h"

#include "Shader.h"

class ShaderWithTwoLights : public Shader {
protected:
	GLuint mBindLightDirection1;
	GLuint mBindLightDirection2;
	GLuint mBindModelViewIT;
	
public:
	NEWDEL_DECL
	ShaderWithTwoLights();
	virtual bool Init();
	void UseProgram();

	GLuint GetBindLightDirection1() { return mBindLightDirection1; }
	GLuint GetBindLightDirection2() { return mBindLightDirection2; }
	GLuint GetBindModelViewIT() { return mBindModelViewIT; }

};

#endif //__ShaderWithTwoLights__
