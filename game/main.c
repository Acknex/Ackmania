#define PRAGMA_PATH "scripts\\engine";
#include "engine.h"

#include <default.c>

#define PRAGMA_PATH "scripts";
#define PRAGMA_PATH "scripts\\main";
#define PRAGMA_PATH "scripts\\race";

#include "sysinit.h"
#include "gamestate.h"

int main() {

   wait(3);

   doSysInit();

   wait(1);
   level_load(null);

   invoke_game_state(GAME_STATE_PRERACE);
}
