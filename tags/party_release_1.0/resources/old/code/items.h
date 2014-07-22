/*
 *******************************************************************************
 * items.h
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
 * definition script for collectible items
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef ITEMS_H
#define ITEMS_H
 
#define is_collected  FLAG4

/*
action item_coin();
action item_life();
action item_jetpack();
*/

void item_setup();
void item_fade();
void item_coin_evt();
void item_life_evt();
void item_jetpack_evt();
void item_particleFader (PARTICLE *p);
void item_particle (PARTICLE *p);

#include "items.c"

#endif