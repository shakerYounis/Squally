#include "GlobalDirector.h"

#include "cocos/2d/CCScene.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Events/SaveEvents.h"
#include "Engine/Events/SceneEvents.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/GlobalHud.h"
#include "Engine/GlobalScene.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/SmartScene.h"
#include "Engine/Utils/GameUtils.h"

using namespace cocos2d;

GlobalDirector* GlobalDirector::Instance = nullptr;

GlobalDirector* GlobalDirector::getInstance()
{
	if (GlobalDirector::Instance == nullptr)
	{
		GlobalDirector::Instance = new GlobalDirector();
	}

	return GlobalDirector::Instance;
}

GlobalDirector::GlobalDirector()
{
}

GlobalDirector::~GlobalDirector()
{
}

void GlobalDirector::loadScene(std::function<SmartScene*()> sceneCreator)
{
	SaveEvents::TriggerSoftSaveGameState();
	SceneEvents::TriggerBeforeSceneChange();
	SceneEvents::TriggerBeforeSceneChangeFinalize();

	CollisionObject::ClearCollisionObjects();

	SmartScene* scene = sceneCreator();

	if (GlobalDirector::getInstance()->activeScene == nullptr)
	{
		Director::getInstance()->runWithScene(scene);
	}
	else
	{
		GlobalDirector::getInstance()->getParent()->removeChildNoExit(GlobalDirector::getInstance());
		GameUtils::pause(GlobalDirector::getInstance()->activeScene);
		Director::getInstance()->replaceScene(scene);
	}

	// Reparent the global director onto the active scene so that all global nodes are part of the active scene graph
	scene->addChild(GlobalDirector::getInstance());

	SaveManager::Save();
	GlobalDirector::getInstance()->activeScene = scene;
	GameUtils::resume(scene);

	SceneEvents::TriggerAfterSceneChange();
}

void GlobalDirector::RegisterGlobalNode(GlobalNode* node)
{
	if (node != nullptr)
	{
		GlobalDirector::getInstance()->addChild(node);
		GlobalDirector::getInstance()->globalNodes.push_back(node);
	}
}

void GlobalDirector::RegisterGlobalNode(GlobalHud* node)
{
	if (node != nullptr)
	{
		GlobalDirector::getInstance()->addChild(node);
		GlobalDirector::getInstance()->globalNodes.push_back(node);
	}
}

void GlobalDirector::RegisterGlobalScene(GlobalScene* scene)
{
	if (scene != nullptr)
	{
		scene->retain();

		GlobalDirector::getInstance()->globalScenes.push_back(scene);
	}
}
