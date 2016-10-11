#include "skse/GameEvents.h" //dispatchers and structs
#include "skse/GameRTTI.h" //DYNAMIC_CAST
#include "skse/PapyrusArgs.h"
#include "skse/Utilities.h"	//DumpClass etc

#include "Events.h"
#include "Hooks.h"


template <typename T> void SetVMValue(VMValue * val, T arg)
{
	VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
	PackValue(val, &arg, registry);
}

template <> void SetVMValue<bool> (VMValue * val, bool arg)						{ val->SetBool(arg); }
template <> void SetVMValue<SInt32> (VMValue * val, SInt32 arg)					{ val->SetInt(arg); }
template <> void SetVMValue<float> (VMValue * val, float arg)					{ val->SetFloat(arg); }
template <> void SetVMValue<BSFixedString> (VMValue * val, BSFixedString arg)	{ val->SetString(arg.data); }

// ======================================================
// Generic Functors
// ======================================================

class EventQueueFunctor0 : public IFunctionArguments
{
public:
	EventQueueFunctor0(BSFixedString & a_eventName)
		: eventName(a_eventName.data) {}

	virtual bool	Copy(Output * dst)
	{
		dst->Resize(0);
		return true;
	}

	void			operator() (const EventRegistration<NullParameters> & reg)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		registry->QueueEvent(reg.handle, &eventName, this);
	}

private:
	BSFixedString	eventName;
};

template <typename T1>
class EventQueueFunctor1 : public IFunctionArguments
{
public:
	EventQueueFunctor1(BSFixedString & a_eventName, T1 a_arg1)
		: eventName(a_eventName.data), arg1(a_arg1) {}

	virtual bool	Copy(Output * dst)
	{
		dst->Resize(1);
		SetVMValue(dst->Get(0), arg1);

		return true;
	}

	void			operator() (const EventRegistration<NullParameters> & reg)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		registry->QueueEvent(reg.handle, &eventName, this);
	}

private:
	BSFixedString	eventName;
	T1				arg1;
};

template <typename T1, typename T2>
class EventQueueFunctor2 : public IFunctionArguments
{
public:
	EventQueueFunctor2(BSFixedString & a_eventName, T1 a_arg1, T2 a_arg2)
		: eventName(a_eventName.data), arg1(a_arg1), arg2(a_arg2) {}

	virtual bool	Copy(Output * dst)
	{
		dst->Resize(2);
		SetVMValue(dst->Get(0), arg1);
		SetVMValue(dst->Get(1), arg2);

		return true;
	}

	void			operator() (const EventRegistration<NullParameters> & reg)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		registry->QueueEvent(reg.handle, &eventName, this);
	}

private:
	BSFixedString	eventName;
	T1				arg1;
	T2				arg2;
};

// ======================================================
// ReceiveEvent
// ======================================================

