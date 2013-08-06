#ifndef __WiresData__
#define __WiresData__

#include "Config.h"
#include "TL/Map.h"
#include "TL/MyString.h"


class ModelData;

struct WirePoint {
public:
	NEWDEL_DECL
	float x;
	float z;

	float tanx;
	float tanz;
};

struct Wire
{
public:
	NEWDEL_DECL
	Wire()
	{
	}
	virtual ~Wire()
	{
		delete [] wirePoints;
	}
	int numOfPoints;
	String name;
	WirePoint * wirePoints;
};

class WiresData
{
public:
	NEWDEL_DECL
	WiresData(void);
	virtual ~WiresData(void);

	inline int GetNumOfWires()
	{
		return mNumOfWires;
	}

	inline Wire * GetWire(int index)
	{
		return &mWires[index];
	}

	inline Wire * GetWire(String name)
	{
		if (mWiresTable.FindByKey(name.GetPtr()) == false )
			return NULL;

		int index = (int)mWiresTable.GetByKey( name.GetPtr() );
		return &mWires[index];
	}

	void AllocateWires(int numOfWires);

public:
	Map mWiresTable;

private:
	int mNumOfWires;
	Wire * mWires;
};


#endif //__WiresData__