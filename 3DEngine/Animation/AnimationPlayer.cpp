#include "AnimationPlayer.h"
#include "AnimationManager.h"
#include "AnimationClip.h"
#include "../Shell.h"
#include "../Vertices.h"
#include "../VerticesSkin.h"



NEWDEL_IMPL(AnimationPlayer)
AnimationPlayer::AnimationPlayer(void)
{

}

AnimationPlayer::~AnimationPlayer(void)
{

}


bool AnimationPlayer::PlayClip(ModelData * pModelData, const char * clipName, AnimTrackEnum track, unsigned int loopCount, float speed)
{
    LOGI("Play clip: %s", clipName);
    
	AnimationManager & animManager = AnimationManager::GetAnimationManager();
	AnimationClip * pClip = animManager.LoadClip(clipName);
	if (pClip == NULL)
	{
		LOGE("Failed to play clip: %s", clipName);
		STOP;
		return false;
	}

	AnimationClipLogic * pCurrentClip = pModelData->GetModelAnimations().GetCurrentClip(track);
	if (pCurrentClip && pCurrentClip->GetLoopCount() == INFINITE)
	{
		RemoveCurrentClip(pModelData, track);
	}

	pModelData->AddClip(pClip, track, loopCount, speed);

	bool found = mAnimatedModels.Find(pModelData);
	if (found == false)
	{
		mAnimatedModels.Add(pModelData);
	}

	return true;
}

bool AnimationPlayer::PlayUVClip(ModelData * pModelData, const char * clipName, AnimTrackEnum track, unsigned int loopCount, float speed )
{
	AnimationManager & animManager = AnimationManager::GetAnimationManager();
	AnimationUVClip * pClip = animManager.LoadUVClip(clipName);
	if (pClip == NULL)
	{
		LOGE("Failed to play uv clip: %s", clipName);
		STOP;
		return false;
	}

	pModelData->AddClip(pClip, track,  loopCount, speed);

	bool found = mAnimatedModels.Find(pModelData);
	if (found == false)
	{
		mAnimatedModels.Add(pModelData);
	}

	return true;
}

void AnimationPlayer::Destroy()
{
	ListIterator listIter(mAnimatedModels);
	while (ModelData * pModel = (ModelData*)listIter.GetPtr())
	{
		//ModelAnimations & modelAnimations = pModel->GetModelAnimations();
        //modelAnimations.
	}

	mAnimatedModels.RemoveAll();
}

void AnimationPlayer::ClearTrack(ModelData * pModelData, AnimTrackEnum track)
{
	pModelData->GetModelAnimations().ClearTrack(track);
}

int AnimationPlayer::RemoveCurrentClip(ModelData * pModelData, AnimTrackEnum track)
{
	return pModelData->GetModelAnimations().RemoveCurrentClip(track);
}

void AnimationPlayer::RemoveAllAnimatedModelsDebug(AnimTrackEnum track)
{
	ListIterator listIter(mAnimatedModels);
	while (ModelData * pModel = (ModelData*)listIter.GetPtr())
	{
		ModelAnimations & modelAnimations = pModel->GetModelAnimations();
		modelAnimations.ClearTrack(track);
	}

	mAnimatedModels.RemoveAll();
}

