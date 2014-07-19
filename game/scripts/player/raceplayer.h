#ifndef raceplayer_h
#define raceplayer_h

#include "skilldefines.h"

var gamepad_active;
var key_up = 0;
var key_down = 0;
var key_left = 0;
var key_right = 0;
var key_hop = 0;
var key_hop_off = 0;
var key_item = 0;

var g_raceplayerTurnSpeed = 4;
var g_raceplayerMaxSpeed = 50;
var g_raceplayerAccelSpeed = 1.75;
var g_raceplayerBreakForce = 15;
var g_raceplayerBadGroundFac = 0.333;

#define falling skill78
#define particle_emit skill79
#define old_speed skill80
#define bump_ang skill81
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
#define group_track 4

var is_kart_turning(ENTITY* ent); // left = 1, right = -1
var get_kart_accel(ENTITY* ent);
var get_kart_speed(ENTITY* ent, VECTOR* vdir);

void postConstructPlayer (ENTITY* ent);

void loadPlayerHumanControlParams (ENTITY* ent);
void loadPlayerCpuControlParams (ENTITY* ent);

void updatePlayer (ENTITY* ent);

#include "raceplayer.c"

#endif /* raceplayer_h */
