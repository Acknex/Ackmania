#ifndef _MENU_H_
#define _MENU_H_

#define MENU_DEF_THRESHOLD 50

/**
 * Left "click" check
 */
#define MENU_DEF_LEFT (key_cul | (joy_raw.x < -(MENU_DEF_THRESHOLD)) | (joy2_raw.x < -(MENU_DEF_THRESHOLD)))

/**
 * Right "click" check
 */
#define MENU_DEF_RIGHT (key_cur | (joy_raw.x > (MENU_DEF_THRESHOLD)) | (joy2_raw.x > (MENU_DEF_THRESHOLD)))

/**
 * Action "click" check
 */
#define MENU_DEF_ACTION (key_space | key_enter | joy_1 | joy2_1)

/**
 * Back "click" check
 */
#define MENU_DEF_BACK (key_esc| key_bksp | joy_2 | joy2_2)

/**
 * Globales Menü-Struct für Callbacks und Einstellungen.
 */
typedef struct { 
	/**
	 * Wird aufgerufen, wenn ein Level gestartet werden soll.
	 */
	void onLevelStart(int levelId);	
	
	/**
	 * Wird aufgerufen, wenn die Credits angezeigt werden sollen.
	 */
	void onCredits(void);
	
	/**
	 * Wird aufgerufen, wenn das Spiel beendet werden soll.
	 */
	void onExit(void);
	
	/**
	 * Definiert, ob das Menü sich selbst schließt, wenn ein Callback
	 * aufgerufen werden kann (!= NULL).
	 */
	BOOL closeOnCallback;
} Menu;

Menu menu;

/**
 * Initialisiert das Menü.
 */
void menu_init(int baseLayer);

/**
 * Öffnet das Menü.
 */
void menu_open();

/**
 * Schließt das Menü.
 */
void menu_close();

bool g_menuOpenedFirst = true;

SOUND* g_sndMenuOpen1st = "menu_open_1st.ogg";
SOUND* g_sndMenuOpenNext = "menu_open_next.ogg";

#include "menu.c"

#endif // #ifndef _MENU_H_
