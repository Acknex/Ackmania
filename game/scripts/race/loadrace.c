#ifndef loadrace_c
#define loadrace_c

#include "engine.h"
#include "loadrace.h"
#include "startgrid.h"
#include "camera.h"
#include "circuitinfo.h"
#include "environment.h"
#include "postprocessing.h"
#include "gamestate.h"
#include "hud.h"

void quit_race() {
   invoke_game_state(GAME_STATE_MENU, 0);
}

void load_race(int index)
{
   level_load(null);

   g_doNotDrive = true;

   on_esc = quit_race;
   on_space = null;
   on_enter = null;

   g_preraceActive = true;

   hide_hud();

   STRING* strMusicFilename = getCircuitMusicFilenameStr(index);
   STRING* strLevelFilename = getCircuitLevelFilenameStr(index);

   media_stop(g_raceMusicHandle);

   STRING* str = str_create("#128");
   str_cpy(str, "media\\");
   str_cat(str, strMusicFilename);

   if (strMusicFilename != null) {
      g_raceMusicHandle = media_loop(str, null, g_raceMusicVolume);
   }

   str_remove(str);

   g_numGridConfigUploaded = 0;

   double skyspeed = g_circuit_skyspeeds[index];
   environment_load(strLevelFilename, skyspeed);

   float hdrstrength = g_circuit_hdrstrengths[index];
   float hdrthreshold = g_circuit_hdrthresholds[index];
   float hdrexposure = g_circuit_hdrexposures[index];

   setHdr(hdrstrength, hdrthreshold, hdrexposure);
   show_camera();

   wait(2);

   create_kart_drivers();

   g_race_pretimer = 3 * 16;

   while (g_preraceActive) {
      update_race_pre();
      wait(1);
   }

   invoke_game_state(GAME_STATE_RACE, index);
}

void update_race_pre()
{
   g_race_pretimer -= time_step;
   update_camera();

   if (g_race_pretimer < 0) {
      g_preraceActive = false;
   }
}

#endif /* loadrace_c */
