#ifndef gamestate_h
#define gamestate_h

#define GAME_STATE_VIRGIN 0 // Spiel im startup
#define GAME_STATE_MENU 1 // Menü
#define GAME_STATE_INTRO 2 // Intro
#define GAME_STATE_PRERACE 3 // bevor das Rennen startet
#define GAME_STATE_RACE 4 // Rennen
#define GAME_STATE_POSTRACE 5 // nach dem Rennen
#define GAME_STATE_FINISH 6 // Abschluss
#define GAME_STATE_LOST 7 // Verloren
#define GAME_STATE_WIN 8 // Gewonnen
#define GAME_STATE_CREDITS 9 // Credits

int g_gameState = GAME_STATE_VIRGIN;

// gibt den aktuellen Spielstatus zurück
int get_game_state ();

// wechselt vom aktuellen Spielstand in den angegebenen - gibt den letzten zurück; -1 wenn nicht erlaubt
int invoke_game_state (int gameState, int index);

// --------------------------------------------------------------------------------------------------------------------

void invoke_game_state_menu(int oldGameState, int levelIndex);
void invoke_game_state_prerace(int oldGameState, int index);

void invoke_game_state_menu_exit_ev();
void invoke_game_state_menu_credits();
void invoke_game_state_menu_level_ev(int i);

void invoke_game_state_prerace(int oldGameState, int index);

void invoke_game_state_race(int oldGameState, int levelIndex);

#include "gamestate.c"

#endif /* gamestate_h */
