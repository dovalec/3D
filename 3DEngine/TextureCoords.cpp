#include "TextureCoords.h"

TextureCoords::TextureCoords()
{
	mVbo = (GLuint)-1;
	mUVBuffer = NULL;
	mSize = 0;
	mCount = 0;
}

void TextureCoords::Allocate(int count)
{
	mCount = count;
	mSize = count * 2 * sizeof(GLfloat);
	mUVBuffer = (float*)malloc(mSize);
	
	// Generate the uv buffer object (VBO)
	glGenBuffers(1, &mVbo);
}

void TextureCoords::PutValues(float * values)
{
	memcpy(mUVBuffer, values, mCount * 2 * sizeof(float));
}

void TextureCoords::Apply()
{

}
