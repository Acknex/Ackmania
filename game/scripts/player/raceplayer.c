#ifndef raceplayer_c
#define raceplayer_c

#include "engine.h"
#include "raceplayer.h"

#define DEBUG_FELIX

action ac_track()
{
	my.group = group_track;
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
   VECTOR temp;
   var up, down, left, right, hop, item, underground, old_contact, turn;

   up = !!(ent->kart_input & INPUT_UP);
   down = !!(ent->kart_input & INPUT_DOWN);
   left = !!(ent->kart_input & INPUT_LEFT);
   right = !!(ent->kart_input & INPUT_RIGHT);
   hop = !!(ent->kart_input & INPUT_HOP);
   item = !!(ent->kart_input & INPUT_ITEM);

   if (!ent->drifting) {
      turn = g_raceplayerTurnSpeed * (left - right);
   } else {
      turn = ent->drift_pan * 0.2;
   }

   ent->turn_speed += (turn * (ent->speed / g_raceplayerMaxSpeed) - ent->turn_speed) * 0.55 * time_step;
   ent->turn_speed2 += clamp((ent->turn_speed - ent->turn_speed2), -0.5, 0.5) * time_step;
   ent->drive_pan += ent->ground_contact * ent->turn_speed * time_step;
	ent->drive_pan += (ent->bump_ang*0.35+ent->ground_contact*ent->turn_speed)*time_step;
	ent->drive_pan = ang(ent->drive_pan);
	ent->bump_ang += -ent->bump_ang*0.35*time_step;
	
  if (up && !down) {
      ent->speed = minv(ent->speed + minv((g_raceplayerMaxSpeed-ent->speed)*0.1,0.5 * g_raceplayerAccelSpeed) * time_step,
            (g_raceplayerMaxSpeed - 4 * ent->turn_speed2 * !ent->drifting) * ent->underground);
   }

   if (!(up || down)) {
      ent->speed += clamp(-ent->speed * 0.25, -g_raceplayerAccelSpeed, g_raceplayerAccelSpeed) * time_step;
   }

   if (!up && down) {
      ent->speed = maxv(ent->speed - (0.5+0.5*(ent->speed > 0))*g_raceplayerAccelSpeed * time_step, -g_raceplayerBreakForce*!ent.drifting);
   }

#ifdef DEBUG_FELIX
   DEBUG_VAR(ent->speed, screen_size.y - 40);
#endif

   ent->pan = ent->drive_pan;
   vec_set(temp, vector(ent->speed, 0, 0));
   vec_rotate(temp, vector(ent->drive_pan, 0, 0));
	vec_add(temp,vector(ent.bounce_x,ent.bounce_y,0));
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

#ifdef DEBUG_FELIX
   DEBUG_VAR(ent->drifting, screen_size.y - 80);
#endif

   if (ent->ground_contact) {

      c_ignore(group_kart, 0);
      c_trace(vector(ent->x, ent->y, ent->z + 64), vector(ent->x, ent->y, -16), IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE);

      if (tex_flag4 || tex_flag5 || tex_flag6 || tex_flag7 || tex_flag8) {
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

      ent->drift_pan += clamp((ent->drift_dir * (10 + 10 * abs(left - right) * (sign(left - right) == ent->drift_dir)) - ent->drift_pan)*0.4, -3.5, 3.5)
            * time_step;
   }

   vec_set(ent->parent->x, ent->x);
   vec_set(ent->parent->pan, ent->pan);
   ent->parent->pan = ent->pan + ent->drift_pan;
   ent->parent->skill1 += ent->speed_z * time_step;
   ent->parent->skill1 = maxv(ent->parent->skill1, ent->kart_height);
   ent->parent->z = ent->parent->skill1;
   ent->speed_z -= 9 * time_step;
}

#endif /* raceplayer_c */
