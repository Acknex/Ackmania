/*
 *******************************************************************************
 * camera.c
 * Creation date: 19.07.2014
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2011-02-23 22:27:12 +0100 (Mi, 23 Feb 2011) $
 * $Revision: 6 $
 * $Author: Firo $
 *
 *******************************************************************************
 * Description
 *
 * Script for camera movement
 *
 * Comments
 * 
 * for short descriptions see comments in camera.h
 *
 *******************************************************************************
 */
 
//#include "movement.h"
#include "camera_cfg.h"


ENTITY* camera_focus_ent = NULL;
VIEW* cam;

void create_camera()
{
	create_camera(1);
}

void create_camera(int layer)
{
	cam = view_create(layer);
	cam->pan = CAMERA_PAN;
	cam->tilt = CAMERA_TILT; 
	cam->flags |= ISOMETRIC;
}

void camera_focus(ENTITY* ent)
{
	camera_focus_ent = ent;
}

void remove_camera()
{
	if (cam != NULL)
	{
		ptr_remove(cam);
		cam = NULL;
	}	
}

void update_camera()
{
	var vDistanceFactor;
	VECTOR vecPos;
	
	if (camera_focus_ent != NULL)
	{
//		vDistanceFactor = 1 + get_kart_speed(camera_focus_ent, NULL);
		vDistanceFactor = 1;
	
		vec_set(vecPos, vector(-vDistanceFactor * CAMERA_DIST, 0, 0));
		vec_rotate(vecPos, cam->pan);
		vec_add(vecPos, camera_focus_ent->x);
		vec_sub(vecPos, cam->x);
		vec_scale(vecPos, CAMERA_DELAY);
		vec_add(cam->x, vecPos);
	}
}

void show_camera()
{
	if (cam != NULL)
	{
		camera->flags &= ~SHOW;
		wait(0); //WTF!?
		cam->flags |= SHOW;
	}
}

void hide_camera()
{
	if (cam != NULL)
	{
		cam->flags &= ~SHOW;
		camera->flags |= SHOW;
	}
}

 

