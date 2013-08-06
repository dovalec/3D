#include "ShaderWithTwoLights.h"
#include "../CameraManager.h"
#include "../Mesh.h"
#include "../RenderLayerManager.h"

NEWDEL_IMPL(ShaderWithTwoLights)
ShaderWithTwoLights::ShaderWithTwoLights() {

	mFragCode = (char*)"WithTwoLights.fs";
	mVertCode = (char*)"WithTwoLights.vs";
}

bool ShaderWithTwoLights::Init()
{
	if (!Shader::Init())
		return false;

	ERR( mBindLightDirection1 = glGetUniformLocation(mProgramObject, "myLightDirection1") );
	ERR( mBindLightDirection2 = glGetUniformLocation(mProgramObject, "myLightDirection2") );
	ERR( mBindModelViewIT = glGetUniformLocation(mProgramObject, "myModelViewIT") );

	return true;
}

void ShaderWithTwoLights::UseProgram()
{
	Shader::UseProgram();
	
	CameraManager * pCameraManager = CameraManager::GetCameraManager();
	Camera * pCurrentCamera = pCameraManager->GetCurrentCamera();
	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	Mesh * pCurrentMesh = renderManager.GetCurrentMesh();


	float dir1[3]={1.0f	, 1.0f, 1.0f};
	float dir2[3]={-1.0f, 1.0f, 1.0f};
	ERR( glUniform3fv(mBindLightDirection1,1,dir1) );
	ERR( glUniform3fv(mBindLightDirection2,1,dir2) );


	PVRTMat4 & viewMtx = pCurrentCamera->GetViewMtx();
	PVRTMat4 modelView = viewMtx * pCurrentMesh->GetWorldMtx();
	PVRTMat3 modelViewIT = PVRTMat3(modelView).inverse().transpose();
	ERR( glUniformMatrix3fv( mBindModelViewIT , 1, GL_FALSE, modelViewIT.ptr()) );


}

