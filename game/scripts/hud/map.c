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
#include "startgrid.h"

var vMapScale;
VECTOR vecMapMin;
VECTOR vecMapMax;
var* vNodeX;
var* vNodeY;
var vMapActive = 0;
var vMapNodes;
var vMap3DSize;
PANEL* panMapMarker[4];

void map_particle(PARTICLE *p); 

void create_map()
{
	ENTITY* ent;
	VECTOR vecTemp;
	var i;
	var vTemp;
	
	while(level_ent == NULL)
	{
		wait(1);
	}

//	vec_set(&vecMapMin, level_ent->min_x);
//	vec_set(&vecMapMax, level_ent->max_x);
	vec_set(&vecMapMin, vector(999999,999999,0));
	vec_set(&vecMapMax, vector(-999999,-999999,0));
//	vecMapMin.z = 0;
//	vecMapMax.z = 0;
//	vec_scale(&vecMapMin, -1);
	
	ent = ent_create(NULL,nullvector,NULL);
	vMapNodes = path_next(ent);
	
	/* get path nodes with 0/0 offset */
	if(vMapNodes > 0)
	{
		vNodeX = sys_malloc(sizeof(var) * (vMapNodes+1));
		vNodeY = sys_malloc(sizeof(var) * (vMapNodes+1));
		for(i = 1; i <= vMapNodes; i++)
		{
			path_getnode(ent, i, &vecTemp, NULL);
			vNodeX[i] = vecTemp.x;
			vNodeY[i] = -vecTemp.y;
			if (vecMapMin.x > vNodeX[i]) vecMapMin.x = vNodeX[i];
			if (vecMapMin.y > vNodeY[i]) vecMapMin.y = vNodeY[i];
			if (vecMapMax.x < vNodeX[i]) vecMapMax.x = vNodeX[i];
			if (vecMapMax.y < vNodeY[i]) vecMapMax.y = vNodeY[i];
		}
		
		for(i = 1; i <= vMapNodes; i++)
		{
			vNodeX[i] += vecMapMin.x;
			vNodeY[i] += vecMapMin.y;
		}
		vTemp = vecMapMin.x + vecMapMax.x;
		vMap3DSize = vecMapMin.y + vecMapMax.y;
		if (vTemp > vMap3DSize)
		{
			vMap3DSize = vTemp;
		}
	}
	//else
	//{
	//	error("no path in level! Crashes coming up");
	//}				
	//vec_add(&vecMapMax, &vecMapMin);
	//vec_scale(&vecMapMax, 0.9); /* looks better */
	//vec_set(&vecMapMin, nullvector); //old value needed in update function

	/* create panels */
	for (i = 0; i < 4; i++)
	{
		panMapMarker[i] = pan_create(NULL, MAP_LAYER);
		panMapMarker[i]->flags |= SHOW|LIGHT;
	}
	vec_set(panMapMarker[0]->blue, vector(255,0,0));
	vec_set(panMapMarker[1]->blue, vector(0,0,255));
	vec_set(panMapMarker[2]->blue, vector(0,255,255));
	vec_set(panMapMarker[3]->blue, vector(0,255,0));

	wait(1);
	vMapActive = 1;
	ptr_remove(ent);
}

void remove_map()
{
	vMapActive = 0;
	ptr_remove(panMapMarker[0]);
	ptr_remove(panMapMarker[1]);
	ptr_remove(panMapMarker[2]);
	ptr_remove(panMapMarker[3]);
	sys_free(vNodeX);
	sys_free(vNodeY);
}

void update_map()
{
	VECTOR vecMapOffset;
	
	var vMapPosX;
	var vMapPosY;
	var vMapSize;

	var i;
	var x, y;
	var xNew, yNew;
	ENTITY* ent;
	var vMapScale;
	
	if (vMapActive != 0)
	{
		vMapScale = screen_size.y / 1200;	
		vMapSize = screen_size.y * 0.33;
		vMapPosX = screen_size.x - vMapSize;
		vMapPosY = screen_size.y - vMapSize;

		/* draw race track */
		for(i = 1; i <= vMapNodes; i++)
		{
			x = vMapPosX + (vNodeX[i] / vMap3DSize) * vMapSize;
			y = vMapPosY + (vNodeY[i] / vMap3DSize) * vMapSize;
			if (i > 1)
			{
				draw_line(vector(x,y,0),vector(255,255,255),100);
			}
			else
			{
				draw_line(vector(x,y,0),NULL,100);
			}
			
		}
		x = vMapPosX + (vNodeX[1] / vMap3DSize) * vMapSize;
		y = vMapPosY + (vNodeY[1] / vMap3DSize) * vMapSize;
		draw_line(vector(x,y,0),vector(255,255,255),100);

		/* draw player positions */
		for (i = 0; i < 4; i++)
		{
			ent = get_kart_driver(i);
			x = vMapPosX + ((ent->x + vecMapMin.x) / vMap3DSize) * vMapSize;
			y = vMapPosY + ((-ent->y  + vecMapMin.y) / vMap3DSize) * vMapSize;
			panMapMarker[i]->scale_x = vMapScale;
			panMapMarker[i]->scale_y = vMapScale;
			panMapMarker[i]->pos_x = x - (panMapMarker[i]->size_x * 0.5 * panMapMarker[i]->scale_x);
			panMapMarker[i]->pos_y = y - (panMapMarker[i]->size_y * 0.5 * panMapMarker[i]->scale_y);
			panMapMarker[i]->size_x = MAP_MARKERSIZE;
			panMapMarker[i]->size_y = MAP_MARKERSIZE;
		}
	}	
	
}

