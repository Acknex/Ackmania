#ifndef achievements_c
#define achievements_c

#include "achievements.h"

BOOL isUnityInstalled ()
{
	BOOL b = false;
	
	HKEY hTestKey;
	
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Unity Technologies", 0, KEY_READ, &hTestKey) == ERROR_SUCCESS)
	{
		b = true;
		RegCloseKey(hTestKey);
	}
	
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Unity", 0, KEY_READ, &hTestKey) == ERROR_SUCCESS)
	{
		b = true;
		RegCloseKey(hTestKey);
	}
	
	return b;
}

BOOL isMed8Installed ()
{
	BOOL b = false;
	
	HKEY hTestKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\MED8", 0, KEY_READ, &hTestKey) == ERROR_SUCCESS)
	{
		b = true;
		RegCloseKey(hTestKey);
	}
	
	return b;
}

void achievementUnityGamestudio ()
{
	wait(-3);
	if (isUnityInstalled())
		achievement("unityinstalled");
	else
		achievement("unitynotinstalled");
		
	if (isMed8Installed())
		achievement("medinstalled");
	else
		achievement("mednotinstalled");
}

void achievement (char* name, int cnt)
{
	g_bAchievementUnlocked = false;
	g_bAchievementFound = false;
	
	char bufId [128], bufTxt [128], bufDesc [128], bufIco [128];
	
	if (g_txtAchievements->strings > 0)
	{
		int i;
		for (i = 0; i < g_txtAchievements->strings; i++)
		{
			STRING* str = (g_txtAchievements->pstring)[i];
			if (str != NULL)
			{
				if (extractAchievementTokens(str, bufId, bufTxt, bufDesc, bufIco))
				{
					if (str_cmpi(bufId, name))
					{
						if ((cnt == ACHIEVEMENTS_REG_IGNORE_CNT && !getAchievementFlag(name)) || (cnt != ACHIEVEMENTS_REG_IGNORE_CNT))
						{
							if (cnt == ACHIEVEMENTS_REG_IGNORE_CNT || getAchievementNum(name)+1 == cnt)
							{	
								incAchievementNum(ACHIEVMENTS_REG_CNT_NAME);
								
								g_bAchievementGiven = true;
								putAchievement(bufTxt, bufDesc, bufIco);
								
								if (cnt != ACHIEVEMENTS_REG_IGNORE_CNT)
									incAchievementNum(name);
								else
									setAchievementFlag(name);								
							}
							else
							{
								if (cnt != ACHIEVEMENTS_REG_IGNORE_CNT)
								{
									incAchievementNum(name);
								}
							}							
						}
					}
				}
			}
		}
	}
}

void achievement (char* name)
{
	achievement(name, ACHIEVEMENTS_REG_IGNORE_CNT);
}

void achievements_c_startup ()
{
	on_alt = NULL;
	on_f4 = NULL;
	on_esc = NULL;
	
	on_exit = achievementsOnExit;
	
	achievementWorker();
	resetAchievementsWorker();
	achievementBoxWorker();
	achievementTimeoutWorker();
	achievementUnityGamestudio();
	
	while (total_secs < 2)
		wait(1);
	
	// gestartet aber nicht ordnungsgemäß geschlossen
	if (getAchievementFlag(ACHIEVEMENT_FLAG_ONEXITEXIT) && !getAchievementFlag(ACHIEVEMENT_FLAG_CORRECTEXIT) && getAchievementFlag(ACHIEVEMENT_FLAG_CORRECTSTART))
		achievement("wrongexit");
		
	// first and second start
	achievement("firststart");
	achievement("secondstart", 2);
		
	setAchievementFlag(ACHIEVEMENT_FLAG_CORRECTSTART, true);
}

void achievementTimeoutWorker ()
{
	while (1)
	{
		if (mouse_moving == 1 || key_any)
			g_achievementTimeout = 0;
		else
			g_achievementTimeout += time_step / 16;
			
		if (g_achievementTimeout > 60)
			achievement("timeout_a");
			
		if (g_achievementTimeout > 600)
			achievement("timeout_b");
			
		if (g_achievementTimeout > 3600)
			achievement("timeout_c");
		
		wait(1);
	}
}

void achievementWorker ()
{
	while (1)
	{
		while (g_achievementQueueHead == NULL)
			wait(1);
			
		AchievementQueueElem* a = g_achievementQueueHead;
			
		showAchievement(a);
		
		wait_for(showAchievement);
		
		wait(-1);
		
		if (a == g_achievementQueueTail)
		{
			g_achievementQueueHead = g_achievementQueueTail = NULL;
			g_bAchievementsExist = false;
		}
		else
		{
			g_achievementQueueHead = a->next;
		}
		
		sys_free(a);
		
		wait(1);
	}
}

