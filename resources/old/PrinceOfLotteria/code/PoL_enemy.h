
//Global vars
var PoL_enemy_dead_enemies = 0;

//Skills
#define PoL_enemy_health skill1
#define PoL_enemy_speed skill2
#define PoL_enemy_akdir skill3
#define PoL_enemy_tdist skill4
#define PoL_enemy_intcalc skill5
#define PoL_enemy_intcalcdir skill6
#define PoL_enemy_anmperc skill7
#define PoL_waittimer skill8
#define PoL_attacktriggered skill9
#define PoL_attackanm skill10
#define PoL_enemyseeplayer skill11
#define PoL_is_enemy FLAG2

//Wait
#define PoL_maxwaitticks 64
#define POL_randomwaittimer 160

//Defaults
#define PoL_enemy_defaultspeed 10
#define PoL_enemy_defaulthealth 3

//Params
#define PoL_enemy_gravity_force 40
#define PoL_enemy_checkaheadfactor 5 
#define PoL_enemy_changedirspeed 45
#define PoL_enemy_playerdetectrange 450
#define PoL_enemy_playerdetectrangevertical 80
#define PoL_enemy_playerattackrange 100
#define PoL_enemy_diseredattackdist 85
#define PoL_enemy_attackcooldown 5


//Animation
#define PoL_enemy_walkanimname "walk"
#define PoL_enemy_walkanimspeed 1

#define PoL_enemy_waitanimname "idle"
#define PoL_enemy_waitanimpercpertick 2

#define PoL_enemy_attackanimname "attack2"
#define PoL_enemy_attackanimpercpertick 15

#define PoL_enemy_deathanimname "death1"
#define PoL_enemy_deathanimpercpertick 5

#include "Pol_enemy.c"