#ifndef characters_c
#define characters_c

#include "characters.h"

STRING* get_charactername (int id) {
   return (g_characterNames->pstring)[id];
}

#endif /* characters_c */
