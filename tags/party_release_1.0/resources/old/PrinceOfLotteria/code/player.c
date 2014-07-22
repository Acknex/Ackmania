#ifndef _PLAYER_C_
#define _PLAYER_C_

#include "gui.h"
#include "PoL_enemy.h"
#include "zorroMesh.h"
#include "traps.h"
#include "platform.h"
#include "sky.h" //fix by firo

//#define PL_DEBUG
// -----------------------------------------------------------------------------------
// Particles
// -----------------------------------------------------------------------------------

void pAlphaFade(PARTICLE *p) {
	p.alpha -= p.skill_a*time_step;
	if (p.alpha <= 0) {
		p.lifespan = 0;
	}
}

void pJetpack(PARTICLE *p) {
	VECTOR vecTemp;
	p.skill_a = 10+random(4);
	vec_randomize(vecTemp, 10);
	vec_add (p.vel_x, vecTemp);
	p.vel_z = -20;
	vec_set(p.blue, vector(255,255,255));
	set(p, MOVE | TRANSLUCENT | BRIGHT);
	p.lifespan = 1+random(4);
	p.size = 40+random(10);
	p.event = pAlphaFade;
}

void pBlood(PARTICLE *p) {
	VECTOR vecTemp;
	p.skill_a = 10+random(4);
	vec_randomize(vecTemp, 10);
	p.bmap = bmapBlood;
	vec_add (p.vel_x, vecTemp);
	//p.vel_z = -20;
	//vec_set(p.blue, vector(255,255,255));
	set(p, MOVE | TRANSLUCENT);
	p.lifespan = 1+random(4);
	p.size = 40+random(10);
	p.event = pAlphaFade;
}


// -----------------------------------------------------------------------------------
// player sounds
// -----------------------------------------------------------------------------------

SOUND* snd_narrator_death_spikes = "death_by_spikes.wav";
SOUND* snd_narrator_death_drown  = "death_by_drowning.wav";
SOUND* snd_narrator_death_fire   = "death_by_fire.wav";
SOUND* snd_narrator_death_lava   = "death_by_lava.wav";
SOUND* snd_narrator_death_stones = "death_by_rock.wav";

// -----------------------------------------------------------------------------------
// Player code
// -----------------------------------------------------------------------------------

void animate_player(var _speed) {
	if (!me.PL_IS_JUMPING || key_space) {
		if (_speed != 0)	{
			if (me.PL_IS_JUMPING == 0 && !key_space) {
				my.PL_ANIMATION_PERCENTAGE += 10 * time_step;
				ent_animate(me,"walk ",my.PL_ANIMATION_PERCENTAGE,ANM_CYCLE);
			}
		} else { 
			my.PL_ANIMATION_PERCENTAGE += 3 * time_step;
			ent_animate(me,"idle ",my.PL_ANIMATION_PERCENTAGE,ANM_CYCLE);
		}
	} else {
		my.PL_ANIMATION_PERCENTAGE += 3 * time_step;
		ent_animate(me,"jump ",my.PL_ANIMATION_PERCENTAGE,ANM_CYCLE);
	}
	
	if (me.PL_IS_ATTACKING) {
		my.PL_ATTACKING_PERCENTAGE +=10*time_step;
		ent_animate(me, "attack ", my.PL_ATTACKING_PERCENTAGE, ANM_ADD);
	}
}

