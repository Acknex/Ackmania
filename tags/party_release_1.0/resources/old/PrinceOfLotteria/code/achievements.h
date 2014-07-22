#ifndef achievements_h
#define achievements_h

#include <windows.h>

/* added by firo - start */
#define PRAGMA_BIND "ackAltF4.tga"
#define PRAGMA_BIND "ackGeneric1.tga"
#define PRAGMA_BIND "ackGeneric2.tga"
#define PRAGMA_BIND "ackGeneric3.tga"
#define PRAGMA_BIND "ackDeath.tga"
#define PRAGMA_BIND "ackUnity.tga"
#define PRAGMA_BIND "ackGamestudio.tga"
#define PRAGMA_BIND "ackResetCombo.tga"
#define PRAGMA_BIND "ackAckForNoAck.tga"
/* added by firo - end */

#define ACHIEVEMENTS_DELIMITER_IDTXT "$"
#define ACHIEVEMENTS_DELIMITER_TXTDESC "#"
#define ACHIEVEMENTS_DELIMITER_DESCICO "*"

#define ACHIEVEMENTS_REG_SECTION "princeoflotteria"

#define ACHIEVEMENTS_REG_TRUE 1
#define ACHIEVEMENTS_REG_FALSE 0
#define ACHIEVEMENTS_REG_IGNORE_CNT -1
#define ACHIEVMENTS_REG_CNT_NAME "omnomnom"

#define ACHIEVEMENT_SPEED_SHOW 10
#define ACHIEVEMENT_SPEED_PAUSE 3.0
#define ACHIEVEMENT_SPEED_HIDE 25

#define ACHIEVEMENTS_DEF_TXT "Achievement unlocked"
#define ACHIEVEMENTS_DEF_DESC "You've unlocked an achievement!"

#define ACHIEVEMENTS_RESET_CHEAT (key_j && key_c && key_l)

BOOL g_bAchievementActive = false;

BOOL g_bAchievementUnlocked = false;
BOOL g_bAchievementFound = false;

BOOL g_bAchievementGiven = false;

#define ACHIEVEMENT_FLAG_CORRECTSTART 0
#define ACHIEVEMENT_FLAG_CORRECTEXIT 1
#define ACHIEVEMENT_FLAG_ONEXITEXIT 2

TEXT* g_txtAchievementFlags =
{
	string
	(
		"correctstart",
		"correctexit",
		"onexitexit",
	);
}

TEXT* g_txtAchievements =
{
	string
	(
		"exitapp$Game exited!#You have exited the game properly.*ackAltF4.tga",
		"wrongexit$Bad boy!#You closed the game in a wrong way.*ackAltF4.tga",
		
		"firststart$First start!#You've started the game.*ackGeneric1.tga",
		"secondstart$Second start!#You've started the game twice. Very very very nice, f-yeah!!!111*ackGeneric1.tga",
		
		"achievementsresetted$Achievements resetted!#You pressed J+C+L to reset all achievements.*ackResetCombo.tga",
		
		"timeout_a$Lazyness!#You didn't do anything for 60 seconds.*ackGeneric1.tga",
		"timeout_b$More lazyness!#You didn't do anything for 10 minutes.*ackGeneric2.tga",
		"timeout_c$Even more lazyness!#You didn't do anything for 1 hour.*ackGeneric3.tga",
		
		"unityinstalled$Virus alert!#Your system is infected with Unity 3D, please remove it. Thanks.*ackDeath.tga",
		"unitynotinstalled$F-ing Unity!#No one needs this bloated stuff, thanks for not installing Unity.*ackUnity.tga",
		
		"medinstalled$Viva la Gamestudio A8!#Very nice! You installed Gamestudio A8 and we are very proud of you.*ackGamestudio.tga",
		"mednotinstalled$Lost in Unity?#Try Gamestudio A8, the world's easiest and powerful game developing system.*ackDeath.tga",	
		
		"noachievement$No achievement#You didn't received any achievement, so we give you one for free.*ackAckForNoAck.tga",
		"firstjetpack$Jetpack collected!#Congratulations! Don't smash your head.*ackGeneric1.tga",
		"firstcube$First Acknex Cube collected!#Oh my precious...*ackGeneric2.tga",
		"firstlive$Extra live acquired!#Waste it wisely...*ackGeneric3.tga",
		"firstlevel$First level completed!#The next level will need even more tries to complete!*ackGeneric3.tga",
		"lottifant$Lottifant found!!#Oh my god it's soooo cute...^^*ackGeneric2.tga"
	);
}

