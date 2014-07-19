#ifndef _RACEINTRO_C_
#define _RACEINTRO_C_

#include "raceintro.h"

void raceintro_trigger(ENTITY* ent, void *callback)
{
	var i;
	ent.flags2 |= SHOW;
	
	ent->pan = -100;

	snd_play(g_sndIntroBeep, 100, 0);

	ent->material = mtl_unlit;
	ent->ambient = 200;

	var i = 0;
	for(i = 0; i < g_raceintro_ticks; i += time_step) {

	   vec_set(ent.x, vector(0.5 * screen_size.x, 0.5 * screen_size.y, g_raceintro_offset + i*g_raceintro_speed));
		rel_for_screen(ent.x, camera);

		ent->pan += 2 *time_step;

		ent.u = 10*total_ticks;
		ent.v = 10*total_ticks;

		wait(1);
	}
	
	ent.flags2 &= ~SHOW;
	
	if(callback != NULL) {
		void tmp();
		tmp = callback;
		tmp();
	}
}

#endif // #ifndef _RACEINTRO_C_
