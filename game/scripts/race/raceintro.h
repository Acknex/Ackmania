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

/*
 * Starts the raceintro
 */
void raceintro_trigger(ENTITY* ent, void *callback);

#include "raceintro.c"

#endif // #ifndef _RACEINTRO_H_
