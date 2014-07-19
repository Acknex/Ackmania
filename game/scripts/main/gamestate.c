#ifndef gamestate_c
#define gamestate_c

#include "engine.h"
#include "gamestate.h"
#include "loadrace.h"
#include "playrace.h"
#include "menu.h"

// gibt den aktuellen Spielstatus zurück
int get_game_state() {
   return g_gameState;
}

void enforce_game_state_credits_stop () {
   invoke_game_state(GAME_STATE_MENU, 0);
}

void invoke_game_state_credits(int oldGameState, int levelIndex)
{
   switch (oldGameState) {

      case GAME_STATE_MENU:
         menu_close();
         break;
   }

   on_esc = enforce_game_state_credits_stop;
   on_space = enforce_game_state_credits_stop;
   on_enter = enforce_game_state_credits_stop;

   credits_start();
}

// wechselt vom aktuellen Spielstand in den angegebenen - gibt den letzten zurück; -1 wenn nicht erlaubt
int invoke_game_state(int gameState, int levelIndex) {

   int oldGameState = g_gameState;

   switch (gameState) {

      case GAME_STATE_MENU:
         invoke_game_state_menu(oldGameState, levelIndex);
         break;

      case GAME_STATE_PRERACE:
         invoke_game_state_prerace(oldGameState, levelIndex);
         break;

      case GAME_STATE_RACE:
         invoke_game_state_race(oldGameState, levelIndex);
         break;

      case GAME_STATE_CREDITS:
         invoke_game_state_credits(oldGameState, levelIndex);
         break;
   }

   g_gameState = gameState;
   return oldGameState;
}

void invoke_game_state_menu(int oldGameState, int levelIndex) {

   switch (oldGameState) {

      case GAME_STATE_VIRGIN:
         break;

      case GAME_STATE_CREDITS:
         credits_stop();
         break;
   }

   menu_init(15);

   menu.onExit = invoke_game_state_menu_exit_ev;
   menu.onCredits = invoke_game_state_menu_credits;
   menu.onLevelStart = invoke_game_state_menu_level_ev;

   menu_open();
}

void invoke_game_state_menu_exit_ev() {
   sys_exit("Bye bye Lotti!");
}

void invoke_game_state_menu_credits() {
   invoke_game_state(GAME_STATE_CREDITS, 0);
}

void invoke_game_state_menu_level_ev(int i) {

   menu_close();

   //invoke_game_state(GAME_STATE_INTRO);
   invoke_game_state(GAME_STATE_PRERACE, i);

   //load_race(i);
}

void invoke_game_state_prerace(int oldGameState, int levelIndex) {

   switch (oldGameState) {

      case GAME_STATE_VIRGIN:
         load_race(levelIndex);
         break;

      case GAME_STATE_MENU:
         load_race(levelIndex);
         break;
   }
}

void invoke_game_state_race(int oldGameState, int levelIndex) {

   switch (oldGameState) {

      case GAME_STATE_PRERACE:
         play_race(levelIndex);
         break;
   }
}

#endif /* gamestate_c */
