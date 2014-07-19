#ifndef _CREDITS_C_
#define _CREDITS_C_

#include <acknex.h>
#include <strio.c>
#include <mtlView.c>
#include "credits.h"
#include "ini.h"
#include "menu.h"

var creditsFinished = 0;

SOUND *creditsExplo = "explo.wav";

BMAP *creditsTex = "terrainTexture.tga";
BMAP *creditsDetail = "terrainDetail.tga";

FONT *creditsFontHead = "Arial#26";
FONT *creditsFontBody = "Arial#20";

STRING *creditsText1Head = "#256";
STRING *creditsText1Body = "#256";
STRING *creditsText2Head = "#256";
STRING *creditsText2Body = "#256";

TEXT *creditsHead1 = { string = creditsText1Head; font = creditsFontHead; pos_y = 100; }
TEXT *creditsBody1 = { string = creditsText1Body; font = creditsFontBody; pos_y = 150; }
TEXT *creditsHead2 = { string = creditsText2Head; font = creditsFontHead; flags = ARIGHT; pos_y = 0; }
TEXT *creditsBody2 = { string = creditsText2Body; font = creditsFontBody; flags = ARIGHT; pos_y = 0; }

MATERIAL *creditsBlinker = 
{
	effect = "
	float4 vecTime;
	float4 ps(float2 pos : TEXCOORD0) : COLOR0
	{
		float4 color = 0;
		
		float3 time = vecTime.w * float3(1.2, -0.9, 1.0);
		
		color.rgb = 
			0.5 + 
			0.5 * sin(time + float3(0, 1, 2)) * 
			cos(time + float3(3, 2, 1));
		color.a = 1;
		
		color.rgb = pow(0.5 + color.rgb, 2) - 0.5;
		
		return color;
	}
	
	technique
	{
		pass
		{
			PixelShader = compile ps_2_0 ps();
		}
	}";
}

int currentRow = 0;

void credits_fire_event(PARTICLE *p)
{
	p->alpha -= 5 * time_step;
	if(p->alpha <= 0)
		p->lifespan = 0;
}

void credits_fire(PARTICLE *p)
{
	p->z -= random(p->vel_z);
	p->vel_x = random(1) - 0.5;
	p->vel_y = random(1) - 0.5;
	p->vel_z = random(1) - 0.5;
	p->size = 4 + random(2);
	p->red = 128;
	p->green = 64;
	p->blue = 0;
	p->flags = BRIGHT | MOVE;
	p->event = credits_fire_event;
}

action explo_sprite()
{
	my.flags |= BRIGHT | TRANSLUCENT;
	my.alpha = 100;
	while(my.frame < 13)
	{
		my.frame += 0.7 * time_step;
		wait(1);
	}
	ent_remove(me);
}

action credits_lotti()
{
	var init = random(100);
	while(my.alpha < 100)
	{
		my.z = my.alpha - 100;
		my.pan = 5.4 * my.alpha;
		wait(1);
	}
	
	int exploder = integer(random(15)) == 0;
	
	var time = 64 + random(128);
	while(my.x > currentRow)
	{
		ent_animate(me, "idle", 5 * total_ticks + init, ANM_CYCLE);
		time -= time_step;
		if(time < 0 && exploder)
			break;
		wait(1);
	}
	time = 10 + random(12);
	VECTOR accel, speed;
	if(exploder)
	{
		accel.x = 2.5 * (random(1) - 0.5);
		accel.y = 2.5 * (random(1) - 0.5);
	}
	else
	{
		accel.x = 0.25 * (random(1) - 0.5);
		accel.y = 0.25 * (random(1) - 0.5);
	}
	accel.z = 1 + random(2);
	
	vec_to_angle(my.pan, accel);
	my.tilt -= 90;
	
	vec_zero(speed);
	while(time > 0)
	{
		vec_add(speed, vector(accel.x * time_step, accel.y * time_step, accel.z * time_step));
		vec_add(my.x, vector(speed.x * time_step, speed.y * time_step, speed.z * time_step));
		
		VECTOR feet;
		feet.x = 0;
		feet.y = 0;
		feet.z = 0.5 * my.min_z;
		vec_rotate(feet, my.pan);
		vec_add(feet, my.x);
		
		effect(credits_fire, 10, feet, speed);
		
		time -= time_step;
		
		wait(1);
	}
	ent_create("explo+13.tga", my.x, explo_sprite);
	var s = snd_play(creditsExplo, 100, 0);
	snd_tune(s, 100, 80 + random(40), 0);
	ent_remove(me);
}

void credits_populate()
{
	int row = 0;
	int column = 0;
	var cambase = 64;
	var camspeed = 32;
	var fadespeed = 15;
	while(!creditsFinished)
	{
		VECTOR pos;
		pos.x = 256 + 128 * row;
		pos.y = 128 * column - 64 * row;
		pos.z = 8;
		
		camera.tilt = -15;
		
		you = ent_create("lotter_dummy.mdl", pos, credits_lotti);
		you.pan = 170 + random(20);
		you.flags |= TRANSLUCENT;
		
		for(you.alpha = 0; you.alpha < 100; you.alpha += fadespeed * time_step)
		{
			camera.z = cambase + camspeed * (row) + camspeed * ((var)(column + 0.005 * you.alpha) / (row+1));
			camera.x = -camspeed * (row) - camspeed * ((var)(column + 0.005 * you.alpha) / (row+1));
			wait(1);
		}
		you.alpha = 100;
		
		var i;
		for(i = 0; i < 100; i += fadespeed * time_step)
		{
			camera.z = cambase + camspeed * (row) + camspeed * ((var)(column + 0.5 + 0.005 * i) / (row+1));
			camera.x = -camspeed * (row) - camspeed * ((var)(column + 0.5 + 0.005 * i) / (row+1));
			wait(1);
		}
		
		column += 1;
		if(column > row)
		{
			column = 0;
			row += 1;
		}
	}
}

