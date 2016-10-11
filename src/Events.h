#include "skse/GameObjects.h"
#include "skse/GameReferences.h"
#include "skse/PapyrusEvents.h"


// ======================================================
// New Structures
// ======================================================

namespace newEvents
{

struct TESFurnitureEvent
{
	TESObjectREFR*	character;
	TESObjectREFR*	furniture;
	UInt32			state;
};

struct TESLockChangedEvent
{
	TESObjectREFR*	lock;
	TESObjectREFR*	unlocker;
};

struct TESObjectLoadedEvent
{
	UInt32	formId;
	UInt32	unk1;
	UInt32	unk2;
	void*	loaded;
};

struct TESQuestStageEvent
{
	void*	finishedCallback;
	UInt32	formId;
	UInt32	stage;
};

struct TESWaitStartEvent
{
	float	current;
	float	desired;
};

struct TESWaitStopEvent
{
	//empty?
};

struct TESItemHarvestEvent
{
	TESForm*	object;
	TESForm*	player;
};

struct TESLocationClearedEvent
{
	//empty?
};

struct TESBookReadEvent
{
	TESObjectBOOK* book;
};

struct TESSkillIncreaseEvent
{
	TESForm*	character;
	UInt32		avId;
};

struct TESLevelIncreaseEvent
{
	TESForm*	character;
	UInt32		level;
};

struct TESItemAddedEvent
{
	TESForm*	character;
};

struct TESHourPassedEvent
{
	UInt32		sleep;
};

struct TESLocationDiscoveryEvent
{
	TESFullName*	fullname;
};

struct TESQuestStatusEvent
{
	UInt32		status;	
	TESQuest*	quest;		
};

struct TESActorKillEvent
{
	TESObjectREFR*	killer;
	TESObjectREFR*	victim;
};

struct TESCriticalHitEvent
{
	TESForm*		character;
	TESObjectWEAP*	weapon;
};

struct TESDisarmedEvent
{
	TESObjectREFR*	source;
	TESObjectREFR*	target;
};

struct TESWordLearnedEvent
{
	UInt32			first; //flag
	UInt32			unk;
	TESWordOfPower*	word;
};

struct TESSpellLearnedEvent
{
	//empty?
};

struct TESDragonSoulGainedEvent
{
	float	souls;
};

struct TESSoulTrappedEvent
{
	TESObjectREFR*	player;
	TESObjectREFR*	target;	
};

struct TESShoutAttackEvent
{
	TESShout*	shout;
};

struct TESLockPickedEvent
{
	//empty?
};

struct TESBountyEvent
{
	TESFaction*		faction;
	UInt32			before;
	UInt32			after;
};

struct TESDaysJailedEvent
{
	UInt32			days;
	BGSLocation*	location;
	TESFaction*		faction;
	UInt32			bounty;
};

struct TESGrandTheftHorseEvent
{
	//empty?
};

struct TESAssaultCrimeEvent
{
	char			offset;
	TESObjectREFR*	target;
};

struct TESMurderCrimeEvent
{
	char			offset;
	TESObjectREFR*	source; //?
	TESObjectREFR*	target;
};

}

// ======================================================
// EventSink templates
// ======================================================

namespace newEvents
{

/*
	TESActivateEvent
	TESActiveEffectApplyRemoveEvent
	TESActorLocationChangeEvent
	TESBookReadEvent
	TESCellAttachDetachEvent
	TESCellFullyLoadedEvent
	TESCombatEvent
	TESContainerChangedEvent
	TESDeathEvent
	TESDestructionStageChangedEvent
	TESEnterBleedoutEvent
	TESEquipEvent
	TESFormDeleteEvent
*/

template <>
class BSTEventSink <TESFurnitureEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESFurnitureEvent * evn, EventDispatcher<TESFurnitureEvent> * dispatcher) = 0;
};

/*
	TESGrabReleaseEvent
	TESHitEvent
	TESInitScriptEvent
	TESLoadGameEvent
*/

template <>
class BSTEventSink <TESLockChangedEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESLockChangedEvent * evn, EventDispatcher<TESLockChangedEvent> * dispatcher) = 0;
};
	
