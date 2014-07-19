#ifndef playrace_c
#define playrace_c

#include "playrace.h"
#include "timer.h"

void play_race(int index)
{
   g_doNotDrive = false;

   reset_timer();
   show_hud();

   while (1) {
      update_camera();
      update_hud();
      wait(1);
   }
}


#endif /* playrace_c */
