#ifndef _GUI_H_
#define _GUI_H_

BMAP* bmapJetpackColor  = "..\\images\\jetpack.tga";
BMAP* bmapJetpackBorder = "..\\images\\jetpackBorder.tga";
BMAP* bmapZorroLife     = "..\\images\\zorrolifes.tga";
BMAP* bmapA4Cube        = "..\\images\\a4cube.tga";

PANEL* panJetpack = NULL;
PANEL* panJetpackBorder = NULL;

PANEL* panHeart1  = NULL;
PANEL* panHeart2  = NULL;
PANEL* panHeart3  = NULL;
TEXT* txtLifes    = NULL;

PANEL* panA4Cube  = NULL;
TEXT* txtA4Cube   = NULL;

TEXT* txtGameOver = NULL;

void gui_start();
void gui_hide();
void gui_update_jetpack(var _percentage);
void gui_update_hearts();
void gui_update_a4();

#include "gui.c"

#endif