/**
 * Starts the credits.
 */
void credits_run(void)
{
	creditsFinished = 0;

	STRING *ini = "#256";
	str_cpy(ini, work_dir);
	str_cat(ini, "\\credits.ini");
	var song = media_play("music\\CloneFall.ogg", NULL, 100);
	//var song = media_play("demosong.ogg", NULL, 100);
	
	int pos = 0;
	int count = ini_read_int(ini, "Credits", "max", "0");
	var duration = ini_read_int(ini, "Credits", "duration", "15");
	
	var itemDuration = duration / (var)count;
	
	creditsHead1.flags |= SHOW;
	creditsBody1.flags |= SHOW;
	creditsHead2.flags |= SHOW;
	creditsBody2.flags |= SHOW;
	
	creditsHead2.pos_y = screen_size.y - 150;
	creditsBody2.pos_y = screen_size.y - 100;
		
	int size1 = 300;
	int size2 = 600;
	
	bmap_to_mipmap(creditsTex);
	bmap_to_mipmap(creditsDetail);
	
	d3d_fogcolor4.red = 123;
	d3d_fogcolor4.green = 174;
	d3d_fogcolor4.blue = 163;
	camera.fog_start = 4096;
	camera.fog_end = 8192;
	camera.arc = 60;
	fog_color = 4;
	detail_size = -64;
	level_load(NULL);
	ENTITY *terrain = ent_create("credits.hmp", vector(0, 0, -64), NULL);
	ent_setskin(terrain, creditsTex, 1);
	ent_setskin(terrain, creditsDetail, 2);
	terrain.material = mtl_terrain;
	
	you = ent_create("acknex.mdl", nullvector, NULL);
	vec_for_vertex(you.x, terrain, 493);
	you.flags |= NOFOG;
	you.material = creditsBlinker;
	ENTITY* mysky = ent_createlayer("Sky_1+6.tga", SKY | CUBE, 2);
	
	credits_populate();
	sun_light = 100;
	vec_fill(sun_color, 255);
	
	var x = 0;
	var len = 0;
	do
	{
		if(media_playing(song) == 0 || key_q)
			break;
		if(pos >= count)
		{
			wait(1);
			continue;
		}
	
		STRING *section = "#256";
		str_cpy(section, "Credit");
		str_cat(section, str_for_int(NULL, pos + 1));

		ini_read(creditsText1Head, ini, section, "Head1", "MISSING!");
		ini_read(creditsText1Body, ini, section, "Body1", "MISSING!");
		ini_read(creditsText2Head, ini, section, "Head2", "MISSING!");
		ini_read(creditsText2Body, ini, section, "Body2", "MISSING!");
		
		str_replaceall(creditsText1Body, "##", "\n");
		str_replaceall(creditsText2Body, "##", "\n");
		
		creditsHead1.pos_x = -size1;
		creditsBody1.pos_x = -size1;
		
		creditsHead2.pos_x = screen_size.x + size2;
		creditsBody2.pos_x = screen_size.x + size2;
		
		var fadetime = 0.22 * itemDuration;
		
		var fade = 0;
		var fadespeed = 16 / fadetime;
		for(fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead1.pos_x = -size1 + 1.5 * size1 * (1 - pow(1 - 0.01 * fade, 2));
			creditsBody1.pos_x = -size1 + 1.7 * size1 * (1 - pow(1 - 0.01 * fade, 2));
			if(key_q)
				break;
			wait(1);
		}
		for(fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead2.pos_x = screen_size.x + size2 - 1.2 * size2 * (1 - pow(1 - 0.01 * fade, 2));
			creditsBody2.pos_x = screen_size.x + size2 - 1.3 * size2 * (1 - pow(1 - 0.01 * fade, 2));
			if(key_q)
				break;
			wait(1);
		}
		if(key_q)
				break;
		wait(-0.6 * itemDuration);
		
		var scrollspeed = 120;
		
		for(fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead1.pos_x += scrollspeed * time_step;
			if(fade > 20)
				creditsBody1.pos_x += scrollspeed * time_step;
			if(key_q)
				break;
			wait(1);
		}
		for(fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead2.pos_x -= scrollspeed * time_step;
			if(fade > 20)
				creditsBody2.pos_x -= scrollspeed * time_step;
			if(key_q)
				break;
			wait(1);
		}
		
		pos += 1;
	} while(media_playing(song) && !key_q);
	media_stop(song);
	creditsHead1.flags &= ~SHOW;
	creditsBody1.flags &= ~SHOW;
	creditsHead2.flags &= ~SHOW;
	creditsBody2.flags &= ~SHOW;
	
	creditsFinished = 1;
	
	currentRow = 25000;
	wait(-5.0);

	TEXT *txt = { string = "Press and hold [SPACE] to quit to main menu."; flags = CENTER_X; font = "Arial#40b"; }
	txt.flags |= SHOW;
	txt.pos_x = 0.5 * screen_size.x;
	txt.pos_y = screen_size.y - 60;
	
	while(1)
	{
		if(key_space)
		{
			menu_open();
			txt.flags &= ~SHOW;
			ptr_remove(mysky);
			ptr_remove(terrain);
			return;
		}
		wait(1);
	}
}

#endif // #ifndef _CREDITS_C_