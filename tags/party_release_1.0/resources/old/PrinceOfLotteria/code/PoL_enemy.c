
void PoL_enemy_init()
{
	if(my.PoL_enemy_speed==0)
	my.PoL_enemy_speed=PoL_enemy_defaultspeed;
	if(my.PoL_enemy_health==0)
	my.PoL_enemy_health=PoL_enemy_defaulthealth;
	my.PoL_enemy_akdir=1;
	my.PoL_enemy_intcalc=90;
	c_setminmax(my);
	wait(1);
	my.min_x=-8;
	my.max_x=8;
	my.min_y=-20;
	my.max_y=20;
	my.PoL_enemy_tdist=my.min_z+5;
	my.min_z/=2;
	
	my.PoL_attacktriggered=0;
	my.PoL_enemy_intcalcdir=1;
	my.PoL_enemy_anmperc=0;
	set(me, PoL_is_enemy);
	//set (me, PASSABLE);
}

void PoL_enemy_applygravity()
{
	var d=c_trace(my.x,vector(my.x,my.y,my.z+my.PoL_enemy_tdist*my.scale_z),IGNORE_ME|IGNORE_PASSABLE);
	if(!trace_hit)
	{
		c_move(my,NULL,vector(0,0,-PoL_enemy_gravity_force*time_step),IGNORE_PASSABLE);
	}
	else
	{
		c_move(my,NULL,vector(0,0,(-my.PoL_enemy_tdist*my.scale_z-d)*time_step),IGNORE_PASSABLE);
	}
}

void PoL_enemy_checkattack()
{
	if(vec_dist(my.x,player.x)<PoL_enemy_playerattackrange)
	if(sign(player.x-my.x)==sign(my.PoL_enemy_akdir))
	{
		player.PL_HEALTH-=1;
	}
}

void PoL_enemy_animate(var d_move)
{
	if(my.PoL_attacktriggered==1)
	{
		my.PoL_attackanm=minv(my.PoL_attackanm+PoL_enemy_attackanimpercpertick*time_step,100);
		ent_animate(my,PoL_enemy_attackanimname,my.PoL_attackanm,NULL);
		if(my.PoL_attackanm>=100)
		{
			my.PoL_attacktriggered=-PoL_enemy_attackcooldown;
			PoL_enemy_checkattack();
		}
	}
	else
	{
		if(d_move>0)
		{
			my.PoL_enemy_anmperc=cycle(my.PoL_enemy_anmperc+PoL_enemy_walkanimspeed*d_move,0,100);
			ent_animate(my,PoL_enemy_walkanimname,my.PoL_enemy_anmperc,ANM_CYCLE);
		}
		else
		{
			my.PoL_enemy_anmperc=cycle(my.PoL_enemy_anmperc+PoL_enemy_waitanimpercpertick*time_step,0,100);
			ent_animate(my,PoL_enemy_waitanimname,my.PoL_enemy_anmperc,ANM_CYCLE);
		}
	}
}

void PoL_enemy_move()
{
	if(my.PoL_enemy_akdir==0||my.PoL_attacktriggered==1)
	{
		PoL_enemy_animate(0);
		return;
	}
	//reset(my,PASSABLE);
	PoL_enemy_animate(c_move(my,NULL,vector(my.PoL_enemy_speed*time_step*my.PoL_enemy_akdir,0,0),GLIDE|IGNORE_PASSABLE));
	//set(my,PASSABLE);
	my.y = player.y; //this hack was sponsored by firo
}

void PoL_enemy_update_dir()
{
	if(my.PoL_enemy_akdir!=my.PoL_enemy_intcalcdir)
	{
		if(my.PoL_enemy_intcalcdir==0)
		{
			my.PoL_enemy_intcalc-=sign(my.PoL_enemy_intcalc)*minv(abs(my.PoL_enemy_intcalc),PoL_enemy_changedirspeed*time_step*0.2);
		}
		else
		{
			my.PoL_enemy_intcalc+=my.PoL_enemy_intcalcdir*PoL_enemy_changedirspeed*time_step;
			if(abs(my.PoL_enemy_intcalc)>90)
			my.PoL_enemy_intcalc=sign(my.PoL_enemy_intcalc)*90;
		} 
		
		my.PoL_enemy_akdir=sin(my.PoL_enemy_intcalc);
		my.pan=my.PoL_enemy_intcalc-90;
	}
}

