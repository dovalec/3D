// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef __stdafx__
#define __stdafx__

#include "targetver.h"

#include <stdio.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>

using namespace std;

#include <OpenMayaMac.h>

#include <MTime.h> 
#include <MGlobal.h> 
#include <MFnDependencyNode.h>
#include <MDagPathArray.h>
#include <MFnTransform.h> 
#include <MItDependencyNodes.h>
#include <MDagPath.h>
#include <MVector.h>
#include <MStatus.h>
#include <MTime.h>
#include <MFileIO.h>
#include <MLibrary.h>
#include <MFnMesh.h>
#include <MFnCamera.h>
#include <MItMeshPolygon.h>
#include <MItGeometry.h>
#include <MFloatPointArray.h>

#include <MPlug.h>
#include <MFnLambertShader.h>
#include <MImage.h>
#include <MFnSkinCluster.h>
#include <MFnWeightGeometryFilter.h>

#include <MQuaternion.h>
#include <MFnIkJoint.h>


#include <MAnimControl.h>
#include <MFnAnimCurve.h>
#include <MFnCharacter.h>
#include <MFnClip.h>

#include <MFnNurbsCurve.h>
#include <MFnNurbsSurface.h>
#include <MBoundingBox.h>
#include <MFnRenderLayer.h>

#include <MPointArray.h>
#include <MMatrix.h>


void err_code(MStatus & stat);
void err_stop(string message);
void err_info(string message);
void err_print(string message);
void err_progress();

#define MAX_JOINTS_PER_FACE 12

#endif //__stdafx__
