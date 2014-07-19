#ifndef startgrid_c
#define startgrid_c

#include "engine.h"
#include "startgrid.h"
#include "skilldefines.h"
#include "raceactions.h"

void setStartgridConfig(int rank, VECTOR* pos, var ang) {
   vec_set(&(g_vecStartgridPos[rank - 1]), pos);
   g_vecStartgridAng[rank - 1] = ang;
   g_numGridConfigUploaded++;
}

void create_kart_drivers_ev() {
   ac_race_kart_ent();
}

// erzeugt alle Spieler und gibt das globale Entity-Array zurück
ENTITY** create_kart_drivers()
{
   memset(g_kartEnts, 0, sizeof(ENTITY*) * MAX_STARTGRID_POS);

   int i;
   for (i = 0; i < MAX_STARTGRID_POS; i++) {

      VECTOR* pos = &(g_vecStartgridPos[i]);
      var ang = g_vecStartgridAng[i];

      ENTITY* ent = ent_create(CUBE_MDL, pos, create_kart_drivers_ev);
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
