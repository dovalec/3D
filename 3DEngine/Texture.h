#ifndef __Texture__
#define __Texture__


#include "Config.h"

#include "Image.h"
#include "Matrix.h"
#include "TextureBlendFunction.h"
#include "TextureFilterMode.h"
#include "TextureWrapMode.h"
#include "ImageFormat.h"


class  Texture
{
public:
	NEWDEL_DECL
	Image mImage;
	float mURepeat;
	float mVRepeat;
protected:
	int mTexObjID;

public:

	Texture();
	virtual ~Texture()
	{
	}

	TextureBlendFunction mTextureBlendFunction;
	TextureFilterMode mTextureFilterMode;
	TextureWrapMode mTextureWrapMode_T;
	TextureWrapMode mTextureWrapMode_S;

	bool mNeedUpdate;
private:

	bool mFirstRender;

public:


	virtual void Apply();
	virtual bool IsNeedUpdate();
	virtual void SetNeedUpdate(bool needUpdate);
	inline Image & GetImage()
	{
		return mImage;
	}
};

#endif //__Texture__


