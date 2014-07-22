#ifndef _RACEINTRO_H_
#define _RACEINTRO_H_

var g_raceintro_ticks = 16;
var g_raceintro_speed = 2;
var g_raceintro_offset = 300;

SOUND* g_sndIntroBeep = "startbeep.ogg";

ENTITY *raceintroNumber1 = {
   type = "numOne.mdl";
   pan = -90;
}
ENTITY *raceintroNumber2 = {
   type = "numTwo.mdl";
   pan = -90;
}
ENTITY *raceintroNumber3 = {
   type = "numThree.mdl";
   pan = -90;
}
ENTITY *raceintroNumber4 = {
   type = "numExc.mdl";
   pan = -90;
}

SOUND *raceintroBeep = "beep.ogg";

SOUND* g_sndStartRace1 = "start_race_1.wav";
SOUND* g_sndStartRace2 = "start_race_2.wav";
SOUND* g_sndStartRace3 = "start_race_3.wav";
SOUND* g_sndStartRaceGo = "start_race_go.wav";

/*
 * Starts the raceintro
 */
void raceintro_trigger(ENTITY* ent, SOUND* snd, void *callback);

#include "raceintro.c"

#endif // #ifndef _RACEINTRO_H_
