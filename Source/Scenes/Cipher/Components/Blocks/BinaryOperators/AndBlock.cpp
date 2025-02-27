#include "AndBlock.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Scenes/Cipher/CipherConfig.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

AndBlock* AndBlock::create(BlockType blockType)
{
	AndBlock* instance = new AndBlock(blockType);

	instance->autorelease();

	return instance;
}

AndBlock::AndBlock(BlockType blockType) : super(blockType, ConnectionType::Double, ConnectionType::Single, ClickableNode::create(CipherResources::Blocks_BlockBin, CipherResources::Blocks_BlockBin), CipherResources::Icons_LogicalAnd, Strings::Cipher_Operations_AND::create())
{
}

AndBlock::~AndBlock()
{
}

void AndBlock::onEnter()
{
	super::onEnter();
}

void AndBlock::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();
}

unsigned char AndBlock::compute()
{
	return this->inputLeft & this->inputRight;
}