void achievementBoxWorker ()
{
	str_cpy(delimit_str, "§");
	
	while (1)
	{
		g_pnlAchievementBox->pos_x = screen_size.x / 2 - g_pnlAchievementBox->bmap->width / 2;
		g_pnlAchievementBox->pos_y = screen_size.y - g_pnlAchievementBox->bmap->height - 20;
		
		var txtOffset = 0;
		
		if (g_bmapAchievementIco != NULL)
		{
			int offset = (g_pnlAchievementBox->bmap->height - g_bmapAchievementIco->height) / 2;
			
			g_pnlAchievementIco->pos_x = g_pnlAchievementBox->pos_x + offset;
			g_pnlAchievementIco->pos_y = g_pnlAchievementBox->pos_y + offset;
			
			g_pnlAchievementIcoOffset = offset;
			
			txtOffset += offset + g_bmapAchievementIco->width;
		}
		
		g_pnlAchievementTextOffset = g_pnlAchievementBox->pos_x + txtOffset + 10;
		
		var targetSizeX = g_pnlAchievementBox->bmap->width * (g_percentAchievementBox / 100);
		
		g_txtAchievementTxt->pos_x = g_pnlAchievementBox->pos_x + txtOffset + 10;
		g_txtAchievementTxt->pos_y = g_pnlAchievementBox->pos_y + 15;
		g_txtAchievementTxt->alpha = g_percentAchievementBox;
		
		g_txtAchievementDesc->pos_x = g_txtAchievementTxt->pos_x;
		g_txtAchievementDesc->pos_y = g_txtAchievementTxt->pos_y + 20;
		g_txtAchievementDesc->alpha = g_percentAchievementBox * 0.75;
		
		g_percentAchievementBox = clamp(g_percentAchievementBox, 0, 100);
		
		if (targetSizeX < 1)
		{
			reset(g_pnlAchievementBox, SHOW);
			reset(g_pnlAchievementIco, SHOW);
			reset(g_txtAchievementTxt, SHOW);
			reset(g_txtAchievementDesc, SHOW);
		}
		else
		{
			set(g_pnlAchievementBox, SHOW);
			set(g_pnlAchievementIco, SHOW);
			set(g_txtAchievementTxt, SHOW);
			set(g_txtAchievementDesc, SHOW);
			
			g_pnlAchievementBox->size_x = targetSizeX;
			
			if (g_bmapAchievementIco != NULL)
				g_pnlAchievementIco->size_x = clamp(targetSizeX - g_pnlAchievementIcoOffset, 0, g_bmapAchievementIco->width);
		}
		
		wait(1);
	}
}

void achievementsOnExit ()
{
	setAchievementFlag(ACHIEVEMENT_FLAG_ONEXITEXIT);
}

void achievementsExit ()
{
	if (g_bAchievementGiven == false)
		achievement("noachievement");

	setAchievementFlag(ACHIEVEMENT_FLAG_CORRECTEXIT);
	achievement("exitapp");
	
	wait(-1);
	
	while (g_bAchievementsExist)
		wait(1);
		
	sys_exit("lotter on!");
}

void setAchievementFlag (char* name, BOOL bFlag)
{
	if (bFlag)
		setAchievementNum(name, ACHIEVEMENTS_REG_TRUE);
	else
		setAchievementNum(name, ACHIEVEMENTS_REG_FALSE);
}

void setAchievementFlag (char* name)
{
	setAchievementFlag(name, true);
}

void resetAchievementFlag (char* name)
{
	setAchievementFlag(name, false);
}

int getAchievementNum (char* name)
{
	return sys_getvar(ACHIEVEMENTS_REG_SECTION, name);
}

int setAchievementNum (char* name, int num)
{
	sys_setvar(ACHIEVEMENTS_REG_SECTION, name, num);
}

BOOL getAchievementFlag (char* name)
{
	BOOL b = false;
	var v = sys_getvar(ACHIEVEMENTS_REG_SECTION, name);
	
	if (v > 0)
		b = true;
	
	return b;
}

void setAchievementFlag (int id, BOOL bFlag)
{
	setAchievementFlag(_chr((g_txtAchievementFlags->pstring)[id]), bFlag);
}

void setAchievementFlag (int id)
{
	setAchievementFlag(id, true);
}

BOOL getAchievementFlag (int id)
{
	return getAchievementFlag(_chr((g_txtAchievementFlags->pstring)[id]));
}

int incAchievementNum (char* name)
{
	int num = getAchievementNum(name);
	int incNum = num + 1;
	
	setAchievementNum(name, incNum);
	
	return incNum;
}

void resetAchievementsWorker ()
{
	while (1)
	{
		if (ACHIEVEMENTS_RESET_CHEAT)
		{
			while (ACHIEVEMENTS_RESET_CHEAT)
				wait(1);
				
			resetAchievements();
		}
			
		wait(1);
	}
}

