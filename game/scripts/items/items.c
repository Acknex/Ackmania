#ifndef items_c
#define items_c

#include "helper.h"
#include <particles.c>

// Gib das aktuelle Item des Spielers zurück
var get_current_item_id()
{
	ENTITY* driver = get_kart_player();
	if (driver != NULL)
	{
		return driver.item_id;
	}
	return NULL;
}

// Initialisiert jedes Item, mit dem der Spieler interagieren kann.
void _item_setup()
{
	my->emask |= ENABLE_TRIGGER; 
	my->trigger_range = 90;
	set (me, PASSABLE);
	c_setminmax(me);
	set(me, FLAG2);
	set(me, SHADOW);
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
		snd_play(sndCollectA4Cube, 50, 0);
		if (you != NULL) you.PL_A4_COUNT +=1;
		//achievement("firsta4cube");
		set(me,INVISIBLE);
		wait(-3);
		vec_set(my.scale_x, vector(0,0,0));
		reset(me,INVISIBLE);
		while(my.scale_x < 0.5) {
			vec_add(my.scale_x, vector(time_step / 20, time_step / 20, time_step / 20));
			wait(1);
		}
		vec_set(my.scale_x, vector(0.5,0.5,0.5));
		my->event = _item_a4_cube_evt;
	}
}

// Rotiert, kann eingesammelt werden.
// Bringt Punkte. Bei 10 Punkten -> Sofortiger Speedboost.
action a4_cube()
{
	_item_setup();
	//my->trigger_range = 200;
	place_on_floor(me);
	my->z -=40;
	my->emask |=ENABLE_TRIGGER;
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
}

// Gebe dem Spieler ein zufälliges Item
void _give_random_item(ENTITY* driver)
{
	if (driver != NULL)
	{
		if (driver.item_id == ITEM_NONE) {
			driver.item_id = 1 + integer(random(7));
			
			// Todo: Zeige im Item-Panel wahllos ein paar Items in schneller
			// Rotation an.
			
			snd_play(sndGotNewItem, 50, 0);
		}
	}
}

// Event, der die Itemvergabe regelt
void _item_evt()
{
	if (event_type == EVENT_TRIGGER)
	{
		my->event = NULL;
		
		// Gib dem berührenden Fahrer ein neues Item
		_give_random_item(you);
		
		//achievement("firstitem");
		set(me,INVISIBLE);
		wait(-3);
		vec_set(my.scale_x, vector(0,0,0));
		reset(me,INVISIBLE);
		while(my.scale_x < 0.5) {
			vec_add(my.scale_x, vector(time_step / 20, time_step / 20, time_step / 20));
			wait(1);
		}
		vec_set(my->scale_x, vector(0.5,0.5,0.5));
		my->event = _item_evt;
	}
}

// Wird eingesammelt und vergibt ein Zufallsitem, falls noch
// keins im Slot ist. Verpufft, und wird 3 Sekunden später gerespawned.
action item()
{
	_item_setup();	
	place_on_floor(me);
	my->z -=40;
	my->emask |=ENABLE_TRIGGER;
	my->event = _item_evt;
	var vZ = my->z;
	var vOffset = random(500);
	var vParticles = 0;
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
		snd_play(sndGraveCollision, 50, 0);
		
		// Drehe Spieler
		trap_driver(you, 1);
		
		wait(1);
		ent_remove(me);
	}
}

// Aktion eines Grabsteins (Falle)
action grave() {
	_item_setup();
	wait(-1);
	my->emask |=ENABLE_TRIGGER;
	my->event = _grave_evt;
}

// Legt ein A9-Grab ab, das Fahrer ausbremst, wenn sie drüberfahren
void plant_grave(ENTITY* driver) {
	driver->item_id = ITEM_NONE;
	ENTITY* entGrave = ent_create(ITEM_GRAVE_MODEL, vector(driver->x-20, driver->y, driver->z+10), grave);
	vec_set(entGrave.scale_x, vector(0.2,0.2,0.2));
	place_on_floor(entGrave);
	driver->item_id = ITEM_NONE;
}

