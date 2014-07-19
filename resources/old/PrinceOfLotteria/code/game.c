#ifndef _GAME_C_
#define _GAME_C_

#include "gui.h"
#include "sky.h"
#include "platform.h"

var vDummy = 0;

void game_start()
{
	PANEL* panBlack = pan_create(" ", 100);
	vec_set(panBlack.blue, vector(8,8,8));
	panBlack.size_x = screen_size.x;
	panBlack.size_y = screen_size.y;
	pan_setdigits(panBlack, 0, 5, 5, "Press [Space] to skip", font_create("Arial#24b"), 1, vDummy);
	pan_setcolor(panBlack, 1, 1, vector(255,255,255));
	set(panBlack, SHOW | LIGHT);
	var vMediaHandle = media_play("music\\lotteria_intro_with_music.wav", NULL, 100);
	while(key_esc || key_space || key_enter) wait(1);
	while(media_playing(vMediaHandle) && !key_esc && !key_space && !key_enter) {
		wait(1);
	}
	//reset(panBlack, SHOW); //removed by firo
	ptr_remove(panBlack); //fix by firo (cleanup due to restart after game over)
	media_stop(vMediaHandle);
	vMediaHandle = 0;
	
	nPlayerLifes = 3; //fix by firo
	flying_man = 0;//fix by firo
	level_load("level01_v7.wmb");
	//level_load("bosslevel.wmb");
	VECTOR* vecPlayerPosition = vector(-432,-80,40);
	wait(3);
	skychange();
	freeze_mode = 0;
	gui_start();
	gui_show();
	vGameMusicHandle = media_play("music\\POL_game_intro.wav", NULL, 70);
	wait(-8);
	vGameMusicHandle = media_loop("music\\POL_game_looped.wav", NULL, 50);
}

void game_restart(void) {
	level_load(level_name);
}

#endif // #ifndef _GAME_C_