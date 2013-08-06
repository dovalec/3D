#include "StdAfx.h"
#include "AnimationExport.h"
#include "ModelExporter.h"

AnimationExport::AnimationExport(void)
{
}

AnimationExport::~AnimationExport(void)
{
}


void AnimationExport::Export(string folder, string fileName)
{

	DisableAddClips();


	MStatus stat;
	MTime time(0, MTime::kFilm);
	stat = MGlobal::executeCommand(MString("currentTime -edit 0;"));
	err_code(stat);

	mFolder = folder;
	mFileName = fileName;

	MItDependencyNodes itDepChar(MFn::kCharacter,&stat);
	err_code(stat);

	// disable all clips
	while (!itDepChar.isDone())
	{
		// find root sub character;
		MObject obj = itDepChar.item();
		MFnCharacter character(obj, &stat);
		err_code(stat);

		MString cmd = MString("reference -q -f ") + character.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDepChar.next();
			continue;
		}

		ExportCharacter(character);

		stat = itDepChar.next();
		err_code(stat);
	}

	WriteAnimations();
}

void AnimationExport::DisableAddClips()
{
	MStatus stat;

	MItDependencyNodes itDepChar(MFn::kCharacter,&stat);
	err_code(stat);

	// disable all clips
	while (!itDepChar.isDone())
	{
		// find root sub character;
		MObject obj = itDepChar.item();
		MFnCharacter character(obj, &stat);
		err_code(stat);

		MString cmd = MString("reference -q -f ") + character.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDepChar.next();
			continue;
		}

		int numOfClips = character.getScheduledClipCount(&stat);
		err_code(stat);

		for (int clip = 0 ; clip < numOfClips ; clip++)
		{
			MObject obj = character.getScheduledClip(clip, &stat);
			MFnClip clipObj(obj, &stat);		
			err_code(stat);

			stat = clipObj.setEnabled(false);
			err_code(stat);
		}

		stat = itDepChar.next();
		err_code(stat);
	}

}

void AnimationExport::ExportCharacter(MFnCharacter & character)
{
	MStatus stat;
/*
	MSelectionList subCharacters;
	stat = character.getSubCharacters(subCharacters);
	err_code(stat);

	int numOfSub = subCharacters.length(&stat);
	err_code(stat);

	for (int sub = 0 ; sub<numOfSub;sub++)
	{
		MObject depNode;
		subCharacters.getDependNode(sub, depNode);
		MFnCharacter subChar(depNode, &stat);
		err_code(stat);

		ExportCharacter(subChar);
	}
*/

	err_info(string("Character: ") + character.name().asChar());

	int numOfClips = character.getScheduledClipCount(&stat);
	err_code(stat);

	for (int clip = 0 ; clip < numOfClips ; clip++)
	{
		MObject obj = character.getScheduledClip(clip, &stat);
		MFnClip clipObj(obj, &stat);		
		err_code(stat);

		stat = clipObj.setEnabled(true);
		err_code(stat);

		MObjectArray curves;
		MPlugArray attrPlugs;
		stat = clipObj.getMemberAnimCurves(curves, attrPlugs);
		err_code(stat);

		map<string, NodeAnimationData> nodesAnimInfo;
		for (unsigned int i = 0; i < curves.length(); ++i)
        {

			MPlug plug = attrPlugs[i];
			MString info = plug.info(&stat);
			err_code(stat);

			const char * pInfoStr = info.asChar();
			string nodeName(pInfoStr);
			string attrName(pInfoStr);
			nodeName.erase(nodeName.find('.'), nodeName.length() - nodeName.find('.') );
			attrName.erase(0,attrName.find('.')+1);
			
			if (nodesAnimInfo.find(nodeName) == nodesAnimInfo.end())
			{
				
				NodeAnimationData modeAnimData;
				modeAnimData.name = nodeName;			
				nodesAnimInfo[nodeName] = modeAnimData;
			}
	
			NodeAnimationData & bodeAnimData = nodesAnimInfo[nodeName];
			
			if (attrName == "translateX" || 
				attrName == "translateY" ||
				attrName == "translateZ")
			{
				bodeAnimData.foundTranslation = 1;
			}
			else if (attrName == "rotateX" || 
				attrName == "rotateY" ||
				attrName == "rotateZ")
			{
				bodeAnimData.foundRotation = 1;
			}
			else if (attrName == "scaleX" || 
				attrName == "scaleY" ||
				attrName == "scaleZ")
			{
				bodeAnimData.foundScale = 1;
			}
			else if (attrName == "visibility")
			{
				bodeAnimData.foundVisibility = 1;
			}
		}

		ExportTransformAnim(nodesAnimInfo,clipObj); 

		stat = clipObj.setEnabled(false);
		err_code(stat);
	}

}


