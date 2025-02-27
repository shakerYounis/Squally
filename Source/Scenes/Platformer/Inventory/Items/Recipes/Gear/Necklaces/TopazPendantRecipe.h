#pragma once

#include "Scenes/Platformer/Inventory/Items/Recipes/Gear/Necklaces/NecklaceRecipe.h"

class TopazPendantRecipe : public NecklaceRecipe
{
public:
	static TopazPendantRecipe* create();

	std::vector<Item*> craft() override;
	Item* clone() override;
	LocalizedString* getString() override;
	const std::string& getIconResource() override;
	std::string getCraftedItemIconResource() override;
	const std::string& getIdentifier() override;

	static const std::string SaveKey;

protected:
	TopazPendantRecipe();
	virtual ~TopazPendantRecipe();
	
	std::vector<std::tuple<Item*, int>> getReagentsInternal() override;

private:
	typedef NecklaceRecipe super;
};
