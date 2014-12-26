/*
 *******************************************************************************
 * camera.h
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
void remove_camera();
void update_camera();
void show_camera();
void hide_camera();
void focus_camera();
void change_camera();
VIEW* get_camera();
 
 
#include "camera.c"

#endif