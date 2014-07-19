#ifndef sysinit_c
#define sysinit_c

#include "engine.h"
#include "sysinit.h"
#include "postprocessing.h"
#include "credits.h"
#include "gamestate.h"

void onCreditsEnd_ev() {
   if (g_creditsStopEnforced == false) {
      invoke_game_state(GAME_STATE_MENU, 0);
   }
}

void doSysInit() {

   // rendering
   fps_max = 60;
   mip_flat = 2;
   d3d_antialias = 0;
   shadow_stencil = 0; // activate external shadows

   // window + system
   preload_mode = 7;
   mouse_pointer = 0;

   double resFac = 0.85; //%
   long resX = (double) sys_metrics(SM_CXSCREEN) * resFac;
   long resY = (double) sys_metrics(SM_CYSCREEN) * resFac;

   video_set(resX, resY, 0, 2);
   video_window(NULL, NULL, 0, "deadnex");

   random_seed((sys_seconds % sys_month) * sys_hours - 42);

   // credits
   credits_init();
   credits.onCreditsEnd = onCreditsEnd_ev;
}

#endif /* sysinit_c */