void AnimationPlayer::OnTick()
{
	ListIterator listIter(mAnimatedModels);
	while (ModelData * pModel = (ModelData*)listIter.GetPtr())
	{

		for (int track = (int)ANIM_TRACK_0; track < (int)ANIM_TRACK_LAST ; track++)
		{
			ModelAnimations & modelAnimations = pModel->GetModelAnimations();
			if ( modelAnimations.IsEmpty( (AnimTrackEnum)track ) )
				continue;

			if ( !pModel->GetRoot()->IsInFrustum() ||
				 !pModel->GetRoot()->IsVisible() ||
				 !pModel->IsUsingAnimation()
				)		
				continue;

			AnimationClipLogic * pCurrentClipLogic = modelAnimations.GetCurrentClip( (AnimTrackEnum)track );
			AnimationClip * pCurrentClip = pCurrentClipLogic->GetClip();
			AnimationClip::ClipType clipType = pCurrentClip->GetClipType();

			if (clipType == AnimationClip::clip_transform)
			{

				int numOfTransforms = pCurrentClip->GetNumOfTransforms();
				int numOfKeys = pCurrentClip->GetTransformAndKeysByNum(0).keys.GetNumKeys(); // or other

				for (int trans=0 ; trans<numOfTransforms ; trans++)
				{
					TransformAndKeys & transAndKeys = pCurrentClip->GetTransformAndKeysByNum(trans);
					Node * pNode = (Node*)pModel->mTransforms.GetArray(transAndKeys.transformIndex);

					float framePos = pCurrentClipLogic->GetFramePos();
					int firstKey = pCurrentClipLogic->GetFirstFrame();
					int nextKey = pCurrentClipLogic->GetSecondFrame();

                    
					if (transAndKeys.keys.HasTranslateKeys())
					{
						TranslateKey v1  = transAndKeys.keys.GetTranslateKey((int)firstKey);						
						TranslateKey v2 = transAndKeys.keys.GetTranslateKey((int)nextKey);			
                        v1.tx = v1.tx + framePos * (v2.tx - v1.tx);
                        v1.ty = v1.ty + framePos * (v2.ty - v1.ty);
                        v1.tz = v1.tz + framePos * (v2.tz - v1.tz);
						

						pNode->SetTranslation(v1.tx,v1.ty,v1.tz);
					}

					if (transAndKeys.keys.HasRotateKeys())
					{
						RotateKey v1 = transAndKeys.keys.GetRotateKey((int)firstKey);
                        RotateKey v2 = transAndKeys.keys.GetRotateKey((int)nextKey);
						
                        PVRTQUATERNION q1;
                        PVRTQUATERNION q2;
                        PVRTQUATERNION q;
                        
                        q1.x = v1.rx;
                        q1.y = v1.ry;
                        q1.z = v1.rz;
                        q1.w = v1.rw;
                        
                        q2.x = v2.rx;
                        q2.y = v2.ry;
                        q2.z = v2.rz;
                        q2.w = v2.rw;
                        
                        PVRTMatrixQuaternionSlerp(q,q1,q2,framePos);
                        
                        pNode->SetRotation(q.x, q.y, q.z, q.w);
						
					}

					if (transAndKeys.keys.HasScaleKeys())
					{
						ScaleKey v1  = transAndKeys.keys.GetScaleKey((int)firstKey);
                        ScaleKey v2 = transAndKeys.keys.GetScaleKey((int)nextKey);
                        v1.sx = v1.sx + framePos * (v2.sx - v1.sx);
                        v1.sy = v1.sy + framePos * (v2.sy - v1.sy);
                        v1.sz = v1.sz + framePos * (v2.sz - v1.sz);
						
						pNode->SetScale(v1.sx,v1.sy,v1.sz);
					}
/*
					if (transAndKeys.keys.HasVisibilityKeys())
					{
						float v1  = (float)transAndKeys.keys.GetVisibilityKey((int)firstKey);
						if (pCurrentClipLogic->GetSpeed()!=1.0f)
						{
							float v2 = (float)transAndKeys.keys.GetVisibilityKey((int)nextKey);
							v1 = v1 + framePos * (v2 - v1);

						}
						pNode->SetVisible( v1 >= 0.5f ? true : false );
					}
*/
				}
			}
			else if (clipType == AnimationClip::clip_uv)
			{
				AnimationUVClip * pUVClip = (AnimationUVClip*)pCurrentClip;
				int meshIndex = pUVClip->GetMeshIndex();
				Mesh * pMesh = (Mesh*)pModel->mMeshes.GetArray(meshIndex);
				Vertices * pVertices = pMesh->GetVertices();
				Vertices::VerticesType verticesType = pVertices->GetVerticesType();

				int u_offest;
				int v_offest;
				if (verticesType == Vertices::vertices_regular)
				{
					u_offest = offsetof(Vertex, u);
					v_offest = offsetof(Vertex, v);

				}
				else if (verticesType == Vertices::vertices_skin)
				{
					u_offest = offsetof(VertexSkin, u);
					v_offest = offsetof(VertexSkin, v);
				}

				GLubyte * pVertexBuff = (GLubyte*)pVertices->GetVerticesBuffer();
				int numVertices = pUVClip->GetNumOfVertices();
				for (int v = 0; v<numVertices ; v++)
				{
					VertexUVKeys & vertexUVKeys = pUVClip->GetVertexUVKeysByNum(v);
					int vertex = vertexUVKeys.vertex;

					float framePos = pCurrentClipLogic->GetFramePos();
					int firstKey = pCurrentClipLogic->GetFirstFrame();
					int nextKey = pCurrentClipLogic->GetSecondFrame();
					

					UVKey v1 = vertexUVKeys.keys.GetKey((int)firstKey);

					if (pCurrentClipLogic->GetSpeed()!=1.0f)
					{
						UVKey & v2  = vertexUVKeys.keys.GetKey((int)nextKey);			
						v1.u = v1.u + framePos * (v2.u - v1.u);
						v1.v = v1.v + framePos * (v2.v - v1.v);
					}

					GLubyte * pVertexMem = pVertexBuff + (pVertices->mStride * vertex);
					*((float*)(pVertexMem + u_offest)) = v1.u;
					*((float*)(pVertexMem + v_offest)) = v1.v;
				}

				pVertices->UpdateVBO();

			}

			bool finished = false;
			pCurrentClipLogic->ProgressAnimation(finished);
			if (finished)
			{
				modelAnimations.RemoveCurrentClip( (AnimTrackEnum)track );
			}
		}	
	}

}

