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
#include "raceintro.h"

void quit_race() {

   media_stop(g_raceMusicHandle);
   g_raceMusicHandle = 0;

   invoke_game_state(GAME_STATE_MENU, 0);
}

void load_race(int index)
{
   level_load(null);

   g_doNotDrive = true;

   on_esc = null;
   on_space = null;
   on_enter = null;

   setPostprocessingChain(get_camera());

   g_preraceActive = true;
   camera.roll = 0;
   resetPpSwirl ();


   hide_hud();

   STRING* strMusicFilename = getCircuitMusicFilenameStr(index);
   STRING* strLevelFilename = getCircuitLevelFilenameStr(index);

   media_stop(g_raceMusicHandle);

   STRING* str = str_create("#128");
   str_cpy(str, "media\\");
   str_cat(str, strMusicFilename);

   if (strMusicFilename != null) {
      g_raceMusicHandle = media_loop(str, null, g_music);
   }

   str_remove(str);

   g_numGridConfigUploaded = 0;

   double skyspeed = g_circuit_skyspeeds[index];
   environment_load(strLevelFilename, skyspeed);

   float hdrstrength = g_circuit_hdrstrengths[index];
   float hdrthreshold = g_circuit_hdrthresholds[index];
   float hdrexposure = g_circuit_hdrexposures[index];

   setPpSwirl(0, 0, 0.5, 0.5, 0.25+sin(total_ticks*3)*0.25);

   setHdr(hdrstrength, hdrthreshold, hdrexposure);
   show_camera();

   wait(2);

   create_kart_drivers();

   focus_camera();

   g_race_pretimer = 0.5 * 16;

   while (g_preraceActive) {
      update_race_pre();
      wait(1);
   }

   raceintro_trigger(raceintroNumber3, g_sndStartRace3, null);
   wait_for(raceintro_trigger);

   raceintro_trigger(raceintroNumber2, g_sndStartRace2, null);
   wait_for(raceintro_trigger);

   raceintro_trigger(raceintroNumber1, g_sndStartRace1, null);
   wait_for(raceintro_trigger);

   resetPpSwirl();

   invoke_game_state(GAME_STATE_RACE, index);
   on_esc = quit_race;

   raceintro_trigger(raceintroNumber4, g_sndStartRaceGo, null);
   wait_for(raceintro_trigger);
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
