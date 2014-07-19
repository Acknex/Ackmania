///////////////////////////////
#include <acknex.h>
#include <default.c>
///////////////////////////////

var gamepad_active;
var key_up = 0;
var key_down = 0;
var key_left = 0;
var key_right = 0;
var key_hop = 0;
var key_hop_off = 0;
var key_item = 0;
var key_sex_porn = 69;

#define ACCEL_SPEED 3
#define MAX_SPEED 50

#define drift_dir skill82
#define bounce_x skill83
#define bounce_y skill84
#define bounce_z skill85
#define turn_speed skill86
#define turn_speed2 skill87
#define drift_pan skill88
#define drifting skill89
#define ground_contact skill90
#define speed_x skill91
#define speed_y skill92
#define speed_z skill93
#define drive_pan skill94
#define underground skill95
#define speed skill96
#define kart_input skill97
#define INPUT_UP (1<<0)
#define INPUT_DOWN (1<<1)
#define INPUT_LEFT (1<<2)
#define INPUT_RIGHT (1<<3)
#define INPUT_HOP (1<<4)
#define INPUT_ITEM (1<<5)
#define kart_height skill98
#define kart_id skill99
#define _type skill100
#define type_player_kart 1
#define type_enemy_kart 2
#define type_kart_spawn 3

#define group_kart 1
#define group_item 2
#define group_obstacle 3

#define MAX_KARTS 8
ENTITY* ent_karts[MAX_KARTS];

void gamepad_update()
{
	if(gamepad_active)
	{
		key_up = 0;
		key_down = 0;
		key_left = 0;
		key_right = 0;
		key_hop = 0;
		key_item = 0;
	}
	else
	{
		key_up = key_cuu;
		key_down = key_cud;
		key_left = key_cul;
		key_right = key_cur;
		key_hop = key_space;
		key_item = key_ctrl;
	}
}

var get_xyangle(VECTOR* vec)
{
	var angle = 0;
	
	angle = asinv(vec.y);
	
	return angle;
}


void kart_event()
{
	VECTOR temp;
	
	vec_to_angle(temp,bounce);
	my.speed *= 0.75;
	my.drive_pan = get_xyangle(bounce);
}

void kart_init()
{
	VECTOR temp;
	set(my,INVISIBLE);
	c_setminmax(my);
	my.event = kart_event;
	my.emask |= ENABLE_BLOCK | ENABLE_ENTITY | ENABLE_ENTITY;
	my.ambient = -50;
	vec_for_min(temp,my);
	my.kart_height = -temp.z;
	my.kart_id = 0;
	my.group = group_kart;
	my.parent = ent_create(str_for_entfile(NULL,my),my.x,NULL);
	set(my.parent,PASSABLE);
}

void camera_focus(ENTITY* ent)
{
	VECTOR temp;
	
	/*vec_set(camera.x,vector(256,-256,1024));
	vec_add(camera.x,ent.x);
	vec_diff(temp,ent.x,camera.x);
	vec_to_angle(camera.pan,temp);*/
	vec_set(camera.x,vector(-384,0,196));
	vec_rotate(camera.x,vector(ent.drive_pan,0,0));
	vec_add(camera.x,ent.x);
	vec_set(camera.pan,vector(ent.drive_pan,-22.5,0));
}

