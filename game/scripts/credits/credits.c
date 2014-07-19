#ifndef _CREDITS_C_
#define _CREDITS_C_

#define TEST_DEBUG

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
	#define PRAGMA_PATH "../../textures/"

	#include <acknex.h>
	#include <windows.h>
	#include <default.c>
	
	#define BOOL int
#endif // TEST_DEBUG

#include "credits.h"

var _credits_music = 0;

/**
 * Initializes the credits.
 */
void credits_init()
{
	memset(&credits, 0, sizeof(Credits));
	wait(1);
	while(1) {
	
	
	
		wait(1);
	}
}

/**
 * Starts the credits.
 */
void credits_start()
{
	if(_credits_music != 0) {
		return;
	}
	level_load("creditsTerrain.hmp");	
	ent_createlayer("cosmo_f02+6.jpg", SKY | CUBE, 10);
	
#ifdef TEST_DEBUG
	_credits_music = media_play("../../media/outro-demo.ogg", NULL, 100);
#else
	_credits_music = media_play("media\\outro-demo.ogg", NULL, 100);
#endif
}

/**
 * Stops the credits.
 */
void credits_stop()
{
	if(_credits_music == 0) {
		return;
	}
	if(credits.onCreditsEnd != NULL) {
		void tmp();
		tmp = credits.onCreditsEnd;
		tmp();
	}	
	media_stop(_credits_music);
	_credits_music = 0;
}


#ifdef TEST_DEBUG

function main()
{
	credits_init();
	
	
	on_space = credits_start;
}


#endif

#endif // #ifndef _CREDITS_C_