/*
	TESMagicEffectApplyEvent
	TESMagicWardHitEvent
	TESMoveAttachDetachEvent	
*/

template <>
class BSTEventSink <TESObjectLoadedEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESObjectLoadedEvent * evn, EventDispatcher<TESObjectLoadedEvent> * dispatcher) = 0;
};

/*
	TESObjectREFRTranslationEvent
	TESOpenCloseEvent
	TESPackageEvent
	TESPerkEntryRunEvent
	TESQuestInitEvent
*/

template <>
class BSTEventSink <TESQuestStageEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESQuestStageEvent * evn, EventDispatcher<TESQuestStageEvent> * dispatcher) = 0;
};

/*
	TESResetEvent
	TESResolveNPCTemplatesEvent
	TESSceneEvent
	TESSceneActionEvent
	TESScenePhaseEvent
	TESSellEvent
	TESSleepStartEvent
	TESSleepStopEvent
	TESSpellCastEvent
	TESPlayerBowShotEvent
	TESTopicInfoEvent
	TESTrackedStatsEvent
	TESTrapHitEvent
	TESTriggerEvent
	TESTriggerEnterEvent
	TESTriggerLeaveEvent
	TESUniqueIDChangeEvent
*/

template <>
class BSTEventSink <TESWaitStartEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESWaitStartEvent * evn, EventDispatcher<TESWaitStartEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESWaitStopEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESWaitStopEvent * evn, EventDispatcher<TESWaitStopEvent> * dispatcher) = 0;
};

/*
	TESSwitchRaceCompleteEvent

*/

template <>
class BSTEventSink <TESItemHarvestEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESItemHarvestEvent * evn, EventDispatcher<TESItemHarvestEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESLocationClearedEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESLocationClearedEvent * evn, EventDispatcher<TESLocationClearedEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESBookReadEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESBookReadEvent * evn, EventDispatcher<TESBookReadEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESSkillIncreaseEvent>
{
public:
    virtual ~BSTEventSink() {}; // todo?
    virtual    EventResult ReceiveEvent(TESSkillIncreaseEvent * evn, EventDispatcher<TESSkillIncreaseEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESLevelIncreaseEvent>
{
public:
    virtual ~BSTEventSink() {}; // todo?
    virtual    EventResult ReceiveEvent(TESLevelIncreaseEvent * evn, EventDispatcher<TESLevelIncreaseEvent> * dispatcher) = 0;
};

/*
	TESItemAddedEvent
	TESHoursPassedEvent
	TESDaysPassedEvent
*/

template <>
class BSTEventSink <TESLocationDiscoveryEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESLocationDiscoveryEvent * evn, EventDispatcher<TESLocationDiscoveryEvent> * dispatcher) = 0;
};

/*
	TESChestsLootedEvent
	TESTimesTrainedEvent
	TESTimesBarteredEvent
	TESContractedDiseaseEvent
	TESPoisonedWeaponEvent
*/

template <>
class BSTEventSink <TESQuestStatusEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESQuestStatusEvent * evn, EventDispatcher<TESQuestStatusEvent> * dispatcher) = 0;
};

/*
	TESObjectiveStateEvent
*/

template <>
class BSTEventSink <TESActorKillEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESActorKillEvent * evn, EventDispatcher<TESActorKillEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESCriticalHitEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESCriticalHitEvent * evn, EventDispatcher<TESCriticalHitEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESDisarmedEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESDisarmedEvent * evn, EventDispatcher<TESDisarmedEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESWordLearnedEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESWordLearnedEvent * evn, EventDispatcher<TESWordLearnedEvent> * dispatcher) = 0;
};

/*
	TESWordUnlockedEvent
*/

template <>
class BSTEventSink <TESSpellLearnedEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESSpellLearnedEvent * evn, EventDispatcher<TESSpellLearnedEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESDragonSoulGainedEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESDragonSoulGainedEvent * evn, EventDispatcher<TESDragonSoulGainedEvent> * dispatcher) = 0;
};

/*
	TESSoulGemsUsedEvent
*/

