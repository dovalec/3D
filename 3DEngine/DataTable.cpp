#include "DataTable.h"

NEWDEL_IMPL(DataTable)
DataTable::DataTable()
{

}

DataTable::~DataTable()
{

}

void DataTable::AddDataByName(const char * name, void * pData)
{
	mData.Add(name, pData);
}

void * DataTable::GetDataByName(const char * name)
{
	return mData.GetByKey(name);
}






