#include "GraphData.h"
#include "ModelData.h"

NEWDEL_IMPL(GraphNode)
NEWDEL_IMPL(GraphData)
NEWDEL_IMPL(GraphMeshData)


GraphNode::GraphNode(void)
{
	mEdges = NULL;
	mTaken = false;
}

GraphNode::~GraphNode(void)
{
	delete [] mEdges;
}

void GraphNode::AllocateEdges(int numOfEdges)
{
	mNumOfEdges = numOfEdges;
	mEdges = new GraphNode * [mNumOfEdges];
}


/////////////////////////////////////////////////////////////////////


GraphData::GraphData(void)
{
	mGraphNodes = NULL;
}

GraphData::~GraphData(void)
{
	delete [] mGraphNodes;
}

void GraphData::AllocateGraphNodes(int numOfGraphNodes)
{
	mGraphNodes = new GraphNode[numOfGraphNodes];
	mNumOfGraphNodes = numOfGraphNodes;
}




/////////////////////////////////////////////////////////////

GraphMeshData::GraphMeshData()
{
	mGraphData = NULL;
}

GraphMeshData::~GraphMeshData()
{
	delete [] mGraphData;
}


void GraphMeshData::AllocateGraphMesh(int numOfGraphData)
{
	mGraphData = new GraphData[numOfGraphData];
	mNumOfGraphData = numOfGraphData;
}
