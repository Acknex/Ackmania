#ifndef items_c
#define items_c

// Initialisiert jedes Item, mit dem der Spieler interagieren kann.
void _item_setup()
{
	my->emask |= ENABLE_TRIGGER; 
	my->trigger_range = 20;
	set (me, PASSABLE);
}

// L�sst ein Item in die obere linke Ecke verschwinden
action _item_fade()
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
	ent_remove(me);
}

// Event f�r einen einsammelbaren Cube (M�nzen)
void _item_a4_cube_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		my->event = NULL;
		ent_playsound(me, sndCollectA4Cube, 1000);
		if (you != NULL) you.PL_A4_COUNT +=1;
		//achievement("firsta4cube");
		set(me,INVISIBLE);
		ent_create(str_for_entfile(NULL,me), my->x, _item_fade);
		wait(-3);
		vec_set(my.scale_x, vector(0,0,0));
		reset(me,INVISIBLE);
		while(my.scale_x < 1.0) {
			vec_add(my.scale_x, vector(time_step / 20, time_step / 20, time_step / 20));
			wait(1);
		}
		my->event = _item_a4_cube_evt;
	}
}

// Rotiert, kann eingesammelt werden.
// Bringt Punkte. Bei 10 Punkten -> Sofortiger Speedboost.
action a4_cube()
{
	_item_setup();	
	my->event = _item_a4_cube_evt;
	var vZ = my->z;
	var vOffset = random(500);
	var vParticles = 0;
	my->material = mtl_specBump;
	my->pan = random(360);
	
	while(me)
	{
		vParticles += time_step;
		if (vParticles > 10)
		{
			if (!is(me,INVISIBLE)) {
				effect(item_particle, 10, &my->x, nullvector);
				vParticles -= 10;
			}
		}
		my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan -= 5 * time_step;
		my->tilt = 30 * sinv(total_ticks * 10 + vOffset);
		wait(1);
	}
	_item_fade();
}

// Gebe dem Spieler ein zuf�lliges Item
void _give_random_item(ENTITY* driver)
{
	if (driver != NULL)
	{
		if (driver.CURRENT_ITEM == ITEM_NONE) {
			driver.CURRENT_ITEM = 1 + integer(random(5));
			
			switch(driver.CURRENT_ITEM) {
				case 1: ent_create(ITEM_GRAVE_MODEL, my->x, _item_fade); break;
				case 2: ent_create(ITEM_ROCKET_MODEL, my->x, _item_fade); break;
				case 3: ent_create(ITEM_AIM_ROCKET_MODEL, my->x, _item_fade); break;
				case 4: ent_create(ITEM_TURBO_MODEL, my->x, _item_fade); break;
				case 5: ent_create(ITEM_BADASS_ROCKET_MODEL, my->x, _item_fade); break;
				case 6: ent_create(ITEM_MUSHROOM_MODEL, my->x, _item_fade); break;
			}
		}
	}
}

// Event, der die Itemvergabe regelt
void _item_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		my->event = NULL;
		ent_playsound(me, sndCollectItem, 1000);
		
		// Gib dem ber�hrenden Fahrer ein neues Item
		_give_random_item(you);
		
		//achievement("firstitem");
		set(me,INVISIBLE);
		wait(-3);
		vec_set(my.scale_x, vector(0,0,0));
		reset(me,INVISIBLE);
		while(my.scale_x < 1.0) {
			vec_add(my.scale_x, vector(time_step / 20, time_step / 20, time_step / 20));
			wait(1);
		}
		my->event = _item_evt;
	}
}

// Wird eingesammelt und vergibt ein Zufallsitem, falls noch
// keins im Slot ist. Verpufft, und wird 3 Sekunden sp�ter gerespawned.
action item()
{
	_item_setup();	
	my->event = _item_evt;
	var vZ = my->z;
	var vOffset = random(500);
	var vParticles = 0;
	my->material = mtl_specBump;
	my->pan = random(360);
	
	while(me)
	{
		vParticles += time_step;
		if (vParticles > 10)
		{
			if (!is(me,INVISIBLE)) {
				effect(item_particle, 10, &my->x, nullvector);
				vParticles -= 10;
			}
		}
		my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan -= 5 * time_step;
		my->tilt = 30 * sinv(total_ticks * 10 + vOffset);
		wait(1);
	}
}

