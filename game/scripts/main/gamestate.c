#ifndef gamestate_c
#define gamestate_c

#include "engine.h"
#include "gamestate.h"
#include "loadrace.h"
#include "menu.h"

// gibt den aktuellen Spielstatus zurück
int get_game_state() {
   return g_gameState;
}

// wechselt vom aktuellen Spielstand in den angegebenen - gibt den letzten zurück; -1 wenn nicht erlaubt
int invoke_game_state(int gameState) {

   int oldGameState = g_gameState;

   switch (gameState) {

      case GAME_STATE_MENU:
         invoke_game_state_menu(oldGameState);
         break;

      case GAME_STATE_PRERACE:
         invoke_game_state_prerace(oldGameState);
         break;
   }

   g_gameState = gameState;
   return oldGameState;
}

void exitCallback () {
   error("exitCallback");
}

void creditsCallback () {
   error("creditsCallback");
}

void selectLevelCallback (int i) {
   error(str_printf(null, "selectLevelCallback i=%d", i));
}

void invoke_game_state_menu(int oldGameState) {

   switch (oldGameState) {

      case GAME_STATE_VIRGIN:

         menu_init(15);

         menu.onExit = exitCallback;
         menu.onCredits = creditsCallback;
         menu.onLevelStart = selectLevelCallback;

         menu_open();

         break;
   }
}

void invoke_game_state_prerace(int oldGameState) {

   switch (oldGameState) {

      case GAME_STATE_VIRGIN:
         load_race("dummy_level.wmb");
         break;
   }
}

#endif /* gamestate_c */
