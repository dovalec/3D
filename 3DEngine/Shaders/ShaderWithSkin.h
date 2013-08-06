#ifndef __ShaderWithSkin__
#define __ShaderWithSkin__

#include "../Config.h"
#include "../Matrix.h"

#include "Shader.h"

class ShaderWithSkin : public Shader {
protected:
	//attributes:
	GLuint mBindBoneIndex;
	GLuint mBindBoneWeights;

	//uniforms:
	//GLuint mBindBoneCount; 
	GLuint mBindBoneMatrixArray; 
	
public:
	NEWDEL_DECL
	ShaderWithSkin();
	virtual bool Init();
	virtual void UseProgram();

	GLuint GetBindBoneIndexAttrLocation() { return mBindBoneIndex; }
	GLuint GetBindBoneWeightsAttrLocation() { return mBindBoneWeights; }

	//GLuint GetBindBoneCount() { return mBindBoneCount; }
	GLuint GetBindBoneMatrixArray() { return mBindBoneMatrixArray; }

};

#endif //__ShaderWithSkin__
