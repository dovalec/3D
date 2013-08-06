#include "AnimationKeys.h"

NEWDEL_IMPL(AnimationKeys)
NEWDEL_IMPL(RotateKey)
NEWDEL_IMPL(TranslateKey)
NEWDEL_IMPL(ScaleKey)
AnimationKeys::AnimationKeys(void)
{
	mpRotateKeys = NULL;
	mpTranslateKeys = NULL;
	mpScaleKeys = NULL;

	mHasRotateKeys = false;
	mHasTranslateKeys = false;
	mHasScaleKeys = false;
	mHasVisibilityKeys = false;

	mNumKeys = 0;

}

AnimationKeys::~AnimationKeys(void)
{
	HeapTlsf * tlsf = HeapTlsf::GetTLSF();
	if (mpRotateKeys!=NULL)
		tlsf->TLSF_DEL(mpRotateKeys);

	if (mpTranslateKeys!=NULL)
		tlsf->TLSF_DEL(mpTranslateKeys);

	if (mpScaleKeys!=NULL)
		tlsf->TLSF_DEL(mpScaleKeys);

	if (mpVisibilityKeys!=NULL)
		tlsf->TLSF_DEL(mpVisibilityKeys);

}

void AnimationKeys::AllocateRotation()
{
	//mpRotateKeys = (RotateKey*)HeapTlsf::GetTLSF()->TLSF_NEW( mNumKeys * sizeof(RotateKey));
	mpRotateKeys = new RotateKey[mNumKeys];
	mHasRotateKeys = true;
}

void AnimationKeys::AllocateTranslation()
{
	//mpTranslateKeys = (TranslateKey*)HeapTlsf::GetTLSF()->TLSF_NEW( mNumKeys * sizeof(TranslateKey));
	mpTranslateKeys = new TranslateKey[mNumKeys];
	mHasTranslateKeys = true;
}

void AnimationKeys::AllocateScale()
{
	//mpScaleKeys = (ScaleKey*)HeapTlsf::GetTLSF()->TLSF_NEW( mNumKeys * sizeof(ScaleKey));
	mpScaleKeys = new ScaleKey[mNumKeys];
	mHasScaleKeys = true;
}

void AnimationKeys::AllocateVisibility()
{
	mpVisibilityKeys = (int*)HeapTlsf::GetTLSF()->TLSF_NEW( mNumKeys * sizeof(int));
	mHasVisibilityKeys = true;
}

void AnimationKeys::PutRotateKeys(RotateKey * pKeys)
{
	int dataSize = mNumKeys * sizeof(RotateKey);
	memcpy(mpRotateKeys, pKeys, dataSize);
}

void AnimationKeys::PutTranslateKeys(TranslateKey * pKeys)
{
	int dataSize = mNumKeys * sizeof(TranslateKey);
	memcpy(mpTranslateKeys, pKeys, dataSize);
}

void AnimationKeys::PutScaleKeys(ScaleKey * pKeys)
{
	int dataSize = mNumKeys * sizeof(ScaleKey);
	memcpy(mpScaleKeys, pKeys, dataSize);
}

void AnimationKeys::PutVisibilityKeys(int * pKeys)
{
	int dataSize = mNumKeys * sizeof(int);
	memcpy(mpVisibilityKeys, pKeys, dataSize);
}