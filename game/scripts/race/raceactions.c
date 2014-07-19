#ifndef raceactions_c
#define raceactions_c

#include "engine.h"
#include "raceactions.h"
#include "startgrid.h"
#include "skilldefines.h"
#include "raceplayer.h"

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
   wait(1);

   postConstructPlayer(my);

   while (1) {

      int id = my->sk_kart_id;

      switch (id)
      {
         case 1:
            loadPlayerHumanControlParams(my);
            break;

         default:
            loadPlayerCpuControlParams(my);
            break;
      }

      updatePlayer(my);

      wait(1);
   }
}

#endif /* raceactions_c */