void showAchievement (AchievementQueueElem* a)
{
	playRandomAchievementSound();
	
	if (g_bmapAchievementIco != NULL)
	{
		bmap_purge(g_bmapAchievementIco);
		ptr_remove(g_bmapAchievementIco);
		g_bmapAchievementIco = NULL;
	}
	
	g_pnlAchievementIco->bmap = NULL;
	g_pnlAchievementIco->size_x = g_pnlAchievementIco->size_y = 8;
	
	if (a->ico != NULL)
	{
		g_bmapAchievementIco = bmap_create(a->ico);
		
		if (g_bmapAchievementIco != NULL)
		{
			g_pnlAchievementIco->bmap = g_bmapAchievementIco;
			g_pnlAchievementIco->size_x = bmap_width(g_bmapAchievementIco);
			g_pnlAchievementIco->size_y = bmap_height(g_bmapAchievementIco);
		}
	}
	
	if (a->txt != NULL)
		str_cpy(g_strAchievementTxt, a->txt);
	else
		str_cpy(g_strAchievementTxt, ACHIEVEMENTS_DEF_TXT);
		
	if (a->desc != NULL)
		str_cpy(g_strAchievementDesc, a->desc);
	else
		str_cpy(g_strAchievementDesc, ACHIEVEMENTS_DEF_DESC);		
	
	var percent = 0;
	
	while (percent < 100)
	{
		percent = clamp(percent + ACHIEVEMENT_SPEED_SHOW * time_step, 0, 100);
		g_percentAchievementBox = percent;
		
		wait(1);
	}
	
	wait(-ACHIEVEMENT_SPEED_PAUSE);
	
	while (percent > 0)
	{
		percent = clamp(percent - ACHIEVEMENT_SPEED_HIDE * time_step, 0, 100);
		g_percentAchievementBox = percent;
		
		wait(1);
	}
}

void putAchievement (char* strTxt, char* strDesc, char* strIco)
{
	AchievementQueueElem* a = (AchievementQueueElem*)sys_malloc(sizeof(AchievementQueueElem));
	
	strcpy(a->txt, strTxt);
	strcpy(a->desc, strDesc);
	strcpy(a->ico, strIco);
	
	if (g_achievementQueueHead == NULL)
	{
		g_achievementQueueHead = g_achievementQueueTail = a;
	}
	else
	{
		a->prev = g_achievementQueueTail;
		g_achievementQueueTail->next = a;
		
		g_achievementQueueTail = a;
	}
	
	g_bAchievementsExist = true;
}

void resetAchievements ()
{
	if (g_txtAchievements->strings > 0)
	{
		int i;
		for (i = 0; i < g_txtAchievements->strings; i++)
		{
			STRING* str = (g_txtAchievements->pstring)[i];
			
			if (str != NULL)
			{
				char bufId [128], bufDummy [128];
				
				if (extractAchievementTokens(str, bufId, bufDummy, bufDummy, bufDummy))
					resetAchievementFlag(bufId);
			}
		}		
	}
	
	if (g_txtAchievementFlags->strings > 0)
	{
		int i;
		for (i = 0; i < g_txtAchievementFlags->strings; i++)
		{
			STRING* str = (g_txtAchievementFlags->pstring)[i];
			
			if (str != NULL)
				resetAchievementFlag(_chr(str));
		}
		
		resetAchievementFlag(ACHIEVMENTS_REG_CNT_NAME);
	}
	
	achievement("achievementsresetted");
}

BOOL extractAchievementTokens (STRING* str, char* strId, char* strTxt, char* strDesc, char* strIco)
{
	STRING* sId = str_create("#128");
	STRING* sTxt = str_create("#128");
	STRING* sDesc = str_create("#128");
	STRING* sIco = str_create("#128");
	
	str_cpy(sId, "");
	str_cpy(sTxt, "");
	str_cpy(sDesc, "");
	str_cpy(sIco, "");

	int delimiterIdTxt = str_stri(str, ACHIEVEMENTS_DELIMITER_IDTXT);
	int delimiterTxtDesc = str_stri(str, ACHIEVEMENTS_DELIMITER_TXTDESC);
	int delimiterDescIco = str_stri(str, ACHIEVEMENTS_DELIMITER_DESCICO);
	
	if (delimiterIdTxt > 0 && delimiterTxtDesc > 0)
	{
		str_cpy(sId, str);
		str_trunc(sId, str_len(str) - delimiterIdTxt + 1);
		
		str_cpy(sTxt, str);
		str_trunc(sTxt, str_len(str) - delimiterTxtDesc + 1);
		str_clip(sTxt, delimiterIdTxt);
		
		str_cpy(sDesc, str);
		str_trunc(sDesc, str_len(str) - delimiterDescIco + 1);
		str_clip(sDesc, delimiterTxtDesc);
		
		str_cpy(sIco, str);
		str_clip(sIco, delimiterDescIco);
		
		strcpy(strId, _chr(sId));
		strcpy(strTxt, _chr(sTxt));
		strcpy(strDesc, _chr(sDesc));
		strcpy(strIco, _chr(sIco));		
	}
	
	BOOL b = (strlen(sId) > 0);
	
	ptr_remove(sId);
	ptr_remove(sTxt);
	ptr_remove(sDesc);
	
	return b;
}

void playRandomAchievementSound ()
{
	int r = random(ACH_SND_NUM);
	snd_play(g_achSndArr[r], 100, 0);
}

int numTotalAchievements ()
{
	return g_txtAchievements->strings + ACHIEVEMENTS_DUMMY_NUM;
}

int numAchievedAchievements ()
{
	return getAchievementNum(ACHIEVMENTS_REG_CNT_NAME);
}

#endif /* achievements_c */