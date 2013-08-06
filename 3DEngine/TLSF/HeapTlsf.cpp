
#include "HeapTlsf.h"
#include "Tlsf.h"

#define DEFAULT_TLSF_SIZE 1024*1024 

HeapTlsf::HeapTlsf(const char * userName)
{
	mUserName = userName;
	mDefaultSize = DEFAULT_TLSF_SIZE;
	mNextDefaultSize = DEFAULT_TLSF_SIZE;
	
 
	mTotalSize = 0;
	for (unsigned int n=0;n<MAX_TLSF_POOLS;n++)
		mTlsfPools[n] = NULL;

	mTlsfIndex = 0;

	mActive = false;
}
HeapTlsf::~HeapTlsf()
{
	//TLSF_RELEASE();
}
void HeapTlsf::TLSF_OFF()
{
	mActive = false;
}

void HeapTlsf::TLSF_ON()
{
	mActive = true;
}

void * HeapTlsf::TLSF_NEW(unsigned int size)
{
	void * p = NULL;

	if (!mActive)
	{
		p = malloc(size);
		memset(p, 0, size);
		return p;
	}
	
	
	for (unsigned int n=0 ; n<mTlsfIndex ; n++)
	{
		p = malloc_ex (size + 4, mTlsfPools[n]);
		if (p!=NULL)
		{
			*((int*)p) = n;
			break;
		}
	}

	if (p==NULL)
	{		
		TLSF_INIT();

		p = malloc_ex (size + 4, mTlsfPools[mTlsfIndex]);
		if (p == NULL)
		{
			LOGE("Failed to allocate");
			STOP;
		}
		*((int*)p) = mTlsfIndex;
		mTlsfIndex++;
	}

	char * pData = (char*)p+4;
	memset(pData,0,size);


	return pData;
}



void HeapTlsf::TLSF_DEL(void * p )
{
	if (!mActive)
		return free(p);

	//free(p);
	//return;

	if (p == NULL)
		return;

	p = (char*)p - 4;
	unsigned int index  = *((int*)p);
	free_ex (p, mTlsfPools[index]);

}


void HeapTlsf::TLSF_INIT()
{
	if (mTlsfIndex >= MAX_TLSF_POOLS)
	{
		LOGE("No more TLSF POOLS!!!");
		STOP;
	}

	if (mTlsfPools[mTlsfIndex] == NULL)
	{
		mTotalSize += DEFAULT_TLSF_SIZE;

		mTlsfPools[mTlsfIndex] = (char*)malloc(DEFAULT_TLSF_SIZE);
		if (mTlsfPools[mTlsfIndex] == NULL)
		{
			LOGE("Failed to malloc in TLSF_INIT");
			STOP;
		}
		mTlsfPoolsMem[mTlsfIndex] = init_memory_pool(DEFAULT_TLSF_SIZE, mTlsfPools[mTlsfIndex]);
		LOGI("TLSF_INIT: %s (%x), Size: %d, Count: %d\n", mUserName, (unsigned int)this, DEFAULT_TLSF_SIZE, mTlsfIndex);
	}
	else
	{
		LOGE("Bad tlsf index");
		STOP;
	}

}

void HeapTlsf::TLSF_RELEASE()
{	

	if (mTotalSize > 0)
	{
		for (unsigned int n=0;n<mTlsfIndex;n++)
		{
			free(mTlsfPools[n]);
			mTlsfPools[n] = NULL;
			LOGI("Tlsf releasing pool: %d\n", n);
		}

		mTlsfIndex = 0;
		LOGI("TLSF_RELEASE: %d\n", mTotalSize);
	}
}


void HeapTlsf::TLSF_RESET()
{	
	if (mTotalSize > 0)
	{
		for (unsigned int n=0;n<mTlsfIndex;n++)
		{
			LOGI("Tlsf reset pool: %d\n", n);
			destroy_memory_pool(mTlsfPools[n]);	
			memset(mTlsfPools[n], 9, DEFAULT_TLSF_SIZE);
			mTlsfPoolsMem[n] = init_memory_pool( DEFAULT_TLSF_SIZE, mTlsfPools[n] );
		}
	}
}

