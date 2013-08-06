#pragma once

class TextureData
{
public:
	TextureData(void);
	~TextureData(void);

	int index;
	string textureFileName;
	string pvrTextureFileName;
	
	unsigned int width;
	unsigned int height;
	unsigned int hasAlpha;

	float uRepeat;
	float vRepeat;
};
