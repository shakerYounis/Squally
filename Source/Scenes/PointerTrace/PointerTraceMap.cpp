#include "PointerTraceMap.h"

#include "cocos/2d/CCSprite.h"
#include "cocos/2d/CCAction.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Deserializers/Isometric/IsometricDecorDeserializer.h"
#include "Deserializers/Isometric/IsometricEntityDeserializer.h"
#include "Deserializers/Isometric/IsometricObjectDeserializer.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Deserializers/Meta/BackgroundDeserializer.h"
#include "Engine/Deserializers/Meta/MetaLayerDeserializer.h"
#include "Engine/Deserializers/Meta/MusicDeserializer.h"
#include "Engine/Deserializers/Objects/ObjectLayerDeserializer.h"
#include "Engine/GlobalDirector.h"
#include "Engine/Maps/GameMap.h"
#include "Engine/Maps/TileLayer.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Sound/Music.h"
#include "Engine/UI/HUD/Hud.h"
#include "Engine/UI/Mouse.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Isometric/PointerTrace/GridEntity.h"
#include "Events/PointerTraceEvents.h"
#include "Objects/Isometric/PointerTrace/GridObject.h"
#include "Objects/Isometric/PointerTrace/RegisterInitializers/RegisterInitializer.h"
#include "Objects/Isometric/PointerTrace/MemoryGrid.h"
#include "Scenes/PointerTrace/Huds/PointerTraceHud.h"
#include "Scenes/PointerTrace/Menus/VictoryMenu.h"
#include "Scenes/PointerTrace/RegisterState.h"

#include "Resources/IsometricObjectResources.h"
#include "Resources/MusicResources.h"

using namespace cocos2d;

PointerTraceMap* PointerTraceMap::create(std::string mapFile, std::function<void()> onLevelClearCallback)
{
	PointerTraceMap* instance = new PointerTraceMap(mapFile, onLevelClearCallback);

	instance->autorelease();

	return instance;
}

PointerTraceMap::PointerTraceMap(std::string mapFile, std::function<void()> onLevelClearCallback) : super(false)
{
	this->onLevelClearCallback = onLevelClearCallback;
	this->collisionDebugNode = Node::create();
	this->pointerTraceHud = PointerTraceHud::create();
	this->victoryMenu = VictoryMenu::create();

	this->addLayerDeserializers({
			MetaLayerDeserializer::create(
			{
				BackgroundDeserializer::create(),
				MusicDeserializer::create(),
			}),
			ObjectLayerDeserializer::create({
				{ IsometricDecorDeserializer::MapKeyTypeDecor, IsometricDecorDeserializer::create() },
				{ IsometricEntityDeserializer::MapKeyTypeEntity, IsometricEntityDeserializer::create() },
				{ IsometricObjectDeserializer::MapKeyTypeObject, IsometricObjectDeserializer::create() },
			})
		}
	);

	this->collisionDebugNode->setVisible(false);

	this->loadMap(mapFile);

	this->addChild(this->collisionDebugNode);
	this->hud->addChild(this->pointerTraceHud);
	this->menuHud->addChild(this->victoryMenu);
}

PointerTraceMap::~PointerTraceMap()
{
}

void PointerTraceMap::onEnter()
{
	super::onEnter();

	this->victoryMenu->setVisible(false);

	ObjectEvents::QueryObject<MemoryGrid>([=](MemoryGrid* memoryGrid)
	{
		this->memoryGrid = memoryGrid;
	});

	this->buildCollisionMaps();
	this->initializeGridObjects();

	this->scheduleUpdate();
}

void PointerTraceMap::initializePositions()
{
	super::initializePositions();
	
	CSize visibleSize = Director::getInstance()->getVisibleSize();

	this->victoryMenu->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f));
}

void PointerTraceMap::initializeListeners()
{
	super::initializeListeners();

	this->addEventListener(EventListenerCustom::create(PointerTraceEvents::EventResetState, [=](EventCustom* eventCustom)
	{
		this->resetState();
	}));

	this->addEventListener(EventListenerCustom::create(PointerTraceEvents::EventVictory, [=](EventCustom* eventCustom)
	{
		if (this->onLevelClearCallback != nullptr)
		{
			this->onLevelClearCallback();
		}

		this->openVictoryMenu();
	}));

	this->addEventListener(EventListenerCustom::create(PointerTraceEvents::EventRequestMovement, [=](EventCustom* eventCustom)
	{
		PointerTraceEvents::PointerTraceRequestMovementArgs* args = static_cast<PointerTraceEvents::PointerTraceRequestMovementArgs*>(eventCustom->getData());

		if (args != nullptr)
		{
			this->moveGridEntity(*args);
		}
	}));

	this->addEventListener(EventListenerCustom::create(PointerTraceEvents::EventResumeMovement, [=](EventCustom* eventCustom)
	{
		PointerTraceEvents::PointerTraceRequestMovementArgs* args = static_cast<PointerTraceEvents::PointerTraceRequestMovementArgs*>(eventCustom->getData());

		if (args != nullptr)
		{
			this->tryResumeMovement(*args);
		}
	}));
}

