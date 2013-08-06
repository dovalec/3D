#ifndef __Node__
#define __Node__


#include "Config.h"
#include "Matrix.h"
#include "TL/List.h"
#include "TL/String.h"

class ModelData;

class Node {
public:
	NEWDEL_DECL
public:
	Node();
	virtual ~Node()
	{

	}

	void SetModelData(ModelData * pModelData)
	{
		mParentModel = pModelData;
	}

	inline ModelData * GetModelData()
	{
		return mParentModel;
	}

	inline PVRTVec3 & GetTranslation()
	{
		return mTranslation;
	}

	inline PVRTQUATERNION & GetRotation()
	{
		return mRotation;
	}

	inline PVRTMat4 GetWorldRotationMatrix()
	{
		PVRTMat4 worldRot = mWorld;
		worldRot.f[12] = 0.0f;
		worldRot.f[13] = 0.0f;
		worldRot.f[14] = 0.0f;

		return worldRot;
	}

	inline PVRTVec3 & GetScale()
	{
		return mScale;
	}

	inline void SetTranslation( PVRTVec3 trans )
	{
		mTranslation = trans;		
		UpdateMatrices();
	}

	inline void SetTranslation(float x, float y, float z )
	{
		mTranslation.x = x;
		mTranslation.y = y;
		mTranslation.z = z;
		UpdateMatrices();
	}

	inline void SetPivot(float x, float y, float z )
	{
		mPivot.x = x;
		mPivot.y = y;
		mPivot.z = z;
		UpdateMatrices();
	}

	inline void SetRotation(PVRTQUATERNION quat)
	{
		mRotation = quat;
		UpdateMatrices();
	}

	inline void SetRotation(float x, float y, float z, float w)
	{
		mRotation.x = x;
		mRotation.y = y;
		mRotation.z = z;
		mRotation.w = w;
		UpdateMatrices();
	}

	inline void SetScale(float x, float y, float z)
	{
		mScale.x = x;
		mScale.y = y;
		mScale.z = z;
		UpdateMatrices();
	}

	inline void SetScale(float xyz)
	{
		mScale.x = xyz;
		mScale.y = xyz;
		mScale.z = xyz;
		UpdateMatrices();
	}

	inline void UpdateLocal()
	{
		UpdateMatrices();
	}

	inline void UpdateMatrices()
	{
		mUpdateLocal = true;
	}

	static PVRTMat4 LookAt(const PVRTVec3& vEye, const PVRTVec3& vAt, const PVRTVec3& vUp, bool bRightHanded);

	void SetName(String name);

	const String & GetName();

	void SetVisible(bool flag);

	void SetVisisbleChildren(bool flag);

	inline bool IsVisible()
	{
		return mVisible;
	}

	inline void SetInFrustum(bool flag)
	{
		mInFrustum = flag;
	}

	inline bool IsInFrustum()
	{
		return mInFrustum;
	}

	void SetPicked(bool flag);

	bool GetPicked();

	void AddChild(Node * child);

	void RemoveChild(Node * child);

	inline int ChildrenCount()
	{
		return mChildren.Size();
	}

	inline Node * GetParent()
	{
		return mParent;
	}

	virtual void Update();
	virtual void Render();

	inline const PVRTMat4 & GetWorldMtx() const
	{
		return mWorld;
	}

	inline PVRTVec3 GetWorldTranslation()
	{
		PVRTVec3 pos(mWorld.f[12], mWorld.f[13], mWorld.f[14]);
		return pos;
	}

	PVRTVec2 GetProjected();
	inline const PVRTMat4 & GetWorldInverseMtx() const
	{
		return mWorldInverse;
	}

	inline const PVRTMat4 & GetLocalMtx() const
	{
		return mLocal;
	}

	inline void SetLocalMtx(PVRTMat4 mtx)
	{
		mLocal = mtx;
	}

	void UpdateInverseWorldTransform();

	inline bool GetUseOcclusionCulling()
	{
		return mUseOcclusionCulling;
	}

	inline void SetUseOcclusionCulling(bool culling)
	{
		mUseOcclusionCulling = culling;
	}

	void SetUseOcclusionCullingTraverse(bool culling);

	void printWorld();
protected:
	virtual void UpdateLocalMatrix();
	virtual void UpdateWorldTransform();


	bool mUpdateLocal;
	bool mUpdateInverse;

public:
	virtual void Apply();


public:
	int mParentIndex;

protected:
	Node * mParent;
	ModelData * mParentModel;

	String mName;

	bool mUseOcclusionCulling;
	bool mVisible;
	bool mPicked;
	bool mInFrustum;

	PVRTMat4 mLocal;
	PVRTMat4 mWorld;
	PVRTMat4 mWorldInverse;

	PVRTVec3 mTranslation;
	PVRTVec3 mPivot;
	PVRTQUATERNION mRotation;
	PVRTVec3 mScale;

	List mChildren;

};

#endif //__Node__
