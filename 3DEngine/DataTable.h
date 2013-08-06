#ifndef __DataTable__
#define __DataTable__

#include "Config.h"
#include "TL/Map.h"


class DataTable {
public:
	DataTable();
	virtual ~DataTable();

public:
	NEWDEL_DECL
	inline static DataTable & GetDataTable()
	{
		return *GlobalHeap::GetDataTable();		
	}

	void AddDataByName(const char * name, void * pData);
	void * GetDataByName(const char * name);
	

protected:
	Map mData;
};

#endif //__DataTable__
