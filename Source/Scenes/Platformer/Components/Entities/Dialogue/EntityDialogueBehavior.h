#pragma once

#include <queue>

#include "Engine/Components/GameComponent.h"
#include "Events/DialogueEvents.h"

class DialogueSet;
class InteractMenu;
class InteractObject;
template <class T> class LazyNode;
class LocalizedString;
class PlatformerEntity;
class Scrappy;
class SpeechBubble;

class EntityDialogueBehavior : public GameComponent
{
public:
	static EntityDialogueBehavior* create(GameObject* owner);

	void enqueuePretext(DialogueEvents::DialogueOpenArgs pretext);
	void clearPretext();
	void setActiveDialogueSet(DialogueSet* dialogueSet, bool showDialogue = true);
	void addDialogueSet(DialogueSet* dialogueSet);
	void removeDialogueSet(DialogueSet* dialogueSet);
	DialogueSet* getMainDialogueSet();
	SpeechBubble* getSpeechBubble();

	static const std::string MapKey;

protected:
	EntityDialogueBehavior(GameObject* owner);
	virtual ~EntityDialogueBehavior();

	void initializePositions() override;
	void onLoad() override;
	void onDisable() override;
	void update(float dt) override;

private:
	typedef GameComponent super;

	void onInteract();
	void progressDialogue();
	void showOptions();
	bool hasDialogueOptions();
	void updateInteractable();
	LocalizedString* getOptionString(int index, LocalizedString* optionText);
	SpeechBubble* buildSpeechBubble();

	PlatformerEntity* entity = nullptr;
	Scrappy* scrappy = nullptr;
	LazyNode<SpeechBubble>* speechBubble = nullptr;
	InteractObject* dialogueInteraction = nullptr;
	bool optionsVisible = false;
	
	std::queue<DialogueEvents::DialogueOpenArgs> pretextQueue;
	std::vector<DialogueSet*> dialogueSets;
	DialogueSet* mainDialogueSet = nullptr;
	DialogueSet* activeDialogueSet = nullptr;
	cocos2d::Node* pretextNode = nullptr;
	cocos2d::Node* dialogueSetNode = nullptr;
};
