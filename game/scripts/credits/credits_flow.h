#ifndef _CREDITS_FLOW_H_
#define _CREDITS_FLOW_H_

#define CREDITS_DEF_MAXSTAGES 64

typedef struct {
	var x, y, z;
	var pan, tilt, roll;
	var speed;
} _CreditCameraStage;

int credits_stageCount;
_CreditCameraStage credits_stages[CREDITS_DEF_MAXSTAGES];

void _add_stage(var x, var y, var z, var pan, var tilt, var roll, var speed) {
	
	credits_stages[credits_stageCount].x = x;
	credits_stages[credits_stageCount].y = y;
	credits_stages[credits_stageCount].z = z;
	
	credits_stages[credits_stageCount].pan = pan;
	credits_stages[credits_stageCount].tilt = tilt;
	credits_stages[credits_stageCount].roll = roll;
	
	credits_stages[credits_stageCount].speed = speed;
	
	credits_stageCount += 1;
}

void credits_initstages() {
	_add_stage(175, -2211, 531, 94, -13, 0, 1);
	_add_stage(-175, -1211, 531, 86, -24, 0, 1);
	_add_stage(362.563, -104.068, 370.712, 119.317, -20.674, 0, 1);
	_add_stage(-244.373, 178.449, 248.488, 56.722, -14.012, 0, 1);
	_add_stage(287.897, 340.530, 181.141, 144.803, -7.349, 0, 1);
	_add_stage(330.821, 601.306, 181.150, 198.104, -7.349, 0, 1);
	_add_stage(2.029, 661.280, 210.628, 270.368, -13.986, 0, 1);
	_add_stage(-234.618, 629.067, 208.233, 294.617, -13.986, 0, 1);
	_add_stage(-439.249, 854.975, 578.169, 296.664, -32.027, 0, 1);
	_add_stage(-742.185, -41.973, 472.564, 397.829, -22.253, 0, 1);
	_add_stage(-267.319, -339.175, 1083.993, 431.818, -60.420, 0, 1);
	_add_stage(556.372, -232.182, 246.143, 484.703, 11.090, 0, 1);
	_add_stage(785.279, -562.790, 54.465, 484.703, 12.754, 0, 1);
}

TEXT *creditsContent = {
	string(
		"Jonas Freiknecht", "Armory Supervisor", "Item Supervisor", "Pyrotechnician"
		"Felix Queissner", "Ingame Selection Maker", "Credit Writer", "",
		"Andreas Schmidt", "Credit Designer", "", "",
		"Robert Jäger", "2D Display Maker", "2D Map Minificator", "Chasing Camera Holder",
		"Christian Behrenberg", "Lower Management", "Sound Effect Voice Actor", "Framework Code Creator",
		"Marius Kay Appes", "Upper Management", "Music Composer", ""
		"Michael Schwarz", "Voice Actor", "Level Designer", "",
		"Simon Trümpler", "Chief 3D Designer", "", "",
		"Patrick Jendral", "Detail Creationist", "3D Font Maker", "",
		"Felix Pohl", "Cart Movement Enabler", "Armory Manager", ""
		"CocaCola", "Gardener", "Cloud Maker", ""
		"Kanal 21", "Room Provider", "Internet Provider", "",
		"Achim Lübbeke", "Bielefeld Connections", "", "",
		"Lottifant", "Missing Link", "", "",
		"Eselslerche", "Flown Out", "", "",
		"JCL", "Indirect sponsor of this game", "", "");
}

#endif // #ifndef _CREDITS_FLOW_H_