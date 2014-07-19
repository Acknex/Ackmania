/*
 *******************************************************************************
 * hud.c
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
 * for short descriptions see comments in hud.h
 *
 *******************************************************************************
 */
 
#include <windows.h>
#include "startgrid.h"
#include "timer.h"

#define HUD_LAYER 5
#define HUD_FNTTIME_SIZE 80
#define HUD_FNTRANK_SIZE 150

#define HUD_PANTIME_POSX 		(HUD_FNTTIME_SIZE * 7)
#define HUD_PANTIME_POSY 		(HUD_FNTTIME_SIZE * 0.4)
#define HUD_PANTIME_OFFSX_SEC (HUD_FNTTIME_SIZE * 2)
#define HUD_PANTIME_OFFSX_HUN (HUD_FNTTIME_SIZE * 2.6)
#define HUD_PANRANK_POSX 		(HUD_FNTTIME_SIZE * 0.2) 
#define HUD_PANRANK_POSY 		(HUD_FNTTIME_SIZE * 2)
#define HUD_PANRANK_OFFSX 		(HUD_FNTRANK_SIZE * 0.9) 


PANEL* panLaps;
PANEL* panTime;
PANEL* panRank;
FONT* fntTime;
FONT* fntRank;


var vTimeMinutes;
var vTimeSeconds;
var vTimeHundreds;
var vRank;
var vHudScale = 0;
STRING** strRank[5];


void scale_hud();
void update_hudrank();

 
void create_hud()
{
	AddFontResource("media//anudaw.ttf");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, (WPARAM)0, (LPARAM)0);
	draw_textmode("anudaw", 0, 70, 90);
	
	panLaps = pan_create(NULL, HUD_LAYER);
	
	/* timer panel */
	panTime = pan_create(NULL, HUD_LAYER);
	pan_setdigits(panTime, 0, 0, 0, "%2.0f", "*", 1, &vTimeMinutes);
	pan_setdigits(panTime, 0, 0, 0, "%2.0f", "*", 1, &vTimeSeconds);
	pan_setdigits(panTime, 0, 0, 0, "%2.0f", "*", 1, &vTimeHundreds);
	pan_setdigits(panTime, 0, 0, 0, "  '  ''  ", "*", 1, &vTimeHundreds);
	vec_set(panTime->blue, vector(255, 255, 255));
	panTime->alpha = 85;
	
	/* rank panel */
	panRank = pan_create(NULL, HUD_LAYER);
	pan_setdigits(panRank, 0, 0, 0, "%1.0f", "*", 1, &vRank);
	pan_setdigits(panRank, 0, 0, 0, "st", "*", 1, &vRank);
	panRank->alpha = 85;	
	strRank[0] = str_create("");
	strRank[1] = str_create("st");
	strRank[2] = str_create("nd");
	strRank[3] = str_create("rd");
	strRank[4] = str_create("th");
	
	//update_hud();
	panTime->flags |= TRANSLUCENT|SHOW;
	panRank->flags |= TRANSLUCENT|SHOW;
reset_timer();//temp

	wait(1);
	update_hud();
}

void remove_hud()
{
	RemoveFontResource("media//anudaw.ttf");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, (WPARAM)0, (LPARAM)0);
	
	ptr_remove(panLaps);
	ptr_remove(panTime);
	ptr_remove(panRank);
}

void update_hud()
{
	var vHudScaleOld;
	var vRankOld;
	
	/* timer */
	vTimeHundreds = timer_getHundreds();
	vTimeSeconds = timer_getSeconds();
	vTimeMinutes = timer_getMinutes();

	/* rescale HUD if needed */
	vHudScaleOld = vHudScale;
   vHudScale = screen_size.y / 1200;	
   if (vHudScale != vHudScaleOld)
   {
   	scale_hud();
   }

	/* update rank */
   vRankOld = vRank;
   //vRank = get_kart_rank (0);
	if (vRank != vRankOld)
	{
		update_hudrank();
   }
   	
}


void scale_hud()
{
	STRING* strTemp = "#64";
	var vPosX;
	var vPosY;
	
	str_printf(strTemp, "anudaw#%ii", (int)(HUD_FNTTIME_SIZE * vHudScale));
	ptr_remove(fntTime);
	fntTime = font_create(strTemp);
	vPosY = HUD_PANTIME_POSY * vHudScale;
	vPosX = screen_size.x - (HUD_PANTIME_POSX * vHudScale);
	pan_setdigits(panTime, 4, vPosX, vPosY, "  '  ''  ", fntTime, 1, &vTimeHundreds);
	pan_setdigits(panTime, 1, vPosX, vPosY, "%02.0f", fntTime, 1, &vTimeMinutes);
	vPosX += HUD_PANTIME_OFFSX_SEC * vHudScale;
	pan_setdigits(panTime, 2, vPosX, vPosY, "%02.0f", fntTime, 1, &vTimeSeconds);
	vPosX += HUD_PANTIME_OFFSX_HUN * vHudScale;
	pan_setdigits(panTime, 3, vPosX, vPosY, "%02.0f", fntTime, 1, &vTimeHundreds);	

	str_printf(strTemp, "anudaw#%ii", (int)(HUD_FNTRANK_SIZE * vHudScale));
	ptr_remove(fntRank);
	fntRank = font_create(strTemp);
	vPosY = screen_size.y - (HUD_PANRANK_POSY * vHudScale);
	vPosX = HUD_PANRANK_POSX * vHudScale;
	pan_setdigits(panRank, 1, vPosX, vPosY, "%1.0f", fntRank, 1, &vRank);
	update_hudrank();

}

void update_hudrank()
{
	var vPosX;
	var vPosY;
	vPosY = screen_size.y - (HUD_PANRANK_POSY * vHudScale);
	vPosX = (HUD_PANRANK_POSX + HUD_PANRANK_OFFSX) * vHudScale;
	pan_setdigits(panRank, 2, vPosX, vPosY, (strRank[vRank]), fntTime, 1, &vRank);
}



