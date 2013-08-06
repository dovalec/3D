#include "StdAfx.h"
#include "GraphExport.h"
#include "ModelExporter.h"



GraphExport::GraphExport(void)
{
}

GraphExport::~GraphExport(void)
{
}


void GraphExport::Export(string folder, string fileName)
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();
	modelExporter.ResetTimeline();

	MStatus stat;
	MFnRenderLayer graphLayer;

	MItDependencyNodes itRenderLayers(MFn::kRenderLayer,&stat);
	err_code(stat);

	while (!itRenderLayers.isDone())
	{		
		MFnRenderLayer renderLayer( itRenderLayers.item() );

		if (renderLayer.name() == "graph")
		{
			stat = graphLayer.setObject( renderLayer.object() );
			err_code(stat);
			break;
		}

		stat = itRenderLayers.next();
		err_code(stat);
	}

	MItDependencyNodes itDep(MFn::kMesh,&stat);


	while (!itDep.isDone()) 
	{
		MObject obj = itDep.item();
		MFnMesh mesh(obj, &stat);
		err_code(stat);

		if ( mesh.isIntermediateObject() )
		{
			itDep.next();
			continue;
		}

		bool objInLayer = graphLayer.inLayer(obj, &stat);
		//err_code(stat);	

		if (!objInLayer)
		{
			stat = itDep.next();
			err_code(stat);	
			continue;
		}

		MString cmd = MString("reference -q -f ") + mesh.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDep.next();
			continue;
		}

		MString meshName = mesh.name(&stat);
		err_code(stat);

		
		GraphMesh grpMesh;
		grpMesh.name = meshName.asChar();
		
		int numOfNodes = mesh.numVertices(&stat);
		err_code(stat);

		grpMesh.graphNodes.resize(numOfNodes);

		for (int n=0;n<numOfNodes;n++)
		{
			GraphNode * pGraphNode = new GraphNode();

			MPoint pos;
			stat = mesh.getPoint(n, pos, MSpace::kTransform);
			err_code(stat);


			pGraphNode->pos_x = pos.x;
			pGraphNode->pos_y = pos.y;
			pGraphNode->pos_z = pos.z;
			pGraphNode->id = n;
			grpMesh.graphNodes[n].pNode = pGraphNode;
		}


		int numOfEdges = mesh.numEdges(&stat);
		for (int n=0;n<numOfEdges ; n++)
		{
			int2 vertexList;
			mesh.getEdgeVertices(n, vertexList);

			grpMesh.graphNodes[vertexList[0]].nodes.push_back(vertexList[1]);
			grpMesh.graphNodes[vertexList[1]].nodes.push_back(vertexList[0]);
		}


		for (int n=0;n<numOfNodes;n++)
		{
			GraphNode * pGraphNode =  grpMesh.graphNodes[n].pNode;
			int numOfConn = grpMesh.graphNodes[n].nodes.size();

			MVector vec1( pGraphNode->pos_x, pGraphNode->pos_y,  pGraphNode->pos_z );

			pGraphNode->numOfConnections = numOfConn;
			pGraphNode->graphNodes = new unsigned int[numOfConn];
			pGraphNode->distances = new double[numOfConn];

			for (int i=0;i<numOfConn;i++)
			{	
				pGraphNode->graphNodes[i] = grpMesh.graphNodes[n].nodes[i];
				GraphNode * pConnGraphNode =  grpMesh.graphNodes[i].pNode;

				MVector vec2( pConnGraphNode->pos_x, pConnGraphNode->pos_y,  pConnGraphNode->pos_z ); 
				MVector distVec = vec2 - vec1;
				pGraphNode->distances[i] = distVec.length();
			}
		}


		
		mGrpMeshVec.push_back(grpMesh);
		
		stat = itDep.next();
		err_code(stat);
	}

	if (mGrpMeshVec.size())
	{		
		mSaver.SetFolder(folder);
		mSaver.CreateFile(fileName + ".grp");
		WriteGraphMesh( );
	}
}


void GraphExport::WriteGraphMesh( )
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;

	//Write number of meshes
	mSaver.writeInt(mGrpMeshVec.size()); //!!!!!!!!!!!!!!!
	
	for (int c=0;c<mGrpMeshVec.size();c++)
	{
		GraphMesh & grpMesh = mGrpMeshVec[c];
		mSaver.writeString(grpMesh.name.c_str()); //!!!!!!!!!!!!!!!
		

		int numOfGraphNodes = grpMesh.graphNodes.size();
		mSaver.writeUInt( numOfGraphNodes);

		for (int n=0;n<numOfGraphNodes ; n++)
		{
			GraphNodeData & node = grpMesh.graphNodes[n];
			GraphNode * pNode = node.pNode;
			
			mSaver.writeUInt( pNode->id );
			mSaver.writeFloat32( (float)pNode->pos_x * GLOBAL_SCALE);
			mSaver.writeFloat32( (float)pNode->pos_y * GLOBAL_SCALE );
			mSaver.writeFloat32( (float)pNode->pos_z * GLOBAL_SCALE );
			mSaver.writeUInt( pNode->numOfConnections );

			for(unsigned int n = 0; n < pNode->numOfConnections; n++)
				mSaver.writeUInt( pNode->graphNodes[n] );
		   
			//for(unsigned int n = 0; n < pNode->numOfConnections; n++)
			//	mSaver.writeFloat32( (float)pNode->distances[n] * GLOBAL_SCALE );
	   
		}

	}
}




		

