#include "AsciiTable.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCDrawNode.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/UI/Controls/RadioButton.h"
#include "Engine/UI/Controls/ScrollPane.h"
#include "Engine/Utils/GameUtils.h"
#include "Scenes/Cipher/Components/Blocks/Special/ImmediateBlock.h"
#include "Scenes/Cipher/Components/DisplayModeToggles.h"
#include "Scenes/Cipher/Components/Letters/SmartAsciiLabel.h"
#include "Scenes/Cipher/CipherConfig.h"
#include "Scenes/Cipher/CipherState.h"

#include "Resources/CipherResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

AsciiTable* AsciiTable::create()
{
	AsciiTable* instance = new AsciiTable();

	instance->autorelease();

	return instance;
}

AsciiTable::AsciiTable()
{
	this->background = Sprite::create(CipherResources::PopupPanelBack);
	this->scrollPane = ScrollPane::create(CSize(1408.0f, 456.0f), UIResources::Menus_Buttons_SliderButton, UIResources::Menus_Buttons_SliderButtonSelected, CSize(0.0f, 32.0f));

	for (int next = 0; next < 256; next++)
	{
		// this->asciiLetters.push_back(ImmediateBlock::create((unsigned char)(next), BlockBase::BlockType::Static));
	}

	this->selectionSprite = Sprite::create(CipherResources::Blocks_BlockHexHuge);

	this->frame = Sprite::create(CipherResources::PopupPanelFrame);

	this->displayModeToggles = DisplayModeToggles::create(true);

	LocalizedLabel*	returnLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H3, Strings::Menus_Return::create());
	LocalizedLabel*	returnLabelSelected = returnLabel->clone();

	returnLabel->enableShadow(Color4B::BLACK, CSize(-2.0f, -2.0f), 2);
	returnLabel->enableGlow(Color4B::BLACK);

	returnLabelSelected->setColor(Color3B::YELLOW);
	returnLabelSelected->enableShadow(Color4B::BLACK, CSize(-2.0f, -2.0f), 2);
	returnLabelSelected->enableGlow(Color4B::ORANGE);

	this->returnButton = ClickableTextNode::create(returnLabel, returnLabelSelected, CipherResources::Buttons_TestRunButton, CipherResources::Buttons_TestRunButtonSelected);	
	this->asciiTableTitle = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H3, Strings::Cipher_AsciiTable::create());	
	this->chooseANewValueTitle = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H3, Strings::Cipher_ChooseANewValue::create());
	
	this->scrollPane->renderCustomBackground([=](DrawNode* customBackground, CSize paneSize, CSize paddingSize, CSize marginSize)
	{
		std::vector<Vec2> verticies = std::vector<Vec2>();

		const float CornerSizeInner = 72.0f;
		const float CornerSize = 56.0f;

		CSize totalSize = paneSize + paddingSize * 2.0f + marginSize * 2.0f;

		// Bottom left
		verticies.push_back(Vec2(CornerSizeInner, 0.0f));
		verticies.push_back(Vec2(0.0f, CornerSize));

		// Top left
		verticies.push_back(Vec2(0.0f, totalSize.height - CornerSize));
		verticies.push_back(Vec2(CornerSizeInner, totalSize.height));

		// Top right
		verticies.push_back(Vec2(totalSize.width - CornerSizeInner, totalSize.height));
		verticies.push_back(Vec2(totalSize.width, totalSize.height - CornerSize));

		// Bottom right
		verticies.push_back(Vec2(totalSize.width, CornerSize));
		verticies.push_back(Vec2(totalSize.width - CornerSizeInner, 0.0f));

		for (auto next : verticies)
		{
			next.x += (28.0f - paddingSize.width - marginSize.width);
			next.y += (24.0f - paddingSize.height - marginSize.height);
		}

		customBackground->drawSolidPoly(verticies.data(), verticies.size(), Color4F(Color4B(0, 0, 0, 128)));
	});

	this->asciiTableTitle->enableShadow(Color4B::BLACK, CSize(2, -2), 2);
	this->chooseANewValueTitle->enableShadow(Color4B::BLACK, CSize(2, -2), 2);

	this->addChild(this->background);

	this->scrollPane->suspendUpdate();

	for (auto next : this->asciiLetters)
	{
		this->scrollPane->addChild(next);
	}
	
	this->scrollPane->addChild(this->selectionSprite);
	
	this->scrollPane->resumeUpdate();

	this->addChild(this->scrollPane);
	this->addChild(this->frame);
	this->addChild(this->displayModeToggles);
	this->addChild(this->returnButton);
	this->addChild(this->asciiTableTitle);
	this->addChild(this->chooseANewValueTitle);
}

AsciiTable::~AsciiTable()
{
}

void AsciiTable::onEnter()
{
	super::onEnter();
}

void AsciiTable::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();

	this->background->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f));
	this->scrollPane->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f + 52.0f + 24.0f));

	for (int index = 0; index < int(this->asciiLetters.size()); index++)
	{
		const int GridWidth = 8;
		const int GridHeight = 32;

		int x = index / GridHeight;
		int y = index % GridHeight;

		this->asciiLetters[index]->setPosition(Vec2(32.0f + (float(x - GridWidth / 2) + 0.5f) * 160.0f, float(-y) * 80.0f));
	}

	this->frame->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f));

	this->returnButton->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f - 336.0f));
	this->asciiTableTitle->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f + 380.0f));
	this->chooseANewValueTitle->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f + 380.0f));
}

void AsciiTable::initializeListeners()
{
	super::initializeListeners();

	for (auto block : this->asciiLetters)
	{
		block->getBlock()->setMouseClickCallback([=](InputEvents::MouseEventArgs*)
		{
			this->select(block);
		});
	}

	this->returnButton->setMouseClickCallback([=](InputEvents::MouseEventArgs*)
	{
		this->close();
	});
}

void AsciiTable::onAnyStateChange(CipherState* cipherState)
{
	super::onAnyStateChange(cipherState);
}

void AsciiTable::open(ImmediateBlock* immediateBlock, std::function<void()> onCloseCallback)
{
	this->immediateBlock = immediateBlock;
	this->onCloseCallback = onCloseCallback;

	this->setVisible(true);
	GameUtils::focus(this);

	if (this->immediateBlock == nullptr)
	{
		this->asciiTableTitle->setVisible(true);
		this->chooseANewValueTitle->setVisible(false);
		this->selectionSprite->setVisible(false);

		for (auto next : this->asciiLetters)
		{
			next->getBlock()->disableInteraction();
		}
	}
	else
	{
		this->asciiTableTitle->setVisible(false);
		this->chooseANewValueTitle->setVisible(true);
		this->selectionSprite->setVisible(true);

		for (auto next : this->asciiLetters)
		{
			next->getBlock()->enableInteraction();
		}
	}
	
	this->select(this->immediateBlock);
}

void AsciiTable::select(ImmediateBlock* block)
{
	this->selectedBlock = nullptr;

	if (block != nullptr)
	{
		for (auto next : this->asciiLetters)
		{
			if (next->getValue() == block->getValue())
			{
				this->selectedBlock = next;
			}
		}
		
		if (this->selectedBlock != nullptr)
		{
			this->selectionSprite->setPosition(this->selectedBlock->getPosition());
		}
	}
}

void AsciiTable::close()
{
	this->setVisible(false);
	GameUtils::focus(this->getParent());
	
	if (this->immediateBlock != nullptr && this->selectedBlock != nullptr)
	{
		this->immediateBlock->setValue(this->selectedBlock->getValue());
	}

	if (this->onCloseCallback != nullptr)
	{
		this->onCloseCallback();
	}
}
