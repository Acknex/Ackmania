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

MATERIAL *_credits_mtlAlphaAdd = {
	effect = "technique { pass { CullMode = CCW; AlphaTestEnable = false; AlphaBlendEnable = True; SrcBlend = SrcAlpha; DestBlend = One; ZEnable = true; ZWriteEnable = false; ZFunc = LessEqual; } }";
}

MATERIAL *_credits_mtlMetal = {
	effect = "technique { pass { CullMode = None; AlphaTestEnable = false; AlphaBlendEnable = false; ZEnable = true; ZWriteEnable = true; ZFunc = LessEqual; } }";
	power = 16;
	specular_blue = 255;
	specular_green = 230;
	specular_red = 230;
}

TEXT *creditsNamePan = {
	pos_x = 32;
	pos_y = 32;
	strings = 1;
	font = "Courier#64b";
	flags = TRANSLUCENT;
}

TEXT *creditsWorkPan1 = {
	pos_x = 32;
	pos_y = 32;
	strings = 1;
	font = "Courier#48b";
	flags = TRANSLUCENT | ARIGHT;
}

TEXT *creditsWorkPan2 = {
	pos_x = 32;
	pos_y = 32;
	strings = 1;
	font = "Courier#48b";
	flags = TRANSLUCENT | ARIGHT;
}

TEXT *creditsWorkPan3 = {
	pos_x = 32;
	pos_y = 32;
	strings = 1;
	font = "Courier#48b";
	flags = TRANSLUCENT | ARIGHT;
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

action credits_lottiLatsch()
{
	VECTOR t;
	t.x = random(1000) - 500;
	t.y = 350 - random(500);
	t.z = 26;
	var offset = random(100);
	while(me) {
		
		if(vec_dist(my.x, t) < 0.1) break;
		
		vec_diff(my.pan, t, my.x);
		vec_normalize(my.pan, minv(10 * time_step, vec_dist(my.x, t)));
		vec_add(my.x, my.pan);
		vec_to_angle(my.pan, my.pan);
		my.roll = 0;
		
		ent_animate(me, "walk", 10 * total_ticks + offset, ANM_CYCLE);
		
		wait(1);
	}
	
	// Look at stage
	vec_diff(my.pan, vector(0, 800, 20), my.x);
	vec_to_angle(my.pan, my.pan);
	my.roll = 0;
	
	while(me) {
	
		var time = 16 + random(16);
		while(time > 0) {
			ent_animate(me, "stand", 8.9 * total_ticks + offset, ANM_CYCLE);
			time -= time_step;
			wait(1);
		}
		for(time = 0; time < 60; time += 10 * time_step) {
			ent_animate(me, "action", time, 0);
			wait(1);
		}
		for(; time > 0; time -= 10 * time_step) {
			ent_animate(me, "action", time, 0);
			wait(1);
		}
		wait(1);
	}
}

int creditsCurrentEntry = 0;

void credits_nextentry() {
	int count = creditsContent.strings / 4;
	
	if(creditsCurrentEntry >= count) {
		return;
	}
	
	TEXT *texts[4];
	texts[0] = creditsNamePan;
	texts[1] = creditsWorkPan1;
	texts[2] = creditsWorkPan2;
	texts[3] = creditsWorkPan3;
	
	var blendSpeed = 12;
	
	int i;
	for(i = 0; i < 4; i++) {
		(texts[i].pstring)[0] = (creditsContent.pstring)[4 * creditsCurrentEntry + i];
		for(texts[i].alpha = 0; texts[i].alpha < 100; texts[i].alpha += blendSpeed * time_step) {
			wait(1);
		}
		texts[i].alpha = 100;
	}
	
	creditsCurrentEntry += 1;
	wait(-1);
	
	blendSpeed = 24;
	
	for(i = 3; i >= 0; i--) {
		for(texts[i].alpha = 100; texts[i].alpha > 0; texts[i].alpha -= blendSpeed * time_step) {
			wait(1);
		}
		texts[i].alpha = 0;
	}
}

/**
 * Initializes the credits.
 */
void credits_init()
{
	memset(&credits, 0, sizeof(Credits));
	credits.lottiSpeed = 18;
	
	credits_initstages();
	
	credits_addev(0, credits_stage);
	//credits_addev(3500, credits_nextentry);
	credits_addev(7000, credits_stage);
	credits_addev(14000, credits_stage);
	credits_addev(21000, credits_stage);
	credits_addev(28000, credits_stage);
	credits_addev(35000, credits_stage);
	credits_addev(42000, credits_stage);
	credits_addev(49000, credits_stage);
	credits_addev(56000, credits_stage);
	credits_addev(63000, credits_stage);
	credits_addev(70000, credits_stage);
	credits_addev(77000, credits_stage);
	credits_addev(84000, credits_stage);
	credits_addev(90000, credits_nextentry);
	credits_addev(91000, credits_stage);
	credits_addev(95000, credits_nextentry);
	
	int i;
	int count = creditsContent.strings / 4;
	for(i = 2; i < count; i++) {
		credits_addev(100000 + 5000 * (i-2), credits_nextentry);
	}
	
	wait(1);
	
	while(1) {
		// Wait for running credits
		while(_credits_music == 0) wait(1);

		creditsCurrentEntry = 0;
		creditsCurrentStage = 0;
		creditsBlendPerc = 0;
		
		creditsNamePan.flags |= SHOW;
		creditsWorkPan1.flags |= SHOW;
		creditsWorkPan2.flags |= SHOW;
		creditsWorkPan3.flags |= SHOW;

		var nextLotti = 128;
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
			
			if(creditsBlendPerc > 0 && creditsBlendPerc < 100) {
				creditsBlendPerc = minv(creditsBlendPerc + creditsBlendSpeed * time_step, 100);
				if(creditsBlendPerc == 100) {
					//diag("\nBlend stage finished: ");
					//diag(str_for_int(NULL, time));
					//diag(" ms");
				}
				
				var blend = sin(0.5 * 0.0314159 * creditsBlendPerc);
				
				vec_lerp(camera.x, creditsFrom, creditsTo, blend);
				_ang_lerp(camera.pan, creditsFromA, creditsToA, blend);
			}
			
			if(nextLotti <= 0 && num_entities < 80) {
				if(random(100) < 80) {
					ent_create("lotter.mdl", vector(random(2000)-1000, -2048, 26), credits_lottiLatsch);
				} else {
					ent_create("lottifant.mdl", vector(random(2000)-1000, -2048, 26), credits_lottiLatsch);
				}
				nextLotti = 16 + random(32);
			}
			
			creditsWorkPan1.pos_x = screen_size.x - 64;
			creditsWorkPan2.pos_x = screen_size.x - 64;
			creditsWorkPan3.pos_x = screen_size.x - 64;
			
			creditsWorkPan1.pos_y = screen_size.y - 64 * 1 - 48;
			creditsWorkPan2.pos_y = screen_size.y - 64 * 2 - 48;
			creditsWorkPan3.pos_y = screen_size.y - 64 * 3 - 48;
			
			nextLotti -= time_step;
			
			setPpSwirl(0.5, 0, 0.5, 0.5, 0.1 + sin(total_ticks * 3) * 0.1);
			camera.roll = sin(total_ticks) * 5;

			wait(1);	
		}
	
		creditsNamePan.flags &= ~SHOW;
		creditsWorkPan1.flags &= ~SHOW;
		creditsWorkPan2.flags &= ~SHOW;
		creditsWorkPan3.flags &= ~SHOW;
	}
}


