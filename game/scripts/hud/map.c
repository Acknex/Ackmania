/*
 *******************************************************************************
 * map.c
 * Creation date: 19.07.2014
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2014-07-19 19:41:07 +0200 (Sa, 19 Jul 2014) $
 * $Revision: 212 $
 * $Author: Firoball $
 *
 *******************************************************************************
 * Description
 *
 * definition script for mini map
 *
 * Comments
 * 
 * for short descriptions see comments in map.h
 *
 *******************************************************************************
 */
 
#include "map_cfg.h"

var vMapScale;
VECTOR vecMapMin;
VECTOR vecMapMax;
var* vNodeX;
var* vNodeY;
var vMapSize;
var vMapActive = 0;
var vMapNodes;

void create_map()
{
	ENTITY* ent;
	VECTOR vecTemp;
	var i;
	
	while(level_ent == NULL)
	{
		wait(1);
	}

	vec_set(&vecMapMin, level_ent->min_x);
	vec_set(&vecMapMax, level_ent->max_x);
	vecMapMin.z = 0;
	vecMapMax.z = 0;
	vec_scale(&vecMapMin, -1);
	
	ent = ent_create(NULL,nullvector,NULL);
	vMapNodes = path_next(ent);
	
	if(vMapNodes > 0)
	{
		vNodeX = sys_malloc(sizeof(var) * (vMapNodes+1));
		vNodeY = sys_malloc(sizeof(var) * (vMapNodes+1));
		for(i = 1; i <= vMapNodes; i++)
		{
			path_getnode(ent, i, &vecTemp, NULL);
			vNodeX[i] = vecTemp.x + vecMapMin.x;
			vNodeY[i] = -vecTemp.y + vecMapMin.y;
			//printf("%d %d", (int)vNodeX[i], (int)vNodeY[i]);
		}
	}
				
	vec_add(&vecMapMax, &vecMapMin);
	vec_scale(&vecMapMax, 0.9); 
	vec_set(&vecMapMin, nullvector);
	vMapActive = 1;
	wait(1);
	ptr_remove(ent);
}

void remove_map()
{
	vMapActive = 0;
	sys_free(vNodeX);
	sys_free(vNodeY);
}

void update_map()
{
	VECTOR vecMapOffset;
	
	var vMapPosX;
	var vMapPosY;
	var i;
	var x, y;
	
	if (vMapActive != 0)
	{
		vMapSize = screen_size.y * 0.33;
		vMapPosX = screen_size.x - vMapSize;
		vMapPosY = screen_size.y - vMapSize;
		for(i = 1; i <= vMapNodes; i++)
		{
			x = vMapPosX + (vNodeX[i] / vecMapMax.x) * vMapSize;
			y = vMapPosY + (vNodeY[i] / vecMapMax.y) * vMapSize;
				//printf("%d %d %d", (int)i, (int)x, (int)y);
			if (i == 1)
				draw_line(vector(x,y,0),NULL,100);
			else
				draw_line(vector(x,y,0),vector(255,255,255),100);
		}
		x = vMapPosX + (vNodeX[1] / vecMapMax.x) * vMapSize;
		y = vMapPosY + (vNodeY[1] / vecMapMax.x) * vMapSize;
		draw_line(vector(x,y,0),vector(255,255,255),100);
	}	
	
}

