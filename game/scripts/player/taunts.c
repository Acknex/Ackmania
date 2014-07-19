#ifndef taunts_c
#define taunts_c

#include "taunts.h"

void loadTaunts()
{
   int playerIndex;
   for (playerIndex = 0; playerIndex < MAX_PLAYERS; playerIndex++) {

      SOUND** tauntArr = g_taunts[playerIndex];
      if (tauntArr != null) {

         int tauntNum = 0;
         int loadedTauntIndex = 0;

         int tauntIndex;
         for (tauntIndex = 0; tauntIndex < NUM_MAX_TAUNTS; tauntIndex++) {

            char sndFilename[256];
            sprintf(sndFilename, "taunt_p%d_%d.ogg", playerIndex + 1, tauntIndex);

            if (file_exists(sndFilename)) {

               SOUND* snd = snd_create(sndFilename);
               if (snd != null) {

                  tauntArr[loadedTauntIndex] = snd;

                  tauntNum++;
                  loadedTauntIndex++;
               }
            }
         }

         g_num_taunts[playerIndex] = tauntNum;
      }
   }
}

void playTaunt(int index)
{
   if (g_num_taunts[index] > 0)
         {
      int sndIndex = 0;

      if (g_num_taunts[index] > 1) {
         sndIndex = (int) random(g_num_taunts[index]);
      }

      if (g_tauntHandles[index] != 0) {
         snd_stop(g_tauntHandles[index]);
         g_tauntHandles[index] = 0;
      }

      SOUND** sndArr = g_taunts[index];
      if (sndArr != null) {

         SOUND* snd = sndArr[sndIndex];
         if (snd != null) {
            g_tauntHandles[index] = snd_play(snd, g_tauntVolume, 0);
         }
      }
   }
}

#endif /* taunts_c */
