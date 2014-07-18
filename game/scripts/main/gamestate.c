#ifndef gamestate_c
#define gamestate_c

#include "engine.h"
#include "gamestate.h"
#include "loadrace.h"

// gibt den aktuellen Spielstatus zurück
int get_game_state() {
   return g_gameState;
}

// wechselt vom aktuellen Spielstand in den angegebenen - gibt den letzten zurück; -1 wenn nicht erlaubt
int invoke_game_state(int gameState) {

   int oldGameState = g_gameState;

   switch (gameState) {
      case GAME_STATE_PRERACE:
         invoke_game_state_prerace(oldGameState);
         break;
   }

   g_gameState = gameState;
   return oldGameState;
}

void invoke_game_state_prerace(int oldGameState) {

   switch (oldGameState) {

      case GAME_STATE_VIRGIN:
         load_race("dummy_level.wmb");
         break;
   }
}

#endif /* gamestate_c */
