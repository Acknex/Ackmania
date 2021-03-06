//#define DEBUG

#define PRAGMA_PATH "levels";
#define PRAGMA_PATH "scripts";
#define PRAGMA_PATH "sprites";
#define PRAGMA_PATH "music";
#define PRAGMA_PATH "scripts\\camera";
#define PRAGMA_PATH "scripts\\credits";
#define PRAGMA_PATH "scripts\\environment";
#define PRAGMA_PATH "scripts\\helper";
#define PRAGMA_PATH "scripts\\hud";
#define PRAGMA_PATH "scripts\\items";
#define PRAGMA_PATH "scripts\\main";
#define PRAGMA_PATH "scripts\\menu";
#define PRAGMA_PATH "scripts\\player";
#define PRAGMA_PATH "scripts\\race";
#define PRAGMA_PATH "scripts\\shaders";

#include "engine.h"

#ifdef DEBUG
#include <default.c>
#else
#include "keys.h"
#endif

#include "sysinit.h"
#include "skilldefines.h"
#include "gamestate.h"
#include "raceactions.h"
#include "circuitinfo.h"
#include "camera.h"
#include "hud.h"

#define ENABLE_TAUNT_HOTKEY
#include "taunts.h"

#define TARGET_GAME_STATE GAME_STATE_MENU
//#define TARGET_GAME_STATE GAME_STATE_PRERACE


int main() {

   doSysInit();

   menu_init(15);

   create_camera();
   create_hud();

   initPostprocessing(get_camera());

   invoke_game_state(TARGET_GAME_STATE, 0);
	
}

