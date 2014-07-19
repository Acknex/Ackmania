#ifndef loadrace_c
#define loadrace_c

#include "engine.h"
#include "loadrace.h"
#include "startgrid.h"

void load_race (char* filename)
{
   level_load(null);

   g_numGridConfigUploaded = 0;
   level_load(filename);

   wait(1);

   create_kart_drivers();
}

#endif /* loadrace_c */
