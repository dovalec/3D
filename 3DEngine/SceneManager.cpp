#include "SceneManager.h"
#include "RenderLayerManager.h"
#include "DataTable.h"
#include "CameraManager.h"
#include "AI/Collision.h"

NEWDEL_IMPL(SceneManager)
SceneManager::SceneManager()
{
	mNodeCount = 0;
	mToApplyCount = 0;
	mLookMtx = NULL;
}

SceneManager::~SceneManager()
{

}


void SceneManager::AddRootNode(Node * pNode)
{
	mRootNodes[mNodeCount] = pNode;
	mNodeCount++;
}


void SceneManager::Update()
{
	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	const PVRTVec3 center = renderManager.GetCenter();
	float occlusionRadius = renderManager.GetOcclusionRadius();


    PVRTVec4 vecA( mLookMtx->f[12], 0.0f,  mLookMtx->f[14], 1);
	PVRTVec4 vecB( GLOBAL_SCALE *  FRUSTUM_W, 0.0f,  GLOBAL_SCALE * FRUSTUM_D, 1);
	PVRTVec4 vecC( GLOBAL_SCALE * -FRUSTUM_W, 0.0f,  GLOBAL_SCALE * FRUSTUM_D, 1);
	
    vecB = *mLookMtx * vecB;
    vecC = *mLookMtx * vecC;

	PVRTVec2 A(vecA.x, vecA.z);
	PVRTVec2 B(vecB.x, vecB.z);
	PVRTVec2 C(vecC.x, vecC.z);



	mToApplyCount = 0;

	if (mQuadTree)
	{
		static QuadNode * quadNodes[256]={0}; 
		int quadNodeCount = 0;

		//mQuadTree->GetQuads(center.x, center.z, occlusionRadius, quadNodes, quadNodeCount);
		mQuadTree->GetQuadsCameraFrustum(quadNodes, quadNodeCount,  mLookMtx);
		quadNodeCount--;

		bool useFrustumCulling = true; //!!!!!!!!!!!!!!!!!!!!!

		for (int quad = quadNodeCount ; quad >=0 ; quad--)
		{
			QuadNode * pQuadNode = quadNodes[quad];

			List & dataList = pQuadNode->GetDataList();
			ListIterator listIter(dataList);
			while( Node * pRootNode = (Node*)listIter.GetPtr() )	
			{			
				if (!pRootNode->IsVisible())
					continue;

				//pRootNode->UpdateWithoutChildren();
				bool useOcclusionRadius  = pRootNode->GetUseOcclusionCulling();
				PVRTVec3 worldPos = pRootNode->GetWorldTranslation();

				if (!useFrustumCulling && useOcclusionRadius)
				{
					PVRTVec3 distVec = worldPos - center;

					if ( distVec.lenSqr() < MM(occlusionRadius) ) 
					{
						pRootNode->SetInFrustum(true);
						pRootNode->Update();
						mToApply[mToApplyCount] = pRootNode;
						mToApplyCount++;
					}
					else
					{
						pRootNode->SetInFrustum(false);
					}
				}
				else if (useFrustumCulling)
				{
					PVRTVec2 P(worldPos.x, worldPos.z);

					PVRTVec2 v0 = C - A;
					PVRTVec2 v1 = B - A;
					PVRTVec2 v2 = P - A;

					// Compute dot products
					float dot00 = v0.dot(v0);
					float dot01 = v0.dot(v1);
					float dot02 = v0.dot(v2);
					float dot11 = v1.dot(v1);
					float dot12 = v1.dot(v2);

					// Compute barycentric coordinates
					float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
					float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
					float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

					bool addToList = false;
					// Check if point is in triangle
					//PVRTVec3 distVec = worldPos - center;
					//if ( distVec.lenSqr() < MM(occlusionRadius) ) 
					{
						if ( (u > 0) && (v > 0) && (u + v < 1))
						{
							addToList = true;
						}
						else if ( Collision::CircleTriangleEdgeIntersection(A,B,P, pRootNode->GetRadius() ) )
						{
							addToList = true;
						}
						else if ( Collision::CircleTriangleEdgeIntersection(A,C,P, pRootNode->GetRadius() ))
						{
							addToList = true;
						}

						if (addToList)
						{
							pRootNode->SetInFrustum(true);
							//pRootNode->Update();
							mToApply[mToApplyCount] = pRootNode;
							mToApplyCount++;
						}
                        else
                        {
                            pRootNode->SetInFrustum(false);
                        }
					}
					//else
					//{
					//	pRootNode->SetInFrustum(false);
					//}
				}
				else
				{
					pRootNode->SetInFrustum(true);
					//pRootNode->Update();
					mToApply[mToApplyCount] = pRootNode;
					mToApplyCount++;
				}

			}
		}
	}
	
	
	for (int n=0;n<mNodeCount;n++)
	{
		Node * pRootNode = mRootNodes[n];
		if (!pRootNode->IsVisible())
			continue;

		pRootNode->UpdateWithoutChildren();

		bool useOcclusionRadius  = pRootNode->GetUseOcclusionCulling();

		PVRTVec3 worldPos = pRootNode->GetWorldTranslation();
		PVRTVec3 distVec = worldPos - center;

		if (useOcclusionRadius)
		{
			if ( distVec.lenSqr() < MM(occlusionRadius) ) 
			{

				PVRTVec2 P(worldPos.x, worldPos.z);

				PVRTVec2 v0 = C - A;
				PVRTVec2 v1 = B - A;
				PVRTVec2 v2 = P - A;

				// Compute dot products
				float dot00 = v0.dot(v0);
				float dot01 = v0.dot(v1);
				float dot02 = v0.dot(v2);
				float dot11 = v1.dot(v1);
				float dot12 = v1.dot(v2);

				// Compute barycentric coordinates
				float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
				float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
				float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

				bool addToList = false;
				// Check if point is in triangle
				//PVRTVec3 distVec = worldPos - center;
				//if ( distVec.lenSqr() < MM(occlusionRadius) ) 
				{
					if ( (u > 0) && (v > 0) && (u + v < 1))
					{
						addToList = true;
					}
					else if ( Collision::CircleTriangleEdgeIntersection(A,B,P, pRootNode->GetRadius() ) )
					{
						addToList = true;
					}
					else if ( Collision::CircleTriangleEdgeIntersection(A,C,P, pRootNode->GetRadius() ))
					{
						addToList = true;
					}

					if (addToList)
					{
						pRootNode->SetInFrustum(true);
						pRootNode->Update();
						mToApply[mToApplyCount] = pRootNode;
						mToApplyCount++;
					}
                    else
                    {
                        pRootNode->SetInFrustum(false);
                    }
				}
/*
				pRootNode->SetInFrustum(true);
				pRootNode->Update();
				mToApply[mToApplyCount] = pRootNode;
				mToApplyCount++;
*/
			}
			else
			{
				pRootNode->SetInFrustum(false);
			}
		}
		else
		{
			pRootNode->SetInFrustum(true);
			pRootNode->Update();
			mToApply[mToApplyCount] = pRootNode;
			mToApplyCount++;
		}

		/*

		PVRTVec3 worldPos = pRootNode->GetWorldTranslation();
		PVRTVec3 distVec = worldPos - center;

		if (!pRootNode->GetUseOcclusionCulling())
		{
		pRootNode->SetInFrustum(true);
		}
		else if ( distVec.lenSqr() < occlusionRadius ) 
		{
		pRootNode->SetInFrustum(true);		
		}
		else
		{
		pRootNode->SetInFrustum(false);
		}
		*/

	}

}

void SceneManager::SetSceneQuadTree(QuadTree * pQuadTree)
{
	mQuadTree = pQuadTree;
}

void SceneManager::Apply()
{
	for (int n=0;n<mToApplyCount;n++)
	{
		Node * pRootNode = mToApply[n];

		//if (pRootNode->IsInFrustum())
		{
			pRootNode->Apply();
		}
	}
}




