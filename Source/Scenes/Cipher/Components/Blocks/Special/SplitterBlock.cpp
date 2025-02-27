#include "SplitterBlock.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Scenes/Cipher/CipherConfig.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

SplitterBlock* SplitterBlock::create(BlockType blockType)
{
	SplitterBlock* instance = new SplitterBlock(blockType);

	instance->autorelease();

	return instance;
}

SplitterBlock::SplitterBlock(BlockType blockType) : super(blockType, ConnectionType::Single, ConnectionType::Double, ClickableNode::create(CipherResources::Blocks_BlockYellow, CipherResources::Blocks_BlockYellow), CipherResources::Icons_Splitter, Strings::Cipher_Operations_SHR::create())
{
}

SplitterBlock::~SplitterBlock()
{
}

void SplitterBlock::onEnter()
{
	super::onEnter();
}

void SplitterBlock::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();
}

unsigned char SplitterBlock::compute()
{
	return this->inputLeft;
}
