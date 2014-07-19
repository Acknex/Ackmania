#ifndef circuitinfo_c
#define circuitinfo_c

#include "engine.h"
#include "circuit_def.h"
#include "circuitinfo.h"

int getNumCircuits()
{
   int numStr = g_circuits->strings;
   int num = numStr / CIRCUIT_DEF_ENTRIES;

   return num;
}

STRING* getCircuitTitleStr(int index) {
   return (g_circuits->pstring)[index * CIRCUIT_DEF_ENTRIES + 0];
}

STRING* getCircuitMusicFilenameStr(int index) {
   return (g_circuits->pstring)[index * CIRCUIT_DEF_ENTRIES + 1];
}

STRING* getCircuitPicFilenameStr(int index) {
   return (g_circuits->pstring)[index * CIRCUIT_DEF_ENTRIES + 2];
}

STRING* getCircuitLevelFilenameStr(int index) {
   return (g_circuits->pstring)[index * CIRCUIT_DEF_ENTRIES + 3];
}

#endif /* circuitinfo_c */
