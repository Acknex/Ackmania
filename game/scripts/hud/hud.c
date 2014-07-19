/*
 *******************************************************************************
 * hud.c
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
 * for short descriptions see comments in hud.h
 *
 *******************************************************************************
 */
 
#include <windows.h>
#include "hud_cfg.h"
#include "startgrid.h"
#include "timer.h"
#include "items.h"


PANEL* panLaps;
PANEL* panTime;
PANEL* panRank;
PANEL* panItem;
FONT* fntTime;
FONT* fntRank;


var vTimeMinutes;
var vTimeSeconds;
var vTimeHundreds;
var vRank;
var vCurrentLap;
var vTotalLaps;
var vHudScale = 0;
var vHudCreated = 0;
var vItemX;
var vItemY;
var vItemId;
STRING** strRank[5];
BMAP* bmapItem = "item_strip.tga";


void scale_hud();
void update_hudrank();
 
void create_hud()
{
	AddFontResource("media//anudaw.ttf");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, (WPARAM)0, (LPARAM)0);
	draw_textmode("anudaw", 0, 70, 90);
	
	/* laps panel */
	panLaps = pan_create(NULL, HUD_LAYER);
	pan_setdigits(panLaps, 0, 0, 0, "%1.0f", "*", 1, &vCurrentLap);
	pan_setdigits(panLaps, 0, 0, 0, "%1.0f", "*", 1, &vTotalLaps);
	pan_setdigits(panLaps, 0, 0, 0, "/", "*", 1, &vCurrentLap);
	panLaps->alpha = 85;
	panLaps->flags |= TRANSLUCENT;
	
	/* timer panel */
	panTime = pan_create(NULL, HUD_LAYER);
	pan_setdigits(panTime, 0, 0, 0, "%2.0f", "*", 1, &vTimeMinutes);
	pan_setdigits(panTime, 0, 0, 0, "%2.0f", "*", 1, &vTimeSeconds);
	pan_setdigits(panTime, 0, 0, 0, "%2.0f", "*", 1, &vTimeHundreds);
	pan_setdigits(panTime, 0, 0, 0, "  '  ''  ", "*", 1, &vTimeHundreds);
	panTime->alpha = 85;
	panTime->flags |= TRANSLUCENT;
	
	/* rank panel */
	panRank = pan_create(NULL, HUD_LAYER);
	pan_setdigits(panRank, 0, 0, 0, "%1.0f", "*", 1, &vRank);
	pan_setdigits(panRank, 0, 0, 0, "st", "*", 1, &vRank);
	panRank->alpha = 85;	
	panRank->flags |= TRANSLUCENT;
	strRank[0] = str_create("");
	strRank[1] = str_create("st");
	strRank[2] = str_create("nd");
	strRank[3] = str_create("rd");
	strRank[4] = str_create("th");
	
	/* item panel */
	panItem = pan_create(NULL, HUD_LAYER);
	panItem->bmap = bmap_create("item_border.tga");
	pan_setwindow (panItem, 0, HUD_PANITEM_BORDERWIDTH, HUD_PANITEM_BORDERWIDTH, HUD_PANITEM_SIZE, HUD_PANITEM_SIZE, bmapItem, &vItemX, &vItemY);
	panItem->flags |= TRANSLUCENT;
	
	
	wait(1);
	update_hud();
	vHudCreated = 1;
	show_hud();	
}

void remove_hud()
{
	var i;
	vHudCreated = 0;
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, (WPARAM)0, (LPARAM)0);
	
	ptr_remove(panLaps);
	ptr_remove(panTime);
	ptr_remove(panRank);
	ptr_remove(panItem);
	ptr_remove(panItem->bmap);
	for (i = 0; i < 5; i++)
	{
		ptr_remove(strRank[i]);
	}
	RemoveFontResource("media//anudaw.ttf");
}

void update_hud()
{
	var vHudScaleOld;
	var vRankOld;
	BMAP* bmapItemOld;
	
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
	vRankOld = vRank + 1; //TEMP
	//vRank = clamp(get_kart_rank (0), 1, 4);
	vRank = 3; //TEMP
	if (vRank != vRankOld)
	{
		update_hudrank();
	}
	
	/* update laps */
	//vCurrentLap = 2;
	//vTotalLaps = 5;   	

	/* update item slot */
	vItemId = get_current_item_id();
	vItemX = clamp(vItemId, 0, 7) * HUD_PANITEM_SIZE;
	vItemY = 0;
}

