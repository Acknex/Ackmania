/*
 *******************************************************************************
 * keys.c
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
 * definition script for key bindings
 *
 * Comments
 * 
 * for short descriptions see comments in keys.h
 *
 *******************************************************************************
 */


void def_video(); 
void def_screen(); 
void def_shot();
void change_camera();


void bind_keys_startup()
{
	/* default keys */
	if (!on_f5) on_f5 = def_video;
	if (!on_f6) on_f6 = def_shot;
	if (!on_f7) on_f7 = change_camera;
	while(1)
	{
		def_screen();
		wait(1);
	}
}

var def_screen_lock = 0;
void def_screen() 
{
	if (!key_alt && !key_enter) 
		def_screen_lock = 0;
		
	if (!key_alt || !key_enter || def_screen_lock == 1) 
		return;
	var mode = video_screen + 1;
	if (mode > 2) 
		mode = 1;
	if (video_switch(0,0,mode) == 0) 
		beep();
	def_screen_lock = 1;
}

/* all stolen from default.c */
void def_video() 
{
	var mode = video_mode;
	while(1) {
		if (!key_shift) 
			mode++; 
		else 
			mode--;
		mode = cycle(mode,6,13); 
		if (video_switch(mode,0,0)) 
			break;
   }
}

var def_shot_num = 0;
void def_shot() 
{ 
	file_for_screen("shot_",def_shot_num); 
	beep(); 
	def_shot_num++; 
}
