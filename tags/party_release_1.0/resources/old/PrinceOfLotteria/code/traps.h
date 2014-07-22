/*
 *******************************************************************************
 * traps.h
 * Creation date: 08.06.2013
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2011-02-23 22:27:12 +0100 (Mi, 23 Feb 2011) $
 * $Revision: 6 $
 * $Author: Firo $
 *
 *******************************************************************************
 * Description
 *
 * definition script for level traps
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef TRAPS_H
#define TRAPS_H
 
#define is_trap       FLAG3
#define is_triggered  FLAG4
#define is_collided   FLAG5
 
#define SPIKE_TIMER_UP         3
#define SPIKE_TIMER_DOWN       1
#define SPIKE_SPEED            20
#define STONE_TRIGGERDIST      200
#define STONE_SPEED            30
#define STALACTITE_TRIGGERDIST 30
#define STALACTITE_SPEED       60
 
/*
action moving_spikes();
action falling_stone();
action falling_stalactite();
action trap();
*/

void falling_trap_evt();

#include "traps.c"

#endif