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
var vViewWidth;

var vCameraPosType = CAMERA_POS_3RDPERSON;

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

	/* width = view.size_x * 2 * tan(view.arc/2); - from manual (view.arc) */
	vViewWidth = CAMERA_REFSCRSIZEX * 2 * tanv(CAMERA_REFARC * 0.5);
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
	VECTOR vecPos;
	
	if (camera_focus_ent != NULL)
	{
		switch(vCameraPosType) {
		
			// Top
			case CAMERA_POS_TOP:
				cam->flags &= ~ISOMETRIC;
				vec_set(cam->x, camera_focus_ent->x);
				vec_add(cam->x, vector(0, 0, 1000));
				cam->pan = camera_focus_ent.pan;
				cam->tilt = 270;
				vDistanceFactor += ((is_kart_accelerating(camera_focus_ent) > 0) * 0.05 - 0.02) * time_step;
				vDistanceFactor = clamp(abs(vDistanceFactor), 0.28, 0.6);
				cam->arc = 2 * atanv(vViewWidth * 0.5 / (screen_size.x / vDistanceFactor));
			break;
			
			// Iso
			case CAMERA_POS_ISO:
				cam->pan = CAMERA_PAN;
				cam->tilt = CAMERA_TILT; 
				cam->flags |= ISOMETRIC;
				vec_set(vecPos, vector(-CAMERA_DIST, 0, 0));
				vec_rotate(vecPos, cam->pan);
				vec_add(vecPos, vector(camera_focus_ent->x, camera_focus_ent->y, 0));
				vec_set(cam->x, vecPos);
		
				vDistanceFactor += ((is_kart_accelerating(camera_focus_ent) > 0) * 0.05 - 0.02) * time_step;
				vDistanceFactor = clamp(abs(vDistanceFactor), 0.28, 0.6);
		
				// view.arc = 2*atan(width/(view.size_x * 2)); // from manual (view.arc)
				// change camera arc by manipulating view size // delivers resolution independent result
				cam->arc = 2 * atanv(vViewWidth * 0.5 / (screen_size.x / vDistanceFactor));
			break;
			
			// 3rd person
			case CAMERA_POS_3RDPERSON:
				cam->flags &= ~ISOMETRIC;
				
				cam->pan = camera_focus_ent.pan;
				vec_set(cam->x, vector(camera_focus_ent->x - 250 * cos(cam->pan), camera_focus_ent->y - 250 * sin(cam->pan), camera_focus_ent->z + 90));
				cam->tilt = 350;			
			break;
			
			// 1st person
			case CAMERA_POS_1STPERSON:
				cam->flags &= ~ISOMETRIC;
				vec_set(cam->x, camera_focus_ent->x);
				vec_add(cam->x, vector(0, 0, 10));
				cam->pan = camera_focus_ent.pan;
				cam->tilt = 350;			
			break;
		}
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

void focus_camera()
{
	camera_focus_ent = NULL;
	do
	{
 		camera_focus_ent = get_kart_player();
		wait(1);
	} while (camera_focus_ent == NULL);
}

void change_camera()
{
	if (vCameraPosType < 4) {
		vCameraPosType +=1;
	} else {
		vCameraPosType = 1;
	}
}
 
VIEW* get_camera()
{
	return cam;
}