void PointerTraceMap::update(float dt)
{
	super::update(dt);
}

void PointerTraceMap::onDeveloperModeEnable(int debugLevel)
{
	super::onDeveloperModeEnable(debugLevel);

	this->collisionDebugNode->setVisible(true);
}

void PointerTraceMap::onDeveloperModeDisable()
{
	super::onDeveloperModeDisable();

	this->collisionDebugNode->setVisible(false);
}

void PointerTraceMap::tryResumeMovement(PointerTraceEvents::PointerTraceRequestMovementArgs args)
{
	if (args.gridEntity == nullptr || this->memoryGrid == nullptr)
	{
		return;
	}

	int sourceIndex = args.gridEntity->getGridIndex();

	RegisterState::setRegisterEip(sourceIndex);

	if (this->collisionMap.contains(sourceIndex) || this->segfaultMap.contains(sourceIndex))
	{
		args.gridEntity->lockMovement();
		this->doSegfault();
		return;
	}

	args.gridEntity->unlockMovement();
	args.gridEntity->uninterruptMovement();

	PointerTraceEvents::TriggerEntityMoved(PointerTraceEvents::PointerTraceEntityMovedArgs(
		this->memoryGrid,
		args.gridEntity,
		args
	));
}

void PointerTraceMap::moveGridEntity(PointerTraceEvents::PointerTraceRequestMovementArgs args)
{
	if (args.gridEntity == nullptr || this->memoryGrid == nullptr)
	{
		return;
	}

	if (!args.gridEntity->isMovementLocked())
	{
		float speed = MathUtils::clamp(args.speed, 0.0f, 1.0f);
		int gridWidth = this->memoryGrid->getGridWidth();
		int gridHeight = this->memoryGrid->getGridHeight();
		int sourceIndex = args.gridEntity->getGridIndex();
		int destinationIndex = sourceIndex;
		bool outOfBounds = false;
		Vec2 outOfBoundsMovement = Vec2::ZERO;

		if (gridWidth == 0 || gridHeight == 0)
		{
			return;
		}

		switch(args.direction)
		{
			default:
			case PointerTraceEvents::PointerTraceRequestMovementArgs::Direction::Left:
			{
				destinationIndex--;
				outOfBoundsMovement = Vec2(-128.0f, -64.0f);

				if (destinationIndex < 0 || destinationIndex / gridWidth != sourceIndex / gridWidth)
				{
					outOfBounds = true;
				}
				break;
			}
			case PointerTraceEvents::PointerTraceRequestMovementArgs::Direction::Right:
			{
				destinationIndex++;
				outOfBoundsMovement = Vec2(128.0f, 64.0f);
				
				if (destinationIndex / gridWidth != sourceIndex / gridWidth)
				{
					outOfBounds = true;
				}
				break;
			}
			case PointerTraceEvents::PointerTraceRequestMovementArgs::Direction::Up:
			{
				destinationIndex -= this->memoryGrid->getGridWidth();
				outOfBoundsMovement = Vec2(-128.0f, 64.0f);

				if (destinationIndex < 0)
				{
					outOfBounds = true;
				}
				break;
			}
			case PointerTraceEvents::PointerTraceRequestMovementArgs::Direction::Down:
			{
				destinationIndex += this->memoryGrid->getGridWidth();
				outOfBoundsMovement = Vec2(128.0f, -64.0f);

				if (destinationIndex >= this->memoryGrid->getMaxIndex())
				{
					outOfBounds = true;
				}
				
				break;
			}
		}

		// Segfault!
		if (outOfBounds || this->segfaultMap.contains(destinationIndex))
		{
			args.gridEntity->lockMovement();

			args.gridEntity->runAction(
				Sequence::create(
					MoveBy::create(speed, outOfBoundsMovement),
					CallFunc::create([=]()
					{
						this->doSegfault();
					}),
					nullptr
				)
			);
			return;
		}

		// Collision detection
		if (this->collisionMap.contains(destinationIndex))
		{
			return;
		}

		RegisterState::setRegisterEip(destinationIndex);

		// Create a copy or the lambda will botch this variable
		Vec2 destination = this->memoryGrid->gridIndexToWorldPosition(destinationIndex);
		args.gridEntity->lockMovement();

		PointerTraceEvents::PointerTraceRequestMovementArgs argsClone = PointerTraceEvents::PointerTraceRequestMovementArgs(
			args.gridEntity,
			args.direction,
			args.source,
			args.speed
		);

		args.gridEntity->runAction(
			Sequence::create(
				MoveTo::create(speed, destination),
				CallFunc::create([=]()
				{
					argsClone.gridEntity->setGridIndex(destinationIndex);
					argsClone.gridEntity->unlockMovement();

					PointerTraceEvents::TriggerEntityMoved(PointerTraceEvents::PointerTraceEntityMovedArgs(
						this->memoryGrid,
						argsClone.gridEntity,
						argsClone
					));

					if (!argsClone.gridEntity->isMovementInterrupted())
					{
						this->moveGridEntity(argsClone);
					}
				}),
				nullptr
			)
		);
	}
}

