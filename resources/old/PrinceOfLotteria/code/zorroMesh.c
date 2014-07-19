#ifndef zorroMesh_c
#define zorroMesh_c

#include "zorroMesh.h"

void zorroSetMesh (ENTITY* ent, BOOL b, int group)
{
	if (!b)
		ent->vmask |= 1 << group;
	else
		ent->vmask &= ~(1 << group);
}

void zorroIndiJonesHat (ENTITY* ent, BOOL b)
{
	if (ent != NULL)
	{
		zorroSetMesh(ent, !b, ZORRO_MESH_ZORROHAT);
		zorroSetMesh(ent, b, ZORRO_MESH_INDIHAT);
	}
}

void zorroSwordAtBelt (ENTITY* ent, BOOL b)
{
	if (ent != NULL)
	{
		zorroSetMesh(ent, !b, ZORRO_MESH_SWORDINHAND);
		zorroSetMesh(ent, b, ZORRO_MESH_SWORDATBELT);	
	}
}

void zorroHasJetpack (ENTITY* ent, BOOL b)
{
	if (ent != NULL)
	{
		zorroSetMesh(ent, b, ZORRO_MESH_JETPACK);	
	}
}

void zorroMeshOptions (ENTITY* ent, BOOL bIndiHat, BOOL bSwordAtBelt, BOOL bHasJetpack)
{
	zorroIndiJonesHat(ent, bIndiHat);
	zorroSwordAtBelt(ent, bSwordAtBelt);
	zorroHasJetpack(ent, bHasJetpack);
}

#endif /* zorroMesh_c */