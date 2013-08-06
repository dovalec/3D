#ifndef __AnimationUVClip__
#define __AnimationUVClip__

#include "../Config.h"
#include "AnimationUVKeys.h"
#include "AnimationClip.h"

class Node;

struct VertexUVKeys
{
	NEWDEL_DECL
	int vertex;
	AnimationUVKeys keys;
};

class AnimationUVClip : public AnimationClip
{
public:
	NEWDEL_DECL
	AnimationUVClip(void);
	virtual ~AnimationUVClip(void);

	void AllocateVertexUVKeys(int numOfVertex);
	VertexUVKeys & GetVertexUVKeysByNum(int num);

	inline int GetNumOfVertices()
	{
		return mNumOfVertices;
	}

	void SetMeshIndex(int meshIndex);

	inline int GetMeshIndex()
	{
		return mMeshIndex;
	}

private:
	int mMeshIndex;
	int mNumOfVertices;
	VertexUVKeys * mpVertexUVKeys;
};


#endif // __AnimationUVClip__