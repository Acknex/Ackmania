#ifndef characters_c
#define characters_c

#include "characters.h"

STRING* get_charactername (int id) {
   return (g_characterNames->pstring)[id];
}

STRING* get_kartfilename (int id) {
   return (g_characterKartFilenames->pstring)[id];
}

STRING* get_kartplayerfilename (int id) {
   return (g_characterPlayerFilenames->pstring)[id];
}


#endif /* characters_c */
