#include "Texture.h"

NEWDEL_IMPL(Texture)
Texture::Texture() 
{
	mTextureFilterMode = TextureFilterModeLinear;
	mTextureWrapMode_T = TextureWrapModeClamp;
	mTextureWrapMode_S = TextureWrapModeClamp;
	
	mURepeat = 1.0f;
	mVRepeat = 1.0f;

	mFirstRender = true;
}

void Texture::Apply() {
	if (!mImage.IsReady()) {
		return;
	}

	if (mFirstRender) {
		GLuint texObjIDs[1] = {0};
		ERR( glGenTextures(1, texObjIDs) );
		mTexObjID = texObjIDs[0];
	}

	ERR( glActiveTexture(GL_TEXTURE0) );
	ERR( glBindTexture(GL_TEXTURE_2D, mTexObjID) );
	

	const int level = 0;

	if (mFirstRender) {
		if (mImage.mImageFormat == ImageFormatARGB_8888)
		{
			ERR( glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, mImage.mWidth,
			mImage.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			mImage.m_pBuffer) );
		}
		else if (mImage.mImageFormat == ImageFormatARGB_4444)
		{
			ERR( glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, mImage.mWidth,
			mImage.mHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4,
			mImage.m_pBuffer) );
		}
		else if (mImage.mImageFormat == ImageFormatRGB_565)
		{
			ERR( glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, mImage.mWidth,
			mImage.mHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5,
			mImage.m_pBuffer) );
		}
		else if (mImage.mImageFormat == ImageFormatALPHA_8)
		{
			ERR( glTexImage2D(GL_TEXTURE_2D, level, GL_ALPHA, mImage.mWidth,
			mImage.mHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
			mImage.m_pBuffer) );
		}
		mFirstRender = false;
	}
	else
	{
		ERR( glTexSubImage2D(GL_TEXTURE_2D, 0,0,0,mImage.mWidth, mImage.mHeight, GL_RGBA, GL_UNSIGNED_BYTE, mImage.m_pBuffer) );
	}


	if (mTextureWrapMode_S == TextureWrapModeRepeat)
	{
		ERR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
	}
	else if (mTextureWrapMode_S == TextureWrapModeClamp)
	{
		ERR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	}

	if (mTextureWrapMode_T == TextureWrapModeRepeat)
	{
		ERR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
	}
	else if (mTextureWrapMode_T == TextureWrapModeClamp)
	{
		ERR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE  ) );
	}

	

	if (mTextureFilterMode == TextureFilterModeLinear) {
		ERR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
		ERR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
	}

}

bool Texture::IsNeedUpdate() {
	return mNeedUpdate;
}

void Texture::SetNeedUpdate(bool needUpdate) {
	mNeedUpdate = needUpdate;
}