/**
 * Spotlights moved by randomness.                            /¯¯/
 * "Tonight the Super Trouper beams are gonna blind me ..." o/ o/
 *
 * Skill1-3 are min Values of destination angle, Skill4-6 are maximum
 * skill7 is Pointer to parent object to keep pan
 */
 void credits_spotlight(void)
 {
	ANGLE currAngle;
	ANGLE targetAngle;
	ENTITY* myParent = NULL;
	var lerpfactor = 0;
	while (1)
	{
		if (vec_length(vec_diff(NULL, currAngle, targetAngle)) == 0)
		{
			vec_set(targetAngle,
					_vec( random(my.skill4 - my.skill1) + my.skill1,
						  random(my.skill5 - my.skill2) + my.skill2,
						  random(my.skill6 - my.skill3) + my.skill3)
					);
			lerpfactor = 0;
		}
		else
		{
			lerpfactor += minv(time_step*0.01, 1);
			vec_lerp(currAngle, currAngle, targetAngle, lerpfactor);
		}
		
		if (my.skill7)
		{
			myParent = my.skill7;
			vec_set(my.x, myParent.x);
			vec_set(my.pan, myParent.pan);
		}
		else
		{
			vec_set(my.pan, nullvector);
		}
		
		vec_add(my.pan, currAngle);
		wait(1);
	}
 }
 
 /**
  * See the Flash when looking into a spotlight
  * skill1-3 is the offset to the parent object
  * Skill7 is the Pointer to the parent object
  */
 void credits_flash(void)
 {
	VECTOR offsetPos;
	
	VECTOR camDir;
	VECTOR parentDir;
	
	var diffVar = 0;
	
	ENTITY* myParent = NULL;
	var scaling = 0;
	while (1)
	{
		if (my.skill7)
		{
			myParent = my.skill7;
			offsetPos.x = my.skill1;
			offsetPos.y = my.skill2;
			offsetPos.z = my.skill3;
			vec_rotate(offsetPos, myParent.pan);
			vec_set(my.x, myParent.x);
			vec_add(my.x, offsetPos);
			
			// Weil der Scheinwerfer in Grundstellung nicht nach X sonder nach Z
			// zeigt, muss die Achse gedreht werden.
			vec_for_angle(parentDir, _vec(myParent.pan, myParent.tilt+90, myParent.roll));
			vec_for_angle(camDir, camera.pan);
			
			diffVar = acos(vec_dot(parentDir,camDir)/(vec_length(parentDir)*vec_length(camDir))); 
			
			scaling = maxv(diffVar-90, 0)/90.0;
			
			vec_fill(my.scale_x, scaling);
			
		}
		wait(1);
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
	_credits_sky = ent_createlayer("cosmo_f02+6.tga", SKY | CUBE, 10);
	
	ENTITY *stage = ent_create("stage_buehne.mdl", vector(0, 500, 20), NULL);
	stage.pan = 180;
	stage.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_dach.mdl", stage.x, NULL);
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_drums.mdl", stage.x, NULL);
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_vorhang.mdl", stage.x, NULL);
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_lichter.mdl", stage.x, NULL);
	you.pan = 180;
	you.material = _credits_mtlAlphaAdd;
	
	you = ent_create("stage_stangen.mdl", stage.x, NULL);
	you.pan = 180;
	you.material = _credits_mtlMetal;
	
	you = ent_create("lotterGitarre.mdl", stage.x, credits_lottiPlay);
	vec_add(you.x, vector(-100, 40, 90));
	you.pan = 300;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_mikro.mdl", stage.x, NULL);
	vec_add(you.x, vector(0, 0, -1));
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_mikro2.mdl", stage.x, NULL);
	vec_add(you.x, vector(0, 0, -1));
	you.pan = 180;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("lotterBass.mdl", stage.x, credits_lottiPlay);
	vec_add(you.x, vector(100, 40, 90));
	you.pan = 230;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("lotterDrums.mdl", stage.x, credits_lottiPlay);
	vec_add(you.x, vector(0, 76, 112));
	you.pan = 270;
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_fluter_basis.mdl", stage.x, NULL);
	vec_add(you.x, vector(150, -40, 275));
	you.material = _credits_mtlAlphaTest;
	
	you = ent_create("stage_fluter_basis.mdl", stage.x, NULL);
	vec_add(you.x, vector(-150, -40, 275));
	you.material = _credits_mtlAlphaTest;
	
	ENTITY* gelenkr = ent_create("stage_fluter_gelenk.mdl", stage.x, credits_spotlight);
	vec_add(gelenkr.x, vector(150, -40, 275));
	gelenkr.material = _credits_mtlAlphaTest;
	gelenkr.skill1 = -150;
	gelenkr.skill4 = 30;
	
	ENTITY* gelenkl = ent_create("stage_fluter_gelenk.mdl", stage.x, credits_spotlight);
	vec_add(gelenkl.x, vector(-150, -40, 275));
	gelenkl.material = _credits_mtlAlphaTest;
	gelenkl.skill1 = -30;
	gelenkl.skill4 = -210;
	
	ENTITY* werferr = ent_create("stage_fluter_scheinwerfer.mdl", stage.x, credits_spotlight);
	werferr.material = _credits_mtlAlphaTest;
	werferr.skill2 = -110;
	werferr.skill5 = 0;
	werferr.skill7 = gelenkr;
	
	ENTITY* werferl = ent_create("stage_fluter_scheinwerfer.mdl", stage.x, credits_spotlight);
	werferl.material = _credits_mtlAlphaTest;
	werferl.skill2 = -110;
	werferl.skill5 = 0;
	werferl.skill7 = gelenkl;
	
	you = ent_create("stage_fluter_licht.mdl", stage.x, credits_spotlight);
	you.material = _credits_mtlAlphaAdd;
	you.skill7 = werferr;
	
	you = ent_create("stage_fluter_licht.mdl", stage.x, credits_spotlight);
	you.material = _credits_mtlAlphaAdd;
	you.skill7 = werferl;
	
	you = ent_create("flash3.tga", stage.x, credits_flash);
	you.material = _credits_mtlAlphaAdd;
	you.skill3 = 40;
	you.skill7 = werferr;
	
	you = ent_create("flash3.tga", stage.x, credits_flash);
	you.material = _credits_mtlAlphaAdd;
	you.skill3 = 40;
	you.skill7 = werferl;
	
	_credits_music = media_play("media\\lotti_credits.wav", NULL, 100);
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
	media_stop(_credits_music);
	_credits_music = 0;
	if(credits.onCreditsEnd != NULL) {
		void tmp();
		tmp = credits.onCreditsEnd;
		tmp();
	}
}

#endif // #ifndef _CREDITS_C_
