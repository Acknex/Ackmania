/*
 *******************************************************************************
 * map.c
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

VECTOR vecMapMin;
VECTOR vecMapMax;
var* vNodeX;
var* vNodeY;
var vMapActive = 0;
var vMapScale;
var vMapNodes;
var vMap3DSize;
PANEL* panMapMarker[4];
PANEL** panMapSegments;


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

	vec_set(&vecMapMin, vector(999999,999999,0));
	vec_set(&vecMapMax, vector(-999999,-999999,0));
	
	ent = ent_create(NULL,nullvector,NULL);
	vMapNodes = path_next(ent);
	
	/* get path nodes */
	if(vMapNodes > 0)
	{
		vNodeX = sys_malloc(sizeof(var) * (vMapNodes+1));
		vNodeY = sys_malloc(sizeof(var) * (vMapNodes+1));
		for(i = 1; i <= vMapNodes; i++)
		{
			path_getnode(ent, i, &vecTemp, NULL);
			vNodeX[i] = vecTemp.x;
			vNodeY[i] = -vecTemp.y;
			
			/* track min/max path (race track) dimensions */
			if (vecMapMin.x > vNodeX[i]) vecMapMin.x = vNodeX[i];
			if (vecMapMin.y > vNodeY[i]) vecMapMin.y = vNodeY[i];
			if (vecMapMax.x < vNodeX[i]) vecMapMax.x = vNodeX[i];
			if (vecMapMax.y < vNodeY[i]) vecMapMax.y = vNodeY[i];
		}
		
		/* remove any offset, set relative to level origin */
		for(i = 1; i <= vMapNodes; i++)
		{
			vNodeX[i] -= vecMapMin.x;
			vNodeY[i] -= vecMapMin.y;
		}
		
		/* determine race track dimension (3D) */
		vTemp =  vecMapMax.x - vecMapMin.x;
		vMap3DSize = vecMapMax.y - vecMapMin.y;
		if (vTemp > vMap3DSize)
		{
			vMap3DSize = vTemp;
		}
		vMap3DSize *= 1.1; /* pretend on slightly bigger map for better look */
		//printf("%d %d %d %d %d", (int)vecMapMin.x, (int)vecMapMin.y, (int)vecMapMax.x, (int)vecMapMax.y, (int)vMap3DSize);

		/* normalize positions */
		for(i = 1; i <= vMapNodes; i++)
		{
			vNodeX[i] /= vMap3DSize;
			vNodeY[i] /= vMap3DSize;
		}
	
		/* create driver markers */
		for (i = 0; i < 4; i++)
		{
			panMapMarker[i] = pan_create(NULL, MAP_LAYER_MARKER);
			//panMapMarker[i]->flags |= SHOW|FILTER|LIGHT;
			panMapMarker[i]->flags |= SHOW|FILTER;
			panMapMarker[i]->size_x = MAP_MARKERSIZEX;
			panMapMarker[i]->size_y = MAP_MARKERSIZEY;
			panMapMarker[i]->center_x = panMapMarker[i]->size_x * 0.5;
			panMapMarker[i]->center_y = panMapMarker[i]->size_y * 0.5;
		}
		panMapMarker[0]->bmap = bmap_create("kartpanel1.tga");
		panMapMarker[1]->bmap = bmap_create("kartpanel2.tga");
		panMapMarker[2]->bmap = bmap_create("kartpanel3.tga");
		panMapMarker[3]->bmap = bmap_create("kartpanel4.tga");
		//vec_set(panMapMarker[0]->blue, vector(255,0,0));
		//vec_set(panMapMarker[1]->blue, vector(0,0,255));
		//vec_set(panMapMarker[2]->blue, vector(0,255,255));
		//vec_set(panMapMarker[3]->blue, vector(0,255,0));
	
		/* create track segments */
		panMapSegments = sys_malloc(sizeof(PANEL*) * vMapNodes);
		for(i = 0; i < vMapNodes; i++)
		{
			panMapSegments[i] = pan_create(NULL, MAP_LAYER);
			vec_set(panMapSegments[i]->blue, vector(100,100,100));
			panMapSegments[i]->flags |= SHOW|LIGHT;
		}
		
		wait(1);
		vMapScale = 0; //make sure whole map update will be triggered once (bleh)
		vMapActive = 1;
	}
	ptr_remove(ent);
}

void remove_map()
{
	var i;
	if (vMapActive == 1)
	{
		vMapActive = 0;
		for (i = 0; i < 4; i++)
		{
			ptr_remove(panMapMarker[i]->bmap);
			ptr_remove(panMapMarker[i]);
		}
		sys_free(vNodeX);
		sys_free(vNodeY);
		for (i = 0; i < vMapNodes; i++)
		{
			ptr_remove(panMapSegments[i]);
		}
		sys_free(panMapSegments);
	}
}

