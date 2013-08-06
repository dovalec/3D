#ifndef __AnimationUVKeys__
#define __AnimationUVKeys__

#include "../Config.h"

struct UVKey
{
public:
	NEWDEL_DECL
	float u;
	float v;
};

class AnimationUVKeys
{
public:
	NEWDEL_DECL
	AnimationUVKeys(void);
	virtual ~AnimationUVKeys(void);

	void AllocateKeys();
	
	void PutKeys(UVKey * pKeys);
	
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

	inline UVKey & GetKey(int n)
	{
		return mpKeys[n];
	}

private:
	unsigned int mNumKeys;
	UVKey * mpKeys;

};

#endif // __AnimationUVKeys__