void p_rocket_smoke(PARTICLE* p)
{
	VECTOR vecTemp;
	vec_randomize(vecTemp, 2);
	vec_add(p.vel_x, vector(-1+random(2), -1+random(2), 1));
	vec_set(p.blue, vector(140, 140, 140));
	
	set(p, MOVE | TRANSLUCENT);
	p.alpha = 60 + random(20);
	p.size = 20;
	p.gravity = 0;
	p.skill_a = 10;
	p.event = p_fade;
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
	int liveTime = ROCKET_LIFE_FRAMES + integer(random(100));
	var flyHeight = 0;
	int reachedTop = 0;
	var initHeight = 0;
	var zSpeed = 0;
	var xSpeed = 0;
	var animPercentage = 0;
	set(me, PASSABLE);
	vec_set(my.scale_x, vector(0.1, 0.1, 0.1));
	c_setminmax(me);
	snd_play(sndRocketFire, 50, 0);
	VECTOR vertexPos;
	while(me && (liveTime > 0))
	{
		// Lass die Rakete nach Abschuss hüpfen
		if (reachedTop == 0) {
			flyHeight +=20 * time_step;
			zSpeed = 20 * time_step;
			xSpeed = 20 * time_step;
			if (flyHeight >= 60) reachedTop = 1;
		} else {
			if (flyHeight >= initHeight+3) {
				flyHeight -=20 * time_step;
				zSpeed = -20 * time_step;
				xSpeed +=ROCKET_SPEED * time_step;
			} else {
				zSpeed = 0;
			}
		}
		
		if (animPercentage + 20 * time_step < 80) {
			animPercentage +=20 * time_step;
		} else {
			animPercentage = 80;
			reset(me, PASSABLE);
			xSpeed = ROCKET_SPEED * 5 * time_step;
		}
		ent_animate(me, "Transform ",animPercentage, ANM_CYCLE);
		
		// Vertex 634
		vec_for_vertex(vertexPos, me, 634);
		effect(p_rocket_smoke, maxv(2,time_step), vertexPos, nullvector);
		
		c_move(me, vector(xSpeed, 0, zSpeed), nullvector, IGNORE_PASSABLE | IGNORE_PASSENTS | ACTIVATE_SHOOT | IGNORE_FLAG2 | GLIDE);
		
		if (HIT_TARGET != NULL)
		{
			if (hit.entity._type == type_kart)
			{
				driver_hit(hit.entity, 2);
			}
			liveTime = 0;
		}
		
		snd_play(sndRocketFly, 50, 0);
		liveTime--;
		wait(1);
		
	}
	effect(p_rocket_explode, maxv(40, 80*time_step), my->x, nullvector);
	// Play sound boom
	ent_remove(me);
}

// Schießt eine Rakete ab
void shoot_rocket(ENTITY* driver)
{
	// Erzeuge Rakete
	VECTOR* rocketStart = vector(ROCKET_X_OFFSET,0,0);
	vec_rotate(rocketStart, driver->pan);
	vec_add(rocketStart, driver->x);
	ENTITY* rocket = ent_create(ITEM_ROCKET_MODEL, rocketStart->x, _rocket);
	rocket->pan = driver->pan;
	driver->item_id = ITEM_NONE;
}

// Rakete, die zielgelenkt in Richtung des nächsten Spielers fliegt
action _aiming_rocket()
{
	int liveTime = ROCKET_LIFE_FRAMES + integer(random(100));
	var flyHeight = 0;
	int reachedTop = 0;
	var initHeight = 0;
	var zSpeed = 0;
	var xSpeed = 0;
	var animPercentage = 0;
	set(me, PASSABLE);
	vec_set(my.scale_x, vector(0.1, 0.1, 0.1));
	c_setminmax(me);
	snd_play(sndRocketFire, 50, 0);
	VECTOR vertexPos;
	while(me && (liveTime > 0))
	{
		// Lass die Rakete nach Abschuss hüpfen
		if (reachedTop == 0) {
			flyHeight +=20 * time_step;
			zSpeed = 20 * time_step;
			xSpeed = 20 * time_step;
			if (flyHeight >= 60) reachedTop = 1;
		} else {
			if (flyHeight >= initHeight+3) {
				flyHeight -=20 * time_step;
				zSpeed = -20 * time_step;
				xSpeed +=ROCKET_SPEED * time_step;
			} else {
				zSpeed = 0;
			}
		}
		
		if (animPercentage + 20 * time_step < 80) {
			animPercentage +=20 * time_step;
		} else {
			animPercentage = 80;
			reset(me, PASSABLE);
			xSpeed = ROCKET_SPEED * 5 * time_step;
		}
		ent_animate(me, "Transform ",animPercentage, ANM_CYCLE);
		
		// Vertex 634
		vec_for_vertex(vertexPos, me, 634);
		effect(p_rocket_smoke, maxv(2,time_step), vertexPos, nullvector);
		
		c_move(me, vector(xSpeed, 0, zSpeed), nullvector, IGNORE_PASSABLE | IGNORE_PASSENTS | ACTIVATE_SHOOT | IGNORE_FLAG2 | GLIDE);
		
		if (HIT_TARGET != NULL)
		{
			if (hit.entity._type == type_kart)
			{
				driver_hit(hit.entity, 3);
			}
			liveTime = 0;
		}
		
		snd_play(sndRocketFly, 50, 0);
		liveTime--;
		wait(1);
	}
	effect(p_rocket_explode, maxv(40, 80*time_step), my->x, nullvector);
	// Play sound boom
	ent_remove(me);
}

// OPTIONAL: Schießt eine leicht zielgelenkte Rakete ab
void shoot_aiming_rocket(ENTITY* driver)
{
	// Erzeuge Rakete
	VECTOR* rocketStart = vector(ROCKET_X_OFFSET,0,0);
	vec_rotate(rocketStart, driver->pan);
	vec_add(rocketStart, driver->x);
	ENTITY* rocket = ent_create(ITEM_AIMING_ROCKET_MODEL, rocketStart->x, _aiming_rocket);
	rocket->pan = driver->pan;
	driver->item_id = ITEM_NONE;
}

