

#ifndef __GlobalsHeap__
#define __GlobalsHeap__

class Shell;
class NetShell;
class ResourceLoader;
class RenderLayerManager;
class CameraManager;
class ConstraintManager;
class EventManager;
class FsmMachineManager;
class SceneManager;
class AnimationManager;
class AnimationPlayer;
class SoundManager;
class DeviceManager;
class ShadersManager;
class HeapTlsf;
class DataTable;
class ScoreManager;
class JoyStick;


class GlobalHeap
{
public:
	static void Create();
	static void Destroy();

	inline static ResourceLoader * GetResourceLoader()
	{
		return mResourceLoader;
	}
	inline static CameraManager * GetCameraManager()
	{
		return mCameraManager;
	}

	inline static ConstraintManager * GetConstraintManager()
	{
		return mConstraintManager;
	}

	inline static FsmMachineManager * GetFsmMachineManager()
	{
		return mFsmMachineManager;
	}
	inline static SceneManager * GetSceneManager()
	{
		return mSceneManager;
	}
	inline static AnimationManager * GetAnimationManager()
	{
		return mAnimationManager;
	}
	inline static AnimationPlayer * GetAnimationPlayer()
	{
		return mAnimationPlayer;
	}
	inline static SoundManager * GetSoundManager()
	{
		return mSoundManager;
	}

	inline static DeviceManager * GetDeviceManager()
	{
		return mDeviceManager;
	}

	inline static DataTable * GetDataTable()
	{
		return mDataTable;
	}

	inline static ScoreManager * GetScoreManager()
	{
		return mScoreManager;
	}

    inline static JoyStick * GetJoyStick()
	{
		return mJoyStick;
	}
	


private:
	static ResourceLoader * mResourceLoader;
	static CameraManager * mCameraManager;
	static ConstraintManager * mConstraintManager;
	static FsmMachineManager * mFsmMachineManager;
	static SceneManager  * mSceneManager;
	static AnimationManager * mAnimationManager;
	static AnimationPlayer  * mAnimationPlayer;
	static SoundManager * mSoundManager;
	static DeviceManager * mDeviceManager;
	static DataTable * mDataTable;
	static ScoreManager * mScoreManager;
    static JoyStick * mJoyStick;
};

class GlobalSys
{
public:
	static void Create();
	static void Destroy();

    inline static Shell * GetShell()
	{
		return mShell;
	}
    
    inline static NetShell * GetNetShell()
	{
		return mNetShell;
	}
    
    inline static HeapTlsf * GetTLSF()
	{
		return mTLSF;
	}
    
	inline static ShadersManager * GetShadersManager()
	{
		return mShadersManager;
	}

	inline static RenderLayerManager * GetRenderLayerManager()
	{
		return mRenderLayerManager;
	}
	inline static EventManager * GetEventManager()
	{
		return mEventManager;
	}

private:
    static HeapTlsf * mTLSF;
    static Shell * mShell;
    static NetShell * mNetShell;
	static ShadersManager * mShadersManager;
	static RenderLayerManager * mRenderLayerManager;
	static EventManager * mEventManager;
};

#endif //__GlobalsHeap__