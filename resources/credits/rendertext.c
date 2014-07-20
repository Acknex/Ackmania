#include <acknex.h>

STRING* ackcon_str = "AckCon Open Air 2014";

TEXT* ackcon_txt =	
{
	layer 	= 	1;
  	pos_x 	= 	10;
  	pos_y 	= 	10;
  	red 	= 	255;
  	green 	= 	255;
  	blue 	= 	255;
  	string 	(ackcon_str);
  	flags 	= 	SHOW;
} 

void main (void)
{
	level_load(NULL);
	file_for_screen("shot.tga",0);
}