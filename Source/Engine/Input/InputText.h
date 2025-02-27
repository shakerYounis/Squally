#pragma once

#include "cocos/ui/UITextField.h"

#include "Engine/Localization/LocalizedLabel.h"

class ConstantString;
class ClickableNode;

class InputText : public cocos2d::ui::UICCTextField
{
public:
	static InputText* create(cocos2d::CSize minimumInputSize, LocalizedLabel::FontStyle fontStyle, LocalizedLabel::FontSize fontSize, bool unfuck = false);

	void focus();
	void unfocus();
	void setString(const std::string& label) override;
	void setStringChangeCallback(std::function<void(std::string)> stringChangeCallback);
	std::string getFont();
	float getFontSize();
	ClickableNode* getHitbox();

private:
	typedef UICCTextField super;
	InputText(cocos2d::CSize minimumInputSize, LocalizedLabel::FontStyle fontStyle, LocalizedLabel::FontSize fontSize, bool unfuck = false);
	virtual ~InputText();

	void onEnter() override;
	void update(float dt) override;
	void initializePositions();
	cocos2d::CSize resize();

	// This is a hacky fix to some weird scenario where the position changes when there is input in the textfield
	bool unfuck = false;
	std::string stringCache;
	ConstantString* labelText = nullptr;
	LocalizedLabel* inputLabel = nullptr;
	ClickableNode* hitBox = nullptr;
	std::function<void(std::string)> stringChangeCallback = nullptr;

	cocos2d::Vec2 initCoords;
	cocos2d::CSize minimumInputSize;
};
