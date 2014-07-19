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
var key_item = 0;

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

void kart_init()
{
	c_setminmax(my);
	my.kart_height = 0;
	my.kart_id = 0;
}

void kart_update(ENTITY* ent)
{
	if(!my.kart_id)
	{
		my.kart_input = key_up*INPUT_UP | key_down*INPUT_DOWN | key_left*INPUT_LEFT | key_right*INPUT_RIGHT | key_hop*INPUT_HOP | key_item*INPUT_ITEM;
	}
	
}

void update_karts()
{
	for(i = 0; i < MAX_KARTS; i++)
	{
		if(ent_karts[i]) kart_update(ent_karts[i]);
	}
}

void InitKarts()
{
	int i;
	for(i = 0; i < MAX_KARTS; i++) ent_karts[i] = NULL;
	i = 0;
	/*for(you = ent_next(NULL); you; you = ent_next(you))
	{
		if(your._type == type_kart_spawn)
		{
			ent_karts[i] = ent_create("kart.mdl",your.x,fnc_kart);
			ent_karts[i].kart_id = i;
			i++;
		}
	}*/
}

void main()
{
	fps_max = 60;
	level_load("testlevel.wmb");
	InitKarts();
}