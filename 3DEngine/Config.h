

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define EGL_NOT_PRESENT

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif //ANDROID
#include "OGLES2Tools.h"

#include <cerrno>
#include <cstddef>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "TLSF/HeapTlsf.h"
#include "GlobalHeap.h"
#include "GeneralHashFunctions.h"


//#include "../../me2013/assets/content/EventEnums.h"
//#include "../../me2013/assets/content/SoundEnums.h"


//###################################################
//###################################################

extern float GLOBAL_SCALE;

#define FRUSTUM_W 260.0f
#define FRUSTUM_D 500.0f


#define FPS_25 0.04f
#define FPS_50 0.02f
#define GAME_FPS FPS_25
#define ENTITY_WALK_DIST 1.2f

#define MM(a) ((a)*(a))
#define MAX_COLLISION_OBJ_NUM 256
#define MAX_COLLISION 32
#ifdef INFINITE
	#undef INFINITE
#endif

#define INFINITE 0xFFFFFFFF


#define SAFE_DEL(p) { if (p) delete p; p = NULL; }

//###################################################
//###################################################



typedef short SHORT;
typedef unsigned short USHORT;

typedef int INT;
typedef unsigned int UINT;

typedef char CHAR;
typedef unsigned char UCHAR;

#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505


#ifdef WIN32
	#define  LOG_TAG    "GameEngine"

	#define  LOGI(...)  
	#define  LOGW(...)  
	#define  LOGE(...)  
	#define  LOGS(...)  

	void SoftStop();

	#define ERR(code) \
	{ \
		code; \
		GLenum err = glGetError(); \
		if (GL_NO_ERROR != err) \
		{ \
			OutputDebugStringA("Error: "); \
			if (err == GL_INVALID_ENUM) OutputDebugStringA("GL_INVALID_ENUM\n"); \
			if (err == GL_INVALID_VALUE) OutputDebugStringA("GL_INVALID_VALUE\n"); \
			if (err == GL_INVALID_OPERATION) OutputDebugStringA("GL_INVALID_OPERATION\n"); \
			if (err == GL_OUT_OF_MEMORY) OutputDebugStringA("GL_OUT_OF_MEMORY\n"); \
			SoftStop(); \
		} \
	}

	#define STOP SoftStop()
	
	#define ERR2(code) \
	{ \
		code; \
	}

#endif

#ifdef __APPLE__

	#define  LOG_TAG    "GameEngine"

	#define  LOGI(...)  printf(__VA_ARGS__);printf("\n");
	#define  LOGW(...)  printf(__VA_ARGS__);printf("\n");
	#define  LOGE(...)  printf(__VA_ARGS__);printf("\n");

	#define ERR(code) \
	{ \
		code; \
		GLenum err = glGetError(); \
		if (GL_NO_ERROR != err) \
		{ \
			LOGE("Error: %s - %d", __FILE__, __LINE__); \
			if (err == GL_INVALID_ENUM) LOGE("GL_INVALID_ENUM\n"); \
			if (err == GL_INVALID_VALUE) LOGE("GL_INVALID_VALUE\n"); \
			if (err == GL_INVALID_OPERATION) LOGE("GL_INVALID_OPERATION\n"); \
			if (err == GL_OUT_OF_MEMORY) LOGE("GL_OUT_OF_MEMORY\n"); \
		} \
	}

    void SoftStop();

	#define STOP SoftStop();

	#define ERR222(code) \
	{ \
		code; \
	}
#endif



#endif //__CONFIG_H__