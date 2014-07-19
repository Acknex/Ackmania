/*
 *******************************************************************************
 * traps.c
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
 * Script for level traps 
 *
 * Comments
 * 
 * for short descriptions see comments in traps.h
 *
 *******************************************************************************
 */


SOUND* sndSpikesDown = "spikes01.wav";
SOUND* sndSpikesUp   = "spikes02.wav";
SOUND* sndStalactite = "squish_loud.wav";
SOUND* sndStoneFall  = "stone_fall.wav";
SOUND* sndStoneImpact= "stone_impact.wav";

action trap()
{
	set (me, is_trap | PASSABLE);
}

action moving_spikes()
{
	var vPosDown;
	var vPosUp;
	var vTimer;
	
	set (me, is_trap | PASSABLE);
	wait(1);
	vPosDown = my->z - (my->max_z - my->min_z) * 0.8;
	vPosUp   = my->z;

	while(1)
	{
		wait(-SPIKE_TIMER_UP);
		reset(me, is_trap);
		ent_playsound(me, sndSpikesDown, 200);
		while(my->z > vPosDown)
		{
			wait(1);
			c_move(me, nullvector, vector(0, 0, -SPIKE_SPEED * time_step), IGNORE_MODELS | IGNORE_SPRITES | IGNORE_WORLD | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS);
	
		}
		my->z = vPosDown;
		
		wait(-SPIKE_TIMER_DOWN);	
		set(me, is_trap);
		ent_playsound(me, sndSpikesUp, 200);
		while(my->z < vPosUp)
		{
			wait(1);
			c_move(me, nullvector, vector(0, 0, SPIKE_SPEED * time_step), IGNORE_MODELS | IGNORE_SPRITES | IGNORE_WORLD | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS);
	
		}
		my->z = vPosUp;
		
		wait(1);
	}
}

action falling_stone()
{
	set (me, is_trap);
	while (player == NULL) wait (1);
	my->alpha = 100;
	vec_scale (&my->scale_x, random(0.4) + 0.6);
	var vOffset = random(300) - 150;
	
	while(my->alpha > 0)
	{
		wait (1);
		if ((absv(player.x - my.x) < STONE_TRIGGERDIST) && (my->z > player->z) && !is(me, is_triggered))
		{
			set (me, is_triggered);
			my->emask = ENABLE_BLOCK;
			my->event = falling_trap_evt;
			ent_playsound(me, sndStoneFall, 1000);			
		}
		
		if (!is(me, is_collided))
		{
			if (is(me, is_triggered))
			{
				c_move(me, nullvector, vector(0, 0, -STONE_SPEED * time_step), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PASSENTS);
			}
			c_rotate(me, vector(0, 
				2 * time_step * cosv(total_ticks * 17 + vOffset),
				3 * time_step * sinv(total_ticks * 20 + vOffset)
				), 
				IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PASSENTS);
		}
		else
		{
			my->alpha = maxv(my->alpha - (time_step * 7), 0);
		}
	}
	ptr_remove(me);
}

action falling_stalactite()
{
	set (me, is_trap);
	while (player == NULL) wait (1);
	my->alpha = 100;
	my->scale_z *= random(0.4) + 0.8;
	
	while(my->alpha > 0)
	{
		wait (1);
		if ((absv(player.x - my.x) < STALACTITE_TRIGGERDIST) && (my->z > player->z) && !is(me, is_triggered))
		{
			set (me, is_triggered);
			my->emask = ENABLE_BLOCK;
			my->event = falling_trap_evt;
			ent_playsound(me, sndStalactite, 1000);
		}
		
		if (!is(me, is_collided))
		{
			if (is(me, is_triggered))
			{
				c_move(me, nullvector, vector(0, 0, -STALACTITE_SPEED * time_step), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PASSENTS);
				c_rotate(me, vector(0, 2 * time_step * cosv(total_ticks * 17),3 * time_step * sinv(total_ticks * 20)), 
					IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PASSENTS);
			}
		}
		else
		{
			my->alpha = maxv(my->alpha - (time_step * 7), 0);
		}
	}
	ptr_remove(me);
}

void falling_trap_evt()
{
	if (event_type == EVENT_BLOCK)
	{
		set(me, PASSABLE | TRANSLUCENT | is_collided);		
		reset(me, is_trap);
		ent_playsound(me, sndStoneImpact, 2000);			
	}
}