template <>
class BSTEventSink <TESSoulTrappedEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESSoulTrappedEvent * evn, EventDispatcher<TESSoulTrappedEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESShoutAttackEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESShoutAttackEvent * evn, EventDispatcher<TESShoutAttackEvent> * dispatcher) = 0;
};

/*
	TESShoutMasteredEvent
	...
	...
*/

template <>
class BSTEventSink <TESLockPickedEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESLockPickedEvent * evn, EventDispatcher<TESLockPickedEvent> * dispatcher) = 0;
};

/*
	...
	...
	TESJailEscape
*/

template <>
class BSTEventSink <TESGrandTheftHorseEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESGrandTheftHorseEvent * evn, EventDispatcher<TESGrandTheftHorseEvent> * dispatcher) = 0;
};

}

// ======================================================
// Handler classes
// ======================================================

namespace newEvents
{

class FurnitureEventHandler : public BSTEventSink <TESFurnitureEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESFurnitureEvent * evn, EventDispatcher<TESFurnitureEvent> * dispatcher);
};

class LockChangedEventHandler : public BSTEventSink <TESLockChangedEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESLockChangedEvent * evn, EventDispatcher<TESLockChangedEvent> * dispatcher);
};

class ObjectLoadedEventHandler : public BSTEventSink <TESObjectLoadedEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESObjectLoadedEvent * evn, EventDispatcher<TESObjectLoadedEvent> * dispatcher);
};

class QuestStageEventHandler : public BSTEventSink <TESQuestStageEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESQuestStageEvent * evn, EventDispatcher<TESQuestStageEvent> * dispatcher);
};

class WaitStartEventHandler : public BSTEventSink <TESWaitStartEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESWaitStartEvent * evn, EventDispatcher<TESWaitStartEvent> * dispatcher);
};

class WaitStopEventHandler : public BSTEventSink <TESWaitStopEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESWaitStopEvent * evn, EventDispatcher<TESWaitStopEvent> * dispatcher);
};



class ItemHarvestEventHandler : public BSTEventSink <TESItemHarvestEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESItemHarvestEvent * evn, EventDispatcher<TESItemHarvestEvent> * dispatcher);
};

class LocationClearedEventHandler : public BSTEventSink <TESLocationClearedEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESLocationClearedEvent * evn, EventDispatcher<TESLocationClearedEvent> * dispatcher);
};

class BookReadEventHandler : public BSTEventSink <TESBookReadEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESBookReadEvent * evn, EventDispatcher<TESBookReadEvent> * dispatcher);
};

class SkillIncreaseEventHandler : public BSTEventSink <TESSkillIncreaseEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESSkillIncreaseEvent * evn, EventDispatcher<TESSkillIncreaseEvent> * dispatcher);
};

class LevelIncreaseEventHandler : public BSTEventSink <TESLevelIncreaseEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESLevelIncreaseEvent * evn, EventDispatcher<TESLevelIncreaseEvent> * dispatcher);
};

class LocationDiscoveryEventHandler : public BSTEventSink <TESLocationDiscoveryEvent>
{
public:
    virtual		EventResult    ReceiveEvent(TESLocationDiscoveryEvent * evn, EventDispatcher<TESLocationDiscoveryEvent> * dispatcher);
};

class QuestStatusEventHandler : public BSTEventSink <TESQuestStatusEvent>
{
public:
    virtual		EventResult		ReceiveEvent(TESQuestStatusEvent * evn, EventDispatcher<TESQuestStatusEvent> * dispatcher);
};

class ActorKillEventHandler : public BSTEventSink <TESActorKillEvent>
{
public:
    virtual		EventResult		ReceiveEvent(TESActorKillEvent * evn, EventDispatcher<TESActorKillEvent> * dispatcher);
};

class CriticalHitEventHandler : public BSTEventSink <TESCriticalHitEvent>
{
public:
	virtual		EventResult		ReceiveEvent(TESCriticalHitEvent * evn, EventDispatcher<TESCriticalHitEvent> * dispatcher);
};

