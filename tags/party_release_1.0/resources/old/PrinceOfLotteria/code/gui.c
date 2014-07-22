#ifndef _GUI_C_
#define _GUI_C_

#include "player.h"

void gui_init() {
	panJetpackBorder = pan_create("", 2);
	panJetpackBorder.bmap = bmapJetpackColor;
	panJetpack = pan_create("", 3);
	panJetpack.bmap = bmapJetpackBorder;
	
	panHeart1 = pan_create("", 2);
	panHeart2 = pan_create("", 2);
	panHeart3 = pan_create("", 2);
	panHeart1.bmap = bmapZorroLife;
	panHeart2.bmap = bmapZorroLife;
	panHeart3.bmap = bmapZorroLife;
	txtLifes = txt_create(1, 2);
	set(txtLifes, OUTLINE);
	txtLifes.font = font_create("Arial#32b");
		
	panA4Cube = pan_create("", 2);
	panA4Cube.bmap = bmapA4Cube;
	txtA4Cube = txt_create(1, 3);
	set(txtA4Cube, OUTLINE);
	txtA4Cube.font = font_create("Arial#32b");
	
	txtGameOver = txt_create(1, 10);
	set(txtGameOver, OUTLINE | CENTER_X | CENTER_Y);
	txtGameOver.font = font_create("Arial#90b");
	vec_set(txtGameOver.blue, vector(0,0,255));
	str_cpy((txtGameOver.pstring)[0], "Game Over");
}

//fix by firo - start
void gui_reinit()
{
	gui_hide();
}
//fix by firo - end

void gui_hide() {
	reset(panJetpackBorder, SHOW);
	reset(panJetpack, SHOW);
	reset(panHeart1, SHOW);
	reset(panHeart2, SHOW);
	reset(panHeart3, SHOW);
	reset(panA4Cube, SHOW);
	reset(txtA4Cube, SHOW);
	reset(txtLifes, SHOW);
}

void gui_show() {
	if (flying_man) { //fix by firo
		set(panJetpackBorder, SHOW);
		set(panJetpack, SHOW);
	}
	set(panHeart1, SHOW);
	set(panHeart2, SHOW);
	set(panHeart3, SHOW);
	set(panA4Cube, SHOW);
	set(txtA4Cube, SHOW);
	set(txtLifes, SHOW);
}

// In achivements.h --> wieviele achivements erreicht

void gui_update_jetpack(var _percentage) {
	if (panJetpack != NULL) {
		if (flying_man) {
			panJetpack.size_y = bmap_height(bmapJetpackColor) - (bmap_height(bmapJetpackColor) / 100 * _percentage);
			set(panJetpack, SHOW);
			set(panJetpackBorder, SHOW);
		} else {
			reset(panJetpack, SHOW);
			reset(panJetpackBorder, SHOW);			
		}
	}
}

void gui_update_hearts() {
	if (player != NULL) {
		if (player.PL_HEALTH > 0) {
			set(panHeart3, SHOW);
		} else {
			reset(panHeart3, SHOW);
		}
		if (player.PL_HEALTH > 1) {
			set(panHeart2, SHOW);
		} else {
			reset(panHeart2, SHOW);
		}
		
		if (player.PL_HEALTH > 2) {
			set(panHeart1, SHOW);
		} else {
			reset(panHeart1, SHOW);
		}
		
		if (txtLifes != NULL) {
			str_for_num((txtLifes.pstring)[0], nPlayerLifes);
		}
	}
}

void gui_update_a4() {
	if (panA4Cube != NULL) {
		set(panA4Cube, SHOW);
		if (player != NULL) {
			if (txtA4Cube != NULL) {
				set(txtA4Cube, SHOW);
				str_for_num((txtA4Cube.pstring)[0], player.PL_A4_COUNT);
			}
		}
	}
}

void gui_start() {
	
	if (panJetpack == NULL) {
		gui_init();
	}
	
	panJetpackBorder.pos_x = 5;
	panJetpackBorder.pos_y = 5;
	panJetpack.pos_x = 5;
	panJetpack.pos_y = 5;
	
	panHeart1.pos_x = screen_size.x - 5 - 3*bmap_width(bmapZorroLife) - 3 * 2;
	panHeart2.pos_x = screen_size.x - 5 - 2*bmap_width(bmapZorroLife) - 2 * 2;
	panHeart3.pos_x = screen_size.x - 5 - 1*bmap_width(bmapZorroLife) - 1 * 2;
	panHeart1.pos_y = 5;
	panHeart2.pos_y = 5;
	panHeart3.pos_y = 5;
	txtLifes.pos_x = panHeart1.pos_x - 20;
	txtLifes.pos_y = 5;
	
	panA4Cube.pos_x = (screen_size.x / 2) - (bmap_width(bmapA4Cube) / 2);
	panA4Cube.pos_y = 5;
	txtA4Cube.pos_x = panA4Cube.pos_x - 15;
	txtA4Cube.pos_y = 10;
	
	txtGameOver.pos_x = screen_size.x / 2;
	txtGameOver.pos_y = screen_size.y / 2;	
}

#endif