namespace newEvents
{

EventResult FurnitureEventHandler::ReceiveEvent(TESFurnitureEvent * evn, EventDispatcher<TESFurnitureEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: FurnitureEvent"); //OnSit, OnGetUp

	BSFixedString eventName = evn->state ? BSFixedString("OnFurnitureLeave") : BSFixedString("OnFurnitureEnter");
	Actor* pActor = DYNAMIC_CAST(evn->character, TESObjectREFR, Actor);

	g_furnitureEventRegs.ForEach(
		EventQueueFunctor2<Actor*, TESObjectREFR*>(eventName, pActor, evn->furniture)
	);

    return kEvent_Continue;
}

EventResult LockChangedEventHandler::ReceiveEvent(TESLockChangedEvent * evn, EventDispatcher<TESLockChangedEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: LockChanged"); //OnLockStateChanged

	if (evn->unlocker == *g_thePlayer)
		g_lockPicked = evn->lock;
	/*
	g_lockChangedEventRegs.ForEach(
		EventQueueFunctor2<TESObjectREFR*, TESObjectREFR*>(BSFixedString("OnLockChanged"), evn->lock, evn->unlocker)
	);
	*/
    return kEvent_Continue;
}

EventResult ObjectLoadedEventHandler::ReceiveEvent(TESObjectLoadedEvent * evn, EventDispatcher<TESObjectLoadedEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: ObjectLoaded");

	BSFixedString eventName = (evn->loaded) ? BSFixedString("OnObjectLoaded") : BSFixedString("OnObjectUnloaded");

	TESObjectREFR* object = DYNAMIC_CAST(LookupFormByID(evn->formId), TESForm, TESObjectREFR);
	UInt32 type = object->baseForm->GetFormType();

	g_objectLoadedEventRegs.ForEach(
		type,
		EventQueueFunctor1<TESObjectREFR*>(eventName, object)
	);

	//_MESSAGE("ReceiveEvent: ObjectLoaded (type=%i)", type);

    return kEvent_Continue;
}

EventResult QuestStageEventHandler::ReceiveEvent(TESQuestStageEvent * evn, EventDispatcher<TESQuestStageEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: QuestStage");
	
	TESQuest* pQuest = DYNAMIC_CAST(LookupFormByID(evn->formId), TESForm, TESQuest);

	g_questStageEventRegs.ForEach(
		EventQueueFunctor2<TESQuest*, UInt32>(BSFixedString("OnQuestStageChanged"), pQuest, evn->stage)
    );

    return kEvent_Continue;
}

EventResult WaitStartEventHandler::ReceiveEvent(TESWaitStartEvent * evn, EventDispatcher<TESWaitStartEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: WaitStart");

	g_waitEventRegs.ForEach(
		EventQueueFunctor2<float, float>(BSFixedString("OnWaitStart"), evn->current, evn->desired)
    );

    return kEvent_Continue;
}

EventResult WaitStopEventHandler::ReceiveEvent(TESWaitStopEvent * evn, EventDispatcher<TESWaitStopEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: WaitStop");

	g_waitEventRegs.ForEach(
		EventQueueFunctor0(BSFixedString("OnWaitStop"))
    );

    return kEvent_Continue;
}

EventResult ItemHarvestEventHandler::ReceiveEvent(TESItemHarvestEvent * evn, EventDispatcher<TESItemHarvestEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: ItemHarvest");

	g_itemHarvestEventRegs.ForEach(
		EventQueueFunctor1<TESForm*>(BSFixedString("OnItemHarvested"), evn->object) //PlayerOnly
    );  

    return kEvent_Continue;
}

EventResult LocationClearedEventHandler::ReceiveEvent(TESLocationClearedEvent * evn, EventDispatcher<TESLocationClearedEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: LocationCleared");

	g_locationClearedEventRegs.ForEach(
		EventQueueFunctor1<BGSLocation*>(BSFixedString("OnLocationCleared"), g_locationCleared)
    );

    return kEvent_Continue;
}

EventResult BookReadEventHandler::ReceiveEvent(TESBookReadEvent * evn, EventDispatcher<TESBookReadEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: BookRead"); //OnRead

    g_bookReadEventRegs.ForEach(
		EventQueueFunctor1<TESObjectBOOK*>(BSFixedString("OnBookRead"), evn->book)
    );  

    return kEvent_Continue;
}

EventResult SkillIncreaseEventHandler::ReceiveEvent(TESSkillIncreaseEvent * evn, EventDispatcher<TESSkillIncreaseEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: SkillIncrease");
	
    g_skillIncreaseEventRegs.ForEach(
		EventQueueFunctor1<UInt32>(BSFixedString("OnSkillIncrease"), evn->avId)
    );    
    return kEvent_Continue;
}

EventResult LevelIncreaseEventHandler::ReceiveEvent(TESLevelIncreaseEvent * evn, EventDispatcher<TESLevelIncreaseEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: LevelIncrease");

    g_levelIncreaseEventRegs.ForEach(
        EventQueueFunctor1<UInt32>(BSFixedString("OnLevelIncrease"), evn->level)
    );    
    return kEvent_Continue;
}

EventResult LocationDiscoveryEventHandler::ReceiveEvent(TESLocationDiscoveryEvent * evn, EventDispatcher<TESLocationDiscoveryEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: LocationDiscovery");

	BSFixedString pName = evn->fullname->name;
	
    g_locationDiscoveryEventRegs.ForEach(
		EventQueueFunctor1<BSFixedString>(BSFixedString("OnLocationDiscovery"), pName)
    );

    return kEvent_Continue;
}

EventResult QuestStatusEventHandler::ReceiveEvent(TESQuestStatusEvent * evn, EventDispatcher<TESQuestStatusEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: QuestStatus");

	BSFixedString eventName = evn->status ? BSFixedString("OnQuestStarted") : BSFixedString("OnQuestCompleted");

    g_questStatusEventRegs.ForEach(
		 EventQueueFunctor1<TESQuest*>(eventName, evn->quest)
    );

    return kEvent_Continue;
}

EventResult ActorKillEventHandler::ReceiveEvent(TESActorKillEvent * evn, EventDispatcher<TESActorKillEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: ActorKill"); //OnDeath,

	Actor* pVictim = DYNAMIC_CAST(evn->victim, TESObjectREFR, Actor);
	Actor* pKiller = DYNAMIC_CAST(evn->killer, TESObjectREFR, Actor);

	g_actorKillEventRegs.ForEach(
		EventQueueFunctor2<Actor*, Actor*>(BSFixedString("OnActorKilled"), pVictim, pKiller)
	);

    return kEvent_Continue;
}


EventResult CriticalHitEventHandler::ReceiveEvent(TESCriticalHitEvent * evn, EventDispatcher<TESCriticalHitEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: CriticalHit");

	Actor* pSource = DYNAMIC_CAST(evn->character, TESForm, Actor);

    g_criticalHitEventRegs.ForEach(
		EventQueueFunctor2<Actor*, TESObjectWEAP*>(BSFixedString("OnCriticalHit"), pSource, evn->weapon) //PlayerOnly ???
    );  

    return kEvent_Continue;
}

EventResult DisarmedEventHandler::ReceiveEvent(TESDisarmedEvent * evn, EventDispatcher<TESDisarmedEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: Disarmed");

	Actor* pTarget = DYNAMIC_CAST(evn->target, TESObjectREFR, Actor);
	Actor* pSource = DYNAMIC_CAST(evn->source, TESObjectREFR, Actor);

    g_disarmedEventRegs.ForEach(
		EventQueueFunctor2<Actor*, Actor*>(BSFixedString("OnDisarmed"), pTarget, pSource) //Player+NPC
    );  

    return kEvent_Continue;
}

EventResult WordLearnedEventHandler::ReceiveEvent(TESWordLearnedEvent * evn, EventDispatcher<TESWordLearnedEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: WordLearned");

    g_wordLearnedEventRegs.ForEach(
		EventQueueFunctor1<TESWordOfPower*>(BSFixedString("OnWordLearned"), evn->word) //PlayerOnly
    );  

    return kEvent_Continue;
}

EventResult SpellLearnedEventHandler::ReceiveEvent(TESSpellLearnedEvent * evn, EventDispatcher<TESSpellLearnedEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: SpellLearned");

	SpellItem* spell = *(SpellItem**)((char*)evn + 29);

    g_spellLearnedEventRegs.ForEach(
		EventQueueFunctor1<SpellItem*>(BSFixedString("OnSpellLearned"), spell) //PlayerOnly
    );  

    return kEvent_Continue;
}

EventResult DragonSoulGainedEventHandler::ReceiveEvent(TESDragonSoulGainedEvent * evn, EventDispatcher<TESDragonSoulGainedEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: DragonSoulGained");

    g_dragonSoulGainedEventRegs.ForEach(
		EventQueueFunctor1<float>(BSFixedString("OnDragonSoulGained"), evn->souls) //PlayerOnly
    );  

    return kEvent_Continue;
}

EventResult SoulTrappedEventHandler::ReceiveEvent(TESSoulTrappedEvent * evn, EventDispatcher<TESSoulTrappedEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: SoulTrapped");

	Actor* pTarget = DYNAMIC_CAST(evn->target, TESObjectREFR, Actor);
	Actor* pPlayer = DYNAMIC_CAST(evn->player, TESObjectREFR, Actor);

    g_soulTrappedEventRegs.ForEach(
		EventQueueFunctor2<Actor*, Actor*>(BSFixedString("OnSoulTrapped"), pTarget, pPlayer)
    );  

    return kEvent_Continue;
}

EventResult ShoutAttackEventHandler::ReceiveEvent(TESShoutAttackEvent * evn, EventDispatcher<TESShoutAttackEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: ShoutAttack");

    g_shoutAttackEventRegs.ForEach(
		EventQueueFunctor1<TESShout*>(BSFixedString("OnPlayerShoutAttack"), evn->shout) //PlayerOnly
    );  

    return kEvent_Continue;
}

EventResult LockPickedEventHandler::ReceiveEvent(TESLockPickedEvent * evn, EventDispatcher<TESLockPickedEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: LockPicked");

	g_lockPickedEventRegs.ForEach(
		EventQueueFunctor1<TESObjectREFR*>(BSFixedString("OnLockPicked"), g_lockPicked) //PlayerOnly
	);

    return kEvent_Continue;
}

EventResult GrandTheftHorseEventHandler::ReceiveEvent(TESGrandTheftHorseEvent * evn, EventDispatcher<TESGrandTheftHorseEvent> * dispatcher)
{
	_DMESSAGE("ReceiveEvent: GrandTheftHorse");

	TESObjectREFR* object = *(TESObjectREFR**)((char*)evn + 13);
	Actor* pHorse = DYNAMIC_CAST(object, TESObjectREFR, Actor);

	g_grandTheftHorseEventRegs.ForEach(
		EventQueueFunctor1<Actor*>(BSFixedString("OnHorseSteal"), pHorse)
	);

    return kEvent_Continue;
}

}

