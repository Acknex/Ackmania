/*
 *******************************************************************************
 * timer.h
 * Creation date: 19.07.2014
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
 * definition script for head up display
 *
 * Comments
 * 
 * for short descriptions see comments in timer.h
 *
 *******************************************************************************
 */

#ifndef TIMER_H
#define TIMER_H

void reset_timer();
var timer_getHundreds();
var timer_getSeconds();
var timer_getMinutes();

#include "timer.c"

#endif