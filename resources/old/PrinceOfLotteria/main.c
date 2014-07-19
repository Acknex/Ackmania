#include <acknex.h>
#include <windows.h>
#include <default.c>
#include <particles.c>

#define PRAGMA_PATH "code"
#define PRAGMA_PATH "music"

#define PRAGMA_LEVEL "level01_v7.wmp"
#define PRAGMA_LEVEL "bosslevel.wmp"

#include "credits.h"
#include "credits.c"
#include "achievements.h"
#include "PoL_playermeshswitcher.c"
#include "camera.c"
#include "game.h"
#include "menu.h"
#include "platform.h"
#include "traps.h"
#include "items.h"
#include "player.h"
#include "gui.h"
#include "PoL_enemy.h"
#include "zorroMesh.h"
#include "level_effects.c"
#include "menu.c"
#include "trigger.h"
#include "geist.c"

void main() {
	fps_max = 60; // By Felix, hatte probleme mit Stockender Framerate (nach oben)
	d3d_antialias = 9; //beautifying by firo
	preload_mode = 2+4; //avoid crumbling platform jerk
	freeze_mode = 1;
	random_seed(0);
	video_mode = 12;
	menu_open();
}