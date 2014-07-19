#ifndef environment_h
#define environment_h

#include "sky.h"

void environment_load (char* str, float skyspeed)
{
   level_load(str);
   skychange(skyspeed);
}

#include "environment.c"

#endif /* environment_h */
