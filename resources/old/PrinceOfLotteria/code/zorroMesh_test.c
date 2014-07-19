#include <acknex.h>
#include <default.c>

#include "zorroMesh.h"

ENTITY* g_player = NULL;

void k_1 () { zorroIndiJonesHat(g_player, true); }
void k_2 () { zorroIndiJonesHat(g_player, false); }

void k_3 () { zorroSwordAtBelt(g_player, true); }
void k_4 () { zorroSwordAtBelt(g_player, false); }

void k_5 () { zorroHasJetpack(g_player, true); }
void k_6 () { zorroHasJetpack(g_player, false); }

int main ()
{
	fps_max = 60;
	video_mode = 8;

	wait(3);
	level_load(NULL);
	
	on_1 = k_1;
	on_2 = k_2;
	on_3 = k_3;
	on_4 = k_4;
	on_5 = k_5;
	on_6 = k_6;

	g_player = ent_create("zorroLotti.mdl", nullvector, NULL);
	zorroMeshOptions(g_player, false, false, false);
	
	vec_set(camera->x, vector(-32,-119,35));
	vec_set(camera->pan, vector(72,-14,0));
	
}