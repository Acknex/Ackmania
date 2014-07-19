#ifndef loadrace_c
#define loadrace_c

#include "engine.h"
#include "loadrace.h"
#include "startgrid.h"
#include "camera.h"
#include "circuitinfo.h"
#include "environment.h"
#include "postprocessing.h"

void load_race(int index)
{
   level_load(null);

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

   // TODO lifecycle
   while (1) {
      update_camera();
      wait(1);
   }
}

#endif /* loadrace_c */
