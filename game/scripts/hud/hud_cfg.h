/*
 *******************************************************************************
 * hud_cfg.h
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
 * configuration script for head up display
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef HUD_CFG_H
#define HUD_CFG_H
 
#define HUD_LAYER 5
#define HUD_FNTTIME_SIZE 80
#define HUD_FNTRANK_SIZE 150

#define HUD_PANLAPS_POSX		(HUD_FNTTIME_SIZE * 0.2) 
#define HUD_PANLAPS_POSY 		(HUD_FNTTIME_SIZE * 0.4)
#define HUD_PANLAPS_OFFSX		(HUD_FNTTIME_SIZE * 1.2) 
#define HUD_PANLAPS_OFFSY 		(HUD_FNTTIME_SIZE * 1.1)
#define HUD_PANLAPS_OFFSY_DIV	(HUD_FNTTIME_SIZE * 0.2)

#define HUD_PANTIME_POSX 		(HUD_FNTTIME_SIZE * 7)
#define HUD_PANTIME_POSY 		(HUD_FNTTIME_SIZE * 0.4)
#define HUD_PANTIME_OFFSX_SEC	(HUD_FNTTIME_SIZE * 2)
#define HUD_PANTIME_OFFSX_HUN	(HUD_FNTTIME_SIZE * 2.6)

#define HUD_PANRANK_POSX 		(HUD_FNTTIME_SIZE * 0.2) 
#define HUD_PANRANK_POSY 		(HUD_FNTTIME_SIZE * 2)
#define HUD_PANRANK_OFFSX 		(HUD_FNTRANK_SIZE * 0.9) 

#define HUD_PANLAPS_POSY 		(HUD_FNTTIME_SIZE * 0.4)
#define HUD_PANITEM_SIZE		128
#define HUD_PANITEM_BORDERSIZE	144
#define HUD_PANITEM_BORDERWIDTH	((HUD_PANITEM_BORDERSIZE - HUD_PANITEM_SIZE) * 0.5)
#define HUD_PANITEM_SCALE		1.0

#define HUD_PANCUBE_SIZE		280
#define HUD_PANCUBE_SCALE		1.0
#define HUD_PANCUBE_POSY 		(HUD_FNTTIME_SIZE * 2)

#define HUD_TXTMSG_FLAP_TIME	48
#endif