
#ifndef __TLSF_HEAP__
#define __TLSF_HEAP__

#define MAX_TLSF_POOLS 256

#include "../GlobalHeap.h"


#define USING_TLSF

class HeapTlsf
{

public:
	HeapTlsf(const char * userName);
	~HeapTlsf();
	
	inline static HeapTlsf * GetTLSF()
	{
        return GlobalSys::GetTLSF();
	}

	void * TLSF_NEW(unsigned int size);
	void TLSF_DEL(void * p );


	void TLSF_RELEASE();
	void TLSF_RESET();

	void TLSF_OFF();
	void TLSF_ON();

private:
	void TLSF_INIT();

private:
	bool mActive;
	char * mTlsfPools[MAX_TLSF_POOLS];
	unsigned int mTlsfPoolsMem[MAX_TLSF_POOLS];
	unsigned int mTlsfIndex;
	unsigned int mDefaultSize;
	unsigned int mNextDefaultSize;
	const char * mUserName;

public:
	unsigned int	mTotalSize;
	unsigned int mDelCount;
};

#ifdef USING_TLSF

    #define NEWDEL_DECL \
    void * operator	new(size_t size); \
    void * operator	new[](size_t size); \
    void * operator	new(size_t size, void * buffer); \
    void   operator	delete [](void * ptr); \
    void   operator	delete (void * ptr);

    #define NEWDEL_IMPL(cls) \
    void * cls::operator	new(size_t size) \
    { \
        return HeapTlsf::GetTLSF()->TLSF_NEW(size); \
    } \
    void * cls::operator	new(size_t size, void * buffer) \
    { \
        return HeapTlsf::GetTLSF()->TLSF_NEW(size); \
    } \
    void * cls::operator	new[](size_t size) \
    { \
        return HeapTlsf::GetTLSF()->TLSF_NEW(size); \
    } \
    void cls::operator delete(void * ptr) \
    { \
        HeapTlsf::GetTLSF()->TLSF_DEL(ptr); \
    } \
    void cls::operator delete[](void * ptr) \
    { \
        HeapTlsf::GetTLSF()->TLSF_DEL(ptr); \
    }

#else

    #define NEWDEL_DECL
    #define NEWDEL_IMPL(cls)

#endif

#endif //__TLSF_HEAP__