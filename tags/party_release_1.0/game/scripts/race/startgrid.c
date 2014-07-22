#ifndef startgrid_c
#define startgrid_c

#include "engine.h"
#include "startgrid.h"
#include "skilldefines.h"
#include "raceactions.h"
#include "characters.h"

void setStartgridConfig(int rank, VECTOR* pos, var ang) {
   vec_set(&(g_vecStartgridPos[rank - 1]), pos);
   g_vecStartgridAng[rank - 1] = ang;
   g_numGridConfigUploaded++;
}

void create_kart_drivers_ev() {
   ac_race_kart_ent();
}

void create_kart_driverplayer_ev() {
   set(my,PASSABLE);
}

// erzeugt alle Spieler und gibt das globale Entity-Array zurück
ENTITY** create_kart_drivers()
{
   memset(g_kartEnts, 0, sizeof(ENTITY*) * MAX_STARTGRID_POS);

   int i;
   for (i = 0; i < MAX_STARTGRID_POS; i++) {

      VECTOR* pos = &(g_vecStartgridPos[i]);
      var ang = g_vecStartgridAng[i];

      STRING* strKartFilename = get_kartfilename(i);
      STRING* strKartModelFilename = get_kartplayerfilename(i);

      ENTITY* ent = ent_create(strKartFilename, pos, create_kart_drivers_ev);
      ENTITY* entplayer = ent_create(strKartModelFilename, pos, create_kart_driverplayer_ev);

      ent->playermodel = entplayer;

      g_kartEnts[i] = ent;

      if (ent != null) {
         ent->pan = ang;
         ent->sk_kart_id = i + 1;
      }
   }

   return g_kartEnts;
}

// gibt den Spieler mit dem Index zurück
ENTITY* get_kart_driver(int index) {
   return g_kartEnts[index];
}

// gibt HUMAN-Spieler zurück
ENTITY* get_kart_player() {
   return g_kartEnts[0];
}

#endif /* startgrid_c */
