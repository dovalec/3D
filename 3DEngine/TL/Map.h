#ifndef __Map__
#define __Map__

#include "../Config.h"

class MapItem {
public:
	NEWDEL_DECL;
	MapItem()
	{
		Reset();
	}

	MapItem(void * ptr)
	{
		Reset();
		mPtr = ptr;		
	}

	inline void Reset()
	{
		mPtr = NULL;
		mNext = NULL;
		mPrevious = NULL;
		mIndexInPool = -1;
		mKey = -1;
	}

	inline void SetPtr(void * ptr)
	{
		mPtr = ptr;
	}

	inline MapItem * GetNext()
	{
		return mNext;
	}

	inline void SetNext(MapItem * pNext)
	{
		mNext = pNext;
	}

	inline MapItem * GetPrevious()
	{
		return mPrevious;
	}

	inline void SetPrevious(MapItem * pPrevious)
	{
		mPrevious = pPrevious;
	}


	inline void * GetPtr()
	{
		return mPtr;
	}

	inline void SetKey(int key)
	{
		mKey = key;
	}

	inline int GetKey()
	{
		return mKey;
	}
	
	inline int GetIndexInPool()
	{
		return mIndexInPool;
	}

	inline void SetIndexInPool(int poolIndex)
	{
		mIndexInPool = poolIndex;
	}


private:
	void * mPtr;
	int mIndexInPool;
	int mKey;
	MapItem * mNext;
	MapItem * mPrevious;
};

#define MAX_MAP_POOL_SIZE 20000

class MapItemPool{
public:
	NEWDEL_DECL;
	MapItemPool()
	{
		Reset();
	}

	virtual ~MapItemPool()
	{

	}

	void Reset()
	{
		mGetCount = 0;
		for (int n=0;n<MAX_MAP_POOL_SIZE;n++)
		{
			mMapItems[n] = &mMemory[n];
			mMapItems[n]->Reset();
		}
	}

	inline MapItem * Get()
	{
		if (mGetCount == MAX_MAP_POOL_SIZE)
		{
			LOGE("mGetCount == MAX_MAP_POOL_SIZE");
			STOP;
		}

		MapItem * pItem = mMapItems[mGetCount];
		pItem->SetIndexInPool(mGetCount);
		mGetCount++;
		return pItem;
	}

	
	inline void Del(MapItem * pItem)
	{
		int indexInPool = pItem->GetIndexInPool();
		mGetCount--;
		MapItem * pLastItem = mMapItems[mGetCount];
		
		pLastItem->SetIndexInPool(indexInPool);
		mMapItems[indexInPool] = pLastItem;
		pItem->Reset();
		mMapItems[mGetCount] = pItem;
	}

protected:
	int mGetCount;
	MapItem mMemory[MAX_MAP_POOL_SIZE];
	MapItem * mMapItems[MAX_MAP_POOL_SIZE];
};

class Map {

public:
	NEWDEL_DECL;
	Map()  
	{
		Reset();
	}
	~Map()  
	{
		RemoveAll();
		if (mArray)
			HeapTlsf::GetTLSF()->TLSF_DEL(mArray);
	}

	void Reset()
	{
		mArray = NULL;
		mSize = 0;
		mFirst = NULL;
		mLast = NULL;
		mDirty = false;
	}

	inline void * At(int index)
	{
		if (index >= mSize)
		{
			STOP;
			return NULL;
		}

		MapItem * pItem = mFirst;
		int count = 0;
		while (pItem)
		{
			if (count == index)
			{
				return pItem->GetPtr();
			}
			pItem = pItem->GetNext();
		}
        
        return NULL;
	}

	inline void * GetArray(int index)
	{
		if (mArray == NULL)
		{
			mArray = (int*)HeapTlsf::GetTLSF()->TLSF_NEW(mSize * sizeof(void*));
			MapItem * pItem = mFirst;		
			for (int n=0;n<mSize;n++)
			{
				*(mArray + n) = (int)pItem->GetPtr();					
				pItem = pItem->GetNext();			
			}

			mDirty = false;
		}
		else if (mArray && mDirty)
		{
			HeapTlsf::GetTLSF()->TLSF_DEL(mArray);
			mArray = (int*)HeapTlsf::GetTLSF()->TLSF_NEW(mSize * sizeof(void*));
			MapItem * pItem = mFirst;
			for (int n=0;n<mSize;n++)
			{
				*(mArray + n) = (int)pItem->GetPtr();					
				pItem = pItem->GetNext();				
			}

			mDirty = false;
		}

        if (index > mSize)
        {
            STOP;
        }
		return (void*)mArray[index];
	}

	inline void Add(const char * keyStr, void * ptr)
	{
		int key = ELFHash(keyStr);
		Add(key, ptr);
	}

	inline void Add(int key, void * ptr)
	{
		if (FindByKey(key))
		{
			STOP;
		}
		//MapItem * pNewItem = new MapItem(ptr);
		MapItem * pNewItem = mPool.Get();
		pNewItem->SetKey(key);
		pNewItem->SetPtr(ptr);

		if (mLast)
		{
			mLast->SetNext(pNewItem);
			pNewItem->SetPrevious(mLast);
			mLast = pNewItem;
		}
		else
		{
			mFirst = pNewItem;
			mLast = pNewItem;
		}
		mDirty = true;
		mSize++;
	}