class DisarmedEventHandler : public BSTEventSink <TESDisarmedEvent>
{
public:
	virtual		EventResult		ReceiveEvent(TESDisarmedEvent * evn, EventDispatcher<TESDisarmedEvent> * dispatcher);
};

class WordLearnedEventHandler : public BSTEventSink <TESWordLearnedEvent>
{
public:
	virtual		EventResult		ReceiveEvent(TESWordLearnedEvent * evn, EventDispatcher<TESWordLearnedEvent> * dispatcher);
};

class SpellLearnedEventHandler : public BSTEventSink <TESSpellLearnedEvent>
{
public:
	virtual		EventResult		ReceiveEvent(TESSpellLearnedEvent * evn, EventDispatcher<TESSpellLearnedEvent> * dispatcher);
};

class DragonSoulGainedEventHandler : public BSTEventSink <TESDragonSoulGainedEvent>
{
public:
	virtual		EventResult		ReceiveEvent(TESDragonSoulGainedEvent * evn, EventDispatcher<TESDragonSoulGainedEvent> * dispatcher);
};

class SoulTrappedEventHandler : public BSTEventSink <TESSoulTrappedEvent>
{
public:
	virtual		EventResult		ReceiveEvent(TESSoulTrappedEvent * evn, EventDispatcher<TESSoulTrappedEvent> * dispatcher);
};

class ShoutAttackEventHandler : public BSTEventSink <TESShoutAttackEvent>
{
public:
	virtual		EventResult		ReceiveEvent(TESShoutAttackEvent * evn, EventDispatcher<TESShoutAttackEvent> * dispatcher);
};

class LockPickedEventHandler : public BSTEventSink <TESLockPickedEvent>
{
public:
    virtual    EventResult		ReceiveEvent(TESLockPickedEvent * evn, EventDispatcher<TESLockPickedEvent> * dispatcher);
};

class GrandTheftHorseEventHandler : public BSTEventSink <TESGrandTheftHorseEvent>
{
public:
    virtual    EventResult		ReceiveEvent(TESGrandTheftHorseEvent * evn, EventDispatcher<TESGrandTheftHorseEvent> * dispatcher);
};

}

// ======================================================
// Registration holders
// ======================================================

namespace newEvents
{

extern RegistrationSetHolder<NullParameters>	g_furnitureEventRegs;
extern RegistrationSetHolder<NullParameters>	g_lockChangedEventRegs;
extern RegistrationMapHolder<UInt32>			g_objectLoadedEventRegs;
extern RegistrationSetHolder<NullParameters>	g_questStageEventRegs;
extern RegistrationSetHolder<NullParameters>	g_waitEventRegs;

extern RegistrationSetHolder<NullParameters>	g_itemHarvestEventRegs;
extern RegistrationSetHolder<NullParameters>	g_locationClearedEventRegs;
extern RegistrationSetHolder<NullParameters>	g_bookReadEventRegs;
extern RegistrationSetHolder<NullParameters>	g_skillIncreaseEventRegs;
extern RegistrationSetHolder<NullParameters>	g_levelIncreaseEventRegs;
extern RegistrationSetHolder<NullParameters>	g_locationDiscoveryEventRegs;
extern RegistrationSetHolder<NullParameters>	g_questStatusEventRegs;
extern RegistrationSetHolder<NullParameters>	g_actorKillEventRegs;
extern RegistrationSetHolder<NullParameters>	g_criticalHitEventRegs;
extern RegistrationSetHolder<NullParameters>	g_disarmedEventRegs;
extern RegistrationSetHolder<NullParameters>	g_wordLearnedEventRegs;
extern RegistrationSetHolder<NullParameters>	g_spellLearnedEventRegs;
extern RegistrationSetHolder<NullParameters>	g_dragonSoulGainedEventRegs;
extern RegistrationSetHolder<NullParameters>	g_soulTrappedEventRegs;
extern RegistrationSetHolder<NullParameters>	g_shoutAttackEventRegs;
extern RegistrationSetHolder<NullParameters>	g_lockPickedEventRegs;
extern RegistrationSetHolder<NullParameters>	g_grandTheftHorseEventRegs;

}

// ======================================================
// Event handlers
// ======================================================

