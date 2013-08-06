#include "Scene.h"
#include "../Shaders/ShadersManager.h"
#include "../Animation/AnimationManager.h"
#include "../Animation/AnimationPlayer.h"
#include "../ResourceLoader.h"
#include "../CameraManager.h"
#include "../RenderLayerManager.h"
#include "../SceneManager.h"
#include "../Shell.h"
#include "../FSM/FsmMachineManager.h"


NEWDEL_IMPL(Scene)
Scene::Scene(void)
{
	
} 

Scene::~Scene(void)
{/*
	ResourceLoader & resourceLoader	 = ResourceLoader::GetResourceLoader();
	resourceLoader.Destroy();

	AnimationManager & animationManager = AnimationManager::GetAnimationManager();
	animationManager.Destroy();

	AnimationPlayer & animPlayer = AnimationPlayer::GetAnimationPlayer();
	animPlayer.Destroy();

	CameraManager * pCameraManager = CameraManager::GetCameraManager();
	pCameraManager->Destroy();

	FsmMachineManager & fsmMachineManager = FsmMachineManager::GetFsmMachineManager();
	fsmMachineManager.Destroy();

	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	renderManager.Destroy();

	SceneManager & sceneManager = SceneManager::GetSceneManager();
	sceneManager.Destroy();

	Shell & shell = Shell::GetShell();
	shell.Destroy();
	*/
}


