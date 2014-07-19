#ifndef loadrace_c
#define loadrace_c

#include "engine.h"
#include "loadrace.h"
#include "startgrid.h"
#include "camera.h"

void load_race (char* filename)
{
   level_load(null);

   g_numGridConfigUploaded = 0;
   level_load(filename);

   create_camera();

   wait(1);

   create_kart_drivers();
   show_camera();

   // TODO lifecycle
   while (1) {
      update_camera();
      wait(1);
   }
}

#endif /* loadrace_c */
