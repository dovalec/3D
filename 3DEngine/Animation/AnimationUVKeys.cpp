#include "AnimationUVKeys.h"

NEWDEL_IMPL(AnimationUVKeys)

AnimationUVKeys::AnimationUVKeys(void)
{
	mpKeys = NULL;
	mNumKeys = 0;
}

AnimationUVKeys::~AnimationUVKeys(void)
{
	HeapTlsf * tlsf = HeapTlsf::GetTLSF();
	if (mpKeys!=NULL)
		tlsf->TLSF_DEL(mpKeys);
}

void AnimationUVKeys::AllocateKeys()
{
	mpKeys = (UVKey*)HeapTlsf::GetTLSF()->TLSF_NEW( mNumKeys * sizeof(UVKey));
}


void AnimationUVKeys::PutKeys(UVKey * pKeys)
{
	int dataSize = mNumKeys * sizeof(UVKey);
	memcpy(mpKeys, pKeys, dataSize);
}
