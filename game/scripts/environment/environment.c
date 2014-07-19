#ifndef environment_c
#define environment_c

#include "environment.h"

void environment_load (char* str, float skyspeed)
{
   level_load(str);
   skychange(skyspeed);
}

#endif /* environment_c */
