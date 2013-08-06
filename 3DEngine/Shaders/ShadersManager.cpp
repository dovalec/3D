#include "ShadersManager.h"
#include "../Shell.h"

NEWDEL_IMPL(ShadersManager)
ShadersManager::ShadersManager()
{
	Shell * pShell = Shell::GetShell();
	bool lowGpu = pShell->GetLowGPU();
	if (lowGpu)
	{
		//LOGS("Creating shader LIGHT:OFF ");
		mShadersTable[GLSL_SHADER_USING_LIGHT] = new Shader();
		mShadersTable[GLSL_SHADER_NO_LIGHT] = new Shader();
		mShadersTable[GLSL_SHADER_SKIN_USING_LIGHT] = new ShaderWithSkin();
		mShadersTable[GLSL_SHADER_SKIN_NO_LIGHT] = new ShaderWithSkin();
		mShadersTable[GLSL_SHADER_USING_LIGHT_PHONG] = new Shader();
	}
	else
	{
		//LOGS("Creating shader LIGHT:ON");
		mShadersTable[GLSL_SHADER_USING_LIGHT] = new ShaderWithTwoLights();
		mShadersTable[GLSL_SHADER_NO_LIGHT] = new Shader();
		mShadersTable[GLSL_SHADER_SKIN_USING_LIGHT] = new ShaderWithLightAndSkin();
		mShadersTable[GLSL_SHADER_SKIN_NO_LIGHT] = new ShaderWithSkin();
		mShadersTable[GLSL_SHADER_USING_LIGHT_PHONG] = new ShaderWithLightPhong();
	}
}

ShadersManager::~ShadersManager()
{
	delete mShadersTable[GLSL_SHADER_USING_LIGHT];
	delete mShadersTable[GLSL_SHADER_NO_LIGHT];
	delete mShadersTable[GLSL_SHADER_SKIN_USING_LIGHT];
	delete mShadersTable[GLSL_SHADER_SKIN_NO_LIGHT];
	delete mShadersTable[GLSL_SHADER_USING_LIGHT_PHONG];
}

void ShadersManager::FreeDeviceResources()
{
	mShadersTable[GLSL_SHADER_USING_LIGHT]->FreeDeviceResources();
	mShadersTable[GLSL_SHADER_NO_LIGHT]->FreeDeviceResources();
	mShadersTable[GLSL_SHADER_SKIN_USING_LIGHT]->FreeDeviceResources();
	mShadersTable[GLSL_SHADER_SKIN_NO_LIGHT]->FreeDeviceResources();
	mShadersTable[GLSL_SHADER_USING_LIGHT_PHONG]->FreeDeviceResources();
}

void ShadersManager::ReInitDeviceResources()
{
	mShadersTable[GLSL_SHADER_USING_LIGHT]->ReInitDeviceResources();
	mShadersTable[GLSL_SHADER_NO_LIGHT]->ReInitDeviceResources();
	mShadersTable[GLSL_SHADER_SKIN_USING_LIGHT]->ReInitDeviceResources();
	mShadersTable[GLSL_SHADER_SKIN_NO_LIGHT]->ReInitDeviceResources();
	mShadersTable[GLSL_SHADER_USING_LIGHT_PHONG]->ReInitDeviceResources();
}

void ShadersManager::Destroy()
{
	delete mShadersTable[GLSL_SHADER_USING_LIGHT];
	delete mShadersTable[GLSL_SHADER_NO_LIGHT];
	delete mShadersTable[GLSL_SHADER_SKIN_USING_LIGHT];
	delete mShadersTable[GLSL_SHADER_SKIN_NO_LIGHT];
	delete mShadersTable[GLSL_SHADER_USING_LIGHT_PHONG];
}

Shader * ShadersManager::GetShader(int type)
{
	return mShadersTable[type];
}
