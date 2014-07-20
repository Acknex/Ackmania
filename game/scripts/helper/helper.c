#ifndef helper_c
#define helper_c

void place_on_floor(ENTITY* _ent)
{
	c_trace(_ent->x, vector(_ent->x, _ent->y, _ent->z - 1000), IGNORE_ME | IGNORE_PASSABLE);
	_ent->z = hit.z - _ent.min_z;
}

var get_nearest_path_point(ENTITY* ent, char* pathname)
{
	ENTITY* e = ent_create(NULL, ent->x, NULL);
	path_set(e, pathname);
	var length = path_length(e);
	var optDist = 0;
	var entDist = 10000;
	
	var dist = 0;
	var vLastPos[3];
	var vDir[3];
	while(dist <= length)
	{
		path_spline(e,e.x,dist);
		dist += 5*time_step;
		if (vec_dist(e->x, ent->x) < entDist) {
			entDist = vec_dist(e->x, ent->x);
			optDist = dist;
		}
	}
	return optDist;
}

#endif /*helper_c*/