// ======================================================
// Registration holders
// ======================================================

namespace newEvents
{

RegistrationSetHolder<NullParameters>	g_furnitureEventRegs;
RegistrationSetHolder<NullParameters>	g_lockChangedEventRegs;
RegistrationMapHolder<UInt32>			g_objectLoadedEventRegs;
RegistrationSetHolder<NullParameters>	g_questStageEventRegs;
RegistrationSetHolder<NullParameters>	g_waitEventRegs;

RegistrationSetHolder<NullParameters>	g_itemHarvestEventRegs;
RegistrationSetHolder<NullParameters>	g_locationClearedEventRegs;
RegistrationSetHolder<NullParameters>	g_bookReadEventRegs;
RegistrationSetHolder<NullParameters>	g_skillIncreaseEventRegs;
RegistrationSetHolder<NullParameters>	g_levelIncreaseEventRegs;
RegistrationSetHolder<NullParameters>	g_locationDiscoveryEventRegs;
RegistrationSetHolder<NullParameters>	g_questStatusEventRegs;
RegistrationSetHolder<NullParameters>	g_actorKillEventRegs;
RegistrationSetHolder<NullParameters>	g_criticalHitEventRegs;
RegistrationSetHolder<NullParameters>	g_disarmedEventRegs;
RegistrationSetHolder<NullParameters>	g_wordLearnedEventRegs;
RegistrationSetHolder<NullParameters>	g_spellLearnedEventRegs;
RegistrationSetHolder<NullParameters>	g_dragonSoulGainedEventRegs;
RegistrationSetHolder<NullParameters>	g_soulTrappedEventRegs;
RegistrationSetHolder<NullParameters>	g_shoutAttackEventRegs;
RegistrationSetHolder<NullParameters>	g_lockPickedEventRegs;
RegistrationSetHolder<NullParameters>	g_grandTheftHorseEventRegs;

}

