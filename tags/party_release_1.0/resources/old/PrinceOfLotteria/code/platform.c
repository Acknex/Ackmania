/*
 *******************************************************************************
 * platform.c
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
 * Script for level actions 
 *
 * Comments
 * 
 * for short descriptions see comments in platform.h
 *
 *******************************************************************************
 */


SOUND* sndPlatformCrumble = "broeseln.wav";

// uses: id 
action platform_marker()
{
	set(my, INVISIBLE | PASSABLE | is_marker);
}

// uses: id, speed, idletime 
action moving_platform()
{
	set(my, is_platform);
	my->ptr_start = NULL;
	my->ptr_end   = NULL;
	if (my->idletime == 0) my->idletime = 0.001;
	
	wait (1);

	for(you = ent_next(NULL); (you != NULL) && (my->ptr_end == NULL); you = ent_next(you))
	{
		if ((your->id == my->id) && (my->id > 0) && is(you, is_marker))
		{
			if (my->ptr_start == NULL)
			{
				my->ptr_start = handle(you);
			}
			else if (my->ptr_end == NULL)
			{
				my->ptr_end = handle(you);
			}
			else
			{
				//do nothing
			}
		}
	}
	
	if ((my->ptr_end == NULL) || (my->id <= 0))
	{
		printf("incorrect platform marker for id %d", (int)my->id);
		return;
	}

	//platform start point
	VECTOR vecSpeed;
	VECTOR vecTemp;
	ENTITY *ent1, *ent2, *entTarget, *entCur, *entTemp;
	
	ent1 = ptr_for_handle(my->ptr_start);
	ent2 = ptr_for_handle(my->ptr_end);
	
	//directional speed
	vec_set(&vecSpeed, &ent2->x);
	vec_sub(&vecSpeed, &ent1->x);
	vec_normalize(&vecSpeed, my->speed);

	if (vec_dist(&ent1->x, &my->x) < vec_dist(&ent2->x, &my->x))
	{
		vec_set(&my->x, &ent1->x);
		entTarget = ent2;
		entCur    = ent1;
	}
	else
	{
		vec_set(&my->x, &ent2->x);			
		entTarget = ent1;
		entCur    = ent2;
		vec_scale(&vecSpeed, -1);
	}

	//entity loop
	while(1)
	{		
		vec_set(vecTemp, vecSpeed);
		vec_scale(vecTemp, time_step);
		vec_set(&my->absspeed_x, &vecTemp);
		
 		you = player;
		c_move(me, nullvector, &vecTemp, IGNORE_MODELS | IGNORE_SPRITES | IGNORE_YOU | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS);

		if(vec_dist(&entTarget->x, &my->x) < 20)
		{
			wait (1);
			vec_set(&my->absspeed_x, nullvector);
			wait(-(my->idletime));

			vec_scale(&vecSpeed, -1);
			entTemp   = entCur;
			entCur    = entTarget;
			entTarget = entTemp;			
		}
		
		wait (1);
	}
}

// uses: id, speed 
action rotating_platform()
{
	set(my, is_platform);
	my->ptr_start = NULL;
	my->ptr_end   = NULL;
	
	wait (1);

	for(you = ent_next(NULL); (you != NULL) && (my->ptr_end == NULL); you = ent_next(you))
	{
		if ((your->id == my->id) && (my->id > 0) && is(you, is_marker))
		{
			if (my->ptr_start == NULL)
			{
				my->ptr_start = handle(you);
			}
			else if (my->ptr_end == NULL)
			{
				my->ptr_end = handle(you);
			}
			else
			{
				//do nothing
			}
		}
	}
	
	if ((my->ptr_end == NULL) || (my->id <= 0))
	{
		printf("incorrect platform marker for id %d", (int)my->id);
		return;
	}

	//platform start point
	VECTOR vecSpeed, vecTemp;
	ENTITY *ent1, *ent2, *entCenter;
	var vRadius;
	var vAngle;
	
	ent1 = ptr_for_handle(my->ptr_start);
	ent2 = ptr_for_handle(my->ptr_end);
	
	if (vec_dist(&ent1->x, &my->x) < vec_dist(&ent2->x, &my->x))
	{
		vec_set(&my->x, &ent1->x);
		entCenter = ent2;
		ent2->y = ent1->y;
	}
	else
	{
		vec_set(&my->x, &ent2->x);			
		entCenter = ent1;
		ent1->y = ent2->y;
	}

	//directional speed
	vec_set(&vecSpeed, nullvector);
	vRadius = vec_dist(&ent1->x, &ent2->x);

	//entity loop
	while(1)
	{		
		vAngle += time_step * my->speed;
		vAngle = ang(vAngle);
		vec_set(&vecSpeed, nullvector);
		vecSpeed.x = vRadius;
		vec_rotate(&vecSpeed, vector(0, vAngle, 0));
		vec_add(&vecSpeed, &entCenter->x);
		vec_sub(&vecSpeed, &my->x);
		vec_scale(&vecSpeed, time_step);
		vec_set(&my->absspeed_x, &vecSpeed);
		
 		you = player;
		c_move(me, nullvector, &vecSpeed, IGNORE_MODELS | IGNORE_SPRITES | IGNORE_YOU | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS);

		wait (1);
	}
}

// uses: durability, speed
action breaking_platform()
{
	ENTITY* ent = ent_create("platform_crumble.wmb", &my->x, NULL); //fuck it
	ent_preload(ent); //fuck it
	set(ent, INVISIBLE | PASSABLE); //fuck it
	
	set(my, is_platform);
	my->event = breaking_platform_evt;
	my->emask |= (ENABLE_IMPACT); 
	if (my->durability <= 0) my->durability = 0.5;
	if (my->speed <= 0)      my->speed = 2;
	
	while(!is(my, is_touched))
	{
		wait (1);
	}

	ent_playsound(me, sndPlatformCrumble, 1000);
	set(me, INVISIBLE | PASSABLE); //fuck it
	reset(ent, INVISIBLE | PASSABLE); //fuck it
	//ent_morph(me, "platform_crumble.wmb");
	wait (-(my->durability));

	ent->speed = my->speed; //fuck it
	ptr_remove(me); //fuck it
	me = ent; //fuck it
	//move downwards
	VECTOR vecSpeed;
	my->alpha = 100;
	set(my, TRANSLUCENT | PASSABLE);
	while(my->alpha != 0)
	{
		vec_set(&vecSpeed, vector(0,0, -(my->speed) * time_step));
		c_move(me, nullvector, &vecSpeed, IGNORE_MODELS | IGNORE_SPRITES | IGNORE_WORLD | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS);
		my->alpha = maxv(my->alpha - (my->speed * time_step * 0.5), 0);
		wait (1);
	}

	ptr_remove(me);
}

function breaking_platform_evt()
{
	if (event_type == EVENT_IMPACT)
	{
		if (you != NULL)
		{
			if (you == player)
			{
				set(my, is_touched);
				my->event = NULL;
			}	
		}
	}
}
