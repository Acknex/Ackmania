#include <acknex.h>
#include <default.c>
#include <mtlFx.c>

#define PL_A4_COUNT skill1
#define CURRENT_ITEM skill2
#define DRIVER_MAX_SPEED skill3
#define MAX_SPEED 50

#define PRAGMA_PATH "..\\..\\scripts\\items";
#define PRAGMA_PATH "..\\..\\scripts\\helper";
#define PRAGMA_PATH "..\\..\\sounds\\items";
#define PRAGMA_PATH "..\\..\\models";

#include "items.h"

action actPlayer()
{
	while(1)
	{
		if (key_space) {
			while(key_space) wait(1);
			shoot_rocket();
		}
		wait(1);
	}
}

void main()
{
	level_load("../../levels/test/dummy_level.wmb");
	vec_set(camera.x, vector(-1200, 400, 600));
	vec_set(camera.pan, vector(335, -30, 0));
	ENTITY* p = ent_for_name("lotter_dummy_005");
	ENTITY* newP = ent_create(str_for_entfile(NULL, p), p->x, actPlayer);
	ent_remove(p);
	
	
	ENTITY* a4 = ent_for_name("a4Cube_mdl_004");
	ENTITY* newa4 = ent_create(str_for_entfile(NULL, a4), a4->x, a4_cube);
	ent_remove(a4);
}