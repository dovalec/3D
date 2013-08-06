#ifndef __AnimationKeys__
#define __AnimationKeys__

#include "../Config.h"

struct RotateKey
{
public:
	NEWDEL_DECL

	float rx;
	float ry;
	float rz;
	float rw;
};


struct TranslateKey
{
public:
	NEWDEL_DECL
	float tx;
	float ty;
	float tz;
};


struct ScaleKey
{
public:
	NEWDEL_DECL
	float sx;
	float sy;
	float sz;
};

struct AnimationKeys
{
public:
	NEWDEL_DECL
	AnimationKeys(void);
	~AnimationKeys(void);

	void AllocateRotation();
	void AllocateTranslation();
	void AllocateScale();
	void AllocateVisibility();

	void PutRotateKeys(RotateKey * pKeys);
	void PutTranslateKeys(TranslateKey * pKeys);
	void PutScaleKeys(ScaleKey * pKeys);
	void PutVisibilityKeys(int * pKeys);

	inline void SetNumKeys(int numKeys)
	{
		if (mNumKeys != 0 && mNumKeys!=numKeys)
		{
			LOGE("mNumKeys != 0 && mNumKeys!=numKeys");
			assert(0);
		}
		mNumKeys = numKeys;
	}

	inline unsigned int GetNumKeys()
	{
		return mNumKeys;
	}

	inline bool HasRotateKeys()
	{
		return mHasRotateKeys;
	}

	inline bool HasTranslateKeys()
	{
		return mHasTranslateKeys;
	}

	inline bool HasScaleKeys()
	{
		return mHasScaleKeys;
	}

	inline bool HasVisibilityKeys()
	{
		return mHasVisibilityKeys;
	}

	inline RotateKey GetRotateKey(int n)
	{
		return mpRotateKeys[n];
	}

	inline TranslateKey GetTranslateKey(int n)
	{
		return mpTranslateKeys[n];
	}

	inline ScaleKey GetScaleKey(int n)
	{
		return mpScaleKeys[n];
	}

	inline int GetVisibilityKey(int n)
	{
		return mpVisibilityKeys[n];
	}

private:
	unsigned int mNumKeys;
	
	bool mHasRotateKeys;
	bool mHasTranslateKeys;
	bool mHasScaleKeys;
	bool mHasVisibilityKeys;

	RotateKey * mpRotateKeys;
	TranslateKey * mpTranslateKeys;
	ScaleKey * mpScaleKeys;
	int * mpVisibilityKeys;

};

#endif // __AnimationKeys__
