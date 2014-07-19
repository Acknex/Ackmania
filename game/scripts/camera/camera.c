/*
 *******************************************************************************
 * camera.c
 * Creation date: 19.07.2014
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date$
 * $Revision$
 * $Author$
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
 
#include "camera_cfg.h"
#include "raceplayer.h"
#include "startgrid.h"


ENTITY* camera_focus_ent = NULL;
VIEW* cam;
var vDistanceFactor;

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
	
	do
	{
 		camera_focus_ent = get_kart_player();
		wait(1);
	} while (camera_focus_ent == NULL);
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
	//var vDistanceFactorNew;
	var vFac;
	VECTOR vecPos;
	
	if (camera_focus_ent != NULL)
	{
		vec_set(vecPos, vector(-CAMERA_DIST, 0, 0));
		vec_rotate(vecPos, cam->pan);
		vec_add(vecPos, vector(camera_focus_ent->x, camera_focus_ent->y, 0));
		vec_set(cam->x, vecPos);

		/* interpolate ARC based on y resolution
		640x400 --> ARC 110
		1920x1200 --> ARC 60
		zoom via ARC based on kart speed
		*/

		vDistanceFactor += ((is_kart_accelerating(camera_focus_ent) > 0) * 0.05 - 0.02) * time_step;
		vDistanceFactor = clamp(abs(vDistanceFactor), 0.28, 0.6);
		vFac = (CAMERA_REFSCRSIZEY - screen_size.y) / CAMERA_REFSCRSIZEY;
		cam->arc = (CAMERA_REFARCBIG * (1 - vFac) + CAMERA_REFARCSMALL * vFac) * vDistanceFactor;

//	DEBUG_VAR(vDistanceFactor, 200);
	}
	
}

void show_camera()
{
	if (cam != NULL)
	{
		cam->flags |= SHOW;
		camera->flags &= ~SHOW;
	}
}

void hide_camera()
{
	if (cam != NULL)
	{
		camera->flags |= SHOW;
		cam->flags &= ~SHOW;
	}
}


 

