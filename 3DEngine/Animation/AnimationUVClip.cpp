#include "AnimationUVClip.h"
#include "../Node.h"

NEWDEL_IMPL(AnimationUVClip)
NEWDEL_IMPL(VertexUVKeys)

AnimationUVClip::AnimationUVClip(void)
{
	mpVertexUVKeys = NULL;
	mDuration = 0.0f;
	mNumOfVertices = 0;
	mMeshIndex = -1;

	mClipType = clip_uv;
}

AnimationUVClip::~AnimationUVClip(void)
{
	if (mpVertexUVKeys)
		delete [] mpVertexUVKeys;
}

void AnimationUVClip::AllocateVertexUVKeys(int numOfVertices)
{
	mNumOfVertices = numOfVertices;
	mpVertexUVKeys = new VertexUVKeys[mNumOfVertices];
}

VertexUVKeys & AnimationUVClip::GetVertexUVKeysByNum(int num)
{
	return mpVertexUVKeys[num];
}


void AnimationUVClip::SetMeshIndex(int meshIndex)
{
	mMeshIndex = meshIndex;
}

