#ifndef raceplayer_h
#define raceplayer_h

void postConstructPlayer (ENTITY* ent);

void loadPlayerHumanControlParams (ENTITY* ent);
void loadPlayerCpuControlParams (ENTITY* ent);

void updatePlayer (ENTITY* ent);

#include "raceplayer.c"

#endif /* raceplayer_h */
