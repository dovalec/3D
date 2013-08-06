#ifndef __PVRTexture__
#define __PVRTexture__


#include "Config.h"

#include "Image.h"
#include "Matrix.h"
#include "TextureBlendFunction.h"
#include "TextureFilterMode.h"
#include "TextureWrapMode.h"
#include "ImageFormat.h"
#include "Texture.h"


class PVRTexture : public Texture
{
protected:
	GLuint mTexObjID;

public:
	NEWDEL_DECL
	PVRTexture();
	virtual ~PVRTexture();

	void FreeDeviceResources();
	void SetTextureObject(GLuint id)
	{
		mTexObjID = id;
	}

	GLuint GetTextureObject()
	{
		return mTexObjID;
	}	


	TextureBlendFunction mTextureBlendFunction;
	TextureFilterMode mTextureFilterMode;
	TextureWrapMode mTextureWrapMode_T;
	TextureWrapMode mTextureWrapMode_S;

private:
	static PVRTexture * spLastTexture;
public:
	virtual void Apply();
};

#endif //__PVRTexture__


