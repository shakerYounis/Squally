﻿#include "Lexicon.h"

#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Events/HackableEvents.h"
#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/BinaryIntroPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/BinarySelectPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/And/AndPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Bswap/BswapPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Not/NotPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Or/OrPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Shl/ShlPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Shr/ShrPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Binary/Xor/XorPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/ChapterSelectPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/ControlFlow/ControlFlowIntroPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/ControlFlow/ControlFlowSelectPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/ControlFlow/Nop/NopPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/DataIntroPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/DataSelectPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Add/AddExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Add/AddPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Dec/DecExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Dec/DecPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Div/DivExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Div/DivPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Inc/IncExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Inc/IncPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Mov/MovExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Mov/MovPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Mul/MulExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Mul/MulPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Neg/NegExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Neg/NegPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Pop/PopExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Pop/PopPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Push/PushExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Push/PushPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Sub/SubExamplesPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Data/Sub/SubPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/IntroPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/FloatingPoint/FloatingPointIntroPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/FloatingPoint/FloatingPointSelectPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Vector/VectorIntroPage.h"
#include "Engine/Hackables/Menus/CodeEditor/Lexicon/Pages/Vector/VectorSelectPage.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Localization/LocalizedSprite.h"
#include "Engine/Utils/GameUtils.h"

#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

Lexicon* Lexicon::create()
{
	Lexicon* instance = new Lexicon();

	instance->autorelease();

	return instance;
}

Lexicon::Lexicon()
{
	this->background = Sprite::create(UIResources::Menus_LexiconMenu_desert_background);
	this->banner = Sprite::create(UIResources::Menus_LexiconMenu_Banner);
	this->title = LocalizedSprite::create(UIResources::Menus_LexiconMenu_Title_Lexicon_en);
	this->lexiconBack = Sprite::create(UIResources::Menus_LexiconMenu_LexiconBack);
	this->lexiconFront = Sprite::create(UIResources::Menus_LexiconMenu_LexiconFront);
	this->leftPageNode = Node::create();
	this->rightPageNode = Node::create();

	LocalizedLabel*	backLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H3, Strings::Menus_Return::create());
	LocalizedLabel*	backLabelSelected = backLabel->clone();

	backLabel->enableOutline(Color4B::BLACK, 2);
	backLabelSelected->enableOutline(Color4B::BLACK, 2);

	this->backButton = ClickableTextNode::create(backLabel, backLabelSelected, UIResources::Menus_Buttons_GenericButton, UIResources::Menus_Buttons_GenericButtonHover);
	this->darkFrame = Sprite::create(UIResources::Menus_LexiconMenu_DarkFrame);

	// Intro
	this->pages.push_back(IntroPage::create());
	this->pages.push_back(ChapterSelectPage::create());

	// Data
	this->pages.push_back(DataIntroPage::create());
	this->pages.push_back(DataSelectPage::create());
	this->pages.push_back(IncExamplesPage::create());
	this->pages.push_back(IncPage::create());
	this->pages.push_back(DecExamplesPage::create());
	this->pages.push_back(DecPage::create());
	this->pages.push_back(AddExamplesPage::create());
	this->pages.push_back(AddPage::create());
	this->pages.push_back(SubExamplesPage::create());
	this->pages.push_back(SubPage::create());
	this->pages.push_back(DivExamplesPage::create());
	this->pages.push_back(DivPage::create());
	this->pages.push_back(MulExamplesPage::create());
	this->pages.push_back(MulPage::create());
	this->pages.push_back(MovExamplesPage::create());
	this->pages.push_back(MovPage::create());
	this->pages.push_back(PushExamplesPage::create());
	this->pages.push_back(PushPage::create());
	this->pages.push_back(PopExamplesPage::create());
	this->pages.push_back(PopPage::create());
	this->pages.push_back(NegExamplesPage::create());
	this->pages.push_back(NegPage::create());

	// Binary
	this->pages.push_back(BinaryIntroPage::create());
	this->pages.push_back(BinarySelectPage::create());
	this->pages.push_back(AndPage::create());
	this->pages.push_back(OrPage::create());
	this->pages.push_back(XorPage::create());
	this->pages.push_back(ShlPage::create());
	this->pages.push_back(ShrPage::create());
	this->pages.push_back(NotPage::create());
	this->pages.push_back(BswapPage::create());

	// Control flow
	this->pages.push_back(ControlFlowIntroPage::create());
	this->pages.push_back(ControlFlowSelectPage::create());
	this->pages.push_back(NopPage::create());

	// SIMD
	this->pages.push_back(VectorIntroPage::create());
	this->pages.push_back(VectorSelectPage::create());

	// FPU
	this->pages.push_back(FloatingPointIntroPage::create());
	this->pages.push_back(FloatingPointSelectPage::create());

	for (auto next : this->pages)
	{
		switch(next->getPageType())
		{
			default:
			case LexiconPage::PageType::Left:
			{
				this->leftPageNode->addChild(next);
				break;
			}
			case LexiconPage::PageType::Right:
			{
				this->rightPageNode->addChild(next);
				break;
			}
		}
	}

	this->addChild(this->background);
	this->addChild(this->banner);
	this->addChild(this->title);
	this->addChild(this->lexiconBack);
	this->addChild(this->lexiconFront);
	this->addChild(this->leftPageNode);
	this->addChild(this->rightPageNode);
	this->addChild(this->darkFrame);
	this->addChild(this->backButton);
}

