#ifndef __ShaderWithLightPhong__
#define __ShaderWithLightPhong__

#include "../Config.h"
#include "../Matrix.h"

#include "Shader.h"

class ShaderWithLightPhong : public Shader {
protected:
	GLuint mBindLightDirection;
	GLuint mBindLightDirection2;

	GLuint myModelView;
	GLuint myModelViewIT;
	GLuint myLightSel;
	GLuint myLightPosition;
	GLuint myLightDirection;
	GLuint myLightColor;

public:
	NEWDEL_DECL
	ShaderWithLightPhong();
	virtual bool Init();
	void UseProgram();

	GLuint GetBindModelView() { return myModelView; }
	GLuint GetBindModelViewIT() { return myModelViewIT; }
	GLuint GetBindLightSel() { return myLightSel; }
	GLuint GetBindLightPosition() { return myLightPosition; }
	GLuint GetBindLightDirection() { return myLightDirection; }
	GLuint GetBindLightColor() { return myLightColor; }

};

#endif //__ShaderWithLightPhong__