// ======================================================
// Event handlers
// ======================================================

namespace newEvents
{

FurnitureEventHandler			g_furnitureEventHandler;
LockChangedEventHandler			g_lockChangedEventHandler;
ObjectLoadedEventHandler		g_objectLoadedEventHandler;
QuestStageEventHandler			g_questStageEventHandler;
WaitStartEventHandler			g_waitStartEventHandler;
WaitStopEventHandler			g_waitStopEventHandler;

ItemHarvestEventHandler			g_itemHarvestEventHandler;
LocationClearedEventHandler		g_locationClearedEventHandler;
BookReadEventHandler			g_bookReadEventHandler;
SkillIncreaseEventHandler		g_skillIncreaseEventHandler;
LevelIncreaseEventHandler		g_levelIncreaseEventHandler;
LocationDiscoveryEventHandler	g_locationDiscoveryEventHandler;
QuestStatusEventHandler			g_questStatusEventHandler;
ActorKillEventHandler			g_actorKillEventHandler;
CriticalHitEventHandler			g_criticalHitEventHandler;
DisarmedEventHandler			g_disarmedEventHandler;
WordLearnedEventHandler			g_wordLearnedEventHandler;
SpellLearnedEventHandler		g_spellLearnedEventHandler;
DragonSoulGainedEventHandler	g_dragonSoulGainedEventHandler;
SoulTrappedEventHandler			g_soulTrappedEventHandler;
ShoutAttackEventHandler			g_shoutAttackEventHandler;
LockPickedEventHandler			g_lockPickedEventHandler;
GrandTheftHorseEventHandler		g_grandTheftHorseEventHandler;

}

