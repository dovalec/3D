#include "Shader.h"
#include "../Vertices.h"
#include "../Loader.h"
#include "../Shell.h"
#include "../CameraManager.h"
#include "../Mesh.h"
#include "../RenderLayerManager.h"

NEWDEL_IMPL(Shader)

Shader * Shader::mLastShader = NULL;

Shader::Shader() {

	mOrthographic = false;

	mFragCode = (char*)"NoLight.fs";
	mVertCode = (char*)"NoLight.vs";

	mVertCompiled = false;
	mFragCompiled = false;
}

Shader::~Shader()
{
	//LOGI("Destroying shader.");
	ERR( glDetachShader(mProgramObject,mFragShader) );
	ERR( glDetachShader(mProgramObject,mVertShader) );
	ERR( glDeleteShader(mFragShader) );
	ERR( glDeleteShader(mVertShader) );
	ERR( glDeleteProgram(mProgramObject) );
}

bool Shader::Init()
{
	mBindVertexAttrLocation = -1;
	mBindNormalAttrLocation = -1;
	mBindUVAttrLocation = -1;

	
	ERR( mVertShader = glCreateShader(GL_VERTEX_SHADER) );
	ERR( mFragShader = glCreateShader(GL_FRAGMENT_SHADER) );

	Loader verLoader;
	Loader fragLoader;

	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char vertCodeFullPathName[256]={0};
	static char fragCodeFullPathName[256]={0};
	sprintf(vertCodeFullPathName,"%s%sc", folder, mVertCode);
	sprintf(fragCodeFullPathName,"%s%sc", folder, mFragCode);


	if (!verLoader.OpenFile(vertCodeFullPathName))
	{
		LOGE("Failed to open compiled shader file: %s", vertCodeFullPathName);
//		ERR( glDeleteShader(mVertShader) );
	}

	if (!fragLoader.OpenFile(fragCodeFullPathName))
	{
		LOGE("Failed to open compiled shader file: %s", fragCodeFullPathName);
//		ERR( glDeleteShader(mFragShader) );
	}

	glShaderBinary(1, &mVertShader, GL_SGX_BINARY_IMG, verLoader.GetReaderPtr(), verLoader.GetDataSize() );
    if (glGetError() != GL_NO_ERROR)
	{
		sprintf(vertCodeFullPathName,"%s%s", folder, mVertCode);
		
		LOGE("Failed to set shader - Trying to compile: %s", vertCodeFullPathName);
		verLoader.FreeData();

		if (!verLoader.OpenTextFile(vertCodeFullPathName))
		{
			LOGE("Failed to open shader file: %s", vertCodeFullPathName);
			ERR( glDeleteShader(mVertShader) );
			return false;
		}

		const char * shaderArr[1] = {verLoader.GetReaderPtr()};

		ERR( glShaderSource(mVertShader, 1, shaderArr, NULL) );
		ERR( glCompileShader(mVertShader) );

		GLint bShaderCompiled;
		ERR( glGetShaderiv(mVertShader, GL_COMPILE_STATUS, &bShaderCompiled) );

		if (!bShaderCompiled)
		{
			char infoLog[256]={0};
			ERR( glGetShaderInfoLog(mVertShader, 256, NULL, infoLog) );
			LOGE( "Error compile shader: %s",infoLog );

			ERR( glDeleteShader(mVertShader) );
			return false;
		}

		mVertCompiled = true;
	}


	glShaderBinary(1, &mFragShader, GL_SGX_BINARY_IMG, fragLoader.GetReaderPtr(), fragLoader.GetDataSize());
    if (glGetError() != GL_NO_ERROR)
	{
		sprintf(fragCodeFullPathName,"%s%s", folder, mFragCode);

		LOGE("Failed to set shader:  - Trying to compile: %s",fragCodeFullPathName);

		fragLoader.FreeData();

		if (!fragLoader.OpenTextFile(fragCodeFullPathName))
		{
			LOGE("Failed to open shader file: %s", fragCodeFullPathName);
			ERR( glDeleteShader(mFragShader) );
			return false;
		}

		const char * shaderArr[1] = {fragLoader.GetReaderPtr()};

		ERR( glShaderSource(mFragShader, 1, (const char**)&shaderArr, NULL) );

		ERR( glCompileShader(mFragShader) );
		GLint bShaderCompiled;
		ERR( glGetShaderiv(mFragShader, GL_COMPILE_STATUS, &bShaderCompiled) );
		if (!bShaderCompiled)
		{
			char infoLog[256]={0};
			ERR( glGetShaderInfoLog(mFragShader, 256, NULL, infoLog) );
			LOGE( "Error compile shader: %s",infoLog );

			ERR( glDeleteShader(mFragShader) );
			return false;
		}

		mFragCompiled = true;
	}

/*
	// Load the source code into it
	ERR( glShaderSource(mFragShader, 1, (const char**)&mFragCode, NULL) );
	ERR( glShaderSource(mVertShader, 1, (const char**)&mVertCode, NULL) );

	// Compile the source code
	ERR( glCompileShader(mFragShader) );
	ERR( glCompileShader(mVertShader) );

	// Check if compilation succeeded
	GLint bShaderCompiled;
	ERR( glGetShaderiv(mFragShader, GL_COMPILE_STATUS, &bShaderCompiled) );
	
	ERR( glGetShaderiv(mVertShader, GL_COMPILE_STATUS, &bShaderCompiled) );
*/	

	// Create the shader program
	ERR( mProgramObject = glCreateProgram() );

	// Attach the fragment and vertex shaders to it
	ERR( glAttachShader(mProgramObject, mFragShader) );
	ERR( glAttachShader(mProgramObject, mVertShader) );

	// Bind the custom vertex attribute "myVertex" to location VERTEX_ARRAY
	/*
	ERR( glBindAttribLocation(mProgramObject, mBindVertexAttrLocation, "myVertex") );
	ERR( glBindAttribLocation(mProgramObject, mBindNormalAttrLocation, "myNormal") );
	ERR( glBindAttribLocation(mProgramObject, mBindUVAttrLocation, "myUV") );
	*/

	// Link the program
	ERR( glLinkProgram(mProgramObject) );

	// Check if linking succeeded in the same way we checked for compilation success
	GLint bLinked;
	ERR( glGetProgramiv(mProgramObject, GL_LINK_STATUS, &bLinked) );
	assert(bLinked);

	// First gets the location of that variable in the shader using its name
	ERR( mViewProjMatrixLocation = glGetUniformLocation(mProgramObject, "myViewProjMatrix") );
	
	ERR( mBindVertexAttrLocation = glGetAttribLocation(mProgramObject, "myVertex") );
	ERR( mBindNormalAttrLocation = glGetAttribLocation(mProgramObject, "myNormal") );
	ERR( mBindUVAttrLocation = glGetAttribLocation(mProgramObject, "myUV") );
	ERR( mUVMtx = glGetUniformLocation(mProgramObject, "myUVMtx") );
	
	
	//ERR( glBindAttribLocation(mProgramObject, mBindVertexAttrLocation, "myVertex") );
	//ERR( glBindAttribLocation(mProgramObject, mBindNormalAttrLocation, "myNormal") );
	//ERR( glBindAttribLocation(mProgramObject, mBindUVAttrLocation, "myUV") );

	return true;

}

