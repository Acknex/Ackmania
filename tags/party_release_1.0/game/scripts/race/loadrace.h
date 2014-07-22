#ifndef loadrace_h
#define loadrace_h

var g_raceMusicHandle = 0;

bool g_preraceActive = false;
var g_race_pretimer = 0;

void load_race (int index);
void update_race_pre();

#include "loadrace.c"

#endif /* loadrace_h */
