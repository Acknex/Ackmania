#ifndef skilldefines_h
#define skilldefines_h

#define MAX_PLAYERS 4
#define sk_kart_id skill1 // 1, 2, 3, 4
#define PL_GRAVITY               7
#define PL_MAX_JUMP_TIME         1
#define PL_MAX_JUMP_HEIGHT       110
#define PL_JETPACK_REFRESH_STEP  0.5
#define PL_JETPACK_MAX_STRENGTH  0.5
#define PL_JETPACK_MAX_FUEL      10

//#define PL_JUMP_TIME             skill1
#define PL_JUMP_HEIGHT           skill2
#define PL_IS_JUMPING            skill3
#define PL_ANIMATION_PERCENTAGE  skill4
#define PL_IS_ATTACKING          skill5
#define PL_ATTACKING_PERCENTAGE  skill6
#define PL_PLAYER_POS_Y          skill7
#define PL_HEALTH                skill8
#define PL_A4_COUNT              skill10
// skill10 to skill20 reserved for item calculations
#define kart_maxspeed2 skill52
#define stuck_x skill53
#define stuck_y skill54
#define stuck_counter skill55
#define fire_item_max skill56
#define fire_item skill57
#define has_finished skill58
#define scale skill59
#define kart_turbo_ent skill60
#define kart_hit skill61
#define kart_turbo skill62
#define kart_small skill63
#define kart_big skill64
#define kart_trapped skill65
#define kart_bot_separate skill66
#define kart_drift_buffer skill67
#define kart_progress_update skill68
#define kart_progress skill69
#define kart_maxspeed skill70
#define kart_checkpoint skill71
#define kart_rank skill72
#define kart_lap skill73
#define bot_path_offset skill74
#define item_id skill75
#define item_cooldown skill76
#define falling skill77
#define falling_dir skill78
#define particle_emit skill79
#define old_speed skill80
#define bump_ang skill81
#define drift_dir skill82
#define bounce_x skill83
#define bounce_y skill84
#define bounce_z skill85
#define turn_speed skill86
#define turn_speed2 skill87
#define drift_pan skill88
#define drifting skill89
#define ground_contact skill90
#define speed_x skill91
#define speed_y skill92
#define speed_z skill93
#define drive_pan skill94
#define underground skill95
#define speed skill96
#define kart_input skill97
#define INPUT_UP (1<<0)
#define INPUT_DOWN (1<<1)
#define INPUT_LEFT (1<<2)
#define INPUT_RIGHT (1<<3)
#define INPUT_HOP (1<<4)
#define INPUT_ITEM (1<<5)
#define kart_height skill98
#define kart_id skill99
#define _type skill100
#define type_kart 1
#define type_kart_spawn 2
#define PL_JUMPKEYSTILLPRESSED   skill90

#define PL_ATTACKED_ONCE         FLAG8
#define group_kart 1
#define group_item 2
#define group_obstacle 3
#define group_track 4

var g_music = 75;

#endif /* skilldefines_h */
