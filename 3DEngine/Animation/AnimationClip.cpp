#include "AnimationClip.h"
#include "../Node.h"

NEWDEL_IMPL(AnimationClip)
NEWDEL_IMPL(TransformAndKeys)

AnimationClip::AnimationClip(void)
{
	mNumOfTransforms = 0;
	mpTransformKeys = NULL;
	mDuration = 0.0f;
	mNumKeys = 0;

	mClipType = clip_transform;
}

AnimationClip::~AnimationClip(void)
{
	if (mpTransformKeys)
		delete(mpTransformKeys);
}

void AnimationClip::AllocateTransformKeys(int numOfTransforms)
{
	mNumOfTransforms = numOfTransforms;
	//mpTransformKeys = (TransformAndKeys*)HeapTlsf::GetTLSF().TLSF_NEW(sizeof(TransformAndKeys) * numOfTransforms);// new TransformAndKeys[numOfTransforms];
	mpTransformKeys = new TransformAndKeys[numOfTransforms];

}

TransformAndKeys & AnimationClip::GetTransformAndKeysByNum(int num)
{
	return mpTransformKeys[num];
}

