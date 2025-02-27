#include "Mouse.h"

#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCInputEvents.h"

#include "Engine/GlobalDirector.h"
#include "Engine/UI/HUD/Hud.h"

using namespace cocos2d;

Mouse* Mouse::Instance = nullptr;

void Mouse::RegisterGlobalNode()
{
	GlobalDirector::RegisterGlobalNode(Mouse::getInstance());
}

Mouse* Mouse::getInstance()
{
	if (Mouse::Instance == nullptr)
	{
		Mouse::Instance = new Mouse();
		Mouse::Instance->autorelease();
	}

	return Mouse::Instance;
}

Mouse::Mouse()
{
	this->mouseHud = Hud::create();
	this->mouseSpriteIdle = Node::create();
	this->mouseSpritePoint = Node::create();
	this->mouseSpritePointPressed = Node::create();
	this->mouseSpriteDrag = Node::create();

	// Anchor point is the top left for the mouse -- this is where the click happens
	this->setAnchorPoint(Vec2(0.0f, 1.0f));
	this->mouseSpriteIdle->setAnchorPoint(Vec2(0.0f, 1.0f));
	this->mouseSpritePoint->setAnchorPoint(Vec2(0.0f, 1.0f));
	this->mouseSpritePointPressed->setAnchorPoint(Vec2(0.0f, 1.0f));
	this->mouseSpriteDrag->setAnchorPoint(Vec2(0.0f, 1.0f));

	this->setActiveMouseSprite(this->mouseSpriteIdle);

	// Nothing should ever go higher in Z index than the mouse
	this->mouseHud->setLocalZOrder(std::numeric_limits<int>().max());

	this->addChild(this->mouseHud);
	this->mouseHud->addChild(this->mouseSpriteIdle);
	this->mouseHud->addChild(this->mouseSpritePoint);
	this->mouseHud->addChild(this->mouseSpritePointPressed);
	this->mouseHud->addChild(this->mouseSpriteDrag);
}

Mouse::~Mouse()
{
}

void Mouse::onEnter()
{
	super::onEnter();

	this->setSpriteToCursorPosition();
	this->setActiveMouseSprite(this->mouseSpriteIdle);
}

void Mouse::initializeListeners()
{
	super::initializeListeners();

	this->addGlobalEventListener(EventListenerCustom::create(InputEvents::EventMouseRefresh, [=](EventCustom* eventCustom)
	{
		InputEvents::MouseEventArgs* args = static_cast<InputEvents::MouseEventArgs*>(eventCustom->getData());

		if (args != nullptr)
		{
			this->readMousePosition = args->mouseCoords;

			if (args->isDragging)
			{
				this->setActiveMouseSprite(this->mouseSpriteDrag);
			}
			else if (args->isLeftClicked && args->canClick)
			{
				this->setActiveMouseSprite(this->mouseSpritePointPressed);
			}
			else if (args->canClick)
			{
				this->setActiveMouseSprite(this->mouseSpritePoint);
			}
			else
			{
				this->setActiveMouseSprite(this->mouseSpriteIdle);
			}

			this->setSpriteToCursorPosition();
		}
	}));
}

void Mouse::registerCursorSet(int setId, CursorSet cursorSet)
{
	this->cursorSets[setId] = cursorSet;
}

void Mouse::setActiveCursorSet(int setId)
{
	if (!this->cursorSets.contains(setId))
	{
		return;
	}

	this->mouseSpriteIdle->removeAllChildren();
	this->mouseSpritePoint->removeAllChildren();
	this->mouseSpritePointPressed->removeAllChildren();
	this->mouseSpriteDrag->removeAllChildren();

	this->activeCursorSet = setId;
	CursorSet cursorSet = this->cursorSets[this->activeCursorSet];

	Sprite* mouseSpriteIdle = Sprite::create(cursorSet.mouseSpriteIdleResource);
	Sprite* mouseSpritePoint = Sprite::create(cursorSet.mouseSpritePointResource);
	Sprite* mouseSpritePointPressed = Sprite::create(cursorSet.mouseSpritePointPressedResource);
	Sprite* mouseSpriteDrag = Sprite::create(cursorSet.mouseSpriteDragResource);

	mouseSpriteIdle->setAnchorPoint(Vec2(0.0f, 1.0f));
	mouseSpritePoint->setAnchorPoint(Vec2(0.0f, 1.0f));
	mouseSpritePointPressed->setAnchorPoint(Vec2(0.0f, 1.0f));
	mouseSpriteDrag->setAnchorPoint(Vec2(0.0f, 1.0f));

	this->mouseSpriteIdle->addChild(mouseSpriteIdle);
	this->mouseSpritePoint->addChild(mouseSpritePoint);
	this->mouseSpritePointPressed->addChild(mouseSpritePointPressed);
	this->mouseSpriteDrag->addChild(mouseSpriteDrag);
}

int Mouse::getActiveCursorSet()
{
	return this->activeCursorSet;
}

void Mouse::setActiveMouseSprite(Node* mouseSprite)
{
	this->mouseSpriteIdle->setVisible(false);
	this->mouseSpritePoint->setVisible(false);
	this->mouseSpritePointPressed->setVisible(false);
	this->mouseSpriteDrag->setVisible(false);

	// This will be one of the ones above
	mouseSprite->setVisible(true);
}

void Mouse::setSpriteToCursorPosition()
{
	this->mouseSpriteIdle->setPosition(this->readMousePosition);
	this->mouseSpritePoint->setPosition(this->readMousePosition);
	this->mouseSpritePointPressed->setPosition(this->readMousePosition);
	this->mouseSpriteDrag->setPosition(this->readMousePosition);
}
