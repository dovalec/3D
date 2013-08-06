#include "StdAfx.h"
#include "MaterialExport.h"
#include "MaterialData.h"
#include "TextureData.h"
#include "ModelExporter.h"
#include "Globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


MaterialExport::MaterialExport(void)
{
}

MaterialExport::~MaterialExport(void)
{
}

void MaterialExport::FillTextureData()
{

	ModelExporter & modelExporter = ModelExporter::GetExporter();


	MStatus stat;
	MItDependencyNodes itDep(MFn::kFileTexture,&stat);
	err_code(stat);

	while (!itDep.isDone()) 
	{
		MFnDependencyNode fnDep(itDep.item(),&stat);
		err_code(stat);

		MString cmd = MString("reference -q -f ") + fnDep.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDep.next();
			continue;
		}

		MPlug textureFilePlug = fnDep.findPlug("fileTextureName", &stat);
		err_code(stat);

		MString textureFileName;
		stat = textureFilePlug.getValue(textureFileName);
		err_code(stat);

		if ( modelExporter.mSceneTexturesTable.find(textureFileName.asChar()) != modelExporter.mSceneTexturesTable.end() )
		{
			itDep.next();
			continue;
		}

		TextureData * pTextureData = new TextureData();
		const char * pTextureName = fnDep.name(&stat).asChar();
		err_code(stat);

		//check for repeat UV			
		MPlug repeatUVPlug = fnDep.findPlug("repeatUV", &stat);
		err_code(stat);

		MPlugArray place2d;
		repeatUVPlug.connectedTo(place2d, true, false, &stat);
		err_code(stat);

		if (place2d.length())
		{
			MObject place2dObj = place2d[0].node(&stat);
			err_code(stat);
			MFnDependencyNode depPlace2d(place2dObj, &stat);
			err_code(stat);
			MString place2dName = depPlace2d.name(&stat);
			err_code(stat);

			MDoubleArray repeatUV;
			MString   cmd = MString("getAttr ") + MString( place2dName ) + MString(".repeatUV");
			stat = MGlobal::executeCommand( cmd, repeatUV );
			err_code(stat);

			pTextureData->uRepeat = (float)repeatUV[0];
			pTextureData->vRepeat = (float)repeatUV[1];		
		} 

		MImage mimage;
		stat = mimage.readFromFile(textureFileName.asChar());
		if( stat != MS::kSuccess ) 
		{ 
			err_stop(string("Texture not found: ") + textureFileName.asChar());
		}
		
		stat = mimage.getSize(pTextureData->width, pTextureData->height);
		err_code(stat);

		pTextureData->index = modelExporter.mTextureCount;
		pTextureData->textureFileName = textureFileName.asChar();

		modelExporter.mSceneTextures.push_back(pTextureData);
		modelExporter.mSceneTexturesTable[textureFileName.asChar()] = modelExporter.mTextureCount;
		modelExporter.mTextureCount++;

		itDep.next();
	}
}


void MaterialExport::ExportTextures()
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();
	MStatus stat;

    Saver texBatch;
    texBatch.AppendFile("export_models_textures.sh");
    
	int numSceneTextures = (int)modelExporter.mSceneTextures.size();
	for (int n=0;n<numSceneTextures;n++)
	{
		TextureData * pTextureData = modelExporter.mSceneTextures[n];

		string folder = pTextureData->textureFileName;
		string pvrFileName = pTextureData->textureFileName;
		
		pvrFileName.erase(0,pvrFileName.rfind('/')+1);
		pvrFileName.erase(pvrFileName.rfind('.'),pvrFileName.length() - pvrFileName.rfind('.'));

		folder.erase(folder.rfind('/'),folder.length() - folder.rfind('/'));
	
		pTextureData->pvrTextureFileName = pvrFileName;

		pvrFileName += ".pvr";


		string cmd;

		if (pTextureData->hasAlpha)
		{
			string cmd;
            
			cmd = string("PVRTexToolCL -m -fOGLPVRTC4 -i") + pTextureData->textureFileName + string(" -o") + folder + string("/sgx/") + pTextureData->pvrTextureFileName + "\n";
           
            texBatch.writeTxt(cmd.c_str());
            
			cmd = string("PVRTexToolCL -m -fOGL4444 -i") + pTextureData->textureFileName + string(" -o") + folder + string("/other/") + pTextureData->pvrTextureFileName + "\n";
           
			texBatch.writeTxt(cmd.c_str());
            
			cmd = string("PVRTexToolCL -m -fOGL4444 -x64 -y64 -i") + pTextureData->textureFileName + string(" -o") + folder + string("/other_low/") + pTextureData->pvrTextureFileName + "\n";
            
            texBatch.writeTxt(cmd.c_str());
            
		}
		else
		{
			cmd = string("PVRTexToolCL -m -fOGLPVRTC4 -i") + pTextureData->textureFileName + string(" -o") + folder + string("/sgx/") + pTextureData->pvrTextureFileName + "\n";
            
            texBatch.writeTxt(cmd.c_str());
            
			cmd = string("PVRTexToolCL -m -fOGL565 -i") + pTextureData->textureFileName + string(" -o") + folder + string("/other/") + pTextureData->pvrTextureFileName + "\n";
          
            texBatch.writeTxt(cmd.c_str());

			cmd = string("PVRTexToolCL -m -fOGL565 -x64 -y64 -i") + pTextureData->textureFileName + string(" -o") + folder + string("/other_low/") + pTextureData->pvrTextureFileName + "\n";
           
            texBatch.writeTxt(cmd.c_str());

		}

	}

}



