#ifndef characters_h
#define characters_h

#include "characters.c"

TEXT* g_characterNames = {
   string (
         "Lotti",
         "Felicitas",
         "Doug",
         "Marco"
   );
}

// gibt den Namen des Spielers mit dem index = 0... zurück
STRING* get_charactername (int id);

#endif /* characters_h */
