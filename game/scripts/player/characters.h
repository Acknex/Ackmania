#ifndef characters_h
#define characters_h

TEXT* g_characterNames = {
   string (
         "Lotti",
         "Felicitas",
         "Doug",
         "Marco"
   );
}

TEXT* g_characterKartFilenames = {
   string (
         "kart-pl1.mdl",
         "kart-pl2.mdl",
         "kart-pl3.mdl",
         "kart-pl4.mdl"
   );
}

// gibt den Namen des Spielers mit dem index = 0... zurück
STRING* get_charactername(int id);

// gibt den Dateinamen des Spielerkarts mit dem index = 0... zurück
STRING* get_kartfilename(int id);

#include "characters.c"

#endif /* characters_h */
