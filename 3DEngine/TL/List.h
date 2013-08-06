#ifndef __List__
#define __List__

#include "../Config.h"

class ListItem {
public:
	NEWDEL_DECL;
	ListItem()
	{
		Reset();
	}

	ListItem(void * ptr)
	{
		Reset();		
	}

	inline void Reset()
	{
		mPtr = NULL;
		mNext = NULL;
		mPrevious = NULL;
		mIndexInPool = -1;
	}

	inline void SetPtr(void * ptr)
	{
		mPtr = ptr;
	}

	inline ListItem * GetNext()
	{
		return mNext;
	}

	inline void SetNext(ListItem * pNext)
	{
		mNext = pNext;
	}

	inline ListItem * GetPrevious()
	{
		return mPrevious;
	}

	inline void SetPrevious(ListItem * pPrevious)
	{
		mPrevious = pPrevious;
	}


	inline void * GetPtr()
	{
		return mPtr;
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
	ListItem * mNext;
	ListItem * mPrevious;
};

#define MAX_LIST_POOL_SIZE 40000

class ListItemPool{
public:
	NEWDEL_DECL;
	ListItemPool()
	{
		Reset();
	}

	virtual ~ListItemPool()
	{

	}

	void Reset()
	{
		mGetCount = 0;
		for (int n=0;n<MAX_LIST_POOL_SIZE;n++)
		{
			mListItems[n] = &mMemory[n];
			mListItems[n]->Reset();
		}
	}

	inline ListItem * Get()
	{
		if (mGetCount == MAX_LIST_POOL_SIZE)
		{
			LOGE("mGetCount == MAX_LIST_POOL_SIZE");
			STOP;
		}

		ListItem * pItem = mListItems[mGetCount];
		pItem->SetIndexInPool(mGetCount);
		mGetCount++;
		return pItem;
	}

	
	inline void Del(ListItem * pItem)
	{
		int indexInPool = pItem->GetIndexInPool();
		mGetCount--;
		ListItem * pLastItem = mListItems[mGetCount];
		
		pLastItem->SetIndexInPool(indexInPool);
		mListItems[indexInPool] = pLastItem;
		pItem->Reset();
		mListItems[mGetCount] = pItem;
	}

protected:
	int mGetCount;
	ListItem mMemory[MAX_LIST_POOL_SIZE];
	ListItem * mListItems[MAX_LIST_POOL_SIZE];
};

class List {

public:
	NEWDEL_DECL;
	List()  
	{
		Reset();
	}
	~List()  
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

		ListItem * pItem = mFirst;
		int count = 0;
		while (pItem)
		{
			if (count == index)
			{
				return pItem->GetPtr();
			}
			pItem = pItem->GetNext();
		}
	}

	inline void * GetArray(int index)
	{
		if (mArray == NULL)
		{
			mArray = (int*)HeapTlsf::GetTLSF()->TLSF_NEW(mSize * sizeof(void*));
			ListItem * pItem = mFirst;		
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
			ListItem * pItem = mFirst;
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

	inline void Add(void * ptr)
	{
		//ListItem * pNewItem = new ListItem(ptr);
		ListItem * pNewItem = mPool.Get();
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

	inline void AddOptimized(void * ptr, ListItem * pNewItem)
	{
		pNewItem->Reset();
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

	inline ListItem * FirstItem()
	{	
		return mFirst;
	}	

	inline ListItem * LastItem()
	{
		return mLast;
	}	

	inline void RemoveFirst()
	{
		if (mFirst)
		{
			ListItem  * pTmp = mFirst;
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

	inline ListItem * RemoveFirstOptimized()
	{
		if (!mFirst)
			return NULL;
		
		ListItem  * pTmp = mFirst;
		mFirst = mFirst->GetNext();			
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
		pTmp->Reset();
		return pTmp;
	}


	inline void RemoveLast()
	{
		if (mLast)
		{
			ListItem  * pTmp = mLast;
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
		ListItem * pItem = mFirst;
		while (pItem)
		{
			if (pItem->GetPtr() == ptr)
				return true;

			pItem = pItem->GetNext();
		}
	
		return false;
	}


	
	bool Remove(void * ptr)
	{
		ListItem * pItem = mFirst;
		while (pItem)
		{
			if (pItem->GetPtr() == ptr)
			{
				ListItem * pPrev = pItem->GetPrevious();
				ListItem * pNext = pItem->GetNext();
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

	bool RemoveOptimized(ListItem * pItem)
	{			
		ListItem * pPrev = pItem->GetPrevious();
		ListItem * pNext = pItem->GetNext();
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

		mSize--;
		mDirty = true;
		return true;
		
	}

	inline void RemoveAll()
	{ 
		while(mFirst)
			RemoveFirst();
	}
/*
	inline void DeleteAll()
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

	static ListItemPool & GetPool()
	{
		return mPool;
	}

protected:
	static ListItemPool mPool;
	int mSize;
	int * mArray;
	bool mDirty;
	ListItem * mFirst;
	ListItem * mLast;
};

class ListIterator {

public:
	NEWDEL_DECL;
	ListIterator(List & mylist) : mList(mylist)
	{
		mItem = mList.FirstItem();
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
	List & mList;
	ListItem * mItem;
};

#endif //__List__