STRING* g_strAchievementId = "#128";
STRING* g_strAchievementTxt = "#256";
STRING* g_strAchievementDesc = "#256";

STRING* g_strAchievementRegTxt = "#32";

var g_percentAchievementBox = 0;

BMAP* g_bmapAchievementBox = "achievementBox.tga";
PANEL* g_pnlAchievementBox =
{
	bmap = g_bmapAchievementBox;
	layer = 100;
}

var g_pnlAchievementIcoOffset = 0;
var g_pnlAchievementTextOffset = 0;

BMAP* g_bmapAchievementIco = NULL;
PANEL* g_pnlAchievementIco =
{
	layer = 200;
}

FONT* g_fntAchievementTxt = "Arial#20b";
TEXT* g_txtAchievementTxt =	
{
  layer = 300;
  font = g_fntAchievementTxt;
  string (g_strAchievementTxt);
  flags = TRANSLUCENT;
}

FONT* g_fntAchievementDesc = "Arial#16";
TEXT* g_txtAchievementDesc =	
{
  layer = 300;
  font = g_fntAchievementDesc;
  string (g_strAchievementDesc);
  size_x = 250;
  size_y = 100;
  flags = WWRAP | TRANSLUCENT;
}

var g_achievementTimeout = 0;

SOUND* g_achSnd0 = "achSnd0.wav";
SOUND* g_achSnd1 = "achSnd1.wav";
SOUND* g_achSnd2 = "achSnd2.wav";
SOUND* g_achSnd3 = "achSnd3.wav";
SOUND* g_achSnd4 = "achSnd4.wav";
SOUND* g_achSnd5 = "achSnd5.wav";

#define ACH_SND_NUM 6
SOUND* g_achSndArr [ACH_SND_NUM];

#define ACHIEVEMENT_STR_LEN 128

BOOL g_bAchievementsExist = false;

typedef struct AchievementQueueElem
{
	char txt [ACHIEVEMENT_STR_LEN];
	char desc [ACHIEVEMENT_STR_LEN];
	char ico [ACHIEVEMENT_STR_LEN];
	
	struct AchievementQueueElem* next;
	struct AchievementQueueElem* prev;
	
} AchievementQueueElem;

AchievementQueueElem* g_achievementQueueHead = NULL;
AchievementQueueElem* g_achievementQueueTail = NULL;

void achievements_h_startup ()
{
	g_achSndArr[0] = g_achSnd0;
	g_achSndArr[1] = g_achSnd1;
	g_achSndArr[2] = g_achSnd2;
	g_achSndArr[3] = g_achSnd3;
	g_achSndArr[4] = g_achSnd4;
	g_achSndArr[5] = g_achSnd5;
}

void achievement (char* name);

void achievementWorker ();
void achievementTimeoutWorker ();
void achievementBoxWorker ();

void achievementsOnExit ();
void achievementsExit ();

void setAchievementFlag (char* name, BOOL bFlag);
void setAchievementFlag (char* name);
void resetAchievementFlag (char* name);
BOOL getAchievementFlag (char* name);
void setAchievementFlag (int id, BOOL bFlag);
void setAchievementFlag (int id);
BOOL getAchievementFlag (int id);
int getAchievementNum (char* name);
int incAchievementNum (char* name);
int setAchievementNum (char* name, int num);

void resetAchievementsWorker ();
void showAchievement (AchievementQueueElem* a);
void putAchievement (char* strTxt, char* strDesc, char* strIco);
void resetAchievements ();

BOOL extractAchievementTokens (STRING* str, char* strId, char* strTxt, char* strDesc, char* strIco);
void playRandomAchievementSound ();

#define ACHIEVEMENTS_DUMMY_NUM 1
int numTotalAchievements ();
int numAchievedAchievements ();

// unity crap

	typedef int HKEY;
	typedef int REGSAM;

	long WINAPI RegEnumKeyEx (HKEY hKey, DWORD dwIndex, char *lpName, long *lpcName, long *lpReserved, char *lpClass, long *lpcClass, FILETIME *lpftLastWriteTime);		
	long WINAPI RegOpenKeyEx(HKEY hKey, char *lpSubKey, DWORD ulOptions, REGSAM samDesired, HKEY *phkResult);
	long WINAPI RegCloseKey(HKEY hKey);

#include "achievements.c"

#endif /* achievements_h */