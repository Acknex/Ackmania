#ifndef taunts_h
#define taunts_h

#include "engine.h"
#include "skilldefines.h"

void loadTaunts();
void playTaunt(int index);

var g_tauntHandles[MAX_PLAYERS];

#define NUM_MAX_TAUNTS 42

SOUND* g_taunt_p1[NUM_MAX_TAUNTS];
SOUND* g_taunt_p2[NUM_MAX_TAUNTS];
SOUND* g_taunt_p3[NUM_MAX_TAUNTS];
SOUND* g_taunt_p4[NUM_MAX_TAUNTS];

SOUND** g_taunts[MAX_PLAYERS];
int g_num_taunts[MAX_PLAYERS];

char* g_taunt_format = "taunt_p%d_%d.ogg";

var g_tauntVolume = 100;

#ifdef ENABLE_TAUNT_HOTKEY
void playTauntHotkey () {
   playTaunt(0);
}
#endif

void taunts_h_startup()
{
   g_taunts[0] = g_taunt_p1;
   g_taunts[1] = g_taunt_p2;
   g_taunts[2] = g_taunt_p3;
   g_taunts[3] = g_taunt_p4;

   loadTaunts();

#ifdef ENABLE_TAUNT_HOTKEY
   on_7 = playTauntHotkey;
#endif
}

#include "taunts.c"

#endif /* taunts_h */
