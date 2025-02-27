#include "XorBlock.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Scenes/Cipher/CipherConfig.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

XorBlock* XorBlock::create(BlockType blockType)
{
	XorBlock* instance = new XorBlock(blockType);

	instance->autorelease();

	return instance;
}

XorBlock::XorBlock(BlockType blockType) : super(blockType, ConnectionType::Double, ConnectionType::Single, ClickableNode::create(CipherResources::Blocks_BlockBin, CipherResources::Blocks_BlockBin), CipherResources::Icons_LogicalXor, Strings::Cipher_Operations_XOR::create())
{
}

XorBlock::~XorBlock()
{
}

void XorBlock::onEnter()
{
	super::onEnter();
}

void XorBlock::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();
}

unsigned char XorBlock::compute()
{
	return this->inputLeft ^ this->inputRight;
}