void kart_update(ENTITY* ent)
{
	VECTOR temp;
	var up,down,left,right,hop,item,underground,old_contact,turn;
	
	if(!ent.kart_id) // player input
	{
		ent.kart_input = key_up*INPUT_UP | key_down*INPUT_DOWN | key_left*INPUT_LEFT | key_right*INPUT_RIGHT | key_hop*INPUT_HOP | key_item*INPUT_ITEM;
	}
	up = !!(ent.kart_input & INPUT_UP);
	down = !!(ent.kart_input & INPUT_DOWN);
	left = !!(ent.kart_input & INPUT_LEFT);
	right = !!(ent.kart_input & INPUT_RIGHT);
	hop = !!(ent.kart_input & INPUT_HOP);
	item = !!(ent.kart_input & INPUT_ITEM);
	
	if(!ent.drifting) turn = 4*(left-right);
	else turn = ent.drift_pan*0.2;
	ent.turn_speed += (turn*(ent.speed/MAX_SPEED)-ent.turn_speed)*0.55*time_step;
	ent.turn_speed2 += clamp((ent.turn_speed-ent.turn_speed2),-0.5,0.5)*time_step;
	ent.drive_pan += ent.ground_contact*ent.turn_speed*time_step;
	ent.turn_speed %= 360;
	if(up && !down) ent.speed = minv(ent.speed+0.5*ACCEL_SPEED*time_step,(MAX_SPEED-4*ent.turn_speed2*!ent.drifting)*ent.underground);
	if(!(up || down)) ent.speed += clamp(-ent.speed*0.25, -ACCEL_SPEED, ACCEL_SPEED)*time_step;
	if(!up && down) ent.speed = maxv(ent.speed-ACCEL_SPEED*time_step, -15);
	DEBUG_VAR(ent.turn_speed, 60);
	DEBUG_VAR(ent.speed, screen_size.y-40);
	DEBUG_VAR(ent.turn_speed2, screen_size.y-60);
	
	ent.pan = ent.drive_pan;
	vec_set(temp,vector(ent.speed,0,0));
	vec_rotate(temp,vector(ent.drive_pan,0,0));
	vec_scale(temp,time_step);
	
	c_move(ent,nullvector,temp,IGNORE_PASSABLE | GLIDE);
	
	old_contact = ent.ground_contact;
	ent.ground_contact = (ent.parent.z <= ent.kart_height);
	if(!old_contact && ent.ground_contact && hop)
	{
		ent.drifting = 1;
	}
	if(!hop) ent.drifting = 0;
	DEBUG_VAR(ent.drifting, screen_size.y-80);
	if(ent.ground_contact)
	{
		c_ignore(group_kart,0);
		c_trace(vector(ent.x,ent.y,ent.z+64),vector(ent.x,ent.y,-16),IGNORE_PASSABLE | IGNORE_PUSH | SCAN_TEXTURE);
		if(tex_flag4 || tex_flag5 || tex_flag6 || tex_flag7 || tex_flag8) underground = 0.33;
		else underground = 1;
	}
	else underground = ent.underground;
	if(ent.ground_contact)
	{
		if(!ent.kart_id && !key_hop) key_hop_off = 1;
		if(hop && (key_hop_off || ent.kart_id > 0))
		{
			if(!ent.kart_id) key_hop_off = 0;
			ent.speed_z = 15;
			ent.ground_contact = 0;
		}
	}
	ent.underground += (underground-ent.underground)*0.5*time_step;
	if(!ent.drifting)
	{
		ent.drift_pan += -ent.drift_pan*0.25*time_step;
		ent.drift_dir = 0;
	}
	else
	{
		if(!ent.drift_dir && (left || right)) ent.drift_dir = left-right;
		ent.drift_pan += clamp(ent.drift_dir*(10+10*abs(left-right)*(sign(left-right) == ent.drift_dir))-ent.drift_pan,-5,5)*time_step;
	}
	
	vec_set(ent.parent.x,ent.x);
	vec_set(ent.parent.pan,ent.pan);
	ent.parent.pan = ent.pan+ent.drift_pan;
	ent.parent.skill1 += ent.speed_z*time_step;
	ent.parent.skill1 = maxv(ent.parent.skill1,ent.kart_height);
	ent.parent.z = ent.parent.skill1;
	ent.speed_z -= 9*time_step;
	
	if(!ent.kart_id) camera_focus(ent);
}

void update_karts()
{
	int i;
	for(i = 0; i < MAX_KARTS; i++)
	{
		if(ent_karts[i]) kart_update(ent_karts[i]);
	}
}

action kart_spawn()
{
	set(my,INVISIBLE | PASSABLE | UNLIT);
	my._type = type_kart_spawn;
}

void init_karts()
{
	int i;
	for(i = 0; i < MAX_KARTS; i++) ent_karts[i] = NULL;
	i = 0;
	for(you = ent_next(NULL); you; you = ent_next(you))
	{
		if(your._type == type_kart_spawn)
		{
			ent_karts[i] = ent_create("kart.mdl",your.x,kart_init);
			ent_karts[i].kart_id = i;
			ent_karts[i].drive_pan = your.pan;
			i++;
			break;
		}
	}
}

void main()
{
	fps_max = 60;
	video_mode = 10;
	level_load("testlevel.wmb");
	wait(1);
	init_karts();
	while(1)
	{
		gamepad_update();
		update_karts();
		wait(1);
	}
}