#ifndef __Shader__
#define __Shader__

#include "../Config.h"
#include "../Matrix.h"

class Shader {
private:
	bool mOrthographic;
protected:
	char * mVertCode;
	char * mFragCode;
	static Shader * mLastShader;

	GLuint mUVMtx;
	GLuint mFragShader;
	GLuint mVertShader;		
	GLuint mProgramObject;

	GLuint mBindVertexAttrLocation;
	GLuint mBindNormalAttrLocation;
	GLuint mBindUVAttrLocation;

	GLint mViewProjMatrixLocation;

	bool mFragCompiled;
	bool mVertCompiled;


public:
	NEWDEL_DECL
	Shader();
	virtual ~Shader();

	virtual bool Init();
	virtual void UseProgram();
	inline void SetOrthographic()
	{
		mOrthographic = true;
	}

	GLuint GetBindVertexAttrLocation() { return mBindVertexAttrLocation; }
	GLuint GetBindNormalAttrLocation() { return mBindNormalAttrLocation; }
	GLuint GetBindUVAttrLocation() { return mBindUVAttrLocation; }

	void FreeDeviceResources();
	void ReInitDeviceResources();

};

#endif //__Shader__
