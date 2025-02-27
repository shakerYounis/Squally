#include "AquaMarineEarrings.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Earrings/AquaMarineEarringsRecipe.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string AquaMarineEarrings::SaveKey = "aquamarine-earrings";

AquaMarineEarrings* AquaMarineEarrings::create()
{
	AquaMarineEarrings* instance = new AquaMarineEarrings();

	instance->autorelease();

	return instance;
}

AquaMarineEarrings::AquaMarineEarrings() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 27 }}), ItemStats(
	// Health
	0,
	// Mana
	2,
	// Attack
	0,
	// Magic Attack
	0,
	// Armor
	0,
	// Speed
	0.05f
))
{
}

AquaMarineEarrings::~AquaMarineEarrings()
{
}

Item* AquaMarineEarrings::clone()
{
	return AquaMarineEarrings::create();
}

LocalizedString* AquaMarineEarrings::getString()
{
	return Strings::Items_Equipment_Gear_Earrings_AquaMarineEarrings::create();
}

const std::string& AquaMarineEarrings::getIconResource()
{
	return ItemResources::Equipment_Gear_Earrings_AquaMarineEarrings;
}

const std::string& AquaMarineEarrings::getIdentifier()
{
	return AquaMarineEarrings::SaveKey;
}

Recipe* AquaMarineEarrings::getRecipe()
{
	return AquaMarineEarringsRecipe::create();
}
