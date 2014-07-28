#ifndef playrace_h
#define playrace_h

void play_race(int index);
void do_race_end(ENTITY* ent);

BMAP* bFinish = "Finish.png";

BMAP* bWinner = "Winner.png";
BMAP* bSecondPlace = "SecondPlace.png";
BMAP* bThirdPlace = "ThirdPlace.png";
BMAP* bloser = "loser.png";

BMAP* bPressSpace = "PressSpace.png";

BMAP* barr [4];

PANEL* bFinishPan = {
      bmap = bFinish;
}

PANEL* bbWinnerPan = {
      bmap = bWinner;
}

PANEL* bPressSpacePan = {
      bmap = bPressSpace;
}

SOUND* splace_1st = "place_1st.ogg";
SOUND* splace_2nd = "place_2nd.ogg";
SOUND* splace_3rd = "place_3rd.ogg";

SOUND* swin [3];
SOUND* splacelost [5];

void playrace_h_startup () {

   barr[0] = bWinner;
   barr[1] = bSecondPlace;
   barr[2] = bThirdPlace;
   barr[3] = bloser;

   swin [0] = splace_1st;
   swin [1] = splace_2nd;
   swin [2] = splace_3rd;

   splacelost[0] = snd_create("place_lost1.ogg");
   splacelost[1] = snd_create("place_lost2.ogg");
   splacelost[2] = snd_create("place_lost3.ogg");
   splacelost[3] = snd_create("place_lost4.ogg");
   splacelost[4] = snd_create("place_lost5.ogg");

}

#define RACE_REF_SCREENSIZE 1020

#include "playrace.c"

#endif /* playrace_h */
