#ifndef __Mesh__
#define __Mesh__


#include "Config.h"

#include "Node.h"
#include "Vertices.h"
#include "Indices.h"
#include "Appearance.h"
#include "TrianglesMode.h"


class Mesh : public Node
{
public:
	NEWDEL_DECL;

	enum DisplayMode
	{
		DisplayModeRegular = 0,
		DisplayModeBillBoard = 1
	};

	Mesh();
	virtual ~Mesh()
	{
		
	}

	void SetLayer(int layer);
	void SetDisplayMode(DisplayMode mode);

	inline int GetLayer()
	{
		return mLayer;
	}

	virtual void Update();
	virtual void Render();
	virtual void Apply();

	virtual void SetExternalUpdateMode(bool mode)
	{
		//mExternalUpdateMode = mode;
	}

	inline void SetVertices(Vertices * pVertices)
	{
		mVertices = pVertices;
	}

	inline Vertices * GetVertices()
	{
		return mVertices;
	}

	inline void SetIndices(Indices * pIndices)
	{
		mIndices = pIndices;
	}

	inline Indices * GetIndices()
	{
		return mIndices;
	}

	inline Appearance & GetAppearance()
	{
		return mAppearance;
	}

	inline void SetTexture(Texture * pTexture) 
	{
		mAppearance.SetTexture(pTexture);
	}

	inline Texture * GetTexture() 
	{
		return mAppearance.GetTexture();
	}

	inline Shader * GetShader()
	{
		return mAppearance.GetShader();
	}

	inline void SetShader(Shader * pShader) 
	{
		mAppearance.SetShader(pShader);
	}

	void SetTransparent(bool state);

	inline bool GetUseDepth()
	{
		return mUseDepth;
	}

	inline void SetUseDepth(bool depth)
	{
		mUseDepth = depth;
	}


	inline bool GetUseBackFaceCulling()
	{
		return mUseBackFaceCulling;
	}

	inline void SetUseBackFaceCulling(bool culling)
	{
		mUseBackFaceCulling = culling;
	}

protected:
	Appearance mAppearance;
	int mNumOfAppearances;
	Vertices * mVertices;
	Indices * mIndices;
	int mLayer;
	DisplayMode mDisplayMode;
	bool mUseDepth;
	bool mUseBackFaceCulling;

};

#endif //__Mesh__