void move_player() {
	
	int nTraceDown = 0;
	int nTraceUp = 0;
	var vFallSpeed = 0;
	int nSndRandom = 0;
	VECTOR vecPlayerMoveSpeed;
	vec_set(vecPlayerMoveSpeed,nullvector);
	
	while(1) {
		player.PL_JUMP_HEIGHT = PL_MAX_JUMP_HEIGHT;
		if(key_force.x > 0){my.pan = 0;}
		if(key_force.x < 0){my.pan = -180;}
		
		// Move player with cursor keys
		vecPlayerMoveSpeed.x = 35 * (key_cur - key_cul) * time_step;
		vecPlayerMoveSpeed.y = 0;
		
		// Jump on curor up AND if player doesn't jump atm
		if ((key_cuu == 1) && (player.PL_IS_JUMPING == 0)) {
			player.PL_IS_JUMPING = 1;
			my.PL_ANIMATION_PERCENTAGE = 0;
			nSndRandom = integer(random(5));
			switch(nSndRandom) {
				case 0: snd_play(sndJump01, 100, 0); break;
				case 1: snd_play(sndJump02, 100, 0); break;
				case 2: snd_play(sndJump03, 100, 0); break;
				case 3: snd_play(sndJump04, 100, 0); break;
				case 4: snd_play(sndJump05, 100, 0); break;
			}
		}
		
		// If player is jumping...
		if (player.PL_IS_JUMPING == 1) {
			#ifdef PL_DEBUG
				draw_text("Is jumping", 10, 60, COLOR_BLUE);
			#endif
			
			// ... and should keep jumping ...
			if (player.PL_JUMP_TIME > -1) {
				
				// ... then move him up in the air
				player.PL_JUMP_TIME -= 0.2 * time_step;
				vecPlayerMoveSpeed.z = player.PL_JUMP_HEIGHT * time_step * player.PL_JUMP_TIME;
			}
		}
	
		// Using jetpack?
		if (flying_man) {
			if (key_space) {
				if (fPlayerJetpack > 0) {
					
					// Play jetpack loop	
					if (!snd_playing(vPlayerSndHandle)) {
						vPlayerSndHandle = ent_playsound(player, sndJetpackLoop, 500);
					}
					
					// Play wohoooo
					if (vPlayerWoho == 0) {
						nSndRandom = integer(random(100));
						if (nSndRandom < 25) {
							snd_play(sndJetpack01, 1000, 0);
						} else {
							if (nSndRandom < 50) {
								snd_play(sndJetpack02, 1000, 0);
							} else {
								if (nSndRandom < 75) {
									snd_play(sndJetpack03, 1000, 0);
								} else {
									if (nSndRandom < 98) {
										snd_play(sndJetpack04, 1000, 0);
									} else {
										snd_play(sndJetpack05, 1000, 0);
									}
								}
							}
						}
						vPlayerWoho = 1;
					}
					player.PL_IS_JUMPING = 1;
					player.PL_JUMP_TIME += 0.25 * time_step;
					
					#ifdef PL_DEBUG
						draw_text("Accel up:", 10, 330, COLOR_RED);
						draw_text(str_for_num(NULL, player.PL_JUMP_TIME), 200, 330, COLOR_RED);
					#endif
					
					player.PL_JUMP_TIME = minv(player.PL_JUMP_TIME, PL_JETPACK_MAX_STRENGTH);
					fPlayerJetpack -=0.1 * time_step;
					fPlayerJetpackCooldown = 10;
					vecPlayerMoveSpeed.z += player.PL_JUMP_TIME * time_step;
					
					if (my.pan == 0) {
						effect(pJetpack,maxv(1,10*time_frame),vector(player.x-10,player.y,player.z+5),vector(0,0,-3));
					} else {
						effect(pJetpack,maxv(1,40*time_frame),vector(player.x+10,player.y,player.z+5),vector(0,0,-3));
					}
				}
			} else {
				
				if (vPlayerSndHandle != 0) snd_stop(vPlayerSndHandle);
				// Regenerate jetpack
				if (fPlayerJetpackCooldown > 0) {
					fPlayerJetpackCooldown -=0.4 * time_step;
					if (fPlayerJetpackCooldown < 0) {
						fPlayerJetpackCooldown = 0;
					}
				} else {
					if (fPlayerJetpack < PL_JETPACK_MAX_FUEL) {
						fPlayerJetpack +=PL_JETPACK_REFRESH_STEP * time_step;
					}
				}
			}
		}
		
		gui_update_jetpack(100 / PL_JETPACK_MAX_FUEL * fPlayerJetpack);
		gui_update_hearts();
		gui_update_a4();
		
		// Jumping below a ceiling
		nTraceUp = c_trace(player.x, vector(player.x,player.y,player.z+5000), IGNORE_ME | IGNORE_PASSABLE | USE_BOX );

		#ifdef PL_DEBUG
			draw_text("Jetpack:", 10, 10, COLOR_RED);
			draw_text("Jetpack cooldown:", 10, 40, COLOR_RED);
			draw_text(str_for_float(NULL, fPlayerJetpack), 180, 10, COLOR_RED);
			draw_text(str_for_float(NULL, fPlayerJetpackCooldown), 180, 40, COLOR_RED);
		#endif
		
		// If player jumps against the ceiling...
		/*if (nTraceUp < 10) {
			
			// And doesn't use the jetpack...
			if (!key_space || fPlayerJetpack <= 0) {
				
				// Let him fall
				player.PL_JUMP_TIME = -0.1;
				vecPlayerMoveSpeed.z = player.PL_JUMP_HEIGHT * time_step * player.PL_JUMP_TIME;
			} else {
				vecPlayerMoveSpeed.z = -0.1 * time_step; // error 0
			}
		}*/
		
		// Can fall down?
		nTraceDown = c_trace(player.x, vector(player.x,player.y,player.z-5000), IGNORE_ME | IGNORE_PASSABLE | USE_BOX);
		
		// When on platform, move the payer with it
		if (you != NULL && nTraceDown < 10) {
			if (is(you, is_platform)) {
				
				#ifdef PL_DEBUG
					draw_text("Auf Plattform", 10, 130, COLOR_RED);
					draw_text(str_for_num(NULL, you->absspeed_x), 10, 160, COLOR_RED);
					draw_text(str_for_num(NULL, you->absspeed_y), 10, 190, COLOR_RED);
					draw_text(str_for_num(NULL, you->absspeed_z), 10, 220, COLOR_RED);
				#endif
				vec_add(player.x, you->absspeed_x);
			}
		}
		
		// If there is space under the player let him fall
		if(nTraceDown > 15 || !HIT_TARGET) {
			#ifdef PL_DEBUG
				draw_text("Should be falling", 10, 250, COLOR_GREEN);
				draw_text(str_for_num(NULL, vecPlayerMoveSpeed.z), 150, 250, COLOR_GREEN);
			#endif
			//accelerate(vecPlayerMoveSpeed.z, -PL_GRAVITY * time_step, 0.2);
			vecPlayerMoveSpeed.z += accelerate(vFallSpeed, -PL_GRAVITY * time_step, 0.2);
		} else {
			// If player is on ground ...
			if((nTraceDown <= 10)) {
				// Allow him to jump again
				if (!key_cuu) {
					player.PL_IS_JUMPING = 0;
					player.PL_JUMP_TIME = PL_MAX_JUMP_TIME;
					vFallSpeed = 0;
					
					if (!key_space) vPlayerWoho = 0;
				}
			}	
		}
		
		// Fallen in spikes?
		nTraceDown = c_trace(player.x, vector(player.x,player.y,player.z-10), IGNORE_ME | USE_BOX);
		if (HIT_TARGET) {
			if (hit.entity != NULL) {
				if (is(hit.entity, is_trap)) {
					my.PL_HEALTH = 0;
					effect(pBlood,maxv(1,10*time_frame),vector(my.x, my.y-10, my.z) ,vector(0,0,-3));
					#ifdef PL_DEBUG
						draw_text("DEAD", 10, 250, COLOR_RED);
					#endif
					switch(hit.entity.skill1)
					{
						case 0:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_spikes, 100, 0);
							break;
						case 1:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_drown, 100, 0);
							break;
						case 2:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_fire, 100, 0);
							break;
						case 3:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_lava, 100, 0);
							break;
						case 4:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_stones, 100, 0);
							break;
					}
				}
			}
		}
		
		// Jumped in spikes?
		nTraceUp = c_trace(player.x, vector(player.x,player.y,player.z+10), IGNORE_ME | USE_BOX);
		if (HIT_TARGET) {
			if (hit.entity != NULL) {
				if (is(hit.entity, is_trap)) {
					my.PL_HEALTH = 0;
					effect(pBlood,maxv(1,10*time_frame),vector(my.x, my.y-10, my.z),vector(0,0,-3));
					#ifdef PL_DEBUG
						draw_text("DEAD", 10, 250, COLOR_RED);
					#endif
					switch(hit.entity.skill1)
					{
					case 0:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_spikes, 100, 0);
						break;
					case 1:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_drown, 100, 0);
						break;
					case 2:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_fire, 100, 0);
						break;
					case 3:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_lava, 100, 0);
						break;
					case 4:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_stones, 100, 0);
						break;
					}
				}
			}
		}
	
		
		// Attack
		if (key_ctrl && me.PL_IS_ATTACKING == 0) {
			me.PL_IS_ATTACKING = 1;
			my.PL_ATTACKING_PERCENTAGE = 0;
			nSndRandom = integer(random(5));
			switch(nSndRandom) {
				case 0: snd_play(sndAttack01, 100, 0); break;
				case 1: snd_play(sndAttack02, 100, 0); break;
				case 2: snd_play(sndAttack03, 100, 0); break;
				case 3: snd_play(sndAttack04, 100, 0); break;
				case 4: snd_play(sndAttack05, 100, 0); break;
			}
		}
		
		if (me.PL_IS_ATTACKING) {
			
			if (my.PL_ATTACKING_PERCENTAGE >= 30 && !is(me, PL_ATTACKED_ONCE)) {
				// Check if enemy is hit	
				c_scan(my.x,my.pan,vector(120,0,200), IGNORE_ME | SCAN_ENTS);
				if (you != NULL) {
					if (is(you, PoL_is_enemy)) {
						you.PoL_enemy_health -=1;
						set(my, PL_ATTACKED_ONCE);
						effect(pBlood,maxv(1,10*time_frame),vector(you.x, you.y-10, you.z),vector(0,0,-3));
					}
				}
			}
			
			if (my.PL_ATTACKING_PERCENTAGE >= 100) {
				me.PL_IS_ATTACKING = 0;
				my.PL_ATTACKING_PERCENTAGE = 0;
				reset(me, PL_ATTACKED_ONCE);
			}
		}
		
		// Is being attacked?
		if (my.PL_HEALTH <= 0) {
			
			// Cheat
			if (godmode) {
				my.PL_HEALTH = 3;
			} else {
				break;
			}
		}

		#ifdef PL_DEBUG
			draw_text(str_for_num(NULL, my.PL_HEALTH), 10, 10, COLOR_RED);
		#endif
		
		// Execute the move
		move_friction = 0; // Fix by Felix, Verhindert 80% aller Stucks
		c_move(me,nullvector,vecPlayerMoveSpeed.x,IGNORE_PASSABLE | IGNORE_PASSENTS | GLIDE | ACTIVATE_TRIGGER);
		my.y = my.PL_PLAYER_POS_Y;
		
		animate_player(vecPlayerMoveSpeed.x * time_step);
		
		wait(1);
	}
	
	// Dead? Play animation
	if (my.PL_HEALTH <= 0) {
		ent_playsound(me, sndDie, 1000);
		gui_update_hearts();
		my.PL_ANIMATION_PERCENTAGE = 0;
		while(my.PL_ANIMATION_PERCENTAGE <= 80) {
			ent_animate(me, "die ", my.PL_ANIMATION_PERCENTAGE, ANM_CYCLE);
			my.PL_ANIMATION_PERCENTAGE +=4 * time_step;
			my.PL_ANIMATION_PERCENTAGE = minv(my.PL_ANIMATION_PERCENTAGE, 100);
			wait(1);
		}
		if (nPlayerLifes > 0) {
			nPlayerLifes -=1;
			game_restart();
		} else {
			set(txtGameOver, SHOW);
			
			wait(-2.5);
			set(txtGameOver, TRANSLUCENT);
			for(txtGameOver.alpha = 100; txtGameOver.alpha >= 0; txtGameOver.alpha -= time_step)
				wait(1);
			reset(txtGameOver, SHOW);
			reset(txtGameOver, TRANSLUCENT);
			txtGameOver.alpha = 100; //fix by firo
			
			media_stop(vGameMusicHandle); //fix by firo
			sky_active = 0;//fix by firo
			level_load(NULL);
			menu_open();//fix by firo
			gui_hide();//fix by firo
			return;
		}
	}
}

void actPlayer() {
	move_friction = 0.3; // Fixed by padmalcom to avoid to get stuck on slopes
	ent_preload(me);
	player = me;
	player.PL_JUMP_TIME = 1;
	player.PL_JUMP_HEIGHT = PL_MAX_JUMP_HEIGHT;
	player.PL_IS_JUMPING = 0;
	player.PL_PLAYER_POS_Y = my.y;
	player.PL_HEALTH = 3;
	player.PL_A4_COUNT = 0;
	player.trigger_range = 100;
	c_setminmax(player);
	wait(1);
	vec_set(player.min_x,vector(-40,-40,-55)); 
	vec_set(player.max_x,vector(40,40,60));
	zorroMeshOptions(me, FALSE, FALSE, FALSE);
	move_player();
	move_camera();
}

void activate_jetpack() {
	flying_man = 1;
	zorroMeshOptions(player, TRUE, FALSE, TRUE);
}

#endif