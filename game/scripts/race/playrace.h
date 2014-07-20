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

void playrace_h_startup () {

   barr[0] = bWinner;
   barr[1] = bSecondPlace;
   barr[2] = bThirdPlace;
   barr[3] = bloser;

}

#include "playrace.c"

#endif /* playrace_h */