	inline void * First()
	{
		if (mFirst == NULL)
			return NULL;

		return mFirst->GetPtr();
	}	

	inline void * Last()
	{
		if (mLast == NULL)
			return NULL;

		return mLast->GetPtr();
	}	

	inline MapItem * FirstItem()
	{	
		return mFirst;
	}	

	inline MapItem * LastItem()
	{
		return mLast;
	}	

	inline void RemoveFirst()
	{
		if (mFirst)
		{
			MapItem  * pTmp = mFirst;
			mFirst = mFirst->GetNext();
			//delete pTmp;
			mPool.Del(pTmp);
			mSize--;

			if (mFirst == NULL)
			{
				mLast = NULL;
			}
			else
			{
				mFirst->SetPrevious(NULL);
			}
			mDirty = true;

		}
	}

	inline void RemoveLast()
	{
		if (mLast)
		{
			MapItem  * pTmp = mLast;
			mLast = mLast->GetPrevious();
			//delete pTmp;
			mPool.Del(pTmp);
			mSize--;

			if (mLast == NULL)
			{
				mFirst = NULL;
			}
			mDirty = true;
		}
	}

	bool Find(void * ptr)
	{
		MapItem * pItem = mFirst;
		while (pItem)
		{
			if (pItem->GetPtr() == ptr)
				return true;

			pItem = pItem->GetNext();
		}
	
		return false;
	}

	bool FindByKey(const char * keyStr )
	{
		int key = ELFHash(keyStr);
		return FindByKey(key);
	}


	bool FindByKey(int key)
	{
		MapItem * pItem = mFirst;
		while (pItem)
		{
			if (pItem->GetKey() == key)
				return true;

			pItem = pItem->GetNext();
		}
	
		return false;
	}

	void * GetByKey(const char * keyStr )
	{
		int key = ELFHash(keyStr);
		return GetByKey(key);
	}

	void * GetByKey(int key)
	{
		MapItem * pItem = mFirst;
		while (pItem)
		{
			if (pItem->GetKey() == key)
				return pItem->GetPtr();

			pItem = pItem->GetNext();
		}
	
		return NULL;
	}

	bool RemoveByKey(const char * keyStr)
	{
		int key = ELFHash(keyStr);
		return RemoveByKey(key);
	}

	bool RemoveByKey(int key)
	{
		MapItem * pItem = mFirst;
		while (pItem)
		{
			if (pItem->GetKey() == key)
			{
				MapItem * pPrev = pItem->GetPrevious();
				MapItem * pNext = pItem->GetNext();
				if (pPrev)
				{
					pPrev->SetNext(pNext);			
				}
				if (pNext)
				{
					pNext->SetPrevious(pPrev);
				}
				if (pItem == mFirst)
				{
					mFirst = pNext;
				}
				if (pItem == mLast)
				{
					mLast = pPrev;
				}

				//delete pItem;
				mPool.Del(pItem);
				mSize--;
				mDirty = true;
				return true;
			}

			pItem = pItem->GetNext();
		}
	
		return false;
	}


	bool RemoveByValue(void * ptr)
	{
		MapItem * pItem = mFirst;
		while (pItem)
		{
			if (pItem->GetPtr() == ptr)
			{
				MapItem * pPrev = pItem->GetPrevious();
				MapItem * pNext = pItem->GetNext();
				if (pPrev)
				{
					pPrev->SetNext(pNext);			
				}
				if (pNext)
				{
					pNext->SetPrevious(pPrev);
				}
				if (pItem == mFirst)
				{
					mFirst = pNext;
				}
				if (pItem == mLast)
				{
					mLast = pPrev;
				}

				//delete pItem;
				mPool.Del(pItem);
				mSize--;
				mDirty = true;
				return true;
			}

			pItem = pItem->GetNext();
		}
	
		return false;
	}

	inline void RemoveAll()
	{ 
		while(mFirst)
			RemoveFirst();
	}

/*	inline void DeleteAll()
	{ 
		while(mFirst)
		{
			delete mFirst->GetPtr();
			RemoveFirst();
		}
	}
*/

	inline int Size()
	{
		return mSize;
	}

	static MapItemPool & GetPool()
	{
		return mPool;
	}

protected:
	static MapItemPool mPool;
	int mSize;
	int * mArray;
	bool mDirty;
	MapItem * mFirst;
	MapItem * mLast;
};

class MapIterator {

public:
	NEWDEL_DECL;
	MapIterator(Map & mylist) : mMap(mylist)
	{
		mItem = mMap.FirstItem();
	}

	inline void * GetPtr()
	{
		if (mItem == NULL)
			return NULL;

		void * ptr = mItem->GetPtr();
		mItem = mItem->GetNext();

		return ptr;
	}


private:
	Map & mMap;
	MapItem * mItem;
};

#endif //__Map__