void PointerTraceMap::initializeGridObjects()
{
	if (this->memoryGrid == nullptr)
	{
		return;
	}

	// Initialize registers first before anything else
	ObjectEvents::QueryObjects<RegisterInitializer>([=](RegisterInitializer* gridObject)
	{
		int gridIndex = this->memoryGrid->worldCoordsToGridIndex(gridObject->getPosition());
		Vec2 realignedPosition = this->memoryGrid->gridIndexToWorldPosition(gridIndex);
		
		gridObject->setPosition(realignedPosition);
		gridObject->setInitialGridIndex(gridIndex);
	});

	ObjectEvents::QueryObjects<GridObject>([=](GridObject* gridObject)
	{
		int gridIndex = this->memoryGrid->worldCoordsToGridIndex(gridObject->getPosition());
		Vec2 realignedPosition = this->memoryGrid->gridIndexToWorldPosition(gridIndex);
		
		gridObject->setPosition(realignedPosition);
		gridObject->setInitialGridIndex(gridIndex);
	});

	ObjectEvents::QueryObjects<GridEntity>([=](GridEntity* gridEntity)
	{
		int gridIndex = this->memoryGrid->worldCoordsToGridIndex(gridEntity->getPosition());
		Vec2 realignedPosition = this->memoryGrid->gridIndexToWorldPosition(gridIndex);
		
		gridEntity->setPosition(realignedPosition);
		gridEntity->setInitialGridIndex(gridIndex);
	});

	this->memoryGrid->setInitialState();
}

void PointerTraceMap::resetState()
{
	if (this->memoryGrid == nullptr)
	{
		return;
	}

	ObjectEvents::QueryObjects<GridObject>([=](GridObject* gridObject)
	{
		gridObject->setGridIndex(gridObject->getInitialGridIndex());
		Vec2 respawnPosition = this->memoryGrid->gridIndexToWorldPosition(gridObject->getGridIndex());
		
		gridObject->setPosition(respawnPosition);
	});

	ObjectEvents::QueryObjects<GridEntity>([=](GridEntity* gridEntity)
	{
		gridEntity->setGridIndex(gridEntity->getInitialGridIndex());
		Vec2 respawnPosition = this->memoryGrid->gridIndexToWorldPosition(gridEntity->getGridIndex());

		gridEntity->unlockMovement();
		gridEntity->uninterruptMovement();
		
		gridEntity->setPosition(respawnPosition);
	});

	this->memoryGrid->resetState();
}

void PointerTraceMap::buildCollisionMaps()
{
	this->collisionMap.clear();
	this->segfaultMap.clear();

	if (this->map == nullptr || this->memoryGrid == nullptr)
	{
		return;
	}

	std::vector<TileLayer*> collisionLayers = this->map->getCollisionLayers();
	CSize visibleSize = Director::getInstance()->getVisibleSize();

	for (auto layer : collisionLayers)
	{
		std::vector<std::vector<int>> gidMap = layer->getGidMap();

		int width = gidMap.empty() ? 0 : gidMap[0].size();
		int height = gidMap.size();
		int y = 0;

		for (int y = 0; y < int(gidMap.size()); y++)
		{
			for (int x = 0; x < int(gidMap[y].size()); x++)
			{
				int gid = gidMap[y][x];

				// Janky garbage math. Seems to work, whatever
				float newX = float(width - y) * 128.0f;
				float newY = float(x) * 128.0f;
				float realX = (newX + newY);
				float realY = (newX - newY) / 2.0f + this->memoryGrid->getPositionY() - 320.0f;
				int gridIndex = this->memoryGrid->worldCoordsToGridIndex(Vec2(realX, realY));

				switch(gid)
				{
					case 1:
					{
						Sprite* debugBox = Sprite::create(IsometricObjectResources::PointerTrace_Selector);

						debugBox->setPosition(this->memoryGrid->gridIndexToWorldPosition(gridIndex));

						this->collisionDebugNode->addChild(debugBox);

						collisionMap.insert(gridIndex);
						break;
					}
					case 2:
					{
						Sprite* debugBox = Sprite::create(IsometricObjectResources::PointerTrace_Crystals_Selector);

						debugBox->setPosition(this->memoryGrid->gridIndexToWorldPosition(gridIndex));

						this->collisionDebugNode->addChild(debugBox);

						segfaultMap.insert(gridIndex);
						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
	}
}

void PointerTraceMap::doSegfault()
{
	PointerTraceEvents::TriggerResetState();
}

void PointerTraceMap::openVictoryMenu()
{
	this->victoryMenu->setVisible(true);
	this->menuBackDrop->setOpacity(196);

	GameUtils::focus(this->victoryMenu);
}
