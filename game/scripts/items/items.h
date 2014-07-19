#ifndef items_h
#define items_h

#define ITEM_NONE 0
#define ITEM_GRAVE 1
#define ITEM_ROCKET					2
#define ITEM_AIM_ROCKET				3
#define ITEM_TURBO					4
#define ITEM_BADASS_ROCKET			5
#define ITEM_MUSHROOM				6

#define ITEM_GRAVE_MODEL			"item_grave.mdl"
#define ITEM_ROCKET_MODEL			"item_rocket.mdl"
#define ITEM_AIM_ROCKET_MODEL		"item_aim_rocket.mdl"
#define ITEM_TURBO_MODEL			"item_turbo.mdl"
#define ITEM_BADASS_ROCKET_MODEL	"item_badass_rocket.mdl"
#define ITEM_MUSHROOM_MODEL		"item_mushroom.mdl"

#define IS_TRAP						FLAG3
 
#define SPIKE_TIMER_UP				3
#define SPIKE_TIMER_DOWN			1
#define SPIKE_SPEED					20

SOUND* sndCollectItem = "";
SOUND* sndCollectA4Cube = "";
SOUND* sndGraveCollision = "";

SOUND* sndSpikesDown = "spikes01.wav";
SOUND* sndSpikesUp   = "spikes02.wav";
SOUND* sndStalactite = "squish_loud.wav";
SOUND* sndStoneFall  = "stone_fall.wav";
SOUND* sndStoneImpact= "stone_impact.wav";



// Rotiert, kann eingesammelt werden.
// Bringt Punkte. Bei 10 Punkten -> Sofortiger Speedboost.
void a4_cube();

// Wird eingesammelt und vergibt ein Zufallsitem, falls noch
// keins im Slot ist. Verpufft, und wird 3 Sekunden später gerespawned.
void item();

// Verhalten eines Grabsteins (Falle)
void grave();

// Legt ein A9-Grab ab, das Fahrer ausbremst, wenn sie drüberfahren
void plant_grave();

// Schießt eine Rakete ab
void shoot_rocket();

// OPTIONAL: Schießt eine leicht zielgelenkte Rakete ab
void shoot_badass_aiming_rocket();

// Beschleunigt den Spieler für 3 Sekunden
// auf 1,4(?)-fache Geschwindigkeit
void use_turbo();

// Spikes, die aus dem Boden fahren und den Spieler
// ausbremsen. Werden bei Berührung für 5 Sekunden inaktiviert.
void spikes();

// OPTIONAL: Schießt einen (Lotti-)Ghost auf den 1. Spieler
// (wenn man es nicht selber ist, sonst auf den letzten).
// Slashed den Getroffenen mit einem LOTTI-Schrei für 3 Sekunden
void shoot_ultra_badass_aiming_rocket();

// Aktiviert den Lotteriatypischen Rainbowmode für 8 Sekunden
void start_rainbow_mode();


#include "items.c"

#endif /* items_h */