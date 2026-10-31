/* The level's moby classes, camera types and sound emitter types: the tables
   the executable's code looks ids up in (sections lvl.vtbl, lvl.camvtbl and
   lvl.sndvtbl). */
#include "types.h"
#include "rnc/audio/sound_handlers.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby_class_tables.h"

void FUN_L00_002caf40();
void FUN_L00_002ea7a0();
void FUN_L00_002ed230();
void big_axebot_update() __asm__("FUN_L04_002ba520");
void blarg_trooper_projectile_update() __asm__("FUN_L04_002af058");
void blarg_trooper_update() __asm__("FUN_L04_002c4850");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_crank_update() __asm__("FUN_L01_002e0c68");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void butterfly_swarm_update() __asm__("FUN_L04_0029ecf8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_moby_state_9() __asm__("FUN_L00_002d57d0");
void circular_saw_enemy_update() __asm__("FUN_L04_002c2270");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_camera_target_update() __asm__("FUN_L04_002e53a0");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drone_update() __asm__("FUN_L00_002ce358");
void eudora_bridge_update() __asm__("FUN_L04_002c6858");
void evaluate_clip_volume_candidate() __asm__("FUN_L03_002e8870");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void floating_platform_update() __asm__("FUN_L04_002cdda0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_circular_saw_enemy_suck_cannon_record() __asm__("FUN_L04_002c41b0");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L04_002e4418");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_circular_saw_enemy_from_suck_cannon() __asm__("FUN_L04_002c40d0");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void leaf_update() __asm__("FUN_L04_002d7e90");
void lieutenant_update() __asm__("FUN_L04_002e3078");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void path_progress_mover_update() __asm__("FUN_L04_002d6f68");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pipe_update() __asm__("FUN_L04_002e79a8");
void platform_booster_update() __asm__("FUN_L04_002c6bb8");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void random_interval_animation_update() __asm__("FUN_L04_002a5a08");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void sand_mouse_update() __asm__("FUN_L01_002e3f20");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_projectile_update() __asm__("FUN_L01_0030c9a8");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void set_transform_state_three_when_outside_clip_volume() __asm__("FUN_L03_002e8980");
void snow_plow_update() __asm__("FUN_L04_002ca420");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suck_cannon_update() __asm__("FUN_L04_002e1a10");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void translation_delta_matrix_update() __asm__("FUN_L04_002d3580");
void transport_ship_update() __asm__("FUN_L04_002ce060");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_moby_suck_cannon_state() __asm__("FUN_L02_002d61c0");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void upper_door_update() __asm__("FUN_L04_002e17d8");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void visibomb_zone_timer_update() __asm__("FUN_L01_00302648");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void woodcut_bot_debris_update() __asm__("FUN_L04_002e5178");
void woodcut_bot_update() __asm__("FUN_L04_002d16b8");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L04_001DD0EC")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L04_001DD104[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

void *D_L04_001DD11C[6]

    __attribute__((section("lvl.vtbl"))) = {update_moby_suck_cannon_state,
                                            update_moby_suck_cannon_state,
                                            launch_circular_saw_enemy_from_suck_cannon,
                                            cancel_suck_cannon_suction_and_set_moby_state_9,
                                            get_circular_saw_enemy_suck_cannon_record,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[169] __asm__("D_L04_001DC900")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,             camera_object_update, default_moby_methods},
        {  11,                    vendor_update, default_moby_methods},
        {  13,                      bolt_update, default_moby_methods},
        {  14,                      bolt_update, default_moby_methods},
        {  15,                      bolt_update, default_moby_methods},
        {  16,                      bolt_update, default_moby_methods},
        {  54,                                0, default_moby_methods},
        {  71,                    wrench_update, default_moby_methods},
        {  74,                      mine_update, default_moby_methods},
        {  85,               noop_main_callback, default_moby_methods},
        {  86,           butterfly_swarm_update, default_moby_methods},
        { 112,             glow_particle_update, default_moby_methods},
        { 121,                      bomb_update, default_moby_methods},
        { 122,                    debris_update, default_moby_methods},
        { 149,   nanotech_crate_fragment_update, default_moby_methods},
        { 153,   gold_devastator_missile_update, default_moby_methods},
        { 157,                devastator_update, default_moby_methods},
        { 161, random_interval_animation_update, default_moby_methods},
        { 163,                  visibomb_update, default_moby_methods},
        { 166,        linked_moby_states_update, default_moby_methods},
        { 167,       thruster_pack_flame_update, default_moby_methods},
        { 168,                   blaster_update, default_moby_methods},
        { 172,          visibomb_missile_update, default_moby_methods},
        { 175,                   taunter_update, default_moby_methods},
        { 176,                 pyrocitor_update, default_moby_methods},
        { 177,                tesla_claw_update, default_moby_methods},
        { 179,            trailing_flame_update, default_moby_methods},
        { 180,                  walloper_update, default_moby_methods},
        { 184,  blarg_trooper_projectile_update, default_moby_methods},
        { 185,                 morphoray_update, default_moby_methods},
        { 186,                  doom_bot_update, default_moby_methods},
        { 188,                trespasser_update, default_moby_methods},
        { 190,                mine_glove_update, default_moby_methods},
        { 192,                bomb_glove_update, default_moby_methods},
        { 203,                     decoy_update, default_moby_methods},
        { 204,           devestator_ammo_update, default_moby_methods},
        { 208,                 swingshot_update, default_moby_methods},
        { 209,            swingshot_hook_update, default_moby_methods},
        { 213,           devestator_ammo_update, default_moby_methods},
        { 214,           devestator_ammo_update, default_moby_methods},
        { 217,                big_axebot_update, default_moby_methods},
        { 222,           devestator_ammo_update, default_moby_methods},
        { 223,           devestator_ammo_update, default_moby_methods},
        { 225,           devestator_ammo_update, default_moby_methods},
        { 226,           devestator_ammo_update, default_moby_methods},
        { 229,             glove_of_doom_update, default_moby_methods},
        { 230,          doom_bot_spawner_update, default_moby_methods},
        { 270,         morphoray_chicken_update,       D_L04_001DD104},
        { 280,                bolt_crank_update, default_moby_methods},
        { 305,               homing_shot_update, default_moby_methods},
        { 340,        circular_saw_enemy_update,       D_L04_001DD11C},
        { 348,   nanotech_crate_fragment_update, default_moby_methods},
        { 349,   nanotech_crate_fragment_update, default_moby_methods},
        { 354,   nanotech_crate_fragment_update, default_moby_methods},
        { 355,   nanotech_crate_fragment_update, default_moby_methods},
        { 356,   nanotech_crate_fragment_update, default_moby_methods},
        { 357,   nanotech_crate_fragment_update, default_moby_methods},
        { 358,   nanotech_crate_fragment_update, default_moby_methods},
        { 359,   nanotech_crate_fragment_update, default_moby_methods},
        { 363,   nanotech_crate_fragment_update, default_moby_methods},
        { 364,   nanotech_crate_fragment_update, default_moby_methods},
        { 427,             blarg_trooper_update, default_moby_methods},
        { 428,       damped_falling_moby_update, default_moby_methods},
        { 432,             eudora_bridge_update, default_moby_methods},
        { 433,               noop_main_callback, default_moby_methods},
        { 434,          platform_booster_update, default_moby_methods},
        { 437,                sand_mouse_update, default_moby_methods},
        { 454,                      ryno_update, default_moby_methods},
        { 457,               ryno_rocket_update, default_moby_methods},
        { 466,                 snow_plow_update, default_moby_methods},
        { 479,                     drone_update, default_moby_methods},
        { 480,                 snow_plow_update, default_moby_methods},
        { 481,         floating_platform_update, default_moby_methods},
        { 483,                                0, default_moby_methods},
        { 484,            transport_ship_update, default_moby_methods},
        { 485,                 snow_plow_update, default_moby_methods},
        { 486,                 snow_plow_update, default_moby_methods},
        { 488,                 snow_plow_update, default_moby_methods},
        { 490,                 snow_plow_update, default_moby_methods},
        { 493,                 snow_plow_update, default_moby_methods},
        { 494,                 snow_plow_update, default_moby_methods},
        { 495,                 snow_plow_update, default_moby_methods},
        { 498,                 snow_plow_update, default_moby_methods},
        { 499,               noop_main_callback, default_moby_methods},
        { 500,                     crate_update, default_moby_methods},
        { 501,                     crate_update, default_moby_methods},
        { 505,                     crate_update, default_moby_methods},
        { 511,                     crate_update, default_moby_methods},
        { 555,                 snow_plow_update, default_moby_methods},
        { 562,               decoy_glove_update, default_moby_methods},
        { 563,               woodcut_bot_update, default_moby_methods},
        { 570,               noop_main_callback, default_moby_methods},
        { 584,  translation_delta_matrix_update, default_moby_methods},
        { 585,            metal_detector_update, default_moby_methods},
        { 604,           sandmouse_house_update, default_moby_methods},
        { 605,              buried_bolts_update, default_moby_methods},
        { 607,                  helipack_update, default_moby_methods},
        { 615,           tresspasser_pad_update, default_moby_methods},
        { 617,       path_progress_mover_update, default_moby_methods},
        { 618,               noop_main_callback, default_moby_methods},
        { 634,                                0, default_moby_methods},
        { 639,           explosion_light_update, default_moby_methods},
        { 642,                      leaf_update, default_moby_methods},
        { 750,                   infobot_update, default_moby_methods},
        { 758,     swingshot_versatarget_update, default_moby_methods},
        { 775,         grow_and_fade_out_update, default_moby_methods},
        { 803,     swingshot_versatarget_update, default_moby_methods},
        { 804,   morphoray_target_effect_update, default_moby_methods},
        { 805,               spawn_point_update, default_moby_methods},
        { 806,             nanotech_ball_update, default_moby_methods},
        { 832,       visibomb_zone_timer_update, default_moby_methods},
        { 849,                suckcannon_update, default_moby_methods},
        {1006,           devestator_ammo_update, default_moby_methods},
        {1052,             eudora_bridge_update, default_moby_methods},
        {1101,                upper_door_update, default_moby_methods},
        {1102,                upper_door_update, default_moby_methods},
        {1120,               suck_cannon_update, default_moby_methods},
        {1134,                 gold_bolt_update, default_moby_methods},
        {1138,                 quad_moby_update, default_moby_methods},
        {1143,                vendor_orb_update, default_moby_methods},
        {1190,                lieutenant_update, default_moby_methods},
        {1192,             glow_particle_update, default_moby_methods},
        {1251,            hydrodisplacer_update, default_moby_methods},
        {1289,               noop_main_callback, default_moby_methods},
        {1290,             pilots_helmet_update, default_moby_methods},
        {1343,                help_hints_update, default_moby_methods},
        {1438,           devestator_ammo_update, default_moby_methods},
        {1447,           devestator_ammo_update, default_moby_methods},
        {1449,           devestator_ammo_update, default_moby_methods},
        {1516,        woodcut_bot_debris_update, default_moby_methods},
        {1531,                upper_door_update, default_moby_methods},
        {1532,                upper_door_update, default_moby_methods},
        {1549,    cutscene_camera_target_update, default_moby_methods},
        {1599,             tumbling_part_update, default_moby_methods},
        {1601,             tumbling_part_update, default_moby_methods},
        {1602,             tumbling_part_update, default_moby_methods},
        {1603,             tumbling_part_update, default_moby_methods},
        {1604,             tumbling_part_update, default_moby_methods},
        {1605,             tumbling_part_update, default_moby_methods},
        {1606,             tumbling_part_update, default_moby_methods},
        {1607,             tumbling_part_update, default_moby_methods},
        {1608,             tumbling_part_update, default_moby_methods},
        {1609,             tumbling_part_update, default_moby_methods},
        {1613,             tumbling_part_update, default_moby_methods},
        {1614,             tumbling_part_update, default_moby_methods},
        {1615,             tumbling_part_update, default_moby_methods},
        {1616,             tumbling_part_update, default_moby_methods},
        {1617,             tumbling_part_update, default_moby_methods},
        {1618,             tumbling_part_update, default_moby_methods},
        {1619,             tumbling_part_update, default_moby_methods},
        {1620,             tumbling_part_update, default_moby_methods},
        {1624,             tumbling_part_update, default_moby_methods},
        {1633,      sandmouse_projectile_update, default_moby_methods},
        {1739,             tumbling_part_update, default_moby_methods},
        {1740,             tumbling_part_update, default_moby_methods},
        {1741,             tumbling_part_update, default_moby_methods},
        {1818,                 sandmouse_update, default_moby_methods},
        {1834,                      pipe_update, default_moby_methods},
        {1835,               noop_main_callback, default_moby_methods},
        {1836,             tumbling_part_update, default_moby_methods},
        {1900,                     decoy_update, default_moby_methods},
        {1939,             tumbling_part_update, default_moby_methods},
        {1940,             tumbling_part_update, default_moby_methods},
        {1941,             tumbling_part_update, default_moby_methods},
        {1942,             tumbling_part_update, default_moby_methods},
        {1944,             tumbling_part_update, default_moby_methods},
        {1945,             tumbling_part_update, default_moby_methods},
        {1946,             tumbling_part_update, default_moby_methods},
        {  -1,                                0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L04_001DD180") __attribute__((section("lvl.camvtbl"))) = {
    { 0,    noop_callback_l,               follow_camera_setup,follow_camera_update,                                 noop_main_callback                                                                       },
    { 1,
     evaluate_clip_volume_candidate,                hud_counter_update,
     hud_counter_update, set_transform_state_three_when_outside_clip_volume                                                                     },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input,        select_camera_mode_by_hero_vector_alignment                                                  },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                                 noop_main_callback                                             },
    { 6,            noop_callback_l, initialize_orbit_camera_from_hero,   hud_counter_update,                                 noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,               conditionally_set_camera_state_three                                                          },
    {17,           FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {23,        volume_camera_check,                noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {-1,                          0,                                 0,                    0,                                                  0},
};

struct SoundHandler sound_handlers[2] __asm__("D_L04_001DD280")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 6, update_music_switch_emitter},
        {-1,                           0},
};
