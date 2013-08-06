#ifndef __Material__
#define __Material__


#include "Config.h"

class Material {

public:
	float mAmbient[4]; // = {.2f,.2f,.2f, 1.0f };
	float mDiffuse[4]; // = {.8f,.8f,.8f, 1.0f };
	float mEmissive[4]; // = { 0, 0, 0, 1.0f };
	float mSpecular[4]; // = { 0, 0, 0, 1.0f };
	float mTransparency[4]; // = { 0.0f,0.0f,0.0f,0.0f };
	float mReflection[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f};

	float mShininess;

	bool mVerticesConstantColor;
	bool mVerticesConstantArray;

	virtual void Apply();

protected:
	void Defaults();

};

#endif //__Material__
