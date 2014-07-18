#ifndef gamestate_h
#define gamestate_h

#define GAME_STATE_VIRGIN 0 // Spiel im startup
#define GAME_STATE_MENU 1 // Men�
#define GAME_STATE_INTRO 2 // Intro
#define GAME_STATE_PRERACE 3 // bevor das Rennen startet
#define GAME_STATE_RACE 4 // Rennen
#define GAME_STATE_POSTRACE 5 // nach dem Rennen
#define GAME_STATE_FINISH 6 // Abschluss
#define GAME_STATE_LOST 7 // Verloren
#define GAME_STATE_WIN 8 // Gewonnen
#define GAME_STATE_CREDITS 9 // Credits

int g_gameState = GAME_STATE_VIRGIN;

// gibt den aktuellen Spielstatus zur�ck
int get_game_state ();

// wechselt vom aktuellen Spielstand in den angegebenen - gibt den letzten zur�ck; -1 wenn nicht erlaubt
int invoke_game_state (int gameState);

// --------------------------------------------------------------------------------------------------------------------

void invoke_game_state_prerace(int oldGameState);

#include "gamestate.c"

#endif /* gamestate_h */
