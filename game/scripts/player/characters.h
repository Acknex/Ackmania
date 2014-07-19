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
         "gocart01.mdl",
         "gocart01.mdl",
         "gocart01.mdl",
         "gocart01.mdl"
   );
}

// gibt den Namen des Spielers mit dem index = 0... zurück
STRING* get_charactername(int id);

// gibt den Dateinamen des Spielerkarts mit dem index = 0... zurück
STRING* get_kartfilename(int id);

#include "characters.c"

#endif /* characters_h */
