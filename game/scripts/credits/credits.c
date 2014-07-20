#ifndef _CREDITS_C_
#define _CREDITS_C_

#include <windows.h>
#include "credits.h"
#include "credits_flow.h"


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


int creditsCurrentStage = 0;

VECTOR creditsFrom, creditsTo;
ANGLE creditsFromA, creditsToA;
var creditsBlendPerc = 0;
var creditsBlendSpeed = 0;

void credits_stage() {
	if(creditsCurrentStage >= credits_stageCount) {
		error("end!");
		return;
	}
	
	creditsBlendPerc = 0.1;
	
	vec_set(creditsFrom, camera.x);
	vec_set(creditsFromA, camera.pan);
	
	vec_set(creditsTo, credits_stages[creditsCurrentStage].x);
	vec_set(creditsToA, credits_stages[creditsCurrentStage].pan);
	
	creditsBlendSpeed = credits_stages[creditsCurrentStage].speed;
	
	creditsCurrentStage += 1;
}

action credits_lottiPlay()
{
	while(me) {
		ent_animate(me, "Play", credits.lottiSpeed * total_ticks, ANM_CYCLE);
		wait(1);
	}
}

/**
 * Initializes the credits.
 */
void credits_init()
{
	memset(&credits, 0, sizeof(Credits));
	credits.lottiSpeed = 10;
	
	credits_initstages();
	
	credits_addev(0, credits_stage);
	//credits_addev(7000, credits_stage);
	//credits_addev(14000, credits_stage);
	//credits_addev(21000, credits_stage);
	//credits_addev(28000, credits_stage);
	//credits_addev(35000, credits_stage);
	//credits_addev(42000, credits_stage);
	//credits_addev(49000, credits_stage);
	//credits_addev(56000, credits_stage);
	//credits_addev(63000, credits_stage);
	//credits_addev(70000, credits_stage);
	//credits_addev(77000, credits_stage);
	//credits_addev(84000, credits_stage);
	//credits_addev(91000, credits_stage);
	//credits_addev(98000, credits_stage);
	
	wait(1);
	
	while(1) {
		// Wait for running credits
		while(_credits_music == 0) wait(1);

		creditsCurrentStage = 0;
		creditsBlendPerc = 0;

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
				diag("\n_add_stage(");
				diag(str_for_num(NULL, camera.x)); diag(", ");
				diag(str_for_num(NULL, camera.y)); diag(", ");
				diag(str_for_num(NULL, camera.z)); diag(", ");
				diag(str_for_num(NULL, camera.pan)); diag(", ");
				diag(str_for_num(NULL, camera.tilt)); diag(", ");
				diag(str_for_num(NULL, camera.roll)); diag(", 1);");
			}
			
			if(creditsBlendPerc > 0 && creditsBlendPerc < 100) {
				creditsBlendPerc = minv(creditsBlendPerc + creditsBlendSpeed * time_step, 100);
				if(creditsBlendPerc == 100) {
					diag("\nBlend stage finished: ");
					diag(str_for_int(NULL, time));
					diag(" ms");
				}
				
				var blend = sin(0.5 * 0.0314159 * creditsBlendPerc);
				
				vec_lerp(camera.x, creditsFrom, creditsTo, blend);
				_ang_lerp(camera.pan, creditsFromA, creditsToA, blend);
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
	
	ENTITY *stage = ent_create("stage_buehne.mdl", vector(0, 500, 20), NULL);
	stage.pan = 180;
	stage.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_dach.mdl", vector(0, 500, 20), NULL);
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_drums.mdl", vector(0, 500, 20), NULL);
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_stangen.mdl", vector(0, 500, 20), NULL);
	you.pan = 180;
	you.material = _credits_mtlMetal;
	
	you = ent_create("lotterBass.mdl", stage.x, credits_lottiPlay);
	vec_add(you.x, vector(-90, 40, 90));
	you.pan = 300;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("lotterGitarre.mdl", stage.x, credits_lottiPlay);
	vec_add(you.x, vector(90, 40, 90));
	you.pan = 230;
	you.material = _credits_mtlAlphaTest;
	
	
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
