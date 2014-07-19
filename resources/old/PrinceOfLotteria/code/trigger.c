#ifndef _TRIGGER_C_
#define _TRIGGER_C_

#include "sky.h"

SOUND* snd_narrator_start = "first_movement.wav";
SOUND* snd_narrator_sky = "sky.wav";
SOUND* snd_narrator_enemy = "enemy.wav";
SOUND* snd_narrator_lottifant = "lottifant.wav";

action trigger_wall() {
	wait(1);
	while(1) {
		if (player != NULL) {
			if (vec_dist(player.x, my.x) <= 150) {
				wait(1);
				achievement("firstlevel");
				level_load("bosslevel.wmb");
				break;
			}
		}
		wait(1);
	}
}

action trigger_credits() {
	wait(1);
	while(1) {
		if (player != NULL) {
			if (vec_dist(player.x, my.x) <= 250) {
				wait(1);
				achievement("lottifant");
				wait(-2);
				media_stop(vGameMusicHandle);
				gui_hide();
				sky_active = 0; //fix by firo
				credits_run();
				return;
			}
		}
		wait(1);
	}
}

action trigger_visible() {
	VECTOR vecTemp;
	while(1)
	{
		vec_set(vecTemp, my.x);
		if (vec_to_screen(vecTemp, camera) != NULL) {
			switch(my.skill1)
			{
			case 0:
				snd_play(snd_narrator_start, 100, 0);
				break;
			case 1:
				snd_play(snd_narrator_sky, 100, 0);
				break;
			case 2:
				snd_play(snd_narrator_enemy, 100, 0);
				break;
			case 3:
				snd_play(snd_narrator_lottifant, 100, 0);
				break;
			}
			break;
		}
		wait(1);
	}
}
#endif