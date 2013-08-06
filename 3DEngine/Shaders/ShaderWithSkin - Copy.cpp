#include "ShaderWithSkin.h"
#include "Vertices.h"

ShaderWithSkin::ShaderWithSkin() {

	mFragCode = (char*)"\
				uniform sampler2D sampler2d;\
				varying mediump vec2 myTexCoord;\
				void main (void)\
				{\
					gl_FragColor = texture2D(sampler2d,myTexCoord);\
				}";


	mVertCode = (char*)"\
				attribute highp vec3	myVertex; \
				attribute mediump vec3	myNormal; \
				attribute mediump vec2	myUV; \
				attribute mediump vec4	myBoneWeights; \
				attribute mediump vec4	myBoneIndex; \
				uniform mediump mat4	myViewProjMatrix; \
				uniform mediump	int		myBoneCount; \
				uniform highp   mat4	myBoneMatrixArray[12]; \
				\
				varying mediump vec2	myTexCoord; \
				\
				void main(void)\
				{\
					mediump ivec4 boneIndex = ivec4(myBoneIndex); \
					mediump vec4 boneWeights = myBoneWeights; \
					\
					highp mat4 boneMatrix = myBoneMatrixArray[boneIndex.x]; \
					highp vec4 position = boneMatrix * vec4(myVertex, 1.0) * boneWeights.x; \
					\
					for (lowp int i = 1; i < 4; ++i) \
					{ \
						if(i<myBoneCount) \
						{ \
							boneIndex = boneIndex.yzwx; \
							boneWeights = boneWeights.yzwx; \
							boneMatrix = myBoneMatrixArray[boneIndex.x]; \
							position += boneMatrix * vec4(myVertex, 1.0) * boneWeights.x; \
						} \
					} \
					gl_Position = myViewProjMatrix * position; \
					myTexCoord = myUV.st; \
				}";
}

void ShaderWithSkin::Init()
{
	Shader::Init();
	
	ERR( mBindBoneCount = glGetUniformLocation(mProgramObject, "myBoneCount") );
	ERR( mBindBoneMatrixArray = glGetUniformLocation(mProgramObject, "myBoneMatrixArray[0]") );

	ERR( mBindBoneIndex = glGetAttribLocation(mProgramObject, "myBoneIndex") );
	ERR( mBindBoneWeights = glGetAttribLocation(mProgramObject, "myBoneWeights") );

	ERR( glUseProgram(mProgramObject) );



}

