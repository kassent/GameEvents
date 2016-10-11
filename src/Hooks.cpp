#include "skse/SafeWrite.h"
#include "Hooks.h"

BGSLocation*	g_locationCleared;
TESObjectREFR*	g_locationDiscMarker;
TESObjectREFR*	g_lockPicked;

// ======================================================
// Gameplay
// ======================================================

const UInt32 kClearedCheck = 0x004B15B0;
const UInt32 kClearedEnt = kClearedCheck + 0x3;
const UInt32 kClearedRet = kClearedCheck + 0x8;

__declspec(naked) void HookLocationCleared()
{
	__asm
		{
			mov		g_locationCleared, ecx

			cmp     [esp+24h+8], 0

			jmp		[kClearedRet]
		}
}

const UInt32 kDiscoveryCheck = 0x00744660;
const UInt32 kDiscoveryEnt = kDiscoveryCheck + 0xCC;
const UInt32 kDiscoveryRet = kDiscoveryCheck + 0xD1;

__declspec(naked) void HookLocationDiscovery()
{
	__asm
		{
			mov		g_locationDiscMarker, edi

			add     esp, 8
			test    edi, edi

			jmp		[kDiscoveryRet]
		}
}

// ======================================================
// Overrides
// ======================================================

void WriteHooks()
{
	_MESSAGE("Writing hooks...");

	WriteRelJump(kClearedEnt, (UInt32)HookLocationCleared);
//	WriteRelJump(kDiscoveryEnt, (UInt32)HookLocationDiscovery); //unreliable, returns loc marker

}