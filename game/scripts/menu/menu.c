#ifndef _MENU_C_
#define _MENU_C_

#define TEST_DEBUG

#ifdef TEST_DEBUG

	#define PRAGMA_PATH "../../panels/menu/"
	#define PRAGMA_PATH "../../shaders/menu/"
	#define PRAGMA_PATH "../../sounds/menu/"

	#include <acknex.h>
	#include <windows.h>
	#include <default.c>
	
	#define BOOL int
#endif // TEST_DEBUG

#include "menu.h"

BOOL _menu_visible = false;
int _menu_selection = 0;
int _menu_level_count = 0;
LevelChoice *_menu_levels = NULL;
STRING *_menu_currentChoice = "< Select Level >";

SOUND *_menu_switchSound = "menu_click.wav";
SOUND *_menu_triggerSound = "menu_action.wav";
BMAP *_menu_baseflag = "menu_flag.png";

PANEL *_menu_background = {
	bmap = "menu_background.png";
}

PANEL *_menu_bigheader = {
	bmap = "menu_header.png";
}

PANEL *_menu_flag = {
	bmap = "menu_flag.png";
}

#ifdef DEBUG
MATERIAL *_menu_flagshader = {
	effect = "menu_flag.fx";
	flags = AUTORELOAD;
}
#else
MATERIAL *_menu_flagshader = {
	effect = "menu_flag.fx";
}
#endif

TEXT *_menu_selection_txt = {
	font = "menu_ackfont_large.png";
	string = (_menu_currentChoice);
	flags = CENTER_X | CENTER_Y;
}

/**
 * Initialisiert das Menü.
 */
void menu_init(int baseLayer)
{
	memset(&menu, 0, sizeof(Menu));
	
	layer_sort(_menu_background,     baseLayer + 0);
	layer_sort(_menu_flag,           baseLayer + 1);
	layer_sort(_menu_bigheader,      baseLayer + 2);
	layer_sort(_menu_selection_txt,  baseLayer + 3);
	
	proc_mode = PROC_LATE;
	
	wait(1);
	
	int lastKeyLeft = 0;
	int lastKeyRight = 0;
	int lastKeyAction = 0;
	
	while(1) {
		lastKeyLeft = MENU_DEF_LEFT;
		lastKeyRight = MENU_DEF_RIGHT;
		lastKeyAction = MENU_DEF_ACTION;
		
		wait(1);
		
		if(_menu_visible) {
			_menu_background.flags |= SHOW;
			_menu_background.scale_x = screen_size.x / bmap_width(_menu_background.bmap);
			_menu_background.scale_y = screen_size.y / bmap_height(_menu_background.bmap);
		
			_menu_bigheader.flags |= SHOW;
			_menu_bigheader.pos_x = 0.5 * screen_size.x - 0.5 * bmap_width(_menu_bigheader.bmap);
			_menu_bigheader.pos_y = 0.25 * screen_size.y - 0.5 * bmap_height(_menu_bigheader.bmap);
			
			_menu_flag.flags |= SHOW;
			_menu_flag.pos_x = 0.5 * screen_size.x - 0.5 * bmap_width(_menu_flag.bmap) + 91;
			_menu_flag.pos_y = 0.65 * screen_size.y - 0.5 * bmap_height(_menu_flag.bmap);
			
			_menu_flagshader.skill1 = floatv(total_ticks);
			bmap_process(_menu_flag.bmap, _menu_baseflag, _menu_flagshader);
			
			_menu_selection_txt.flags |= SHOW;
			_menu_selection_txt.pos_x = 0.5 * screen_size.x;
			_menu_selection_txt.pos_y = 0.5 * screen_size.y;
			
			// Left
			if(MENU_DEF_LEFT && MENU_DEF_LEFT != lastKeyLeft) {
				_menu_selection -= 1;
				snd_play(_menu_switchSound, 100, 0);
			}
			
			// Right
			if(MENU_DEF_RIGHT && MENU_DEF_RIGHT != lastKeyRight) {
				_menu_selection += 1;
				snd_play(_menu_switchSound, 100, 0);
			}
			
			// Action
			if(MENU_DEF_ACTION && MENU_DEF_ACTION != lastKeyAction ) {
				snd_play(_menu_triggerSound, 100, 0);
				
				BOOL hadCallback = false;
				switch(_menu_selection) {
					case 0:
						if(menu.onLevelStart != NULL) {
							void tmp(int i);
							tmp = menu.onLevelStart;
							tmp(0);
							hadCallback = true;
						}
						break;
					case 1:
						if(menu.onCredits != NULL) {
							void tmp();
							tmp = menu.onCredits;
							tmp();
							hadCallback = true;
						}
						break;
					case 2:
						if(menu.onExit != NULL) {
							void tmp();
							tmp = menu.onExit;
							tmp();
							hadCallback = true;
						}
						break;
				}
				if(hadCallback) {
					if(menu.closeOnCallback) {
						menu_close();
					}
					continue; // Refine the menu
				}
			}
			
			if(_menu_selection > 2) {
				_menu_selection = 0;
			} else if(_menu_selection < 0) {
				_menu_selection = 2;
			}
			
			switch(_menu_selection) {
				case 0:
					str_cpy(_menu_currentChoice, "< Select Level >");
					break;
				case 1:
					str_cpy(_menu_currentChoice, "< Credits >");
					break;
				case 2:
					str_cpy(_menu_currentChoice, "< Exit >");
					break;
				default:
					_menu_selection = 0;
					str_cpy(_menu_currentChoice, "< Select Level >");
					break;
			}
		} else {
			_menu_background.flags &= ~SHOW;
			_menu_bigheader.flags &= ~SHOW;
			_menu_flag.flags &= ~SHOW;
			_menu_selection_txt.flags &= ~SHOW;
		
		}
	}
}

/**
 * Öffnet das Menü.
 */
void menu_open()
{
	_menu_visible = true;
}

/**
 * Schließt das Menü.
 */
void menu_close()
{
	_menu_visible = false;
}

/**
 * Setzt die Levelauswahl.
 */
void menu_set_levels(int count, LevelChoice *choices)
{
	if(count > 0) {
		_menu_level_count = count;
		_menu_levels = choices;
	} else {
		_menu_level_count = 0;
		_menu_levels = NULL;
	}
}

#ifdef TEST_DEBUG

void exitCallback() {
	wait(1);
	error("quit!");
}

void creditsCallback() {
	wait(1);
	error("credits!");
}

void selectLevelCallback(int id) {
	wait(1);
	error(str_for_int(NULL, id));
}

function main()
{
	menu_init(15);
	
	//menu.closeOnCallback = true;
	menu.onExit = exitCallback;
	menu.onCredits = creditsCallback;
	menu.onLevelStart = selectLevelCallback;
	
	menu_open();
}

#endif // TEST_DEBUG

#endif // #ifndef _MENU_C_