#ifndef raceplayer_c
#define raceplayer_c

#include "engine.h"
#include "raceplayer.h"


BMAP* bmp_smoke_spr1 = "smoke_spr1.tga";
BMAP* bmp_quad = "quad.tga";

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
	p.size = 9+random(5);
	p.alpha = 30+random(10);
	p.skill_a = p.size*5;
	p.bmap = bmp_smoke_spr1;
	p.event = p_drift_smoke_fade;
}

void p_kart_grass(PARTICLE* p)
{
	vec_add(p.vel_x,vector(1-random(2)-p.vel_x*0.65,1-random(2),7+random(4)));
	vec_set(p.blue,vector(100,130+random(20),109));
	vec_scale(p.blue,0.8+random(0.2));
	set(p,MOVE);
	p.bmap = bmp_quad;
	p.gravity = 3;
	p.lifespan = 24;
	p.size = 2+random(2);
	p.event = NULL;
}

var get_line_segment_point_dist(VECTOR* vec1, VECTOR* vec2, VECTOR* point, VECTOR* vresult, var* t)
{
    var res,i;
    VECTOR dir1,dir2;
    
    vec_diff(&dir1,vec2,vec1);
    vec_diff(&dir2,point,vec1);
    i = vec_dot(&dir2,&dir1)/vec_dot(&dir1,&dir1);
    
    if(i >= 0 && i <= 1)
    {
        vec_lerp(&dir1,vec1,vec2,i);
        
        res = vec_dist(&dir1,point);
        if(vresult) vec_set(vresult,&dir1);
    }
    else res = -1;
    if(t) *t = i;
    
    return res; 
}

var path_get_closest_position(VECTOR* vpos, VECTOR* vresult)
{
	var dist,max_dist = 99999;
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
		dist = get_line_segment_point_dist(temp,temp2,vpos,temp3,NULL);
		if(dist < max_dist)
		{
			max_dist = dist;
			vec_set(vresult,temp3);
		}
	}
	
	ptr_remove(ent);
}


