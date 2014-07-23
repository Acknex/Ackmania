#ifndef items_c
	#define items_c

	#include "helper.h"
	#include "startgrid.h"
	#include "camera.h"
	#include "raceplayer.h"
	#include <particles.c>

	STRING* str_items_helper = "";

	void patrol_path(ENTITY* ent, char* pathname, var _speed, var mode) {
		var vLastPos[3],vDir[3];
		vec_set(vLastPos,ent.x);
		var dist = 0;
		if (pathname) path_set(ent,pathname);
		while(_speed) 
		{
			path_spline(ent,ent.x,dist);
			dist += _speed*time_step;
			place_on_floor(ent);
			vec_diff(vDir,ent.x,vLastPos);
			vec_to_angle(ent.pan,vDir);
			vec_set(vLastPos,ent.x);
			wait(1);
		}
	}


	// Gib das aktuelle Item des Spielers zurück
	var get_current_item_id()
	{
		ENTITY* driver = get_kart_player();
		if (driver != NULL)
		{
			return driver.item_id;
		}
		return 0;
	}

	var get_nr_cubes()
	{
		ENTITY* driver = get_kart_player();
		if (driver != NULL)
		{
			return driver->PL_A4_COUNT;
		}
		return 0;
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
		vec_randomize(&vecTemp, 8);
		vec_normalize(&vecTemp, 4);
		vec_add (&p->vel_x, &vecTemp);
		p->vel_y = 0;
		vec_set(&p->blue, vector(0, 255, 255));
		vec_scale(p->blue,0.8+random(0.2));
		set(p, MOVE | TRANSLUCENT | BRIGHT);
		p->lifespan = 80;
		p->size  = 15 + random(3);
		p->alpha = 60;
		p->event = _item_particleFader;
	}
	
	void _item_particle2 (PARTICLE *p) 
	{
		VECTOR vecTemp;
		vec_randomize(&vecTemp, 8);
		vec_normalize(&vecTemp, 4);
		vec_add (&p->vel_x, &vecTemp);
		p->vel_y = 0;
		vec_set(&p->blue, vector(255, 0, 255));
		vec_scale(p->blue,0.8+random(0.2));
		set(p, MOVE | TRANSLUCENT | BRIGHT);
		p->lifespan = 80;
		p->size  = 15 + random(3);
		p->alpha = 60;
		p->event = _item_particleFader;
	}

	// Event für einen einsammelbaren Cube (Münzen)
	void _item_a4_cube_evt()
	{
		if (event_type == EVENT_TRIGGER)
		{
			my->event = NULL;
			snd_play(sndCollectA4Cube, 50, 0);
			if (you != NULL) you.PL_A4_COUNT = minv(you.PL_A4_COUNT+1,10);
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
		my->z = 32;
		my->trigger_range = 50;
		my->event = _item_a4_cube_evt;
		var vZ = my->z;
		var vOffset = random(500);
		var vParticles = 0;
		my->material = mtl_specBump;
		my->pan = random(360);
		vec_set(my->scale_x, vector(0.5,0.5,0.5));
		
		while(me)
		{
			vParticles += time_step;
			if (vParticles > 5)
			{
				if (!is(me,INVISIBLE)) {
					effect(_item_particle2, 5, &my->x, nullvector);
					vParticles -= 5;
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
		var item_lastid;
		var item_shuffle = 8;
		if (driver != NULL)
		{
			if (driver.item_id == ITEM_NONE) {
				
				if(driver.sk_kart_id == 1)
				{
					driver->fire_item = -1; /* lock fire button */
					for (item_shuffle = 0; item_shuffle < 8; item_shuffle++)
					{
						item_lastid = driver.item_id;
						do
						{
							driver.item_id = 1 + integer(random(7));
						} while (driver.item_id == item_lastid);
						snd_play(sndItemShuffle, 50, 0);
						wait(-0.1);
					}
					driver->fire_item = 0; /* enable fire */
				}
				driver.item_id = 1 + integer(random(7));
				
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
		my->z = -8 -my->min_z;
		my->emask |=ENABLE_TRIGGER;
		my->event = _item_evt;
		var vZ = my->z;
		my->trigger_range = 50;
		var vOffset = random(500);
		var vParticles = 0;
		my->pan = random(360);
		my->tilt = -90;
		vec_set(my->scale_x, vector(0.5,0.5,0.5));
		
		while(me)
		{
			vParticles += time_step;
			if (vParticles > 5)
			{
				if (!is(me,INVISIBLE)) {
					effect(_item_particle, 5, &my->x, nullvector);
					vParticles -= 5;
				}
			}
			//my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
			//my->pan -= 5 * time_step;
			//my->tilt = 30 * sinv(total_ticks * 10 + vOffset);
			wait(1);
		}
	}

	void p_blumenkasten2(PARTICLE*);
	void p_grave(PARTICLE*);

	// Event, der die Kollision mit einem Grabstein regelt
	void _grave_evt()
	{
		if (event_type == EVENT_TRIGGER)
		{
			my->event = NULL;
			snd_play(sndGraveCollision, 50, 0);
			set(my,PASSABLE);
			effect(p_blumenkasten2,32,my.x,nullvector);
			effect(p_grave,32,my.x,nullvector);
			
			// Drehe Spieler
			trap_driver(you, 3);
			
			wait(1);
			ent_remove(me);
		}
	}

	// Aktion eines Grabsteins (Falle)
	action grave() {
		_item_setup();
		reset(my,SHADOW);
		my->pan = random(360);
		wait(-0.5);
		my->emask |=ENABLE_TRIGGER;
		my->event = _grave_evt;
	}

	// Legt ein A9-Grab ab, das Fahrer ausbremst, wenn sie drüberfahren
	void plant_grave(ENTITY* driver) {
		driver->item_id = ITEM_NONE;
		ENTITY* entGrave = ent_create(ITEM_GRAVE_MODEL, vector(driver->x-20, driver->y, driver->z+10), grave);
		vec_fill(entGrave.scale_x, 0.35);
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
		VECTOR temp,temp2,temp3;
		var liveTime = 32;
		var flyHeight = 0;
		int reachedTop = 0;
		var initHeight = 0;
		var zSpeed = 0;
		var xSpeed = 0;
		var animPercentage = 0;
		//set(me, PASSABLE);
		vec_set(my.scale_x, vector(0.1, 0.1, 0.1));
		snd_play(sndRocketFire, 50, 0);
		while(me && (liveTime > 0))
		{
			animPercentage = minv(animPercentage+20*time_step,100);
			ent_animate(me, "Transform ",animPercentage, 0);
			
			// Vertex 634
			vec_for_vertex(temp, me, 634);
			effect(p_rocket_smoke, 2, temp, nullvector);
			
			if(animPercentage >= 50)
			{
				int i;
				for(i = 0; i < 4; i++)
				{
					if(i+1 != my.sk_kart_id)
					{
						ENTITY* ent = get_kart_driver(i);
						if(ent)
						{
							if(vec_dist(vector(ent.x,ent.y,0),vector(my.x,my.y,0)) < 48)
							{
								trap_driver(ent,2.25);
								liveTime = 0;
							}
						}
					}
				}
			}
			ENTITY* ent = NULL;		
			if(my.sk_kart_id > 0)
			{
				ent = get_kart_driver(my.sk_kart_id-1);
				if(ent) set(ent,PASSABLE);
			}
			vec_set(temp2,vector(ROCKET_SPEED * 6 * time_step,0,0));
			c_move(me,temp2,nullvector,IGNORE_PASSABLE | IGNORE_SPRITES | IGNORE_FLAG2);
			if(ent) reset(ent,PASSABLE);
			if(HIT_TARGET) liveTime = 0;
			my.z = 48;
			//vec_rotate(temp2,my.pan);
			//vec_add(my.x,temp2);
			
			liveTime -= time_step;
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
		rocket->sk_kart_id = driver->sk_kart_id;
		driver->item_id = ITEM_NONE;
	}

	// Rakete, die zielgelenkt in Richtung des nächsten Spielers fliegt
	action _aiming_rocket()
	{
		VECTOR temp,temp2,temp3;
		var liveTime = 32;
		var flyHeight = 0;
		int reachedTop = 0;
		var initHeight = 0;
		var zSpeed = 0;
		var animPercentage = 0;
		
		set(me, PASSABLE);
		vec_set(my.scale_x, vector(0.1, 0.1, 0.1));
		snd_play(sndRocketFire, 50, 0);
		
		//determine position near path and side of position in relation to path direction
		path_get_closest_position(my.x,temp,NULL);
		temp.z = my.z;
		my.skill21 = vec_dist(my.x,temp);
		my.skill22 = my.skill2/g_raceTrackWidth;
		path_get_normal_position(my.x,0,my.skill22,temp2);
		temp2.z = my.z;
		if(vec_dist(temp2,my.x) > my.skill21) my.skill22 *= -1;
		my.skill22 *= -1;
		
		while(me && (liveTime > 0))
		{
			animPercentage = minv(animPercentage+20*time_step,100);
			ent_animate(me, "Transform ",animPercentage, 0);
			
			// Vertex 634
			vec_for_vertex(temp, me, 634);
			effect(p_rocket_smoke, 2, temp, nullvector);
			
			if(!my.skill20)
			{
				my.skill22 += -my.skill22*0.025*time_step;
				path_get_normal_position(my.x,64,my.skill22,temp2);
				vec_diff(temp,temp2,my.x);
				vec_to_angle(my.pan,temp);
				my.tilt = 0;
				if(animPercentage >= 50)
				{
					int i;
					for(i = 0; i < 4; i++)
					{
						if(i+1 != my.sk_kart_id)
						{
							ENTITY* ent = get_kart_driver(i);
							if(ent)
							{
								vec_diff(temp,ent.x,my.x);
								vec_set(temp2,temp);
								vec_normalize(temp2,1);
								vec_for_angle(temp3,my.pan);
								if(vec_length(temp) < 256 && vec_dot(temp3,temp2) > 0.1) my.skill20 = i+1;
							}
						}
					}
				}		
			}
			else
			{
				ENTITY* ent = get_kart_driver(my.skill20-1);
				vec_diff(temp,ent.x,my.x);
				temp.z = 0;
				vec_to_angle(temp2,temp);
				my.pan += ang(temp2.x-my.pan)*time_step;	
				if(vec_length(temp) < 48)
				{
					trap_driver(ent,2.25);
					liveTime = 0;
				}
			}
			vec_set(temp2,vector(ROCKET_SPEED * 4.75 * time_step,0,0));
			vec_rotate(temp2,my.pan);
			vec_add(my.x,temp2);
			
			liveTime -= time_step;
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
		rocket->sk_kart_id = driver->sk_kart_id;
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
	
	MATERIAL* mat_bomb_explo = 
	{
		effect = "explo.fx";
		flags = AUTORELOAD;
	}
	
	void bomb_explo()
	{
		my.z = 0;
		set(my,UNLIT | PASSABLE | TRANSLUCENT); // | BRIGHT);
		my.material = mat_bomb_explo;
		my.skill20 = 6;
		while(my.skill20 > 0)
		{
			my.skill41 = floatv(pow(my.skill20/6.0,0.65));
			my.skill20 -= time_step;
			wait(1);
		}
		ptr_remove(me);
	}

	// Rakete, die direkt zum ersten bzw. nächsten Spieler fliegt
	action _badass_aiming_rocket()
	{
		vec_set(my.scale_x, vector(0.2, 0.2, 0.2));
		c_setminmax(me);
		snd_play(sndRocketFire, 50, 0);
		var liveTime = 8; //BADASS_ROCKET_LIFE_FRAMES + integer(random(500));
		var animPercentage = 0;
		set(me, PASSABLE);
		
		path_next(my);
		str_cpy(str_items_helper,"");
		path_set(me, str_items_helper);
		var dist = get_nearest_path_point(me, str_items_helper);
		var vLastPos[3];
		var vDir[3];
		
		while(me && (liveTime > 0))
		{
			effect(p_bomb, maxv(2,time_step), vector(my->x, my->y, my->z+10), nullvector);
			
			path_spline(me,my->x,dist);
			my->z -=35;
			dist -= ROCKET_SPEED * time_step;
			vec_diff(vDir,my.x,vLastPos);
			vec_to_angle(my.pan,vDir);
			vec_set(vLastPos,my.x);
			
			if(liveTime < 6)
			{
				int i;
				for(i = 0; i < 4; i++)
				{
					ENTITY* ent = get_kart_driver(i);
					if(ent)
					{
						if(vec_dist(ent.x,my.x) < 48) liveTime = 0;
					}
				}
			}
			/*if (HIT_TARGET != NULL)
			{
				if (hit.entity._type == type_kart)
				{
					driver_hit(hit.entity, 5);
				}
				liveTime = 0;
			}*/
			
			liveTime -= time_step;
			wait(1);
		}
		int i;
		for(i = 0; i < 4; i++)
		{
			ENTITY* ent = get_kart_driver(i);
			if(ent)
			{
				if(vec_dist(vector(ent.x,ent.y,0),vector(my.x,my.y,0)) < 256) trap_driver(ent,2.5); //driver_hit(ent, 3.5);
			}
		}
		ent_create("bomb_explo.mdl",my.x,bomb_explo);
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
		VECTOR* rocketStart = vector(-32,0,0);
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
		snd_play(sndMushroomStart, 100, 0);
		driver->item_id = ITEM_NONE;
		enlarge_driver(driver, 5.25);
	}

	// Macht alle Spieler klein und langsamer (5 Sekunden)
	// bis auf den Schützen
	void start_flash(ENTITY* driver)
	{
		snd_play(sndFlashStart, 50, 0);
		driver->item_id = ITEM_NONE;
		
		var i;
		VIEW* pV = get_camera();
		for (i = 0; i < 6; i++)
		{
			pV->ambient = 30 + abs(random(70));
			wait (-0.1);
		}
		pV->ambient = 0;
		
		ENTITY* pl = get_kart_driver(0);
		ENTITY* ki1 = get_kart_driver(1);
		ENTITY* ki2 = get_kart_driver(2);
		ENTITY* ki3 = get_kart_driver(3);
		if (pl != NULL && pl != driver)
		{
			if (!pl->kart_big) {
				minimize_driver(get_kart_driver(0), 5);
			}
		}
		if (ki1 != NULL && ki1 != driver)
		{
			if (!ki1->kart_big) {
				minimize_driver(get_kart_driver(1), 5);
			}
		}
		if (ki2 != NULL && ki2 != driver)
		{
			if (!ki2->kart_big) {
				minimize_driver(get_kart_driver(2), 5);
			}
		}
		if (ki3 != NULL && ki3 != driver)
		{
			if (!ki3->kart_big) {
				minimize_driver(get_kart_driver(3), 5);
			}
		}
	}

#endif /*items_c*/