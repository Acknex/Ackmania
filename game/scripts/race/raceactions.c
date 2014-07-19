#ifndef raceactions_c
#define raceactions_c

#include "engine.h"
#include "raceactions.h"
#include "startgrid.h"
#include "skilldefines.h"
#include "raceplayer.h"
#include "camera.h"

void ac_race_kart_pos()
{
   set(my, INVISIBLE | PASSABLE | UNLIT);

   if (my != null) {

      int rank = my->sk_kart_id;
      setStartgridConfig(rank, my->x, my->pan);

      ptr_remove(my);
   }
}

void initKartSnd(ENTITY* ent)
{
   ent->kartsnd_off = true;
   ent->kartsnd_over = false;
   ent->kartsnd_loop_hndl = snd_loop(g_sndKartLoop, 5, 0);
}

void updateKartSnd(ENTITY* ent)
{
   VECTOR vdir;
   var s = get_kart_speed(ent, &vdir);

   if (s > 1) {

      if (s > g_breakdownthreshold+5) {
         ent->kartsnd_over = true;
      } else {
         if (s < g_breakdownthreshold-5 && ent->kartsnd_over) {
            ent->kartsnd_over = false;
            snd_play(g_sndKartBremsen, 100, 0);
         }
      }

      double f = s / g_raceplayerMaxSpeed;
      var freq = 20 + f * 100;
      var vol = 5 + f * 95;

      if (ent->kartsnd_off && s > g_exployothreshold) {
         ent->kartsnd_off = false;
         snd_play(g_sndKartStart, 100, 0);
      }

      snd_tune(ent->kartsnd_loop_hndl, vol, freq, 0);

   } else {
      ent->kartsnd_off = true;
      snd_tune(ent->kartsnd_loop_hndl, 5, 20, 0);
      ent->kartsnd_over = false;
   }
}

void ac_race_kart_ent()
{
   wait(1);

   postConstructPlayer(my);

   initKartSnd(my);

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

      updateKartSnd(my);

      wait(1);
   }
}

#endif /* raceactions_c */
