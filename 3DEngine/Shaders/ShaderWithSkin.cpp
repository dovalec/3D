#include "ShaderWithSkin.h"
#include "../CameraManager.h"
#include "../RenderLayerManager.h"
#include "../Mesh.h"


NEWDEL_IMPL(ShaderWithSkin)
ShaderWithSkin::ShaderWithSkin() {

	mFragCode = (char*)"WithSkin.fs";
	mVertCode = (char*)"WithSkin.vs";
}

bool ShaderWithSkin::Init()
{
	if (!Shader::Init())
		return false;
	
//	ERR( mBindBoneCount = glGetUniformLocation(mProgramObject, "myBoneCount") );
	ERR( mBindBoneMatrixArray = glGetUniformLocation(mProgramObject, "myBoneMatrixArray[0]") );

	ERR( mBindBoneIndex = glGetAttribLocation(mProgramObject, "myBoneIndex") );
	ERR( mBindBoneWeights = glGetAttribLocation(mProgramObject, "myBoneWeights") );

	return true;
}

void ShaderWithSkin::UseProgram()
{
	Shader::UseProgram();

	CameraManager * pCameraManager = CameraManager::GetCameraManager();
	Camera * pCurrentCamera = pCameraManager->GetCurrentCamera();
	
	ERR( glUniformMatrix4fv( mViewProjMatrixLocation, 1, GL_FALSE, pCurrentCamera->GetViewProjMtx().f) );

}

