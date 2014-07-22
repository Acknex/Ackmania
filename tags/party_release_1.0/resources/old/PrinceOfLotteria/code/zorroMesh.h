#ifndef zorroMesh_h
#define zorroMesh_h

#define ZORRO_MESH_ZORROHAT 6
#define ZORRO_MESH_INDIHAT 8
#define ZORRO_MESH_SWORDINHAND 10
#define ZORRO_MESH_SWORDATBELT 11
#define ZORRO_MESH_JETPACK 7

void zorroSetMesh (ENTITY* ent, BOOL b, int group);

void zorroIndiJonesHat (ENTITY* ent, BOOL b);
void zorroSwordAtBelt (ENTITY* ent, BOOL b);
void zorroHasJetpack (ENTITY* ent, BOOL b);

void zorroMeshOptions (ENTITY* ent, BOOL bIndiHat, BOOL bSwordAtBelt, BOOL bHasJetpack);

#include "zorroMesh.c"

#endif /* zorroMesh_h */