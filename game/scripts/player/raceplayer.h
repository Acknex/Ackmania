#ifndef raceplayer_h
#define raceplayer_h

#include "engine.h"
#include "skilldefines.h"

var g_doNotDrive = false;

SOUND* g_sndKartJump = "kart_jump.wav";
var g_kartsnd_jump = 45;

SOUND* g_sndKartHit = "kart_hit.wav";
var g_kartsnd_hit = 60;

BMAP* bmp_smoke_spr1 = "smoke_spr1.tga";
BMAP* bmp_quad = "quad.tga";
BMAP* bmp_shadow = "decal_shadow.tga";

SOUND* g_sndAiaiaiai = "aiaiaiai.ogg";
var g_kartsnd_aiaiai = 100;

SOUND* g_sndDrift = "kart_drift.wav";
var g_sndDriftvol = 60;

var gamepad_active;
var key_up = 0;
var key_down = 0;
var key_left = 0;
var key_right = 0;
var key_hop = 0;
var key_hop_off = 0;
var key_item = 0;

var g_raceplayerTurnSpeed = 5;
var g_raceplayerMaxSpeed = 52.5;
var g_raceplayerAccelSpeed = 1.75;
var g_raceplayerBreakForce = 15;
var g_raceplayerBadGroundFac = 0.4;
var g_raceTrackWidth = 160;

void synplayermodel (ENTITY* ent);

var is_kart_turning(ENTITY* ent); // left = 1, right = -1
var is_kart_accelerating(ENTITY* ent);
var get_kart_accel(ENTITY* ent);
var get_kart_speed(ENTITY* ent, VECTOR* vdir);
var get_kart_lap_player();
var get_kart_rank_player();
var get_max_laps();
var is_kart_player_wrong_way(); // 1 equals wrong way (-1 player not found)
//var is_race_completed();
void trap_driver(ENTITY* ent, var blackOutTime);
void driver_hit(ENTITY* ent, var blackOutTime);
void start_turbo(ENTITY* ent, var time);
void enlarge_driver(ENTITY* ent, var time);
void minimize_driver(ENTITY* ent, var time);

void postConstructPlayer (ENTITY* ent);

void loadPlayerHumanControlParams (ENTITY* ent);
void loadPlayerCpuControlParams (ENTITY* ent);

void updatePlayer (ENTITY* ent);

void path_get_closest_position(VECTOR* vpos, VECTOR* vresult, VECTOR* vdata);
void path_get_offset_position(VECTOR* vdata, var offset, VECTOR* vresult);
void path_get_normal_position(VECTOR* vpos, var path_offset, var offset, VECTOR* vresult);
var ent_path_get_progress(ENTITY* kart, var* length);

	MATERIAL* mat_water1 =
	{
		effect = "water1.fx";
		flags = AUTORELOAD;
	}
	
	MATERIAL* mat_kart1 =
	{
		effect = "kart.fx";
		flags = AUTORELOAD;
	}


#include "raceplayer.c"

#endif /* raceplayer_h */
