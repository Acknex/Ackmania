#ifndef _CREDITS_C_
#define _CREDITS_C_

#include <windows.h>
#include "credits.h"

#define CREDITS_DEF_MAX_EVENTS 256

typedef struct {
	int time;
	void fn();
} _TimedEvent;

var _credits_music = 0;
ENTITY *_credits_sky;

_TimedEvent _credits_events[CREDITS_DEF_MAX_EVENTS];
int _credits_eventCount = 0;

MATERIAL *_credits_mtlAlphaTest = {
	effect = "technique { pass { CullMode = None; AlphaTestEnable = false; AlphaBlendEnable = false; ZEnable = true; ZWriteEnable = true; ZFunc = LessEqual; } }";
}

MATERIAL *_credits_mtlMetal = {
	effect = "technique { pass { CullMode = None; AlphaTestEnable = false; AlphaBlendEnable = false; ZEnable = true; ZWriteEnable = true; ZFunc = LessEqual; } }";
	power = 16;
	specular_blue = 255;
	specular_green = 230;
	specular_red = 230;
}

var _ang_lerp_single(var a1, var a2, var f)
{
	var angle = ang(a1 - a2);
	return ang(a1 - (f * angle));
}

ANGLE* _ang_lerp(ANGLE* a, ANGLE* a1, ANGLE* a2, var f)
{
	ANGLE ai;
	ai.pan = _ang_lerp_single(a1->pan, a2->pan, f);
	ai.tilt = _ang_lerp_single(a1->tilt, a2->tilt, f);
	ai.roll = _ang_lerp_single(a1->roll, a2->roll, f);

	if(a != NULL)
		vec_set(a, &ai);

	return vector(ai.pan, ai.tilt, ai.roll);
}

void credits_addev(int ms, void *fn)
{
	_credits_events[_credits_eventCount].time = ms;
	_credits_events[_credits_eventCount].fn = fn;
	_credits_eventCount++;
}

void credev_init_scene()
{
	vec_set(camera.x, vector(175, -2211, 531));
	vec_set(camera.pan, vector(94, -13, 0));
}

void credev_camera_flight_a()
{
	var delta;
	for(delta = 0; delta <= 100; delta += time_step)
	{
		vec_lerp(
			camera.x, 
			vector(175, -2211, 531),
			vector(0, 0, 0),
			0.01 * delta);
		_ang_lerp(
			camera.pan,
			vector(94, -13, 0),
			vector(-94, 13, 0),
			0.01 * delta);
		wait(1);
	}
}

/**
 * Initializes the credits.
 */
void credits_init()
{
	memset(&credits, 0, sizeof(Credits));
	
	credits_addev(0, credev_init_scene);
	//credits_addev(500, credev_camera_flight_a);
	
	wait(1);
	
	while(1) {
		// Wait for running credits
		while(_credits_music == 0) wait(1);

		int currentEvent = 0;
		int startUpTime = GetTickCount();
		while(_credits_music != 0) {
			int time = GetTickCount() - startUpTime;
			while(currentEvent < _credits_eventCount && _credits_events[currentEvent].time <= time) {
				if(_credits_events[currentEvent].fn != NULL) {
					void callback();
					callback = _credits_events[currentEvent].fn;
					callback();
				}
				currentEvent++;
			}
			if(media_playing(_credits_music) == 0) {
				credits_stop();
				break;
			}
			if(key_f) {
				diag("\n");
				diag("x: "); diag(str_for_num(NULL, camera.x)); diag(" ");
				diag("y: "); diag(str_for_num(NULL, camera.y)); diag(" ");
				diag("z: "); diag(str_for_num(NULL, camera.z)); diag(" ");
			}
			wait(1);	
		}
	
	}
}

/**
 * Starts the credits.
 */
void credits_start()
{
	if(_credits_music != 0) {
		return;
	}
	level_load("creditsTerrain.hmp");
	level_ent.ambient = -100;
	_credits_sky = ent_createlayer("cosmo_f02+6.jpg", SKY | CUBE, 10);
	
	you = ent_create("stage_buehne.mdl", vector(0, 500, 20), NULL);
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_dach.mdl", vector(0, 500, 20), NULL);
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_drums.mdl", vector(0, 500, 20), NULL);
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_stangen.mdl", vector(0, 500, 20), NULL);
	you.pan = 180;
	you.material = _credits_mtlMetal;
	
#ifdef TEST_DEBUG
	_credits_music = media_play("../../media/outro-demo.ogg", NULL, 100);
#else
	_credits_music = media_play("media\\outro-demo.ogg", NULL, 100);
#endif
}

/**
 * Stops the credits.
 */
void credits_stop()
{
	if(_credits_sky != NULL) {
		ptr_remove(_credits_sky);
		_credits_sky = NULL;
	}
	if(_credits_music == 0) {
		return;
	}
	if(credits.onCreditsEnd != NULL) {
		void tmp();
		tmp = credits.onCreditsEnd;
		tmp();
	}	
	media_stop(_credits_music);
	_credits_music = 0;
}

#endif // #ifndef _CREDITS_C_
