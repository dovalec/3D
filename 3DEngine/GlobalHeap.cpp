
#include "GlobalHeap.h"

#include "ResourceLoader.h"
#include "RenderLayerManager.h"
#include "CameraManager.h"
#include "Events/EventManager.h"
#include "FSM/FsmMachineManager.h"
#include "SceneManager.h"
#include "Animation/AnimationManager.h" 
#include "Animation/AnimationPlayer.h"
#include "Sound/SoundManager.h"
#include "Device/DeviceManager.h"
#include "Shaders/ShadersManager.h"
#include "DataTable.h"
#include "Game/ScoreManager.h"
#include "ConstraintManager.h"
#include "Shell.h"
#include "NetShell.h"
#include "JoyStick.h"

#include "TLSF/HeapTlsf.h"

ResourceLoader * GlobalHeap::mResourceLoader = NULL;
CameraManager *GlobalHeap:: mCameraManager = NULL;
ConstraintManager *GlobalHeap:: mConstraintManager = NULL;
FsmMachineManager * GlobalHeap::mFsmMachineManager = NULL;
SceneManager  * GlobalHeap::mSceneManager = NULL;
AnimationManager * GlobalHeap::mAnimationManager = NULL;
AnimationPlayer  * GlobalHeap::mAnimationPlayer = NULL;
SoundManager * GlobalHeap::mSoundManager = NULL;
DeviceManager * GlobalHeap::mDeviceManager = NULL;
DataTable * GlobalHeap::mDataTable = NULL;
ScoreManager * GlobalHeap::mScoreManager = NULL;
JoyStick * GlobalHeap::mJoyStick = NULL;

void GlobalHeap::Create()
{
    HeapTlsf::GetTLSF()->TLSF_ON();
    
	mResourceLoader = new ResourceLoader();
	mCameraManager = new CameraManager();
	mConstraintManager = new ConstraintManager();
	mFsmMachineManager = new FsmMachineManager();
	mSceneManager = new SceneManager();
	mAnimationManager = new AnimationManager();
	mAnimationPlayer = new AnimationPlayer();
	mSoundManager = new SoundManager();	
	mDeviceManager = new DeviceManager();	
	mDataTable = new DataTable();	
	mScoreManager = new ScoreManager();
    mJoyStick = new JoyStick();
}


void GlobalHeap::Destroy()
{
	mResourceLoader = NULL;
	mCameraManager = NULL;
	mConstraintManager = NULL;
	mFsmMachineManager = NULL;
	mSceneManager = NULL;
	mAnimationManager = NULL;
	mAnimationPlayer = NULL;
	mSoundManager = NULL;
	mDeviceManager = NULL;
	mDataTable = NULL;
	mScoreManager = NULL;
}


////////////////////////////////////////////////////////////////////

HeapTlsf * GlobalSys::mTLSF = NULL;
Shell * GlobalSys::mShell = NULL;
NetShell * GlobalSys::mNetShell = NULL;
ShadersManager * GlobalSys::mShadersManager = NULL;
RenderLayerManager * GlobalSys::mRenderLayerManager = NULL;
EventManager * GlobalSys::mEventManager = NULL;

void GlobalSys::Create()
{
    
    mTLSF = new HeapTlsf("main");
    
    HeapTlsf::GetTLSF()->TLSF_OFF();
    
    mShell = new Shell();
    mNetShell = new NetShell();
	mShadersManager = new ShadersManager();
	mRenderLayerManager = new RenderLayerManager();
	mEventManager = new EventManager();
}

void GlobalSys::Destroy()
{
    delete mShell;
    delete mNetShell;
	delete mShadersManager;
	delete mRenderLayerManager;
	delete mEventManager;
    delete mTLSF;

}