void show_hud()
{
	if (vHudCreated != 0)
	{
		panLaps->flags |= SHOW;
		//panTime->flags |= SHOW;
		panRank->flags |= SHOW;
		panItem->flags |= SHOW;
	}
}

void hide_hud()
{
	if (vHudCreated != 0)
	{
		panLaps->flags &= ~SHOW;
		panTime->flags &= ~SHOW;
		panRank->flags &= ~SHOW;
		panItem->flags &= ~SHOW;
	}
}

void scale_hud()
{
	STRING* strTemp = "#64";
	var vPosX;
	var vPosY;
	
	/* fonts */
	str_printf(strTemp, "anudaw#%ii", (int)(HUD_FNTTIME_SIZE * vHudScale));
	ptr_remove(fntTime);
	fntTime = font_create(strTemp);
	str_printf(strTemp, "anudaw#%ii", (int)(HUD_FNTRANK_SIZE * vHudScale));
	ptr_remove(fntRank);
	fntRank = font_create(strTemp);
	
	/* laps panel */
	vPosX = HUD_PANLAPS_POSX * vHudScale;
	vPosY = HUD_PANLAPS_POSY * vHudScale;
	pan_setdigits(panLaps, 1, vPosX, vPosY, "%1.0f", fntTime, 1, &vCurrentLap);
	vPosY += HUD_PANLAPS_OFFSY_DIV * vHudScale;	
	pan_setdigits(panLaps, 3, vPosX, vPosY, "/", fntRank, 1, &vCurrentLap);
	vPosX = (HUD_PANLAPS_OFFSX + HUD_PANLAPS_POSX) * vHudScale;
	vPosY = (HUD_PANLAPS_OFFSY + HUD_PANLAPS_POSY) * vHudScale;
	pan_setdigits(panLaps, 2, vPosX, vPosY, "%1.0f", fntTime, 1, &vTotalLaps);
	
	/* timer panel */
	vPosY = HUD_PANTIME_POSY * vHudScale;
	vPosX = screen_size.x - (HUD_PANTIME_POSX * vHudScale);
	pan_setdigits(panTime, 4, vPosX, vPosY, "  '  ''  ", fntTime, 1, &vTimeHundreds);
	pan_setdigits(panTime, 1, vPosX, vPosY, "%02.0f", fntTime, 1, &vTimeMinutes);
	vPosX += HUD_PANTIME_OFFSX_SEC * vHudScale;
	pan_setdigits(panTime, 2, vPosX, vPosY, "%02.0f", fntTime, 1, &vTimeSeconds);
	vPosX += HUD_PANTIME_OFFSX_HUN * vHudScale;
	pan_setdigits(panTime, 3, vPosX, vPosY, "%02.0f", fntTime, 1, &vTimeHundreds);	

	/* rank panel */
	vPosY = screen_size.y - (HUD_PANRANK_POSY * vHudScale);
	vPosX = HUD_PANRANK_POSX * vHudScale;
	pan_setdigits(panRank, 1, vPosX, vPosY, "%1.0f", fntRank, 1, &vRank);
	update_hudrank();
	
	/* item panel */
	panItem->scale_x = HUD_PANITEM_SCALE * vHudScale;
	panItem->scale_y = HUD_PANITEM_SCALE * vHudScale;
	panItem->pos_x = (screen_size.x - HUD_PANITEM_BORDERSIZE * panItem->scale_x) * 0.5;
	panItem->pos_y = HUD_PANLAPS_POSY * vHudScale;
}

void update_hudrank()
{
	var vPosX;
	var vPosY;

	/* rank panel - special fix on rank change */
	vPosY = screen_size.y - (HUD_PANRANK_POSY * vHudScale);
	vPosX = (HUD_PANRANK_POSX + HUD_PANRANK_OFFSX) * vHudScale;
	pan_setdigits(panRank, 2, vPosX, vPosY, (strRank[vRank]), fntTime, 1, &vRank);
}