void AnimationExport::ExportTransformAnim(map<string, NodeAnimationData> & transformTable, MFnClip & clipObj)
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();

	MStatus stat;

	MString clipName = clipObj.name(&stat);
	err_code(stat);

	err_info(string("\tClip: ") + clipName.asChar());

	mClips[clipName.asChar()] = Clip();
	Clip & clip = mClips[clipName.asChar()];

	float sourceStart;
	float sourceEnd;
	
	MPlug sourceStartPlug = clipObj.findPlug( "sourceStart" , &stat);
	if( stat == MS::kSuccess ) 
	{ 
		stat = sourceStartPlug.getValue(sourceStart);
		err_code(stat);
	} 

	MPlug sourceEndPlug = clipObj.findPlug( "sourceEnd" , &stat);
	if( stat == MS::kSuccess ) 
	{ 
		stat = sourceEndPlug.getValue(sourceEnd);
		err_code(stat);
	}


	MTime startFrame = clipObj.getStartFrame(&stat);
	err_code(stat);

	MTime start(sourceStart, MTime::kSeconds);
	err_code(stat);

	MTime end(sourceEnd, MTime::kSeconds);
	err_code(stat);

	MTime duration = end - start;
	clip.duration = (float)duration.as(MTime::kSeconds);
	
	MTime oneFrame(1.0 / 24.0 , MTime::kSeconds);
	while (start <= end)
	{
		SetTimeLinePos(startFrame);

		map<string, NodeAnimationData>::iterator nodesIter = transformTable.begin();
		while (nodesIter != transformTable.end())
		{
			string transformName = nodesIter->first;
			NodeAnimationData & nodeAnimData = nodesIter->second;

			if (clip.animKeys.find(transformName) == clip.animKeys.end())
			{
				clip.animKeys[transformName] = AnimKeys();
			}
			
			AnimKeys & keys = clip.animKeys[transformName];
			
			MString nodeNameToSelect(transformName.c_str());
			MSelectionList selected;
			stat = selected.add( nodeNameToSelect );
			err_code(stat);

			if (selected.length() != 1)
			{
				err_stop("Error is selecting nodes for animation");
			}
			
			MObject transObj;				
			stat = selected.getDependNode(0,transObj);
			err_code(stat);

			MFnTransform transform(transObj, &stat);
			err_code(stat);
			
			MPlug visPlug = transform.findPlug("visibility", &stat);
			err_code(stat);

			if (nodeAnimData.foundRotation)
			{
				RotateKey rotKey;
				//double rx,ry,rz,rw;
				MQuaternion quat;
				stat = transform.getRotation(quat);
				err_code(stat);

				if (transform.object().hasFn(MFn::kJoint))
				{
					err_code(stat);

					MFnIkJoint joint(transform.object(), &stat);
					err_code(stat);

					MQuaternion RO;
					MQuaternion R;
					MQuaternion JO;
					MQuaternion IS; // We dont have time for this.

					stat = joint.getScaleOrientation(RO);
					err_code(stat);
					stat = joint.getRotation(R); 
					err_code(stat);
					stat = joint.getOrientation(JO); 
					err_code(stat);
								
					quat = RO*R*JO;
				}

				rotKey.rx = -(float)quat.x;
				rotKey.ry = -(float)quat.y;
				rotKey.rz = -(float)quat.z;
				rotKey.rw = (float)quat.w;

				keys.rotateKeys.push_back(rotKey);

			}

			if (nodeAnimData.foundTranslation)
			{
				TranslateKey transKey;

				MVector translate = transform.getTranslation(MSpace::kTransform, &stat);
				err_code(stat);
			
				transKey.tx = (float)translate.x;
				transKey.ty = (float)translate.y;
				transKey.tz = (float)translate.z;

				keys.translateKey.push_back(transKey);
			}

			if (nodeAnimData.foundScale)
			{
				ScaleKey scaleKey;
				double scale[3];
				stat = transform.getScale(scale);
				err_code(stat);

				scaleKey.sx = (float)scale[0];
				scaleKey.sy = (float)scale[1];
				scaleKey.sz = (float)scale[2];

				keys.scaleKey.push_back(scaleKey);
			}		

			if (nodeAnimData.foundVisibility)
			{
                bool visible;
                stat = visPlug.getValue(visible);
				err_code(stat);               
				keys.visibilityKey.push_back( visible ? 1 : 0 );
			}	

			nodesIter++;
		}
		

		startFrame++;//= oneFrame;
		start += oneFrame;
	}
}

