#include <acknex.h>
#include <default.c>

#include "achievements.h"

BMAP* g_testBmap = "achievements_test.jpg";

PANEL* g_pnlTestBmap =
{
	bmap = g_testBmap;
	flags = SHOW;
}

void a1_ev ()
{
	achievement("testa");
}

void a2_ev ()
{
	achievement("testb");
}

void a3_ev ()
{
	achievement("testc");
}

void a4_ev ()
{
	beep();
	achievement("testd", 3);
}
	
int main ()
{
	fps_max = 60;
	video_mode = 8;

	wait(3);
	level_load(NULL);
	
	on_1 = a1_ev;
	on_2 = a2_ev;
	on_3 = a3_ev;
	on_4 = a4_ev;
	
	on_space = achievementsExit;
	
	while (1)
	{
		int numAchieved = numAchievedAchievements();
		int numTotal = numTotalAchievements();
	
		draw_text(str_printf(NULL, "J + C + L = reset\nSPACE = exit\n\n%d / %d achievments achieved!", numAchieved, numTotal), 100, 100, COLOR_WHITE);
		wait(1);
	}
}