#pragma once

#include "Saver.h"


struct GraphNode {
	unsigned int id;

	double pos_x;
	double pos_y;
	double pos_z;

	unsigned int * graphNodes;
	double * distances;

	unsigned int numOfConnections;

};

struct GraphNodeData {
	GraphNode * pNode;
	std::vector<unsigned int> nodes;
	std::vector<float> dists;
};


struct GraphMesh {
	string name;
	std::vector<GraphNodeData> graphNodes;
};


class GraphExport
{
private:
	GraphExport(void);
	~GraphExport(void);

public:
	static GraphExport & GetExporter()
	{
		static GraphExport exporter;
		return exporter;
	}

	void Export(string folder, string fileName);
	
	void WriteGraphMesh( );


	vector<GraphMesh> mGrpMeshVec;

	Saver mSaver;
};