Lexicon::~Lexicon()
{
}

void Lexicon::onEnter()
{
	super::onEnter();

	this->defer([=]()
	{
		HackableEvents::TriggerOpenLexiconPage(OpenLexiconPageArgs(IntroPage::Identifier));
		HackableEvents::TriggerOpenLexiconPage(OpenLexiconPageArgs(ChapterSelectPage::Identifier));
	});
}

void Lexicon::initializePositions()
{
	super::initializePositions();

	const Vec2 BookOffset = Vec2(32.0f, -48.0f);

	CSize visibleSize = Director::getInstance()->getVisibleSize();
	this->background->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f));
	this->banner->setPosition(Vec2(visibleSize.width / 2.0f - 712.0f, visibleSize.height / 2.0f + 32.0f));
	this->title->setPosition(Vec2(visibleSize.width / 2.0f - 712.0f, visibleSize.height / 2.0f + 464.0f));
	this->backButton->setPosition(Vec2(visibleSize.width / 2.0f - 788.0f, 96.0f));
	this->lexiconBack->setPosition(Vec2(visibleSize.width / 2.0f + BookOffset.x, visibleSize.height / 2.0f + BookOffset.y));
	this->lexiconFront->setPosition(Vec2(visibleSize.width / 2.0f + BookOffset.x, visibleSize.height / 2.0f + BookOffset.y + 24.0f));
	this->leftPageNode->setPosition(Vec2(visibleSize.width / 2.0f + BookOffset.x - LexiconPage::PageOffset.x, visibleSize.height / 2.0f + BookOffset.y + LexiconPage::PageOffset.y));
	this->rightPageNode->setPosition(Vec2(visibleSize.width / 2.0f + BookOffset.x + LexiconPage::PageOffset.x, visibleSize.height / 2.0f + BookOffset.y + LexiconPage::PageOffset.y));
	this->darkFrame->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f));
}
	
void Lexicon::initializeListeners()
{
	super::initializeListeners();

	this->backButton->setMouseClickCallback([=](InputEvents::MouseEventArgs*)
	{
		this->close();
	});
}

void Lexicon::open()
{
	this->setVisible(true);
	GameUtils::focus(this);
}

void Lexicon::close()
{
	if (this->isVisible())
	{
		if (closeCallback != nullptr)
		{
			closeCallback();
		}
		
		this->setVisible(false);
	}
}

void Lexicon::setCloseCallBack(std::function<void()> closeCallback)
{
	this->closeCallback = closeCallback;
}
