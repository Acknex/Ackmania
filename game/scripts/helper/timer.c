/*
 *******************************************************************************
 * timer.c
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

var vTimeTicks;

void timer_startup()
{
	reset_timer();
	while(1)
	{
		vTimeTicks += time_step;
		wait(1);
	}
}

void reset_timer()
{
	vTimeTicks = 0;
}

var timer_getHundreds()
{
	return integer(((vTimeTicks / 16) * 100) % 100);
}

var timer_getSeconds()
{
	return integer((vTimeTicks / 16) % 60);
}

var timer_getMinutes()
{
	return integer((vTimeTicks / (60 * 16)) % 60);
}