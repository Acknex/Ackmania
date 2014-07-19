/*
 *******************************************************************************
 * platform.h
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
 * definition script for level actions
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */
 
#ifndef PLATFORM_H
#define PLATFORM_H

#define durability skill1
#define id         skill1
#define speed      skill2
#define idletime   skill3
#define radius     skill3
#define absspeed   skill5
#define absspeed_x skill5
#define absspeed_y skill6
#define absspeed_z skill7
#define ptr_start  skill8
#define ptr_end    skill9

#define is_platform  FLAG1
#define is_marker    FLAG2
#define is_touched   FLAG2
 
/*
action platform_marker();
action moving_platform();
action rotating_platform();
action breaking_platform();
*/

void breaking_platform_evt();

#include "platform.c"

#endif