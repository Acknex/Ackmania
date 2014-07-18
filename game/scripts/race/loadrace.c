#ifndef loadrace_c
#define loadrace_c

#include "engine.h"
#include "loadrace.h"

void load_race (char* filename)
{
   level_load(null);
   level_load(filename);
}

#endif /* loadrace_c */
