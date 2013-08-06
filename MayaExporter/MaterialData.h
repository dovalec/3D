#pragma once

#define GLSL_SHADER_NO_LIGHT 0
#define GLSL_SHADER_USING_LIGHT 1
#define GLSL_SHADER_SKIN_NO_LIGHT 2
#define GLSL_SHADER_SKIN_USING_LIGHT 3

class MaterialData
{
public:
	MaterialData(void);
	~MaterialData(void);
	
	int index;
	int textureIndex;
	//int glsl_shader;
	unsigned int hasAlpha;
	

};
