#ifndef __ShaderWithLight__
#define __ShaderWithLight__

#include "../Config.h"
#include "../Matrix.h"

#include "Shader.h"

class ShaderWithLight : public Shader {
protected:
	GLuint mBindLightDirection;
	GLuint mBindModelViewIT;
	
public:
	NEWDEL_DECL
	ShaderWithLight();
	virtual bool Init();
	void UseProgram();

	GLuint GetBindLightDirection() { return mBindLightDirection; }
	GLuint GetBindModelViewIT() { return mBindModelViewIT; }

};

#endif //__ShaderWithLight__
