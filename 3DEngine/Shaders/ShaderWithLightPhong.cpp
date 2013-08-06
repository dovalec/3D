#include "ShaderWithLightPhong.h"
#include "../CameraManager.h"
#include "../Mesh.h"
#include "../RenderLayerManager.h"

NEWDEL_IMPL(ShaderWithLightPhong)
ShaderWithLightPhong::ShaderWithLightPhong() {

	mFragCode = (char*)"WithLightPhong.fs";
	mVertCode = (char*)"WithLightPhong.vs";
}

bool ShaderWithLightPhong::Init()
{
	if (!Shader::Init())
		return false;

	ERR( myModelView = glGetUniformLocation(mProgramObject, "myModelView") );
	ERR( myModelViewIT = glGetUniformLocation(mProgramObject, "myModelViewIT") );
	ERR( myLightSel = glGetUniformLocation(mProgramObject, "myLightSel") );
	ERR( myLightPosition = glGetUniformLocation(mProgramObject, "myLightPosition") );
	ERR( myLightDirection = glGetUniformLocation(mProgramObject, "myLightDirection") );
	ERR( myLightColor = glGetUniformLocation(mProgramObject, "myLightColor") );

	return true;
}

void ShaderWithLightPhong::UseProgram()
{
	Shader::UseProgram();

	CameraManager * pCameraManager = CameraManager::GetCameraManager();
	Camera * pCurrentCamera = pCameraManager->GetCurrentCamera();
	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	Mesh * pCurrentMesh = renderManager.GetCurrentMesh();


	float lightPosition[] = { 50, 100, 20 };
	float lightDirection[] = { 0, 0, 1 };
	float lightColor[] = { 1, 1, 1 };

	ERR( glUniform3fv(myLightPosition, 1, lightPosition) );
	ERR( glUniform3fv(myLightDirection, 1, lightDirection) );
	ERR( glUniform3fv(myLightColor, 1, lightColor) );
	ERR( glUniform1i(myLightSel, 3) );



	PVRTMat4 viewMtx(pCurrentCamera->GetViewMtx().f);
	PVRTMat4 meshWorld( pCurrentMesh->GetWorldMtx().f );
	PVRTMat4 modelView = viewMtx * meshWorld;
	ERR( glUniformMatrix4fv( myModelView, 1, GL_FALSE, modelView.ptr() ) );

	PVRTMat3 modelViewIT = PVRTMat3(modelView).inverse().transpose();
	ERR( glUniformMatrix3fv(myModelViewIT, 1, GL_FALSE, modelViewIT.ptr()) );

}

