#include "Reflect.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Hackables/HackableObject.h"
#include "Engine/Hackables/Menus/HackablePreview.h"
#include "Engine/Optimization/LazyNode.h"
#include "Engine/Particles/SmartParticles.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Events/PlatformerEvents.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/Reflect/ReflectGenericPreview.h"
#include "Scenes/Platformer/Level/Combat/CombatMap.h"
#include "Scenes/Platformer/Level/Combat/TimelineEvent.h"
#include "Scenes/Platformer/Level/Combat/TimelineEventGroup.h"

#include "Resources/FXResources.h"
#include "Resources/ObjectResources.h"
#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_REFLECT 1

const std::string Reflect::ReflectIdentifier = "reflect";

const int Reflect::MinMultiplier = 2;
const int Reflect::MaxMultiplier = 2;
const float Reflect::Duration = 10.0f;
	
const std::string Reflect::StateKeyDamageReflected = "ANTI_OPTIMIZE_STATE_KEY_DAMAGE_REFLECTED";

Reflect* Reflect::create(PlatformerEntity* caster, PlatformerEntity* target)
{
	Reflect* instance = new Reflect(caster, target);

	instance->autorelease();

	return instance;
}

Reflect::Reflect(PlatformerEntity* caster, PlatformerEntity* target)
	: super(caster, target, UIResources::Menus_Icons_ShieldMagic, AbilityType::Arcane, BuffData(Reflect::Duration, Reflect::ReflectIdentifier))
{
	this->spellEffect = SmartParticles::create(ParticleResources::Platformer_Combat_Abilities_Speed);
	this->bubble = Sprite::create(FXResources::Auras_DefendAura);
	this->spellAura = Sprite::create(FXResources::Auras_ChantAura2);

	this->bubble->setOpacity(0);
	this->spellAura->setColor(Color3B::BLUE);
	this->spellAura->setOpacity(0);

	this->addChild(this->spellEffect);
	this->addChild(this->bubble);
	this->addChild(this->spellAura);
}

Reflect::~Reflect()
{
}

void Reflect::onEnter()
{
	super::onEnter();

	this->spellEffect->start();

	this->bubble->runAction(FadeTo::create(0.25f, 255));

	this->spellEffect->setPositionY(this->owner->getEntityBottomPointRelative().y);
	this->spellAura->runAction(Sequence::create(
		FadeTo::create(0.25f, 255),
		DelayTime::create(0.5f),
		FadeTo::create(0.25f, 0),
		nullptr
	));

	CombatEvents::TriggerHackableCombatCue();
}

void Reflect::initializePositions()
{
	super::initializePositions();
}

void Reflect::registerHackables()
{
	super::registerHackables();

	if (this->owner == nullptr)
	{
		return;
	}

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_REFLECT,
			HackableCode::HackableCodeInfo(
				Reflect::ReflectIdentifier,
				Strings::Menus_Hacking_Abilities_Buffs_Reflect_Reflect::create(),
				HackableBase::HackBarColor::Blue,
				UIResources::Menus_Icons_ShieldMagic,
				LazyNode<HackablePreview>::create([=](){ return ReflectGenericPreview::create(); }),
				{
					{
						HackableCode::Register::zbx, Strings::Menus_Hacking_Abilities_Buffs_Reflect_RegisterEbx::create()->setStringReplacementVariables(
							{
								Strings::Common_ConstantTimes::create()->setStringReplacementVariables(ConstantString::create(std::to_string(-Reflect::MinMultiplier))),
								Strings::Common_ConstantTimes::create()->setStringReplacementVariables(ConstantString::create(std::to_string(Reflect::MaxMultiplier)))
							}),
					},
					{
						HackableCode::Register::zsi, Strings::Menus_Hacking_Abilities_Buffs_Reflect_RegisterEdi::create(),
					}
				},
				int(HackFlags::None),
				this->getRemainingDuration(),
				0.0f,
				{
					HackableCode::ReadOnlyScript(
						Strings::Menus_Hacking_CodeEditor_OriginalCode::create(),
						// x86
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentShr::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentShrBy1::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentElaborate::create()) +
						"shr esi, 1\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentReflect::create()
							->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterEbx::create())) +
						"mov ebx, esi\n\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentHint::create()
							->setStringReplacementVariables({ Strings::Menus_Hacking_Lexicon_Assembly_RegisterEbx::create(), Strings::Menus_Hacking_Lexicon_Assembly_RegisterEsi::create() }))
						
						, // x64
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentShr::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentShrBy1::create()) +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentElaborate::create()) +
						"shr rsi, 1\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentReflect::create()
							->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterRbx::create())) +
						"mov rbx, rsi\n\n" +
						COMMENT(Strings::Menus_Hacking_Abilities_Buffs_Reflect_CommentHint::create()
							->setStringReplacementVariables({ Strings::Menus_Hacking_Lexicon_Assembly_RegisterRbx::create(), Strings::Menus_Hacking_Lexicon_Assembly_RegisterRsi::create() }))
					),
				},
				true
			)
		},
	};

	auto func = &Reflect::applyReflect;
	this->hackables = HackableCode::create((void*&)func, codeInfoMap);

	for (HackableCode* next : this->hackables)
	{
		this->owner->registerCode(next);
	}
}

