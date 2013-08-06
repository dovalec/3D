#include "ShaderWithLight.h"
#include "../CameraManager.h"
#include "../Mesh.h"
#include "../RenderLayerManager.h"

NEWDEL_IMPL(ShaderWithLight)
ShaderWithLight::ShaderWithLight() {

	mFragCode = (char*)"WithLight.fs";
	mVertCode = (char*)"WithLight.vs";
}

bool ShaderWithLight::Init()
{
	if (!Shader::Init())
		return false;

	ERR( mBindLightDirection = glGetUniformLocation(mProgramObject, "myLightDirection") );
	ERR( mBindModelViewIT = glGetUniformLocation(mProgramObject, "myModelViewIT") );

	return true;
}

void ShaderWithLight::UseProgram()
{
	Shader::UseProgram();
	
	float dir[3]={0.0f, 1.0f, 1.0f};

	ERR( glUniform3fv(mBindLightDirection,1,dir) );

	CameraManager * pCameraManager = CameraManager::GetCameraManager();
	Camera * pCurrentCamera = pCameraManager->GetCurrentCamera();
	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	Mesh * pCurrentMesh = renderManager.GetCurrentMesh();

	
	PVRTMat4 & viewMtx = pCurrentCamera->GetViewMtx();
	PVRTMat4 modelView = viewMtx * pCurrentMesh->GetWorldMtx();
	PVRTMat3 modelViewIT = PVRTMat3(modelView).inverse().transpose();
	ERR( glUniformMatrix3fv( mBindModelViewIT , 1, GL_FALSE, modelViewIT.ptr()) );

}

