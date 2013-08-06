#pragma once
#include "MaterialData.h"
#include "TextureData.h"

class MaterialExport
{
public:
	MaterialExport(void);
	~MaterialExport(void);

	void FillTextureData();
	void ExportTextures();
	void Export();

	void getImageForShader(MFnLambertShader & shaderGeneral, MaterialData * pMaterial );
	void WriteTextures();
	void WriteMaterials();
};
