#ifndef items_c
#define items_c

#include "helper.h"
#include <particles.c>


// Initialisiert jedes Item, mit dem der Spieler interagieren kann.
void _item_setup()
{
	my->emask |= ENABLE_TRIGGER; 
	my->trigger_range = 20;
	set (me, PASSABLE);
}

// Lässt ein Item in die obere linke Ecke verschwinden
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

// Effekte für Items
void _item_particleFader(PARTICLE *p) 
{
	p->alpha -= 5 * time_step;
	p->size += time_step;
	if (p->alpha <= 0) 
	{
		p->lifespan = 0;
	}
}

void _item_particle (PARTICLE *p) 
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
	p->event = _item_particleFader;
}

// Event für einen einsammelbaren Cube (Münzen)
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
				effect(_item_particle, 10, &my->x, nullvector);
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

// Gebe dem Spieler ein zufälliges Item
void _give_random_item(ENTITY* driver)
{
	if (driver != NULL)
	{
		if (driver.CURRENT_ITEM == ITEM_NONE) {
			driver.CURRENT_ITEM = 1 + integer(random(5));
			
			// Zeige im Item-Panel wahllos ein paar Items in schneller
			// Rotation an.
			
			ent_playsound(me, sndGotNewItem, 1000);
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
		
		// Gib dem berührenden Fahrer ein neues Item
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
// keins im Slot ist. Verpufft, und wird 3 Sekunden später gerespawned.
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
				effect(_item_particle, 10, &my->x, nullvector);
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

// Legt ein A9-Grab ab, das Fahrer ausbremst, wenn sie drüberfahren
void plant_grave(VECTOR* pos) {
	me->CURRENT_ITEM = ITEM_NONE;
	ENTITY* entGrave = ent_create(ITEM_GRAVE_MODEL, pos, grave);
	place_on_floor(entGrave);
}

void p_rocket_explode(PARTICLE* p)
{
   VECTOR vTemp;
   vec_randomize(vTemp,40);
   vec_add(p.vel_x,vTemp);
	vec_set(p.blue,vector(random(255),random(255),255));
   set(p, MOVE | BRIGHT | TRANSLUCENT);
   p.alpha = 100;
   p.size = 20;
   p.skill_a = 20;
   p.event = p_fade;
}


// Rakete, die geradeaus fliegt
action _rocket()
{
	int liveTime = 300 + integer(random(100));
	var flyHeight = 0;
	int reachedTop = 0;
	var initHeight = 0;
	var zSpeed = 0;
	var xSpeed = 0;
	var animPercentage = 0;
	set(me, PASSABLE);
	while(me && (liveTime > 0))
	{
		// Lass die Rakete nach Abschuss hüpfen
		if (reachedTop == 0) {
			flyHeight +=20 * time_step;
			zSpeed = 20 * time_step;
			xSpeed = 20 * time_step;
			if (flyHeight >= 60) reachedTop = 1;
		} else {
			if (flyHeight >= initHeight) {
				flyHeight -=20 * time_step;
				zSpeed = -20 * time_step;
				xSpeed +=2 * time_step;
			} else {
				zSpeed = 0;
			}
		}
		
		if (animPercentage + 20 * time_step < 80) {
			animPercentage +=20 * time_step;
		} else {
			animPercentage = 80;
			reset(me, PASSABLE);
		}
		ent_animate(me, "Transform ",animPercentage, ANM_CYCLE);
		
		c_move(me, vector(xSpeed, 0, zSpeed), nullvector, IGNORE_PASSABLE | IGNORE_PASSENTS | ACTIVATE_SHOOT);
		ent_playsound(me, sndRocketFly, 1000);
		liveTime--;
		wait(1);
	}
	effect(p_rocket_explode, maxv(40, 80*time_step), my->x, nullvector);
	// Play sound boom
	ent_remove(me);
}

// Schießt eine Rakete ab
void shoot_rocket()
{
	// Erzeuge Rakete
	ENTITY* rocket = ent_create(ITEM_ROCKET_MODEL, vector(my->x+20, my->y, my->z), _rocket);
	vec_set(rocket.scale_x, vector(0.2, 0.2, 0.2));
	c_setminmax(rocket);
	ent_playsound(rocket, sndRocketFire, 1000);
	my->CURRENT_ITEM = ITEM_NONE;
}

// Rakete, die zielgelenkt in Richtung des nächsten Spielers fliegt
action _aiming_rocket()
{
	int liveTime = 4000;
	VECTOR movement;
	while(me && liveTime > 0)
	{
		// Rotiere in Richtung des nächsten Spielers
		vec_set(movement.x, vector(10 * time_step, 0, 0));
		ent_playsound(me, sndRocketFly, 1000);
		liveTime--;
		c_move(me, movement.x, nullvector, IGNORE_PASSABLE | IGNORE_PASSENTS | ACTIVATE_SHOOT);
		wait(1);
	}
	// effect(explode, 20, my->x, nullvector);
	ent_remove(me);
}

// OPTIONAL: Schießt eine leicht zielgelenkte Rakete ab
void shoot_badass_aiming_rocket()
{
	// Erzeuge Rakete
	ENTITY* rocket = ent_create(ITEM_ROCKET_MODEL, me, _aiming_rocket);
	vec_set(rocket.scale_x, vector(0.2, 0.2, 0.2));
	c_setminmax(rocket);
	ent_playsound(me, sndAimingRocketFire, 1000);
	rocket.pan = my.pan;
	my->CURRENT_ITEM = ITEM_NONE;	
}

// Beschleunigt den Spieler für 3 Sekunden
// auf 1,4(?)-fache Geschwindigkeit
void use_turbo() {
	my->CURRENT_ITEM = ITEM_NONE;
	my->DRIVER_MAX_SPEED = MAX_SPEED * 1.5;
	ent_playsound(me, sndTurboStart, 1000);
	// Beschleunige
	wait(-3);
	// Bremse ab
	my->DRIVER_MAX_SPEED = MAX_SPEED;
}

// Spikes, die aus dem Boden fahren und den Spieler
// ausbremsen. Werden bei Berührung für 5 Sekunden inaktiviert.
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

// Rakete, die direkt zum ersten bzw. nächsten Spieler fliegt
action _ultra_badass_aiming_rocket()
{
	int liveTime = 4000;
	VECTOR movement;
	while(me && liveTime > 0)
	{
		// Rotiere in Richtung des ersten Spielers
		vec_set(movement.x, vector(10 * time_step, 0, 0));
		ent_playsound(me, sndRocketFly, 1000);
		liveTime--;
		c_move(me, movement.x, nullvector, IGNORE_PASSABLE | IGNORE_PASSENTS | ACTIVATE_SHOOT);
		wait(1);
	}
	// effect(explode, 20, my->x, nullvector);
	ent_remove(me);
}

// OPTIONAL: Schießt einen (Lotti-)Ghost auf den 1. Spieler
// (wenn man es nicht selber ist, sonst auf den letzten).
// Slashed den Getroffenen mit einem LOTTI-Schrei für 3 Sekunden
void shoot_ultra_badass_aiming_rocket()
{
	// Erzeuge Rakete
	ENTITY* rocket = ent_create(ITEM_ROCKET_MODEL, me, _ultra_badass_aiming_rocket);
	vec_set(rocket.scale_x, vector(0.2, 0.2, 0.2));
	c_setminmax(rocket);
	ent_playsound(rocket, sindAimingBadassRocketFire, 1000);
	rocket.pan = my.pan;
	my->CURRENT_ITEM = ITEM_NONE;		
}

// Aktiviert den Lotteriatypischen Rainbowmode für 8 Sekunden
void start_rainbow_mode() {
	// Tue verückte Dinge mit Farben.
	ent_playsound(me, sndRainbowStart, 1000);
}

#endif /*items_c*/