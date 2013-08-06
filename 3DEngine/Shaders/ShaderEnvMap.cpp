#include "ShaderEnvMap.h"

#include "../CameraManager.h"
#include "../Mesh.h"
#include "../RenderLayerManager.h"

NEWDEL_IMPL(ShaderEnvMap)
ShaderEnvMap::ShaderEnvMap() {

	mFragCode = (char*)"EnvMap.fs";
	mVertCode = (char*)"EnvMap.vs";
}

bool ShaderEnvMap::Init()
{
	if (!Shader::Init())
		return false;

	ERR( myModelWorld = glGetUniformLocation(mProgramObject, "myModelWorld") );
	ERR( myEyePosModel = glGetUniformLocation(mProgramObject, "myEyePosModel") );
	ERR( myCubeReflection = glGetUniformLocation(mProgramObject, "myCubeReflection") );
	ERR( my2DMap = glGetUniformLocation(mProgramObject, "my2DMap") );
	ERR( myCubeMap = glGetUniformLocation(mProgramObject, "myCubeMap") );
	
	return true;
}

void ShaderEnvMap::UseProgram()
{
	Shader::UseProgram();
	
	glUniform1i(myCubeReflection, false);
	
	glUniform1i(my2DMap, 0);
	glUniform1i(myCubeMap, 1);

	CameraManager * pCameraManager = CameraManager::GetCameraManager();
	Camera * pCurrentCamera = pCameraManager->GetCurrentCamera();
	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	Mesh * pCurrentMesh = renderManager.GetCurrentMesh();

	PVRTMat4 viewMtx(pCurrentCamera->GetViewMtx().f);
	
	static float m_fAngleX = 0.0;
	static float m_fAngleY = 0.0;

	PVRTMat4 mModel, mRotX, mRotY;
	mRotX = PVRTMat4::RotationX(m_fAngleX);
	mRotY = PVRTMat4::RotationY(m_fAngleY);
	
	mModel = mRotY * mRotX;

	m_fAngleX += 0.01f;
	//m_fAngleY += 0.011f;

	//PVRTMat4 meshWorld( pCurrentMesh->GetWorldMtx().f );
	PVRTMat4 meshWorld = mModel;
	PVRTMat4 modelView = viewMtx * meshWorld;
	
	// Set model matrix
	PVRTMat3 model3x3 = PVRTMat3(meshWorld);
	glUniformMatrix3fv( myModelWorld, 1, GL_FALSE, model3x3.ptr());

	// Set eye position in model space
	PVRTVec4 eyePosModel;
	eyePosModel = modelView.inverse() * PVRTVec4(0, 0, 0, 1);

	glUniform3fv(myEyePosModel, 1, &eyePosModel.x);


}