void MaterialExport::Export()
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();


	FillTextureData();

	//Then export materials connected to textures:
	MStatus stat;
	MItDependencyNodes itDep(MFn::kLambert,&stat);
	err_code(stat);

	while (!itDep.isDone()) 
	{

		MFnLambertShader shaderGeneral(itDep.item(), &stat);
		err_code(stat);

		MString cmd = MString("reference -q -f ") + shaderGeneral.name();
		MString file_id;
		stat = MGlobal::executeCommand( cmd, file_id );
		if( stat == MS::kSuccess ) 
		{ 
			itDep.next();
			continue;
		}

		MaterialData * pMaterialData = new MaterialData();

		bool defaultNode = shaderGeneral.isDefaultNode(&stat);
		err_code(stat);

		// Beware !!!! we are not using default shaders
		/*	if ( defaultNode )
		{
		itDep.next();
		continue; 
		}
		*/

		const char * pShaderName = shaderGeneral.name().asChar();

		pMaterialData->index = modelExporter.mMaterialCount;
		modelExporter.mSceneMaterials.push_back(pMaterialData);
		modelExporter.mSceneMaterialsTable[pShaderName] = modelExporter.mMaterialCount;
		modelExporter.mMaterialCount++;

		MFn::Type shaderType = itDep.item().apiType();
		if ( shaderType == MFn::kLambert )
		{
			getImageForShader(shaderGeneral, pMaterialData );

			MFnLambertShader fnLambert( itDep.item(), &stat );
			err_code(stat);

			const char * lambertShaderName = fnLambert.name(&stat).asChar();
			// get attributes from lambert shader
			// not used right now ...

		}
		else
		{
			err_stop("Please use labmbert materials only");
		}

		itDep.next();
	}


	ExportTextures();
	WriteMaterials();

}

void MaterialExport::getImageForShader(MFnLambertShader & shaderGeneral, MaterialData * pMaterial )
{

	ModelExporter & modelExporter = ModelExporter::GetExporter();


	MStatus stat;

	MPlugArray plugs;
	MPlugArray alphaPlugs;

	MPlug texturePlug = shaderGeneral.findPlug("color", &stat);
	err_code(stat);

	MPlug alphaPlug = shaderGeneral.findPlug("transparency", &stat);
	err_code(stat);

	texturePlug.connectedTo(plugs,true,false,&stat);
	err_code(stat);

	alphaPlug.connectedTo(alphaPlugs,true,false,&stat);
	err_code(stat);

	if (alphaPlugs.length())
		pMaterial->hasAlpha = 1; //true

	//check for transparency texture connected to material: 
	for(int a=0;a!=alphaPlugs.length();++a)
	{
		// if file texture found
		if (alphaPlugs[a].node().apiType() == MFn::kFileTexture	) 
		{
			// bind a function set to it ....
			MFnDependencyNode fnDepAlpha(alphaPlugs[a].node(),&stat);
			err_code(stat);

			MPlug alphaTextureFilePlug = fnDepAlpha.findPlug("fileTextureName", &stat);
			err_code(stat);

			MString alphaTextureFileName;
			stat = alphaTextureFilePlug.getValue(alphaTextureFileName);
			err_code(stat);

		}
	}

	//check for texture connected to material:
	for(int i=0;i!=plugs.length();++i)
	{
		// if file texture found
		if (plugs[i].node().apiType() == MFn::kFileTexture	) 
		{
			// bind a function set to it ....
			MFnDependencyNode fnDep(plugs[i].node(),&stat);
			err_code(stat);

			MPlug textureFilePlug = fnDep.findPlug("fileTextureName", &stat);
			err_code(stat);

			MString textureFileName;
			stat = textureFilePlug.getValue(textureFileName);
			err_code(stat);

			pMaterial->textureIndex = modelExporter.mSceneTexturesTable[textureFileName.asChar()];
			modelExporter.mSceneTextures[pMaterial->textureIndex]->hasAlpha = pMaterial->hasAlpha;
		}
	}
}
void MaterialExport::WriteTextures()
{
	ModelExporter & modelExporter = ModelExporter::GetExporter();
	Saver & saver = modelExporter.GetSaver();

	//Write number of textures
	saver.writeInt(modelExporter.mSceneTextures.size());

	vector<TextureData*>::iterator textureIter = modelExporter.mSceneTextures.begin();
	while (textureIter != modelExporter.mSceneTextures.end())
	{
		TextureData * pTextData = *textureIter;

		saver.writeInt(pTextData->index);
		
		saver.writeString(pTextData->pvrTextureFileName.c_str());

		saver.writeInt(pTextData->width);
		saver.writeInt(pTextData->height);
		saver.writeInt(pTextData->hasAlpha);

		saver.writeFloat32(pTextData->uRepeat);
		saver.writeFloat32(pTextData->vRepeat);

		textureIter++;
	}
}

void MaterialExport::WriteMaterials()
{
	WriteTextures();


	ModelExporter & modelExporter = ModelExporter::GetExporter();
	Saver & saver = modelExporter.GetSaver();

	//Write number of materials
	saver.writeInt(modelExporter.mSceneMaterials.size());

	vector<MaterialData*>::iterator materialIter = modelExporter.mSceneMaterials.begin();
	while (materialIter != modelExporter.mSceneMaterials.end())
	{
		MaterialData * pMaterialData = *materialIter;

		saver.writeInt(pMaterialData->index);
		saver.writeInt(pMaterialData->textureIndex);
		saver.writeInt(pMaterialData->hasAlpha);
	
		materialIter++;
	}

}