#include "Vertices.h"

NEWDEL_IMPL(Vertices)
Vertices::Vertices()
{
	mVbo = 0;
	mVerticesBuffer = NULL;
	mSize = 0;
	mCount = 0;

	mVerticesType = vertices_regular;
}

Vertices::~Vertices()
{
	//LOGI("Destroying Vertices.");
	if (mVerticesBuffer)
		HeapTlsf::GetTLSF()->TLSF_DEL(mVerticesBuffer);

	ERR( glDeleteBuffers(1, &mVbo) );
}

void Vertices::FreeDeviceResources()
{
	ERR( glDeleteBuffers(1, &mVbo) );
}

void Vertices::ReInitDeviceResources()
{
	ERR(glGenBuffers(1, &mVbo));
	ERR(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
	ERR(glBufferData(GL_ARRAY_BUFFER, mSize, mVerticesBuffer, GL_STATIC_DRAW));
}

void Vertices::UpdateVBO()
{
	ERR(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
	ERR(glBufferData(GL_ARRAY_BUFFER, mSize, mVerticesBuffer, GL_STATIC_DRAW));
}


void Vertices::Allocate(GLuint stride, GLuint count)
{
	mCount = count;
	mSize = count * stride;
	mStride = stride;
	mVerticesBuffer = HeapTlsf::GetTLSF()->TLSF_NEW(mSize);
}

void Vertices::PutValues(GLvoid * values)
{
	memcpy(mVerticesBuffer, values, mSize);
}


void Vertices::Apply(Shader * pShader)
{
	ERR(glBindBuffer(GL_ARRAY_BUFFER, mVbo));

	GLuint bindVertex = pShader->GetBindVertexAttrLocation();
	GLuint bindNormal = pShader->GetBindNormalAttrLocation();
	GLuint bindUV = pShader->GetBindUVAttrLocation();

	ERR(glEnableVertexAttribArray(bindVertex));
	ERR(glVertexAttribPointer(bindVertex, 3, GL_FLOAT, GL_FALSE, mStride, (void*)0));

	if (bindNormal != -1)
	{
		ERR(glEnableVertexAttribArray(bindNormal));
		ERR(glVertexAttribPointer(bindNormal, 3, GL_FLOAT, GL_FALSE, mStride, (void*)(sizeof(GLfloat)*3)));
	}

	ERR(glEnableVertexAttribArray(bindUV));
	ERR(glVertexAttribPointer(bindUV, 2, GL_FLOAT, GL_FALSE, mStride, (void*)(sizeof(GLfloat)*6)));

}
