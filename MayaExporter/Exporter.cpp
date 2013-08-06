// Exporter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModelExporter.h"
#include "AnimationExport.h"
#include "UVAnimationExport.h"
#include "LocatorExport.h"
#include "RefSceneExport.h"
#include "StopCircleExport.h"
#include "BotLauncherExport.h"
#include "CurveExport.h"
#include "WireExport.h"
#include "CollisionMeshExport.h"
#include "DoorsExport.h"
#include "PhysicsExport.h"
#include "GraphExport.h"

#include "Globals.h"

int main(int argc, char * argv[])
{

	MStatus stat;
    
	// initialise the maya library - This basically starts up Maya
	if(MLibrary::initialize(argv[0], true) != MS::kSuccess)
    {
		err_stop("[ERROR] Maya failed to initialise\n");
    }
	std::string fileName = argv[1];
    cout << "Loading file: " << fileName;
    
	//std::string scale = "0.01";
	std::string scale = "1.0";
	std::string todo = "all";
	std::string target = "tegra"; // or tegra

	if (argc > 2)
	{
		scale = argv[2];
	}

	if (argc > 3)
	{
		todo = argv[3];
	}

	if (argc > 4)
	{
		target = argv[4];
	}

	Globals & globalFlags = Globals::GetGlobals();
	globalFlags.TARGET = target;
	globalFlags.SCALE = (float)atof(scale.c_str());

	char txt[128]={0};
	sprintf(txt,"Scale: %f\n",globalFlags.SCALE);
	err_info(txt);
	

	for (unsigned int i=0;i<fileName.length();i++)
		if (fileName[i] == '\\')
			fileName[i] = '/';

    err_info("Opening file: " + fileName);
	stat = MFileIO::open(fileName.c_str());
	err_code(stat);

	int dot = fileName.rfind(".");
	int slash = fileName.rfind("/");

	string folder = fileName;
	folder.erase(slash, folder.length() - slash);
	
	fileName.erase(dot, fileName.length() - dot);
	fileName.erase(0, slash+1);

	if (todo == "model" || todo == "all")
	{
		ModelExporter & modelExport = ModelExporter::GetExporter();
		modelExport.Export(folder, fileName);
	
		AnimationExport & animationExport = AnimationExport::GetExporter();
		animationExport.Export(folder, fileName);

		UVAnimationExport & uvAnimationExport = UVAnimationExport::GetExporter();
		uvAnimationExport.Export(folder, fileName);
	}

	if (todo == "locator" || todo == "all")
	{
		LocatorExport & locatorExport = LocatorExport::GetExporter();
		locatorExport.Export(folder, fileName);
	}

	if (todo == "ref" || todo == "all")
	{
		RefSceneExport & refExport = RefSceneExport::GetExporter();
		refExport.Export(folder, fileName);
	}


	if (todo == "stop_circle" || todo == "all")
	{
		StopCircleExport & stopCircleExport = StopCircleExport::GetExporter();
		stopCircleExport.Export(folder, fileName);
	}

	if (todo == "launcher" || todo == "all")
	{
		BotLauncherExport & launcherExport = BotLauncherExport::GetExporter();
		launcherExport.Export(folder, fileName);
	}

	if (todo == "curve" || todo == "all")
	{
		CurveExport & curveExport = CurveExport::GetExporter();
		curveExport.Export(folder, fileName);
	}

	if (todo == "wire" || todo == "all")
	{
		WireExport & wireExport = WireExport::GetExporter();
		wireExport.Export(folder, fileName);
	}

	if (todo == "collision_mesh" || todo == "all")
	{
		CollisionMeshExport & cllisionMeshExport = CollisionMeshExport::GetExporter();
		cllisionMeshExport.Export(folder, fileName);
	}

	if (todo == "graph_mesh" || todo == "all")
	{
		GraphExport & graphExport = GraphExport::GetExporter();
		graphExport.Export(folder, fileName);
	}


	if (todo == "doors" || todo == "all")
	{
		DoorsExport & doorsExport = DoorsExport::GetExporter();
		doorsExport.Export(folder, fileName);
	}

	if (todo == "physics" || todo == "all")
	{
		PhysicsExport & physicsExport = PhysicsExport::GetExporter();
		physicsExport.Export(folder, fileName);
	}

	return 0;
}

