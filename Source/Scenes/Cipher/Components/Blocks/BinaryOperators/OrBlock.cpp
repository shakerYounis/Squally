#include "OrBlock.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Scenes/Cipher/CipherConfig.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

OrBlock* OrBlock::create(BlockType blockType)
{
	OrBlock* instance = new OrBlock(blockType);

	instance->autorelease();

	return instance;
}

OrBlock::OrBlock(BlockType blockType) : super(blockType, ConnectionType::Double, ConnectionType::Single, ClickableNode::create(CipherResources::Blocks_BlockBin, CipherResources::Blocks_BlockBin), CipherResources::Icons_LogicalOr, Strings::Cipher_Operations_OR::create())
{
}

OrBlock::~OrBlock()
{
}

void OrBlock::onEnter()
{
	super::onEnter();
}

void OrBlock::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();
}

unsigned char OrBlock::compute()
{
	return this->inputLeft | this->inputRight;
}
