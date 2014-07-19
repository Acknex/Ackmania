/*
 *******************************************************************************
 * hud.h
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

#define HUD_LAYER 5
#define HUD_FNTTIME_SIZE 50

#define HUD_PANTIME_POSX 		(HUD_FNTTIME_SIZE * 7)
#define HUD_PANTIME_POSY 		(HUD_FNTTIME_SIZE * 0.4)
#define HUD_PANTIME_OFFSX_SEC (HUD_FNTTIME_SIZE * 2)
#define HUD_PANTIME_OFFSX_HUN (HUD_FNTTIME_SIZE * 2.6)
#define HUD_PANRANK_POSX 		HUD_FNTTIME_SIZE
#define HUD_PANRANK_POSY 		(HUD_FNTTIME_SIZE * 2)


PANEL* panLaps;
PANEL* panTime;
PANEL* panRank;

var vTimeTicks;
var vTimeMinutes;
var vTimeSeconds;
var vTimeHundreds;
var vRank;
var vHudScale = 0;
STRING** strRank[5];

void check_hudscale();
void scale_hud();

 
void create_hud()
{
	AddFontResource("media//anudaw.ttf");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, (WPARAM)0, (LPARAM)0);
	draw_textmode("anudaw", 0, 70, 90);
	
	panLaps = pan_create(NULL, HUD_LAYER);
	
	panTime = pan_create(NULL, HUD_LAYER);
	pan_setdigits(panTime, 0, 0, 0, "%2.0f", "*", 1, &vTimeMinutes);
	pan_setdigits(panTime, 0, 0, 0, "%2.0f", "*", 1, &vTimeSeconds);
	pan_setdigits(panTime, 0, 0, 0, "%2.0f", "*", 1, &vTimeHundreds);
	pan_setdigits(panTime, 0, 0, 0, "  '  ''  ", "*", 1, &vTimeHundreds);
	vec_set(panTime->blue, vector(255, 255, 255));
	panTime->alpha = 85;
	
	panRank = pan_create(NULL, HUD_LAYER);
	pan_setdigits(panRank, 0, 0, 0, "%1.0f", "*", 1, &vRank);
	pan_setdigits(panRank, 0, 0, 0, "st", "*", 1, &vRank);
	panRank->alpha = 85;

	vTimeTicks = 0;	
	

	strRank[0] = str_create("");
	strRank[1] = str_create("st");
	strRank[2] = str_create("nd");
	strRank[3] = str_create("rd");
	strRank[4] = str_create("th");
	
	//update_hud();
	panTime->flags |= TRANSLUCENT|SHOW;

	wait(1);
	check_hudscale();
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
	
	vTimeTicks += time_step;
	vTimeHundreds = integer(((vTimeTicks / 16) * 100) % 100);
	vTimeSeconds = integer((vTimeTicks / 16) % 60);
	vTimeMinutes = integer((vTimeTicks / (60 * 16)) % 60);
	check_hudscale();
   	//draw_text("Lap 1/3", 10, 50, vector(255,255,255));
   	
}

void check_hudscale()
{
	var vHudScaleOld;
	
	vHudScaleOld = vHudScale;
   vHudScale = screen_size.y / 1200;	
   if (vHudScale != vHudScaleOld)
   {
   	scale_hud();
   }
}

void scale_hud()
{
	STRING* strTemp = "#64";
	var vPosX;
	var vPosY;
	FONT* fntTime;
	
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

	vPosY = screen_size.y - (HUD_PANRANK_POSY * vHudScale);
	vPosX = HUD_PANRANK_POSX * vHudScale;
	//pan_setdigits(panRank, 2, vPosX, vPosY, "%s", fntTime, 1, strTemp);
	pan_setdigits(panRank, 1, vPosX, vPosY, "%1.0f", fntTime, 1, &vRank);

}



