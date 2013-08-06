#include "Image.h"

NEWDEL_IMPL(Image)
Image::Image()
{
	mImageFormat = ImageFormatARGB_8888;
	m_pBuffer = NULL;
	mSize = 0;
	mHasAlpha = false;
}

void Image::Allocate(int size) {
	m_pBuffer = (GLubyte*)HeapTlsf::GetTLSF()->TLSF_NEW(size);
	mSize = size;
}

void Image::PutValues(GLubyte * values)
{
	memcpy(m_pBuffer, values, mSize); 
}

void Image::PutValuesDirect(GLubyte * values)
{
	m_pBuffer = values;
}

int Image::GetWidth() {
	return mWidth;
}

void Image::SetWidth(int width) {
	mWidth = width;
}

int Image::GetHeight() {
	return mHeight;
}

void Image::SetHeight(int height) {
	mHeight = height;
}

ImageFormat Image::GetImageFormat() {
	return mImageFormat;
}

void Image::SetImageFormat(ImageFormat imageFormat) {
	mImageFormat = imageFormat;
}