void PoL_enemy_detectturningpoints()
{
	if(sign(my.PoL_enemy_akdir)!=my.PoL_enemy_intcalcdir)
	return;
	/*VECTOR tempvec;
	vec_set(tempvec,my.x);
	if(vec_to_screen(tempvec,camera))
	draw_line(tempvec,vector(0,255,0),0);
	vec_set(tempvec,vector(my.x+PoL_enemy_checkaheadfactor*sign(my.PoL_enemy_akdir)*(my.max_x-my.min_x),my.y,my.z));
	if(vec_to_screen(tempvec,camera))
	draw_line(tempvec,vector(0,255,0),100);*/
	
	c_trace(my.x,vector(my.x+PoL_enemy_checkaheadfactor*sign(my.PoL_enemy_akdir)*(my.max_x-my.min_x),my.y,my.z),IGNORE_ME|IGNORE_PASSABLE);
	//return;
	if(trace_hit&&you!=player)
	{
		my.PoL_enemy_intcalcdir*=-1;
		return;
	}
	
	/*vec_set(tempvec,vector(my.x+PoL_enemy_checkaheadfactor*sign(my.PoL_enemy_akdir)*(my.max_x-my.min_x),my.y,my.z+my.min_z*2*PoL_enemy_checkaheadfactor));
	if(vec_to_screen(tempvec,camera))
	draw_line(tempvec,vector(0,255,0),100);*/
	
	var res=c_trace(vector(my.x+PoL_enemy_checkaheadfactor*sign(my.PoL_enemy_akdir)*(my.max_x-my.min_x),my.y,my.z),
	vector(my.x+PoL_enemy_checkaheadfactor*sign(my.PoL_enemy_akdir)*(my.max_x-my.min_x),my.y,my.z+my.min_z*1.5*PoL_enemy_checkaheadfactor),IGNORE_ME|USE_BOX|IGNORE_PASSABLE);
	if(!trace_hit&&res==0)
	{
		//error("hier");
		my.PoL_attacktriggered=-PoL_enemy_attackcooldown;
		my.PoL_enemy_intcalcdir*=-1;
		return;
	}
}

void PoL_enemy_playerdetector()
{
	if(player==NULL)return;
	if(my==player)return;
	
	my.PoL_enemyseeplayer=0;
	if(my.PoL_attacktriggered<0)
	{
		my.PoL_attacktriggered+=time_step;
		if(my.PoL_attacktriggered>0)
		my.PoL_attacktriggered=0;
		return; //remove if the should not go away from player after performing an attack
	}
	
	var dist=vec_dist(my.x,player.x);
	if(dist<PoL_enemy_playerdetectrange)
	if(abs(my.z-player.z)<PoL_enemy_playerdetectrangevertical)
	{
		my.PoL_enemyseeplayer=1;
		my.PoL_enemy_intcalcdir=sign(player.x-my.x);
		if(dist<PoL_enemy_diseredattackdist&&my.PoL_attacktriggered==0)
		{
			my.PoL_attacktriggered=1;
			my.PoL_attackanm=0;
		}
	}
}

void PoL_enemy_waitlogic()
{
	if(my.PoL_enemyseeplayer==0)
	if(my.PoL_waittimer<0)
	{
		my.PoL_waittimer+=time_step;
		if(my.PoL_waittimer>=0)
		{
			my.PoL_waittimer=PoL_maxwaitticks;
			my.PoL_enemy_intcalcdir=0;
		}
	}
	else
	{
		my.PoL_waittimer-=time_step;
		if(my.PoL_waittimer<0)
		{
			my.PoL_waittimer=-random(POL_randomwaittimer);
			my.PoL_enemy_intcalcdir=sign(random(1));
		}
	}
}

void PoL_enemy_die()
{
	var anim=0;
	if(my==player)
	player=NULL;
	PoL_enemy_dead_enemies++;
	set(my,PASSABLE);
	while(anim<100&&my)
	{
		anim+=time_step*PoL_enemy_deathanimpercpertick;
		ent_animate(my,PoL_enemy_deathanimname,anim,NULL);
		wait(1);
	}
}

action PoL_enemy()
{
	PoL_enemy_init();
	wait(1);
	vec_scale(my.scale_x,2);//remove later
	
	while(my)
	{
		if(my.PoL_enemy_health<=0)
		{
			PoL_enemy_die();
			return;
		}
		PoL_enemy_applygravity();
		PoL_enemy_playerdetector();
		PoL_enemy_detectturningpoints();
		
		PoL_enemy_waitlogic();
		
		PoL_enemy_update_dir();
		PoL_enemy_move();
		wait(1);
	}
}