#pragma once

#include "Engine/Components/GameComponent.h"

namespace cocos2d
{
	class Sprite;
};

class Item;
class Squally;
class Sound;

class SquallyItemDiscoveryBehavior : public GameComponent
{
public:
	static SquallyItemDiscoveryBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	SquallyItemDiscoveryBehavior(GameObject* owner);
	virtual ~SquallyItemDiscoveryBehavior();

	void onLoad() override;
	void onDisable() override;

private:
	typedef GameComponent super;

	void discoverItem(Item* item, bool cinematicHijack);

	cocos2d::Node* container = nullptr;
	cocos2d::Node* itemNode = nullptr;
	cocos2d::Sprite* glow = nullptr;
	Sound* discoverSound = nullptr;
	Squally* squally = nullptr;
};
