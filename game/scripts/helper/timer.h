/*
 *******************************************************************************
 * timer.h
 * Creation date: 19.07.2014
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date$
 * $Revision$
 * $Author$
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