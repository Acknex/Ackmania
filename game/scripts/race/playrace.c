#ifndef playrace_c
#define playrace_c

#include "playrace.h"
#include "timer.h"
#include "hud.h"
#include "gamestate.h"
#include "raceactions.h"

bool g_raceIsEnded = false;
int finalRank = 4;

void do_race_end(ENTITY* ent)
{
   // Weg mit dem scheiss, man kann die endansage nicht hören
   //playTaunt(ent->skill1 - 1);
   g_raceIsEnded = true;
   finalRank = get_kart_rank_player();
}

void play_race(int index)
{
   g_doNotDrive = false;
   g_raceIsEnded = false;

   reset_timer();
   unpause_timer();

   show_hud();

   while (!g_raceIsEnded) {
      update_camera();
      update_hud();
      wait(1);
   }

   if (finalRank <= 3) {
      snd_play(swin[finalRank - 1], 100, 0);
   } else {
      snd_play(splacelost[(int) random(5)], 100, 0);
   }

   hide_hud();

   bFinishPan->pos_x = screen_size.x / 2 - bFinishPan->bmap->width / 2;
   bFinishPan->pos_y = screen_size.y * 0.25 - bFinishPan->bmap->height / 2;

   bbWinnerPan->bmap = barr[finalRank - 1];
   bbWinnerPan->size_x = bbWinnerPan->bmap->width;
   bbWinnerPan->size_y = bbWinnerPan->bmap->height;
   bbWinnerPan->pos_x = screen_size.x / 2 - bbWinnerPan->bmap->width / 2;
   bbWinnerPan->pos_y = screen_size.y * 0.5 - bbWinnerPan->bmap->height / 2;

   bPressSpacePan->pos_x = screen_size.x / 2 - bPressSpacePan->bmap->width / 2;
   bPressSpacePan->pos_y = screen_size.y * 0.75 - bPressSpacePan->bmap->height / 2;

   set(bFinishPan, SHOW);
   set(bbWinnerPan, SHOW);
   set(bPressSpacePan, SHOW);

   while (!key_space && !key_enter && !key_esc) {
		bFinishPan->scale_x = screen_size.y / RACE_REF_SCREENSIZE;
		bFinishPan->scale_y = screen_size.y / RACE_REF_SCREENSIZE;
		bbWinnerPan->scale_x = screen_size.y / RACE_REF_SCREENSIZE;
		bbWinnerPan->scale_y = screen_size.y / RACE_REF_SCREENSIZE;
		bPressSpacePan->scale_x = screen_size.y / RACE_REF_SCREENSIZE;
		bPressSpacePan->scale_y = screen_size.y / RACE_REF_SCREENSIZE;
		bFinishPan->pos_x = screen_size.x / 2 - (bFinishPan->bmap->width / 2) * bFinishPan->scale_x;
		bFinishPan->pos_y = screen_size.y * 0.25 - (bFinishPan->bmap->height / 2) * bFinishPan->scale_y;
		bbWinnerPan->pos_x = screen_size.x / 2 - (bbWinnerPan->bmap->width / 2) * bbWinnerPan->scale_x;
		bbWinnerPan->pos_y = screen_size.y * 0.5 - (bbWinnerPan->bmap->height / 2) * bbWinnerPan->scale_y;
		bPressSpacePan->pos_x = screen_size.x / 2 - (bPressSpacePan->bmap->width / 2) * bPressSpacePan->scale_x;
		bPressSpacePan->pos_y = screen_size.y * 0.75 - (bPressSpacePan->bmap->height / 2) * bPressSpacePan->scale_y;

      update_camera();
      setPpSwirl(0.5, sin(total_ticks) * 0.1, 0.5, 0.5, 0.25 + sin(total_ticks * 3) * 0.25);
      wait(1);
   }

   while (key_space || key_enter || key_esc) {
      update_camera();
      setPpSwirl(0.5, sin(total_ticks) * 0.1, 0.5, 0.5, 0.25 + sin(total_ticks * 3) * 0.25);
      wait(1);
   }

   reset(bFinishPan, SHOW);
   reset(bbWinnerPan, SHOW);
   reset(bPressSpacePan, SHOW);

   resetPpSwirl();

   media_stop(g_raceMusicHandle);
   g_raceMusicHandle = 0;

	ac_race_cleanup();
   wait(1);

   invoke_game_state(GAME_STATE_MENU, index);
}

#endif /* playrace_c */