action ac_track()
{
	my.group = group_track;
	set(my,POLYGON);
	c_setminmax(my);
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

var is_kart_turning(ENTITY* ent)
{
	var left, right;
	left = !!(ent->kart_input & INPUT_LEFT);
	right = !!(ent->kart_input & INPUT_RIGHT);
	return left-right;
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
void kart_event()
{
	var new_angle;
	VECTOR temp;
	
	vec_diff(temp,my->x,target);
	my->bounce_x = temp.x;
	my->bounce_y = temp.y;
	if(my->speed > g_raceplayerMaxSpeed*0.25 || 1)
	{
		my->speed *= 0.3;
		new_angle = get_xyangle(bounce);
		if(abs(ang(new_angle-my->drive_pan)) < 90) my->bump_ang = ang(new_angle-my->drive_pan);
		//my.bump_ang = ang(new_angle-my.drive_pan)*(abs(ang(new_angle-my.drive_pan));
	}
}

void postConstructPlayer(ENTITY* ent)
{
   set(ent, INVISIBLE);
   c_setminmax(ent);

   ent->event = kart_event;
   ent->emask |= ENABLE_BLOCK | ENABLE_ENTITY | ENABLE_ENTITY;

   ent->ambient = -50;

   VECTOR vecMin;
   vec_for_min(&vecMin, ent);
   ent->kart_height = -vecMin.z;

   ent->group = group_kart;

   ent->parent = ent_create(str_for_entfile(null, ent), ent->x, null);
   set(ent->parent, PASSABLE);
}

void loadPlayerHumanControlParams(ENTITY* ent)
{
   if (gamepad_active)
   {
      key_up = 0;
      key_down = 0;
      key_left = 0;
      key_right = 0;
      key_hop = 0;
      key_item = 0;
   } else {
      key_up = key_cuu;
      key_down = key_cud;
      key_left = key_cul;
      key_right = key_cur;
      key_hop = key_space;
      key_item = key_ctrl;
   }

   ent->kart_input = key_up * INPUT_UP | key_down * INPUT_DOWN | key_left * INPUT_LEFT | key_right * INPUT_RIGHT | key_hop * INPUT_HOP
         | key_item * INPUT_ITEM;
}

void loadPlayerCpuControlParams(ENTITY* ent)
{
   // TODO
}

void updatePlayer(ENTITY* ent)
{
   VECTOR temp,temp2;
   var up, down, left, right, hop, item, underground, old_contact, turn;

   if(ent->falling)
   {
   	set(ent,PASSABLE);
   	/*ent->speed += -ent->speed*0.1*time_step;
     vec_set(temp, vector(ent->speed, 0, 0));
   vec_rotate(temp, vector(ent->drive_pan, 0, 0));
   vec_add(temp,vector(ent->bounce_x,ent->bounce_y,0));
   ent->speed_x = temp.x;
   ent->speed_y = temp.y;
   vec_scale(temp, time_step);

   c_ignore(group_track,0);
   c_move(ent, nullvector, temp, IGNORE_PUSH | IGNORE_PASSABLE | GLIDE | USE_POLYGON);

   vec_scale(ent->bounce_x,1-0.4*time_step);*/

ent->speed_z = maxv(ent->speed_z-6*time_step,-90);
ent->falling += (maxv(0,50+ent->speed_z)-ent->falling)*0.15*time_step;
     vec_set(temp, vector(ent->falling, 0, 0));
   vec_rotate(temp, vector(ent->drive_pan+180*(ent->falling_dir == 1)+90*(ent->falling_dir == 2)-90*(ent->falling_dir == 3), 0, 0));
   temp.z += ent->speed_z;
   vec_scale(temp, time_step);
 c_move(ent, nullvector, temp, IGNORE_MODELS | IGNORE_WORLD | IGNORE_SPRITES);
ang_rotate(ent->parent->pan,vector(0,(-(ent->falling_dir == 0)+(ent->falling_dir == 1))*15*time_step,(-(ent->falling_dir == 2)+(ent->falling_dir == 3))*15*time_step));
  vec_set(ent->parent->x, ent->x);
  ent->parent->z += ent->parent->skill1;
 DEBUG_VAR(ent->falling_dir,400);
      return;
   }
   reset(ent,PASSABLE);

   ent->old_speed = ent->speed;
   up = !!(ent->kart_input & INPUT_UP);
   down = !!(ent->kart_input & INPUT_DOWN);
   left = !!(ent->kart_input & INPUT_LEFT);
   right = !!(ent->kart_input & INPUT_RIGHT);
   hop = !!(ent->kart_input & INPUT_HOP);
   item = !!(ent->kart_input & INPUT_ITEM);

   if (!ent->drifting) {
      turn = g_raceplayerTurnSpeed * (left - right);
   } else {
      turn = ent->drift_pan * 0.3;
   }

   ent->turn_speed += (turn * (ent->speed / g_raceplayerMaxSpeed) - ent->turn_speed) * 0.55 * time_step;
   ent->turn_speed2 += clamp((ent->turn_speed - ent->turn_speed2), -0.5, 0.5) * time_step;
   ent->drive_pan += ent->ground_contact * ent->turn_speed * time_step;
	ent->drive_pan += (ent->bump_ang*0.35+ent->ground_contact*ent->turn_speed)*time_step;
	ent->drive_pan = ang(ent->drive_pan);
	ent->bump_ang += -ent->bump_ang*0.35*time_step;
	
  if (up && !down) {
      ent->speed += minv((g_raceplayerMaxSpeed-ent->speed)*0.1,g_raceplayerAccelSpeed) * time_step;
   }
            ent->speed = minv(ent->speed,(g_raceplayerMaxSpeed - g_raceplayerMaxSpeed*0.5*abs(ent->turn_speed2)/g_raceplayerTurnSpeed * !ent->drifting) * ent->underground);

   if (!(up || down)) {
      ent->speed += clamp(-ent->speed * 0.125, -g_raceplayerAccelSpeed*0.25, g_raceplayerAccelSpeed*0.25) * time_step;
   }

   if (!up && down) {
      ent->speed = maxv(ent->speed - (0.5+0.5*(ent->speed > 0))*g_raceplayerAccelSpeed*2 * time_step, -g_raceplayerBreakForce*!ent.drifting);
   }

   ent->pan = ent->drive_pan;
   vec_set(temp, vector(ent->speed, 0, 0));
   vec_rotate(temp, vector(ent->drive_pan, 0, 0));
   vec_add(temp,vector(ent->bounce_x,ent->bounce_y,0));
   ent->speed_x = temp.x;
   ent->speed_y = temp.y;
   vec_scale(temp, time_step);

   c_ignore(group_track,0);
   c_move(ent, nullvector, temp, IGNORE_PUSH | IGNORE_PASSABLE | GLIDE | USE_POLYGON);

   vec_scale(ent->bounce_x,1-0.4*time_step);

   old_contact = ent->ground_contact;
   ent->ground_contact = (ent->parent->z <= ent->kart_height);

   if (!old_contact && ent->ground_contact && hop && ent->speed > 10) {
      ent->drifting = 1;
   }

   if (!hop) {
      ent->drifting = 0;
   }

     c_ignore(group_kart, 0);
      c_trace(vector(ent->x, ent->y, ent->z + 64), vector(ent->x, ent->y, -128), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON);
      //if(!trace_hit) ent->falling = 1;
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
   #define DEBUG_FELI
#ifdef DEBUG_FELIX
   vec_set(temp,ent.x);
   if(vec_to_screen(temp,camera))
   {
      draw_text(str_for_num(NULL,get_kart_speed(ent,NULL)),temp.x-40,temp.y-20,COLOR_RED);
      draw_text(str_for_num(NULL,ent->drifting),temp.x-40,temp.y,COLOR_RED);
      draw_text(str_for_num(NULL,hit.blue),temp.x-40,temp.y+20,COLOR_RED);
      draw_text(str_for_num(NULL,hit.green),temp.x-40,temp.y+40,COLOR_RED);
      draw_text(str_for_num(NULL,hit.red),temp.x-40,temp.y+60,COLOR_RED);
      draw_text(str_for_num(NULL,underground),temp.x-40,temp.y+80,COLOR_RED);
      draw_text(str_for_num(NULL,is_kart_turning(ent)),temp.x-40,temp.y+100,COLOR_RED);
      draw_text(str_for_num(NULL,get_kart_accel(ent)),temp.x-40,temp.y+120,COLOR_RED);
   }
#endif


   if (ent->ground_contact) {

      if (!ent->kart_id && !key_hop) {
         key_hop_off = 1;
      }

      if (hop && (key_hop_off || ent->kart_id > 0)) {

         if (!ent->kart_id) {
            key_hop_off = 0;
         }

         ent->speed_z = 15;
         ent->ground_contact = 0;
      }
   }

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
   ent->parent->pan = ent->pan + ent->drift_pan;
   ent->parent->skill1 += ent->speed_z * time_step;
   ent->parent->skill1 = maxv(ent->parent->skill1, ent->kart_height);
   ent->parent->z = ent->parent->skill1;
   ent->speed_z -= 9 * time_step;
   
   if(ent->speed < 15 || ent->underground < 0.9) ent->drifting = 0;
   if(ent->ground_contact && ent->speed >= g_raceplayerMaxSpeed*0.2) {
   	ent->particle_emit += time_step;
   	if(ent->particle_emit > 0.5)
   	{
   		ent->particle_emit -= 0.5;
   		vec_set(temp,vector(-22,22,-ent->kart_height*0.5));
   		vec_rotate(temp,ent->parent->pan);
   		vec_add(temp,ent->parent->x);
   		vec_set(temp2,vector(-4,0,4));
   		vec_rotate(temp2,ent->parent->pan);
		if(ent->drifting) effect(p_drift_smoke,1,temp,temp2);
		else
		{
      		c_ignore(group_kart, 0);
      		c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON);
      		if(hit.green > 100) effect(p_kart_grass,1,temp,temp2);
		}
   		vec_set(temp,vector(-22,-22,-ent->kart_height*0.5));
   		vec_rotate(temp,ent->parent->pan);
   		vec_add(temp,ent->parent->x);
		if(ent->drifting) effect(p_drift_smoke,1,temp,temp2);
		else
		{
      		c_ignore(group_kart, 0);
      		c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON);
      		if(hit.green > 100) effect(p_kart_grass,1,temp,temp2);
		}
	}
   }
   	vec_set(temp,vector(20,0,0));
   	vec_rotate(temp,ent->pan);
   	vec_add(temp,ent->x);
    c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON);
   	if(!trace_hit)
   	{
   		ent->falling = ent->speed;
   		ent->falling_dir = 0;
   		ent->speed_z = maxv(ent->speed_z,0)+3;
   	}
   	vec_set(temp,vector(-20,0,0));
   	vec_rotate(temp,ent->pan);
   	vec_add(temp,ent->x);
    c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON);
   	if(!trace_hit)
   	{
   		ent->falling = ent->speed;
   		ent->falling_dir = 1;
   		ent->speed_z = maxv(ent->speed_z,0)+3;
   	}
   	vec_set(temp,vector(0,20,0));
   	vec_rotate(temp,ent->pan);
   	vec_add(temp,ent->x);
    c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON);
   	if(!trace_hit)
   	{
   		ent->falling = ent->speed;
   		ent->falling_dir = 2;
   		ent->speed_z = maxv(ent->speed_z,0)+3;
   	}
   	vec_set(temp,vector(0,-20,0));
   	vec_rotate(temp,ent->pan);
   	vec_add(temp,ent->x);
    c_trace(vector(temp.x, temp.y, temp.z + 64), vector(temp.x, temp.y, -128), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE | USE_POLYGON);
   	if(!trace_hit)
   	{
   		ent->falling = ent->speed;
   		ent->falling_dir = 3;
   		ent->speed_z = maxv(ent->speed_z,0)+3;
   	}
   	
   	path_get_closest_position(ent.x,temp2);
   	draw_point3d(temp2,COLOR_RED,100,10);
}

#endif /* raceplayer_c */
