#define PRAGMA_ZERO

#include <acknex.h>
#include <litec.h>
#include <windows.h>
#include <stdio.h>

#include <default.c>

#define PRAGMA_PATH "scripts";
#define PRAGMA_PATH "scripts\\main";

#include "sysdefines.h"
#include "sysinit.h"
#include "gamestate.h"

int main() {

   wait(3);

   doSysInit();

   wait(1);
   level_load(null);

   invoke_game_state(GAME_STATE_PRERACE);
}
