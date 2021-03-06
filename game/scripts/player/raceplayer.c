#ifndef raceplayer_c
	#define raceplayer_c

	#include "engine.h"
	#include "raceplayer.h"
	#include "items.h"
	#include "camera.h"
	#include "taunts.h"
	#include "playrace.h"
	
	BMAP* drift_decal = "decal_skidmark.tga";

	void p_drift_smoke_fade(PARTICLE* p)
	{
		vec_scale(p.vel_x,1-0.15*time_step);
		p.size += (p.skill_a-p.size)*0.35*time_step;
		p.alpha -= clamp(p.alpha*0.5,0.5,3)*time_step;
		if(p.alpha <= 0) p.lifespan = 0;
	}

	void p_drift_smoke(PARTICLE* p)
	{
		vec_add(p.vel_x,vector(1-random(2),1-random(2),1-random(1)));
		vec_fill(p.blue,90+random(20));
		set(p,MOVE);
		p.size = (9+random(5))*0.5;
		p.alpha = 30+random(10);
		p.skill_a = p.size*5;
		p.bmap = bmp_smoke_spr1;
		p.event = p_drift_smoke_fade;
	}

	void p_kart_grass(PARTICLE* p)
	{
		ENTITY* ent = your->parent;
		vec_set(p.blue,p.vel_x);
		vec_set(p.vel_x,vector(-4,0,4));
		vec_rotate(p.vel_x,ent->pan);
		vec_add(p.vel_x,vector(1-random(2)-p.vel_x*0.65,1-random(2),7+random(4)));
		vec_scale(p.blue,0.85+random(0.15));
		set(p,MOVE);
		p.bmap = bmp_quad;
		p.gravity = 3;
		p.lifespan = 24;
		p.size = 2+random(2);
		p.event = NULL;
	}

	void p_blumenkasten_fade(PARTICLE* p)
	{
		p.size = p.skill_a*minv(p.lifespan/3.0,1);
	}

	void p_blumenkasten1(PARTICLE* p)
	{
		vec_add(p.x,vector((1-random(2))*8,(1-random(2))*8,random(8)));
		p.skill_a = 90+random(30);
		vec_set(p.blue,vector(p.skill_a,160,p.skill_a+15));
		vec_set(p.vel_x,vector((1-random(2))*7,(1-random(2))*7,9+random(15)));
		vec_scale(p.blue,0.65+random(0.25));
		set(p,MOVE);
		p.bmap = bmp_quad;
		p.gravity = 3;
		p.lifespan = 12;
		p.skill_a = p.size = 2+random(2);
		p.event = p_blumenkasten_fade;
	}

	void p_blumenkasten2(PARTICLE* p)
	{
		vec_add(p.x,vector((1-random(2))*8,(1-random(2))*8,random(8)));
		vec_set(p.blue,vector(10,44,100));
		vec_lerp(p.blue,p.blue,COLOR_WHITE,random(0.1));
		vec_scale(p.blue,0.35+random(0.15));
		vec_set(p.vel_x,vector((1-random(2))*7,(1-random(2))*7,9+random(15)));
		set(p,MOVE);
		p.bmap = bmp_quad;
		p.gravity = 3;
		p.lifespan = 12;
		p.skill_a = p.size = 2+random(2);
		p.event = p_blumenkasten_fade;
	}
	
	void p_grave(PARTICLE* p)
	{
		vec_add(p.x,vector((1-random(2))*8,(1-random(2))*8,random(8)));
		vec_set(p.blue,vector(20,74,100));
		vec_lerp(p.blue,p.blue,COLOR_WHITE,random(0.1));
		vec_scale(p.blue,0.35+random(0.15));
		vec_set(p.vel_x,vector((1-random(2))*7,(1-random(2))*7,9+random(15)));
		set(p,MOVE);
		p.bmap = bmp_quad;
		p.gravity = 3;
		p.lifespan = 12;
		p.skill_a = p.size = 2+random(2);
		p.event = p_blumenkasten_fade;
	}

	action ac_racetrack()
	{
		c_setminmax(my);
		set(my,POLYGON);
	}
	
	void blumenkasten_event()
	{
		if(your._type == type_kart)
		{
			trap_driver(you,2);
			set(my,PASSABLE | INVISIBLE);
			effect(p_blumenkasten1,32,my.x,nullvector);
			effect(p_blumenkasten2,32,my.x,nullvector);
		}
	}
	
	action ac_blumenkasten()
	{
		c_setminmax(my);
		set(my,POLYGON);
		my.event = blumenkasten_event;
		my.emask |= ENABLE_IMPACT;
	}

	var get_max_laps()
	{
		var laps = 5;
		
		g_raceTrackWidth = 160; // hackfix
		if(str_cmpni("droad.wmb",level_name))
		{
			g_raceTrackWidth = 90;
			laps = 4;
		}
		if(str_cmpni("kbridge.wmb",level_name)) g_raceTrackWidth = 90;
		if(str_cmpni("icenex.wmb",level_name)) g_raceTrackWidth = 95;
		
		return laps;
	}
	
	double dvec_dot(VECTOR* vec1, VECTOR* vec2)
	{
		double d;
		
		d = (double)vec1->x*(double)vec2->x+(double)vec1->y*(double)vec2->y+(double)vec1->z*(double)vec2->z;
		
		return d;
	}

	var get_line_segment_point_dist(VECTOR* vec1, VECTOR* vec2, VECTOR* point, VECTOR* vresult, var* t)
	{
		var res,dist1,dist2,i;
		VECTOR dir1,dir2;

		vec_diff(&dir1,vec2,vec1);
		vec_diff(&dir2,point,vec1);
		i = dvec_dot(&dir2,&dir1)/dvec_dot(&dir1,&dir1);

		if(i >= 0 && i <= 1)
		{
			vec_lerp(&dir1,vec1,vec2,i);

			res = vec_dist(&dir1,point);
			if(vresult) vec_set(vresult,&dir1);
		}
		else
		{
			dist1 = vec_dist(vec1,point);
			dist2 = vec_dist(vec2,point);
			if(dist1 < dist2)
			{
				res = dist1;
				if(vresult) vec_set(vresult,vec1);
			}
			else
			{
				res = dist2;
				if(vresult) vec_set(vresult,vec2);
			}
		}
		
		if(t) *t = i;

		return res;
	}

	void path_get_closest_position(VECTOR* vpos, VECTOR* vresult, VECTOR* vdata)
	{
		var dist,max_dist = 99999,t;
		int i, j, max_nodes;
		VECTOR temp,temp2,temp3;
		ENTITY* ent;

		ent = ent_create(NULL,nullvector,NULL);
		max_nodes = path_next(ent);
		for(i = 1; i <= max_nodes; i++)
		{
			path_getnode(ent,i,temp,NULL);
			if(i == max_nodes) j = 1;
			else j = i+1;
			path_getnode(ent,j,temp2,NULL);
			dist = get_line_segment_point_dist(temp,temp2,vpos,temp3,t);
			if(dist > 0 && dist < max_dist)
			{
				max_dist = dist;
				vec_set(vresult,temp3);
				if(vdata) vec_set(vdata,vector(i,j,t));
			}
		}
		
		ptr_remove(ent);
	}

	void path_get_offset_position(VECTOR* vdata, var offset, VECTOR* vresult)
	{
		var length,t;
		int k, max_nodes;
		VECTOR temp,temp2;
		ENTITY* ent;
		
		ent = ent_create(NULL,nullvector,NULL);
		max_nodes = path_next(ent);
		path_getnode(ent,vdata.x,temp,NULL);
		path_getnode(ent,vdata.y,temp2,NULL);
		length = vec_dist(temp,temp2);
		if(length < 0.1) return;
		t = offset/length;
		if(offset >= 0) k = vdata.y+floor(t+vdata.z);
		else k = vdata.x+floor(t+vdata.z);
		if(k <= 0) k = max_nodes;
		if(k > max_nodes) k = 1;
		if(offset > 0 && k != vdata.y)
		{
			path_getnode(ent,k,temp,NULL);
			offset = offset-(length-vdata.z*length);
			length = vec_dist(temp,temp2);
			if(length < 0.1)
			{
				ptr_remove(ent);
				return;
			}
			t = offset/length;
			vec_lerp(vresult,temp2,temp,t);
		}
		else
		{
			if(offset < 0 && k != vdata.x)
			{
				path_getnode(ent,k,temp2,NULL);
				offset = offset+vdata.z*length;
				length = vec_dist(temp,temp2);
				if(length < 0.1)
				{
					ptr_remove(ent);
					return;
				}
				t = 1+offset/length;
				vec_lerp(vresult,temp,temp2,t);
			}
			else
			{
				vec_lerp(vresult,temp,temp2,vdata.z+t);
			}		
		}
		
		ptr_remove(ent);
	}


	var get_xyangle(VECTOR* vec)
	{
		var angle = 0, length;

		length = vec_length(vector(vec->x,vec->y,0));
		if(abs(length) < 0.015) return -1;
		angle = acosv(vec->x/length);
		if(vec->y < 0) angle *= -1;

		return angle;
	}

	void path_get_normal_position(VECTOR* vpos, var path_offset, var offset, VECTOR* vresult)
	{
		var ang1,ang2;
		VECTOR temp,temp2,temp3,vnormal;
		
		path_get_closest_position(vpos,temp,temp2);
		if(path_offset) path_get_offset_position(temp2,path_offset,temp);
		path_get_offset_position(temp2,32,temp3);
		vec_diff(temp2,temp3,temp);
		vec_set(vnormal,vector(-temp2.y,temp2.x,0));
		vec_normalize(vnormal,g_raceTrackWidth*abs(offset));
		ang1 = get_xyangle(temp2)+360;
		ang2 = get_xyangle(vnormal)+360;
		if(ang1 > 360 && ang1 > ang2) ang1 -= 360; // dirty fix
		if((ang1 > ang2 && offset < 0) || (ang1 < ang2 && offset > 0)) vec_inverse(vnormal);
		vec_set(vresult,temp);
		vec_add(vresult,vnormal);
	}
	
	var ent_path_get_progress(ENTITY* kart, var* length)
	{
		var progress = 0,dist,max_dist = 99999,t;
		int i, j, max_nodes;
		VECTOR temp,temp2,temp3;
		ENTITY* ent;

		ent = ent_create(NULL,nullvector,NULL);
		max_nodes = path_next(ent);
		*length = 0;
		for(i = 1; i < max_nodes; i++)
		{
			path_getnode(ent,i,temp,NULL);
			path_getnode(ent,i+1,temp2,NULL);
			ent->skill[i+1] = vec_dist(temp2,temp);
			*length += ent->skill[i+1];
		}
		for(i = 1; i < max_nodes; i++) ent->skill[i+1] = ent->skill[i+1]+ent->skill[i];

		max_nodes = path_next(ent);
		for(i = 1; i <= max_nodes; i++)
		{
			path_getnode(ent,i,temp,NULL);
			if(i == max_nodes) j = 1;
			else j = i+1;
			path_getnode(ent,j,temp2,NULL);
			dist = get_line_segment_point_dist(temp,temp2,kart.x,temp3,t);
			if(dist > 0 && dist < max_dist)
			{
				max_dist = dist;
				progress = ent->skill[i]+vec_dist(temp3,temp);
			}
		}
		
		ptr_remove(ent);
		
		return progress;
	}


	var is_kart_turning(ENTITY* ent)
	{
		var left, right;
		left = !!(ent->kart_input & INPUT_LEFT);
		right = !!(ent->kart_input & INPUT_RIGHT);
		return left-right;
	}

	var is_kart_accelerating(ENTITY* ent)
	{
		var up, down;
		up = !!(ent->kart_input & INPUT_UP);
		down = !!(ent->kart_input & INPUT_DOWN);
		return up-down;
	}

	var get_kart_accel(ENTITY* ent)
	{
		return (ent->speed-ent->old_speed)/time_step;
	}

	var get_kart_speed(ENTITY* ent, VECTOR* vdir)
	{
		if(vdir) vec_set(vdir,ent->speed_x);

		return ent->speed;
	}

	var get_kart_lap_player()
	{
		ENTITY* ent = get_kart_driver(0);
		if(!ent) return -1;
		return ent->kart_lap;
	}
	var get_kart_rank_player()
	{
		int i;
		var rank;
		ENTITY* ent;
		
		ent = get_kart_driver(0);
		if(!ent) return -1;
		rank = 1;
		for(i = 1; i < 4; i++)
		{
			ENTITY* ent_enemy = get_kart_driver(i);
			if(ent_enemy->kart_progress > ent->kart_progress || (!ent->has_finished && ent_enemy->has_finished) || (ent->has_finished > ent_enemy->has_finished && ent_enemy->has_finished)) rank++;
		}
		ent->kart_rank = rank;
		return rank;
	}

	var is_kart_player_wrong_way()
	{
		ENTITY* ent = get_kart_driver(0);
		if(!ent) return -1;
		return (ent->kart_checkpoint < 0);
	}

	void kart_event()
	{
		var new_angle, factor;
		VECTOR temp;

		if(event_type == EVENT_TRIGGER || abs(my->speed) < 10) return;
		
		factor = 0.3+clamp(my->speed/my->kart_maxspeed,0,0.7);

		if (you != null) {
			if (your._type == type_kart) {
				if((my->kart_big && !your->kart_big) || (!my->kart_small && your->kart_small))
				{
					trap_driver(you,1.5);
					return;
				}
				else
				{
					if((!my->kart_big && your->kart_big) || (my->kart_small && !your->kart_small)) trap_driver(me,1.5);
				}
				factor *= 0.5;
			}
		}

		vec_diff(temp, my->x, target);

		my->bounce_x = temp.x * factor;
		my->bounce_y = temp.y * factor;

		my->speed *= 0.25;

		new_angle = get_xyangle(bounce);

		if (abs(ang(new_angle - my->drive_pan)) < 90) {
			my->bump_ang = (ang(new_angle - my->drive_pan))*factor;
		}

		VECTOR vs;
		vec_set(&vs, my->x);
		VECTOR* vscreen = vec_to_screen(&vs, cam);
		bool inScreen = vscreen != null;

		if (inScreen) {
			var h = snd_play(g_sndKartHit, g_kartsnd_hit, 0);
			snd_tune (h, g_kartsnd_hit, 90+random(20), 0);
		}

	}

	void reduce_coins(ENTITY* ent)
	{
		if(ent->PL_A4_COUNT > 0)
		{
			ent->PL_A4_COUNT = maxv(ent->PL_A4_COUNT-2,0);
			effect(_item_particle2, 25, &my->x, nullvector);
		}
	}
	
	void trap_driver(ENTITY* ent, var blackOutTime)
	{
		ent->kart_trapped = 16*blackOutTime;
		ent->kart_hit = 0;
		ent->kart_turbo = 0;
		reduce_coins(ent);
	}

	void driver_hit(ENTITY* ent, var blackOutTime)
	{
		ent->kart_hit = 16*blackOutTime;
		ent->kart_turbo = 0;
	}

	void start_turbo(ENTITY* ent, var time)
	{
		ent->kart_turbo = 16*time;
	}

	void enlarge_driver(ENTITY* ent, var time)
	{
		ent->kart_big = 16*time;
		ent->kart_small = 0;	
	}

	void minimize_driver(ENTITY* ent, var time)
	{
		ent->kart_small = 16*time;
		ent->kart_big = 0;
		reduce_coins(ent);
	}

	void postConstructPlayer(ENTITY* ent)
	{
		set(ent, INVISIBLE);
		c_setminmax(ent);

		ent->event = kart_event;
		ent->emask |= ENABLE_BLOCK | ENABLE_ENTITY | ENABLE_ENTITY | ENABLE_TRIGGER;
		ent->trigger_range = 32;

		VECTOR vecMin;
		vec_for_min(&vecMin, ent);
		ent->kart_height = -vecMin.z;
		ent->group = group_kart;
		ent->kart_maxspeed2 = g_raceplayerMaxSpeed*(0.95+random(0.1));
		ent->bot_path_offset = random(2)-1;
		ent->_type = type_kart;
		ent->has_finished = 0;
		ent->fire_item_max = 7+random(17);
		ent->kart_progress_update = random(2);
		ent->kart_bot_separate = random(4);

		ent->parent = ent_create(str_for_entfile(NULL, ent), ent->x, NULL);
		set(ent->parent, PASSABLE);
		ent->parent->material = mat_kart1;
		ent->parent->scale = 1;
		ent->parent->skill41 = floatv(ent->parent->scale);
	}

	void loadPlayerHumanControlParams(ENTITY* ent)
	{
		key_up = !g_doNotDrive * key_cuu;
		key_down = !g_doNotDrive * key_cud;
		key_left = key_cul;
		key_right = key_cur;
		key_hop = key_space;
		key_item = key_ctrl;

		if (!(key_item+key_hop+key_right+key_left+key_down+key_up))
		{
			key_up = !g_doNotDrive * ((joy_rot.y > 128) | (joy_raw.z < -128));
			key_down = !g_doNotDrive * ((joy_rot.z > 128) | joy_3);
			key_left = (joy_force.x < -1);
			key_right = (joy_force.x > 1);
			key_hop = joy_6 | joy_5;
			key_item = joy_2 | joy_1;
		}

		ent->kart_input = key_up * INPUT_UP | key_down * INPUT_DOWN | key_left * INPUT_LEFT | key_right * INPUT_RIGHT | key_hop * INPUT_HOP
		| key_item * INPUT_ITEM;
	}

	void loadPlayerCpuControlParams(ENTITY* ent)
	{
		var angle,angle_diff;
		VECTOR temp,temp2;
		
		ent->kart_input = INPUT_UP*!g_doNotDrive;
		path_get_normal_position(ent->x,256,sinv(total_ticks*4+ent->bot_path_offset),temp);
		vec_diff(temp2,temp,ent->x);
		angle = get_xyangle(temp2);
		angle_diff = ang(angle-ent->pan);
		if(abs(angle_diff) > 0)
		{
			if(angle_diff > 0) ent->kart_input |= INPUT_LEFT;
			if(angle_diff < 0) ent->kart_input |= INPUT_RIGHT;
		}
	}
	
	void resetPlayerOnPath(ENTITY* ent)
	{
		VECTOR temp,temp2;
		path_get_closest_position(vector(ent->x,ent->y,0),temp,temp2);
		vec_set(ent->x,temp);
		path_get_offset_position(temp2,64,temp);
		vec_diff(temp2,temp,ent->x);
		vec_to_angle(temp,temp2);
		ent->pan = ent->drive_pan = temp.x;
		vec_set(ent->parent->x, ent->x);
		ent->parent->z += ent->parent->skill1;
		vec_set(ent->parent->pan, ent->pan);
		vec_set(ent->speed_x,nullvector);
		vec_set(ent->bounce_x,nullvector);
		ent->speed = 0;
		ent->drifting = 0;		
	}

	void updatePlayer(ENTITY* ent)
	{
		int i;
		var up, down, left, right, hop, item, underground, old_contact, turn, progress, length, d;
		VECTOR temp,temp2;
		
		ent->kart_maxspeed = ent->kart_maxspeed2*(1+ent.PL_A4_COUNT*0.01);
		if(ent->falling)
		{
			set(ent,PASSABLE);

			if (!ent->isfallingsndplayed) {
				snd_play(g_sndAiaiaiai, g_kartsnd_aiaiai, 0);
				ent->isfallingsndplayed = true;
				reduce_coins(ent);
			}
			if(you = ent->kart_turbo_ent) set(you,INVISIBLE);
			
			ent->speed_z = maxv(ent->speed_z-5*time_step,-90);
			up = maxv(0,40+ent->speed_z);
			if(up > ent->falling) ent->falling += (up-ent->falling)*0.2*time_step;
			else ent->falling += (up-ent->falling)*0.15*time_step;
			vec_set(temp, vector(ent->falling, 0, 0));
			vec_rotate(temp, vector(ent->drive_pan+180*(ent->falling_dir == 1)+90*(ent->falling_dir == 2)-90*(ent->falling_dir == 3), 0, 0));
			temp.z += ent->speed_z;
			vec_scale(temp, time_step);
			c_move(ent, nullvector, temp, IGNORE_MODELS | IGNORE_WORLD | IGNORE_SPRITES);
			ang_rotate(ent->parent->pan,vector(0,(-(ent->falling_dir == 0)+(ent->falling_dir == 1))*15*time_step,(-(ent->falling_dir == 2)+(ent->falling_dir == 3))*15*time_step));
			vec_set(ent->parent->x, ent->x);
			ent->parent->z -= ent->kart_height;
			
			if(ent->z < -512)
			{
				resetPlayerOnPath(ent);
				ent->falling = 0;
				ent->kart_trapped = 0;
				ent->kart_hit = 0;
				ent->kart_turbo = 0;
				ent->kart_big = 0;
				ent->kart_small = 0;
				ent->isfallingsndplayed = false;
			}

			synplayermodel(ent);

			return;
		}
		reset(ent,PASSABLE);
		ent.z = 16;

		if(ent->has_finished) ent->kart_input = 0;
		ent->old_speed = ent->speed;
		up = !!(ent->kart_input & INPUT_UP);
		down = !!(ent->kart_input & INPUT_DOWN);
		left = !!(ent->kart_input & INPUT_LEFT);
		right = !!(ent->kart_input & INPUT_RIGHT);
		hop = !!(ent->kart_input & INPUT_HOP);
		item = !!(ent->kart_input & INPUT_ITEM);
		
		if(up || down)
		{
			ent->stuck_counter += time_step;
			if(vec_dist(vector(ent->x,ent->y,0),vector(ent->stuck_x,ent->stuck_y,0)) > 8 || ent->kart_trapped)
			{
				ent->stuck_x = ent->x;
				ent->stuck_y = ent->y;
				ent->stuck_counter = 0;
			}
			if(ent->stuck_counter >= 32)
			{
				resetPlayerOnPath(ent);
				ent->stuck_counter = 0;
			}
		}

		if (!ent->drifting) {
			ent->kart_drift_buffer += (0-ent->kart_drift_buffer)*0.1*time_step;
			turn = g_raceplayerTurnSpeed * (left - right);
			} else {
			ent->kart_drift_buffer += (1-ent->kart_drift_buffer)*0.85*time_step;
			turn = ent->drift_pan * 0.3;
		}

		//ent->turn_speed += (turn * (ent->speed / ent->kart_maxspeed) - ent->turn_speed) * 0.55 * time_step;
		ent->turn_speed += (turn * pow((1.25*abs(ent->speed) / ent->kart_maxspeed),0.75)*sign(ent->speed) - ent->turn_speed) * 0.55 * time_step;
		ent->turn_speed2 += clamp((ent->turn_speed - ent->turn_speed2), -0.5, 0.5) * time_step;
		ent->drive_pan += (ent->bump_ang*0.35+ent->ground_contact*ent->turn_speed*2*!ent->kart_trapped)*time_step;
		ent->drive_pan = ang(ent->drive_pan);
		ent->bump_ang += -ent->bump_ang*0.35*time_step;

		if(ent->kart_hit)
		{
			ent->speed += clamp(-ent->speed * 0.5, -g_raceplayerAccelSpeed*3, g_raceplayerAccelSpeed*3) * time_step;
		}
		else
		{

			if (!(up || down) || ent->kart_trapped) {
				ent->speed += clamp(-ent->speed * 0.125, -g_raceplayerAccelSpeed*0.25, g_raceplayerAccelSpeed*0.25) * time_step;
			}
			else
			{
				if (up && !down) {
					ent->speed += minv((ent->kart_maxspeed*(1+0.4*!!ent->kart_turbo-0.3*!!ent->kart_small+0.2*!ent->kart_turbo*!!ent->kart_big)-ent->speed)*0.1,g_raceplayerAccelSpeed*(1+0.5*!!ent->kart_turbo)) * time_step;
				}

				if (!up && down) {
					ent->speed = maxv(ent->speed - (0.5+0.5*(ent->speed > 0))*g_raceplayerAccelSpeed*2 * time_step, -g_raceplayerBreakForce*!ent.drifting);
				}
			}
			ent->speed = minv(ent->speed,(ent->kart_maxspeed*(1+0.4*!!ent->kart_turbo-0.3*!!ent->kart_small) - (1-ent->kart_drift_buffer)*ent->kart_maxspeed*0.5*abs(ent->turn_speed2)/g_raceplayerTurnSpeed * !ent->drifting) * ent->underground);
		}

		ent->pan = ent->drive_pan;
		vec_set(temp, vector(ent->speed, 0, 0));
		vec_rotate(temp, vector(ent->drive_pan, 0, 0));
		vec_add(temp,vector(ent->bounce_x,ent->bounce_y,0));
		ent->speed_x = temp.x;
		ent->speed_y = temp.y;
		vec_scale(temp, time_step);

	c_ignore(group_rocket,0);
		c_move(ent, nullvector, temp, IGNORE_PASSABLE | GLIDE | USE_POLYGON | IGNORE_SPRITES | ACTIVATE_TRIGGER); //IGNORE_PUSH | 

		vec_scale(ent->bounce_x,1-0.4*time_step);

		old_contact = ent->ground_contact;
		ent->ground_contact = (ent->parent->z <= ent->kart_height*ent->parent->scale*1.05);

		if (!old_contact && ent->ground_contact && hop && ent->speed > 10 && (left - right) && !ent->drifting) {
			ent->drifting = 1;
			snd_play(g_sndDrift, g_sndDriftvol, 0);
		}

		if (!hop) {
			ent->drifting = 0;
		}

		c_ignore(group_kart, group_rocket, 0);
		c_trace(vector(ent->x, ent->y, ent->z + 64), vector(ent->x, ent->y, -16), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON | IGNORE_SPRITES);
		
		PARTICLE* p = ent_decal(you,bmp_shadow,70*ent->parent->scale,ent->parent->pan);
		set(p,TRANSLUCENT);
		p.alpha = 50;
		p.lifespan = 0.01;
		p.material = mat_model;
		if (ent->ground_contact) {

			if (tex_flag4 || hit.green > 100) {
				underground = g_raceplayerBadGroundFac;
				} else {
				underground = 1;
			}
		}
		else {
			underground = ent->underground;
		}

		if (ent->ground_contact) {

			if (!ent->kart_id && !key_hop) {
				key_hop_off = 1;
			}

			if (hop && (key_hop_off || ent->kart_id > 0)) {

				if (!ent->kart_id) {
					key_hop_off = 0;
				}

				snd_play(g_sndKartJump, g_kartsnd_jump, 0);

				ent->speed_z = 15;
				ent->ground_contact = 0;
			}
		}
		if(ent->kart_turbo) underground = 1;
		ent->underground += (underground - ent->underground) * 0.5 * time_step;

		if (!ent->drifting) {
			ent->drift_pan += -ent->drift_pan * 0.25 * time_step;
			ent->drift_dir = 0;
			} else {

			if (!ent->drift_dir && (left || right)) {
				ent->drift_dir = left - right;
			}

			ent->drift_pan += clamp((ent->drift_dir * (3 + 15 * abs(left - right) * (sign(left - right) == ent->drift_dir)) - ent->drift_pan)*0.4, -3.5, 3.5)
			* time_step;
		}
		vec_set(ent->parent->x, ent->x);
		vec_set(ent->parent->pan, ent->pan);
		ent->parent->pan = ent->pan + ent->drift_pan*3.25 + ent->kart_trapped/16.0*720;
		ent->parent->skill1 += ent->speed_z * time_step;
		ent->parent->skill1 = maxv(ent->parent->skill1, ent->kart_height);
		ent->parent->z = ent->parent->skill1*ent->parent->scale;
		ent->speed_z -= 9 * time_step;

		if(ent->speed < 15 || ent->underground < 0.9) ent->drifting = 0;
		if(ent->ground_contact && ent->speed >= ent->kart_maxspeed*0.2) {
			ent->particle_emit += time_step;
			if(ent->particle_emit > 0.5)
			{
				ent->particle_emit -= 0.5;
				vec_set(temp,vector(-22,22*ent->parent->scale,-ent->kart_height*0.5));
				vec_rotate(temp,ent->parent->pan);
				vec_add(temp,ent->parent->x);
				vec_set(temp2,vector(-4,0,4));
				vec_rotate(temp2,ent->parent->pan);
				if(ent->drifting && (ent->kart_trapped == 0))
				{
					effect(p_drift_smoke,1,temp,temp2);
					me = ent;
					if (c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | USE_POLYGON | SCAN_TEXTURE | IGNORE_SPRITES | IGNORE_ME)>0)// | IGNORE_MODELS) > 0)
					{
						PARTICLE* p = ent_decal (you, drift_decal, 30 * ent->parent->scale, ent->pan);
						p->lifespan = 30 * 16;
					}
				}
				else
				{
					c_ignore(group_kart, group_rocket, 0);
					c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON | IGNORE_SPRITES);
					if(hit.green > 100) effect(p_kart_grass,1,temp,hit.blue); //temp2);
				}
				vec_set(temp,vector(-22,-22*ent->parent->scale,-ent->kart_height*0.5));
				vec_rotate(temp,ent->parent->pan);
				vec_add(temp,ent->parent->x);
				if(ent->drifting && (ent->kart_trapped == 0))
				{ 
					effect(p_drift_smoke,1,temp,temp2);
					me = ent;
					if (c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | USE_POLYGON | SCAN_TEXTURE | IGNORE_SPRITES | IGNORE_ME)>0)// | IGNORE_MODELS) > 0)
					{
						PARTICLE* p = ent_decal (you, drift_decal, 30 * ent->parent->scale, ent->pan);
						p->lifespan = 30 * 16;
					}
				}
				else
				{
					c_ignore(group_kart, group_rocket, 0);
					c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON | IGNORE_SPRITES);
					if(hit.green > 100) effect(p_kart_grass,1,temp,hit.blue); //temp2
				}
			}
		}
		vec_set(temp,vector(20,0,0));
		vec_rotate(temp,ent->pan);
		vec_add(temp,ent->x);
					c_ignore(group_kart, group_rocket, 0);
		c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | USE_POLYGON | IGNORE_SPRITES);
		if(!trace_hit)
		{
			ent->falling = ent->speed;
			ent->falling_dir = 0;
			ent->speed_z = maxv(ent->speed_z,0);
		}
		if(!ent->falling)
		{
			vec_set(temp,vector(-20,0,0));
			vec_rotate(temp,ent->pan);
			vec_add(temp,ent->x);
					c_ignore(group_kart, group_rocket, 0);
			c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | USE_POLYGON | IGNORE_SPRITES);
			if(!trace_hit)
			{
				ent->falling = ent->speed;
				ent->falling_dir = 1;
				ent->speed_z = maxv(ent->speed_z,0);
			}
			if(!ent->falling)
			{
				vec_set(temp,vector(0,20,0));
				vec_rotate(temp,ent->pan);
				vec_add(temp,ent->x);
					c_ignore(group_kart, group_rocket, 0);
				c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | USE_POLYGON | IGNORE_SPRITES);
				if(!trace_hit)
				{
					ent->falling = ent->speed;
					ent->falling_dir = 2;
					ent->speed_z = maxv(ent->speed_z,0);
				}
				if(!ent->falling)
				{
					vec_set(temp,vector(0,-20,0));
					vec_rotate(temp,ent->pan);
					vec_add(temp,ent->x);
					c_ignore(group_kart, group_rocket, 0);
					c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | USE_POLYGON | IGNORE_SPRITES);
					if(!trace_hit)
					{
						ent->falling = ent->speed;
						ent->falling_dir = 3;
						ent->speed_z = maxv(ent->speed_z,0);
					}
				}
			}
		}
		
		// Item-Verwendung
		
		if(ent->item_id) { //item)
			if(ent->sk_kart_id > 1) ent->fire_item += time_step;
			if(item && ent->sk_kart_id == 1 && ent->fire_item != -1) ent->fire_item = 99999;
			if(ent->fire_item > ent->fire_item_max)
			{
				switch(ent->item_id) {
					case ITEM_GRAVE: plant_grave(ent); break;
					case ITEM_ROCKET: shoot_rocket(ent); break;
					case ITEM_AIM_ROCKET: shoot_aiming_rocket(ent); break;
					case ITEM_TURBO: use_turbo(ent); break;
					case ITEM_BADASS_ROCKET: shoot_badass_aiming_rocket(ent); break;
					case ITEM_MUSHROOM: start_mushroom(ent); break;
					case ITEM_FLASH: start_flash(ent); break;
				}
			}
		}
		else ent->fire_item = 0;
		
		ent->kart_progress_update += time_step;
		if(ent->kart_progress_update > 2 && !ent->has_finished)
		{
			ent->kart_progress_update -= 2;
			progress = ent_path_get_progress(ent,&length);
			if(progress < length*0.1)
			{
				if(!ent->kart_lap) ent->kart_lap = 1;
				if(ent->kart_checkpoint > 0)
				{
					if(ent->kart_lap >= get_max_laps())
					{
						ent->has_finished = total_ticks;
						if(ent->sk_kart_id == 1)
						{
							do_race_end(ent);
						}
					}
					else
					{
						playTaunt(ent->sk_kart_id-1);
						ent->kart_lap++;
						ent->kart_checkpoint = 0;
					}
				}
			}
			if(!ent->has_finished)
			{
				d = (ent->kart_lap-1)*length;
				if(progress+d > ent->kart_progress+length*0.25) progress = -999999;
				if(ent->kart_lap > 0 && progress > length*0.5 && progress+d > ent->kart_progress)
				{
					ent->kart_checkpoint = 1;
				}
				if(ent->kart_lap > 0)
				{
					if(progress+d > ent->kart_progress) ent->kart_progress = progress+d;
					else
					{
						if(progress+d < ent->kart_progress-512) ent->kart_checkpoint = -1;
					}
				}
			}
		}

		if(ent->sk_kart_id > 1)
		{
			ent->kart_bot_separate += time_step;
			if(ent->kart_bot_separate > 4)
			{
				ent->kart_bot_separate -= 4;
				for(i = 1; i <= 4; i++)
				{
					ENTITY* ent2 = get_kart_driver(i-1);
					vec_diff(temp,ent2->x,ent->x);
					temp.z = 0;
					length = vec_length(temp);
					if(length < 1) vec_randomize(temp,128);
					if(length < 32)
					{
						vec_normalize(temp,48);
						vec_add(ent->x,temp);
						vec_inverse(temp);
						if(i != 1) vec_add(ent2->x,temp);
					}
				}
			}
		}
		ent->kart_trapped = maxv(ent->kart_trapped-time_step,0);
		ent->kart_hit = maxv(ent->kart_hit-time_step,0);
		ent->kart_turbo = maxv(ent->kart_turbo-time_step,0);
		ent->kart_big = maxv(ent->kart_big-time_step,0);
		ent->kart_small = maxv(ent->kart_small-time_step,0);

		ent->parent->scale += (1.0+0.5*!!ent->kart_big-0.35*!!ent->kart_small-ent->parent->scale)*0.9*time_step;
		ent->parent->skill41 = floatv(ent->parent->scale);
		

		if(ent->kart_turbo)
		{
			if(!ent->kart_turbo_ent)
			{
				you = ent_create("gocart_flame01.mdl",ent->x,NULL);
				set(you,PASSABLE);
				ent->kart_turbo_ent = you;
				your->material = mat_kart1;
				your->ambient = 65;
			}
			you = ent->kart_turbo_ent;
			reset(you,INVISIBLE);
			vec_fill(your->scale_x,ent->parent->scale_x);
			vec_set(your->x,ent->parent->x);
			vec_set(your->pan,ent->parent->pan);
			your->skill41 = floatv(ent->parent->scale);
			ent_bonereset_all(you);
			ent_bonescale(you,"flame_node_tr0",vector(1+0.1*sinv(total_ticks*300),1+0.1*sinv(total_ticks*263),1+0.1*sinv(total_ticks*287)));
		}
		else
		{
			if(you = ent->kart_turbo_ent) set(you,INVISIBLE);
		}

		synplayermodel(ent);
	}

	void synplayermodel(ENTITY* ent)
	{
		if ((ent->parent != null) && (ent->playermodel != 0)) {

			ENTITY* epa = ent->parent;
			ENTITY* epl = (ENTITY*) ent->playermodel;

			if (epa != null && epl != null) {
				
				epl->material = epa->material;
				epl->skill41 = floatv(epa->scale);
				vec_set(epl->x, epa->x);
				vec_set(epl->pan, epa->pan);
				vec_set(epl->scale_x, epa->scale_x);

				ent_animate(epl, "", (ent->skill1 * 42 + total_ticks * 5) % 100, ANM_CYCLE);
			}
		}
	}

#endif /* raceplayer_c */
