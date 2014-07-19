#define PRAGMA_PATH "scripts\\engine";
#include "engine.h"

#include <default.c>

#define PRAGMA_PATH "levels";
#define PRAGMA_PATH "scripts";
#define PRAGMA_PATH "sprites";
#define PRAGMA_PATH "scripts\\camera";
#define PRAGMA_PATH "scripts\\circuits";
#define PRAGMA_PATH "scripts\\globals";
#define PRAGMA_PATH "scripts\\main";
#define PRAGMA_PATH "scripts\\menu";
#define PRAGMA_PATH "scripts\\player";
#define PRAGMA_PATH "scripts\\race";

#include "sysinit.h"
#include "skilldefines.h"
#include "gamestate.h"
#include "raceactions.h"
#include "circuitinfo.h"

#define TARGET_GAME_STATE GAME_STATE_MENU
//#define TARGET_GAME_STATE GAME_STATE_PRERACE

int main() {
   doSysInit();
   invoke_game_state(TARGET_GAME_STATE, 0);
}