// ======================================================
// Event dispatchers
// ======================================================

namespace newEvents
{

// TESActivateEvent																														0x012E4C60
// TESActiveEffectApplyRemoveEvent																										0x012E4C90
// TESActorLocationChangeEvent																											0x012E4CC0
// TESBookReadEvent																														0x012E4CF0
// TESCellAttachDetachEvent																												0x012E4D20
// TESCellFullyLoadedEvent																												0x012E4D50
// TESCell...																															0x012E4D80 //new
// EventDispatcher<TESCombatEvent>* g_combatEventDispatcher = (EventDispatcher<TESCombatEvent>*)										0x012E4DB0;
// EventDispatcher<TESContainerChangedEvent>* g_containerChangedEventDispatcher = (EventDispatcher<TESContainerChangedEvent>*)			0x012E4DE0;
// EventDispatcher<TESDeathEvent>* g_deathEventDispatcher = (EventDispatcher<TESDeathEvent>*)											0x012E4E10;
// TESDestructionStageChangedEvent																										0x012E4E40
// TESEnterBleedoutEvent																												0x012E4E70
// TESEquipEvent																														0x012E4EA0
// TESFormDeleteEvent																													0x012E4ED0
EventDispatcher<TESFurnitureEvent>* g_furnitureEventDispatcher = (EventDispatcher<TESFurnitureEvent>*)									0x012E4F00;
// TESGrabReleaseEvent																													0x012E4F30
// EventDispatcher<TESHitEvent>* g_hitEventDispatcher = (EventDispatcher<TESHitEvent>*)													0x012E4F60;
// EventDispatcher<TESInitScriptEvent>* g_initScriptEventDispatcher = (EventDispatcher<TESInitScriptEvent>*)							0x012E4F90;
// TESLoadGameEvent																														0x012E4FC0
EventDispatcher<TESLockChangedEvent>* g_lockChangedEventDispatcher = (EventDispatcher<TESLockChangedEvent>*)							0x012E4FF0;
// TESMagicEffectApplyEvent																												0x012E5020
// TESMagicWardHitEvent																													0x012E5050
// TESMoveAttachDetachEvent																												0x012E5080
EventDispatcher<TESObjectLoadedEvent>* g_objectLoadedEventDispatcher = (EventDispatcher<TESObjectLoadedEvent>*)							0x012E50B0;
// TESObjectREFRTranslationEvent																										0x012E50E0
// TESOpenCloseEvent																													0x012E5110
// TESPackageEvent																														0x012E5140
// TESPerkEntryRunEvent																													0x012E5170
// TESQuestInitEvent																													0x012E51A0
EventDispatcher<TESQuestStageEvent>* g_questStageEventDispatcher = (EventDispatcher<TESQuestStageEvent>*)								0x012E51D0;
// UNK																																	0x012E5200 //new
// UNK																																	0x012E5230 //new
// TESResetEvent																														0x012E5260
// TESResolveNPCTemplatesEvent																											0x012E5290
// TESSceneEvent																														0x012E52C0
// TESSceneActionEvent																													0x012E52F0
// TESScenePhaseEvent																													0x012E5320
// TESSellEvent																															0x012E5350
EventDispatcher<TESSleepStartEvent>* g_sleepStartEventDispatcher = (EventDispatcher<TESSleepStartEvent>*)								0x012E4580;
// TESSleepStopEvent																													0x012E53B0
// TESSpellCastEvent																													0x012E53E0
// TESPlayerBowShotEvent																												0x012E5410
// TESTopicInfoEvent																													0x012E5440
// TESTrackedStatsEvent																													0x012E5470
// TESTrapHitEvent																														0x012E54A0
// TESTriggerEvent																														0x012E54D0
// TESTriggerEnterEvent																													0x012E5500
// TESTriggerLeaveEvent																													0x012E5530
// EventDispatcher<TESUniqueIDChangeEvent>* g_changeUniqueIDEventDispatcher = (EventDispatcher<TESUniqueIDChangeEvent>*)				0x012E5560;
EventDispatcher<TESWaitStartEvent>* g_waitStartEventDispatcher = (EventDispatcher<TESWaitStartEvent>*)									0x012E5590; //new
EventDispatcher<TESWaitStopEvent>* g_waitStopEventDispatcher =	(EventDispatcher<TESWaitStopEvent>*) 									0x012E55C0; //new
// TESSwitchRaceCompleteEvent																											0x012E55F0


EventDispatcher<TESItemHarvestEvent>* g_itemHarvestEventDispatcher = (EventDispatcher<TESItemHarvestEvent>*)							0x012E5A74;
EventDispatcher<TESLocationClearedEvent>* g_locationClearedEventDispatcher = (EventDispatcher<TESLocationClearedEvent>*)				0x012E7610;
EventDispatcher<TESBookReadEvent>* g_bookReadEventDispatcher = (EventDispatcher<TESBookReadEvent>*)										0x012E736C;
EventDispatcher<TESSkillIncreaseEvent>* g_skillIncreaseEventDispatcher = (EventDispatcher<TESSkillIncreaseEvent>*)						0x01B397D0;
EventDispatcher<TESLevelIncreaseEvent>* g_levelIncreaseEventDispatcher = (EventDispatcher<TESLevelIncreaseEvent>*)						0x01B39804;
// EventDispatcher<>* g_itemAddedEventDispatcher = (EventDispatcher<>*)																	0x012E5FB0;
// EventDispatcher<>* g_hourPassedEventDispatcher = (EventDispatcher<>*)																0x01B3FDE4;
// EventDispatcher<>* g_dayPassedEventDispatcher = (EventDispatcher<>*)																	0x01B2E728;
EventDispatcher<TESLocationDiscoveryEvent>* g_locationDiscoveryEventDispatcher = (EventDispatcher<TESLocationDiscoveryEvent>*)			0x01B39B58;
// EventDispatcher<>* g_chestsLootedEventDispatcher = (EventDispatcher<>*)																0x01B3E788;
// EventDispatcher<>* g_timesTrainedEventDispatcher = (EventDispatcher<>*)																0x01B40130;
// EventDispatcher<>* g_timesBarteredEventDispatcher = (EventDispatcher<>*)																0x01B3E480;
// EventDispatcher<>* g_contractedDiseaseEventDispatcher = (EventDispatcher<>*)															0x01B38F9C;
// EventDispatcher<>* g_poisonedWeaponEventDispatcher = (EventDispatcher<>*)															0x01B39CC4;
EventDispatcher<TESQuestStatusEvent>* g_questStatusEventDispatcher = (EventDispatcher<TESQuestStatusEvent>*)							0x01B113A0;
// EventDispatcher<>* g_objectiveStateEventDispatcher = (EventDispatcher<>*)															0x01B10FE0;
EventDispatcher<TESActorKillEvent>* g_actorKillEventDispatcher = (EventDispatcher<TESActorKillEvent>*)									0x01B38B48;
EventDispatcher<TESCriticalHitEvent>* g_criticalHitEventDispatcher = (EventDispatcher<TESCriticalHitEvent>*)							0x01B38FD0;
EventDispatcher<TESDisarmedEvent>* g_disarmedEventDispatcher = (EventDispatcher<TESDisarmedEvent>*)										0x01B38B7C;
EventDispatcher<TESWordLearnedEvent>* g_wordLearnedEventDispatcher = (EventDispatcher<TESWordLearnedEvent>*)							0x01B39D2C;
// EventDispatcher<>* g_wordUnlockedEventDispatcher = (EventDispatcher<>*)																0x01B39D60;
EventDispatcher<TESSpellLearnedEvent>* g_spellLearnedEventDispatcher = (EventDispatcher<TESSpellLearnedEvent>*)							0x01B3905C;
EventDispatcher<TESDragonSoulGainedEvent>* g_dragonSoulGainedEventDispatcher = (EventDispatcher<TESDragonSoulGainedEvent>*)				0x01B38F2C;
// EventDispatcher<>* g_soulGemsUsedEventDispatcher = (EventDispatcher<>*)																0x01B3FAC8;
EventDispatcher<TESSoulTrappedEvent>* g_soulTrappedEventDispatcher = (EventDispatcher<TESSoulTrappedEvent>*)							0x01B39028;
EventDispatcher<TESShoutAttackEvent>* g_shoutAttackEventDispatcher = (EventDispatcher<TESShoutAttackEvent>*)							0x01B39C90;
// EventDispatcher<>* g_shoutMasteredEventDispatcher = (EventDispatcher<>*)																0x01B39D94;
// EventDispatcher<>* g_itemEquippedEventDispatcher = (EventDispatcher<>*)																0x01B38B14;
// EventDispatcher<>* g_itemCraftedEventDispatcher = (EventDispatcher<>*)																0x01B3E7F4;
// EventDispatcher<>* g_pickpocketEventDispatcher = (EventDispatcher<>*)																0x01B39AF0;
EventDispatcher<TESLockPickedEvent>* g_lockPickedEventDispatcher = (EventDispatcher<TESLockPickedEvent>*)								0x01B3FBBC;
// EventDispatcher<>* g_bountyEventDispatcher = (EventDispatcher<>*)																	0x01B39C28;
// EventDispatcher<>* g_trespassingEventDispatcher = (EventDispatcher<>*)																0x01B3B2A8;
// EventDispatcher<>* g_finePaidEventDispatcher = (EventDispatcher<>*)																	0x01B39B8C;
// EventDispatcher<>* g_daysJailedEventDispatcher = (EventDispatcher<>*)																0x01B39CF8;
// EventDispatcher<>* g_itemsPickpocketedEventDispatcher = (EventDispatcher<>*)															0x01B3E7BC;
// EventDispatcher<>* g_itemStealEventDispatcher = (EventDispatcher<>*)																	0x01B39B24;
// EventDispatcher<>* g_jailingEventDispatcher = (EventDispatcher<>*)																	0x01B39BC0;
// EventDispatcher<>* g_jailEscapeEventDispatcher = (EventDispatcher<>*)																0x01B39BF4;
EventDispatcher<TESGrandTheftHorseEvent>* g_grandTheftHorseEventDispatcher = (EventDispatcher<TESGrandTheftHorseEvent>*)				0x01B38C4C;
// EventDispatcher<>* g_assaultCrimeEventDispatcher = (EventDispatcher<>*)																0x01B38BE4;
// EventDispatcher<>* g_murderCrimeEventDispatcher = (EventDispatcher<>*)																0x01B38C18;

}

