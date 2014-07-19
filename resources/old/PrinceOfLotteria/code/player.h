#ifndef _PLAYER_H_
#define _PLAYER_H_

#define PL_GRAVITY               8
#define PL_MAX_JUMP_TIME         1
#define PL_MAX_JUMP_HEIGHT       110
#define PL_JETPACK_REFRESH_STEP  0.5
#define PL_JETPACK_MAX_STRENGTH  0.5
#define PL_JETPACK_MAX_FUEL      10


#define PL_JUMP_TIME             skill1
#define PL_JUMP_HEIGHT           skill2
#define PL_IS_JUMPING            skill3
#define PL_ANIMATION_PERCENTAGE  skill4
#define PL_IS_ATTACKING          skill5
#define PL_ATTACKING_PERCENTAGE  skill6
#define PL_PLAYER_POS_Y          skill7
#define PL_HEALTH                skill8
//#define PL_LIFES                 skill9
#define PL_A4_COUNT              skill10
#define PL_ATTACKED_ONCE         FLAG8

float fPlayerJetpack          = PL_JETPACK_MAX_FUEL;
float fPlayerJetpackCooldown  = 0;

var vPlayerSndHandle          = 0;
var vPlayerWoho					= 0;

var flying_man				= 0;

var godmode                   = 0;

int nPlayerLifes              = 3;

//BMAP* bmapSmoke = "..\\images\\smoke.tga";
BMAP* bmapBlood = "..\\images\\blood.tga";

SOUND* sndAttack01            = "..\\sounds\\attack01.wav";
SOUND* sndAttack02            = "..\\sounds\\attack02.wav";
SOUND* sndAttack03            = "..\\sounds\\attack03.wav";
SOUND* sndAttack04            = "..\\sounds\\attack04.wav";
SOUND* sndAttack05            = "..\\sounds\\attack05.wav";
SOUND* sndDie                 = "..\\sounds\\die.wav";
SOUND* sndFootsteps01         = "..\\sounds\\footstep01.wav";
SOUND* sndFootsteps02         = "..\\sounds\\footstep02.wav";
SOUND* sndFootsteps03         = "..\\sounds\\footstep03.wav";
SOUND* sndFootsteps04         = "..\\sounds\\footstep04.wav";
SOUND* sndJetpackLoop         = "..\\sounds\\jetpackloop.wav";
SOUND* sndJetpack01           = "..\\sounds\\jetpack01.wav";
SOUND* sndJetpack02           = "..\\sounds\\jetpack02.wav";
SOUND* sndJetpack03           = "..\\sounds\\jetpack03.wav";
SOUND* sndJetpack04           = "..\\sounds\\jetpack04.wav";
SOUND* sndJetpack05           = "..\\sounds\\infinity.wav"; // Rarely
SOUND* sndJump01              = "..\\sounds\\jump01.wav";
SOUND* sndJump02              = "..\\sounds\\jump02.wav";
SOUND* sndJump03              = "..\\sounds\\jump03.wav";
SOUND* sndJump04              = "..\\sounds\\jump04.wav";
SOUND* sndJump05              = "..\\sounds\\jump05.wav";
SOUND* sndLife01              = "..\\sounds\\life01.wav";
SOUND* sndLife02              = "..\\sounds\\life02.wav";

action actPlayer();

void activate_jetpack();

#include "player.c"

#endif