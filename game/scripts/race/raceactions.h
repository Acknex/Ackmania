#ifndef raceactions_h
#define raceactions_h

void ac_race_kart_pos();
void ac_race_kart_ent();

var g_exployothreshold = 10;
var g_breakdownthreshold = 40;

SOUND* g_sndKartStart = "kart_start.wav";
SOUND* g_sndKartLoop = "kart_loop.wav";
SOUND* g_sndKartLeerlauf = "kart_leerlauf.wav";
SOUND* g_sndKartBremsen = "kart_bremsen.wav";

#define kartsnd_off skill30
#define kartsnd_loop_hndl skill31
#define kartsnd_over skill28

#include "raceactions.c"

#endif /* raceactions_h */