// ======================================================
// Event registration
// ======================================================

namespace newEvents
{

void RegisterGameEventHandlers()
{
	_MESSAGE("Registering game event handlers...");

//	g_furnitureEventDispatcher->AddEventSink(&g_furnitureEventHandler);
	g_lockChangedEventDispatcher->AddEventSink(&g_lockChangedEventHandler);
	g_objectLoadedEventDispatcher->AddEventSink(&g_objectLoadedEventHandler);
	g_questStageEventDispatcher->AddEventSink(&g_questStageEventHandler);
	g_waitStartEventDispatcher->AddEventSink(&g_waitStartEventHandler);
	g_waitStopEventDispatcher->AddEventSink(&g_waitStopEventHandler);

}

void RegisterStoryEventHandlers()
{
	_MESSAGE("Registering story event handlers...");

	g_itemHarvestEventDispatcher->AddEventSink(&g_itemHarvestEventHandler);
	g_bookReadEventDispatcher->AddEventSink(&g_bookReadEventHandler);
	g_locationClearedEventDispatcher->AddEventSink(&g_locationClearedEventHandler);
	g_skillIncreaseEventDispatcher->AddEventSink(&g_skillIncreaseEventHandler);
	g_levelIncreaseEventDispatcher->AddEventSink(&g_levelIncreaseEventHandler);
//	g_itemAddedEventDispatcher
//	g_hourPassedEventDispatcher
//	g_dayPassedEventDispatcher
	g_locationDiscoveryEventDispatcher->AddEventSink(&g_locationDiscoveryEventHandler);
//	g_chestsLootedEventDispatcher
//	g_timesTrainedEventDispatcher
//	g_timesBarteredEventDispatcher 
//	g_contractedDiseaseEventDispatcher
//	g_poisonedWeaponEventDispatcher
	g_questStatusEventDispatcher->AddEventSink(&g_questStatusEventHandler);
//	g_objectiveStateEventDispatcher->AddEventSink(&g_objectiveStateEventHandler);
	g_actorKillEventDispatcher->AddEventSink(&g_actorKillEventHandler);
	g_criticalHitEventDispatcher->AddEventSink(&g_criticalHitEventHandler);
	g_disarmedEventDispatcher->AddEventSink(&g_disarmedEventHandler);
	g_wordLearnedEventDispatcher->AddEventSink(&g_wordLearnedEventHandler);
//	g_wordUnlockedEventDispatcher
	g_spellLearnedEventDispatcher->AddEventSink(&g_spellLearnedEventHandler);
	g_dragonSoulGainedEventDispatcher->AddEventSink(&g_dragonSoulGainedEventHandler);
//	g_soulGemsUsedEventDispatcher
	g_soulTrappedEventDispatcher->AddEventSink(&g_soulTrappedEventHandler);
	g_shoutAttackEventDispatcher->AddEventSink(&g_shoutAttackEventHandler);
//	g_shoutMasteredEventDispatcher
//	g_itemEquippedEventDispatcher
//	g_itemCraftedEventDispatcher
//	g_pickpocketEventDispatcher
	g_lockPickedEventDispatcher->AddEventSink(&g_lockPickedEventHandler);
//	g_bountyEventDispatcher
//	g_trespassingEventDispatcher
//	g_finePaidEventDispatcher
//	g_daysJailedEventDispatcher
//	g_itemsPickpocketedEventDispatcher
//	g_itemStealEventDispatcher
//	g_jailingEventDispatcher
//	g_jailEscapeEventDispatcher
	g_grandTheftHorseEventDispatcher->AddEventSink(&g_grandTheftHorseEventHandler);
//	g_assaultCrimeEventDispatcher
//	g_murderCrimeEventDispatcher

}

}