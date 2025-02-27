#pragma once

#include "Engine/SmartNode.h"

namespace cocos2d
{
	class Sprite;
}

class ConstantString;
class LocalizedLabel;

class StackBlock : public SmartNode
{
public:
	static StackBlock* create();

	void clearHighlights();
	void highlightTopPlus1();
	void highlightTop();
	void clear();
	void push(unsigned long long value);
	unsigned long long pop();

	void show();

protected:
	StackBlock();
	virtual ~StackBlock();
	
	void onEnter() override;
	void initializePositions() override;

private:
	typedef SmartNode super;

	cocos2d::Sprite* stackBlock = nullptr;
	cocos2d::Sprite* stackSelector = nullptr;
	LocalizedLabel* stackTitleLabel = nullptr;

	std::vector<unsigned long long> values;
	std::vector<ConstantString*> labelStrings;
	std::vector<LocalizedLabel*> labels;

	static const int StackSize;
};
