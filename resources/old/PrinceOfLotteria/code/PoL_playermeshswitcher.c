
#define PoL_playerHatGroup 1
#define PoL_playerSwordGroup 2
#define PoL_playerSwordOutGroup 3
#define PoL_playerJetpackGroup 4

void PoL_playerSetMesh(BOOL* b,var m)
{
	if(b==FALSE)
	player.vmask|=1<<m;
	else
	player.vmask&=~(1<<m);
}

void PoL_playerSetHat(BOOL* b)
{
	PoL_playerSetMesh(b,PoL_playerHatGroup);
}

void PoL_playerSetSword(BOOL* b)
{
	PoL_playerSetMesh(b,PoL_playerSwordGroup);
	PoL_playerSetMesh(!b,PoL_playerSwordOutGroup);
}

void PoL_playerSetJetpack(BOOL* b)
{
	PoL_playerSetMesh(b,PoL_playerJetpackGroup);
}