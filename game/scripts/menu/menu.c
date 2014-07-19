#ifndef _MENU_C_
#define _MENU_C_

//#define TEST_DEBUG

#ifdef TEST_DEBUG

	#define PRAGMA_PATH "../../panels/menu/"
	#define PRAGMA_PATH "../../shaders/menu/"
	#define PRAGMA_PATH "../../sounds/menu/"
	#define PRAGMA_PATH "../circuits/"
	#define PRAGMA_PATH "../engine/"
	#define PRAGMA_PATH "../../levels/"
	#define PRAGMA_PATH "../../levels/test/"
	#define PRAGMA_PATH "../../models/"
	#define PRAGMA_PATH "../../music/"

	#include <acknex.h>
	#include <windows.h>
	#include <default.c>
	
	#define BOOL int
#endif // TEST_DEBUG

#include "menu.h"
#include "circuitinfo.h"

BOOL _menu_visible = false;
BOOL _menu_trackSelectionActive = false;
int _menu_selection = 0;
int _menu_level_count = 0;
LevelChoice *_menu_levels = NULL;
STRING *_menu_currentChoice = "< Select Level >";

SOUND *_menu_switchSound = "menu_click.wav";
SOUND *_menu_triggerSound = "menu_action.wav";
SOUND *_menu_music = "menu_6db.wav";
BMAP *_menu_baseflag = "menu_flag.png";

BMAP *_menu_trackIcons[1024];

PANEL *_menu_background = {
	bmap = "menu_background.png";
}

PANEL *_menu_bigheader = {
	bmap = "menu_header.png";
}

PANEL *_menu_course = {
	size_x = 256;
	size_y = 256;
}

MATERIAL *_menu_flagshader = {
	effect =
"technique Sepia {
	pass p1 {
		AlphaBlendEnable = false;
		ZEnable = true;
		ZFunc = LessEqual;
		CullMode = None;
	}
}";
	flags = PASS_SOLID;
}

TEXT *_menu_selection_txt = {
	font = "menu_ackfont_large.png";
	string = (_menu_currentChoice);
	flags = CENTER_X | CENTER_Y;
}

