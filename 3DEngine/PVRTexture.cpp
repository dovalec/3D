#include "PVRTexture.h"

PVRTexture * PVRTexture::spLastTexture = NULL;

NEWDEL_IMPL(PVRTexture)
PVRTexture::PVRTexture() 
{
	mTextureFilterMode = TextureFilterModeNearest;
	mTextureWrapMode_T = TextureWrapModeClamp;
	mTextureWrapMode_S = TextureWrapModeClamp;
}

PVRTexture::~PVRTexture()
{
	//LOGI("Destroying PVRTexture.");
	ERR( glDeleteTextures(1,&mTexObjID) );
}

void PVRTexture::FreeDeviceResources()
{
	ERR( glDeleteTextures(1,&mTexObjID) );
}

void PVRTexture::Apply() {
	
	if (spLastTexture != this)
		ERR( glBindTexture(GL_TEXTURE_2D, mTexObjID) );
	
	spLastTexture = this;

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

	
    if (mTextureFilterMode == TextureFilterModeNearest)
    {
        ERR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST) );
    }
    else
    {
        ERR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
    }
    
	ERR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );

}

