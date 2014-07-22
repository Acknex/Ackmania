MATERIAL *perPixelLighting =
{
	effect = "perPixel.fx";
}

void p_alphafade(PARTICLE *p)
{
	p.alpha -= p.skill_a * time_step;
	if(p.alpha <= 0) p.lifespan = 0;
	p.skill_b += 0.1 * time_step;
	if(p.skill_b > 1)
	{
		p.vel_x = 0;
		p.vel_y = 0;
	}
}

void p_alphafade_smoke(PARTICLE *p)
{
	p.alpha -= p.skill_a * time_step;
	if(p.alpha <= 0) p.lifespan = 0;
}

void flame(PARTICLE *p)
{
	p.vel_x = random(2) -1;
	p.vel_y = random(2) -1;
	p.vel_z = 1 + random(1);
	p.alpha = 100;
	p.size = 10 + random(10);
	p.skill_a = 3;
	var l = random(100);
	if(l < 50)
	{
		p.red = 255;
		p.green = 192;
		p.blue = 0;
	}
	else
	{
		p.red = 243;
		p.green = 27;
		p.blue = 27;
	}
	
	p.flags = MOVE | BRIGHT |TRANSLUCENT;
	p.event = p_alphafade;
}

void smoke(PARTICLE *p)
{
	p.vel_x = random(1) -0.5;
	p.vel_y = random(1) -0.5;
	p.vel_z = 1 + random(1);
	p.alpha = 100;
	p.size = 10 + random(10);
	p.skill_a = 3;
	var r = random(10);
	p.red = 30 + r;
	p.green = 30 + r;
	p.blue = 30 + r;
	p.flags = MOVE |TRANSLUCENT;
	p.event = p_alphafade_smoke;
}

action actTorch()
{
	VECTOR tip;
	vec_for_vertex(tip, my, 2);
	ENTITY *dummy = ent_create("sphere.mdl", tip, NULL);
	vec_scale(dummy.scale_x, 1);
	dummy.flags = INVISIBLE | PASSABLE;
	dummy.skill_a = 100;
	dummy.skill_b = 0;
	dummy.red = 255;
	dummy.green = 192;
	dummy.blue = 0;
	dummy.clipfactor = 9000;
	
	var dist = 0;
	var target_d = 0;
	
	var vParticles = 0;	
	while(1)
	{
		vParticles += time_step;
		if (vParticles > 3)
		{
			if (vec_dist(player.x, my.x) < 1500)
			{
				effect(flame, 3, dummy.x, nullvector);
				effect(smoke, 3, vector(dummy.x + random(1), dummy.y + random(1), dummy.z + 20), nullvector);
			}
			vParticles -= 3;
		}
		wait(1);
	}
}

void p_alphafade_lava(PARTICLE *p)
{
	p.size += time_step;
	p.alpha -= p.skill_a * time_step;
	if(p.alpha <= 0) p.lifespan = 0;
	p.vel_x = -0.1 + random(0.2);
	p.vel_y = -0.1 + random(0.2);
}

void lava(PARTICLE *p)
{
	p.vel_x = random(0.3) -0.2;
	p.vel_y = random(0.3) -0.2;
	p.vel_z = 1 + random(1);
	
	p.x += -950 + random(1900);
	p.y += -125 + random(250);
	p.z += random(100);
	
	p.alpha = 100;
	p.size = 90 + random(10);
	p.skill_a = 2;
	var r = random(100);
	if(r < 90)
	{
		p.red = 200 + random(10);
		p.green = 10;
		p.blue = 10;
		p.flags = MOVE | TRANSLUCENT;
	}
	else if( r < 95)
	{
		p.red = 255;
		p.green = 166;
		p.blue = 0;
		p.flags = MOVE |TRANSLUCENT;
	}
	else 
	{
		p.red = 1;
		p.green = 1;
		p.blue = 1;
		p.flags = MOVE;
	}
	p.event = p_alphafade_lava;
}


action actLavaParticle()
{
	my.flags = PASSABLE | INVISIBLE;
	var vParticles = 0;
	while(1)
	{
		vParticles += time_step;
		if (vParticles > 2)
		{
			if(player != NULL)
			{
			if (vec_dist(player.x, my.x) < 2000)
			{
				effect(lava, 45 * time_step, my.x, nullvector);
			}
			}
			vParticles -= 2;
		}
		wait(1);
	}
}

void p_alphafade_water(PARTICLE *p)
{
	p.alpha -= p.skill_a * time_step;
	if(p.alpha <= 0) p.lifespan = 0;
	p.vel_x = random(0.125) -0.3;
	p.vel_y = random(0.125) -0.3;
}

void water(PARTICLE *p)
{
	p.vel_x = random(0.125) -0.3;
	p.vel_y = random(0.125) -0.3;
	p.vel_z = 3;
	
	p.x += -125 + random(250);
	p.y += -125 + random(250);
	
	p.alpha = 100;
	p.size = 50 + random(10);
	p.skill_a = 1;
	var r = random(100);
	if(r < 50)
	{
		p.red = 0;
		p.green = 128;
		p.blue = 128;
		p.flags = MOVE | TRANSLUCENT;
	}
	else
	{
		p.red = 182;
		p.green = 196;
		p.blue = 203;
		p.flags = BRIGHT | MOVE | TRANSLUCENT;
	}
	p.event = p_alphafade_water;
}

action actWaterParticle()
{
	my.flags |= PASSABLE | TRANSLUCENT;
	my.alpha = 90;
	var p = 0;
	while(1)
	{
		ent_animate(me, "w1_", p, ANM_CYCLE);
		p += 20 * time_step;
		p %= 100;
//		effect(water, 15, my.x, nullvector);
		wait(8);
	}
}