ENTITY *_menu_flag = {
	pan = 80;
	type = "flag.mdl";
	material = _menu_flagshader;
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
	layer_sort(_menu_course,         baseLayer + 4);
	
	proc_mode = PROC_LATE;
	
	wait(1);
	
	int lastKeyLeft = 0;
	int lastKeyRight = 0;
	int lastKeyAction = 0;
	int lastKeyBack = 0;
	
	int num = getNumCircuits();
	int i;
	for(i = 0; i < num; i++) {
		_menu_trackIcons[i] = bmap_create(getCircuitPicFilenameStr(i));
	}
	
	var menuMusic = snd_loop(_menu_music, 80, 0);
	snd_pause(menuMusic);
	
	while(1) {
		lastKeyLeft = MENU_DEF_LEFT;
		lastKeyRight = MENU_DEF_RIGHT;
		lastKeyAction = MENU_DEF_ACTION;
		lastKeyBack = MENU_DEF_BACK;
		
		wait(1);
		
		ent_animate(_menu_flag, "Wave", 0.8 * total_ticks, ANM_CYCLE);
		
		if(_menu_visible) {
			if(!snd_playing(menuMusic)) {
				snd_start(menuMusic);
			}
			_menu_background.flags |= SHOW;
			_menu_background.scale_x = screen_size.x / bmap_width(_menu_background.bmap);
			_menu_background.scale_y = screen_size.y / bmap_height(_menu_background.bmap);
		
			_menu_bigheader.flags |= SHOW;
			_menu_bigheader.pos_x = 0.5 * screen_size.x - 0.5 * bmap_width(_menu_bigheader.bmap);
			_menu_bigheader.pos_y = 0.25 * screen_size.y - 0.5 * bmap_height(_menu_bigheader.bmap);
			
			_menu_flag.flags2 |= SHOW;
			VECTOR pos;
			pos.x = 0.5 * screen_size.x;
			pos.y = 0.75 * screen_size.y;
			pos.z = 1000;
			
			rel_for_screen(pos, camera);
			
			vec_set(_menu_flag.x, pos);
			
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
			
			if(_menu_trackSelectionActive) {
				// Cycle over levels
				if(_menu_selection >= getNumCircuits()) {
					_menu_selection = 0;
				} else if(_menu_selection < 0) {
					_menu_selection = getNumCircuits() - 1;
				}
				
				_menu_course.flags |= VISIBLE;
				_menu_course.pos_x = 0.5 * screen_size.x - 128;
				_menu_course.pos_y = 0.5 * screen_size.y - 128;
				_menu_course.bmap = _menu_trackIcons[_menu_selection];
			
				_menu_selection_txt.flags |= SHOW;
				_menu_selection_txt.pos_x = 0.5 * screen_size.x;
				_menu_selection_txt.pos_y = _menu_course.pos_y + 256 + 32;
				
				str_cpy(_menu_currentChoice, "< ");
				str_cat(_menu_currentChoice, getCircuitTitleStr(_menu_selection));
				str_cat(_menu_currentChoice, " >");
				
				
				// Back
				if(MENU_DEF_BACK && MENU_DEF_BACK != lastKeyBack) {
					_menu_selection = 0;
					_menu_trackSelectionActive = false;
					snd_play(_menu_switchSound, 100, 0);
				}
				
				if(MENU_DEF_ACTION && MENU_DEF_ACTION != lastKeyAction ) {
					snd_play(_menu_triggerSound, 100, 0);
					
					if(menu.onLevelStart != NULL) {
						void tmp(int i);
						tmp = menu.onLevelStart;
						tmp(_menu_selection);
						
						if(menu.closeOnCallback) {
							menu_close();
						}
						continue; // Refine the menu
					}
				}
			} else {
				_menu_course.flags &= ~SHOW;
			
				// Cycle over main menu
				if(_menu_selection > 2) {
					_menu_selection = 0;
				} else if(_menu_selection < 0) {
					_menu_selection = 2;
				}
				
				_menu_selection_txt.flags |= SHOW;
				_menu_selection_txt.pos_x = 0.5 * screen_size.x;
				_menu_selection_txt.pos_y = 0.5 * screen_size.y;
				
				// Action
				if(MENU_DEF_ACTION && MENU_DEF_ACTION != lastKeyAction ) {
					snd_play(_menu_triggerSound, 100, 0);
					
					BOOL hadCallback = false;
					switch(_menu_selection) {
						case 0:
							_menu_trackSelectionActive = true;
							_menu_selection = 0;
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
			}
		} else {
			if(snd_playing(menuMusic)) {
				snd_pause(menuMusic);
			}
			_menu_background.flags &= ~SHOW;
			_menu_bigheader.flags &= ~SHOW;
			_menu_flag.flags2 &= ~SHOW;
			_menu_selection_txt.flags &= ~SHOW;
			_menu_course.flags &= ~SHOW;
		}
	}
}

/**
 * Öffnet das Menü.
 */
void menu_open()
{
	_menu_visible = true;

	// play sound on entry
	if (g_menuOpenedFirst) {
	   snd_play(g_sndMenuOpen1st, 100, 0);
	   g_menuOpenedFirst = false;
	} else {
	   snd_play(g_sndMenuOpenNext, 100, 0);
	}

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
	on_esc = NULL;
	menu_init(15);
	
	//menu.closeOnCallback = true;
	menu.onExit = exitCallback;
	menu.onCredits = creditsCallback;
	menu.onLevelStart = selectLevelCallback;
	
	menu_open();
}

#endif // TEST_DEBUG

#endif // #ifndef _MENU_C_
