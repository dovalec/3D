#include "VerticesSkin.h"
#include "Shaders/ShaderWithSkin.h"

NEWDEL_IMPL(VerticesSkin)
VerticesSkin::VerticesSkin()
{
	mVerticesType = vertices_skin;
}


void VerticesSkin::Apply(Shader * pShader)
{
	ShaderWithSkin * pShaderSkin = (ShaderWithSkin*)pShader;
	ERR(glBindBuffer(GL_ARRAY_BUFFER, mVbo));

	// like parent:
	GLuint bindVertex = pShaderSkin->GetBindVertexAttrLocation();
	GLuint bindNormal = pShaderSkin->GetBindNormalAttrLocation();
	GLuint bindUV = pShaderSkin->GetBindUVAttrLocation();
	
	// for skin:
	GLuint bindBonesIndex = pShaderSkin->GetBindBoneIndexAttrLocation();
	GLuint bindWeightsIndex = pShaderSkin->GetBindBoneWeightsAttrLocation();

	ERR(glEnableVertexAttribArray(bindVertex));
	ERR(glVertexAttribPointer(bindVertex, 3, GL_FLOAT, GL_FALSE, mStride, (void*)0));

	if (bindNormal != -1)
	{
		ERR(glEnableVertexAttribArray(bindNormal));
		ERR(glVertexAttribPointer(bindNormal, 3, GL_FLOAT, GL_FALSE, mStride, (void*)(sizeof(GLfloat)*3)));
	}

	ERR(glEnableVertexAttribArray(bindUV));
	ERR(glVertexAttribPointer(bindUV, 2, GL_FLOAT, GL_FALSE, mStride, (void*)(sizeof(GLfloat)*6)));

	ERR(glEnableVertexAttribArray(bindWeightsIndex));
	//ERR( glVertexAttribPointer(bindWeightsIndex , 4 , GL_UNSIGNED_BYTE, GL_TRUE, mStride, (void*)(sizeof(GLfloat)*8 )));
	ERR( glVertexAttribPointer(bindWeightsIndex , 4 , GL_FLOAT, GL_FALSE, mStride, (void*)(sizeof(GLfloat)*8 )));

	ERR( glEnableVertexAttribArray(bindBonesIndex) );
	ERR( glVertexAttribPointer(bindBonesIndex, 4, GL_UNSIGNED_BYTE, GL_FALSE, mStride, (void*)(sizeof(GLfloat)*12)) );



}