void Shader::FreeDeviceResources()
{
	ERR( glDetachShader(mProgramObject,mFragShader) );
	ERR( glDetachShader(mProgramObject,mVertShader) );
	ERR( glDeleteShader(mFragShader) );
	ERR( glDeleteShader(mVertShader) );
	ERR( glDeleteProgram(mProgramObject) );
}

void Shader::ReInitDeviceResources()
{
	if (!Init())
	{
		LOGE("Failed to Shader::ReInitDeviceResources()");
	}
}


void Shader::UseProgram() {

	if (mLastShader != this)
	{
		ERR( glUseProgram(mProgramObject) );
		mLastShader = this;
	}

	CameraManager * pCameraManager = CameraManager::GetCameraManager();
	Camera * pCurrentCamera = pCameraManager->GetCurrentCamera();
	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	Mesh * pCurrentMesh = renderManager.GetCurrentMesh();

	PVRTMat4 & uvMtx = pCurrentMesh->GetAppearance().GetUVMatrix();

	const PVRTMat4 & meshWorld = pCurrentMesh->GetWorldMtx();
	static PVRTMat4 modelViewProj;

	if (mOrthographic)
	{
		static PVRTMat4 orthoProj = PVRTMat4::Ortho(-1,1,1,-1,0,1,PVRTMat4::OGL, false);
		PVRTMatrixMultiply(modelViewProj, meshWorld, orthoProj);		
		mOrthographic = false;
	}
	else
	{
		PVRTMatrixMultiply(modelViewProj, meshWorld, pCurrentCamera->GetViewProjMtx());		
	}

	ERR( glUniformMatrix4fv( mViewProjMatrixLocation, 1, GL_FALSE, modelViewProj.f) );

	if (mUVMtx!=-1)
	{
		ERR( glUniformMatrix4fv( mUVMtx ,1, GL_FALSE, uvMtx.f ) );
	}


}

