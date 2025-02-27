#include "StagingHelperText.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Scenes/Hexus/GameState.h"
#include "Scenes/Hexus/HexusConfig.h"

#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

StagingHelperText* StagingHelperText::create()
{
	StagingHelperText* instance = new StagingHelperText();

	instance->autorelease();

	return instance;
}

StagingHelperText::StagingHelperText()
{
	this->selectionLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, Strings::Common_Empty::create());

	this->selectionLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
	this->selectionLabel->setTextColor(Color4B::WHITE);
	this->selectionLabel->enableOutline(Color4B::BLACK, 2);
	this->selectionLabel->setDimensions(HexusConfig::statusLabelWidth - 48.0f, 0.0f);

	this->cancelButton = ClickableNode::create(UIResources::Menus_Buttons_CancelV2Button, UIResources::Menus_Buttons_CancelV2ButtonHover);
	this->cancelButton->setAnchorPoint(Vec2(0.0f, 1.0f));

	this->addChild(this->selectionLabel);
	this->addChild(this->cancelButton);
}

StagingHelperText::~StagingHelperText()
{
}

void StagingHelperText::onEnter()
{
	super::onEnter();

	this->selectionLabel->setOpacity(0);
	this->cancelButton->setOpacity(0);
}

void StagingHelperText::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();

	this->cancelButton->setPosition(visibleSize.width / 2.0f + HexusConfig::rightColumnCenter + HexusConfig::statusLabelWidth / 2.0f - this->cancelButton->getContentSize().width, visibleSize.height / 2.0f + HexusConfig::statusLabelOffsetY);
	this->selectionLabel->setPosition(visibleSize.width / 2.0f + HexusConfig::rightColumnCenter - HexusConfig::statusLabelWidth / 2.0f - this->cancelButton->getContentSize().width / 2.0f, visibleSize.height / 2.0f + HexusConfig::statusLabelOffsetY);
}

void StagingHelperText::onBeforeStateChange(GameState* gameState)
{
	super::onBeforeStateChange(gameState);
}

void StagingHelperText::onAnyStateChange(GameState* gameState)
{
	super::onAnyStateChange(gameState);

	// Disabled
	return;

	switch (gameState->stateType)
	{
		case GameState::StateType::SelectionStaged:
		{
			this->updateSelectionStatus(gameState);
			this->cancelButton->setMouseClickCallback(CC_CALLBACK_0(StagingHelperText::onSelectionCancel, this, gameState));
			break;
		}
		case GameState::StateType::CombineStaged:
		{
			this->updateCombineStatus(gameState);
			this->cancelButton->setMouseClickCallback(CC_CALLBACK_0(StagingHelperText::onCombineCancel, this, gameState));
			break;
		}
		default:
		{
			this->clearSelectionStatus();
			break;
		}
	}
}

void StagingHelperText::onSelectionCancel(GameState* gameState)
{
	if (gameState->selectedHandCard != nullptr)
	{
		gameState->selectedHandCard->stopAllActions();
		gameState->selectedHandCard->runAction(MoveTo::create(HexusConfig::cardSelectSpeed, gameState->selectedHandCard->position));
		gameState->selectedHandCard = nullptr;
	}

	GameState::updateState(gameState, GameState::StateType::Neutral);
}

void StagingHelperText::updateCombineStatus(GameState* gameState)
{
	if (gameState->turn == GameState::Turn::Player && gameState->stateType == GameState::StateType::CombineStaged && gameState->selectedSourceCard == nullptr)
	{
		this->selectionLabel->setLocalizedString(Strings::Hexus_Actions_ChooseSourceCard::create());
		this->selectionLabel->runAction(FadeTo::create(0.25f, 255));
		this->cancelButton->runAction(FadeTo::create(0.25f, 255));
	}
	else if (gameState->turn == GameState::Turn::Player && gameState->stateType == GameState::StateType::CombineStaged && gameState->selectedDestinationCard == nullptr)
	{
		this->selectionLabel->setLocalizedString(Strings::Hexus_Actions_ChooseTargetCard::create());
		this->selectionLabel->runAction(FadeTo::create(0.25f, 255));
		this->cancelButton->runAction(FadeTo::create(0.25f, 255));
	}
}

void StagingHelperText::updateSelectionStatus(GameState* gameState)
{
	if (gameState->turn == GameState::Turn::Player && gameState->selectedHandCard != nullptr)
	{
		switch (gameState->selectedHandCard->cardData->getCardType())
		{
			case CardData::CardType::Special_AND:
			case CardData::CardType::Special_OR:
			case CardData::CardType::Special_XOR:
			case CardData::CardType::Special_ADD:
			case CardData::CardType::Special_SUB:
			case CardData::CardType::Special_NOT:
			case CardData::CardType::Special_ROL:
			case CardData::CardType::Special_ROR:
			{
				this->selectionLabel->setLocalizedString(Strings::Hexus_Actions_ChooseSourceCard::create());
				break;
			}
			case CardData::CardType::Special_KILL:
			case CardData::CardType::Special_RETURN_TO_HAND:
			case CardData::CardType::Special_STEAL:
			{
				this->selectionLabel->setLocalizedString(Strings::Hexus_Actions_ChooseTargetCard::create());
				break;
			}
			case CardData::CardType::Binary:
			case CardData::CardType::Decimal:
			case CardData::CardType::Hexidecimal:
			case CardData::CardType::Special_SHL:
			case CardData::CardType::Special_SHR:
			case CardData::CardType::Special_FLIP1:
			case CardData::CardType::Special_FLIP2:
			case CardData::CardType::Special_FLIP3:
			case CardData::CardType::Special_FLIP4:
			case CardData::CardType::Special_ABSORB:
			default:
			{
				this->selectionLabel->setLocalizedString(Strings::Hexus_Actions_ChooseRow::create());
				break;
			}
			case CardData::CardType::Special_SUDDEN_DEATH:
			case CardData::CardType::Special_GREED:
			case CardData::CardType::Special_BONUS_MOVES:
			case CardData::CardType::Special_PEEK:
			{
				// TODO: This is new -- these should just be a confirm/deny useage message since they have no targets
				// this->selectionLabel->setLocalizedString(Strings::Hexus_Actions_ChooseRow::create());
				break;
			}
		}

		this->selectionLabel->runAction(FadeTo::create(0.25f, 255));
		this->cancelButton->runAction(FadeTo::create(0.25f, 255));
		this->cancelButton->enableInteraction();
	}
}

void StagingHelperText::clearSelectionStatus()
{
	this->selectionLabel->setLocalizedString(Strings::Common_Empty::create());
	this->cancelButton->setMouseClickCallback(nullptr);
	this->selectionLabel->runAction(FadeTo::create(0.25f, 0));
	this->cancelButton->runAction(FadeTo::create(0.25f, 0));
	this->cancelButton->disableInteraction();
}

void StagingHelperText::onCombineCancel(GameState* gameState)
{
	if (gameState->selectedSourceCard != nullptr)
	{
		gameState->selectedSourceCard->stopAllActions();
		gameState->selectedSourceCard->runAction(MoveTo::create(HexusConfig::cardSelectSpeed, gameState->selectedSourceCard->position));
		gameState->selectedSourceCard = nullptr;
	}

	if (gameState->selectedHandCard != nullptr)
	{
		gameState->selectedHandCard->stopAllActions();
		gameState->selectedHandCard->runAction(MoveTo::create(HexusConfig::cardSelectSpeed, gameState->selectedHandCard->position));
		gameState->selectedHandCard = nullptr;
	}

	GameState::updateState(gameState, GameState::StateType::Neutral);
}
