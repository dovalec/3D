
#ifndef __Appearance__
#define __Appearance__

#include "Config.h"

#include "CompositingMode.h"
#include "PolygonMode.h"
//#include "Material.h"
#include "Shaders/Shader.h"
#include "Texture.h"

class Appearance
{
public:	
	NEWDEL_DECL
	Appearance();
	virtual void Apply();

	
	void SetTexture(Texture * pTexture);
	void SetShader(Shader * pShader);

	inline Shader * GetShader()
	{
		return m_pShader;
	}


	inline PVRTMat4 & GetUVMatrix()
	{
		return mUVMtx;
	}
	
	inline void SetUVMatrix(PVRTMat4 uvMtx)
	{
		mUVMtx = uvMtx;
	}

	inline Texture * GetTexture()
	{
		return m_pTexture;
	}


public:
	CompositingMode mCompositingMode;
	PolygonMode mPolygonMode;
	//Material mMaterial;

protected:
	Texture * m_pTexture;
	Shader	* m_pShader;
	PVRTMat4 mUVMtx;
};

#endif //__Appearance__