// Event, der die Kollision mit einem Grabstein regelt
void _grave_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		my->event = NULL;
		ent_playsound(me, sndGraveCollision, 1000);
		
		// Drehe Spieler
		
		wait(1);
		ent_remove(me);
	}
}

// Aktion eines Grabsteins (Falle)
action grave() {
	_item_setup();
	my->event = _grave_evt;
}

// Legt ein A9-Grab ab, das Fahrer ausbremst, wenn sie dr�berfahren
void plant_grave(VECTOR* pos) {
	me->CURRENT_ITEM = ITEM_NONE;
	ENTITY* entGrave = ent_create(ITEM_GRAVE_MODEL, pos, grave);
	place_on_floor(entGrave);
}

// Rakete, die geradeaus fliegt
action _rocket()
{
	while(me)
	{
		c_move(me, vector(10 * time_step, 0, 0), nullvector, IGNORE_PASSABLE | IGNORE_PASSENTS | GLIDE);
		wait(1);
	}
}

// Schie�t eine Rakete ab
void shoot_rocket()
{
	// Erzeuge Rakete
	ENTITY* rocket = ent_create(ITEM_ROCKET_MODEL, me, _rocket);
	rocket.pan = my.pan;
	my->CURRENT_ITEM = ITEM_NONE;
}

// Rakete, die 
action _aiming_rocket()
{
	VECTOR movement;
	while(me)
	{
		// Rotiere in Richtung des n�chsten Spielers
		vec_set(movement.x, vector(10 * time_step, 0, 0));
		c_move(me, movement.x, nullvector, IGNORE_PASSABLE | IGNORE_PASSENTS | GLIDE);
		wait(1);
	}	
}

// OPTIONAL: Schie�t eine leicht zielgelenkte Rakete ab
void shoot_badass_aiming_rocket()
{
	// Erzeuge Rakete
	ENTITY* rocket = ent_create(ITEM_ROCKET_MODEL, me, _aiming_rocket);
	rocket.pan = my.pan;
	my->CURRENT_ITEM = ITEM_NONE;	
}

// Beschleunigt den Spieler f�r 3 Sekunden
// auf 1,4(?)-fache Geschwindigkeit
void use_turbo() {
	my->CURRENT_ITEM = ITEM_NONE;
	my->DRIVER_MAX_SPEED = MAX_SPEED * 1.5;
	// Beschleunige
	wait(-3);
	// Bremse ab
	my->DRIVER_MAX_SPEED = MAX_SPEED;
}

// Spikes, die aus dem Boden fahren und den Spieler
// ausbremsen. Werden bei Ber�hrung f�r 5 Sekunden inaktiviert.
action spikes()
{
	var vPosDown;
	var vPosUp;
	var vTimer;
	
	set (me, IS_TRAP | PASSABLE);
	wait(1);
	vPosDown = my->z - (my->max_z - my->min_z) * 0.8;
	vPosUp   = my->z;

	while(1)
	{
		wait(-SPIKE_TIMER_UP);
		reset(me, IS_TRAP);
		ent_playsound(me, sndSpikesDown, 200);
		while(my->z > vPosDown)
		{
			wait(1);
			c_move(me, nullvector, vector(0, 0, -SPIKE_SPEED * time_step), IGNORE_MODELS | IGNORE_SPRITES | IGNORE_WORLD | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS);
	
		}
		my->z = vPosDown;
		
		wait(-SPIKE_TIMER_DOWN);	
		set(me, IS_TRAP);
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

// OPTIONAL: Schie�t einen (Lotti-)Ghost auf den 1. Spieler
// (wenn man es nicht selber ist, sonst auf den letzten).
// Slashed den Getroffenen mit einem LOTTI-Schrei f�r 3 Sekunden
void shoot_ultra_badass_aiming_rocket();

// Aktiviert den Lotteriatypischen Rainbowmode f�r 8 Sekunden
void start_rainbow_mode();

#endif /*items_c*/