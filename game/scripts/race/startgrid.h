#ifndef startgrid_h
#define startgrid_h

#define MAX_STARTGRID_POS 4

VECTOR g_vecStartgridPos [MAX_STARTGRID_POS];
var g_vecStartgridAng [MAX_STARTGRID_POS];
ENTITY* g_kartEnts [MAX_STARTGRID_POS];

int g_numGridConfigUploaded = 0;

void setStartgridConfig (int rank, VECTOR* pos, var ang);

#include "startgrid.c"

#endif /* startgrid_h */
