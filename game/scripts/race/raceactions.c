#ifndef raceactions_c
#define raceactions_c

#include "engine.h"
#include "raceactions.h"
#include "startgrid.h"
#include "skilldefines.h"

void ac_race_kart_pos()
{
   if (my != null) {

      int rank = my->sk_kart_id;
      setStartgridConfig(rank, my->x, my->pan);

      ptr_remove(my);
   }
}

void ac_race_kart_ent()
{
   // TODO
   error("X");
}

#endif /* raceactions_c */
