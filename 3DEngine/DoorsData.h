#ifndef __DoorsData__
#define __DoorsData__

#include "Config.h"
#include "Door.h"
#include "TL/Map.h"

class DoorsData
{
public:
	NEWDEL_DECL
	DoorsData(void);
	virtual ~DoorsData(void);

	inline int GetNumOfDoors()
	{
		return mNumOfDoors;
	}

	inline Door * GetDoor(int index)
	{
		return &mDoors[index];
	}

	void AllocateDoors(int numOfDoors);

	Map  mDoorsTable;
	int mNumOfDoors;
	Door * mDoors;
};


#endif //__DoorsData__