void update_map()
{
	var vMapPosX;
	var vMapPosY;
	var vMapSize;
	var i;
	var x, y;
	var vMapScaleOld;
	ENTITY* ent;
	VECTOR* vecTemp;		
	ANGLE angTemp;		
	
	if (vMapActive != 0)
	{
		vMapScaleOld = vMapScale;
		vMapScale = screen_size.y / 1200;	
		vMapSize = screen_size.y * 0.33;
		vMapPosX = screen_size.x - vMapSize;
		vMapPosY = screen_size.y - vMapSize;

#ifdef DRAW_LINE_MAP
		/* draw race track */
		for(i = 1; i <= vMapNodes; i++)
		{
			x = vMapPosX + (vNodeX[i] * vMapSize);
			y = vMapPosY + (vNodeY[i] * vMapSize);
			if (i > 1)
			{
				draw_line(vector(x,y,0),vector(255,255,255),100);
			}
			else
			{
				draw_line(vector(x,y,0),NULL,100);
			}
			
		}
		x = vMapPosX + (vNodeX[1] * vMapSize);
		y = vMapPosY + (vNodeY[1] * vMapSize);
		draw_line(vector(x,y,0),vector(255,255,255),100);
#endif

		/* draw player positions */
		for (i = 0; i < 4; i++)
		{
			ent = get_kart_driver(i);
			/* player positions are not offset corrected and normalized - so do it here */
			x = vMapPosX + ((ent->x - vecMapMin.x) / vMap3DSize) * vMapSize;
			y = vMapPosY + ((-ent->y - vecMapMin.y) / vMap3DSize) * vMapSize;
			panMapMarker[i]->scale_x = vMapScale;
			panMapMarker[i]->scale_y = vMapScale;
			panMapMarker[i]->pos_x = x - (panMapMarker[i]->size_x * 0.5 * panMapMarker[i]->scale_x);
			panMapMarker[i]->pos_y = y - (panMapMarker[i]->size_y * 0.5 * panMapMarker[i]->scale_y);
			panMapMarker[i]->angle = ent->pan;
		}

		/* this only needs to run once on resolution change */
		if(vMapScale != vMapScaleOld)
		{
			for(i = 0; i < vMapNodes; i++)
			{
				/* track length
				   length is defined by distance to next path node.
				   in order to avoid gaps, the length is increased by adding an overdraw value
				   the rotation center is set to half of this value to make sure  the
				   overdraw is used on both ends in x direction
				 */
				panMapSegments[i]->center_x = MAP_TRACK_OVERDRAW * vMapScale * 0.5;
				/* track width */
				panMapSegments[i]->size_y = MAP_TRACK_WIDTH * vMapScale;
				panMapSegments[i]->center_y = panMapSegments[i]->size_y * 0.5;
				/* get node position add offset, scale it to screen */
				panMapSegments[i]->pos_x = vMapPosX + (vNodeX[i+1] * vMapSize);
				panMapSegments[i]->pos_y = vMapPosY + (vNodeY[i+1] * vMapSize);
				/* track segments must be center aligned, offset can be used from center parameters */
				panMapSegments[i]->pos_x -= panMapSegments[i]->center_x;
				panMapSegments[i]->pos_y -= panMapSegments[i]->center_y;
				
				if(i > 0)
				{
					/* get length and direction for last track element */
					/* needs pos of current track element for direction calculation */
					vecTemp = vector(panMapSegments[i]->pos_x, panMapSegments[i]->pos_y, 0);
					vec_sub(vecTemp, vector(panMapSegments[i-1]->pos_x, panMapSegments[i-1]->pos_y, 0));
					panMapSegments[i-1]->size_x = vec_length(vecTemp) + (MAP_TRACK_OVERDRAW * vMapScale);
					vec_to_angle(&angTemp, vecTemp);
					panMapSegments[i-1]->angle = -angTemp.pan;
				}
			}
			/* repeat for last track element which needs reference to first node */
			vecTemp = vector(panMapSegments[0]->pos_x, panMapSegments[0]->pos_y, 0);
			vec_sub(vecTemp, vector(panMapSegments[i-1]->pos_x, panMapSegments[i-1]->pos_y, 0));
			panMapSegments[i-1]->size_x = vec_length(vecTemp) + (MAP_TRACK_OVERDRAW * vMapScale);
			vec_to_angle(&angTemp, vecTemp);
			panMapSegments[i-1]->angle = -angTemp.pan;
		}
	}	
	
}
