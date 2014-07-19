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
         "gocart02.mdl",
         "gocart03.mdl",
         "gocart04.mdl"
   );
}

TEXT* g_characterPlayerFilenames = {
   string (
         "ackmen01.mdl",
         "ackmen02.mdl",
         "ackmen03.mdl",
         "ackmen04.mdl"
   );
}


// gibt den Namen des Spielers mit dem index = 0... zurück
STRING* get_charactername(int id);

// gibt den Dateinamen des Spielerkarts mit dem index = 0... zurück
STRING* get_kartfilename(int id);

// gibt den Dateinamen des Spielermodells mit dem index = 0... zurück
STRING* get_kartplayerfilename(int id);

#include "characters.c"

#endif /* characters_h */
