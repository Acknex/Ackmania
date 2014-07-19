/*
 *******************************************************************************
 * camera.h
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
 * definition script for camera
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */
 
#ifndef CAMERA_H
#define CAMERA_H
 
 
void create_camera();
void create_camera(int);
void camera_focus(ENTITY*);
void remove_camera();
void update_camera();
void show_camera();
void hide_camera();
 
 
#include "camera.c"

#endif