namespace newEvents
{

extern FurnitureEventHandler			g_furnitureEventHandler;
extern LockChangedEventHandler			g_lockChangedEventHandler;
extern ObjectLoadedEventHandler			g_objectLoadedEventHandler;
extern QuestStageEventHandler			g_questStageEventHandler;
extern WaitStartEventHandler			g_waitStartEventHandler;
extern WaitStopEventHandler				g_waitStopEventHandler;

extern ItemHarvestEventHandler			g_itemHarvestEventHandler;
extern LocationClearedEventHandler		g_locationClearedEventHandler;
extern BookReadEventHandler				g_bookReadEventHandler;
extern SkillIncreaseEventHandler		g_skillIncreaseEventHandler;
extern LevelIncreaseEventHandler		g_levelIncreaseEventHandler;
extern LocationDiscoveryEventHandler	g_locationDiscoveryEventHandler;
extern QuestStatusEventHandler			g_questStatusEventHandler;
extern ActorKillEventHandler			g_actorKillEventHandler;
extern CriticalHitEventHandler			g_criticalHitEventHandler;
extern DisarmedEventHandler				g_disarmedEventHandler;
extern WordLearnedEventHandler			g_wordLearnedEventHandler;
extern SpellLearnedEventHandler			g_spellLearnedEventHandler;
extern DragonSoulGainedEventHandler		g_dragonSoulGainedEventHandler;
extern SoulTrappedEventHandler			g_soulTrappedEventHandler;
extern ShoutAttackEventHandler			g_shoutAttackEventHandler;
extern LockPickedEventHandler			g_lockPickedEventHandler;
extern GrandTheftHorseEventHandler		g_grandTheftHorseEventHandler;

void RegisterGameEventHandlers();
void RegisterStoryEventHandlers();

}

// ======================================================
// Event dispatchers
// ======================================================

namespace newEvents
{

extern EventDispatcher<TESFurnitureEvent>* g_furnitureEventDispatcher;
extern EventDispatcher<TESLockChangedEvent>*g_lockChangedEventDispatcher;
extern EventDispatcher<TESObjectLoadedEvent> * g_objectLoadedEventDispatcher;
extern EventDispatcher<TESQuestStageEvent> * g_questStageEventDispatcher;
extern EventDispatcher<TESWaitStartEvent> * g_waitStartEventDispatcher;
extern EventDispatcher<TESWaitStopEvent> * g_waitStopEventDispatcher;

extern EventDispatcher<TESItemHarvestEvent> * g_itemHarvestEventDispatcher;
extern EventDispatcher<TESLocationClearedEvent>* g_locationClearedEventDispatcher;
extern EventDispatcher<TESBookReadEvent>* g_bookReadEventDispatcher;
extern EventDispatcher<TESSkillIncreaseEvent>* g_skillIncreaseEventDispatcher;
extern EventDispatcher<TESLevelIncreaseEvent>* g_levelIncreaseEventDispatcher;
extern EventDispatcher<TESLocationDiscoveryEvent>* g_locationDiscoveryEventDispatcher;
extern EventDispatcher<TESQuestStatusEvent>* g_questStatusEventDispatcher;
extern EventDispatcher<TESActorKillEvent>* g_actorKillEventDispatcher;
extern EventDispatcher<TESCriticalHitEvent>* g_criticalHitEventDispatcher;
extern EventDispatcher<TESDisarmedEvent>* g_disarmedEventDispatcher;
extern EventDispatcher<TESWordLearnedEvent>* g_wordLearnedEventDispatcher;
extern EventDispatcher<TESSpellLearnedEvent>* g_spellLearnedEventDispatcher;
extern EventDispatcher<TESDragonSoulGainedEvent>* g_dragonSoulGainedEventDispatcher;
extern EventDispatcher<TESSoulTrappedEvent>* g_soulTrappedEventDispatcher;
extern EventDispatcher<TESShoutAttackEvent>* g_shoutAttackEventDispatcher;
extern EventDispatcher<TESLockPickedEvent>* g_lockPickedEventDispatcher;
extern EventDispatcher<TESGrandTheftHorseEvent>* g_grandTheftHorseEventDispatcher;

}