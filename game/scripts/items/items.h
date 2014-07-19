#ifndef items_h
#define items_h

// Typen von Items
#define ITEM_NONE						0
#define ITEM_GRAVE					1
#define ITEM_ROCKET					2
#define ITEM_AIM_ROCKET				3
#define ITEM_TURBO					4
#define ITEM_BADASS_ROCKET			5
#define ITEM_MUSHROOM				6

// Modellnamen der Items
#define ITEM_GRAVE_MODEL			"item_grave.mdl"
#define ITEM_ROCKET_MODEL			"item_rocket.mdl"
#define ITEM_AIM_ROCKET_MODEL		"item_aim_rocket.mdl"
#define ITEM_TURBO_MODEL			"item_turbo.mdl"
#define ITEM_BADASS_ROCKET_MODEL	"item_badass_rocket.mdl"
#define ITEM_MUSHROOM_MODEL		"item_mushroom.mdl"

// Flage f�r die Items
#define IS_TRAP						FLAG3
#define SPIKE_TIMER_UP				3
#define SPIKE_TIMER_DOWN			1
#define SPIKE_SPEED					20

// Sounds f�r Interaktion mit Items
SOUND* sndCollectItem					= "spikes01.wav";
SOUND* sndCollectA4Cube					= "spikes01.wav";
SOUND* sndGotNewItem						= "spikes01.wav";

// Sounds f�r Items
SOUND* sndSpikesDown						= "spikes01.wav";
SOUND* sndSpikesUp						= "spikes02.wav";
SOUND* sndGraveCollision				= "spikes01.wav";
SOUND* sndRocketFire						= "spikes01.wav";
SOUND* sndAimingRocketFire				= "spikes01.wav";
SOUND* sindAimingBadassRocketFire	= "spikes01.wav";
SOUND* sndRocketFly						= "spikes01.wav";
SOUND* sndTurboStart						= "spikes01.wav";
SOUND* sndMushroomStart					= "spikes01.wav";
SOUND* sndFlashStart						= "spikes01.wav";

/*COLOR* colExplosionYellow	= { red=247; green=255; blue=5; }
COLOR* colExplosionOrange	= { red=255; green=150; blue=0; }
COLOR* colExplosionRed		= { red=255; green=43; blue=425; }*/

// Rotiert, kann eingesammelt werden.
// Bringt Punkte. Bei 10 Punkten -> Sofortiger Speedboost.
void a4_cube();

// Wird eingesammelt und vergibt ein Zufallsitem, falls noch
// keins im Slot ist. Verpufft, und wird 3 Sekunden sp�ter gerespawned.
void item();

// Verhalten eines Grabsteins (Falle)
void grave();

// Legt ein A9-Grab ab, das Fahrer ausbremst, wenn sie dr�berfahren
void plant_grave();

// Schie�t eine Rakete ab
void shoot_rocket();

// OPTIONAL: Schie�t eine leicht zielgelenkte Rakete ab
void shoot_aiming_rocket();

// Beschleunigt den Spieler f�r 3 Sekunden
// auf 1,4(?)-fache Geschwindigkeit
void use_turbo();

// Spikes, die aus dem Boden fahren und den Spieler
// ausbremsen. Werden bei Ber�hrung f�r 5 Sekunden inaktiviert.
void spikes();

// OPTIONAL: Schie�t einen (Lotti-)Ghost auf den 1. Spieler
// (wenn man es nicht selber ist, sonst auf den letzten).
// Slashed den Getroffenen mit einem LOTTI-Schrei f�r 3 Sekunden
void shoot_badass_aiming_rocket();

// Macht den Spieler gr��er und er kann andere �berfahren,
// die nicht auch gro� sind
void start_mushroom();

// Macht alle Spieler klein und langsamer (5 Sekunden)
// bis auf den Sch�tzen
void start_flash();


#include "items.c"

#endif /* items_h */