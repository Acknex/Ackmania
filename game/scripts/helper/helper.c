#ifndef helper_c
#define helper_c

void place_on_floor(ENTITY* _ent)
{
	c_trace(_ent->x, vector(_ent->x, _ent->y, _ent->z - 1000), IGNORE_ME | IGNORE_PASSABLE);
	_ent->z = hit.z - _ent.min_z;
}

#endif /*helper_c*/