void Reflect::onBeforeDamageTaken(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing)
{
	super::onBeforeDamageTaken(damageOrHealing);

	this->HackStateStorage[Reflect::StateKeyDamageReflected] = Value(damageOrHealing->damageOrHealingValue);
	this->HackStateStorage[Buff::StateKeyDamageDealt] = Value(damageOrHealing->damageOrHealingValue);

	this->applyReflect();

	// Bound multiplier in either direction
	this->HackStateStorage[Reflect::StateKeyDamageReflected] = Value(MathUtils::clamp(
		this->HackStateStorage[Reflect::StateKeyDamageReflected].asInt(),
		-std::abs(this->HackStateStorage[Buff::StateKeyOriginalDamageOrHealing].asInt() * Reflect::MinMultiplier),
		std::abs(this->HackStateStorage[Buff::StateKeyOriginalDamageOrHealing].asInt() * Reflect::MaxMultiplier)
	));
	this->HackStateStorage[Buff::StateKeyDamageDealt] = Value(MathUtils::clamp(
		this->HackStateStorage[Buff::StateKeyDamageDealt].asInt(),
		-std::abs(this->HackStateStorage[Buff::StateKeyOriginalDamageOrHealing].asInt() * Reflect::MinMultiplier),
		std::abs(this->HackStateStorage[Buff::StateKeyOriginalDamageOrHealing].asInt() * Reflect::MaxMultiplier)
	));
	
	*(int*)(GameUtils::getKeyOrDefault(this->HackStateStorage, Buff::StateKeyDamageOrHealingPtr, Value(nullptr)).asPointer()) = GameUtils::getKeyOrDefault(this->HackStateStorage, Buff::StateKeyDamageDealt, Value(0)).asInt();

	// Reflect damage back to attacker (do not let buffs process this damage -- two reflect spells could infinite loop otherwise)
	CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(damageOrHealing->target, damageOrHealing->caster, GameUtils::getKeyOrDefault(this->HackStateStorage, Reflect::StateKeyDamageReflected, Value(0)).asInt(), damageOrHealing->abilityType, true));
}

NO_OPTIMIZE void Reflect::applyReflect()
{
	static volatile int damageDealtLocal = 0;
	static volatile int damageReflectedLocal = 0;

	damageDealtLocal = GameUtils::getKeyOrDefault(this->HackStateStorage, Buff::StateKeyDamageDealt, Value(0)).asInt();
	damageReflectedLocal = GameUtils::getKeyOrDefault(this->HackStateStorage, Buff::StateKeyDamageDealt, Value(0)).asInt();
	
	ASM(push ZSI);
	ASM(push ZBX);

	ASM_MOV_REG_VAR(esi, damageDealtLocal);
	ASM_MOV_REG_VAR(ebx, damageReflectedLocal);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_REFLECT);
	ASM(shr ZSI, 1);
	ASM(mov ZBX, ZSI);
	ASM_NOP16();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(damageDealtLocal, esi);
	ASM_MOV_VAR_REG(damageReflectedLocal, ebx);

	ASM(pop ZBX);
	ASM(pop ZSI);

	this->HackStateStorage[Reflect::StateKeyDamageDealt] = Value(damageDealtLocal);
	this->HackStateStorage[Reflect::StateKeyDamageReflected] = Value(damageReflectedLocal);

	HACKABLES_STOP_SEARCH();
}
END_NO_OPTIMIZE