void AnimationExport::SetTimeLinePos(MTime time)
{
	MStatus stat;
	static char cmd[128] = {0};
	float t = (float)time.value();
	sprintf(cmd,"currentTime -edit %f;", t);
	stat = MGlobal::executeCommand(MString(cmd));
	err_code(stat);
}

void AnimationExport::WriteAnimations()
{
	float GLOBAL_SCALE = Globals::GetGlobals().SCALE;
	ModelExporter & modelExporter = ModelExporter::GetExporter();

	map<string, Clip>::iterator clipIter = mClips.begin();
	
	while(clipIter != mClips.end())
	{
		string clipName = clipIter->first;
		Clip & clip = clipIter->second;

		Saver saver;
		saver.SetFolder(mFolder);
		saver.CreateFile(mFileName + "_" + clipName + ".clp");
        
        

		map<string, AnimKeys>::iterator transformKeysIter = clip.animKeys.begin();
		
		int transformCount = clip.animKeys.size();

		//#####WRITE######
		saver.writeInt(transformCount);  // just for checking stream
		saver.writeFloat32(clip.duration);

		
		while (transformKeysIter != clip.animKeys.end())
		{
			string transformName = transformKeysIter->first;
			AnimKeys & keys = transformKeysIter->second;
			int transformIndex = modelExporter.mSceneTransformsTable[transformName];
			
			int numOfRotationKey = keys.rotateKeys.size();
			int numOfTranslateKey = keys.translateKey.size();
			int numOfScaleKey = keys.scaleKey.size();
			int numOfVisibilityKey = keys.visibilityKey.size();

			//#####WRITE######
			saver.writeInt(transformIndex); 
			
			//#####WRITE######
			saver.writeInt(numOfRotationKey);
			saver.writeInt(numOfTranslateKey);
			saver.writeInt(numOfScaleKey);
			saver.writeInt(numOfVisibilityKey);
			
			for (int n=0;n<numOfRotationKey;n++)
			{
				RotateKey & key = keys.rotateKeys[n];
				saver.writeFloat32(key.rx);
				saver.writeFloat32(key.ry);
				saver.writeFloat32(key.rz);
				saver.writeFloat32(key.rw);
			}

			for (int n=0;n<numOfTranslateKey;n++)
			{
				TranslateKey & key = keys.translateKey[n];
				saver.writeFloat32(GLOBAL_SCALE*key.tx);
				saver.writeFloat32(GLOBAL_SCALE*key.ty);
				saver.writeFloat32(GLOBAL_SCALE*key.tz);

			}

			for (int n=0;n<numOfScaleKey;n++)
			{
				ScaleKey & key = keys.scaleKey[n];
				saver.writeFloat32(key.sx);
				saver.writeFloat32(key.sy);
				saver.writeFloat32(key.sz);
			}

			for (int n=0;n<numOfVisibilityKey;n++)
			{				
				saver.writeInt(keys.visibilityKey[n]);			
			}

			transformCount++;
			transformKeysIter++;
		}

		clipIter++;
	}
}