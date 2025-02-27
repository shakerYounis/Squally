#include "BruteForceBlock.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableNode.h"
#include "Scenes/Cipher/CipherConfig.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

BruteForceBlock* BruteForceBlock::create(BlockType blockType)
{
	BruteForceBlock* instance = new BruteForceBlock(blockType);

	instance->autorelease();

	return instance;
}

BruteForceBlock::BruteForceBlock(BlockType blockType) : super(blockType, ConnectionType::None, ConnectionType::Single, ClickableNode::create(CipherResources::Blocks_BlockRed, CipherResources::Blocks_BlockRed), CipherResources::Icons_BruteForce, Strings::Cipher_Operations_BruteForce::create())
{
}

BruteForceBlock::~BruteForceBlock()
{
}

void BruteForceBlock::onEnter()
{
	super::onEnter();
}

void BruteForceBlock::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();
}

unsigned char BruteForceBlock::compute()
{
	return (unsigned char)(0);
}
