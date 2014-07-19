#ifndef circuitinfo_h
#define circuitinfo_h

int getNumCircuits();

STRING* getCircuitTitleStr(int index);
STRING* getCircuitMusicFilenameStr(int index);
STRING* getCircuitPicFilenameStr(int index);
STRING* getCircuitLevelFilenameStr(int index);

#include "circuitinfo.c"

#endif /* circuitinfo_h */
