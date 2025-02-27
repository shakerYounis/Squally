#pragma once

#include "Engine/Quests/QuestTask.h"

class MagePortal;
class PlatformerEntity;
class QuestLine;
class Squally;

class BeatTutorialA : public QuestTask
{
public:
	static BeatTutorialA* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	BeatTutorialA(GameObject* owner, QuestLine* questLine);
	virtual ~BeatTutorialA();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	MagePortal* portal = nullptr;
	PlatformerEntity* mage = nullptr;
	Squally* squally = nullptr;

	static const std::string QuestPortalTag;
	static const std::string WinLossTrackIdentifier;
};
