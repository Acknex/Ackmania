/*
 *******************************************************************************
 * items.c
 * Creation date: 08.06.2013
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2011-02-23 22:27:12 +0100 (Mi, 23 Feb 2011) $
 * $Revision: 6 $
 * $Author: Firo $
 *
 *******************************************************************************
 * Description
 *
 * Script for collectible items
 *
 * Comments
 * 
 * for short descriptions see comments in items.h
 *
 *******************************************************************************
 */

#include <mtlfx.c>
#include "player.h"

SOUND* sndCollect = "collect.wav";

action item_coin()
{
	item_setup();	
	my->event = item_coin_evt;
	var vZ = my->z;
	var vOffset = random(500);
	var vParticles = 0;
	my->material = mtl_specBump;
	my->pan = random(360);
	
	while(!is(me, is_collected))
	{
		vParticles += time_step;
		if (vParticles > 10)
		{
			effect(item_particle, 10, &my->x, nullvector);
			vParticles -= 10;
		}
		my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan -= 5 * time_step;
		my->tilt = 30 * sinv(total_ticks * 10 + vOffset);
		wait(1);
	}
	item_fade();
}

action item_life()
{
	item_setup();
	my->event = item_life_evt;
	var vZ = my->z;
	var vOffset = random(500);
	var vParticles = 0;
	
	while(!is(me, is_collected))
	{
		vParticles += time_step;
		if (vParticles > 10)
		{
			effect(item_particle, 10, &my->x, nullvector);
			vParticles -= 10;
		}
		my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan += 20 * time_step * sinv(total_ticks * 2 - vOffset);
		wait(1);
	}
	item_fade();
}

SOUND* snd_narrator_jetpack = "jetpack.wav";

action item_jetpack()
{
	item_setup();
	my->event = item_jetpack_evt;
	var vZ = my->z;
	var vOffset = random(500);
	var vParticles = 0;
	
	while(!is(me, is_collected))
	{
		vParticles += time_step;
		if (vParticles > 10)
		{
			effect(item_particle, 10, &my->x, nullvector);
			vParticles -= 10;
		}
		my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan = 135 * sinv(total_ticks * 2 - vOffset);
		my->tilt = 30 * sinv(total_ticks * 10 + vOffset);
		wait(1);
	}
	snd_play(snd_narrator_jetpack, 100, 0);
	item_fade();
}

void item_setup()
{
	my->emask |= ENABLE_TRIGGER; 
	my->trigger_range = 20;
	set (me, PASSABLE);
}

void item_fade()
{
	var vTicks = total_ticks;
	VECTOR vecDist;
	
	vec_set(&vecDist, &my->x);
	vec_sub(&vecDist, &player->x);
	while (my->scale_x > 0)
	{
		//scale down item
		my->scale_x = maxv(my->scale_x - 0.1 * time_step, 0);
		my->scale_y = my->scale_x;
		my->scale_z = my->scale_x;
		my->pan += (total_ticks - vTicks) * 10 * time_step;

		//move item towards player
		vec_set(&my->x, &vecDist);
		vec_scale(&my->x, my->scale_x);
		vec_add(&my->x, &player->x);
		wait(1);
	}
	ptr_remove(me);
}

void item_coin_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		my->event = NULL;
		set (me, is_collected);
		ent_playsound(me, sndCollect, 1000);
		if (player != NULL) player.PL_A4_COUNT +=1;
		achievement("firstcube");
	}
}

void item_life_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		my->event = NULL;
		set (me, is_collected);
		ent_playsound(me, sndCollect, 1000);
		if (player != NULL) nPlayerLifes +=1;
		achievement("firstlive");
	}
}

void item_jetpack_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		my->event = NULL;
		set (me, is_collected);
		ent_playsound(me, sndCollect, 1000);
		PoL_playerSetJetpack(TRUE);
		activate_jetpack();
		achievement("firstjetpack");
	}
}


void item_particleFader(PARTICLE *p) 
{
	p->alpha -= 5 * time_step;
	p->size += time_step;
	if (p->alpha <= 0) 
	{
		p->lifespan = 0;
	}
}

void item_particle (PARTICLE *p) 
{
	VECTOR vecTemp;
	vec_randomize(&vecTemp, 10);
	vec_normalize(&vecTemp, 4);
	vec_add (&p->vel_x, &vecTemp);
	p->vel_y = 0;
	vec_set(&p->blue, vector(0, 255, 255));
	set(p, MOVE | TRANSLUCENT | BRIGHT);
	p->lifespan = 80;
	p->size  = 30 + random(5);
	p->event = item_particleFader;
}

