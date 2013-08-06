#include "Shell.h"

NEWDEL_IMPL(Shell)
Shell::Shell()
{
	mReadPath[0] = '\0';
	mWritePath[0] = '\0';

	mScreenWidth = 0;
	mScreenHeight = 0;

	mJoystickX1 = 0;
	mJoystickY1 = 0;
	mJoystickX2 = 0;
	mJoystickY2 = 0;
    
    mFrameTime = GAME_FPS;
    mFPSFactor = 1.0f;
    
    

    
}

void Shell::InitShell()
{
	bool pvrtc = IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc");
	if (pvrtc)
	{
		SetSystem("sgx");
		mLowGpu = false;
	}
	else
	{
		if (mLowGpu)
			SetSystem("other_low");
		else
			SetSystem("other");
	}
}

void Shell::SetReadPath(const char * pPath)
{
	strcpy(mReadPath, pPath);
}

void Shell::SetWritePath(const char * pPath)
{
	strcpy(mWritePath, pPath);
}

void Shell::SetCameraImage(const char * pPath)
{
	strcpy(mCameraImage, pPath);
}
char * Shell::GetCameraImage()
{
	return mCameraImage;
}
void Shell::ResetCameraImage()
{
	mCameraImage[0] = 0;
}
char * Shell::GetReadPath()
{
	return mReadPath;
}

void Shell::SetSystem(const char * pSystem)
{
	strcpy(mSystem, pSystem);
	//LOGS("Using system: %s",pSystem);
}

void Shell::SetLowGPU(bool low)
{
	mLowGpu = low;
}

bool Shell::GetLowGPU()
{
	return mLowGpu;
}


char * Shell::GetSystem()
{
	return mSystem;
}

char * Shell::GetWritePath()
{
	return mWritePath;
}


void Shell::SetScreenSize(int w, int h)
{
	mScreenWidth = w;
	mScreenHeight = h;		
}


bool Shell::IsGLExtensionSupported(const char *extension)
{
	// The recommended technique for querying OpenGL extensions;
	// from http://opengl.org/resources/features/OGLextensions/
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;

	extensions = glGetString(GL_EXTENSIONS);

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions String. Don't be fooled by sub-strings, etc. */
	start = extensions;
	for (;;) {
		where = (GLubyte *) strstr((const char *) start, extension);
		if (!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return true;
		start = terminator;
	}

	return false;
}