// Beschleunigt den Spieler für 3 Sekunden
// auf 1,4(?)-fache Geschwindigkeit
void use_turbo(ENTITY* driver) {
	driver->item_id = ITEM_NONE;
	snd_play(sndTurboStart, 50, 0);
	start_turbo(driver, 4);
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
		snd_play(sndSpikesDown, 50, 0);
		while(my->z > vPosDown)
		{
			wait(1);
			c_move(me, nullvector, vector(0, 0, -SPIKE_SPEED * time_step), IGNORE_MODELS | IGNORE_SPRITES | IGNORE_WORLD | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS);
	
		}
		my->z = vPosDown;
		
		wait(-SPIKE_TIMER_DOWN);	
		set(me, IS_TRAP);
		snd_play(sndSpikesUp, 50, 0);
		while(my->z < vPosUp)
		{
			wait(1);
			c_move(me, nullvector, vector(0, 0, SPIKE_SPEED * time_step), IGNORE_MODELS | IGNORE_SPRITES | IGNORE_WORLD | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS);
	
		}
		my->z = vPosUp;
		
		wait(1);
	}
}

void p_bomb(PARTICLE *p) {
	VECTOR vecTemp;
	vec_randomize(vecTemp, -1);
	vec_add(p.vel_x, vecTemp);
	vec_set(p.blue, vector(random(255), random(255), 255));
	set(p, MOVE | BRIGHT | TRANSLUCENT);
	p.alpha = 100;
	p.size = 3;
	p.gravity = 1;
	p.skill_a = 3;
	p.bmap = bmapStar;
	p.event = p_fade;
}

// Rakete, die direkt zum ersten bzw. nächsten Spieler fliegt
action _badass_aiming_rocket()
{
	vec_set(my.scale_x, vector(0.2, 0.2, 0.2));
	c_setminmax(me);
	snd_play(sndRocketFire, 50, 0);
	int liveTime = BADASS_ROCKET_LIFE_FRAMES + integer(random(500));
	var animPercentage = 0;
	set(me, PASSABLE);
	while(me && (liveTime > 0))
	{
		effect(p_bomb, maxv(2,time_step), vector(my->x, my->y, my->z+10), nullvector);
		
		c_move(me, vector(ROCKET_SPEED * 5 * time_step, 0, 0), nullvector, IGNORE_PASSABLE | IGNORE_PASSENTS | ACTIVATE_SHOOT | IGNORE_FLAG2 | GLIDE);
		
		if (HIT_TARGET != NULL)
		{
			if (hit.entity._type == type_kart)
			{
				driver_hit(hit.entity, 5);
			}
			liveTime = 0;
		}
			
		snd_play(sndRocketFly, 50, 0);	
		liveTime--;
		wait(1);
	}
	effect(p_rocket_explode, maxv(40, 80*time_step), my->x, nullvector);
	// Play sound boom
	ent_remove(me);
}

// OPTIONAL: Schießt einen (Lotti-)Ghost auf den 1. Spieler
// (wenn man es nicht selber ist, sonst auf den letzten).
// Slashed den Getroffenen mit einem LOTTI-Schrei für 3 Sekunden
void shoot_badass_aiming_rocket(ENTITY* driver)
{
	// Erzeuge Rakete
	VECTOR* rocketStart = vector(ROCKET_X_OFFSET,0,0);
	vec_rotate(rocketStart, driver->pan);
	vec_add(rocketStart, driver->x);
	driver->item_id = ITEM_NONE;
	ENTITY* rocket = ent_create(ITEM_BADASS_ROCKET_MODEL, rocketStart.x, _badass_aiming_rocket);	
	rocket->pan = driver->pan;
}

// Macht den Spieler größer und er kann andere überfahren,
// die nicht auch groß sind
void start_mushroom(ENTITY* driver)
{
	// OPTIONAL: Tue verrückte Dinge mit den Farben, falls nicht schon aktiv
	snd_play(sndMushroomStart, 50, 0);
	driver->item_id = ITEM_NONE;
	enlarge_driver(driver, 5);
}

// Macht alle Spieler klein und langsamer (5 Sekunden)
// bis auf den Schützen
void start_flash(ENTITY* driver)
{
	snd_play(sndFlashStart, 50, 0);
	driver->item_id = ITEM_NONE;
	ENTITY* ki1 = get_kart_driver(1);
	ENTITY* ki2 = get_kart_driver(2);
	ENTITY* ki3 = get_kart_driver(3);
	if (ki1 != NULL)
	{
		if (ki1.scale_x <= 1.0) {
			minimize_driver(get_kart_driver(1), 4);
		}
	}
	if (ki2 != NULL)
	{
		if (ki2.scale_x <= 1.0) {
			minimize_driver(get_kart_driver(2), 4);
		}
	}
	if (ki3 != NULL)
	{
		if (ki3.scale_x <= 1.0) {
			minimize_driver(get_kart_driver(3), 4);
		}
	}
}

#endif /*items_c*/