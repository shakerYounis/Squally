#include "OrcBomberCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlash/BasicSlash.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/ThrowFlamingWeapon/ThrowFlamingWeapon.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/TimeBomb/DropTimeBomb.h"
#include "Scenes/Platformer/Inventory/Items/Consumables/Health/IncrementHealthFlask/IncrementHealthFlask.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string OrcBomberCombatBehavior::MapKey = "orc-bomber-combat";

OrcBomberCombatBehavior* OrcBomberCombatBehavior::create(GameObject* owner)
{
	OrcBomberCombatBehavior* instance = new OrcBomberCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

OrcBomberCombatBehavior::OrcBomberCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.25f);
}

OrcBomberCombatBehavior::~OrcBomberCombatBehavior()
{
}

void OrcBomberCombatBehavior::initializePositions()
{
}

void OrcBomberCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(DropTimeBomb::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::VeryCommon));
		attackBehavior->registerAttack(ThrowFlamingWeapon::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeedVerySlow, PlatformerAttack::Priority::Reasonable));
		attackBehavior->registerAttack(BasicSlash::create(3, 5, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::IfNecessary));
	});
	
	this->entity->watchForComponent<EntityInventoryBehavior>([=](EntityInventoryBehavior* entityInventoryBehavior)
	{
		Inventory* inventory = entityInventoryBehavior->getInventory();

		if (inventory != nullptr)
		{
			// inventory->forceInsert(IncrementHealthFlask::create());
		}
	});
}
