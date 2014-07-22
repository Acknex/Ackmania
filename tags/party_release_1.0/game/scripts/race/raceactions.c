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
   ent->kartsnd_out = 100;
   ent->kartsnd_badground_loop_hndl = snd_loop(g_sndKartBadground, 5, 0);
}

void updateKartSnd(ENTITY* ent)
{
   VECTOR vdir;
   var s = get_kart_speed(ent, &vdir);

   VECTOR vs;
   vec_set(&vs, ent->x);
   VECTOR* vscreen = vec_to_screen(&vs, cam);
   bool inScreen = vscreen != null;

   if (inScreen) {
      ent->kartsnd_out = minv(ent->kartsnd_out + 10 * time_step, 100);
   } else {
      ent->kartsnd_out = maxv(5, ent->kartsnd_out - 10 * time_step);
   }

   if (s > 1) {

      if (ent->underground < 0.9 && inScreen) {
         snd_tune(ent->kartsnd_badground_loop_hndl, maxv(5, (1 - ent->underground) * 100), 0, 0);
      } else {
         snd_tune(ent->kartsnd_badground_loop_hndl, 5, 0, 0);
      }

      if (s > g_breakdownthreshold + 5) {
         ent->kartsnd_over = true;
      } else {
         if (s < g_breakdownthreshold - 5 && ent->kartsnd_over) {
            ent->kartsnd_over = false;

            if (inScreen) {
               snd_play(g_sndKartBremsen, g_racesnd_motor, 0);
            }
         }
      }

      double f = s / g_raceplayerMaxSpeed;

      var freq = 20 + f * 100;

      var vol = minv(g_racesnd_motor, 5 + f * g_racesnd_motor);
      vol *= (ent->kartsnd_out / 100);

      if (ent->kartsnd_off && s > g_exployothreshold) {
         ent->kartsnd_off = false;

         if (inScreen) {
            snd_play(g_sndKartStart, g_racesnd_motor, 0);
         }
      }

      snd_tune(ent->kartsnd_loop_hndl, vol, freq, 0);

   } else {
      ent->kartsnd_off = true;
      snd_tune(ent->kartsnd_loop_hndl, 5, 20, 0);
      snd_tune(ent->kartsnd_badground_loop_hndl, 5, 0, 0);
      ent->kartsnd_over = false;
   }
}

void ac_race_kart_ent()
{
   wait(1);

   postConstructPlayer(my);

   initKartSnd(my);

   while (1) {

      if (!g_raceIsEnded)
      {
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
      }
      else {
         loadPlayerCpuControlParams(my);
      }

      updatePlayer(my);

      updateKartSnd(my);

      wait(1);
   }
}

#endif /* raceactions_c */
