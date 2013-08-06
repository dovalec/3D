#ifndef __ShaderWithLightAndSkin__
#define __ShaderWithLightAndSkin__

#include "../Config.h"
#include "../Matrix.h"

#include "ShaderWithSkin.h"

class ShaderWithLightAndSkin : public ShaderWithSkin {
protected:
	GLuint mBindLightDirection1;
	GLuint mBindLightDirection2;
	GLuint mBindModelViewIT;


public:
	NEWDEL_DECL
	ShaderWithLightAndSkin();
	virtual bool Init();

	virtual void UseProgram();

	GLuint GetBindLightDirection1() { return mBindLightDirection1; }
	GLuint GetBindLightDirection2() { return mBindLightDirection2; }
	GLuint GetBindModelViewIT() { return mBindModelViewIT; }

};

#endif //__ShaderWithLightAndSkin__
