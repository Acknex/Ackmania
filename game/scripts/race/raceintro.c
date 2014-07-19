#ifndef _RACEINTRO_C_
#define _RACEINTRO_C_

ENTITY *raceintroNumber1 = {
	type = "numOne.mdl";
}
ENTITY *raceintroNumber2 = {
	type = "numTwo.mdl";
}
ENTITY *raceintroNumber3 = {
	type = "numThree.mdl";
}
ENTITY *raceintroNumber4 = {
	type = "numExc.mdl";
}

SOUND *raceintroBeep = "beep.ogg";

void raceintro_trigger(void *callback)
{
	var i;
	raceintroNumber1.flags2 |= SHOW;
	
	var i = 0;
	for(i = 0; i < 90; i += time_step) {
	
		vec_set(raceintroNumber1.x, vector(0.5 * screen_size.x, 0.5 * screen_size.y, 100 + i));
		rel_for_screen(raceintroNumber1.x, camera);
		
		wait(1);
	}
	
	raceintroNumber1.flags2 &= ~SHOW;
	
	if(callback != NULL) {
		void tmp();
		tmp = callback;
		tmp();
	}
}

#endif // #ifndef _RACEINTRO_C_