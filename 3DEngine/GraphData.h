#ifndef __GraphData__
#define __GraphData__

#include "Config.h"
#include "Plane.h"
#include "TL/Map.h"
#include "TL/MyString.h"

class GraphNode
{
public:
	NEWDEL_DECL;
	GraphNode(void);
	virtual ~GraphNode(void);
	void AllocateEdges(int numOfEdges);

	inline GraphNode ** GetEdges()
	{
		return mEdges;
	}

	inline int GetNumEdges()
	{
		return mNumOfEdges;
	}

	inline void SetTaken(bool taken)
	{
		mTaken = taken;
	}

	inline bool IsTaken()
	{
		return mTaken;
	}
	

public:
	int mID;
	bool mTaken;

	float x;
	float y;
	float z;

private:
	int mNumOfEdges;
	GraphNode ** mEdges;

};

/////////////////////////////////////////////////////////////////////

class GraphData
{
public:
	NEWDEL_DECL
	GraphData(void);
	virtual ~GraphData(void);

	inline int GetNumOfGraphNodes()
	{
		return mNumOfGraphNodes;
	}

	inline GraphNode * GetGraphNode(int index)
	{
		return &mGraphNodes[index];
	}

	void AllocateGraphNodes(int numOfGraphNodes);	

private:
	int mNumOfGraphNodes;
	GraphNode * mGraphNodes;
};

/////////////////////////////////////////////////////////////////////


class GraphMeshData
{
public:
	NEWDEL_DECL
	GraphMeshData(void);
	virtual ~GraphMeshData(void);

	inline int GetNumOfGraphData()
	{
		return mNumOfGraphData;
	}

	inline GraphData * GetGraphData(int index)
	{
		return &mGraphData[index];
	}

	inline GraphData * GetGraphData(String name)
	{
		if (mGraphDataTable.FindByKey(name.GetPtr()) == false )
			return NULL;

		int index = (int)mGraphDataTable.GetByKey( name.GetPtr() );
		return &mGraphData[index];
	}

	void AllocateGraphMesh(int numOfGraphData);

public:
	Map mGraphDataTable;

private:
	int mNumOfGraphData;
	GraphData * mGraphData;
};


#endif //__GraphData__