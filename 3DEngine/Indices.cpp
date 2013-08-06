#include "Indices.h"

NEWDEL_IMPL(Indices)
Indices::Indices() 
{
	mSubMeshCount = 0;
	mIndicesBuffer = NULL;
}

Indices::~Indices()
{
	//LOGI("Destroying Indices.");
	if (mIndicesBuffer)
		HeapTlsf::GetTLSF()->TLSF_DEL(mIndicesBuffer);

	ERR( glDeleteBuffers(1, &mVbo) );
}

void Indices::Allocate(int count) {

	mSize = count * sizeof(GLushort);
	mCount = count;
	mIndicesBuffer = (GLushort*)HeapTlsf::GetTLSF()->TLSF_NEW( mSize );

	// Generate the index buffer object (VBO)
	//!!!ERR( glGenBuffers(1, &mVbo) );

}
void Indices::FreeDeviceResources()
{
	ERR( glDeleteBuffers(1, &mVbo) );
}

void Indices::ReInitDeviceResources()
{
	ERR( glGenBuffers(1, &mVbo) );
	if (mVbo == 0)
	{
		STOP;
	}
	ERR( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbo) );
	ERR( glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSize, mIndicesBuffer, GL_STATIC_DRAW) );
	
}

void Indices::PutValues(GLushort * values) {
	memcpy(mIndicesBuffer, values, mSize);
}

void Indices::PutValuesPartial(GLushort * values, int place, int size) {
	memcpy( (char*)mIndicesBuffer + place, values, size);
}


void Indices::Apply()
{
	ERR( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbo) );
}
