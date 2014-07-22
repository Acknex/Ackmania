#ifndef items_h
#define items_h

// Typen von Items
#define ITEM_NONE							0
#define ITEM_FLASH						1
#define ITEM_AIM_ROCKET					2
#define ITEM_ROCKET						3
#define ITEM_GRAVE						4
#define ITEM_MUSHROOM					5
#define ITEM_BADASS_ROCKET				6
#define ITEM_TURBO						7

// Modellnamen der Items
#define ITEM_GRAVE_MODEL				"item_grave.mdl"
#define ITEM_ROCKET_MODEL				"item_rocket_red.mdl"
#define ITEM_AIMING_ROCKET_MODEL		"item_rocket_green.mdl"
#define ITEM_TURBO_MODEL				"item_turbo.mdl"
#define ITEM_BADASS_ROCKET_MODEL		"item_bomb.mdl"
#define ITEM_MUSHROOM_MODEL			"item_mushroom.mdl"
#define ITEM_FLASH_MODEL				"item_flash.mdl"

// Flage f�r die Items
#define IS_TRAP							FLAG3
#define SPIKE_TIMER_UP					3
#define SPIKE_TIMER_DOWN				1
#define SPIKE_SPEED						20
#define ROCKET_SPEED						20
#define ROCKET_X_OFFSET					300
#define ROCKET_LIFE_FRAMES				450
#define BADASS_ROCKET_LIFE_FRAMES	1000

// Sounds f�r Interaktion mit Items
SOUND* sndCollectA4Cube					= "collect.wav";
SOUND* sndGotNewItem						= "newitem.wav";

// Sounds f�r Items
SOUND* sndSpikesDown						= "spikes01.wav";
SOUND* sndSpikesUp						= "spikes02.wav";
SOUND* sndGraveCollision				= "graveCollision.ogg";
SOUND* sndRocketFire						= "rocketShoot1.ogg";
SOUND* sndAimingRocketFire				= "rocketShoot2.ogg";
SOUND* sndAimingBadassRocketFire		= "spikes01.wav";
SOUND* sndRocketHit						= "rocketExplode.ogg";
SOUND* sndTurboStart						= "jetpackloop.wav";
SOUND* sndMushroomStart					= "mushroom.wav";
SOUND* sndFlashStart						= "flash.ogg";
SOUND* sndItemShuffle					= "shuffle.wav";

// Bitmaps f�r Effekte
BMAP* bmapRocketSmoke					= "smoke.tga";
BMAP* bmapStar								= "star.tga";

// Gib das aktuelle Item des Spielers zur�ck
var get_current_item_id();

// Rotiert, kann eingesammelt werden.
// Bringt Punkte. Bei 10 Punkten -> Sofortiger Speedboost.
void a4_cube();

// Wird eingesammelt und vergibt ein Zufallsitem, falls noch
// keins im Slot ist. Verpufft, und wird 3 Sekunden sp�ter gerespawned.
void item();

// Verhalten eines Grabsteins (Falle)
void grave();

// Legt ein A9-Grab ab, das Fahrer ausbremst, wenn sie dr�berfahren
void plant_grave(ENTITY* driver);

// Schie�t eine Rakete ab
void shoot_rocket(ENTITY* driver);

// OPTIONAL: Schie�t eine leicht zielgelenkte Rakete ab
void shoot_aiming_rocket(ENTITY* driver);

// Beschleunigt den Spieler f�r 3 Sekunden
// auf 1,4(?)-fache Geschwindigkeit
void use_turbo(ENTITY* driver);

// Spikes, die aus dem Boden fahren und den Spieler
// ausbremsen. Werden bei Ber�hrung f�r 5 Sekunden inaktiviert.
void spikes();

// OPTIONAL: Schie�t einen (Lotti-)Ghost auf den 1. Spieler
// (wenn man es nicht selber ist, sonst auf den letzten).
// Slashed den Getroffenen mit einem LOTTI-Schrei f�r 3 Sekunden
void shoot_badass_aiming_rocket(ENTITY* driver);

// Macht den Spieler gr��er und er kann andere �berfahren,
// die nicht auch gro� sind
void start_mushroom(ENTITY* driver);

// Macht alle Spieler klein und langsamer (5 Sekunden)
// bis auf den Sch�tzen
void start_flash(ENTITY* driver);


#include "items.c"

#endif /* items_h */