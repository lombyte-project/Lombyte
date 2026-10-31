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
void agnogg_ship_front_update() __asm__("FUN_L01_002fad68");
void beehive_update() __asm__("FUN_L01_003021b8");
void billboard_projectile_update() __asm__("FUN_L01_002c7530");
void birdbot_projectile_update() __asm__("FUN_L01_002fa4c0");
void birdbot_update() __asm__("FUN_L01_002e6bf0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_crank_update() __asm__("FUN_L01_002e0c68");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void bomber_jet_update() __asm__("FUN_L01_002f7728");
void bounded_scroll_layers_update() __asm__("FUN_L01_002f6180");
void bridge_half_update() __asm__("FUN_L01_002fb8a8");
void bridge_piece_1_update() __asm__("FUN_L01_002f9d80");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void burning_debris_update() __asm__("FUN_L01_0030ba18");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_state_14() __asm__("FUN_L01_002efb98");
void cave_door_update() __asm__("FUN_L01_002f4348");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_trigger_update() __asm__("FUN_L01_002fb5b0");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void destroyable_cave_wall_update() __asm__("FUN_L01_002fa800");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void door_half_update() __asm__("FUN_L01_002f4710");
void door_left_update() __asm__("FUN_L01_002fed68");
void drone_update() __asm__("FUN_L00_002ce358");
void environment_map_scroll_update() __asm__("FUN_L01_0030f208");
void err_message() __asm__("FUN_0023ab78");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void explosive_projectile_update() __asm__("FUN_L01_002f6a30");
void falling_rock_update() __asm__("FUN_L01_002f9080");
void falling_splash_moby_update() __asm__("FUN_L01_002ffdc0");
void fan_update() __asm__("FUN_L01_002f9bf0");
void fighter_jet_update() __asm__("FUN_L01_002f4428");
void flickering_point_light_update() __asm__("FUN_L01_0030b618");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void get_moby_data_plus_0xc0() __asm__("FUN_L01_002efbc8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void gold_weapon_case_update() __asm__("FUN_L01_002e1ac0");
void green_mushroom_update() __asm__("FUN_L01_002fd9a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L01_0030acb8");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_camera_transform_and_paths() __asm__("FUN_L01_00315de0");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void large_amoeboid_update() __asm__("FUN_L01_002edca0");
void launch_suck_cannon_projectile_and_sync_moby_state() __asm__("FUN_L01_002f1d18");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void level01_cutscene_effects_update() __asm__("FUN_L01_0030c190");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_hold_state_if_found() __asm__("FUN_L01_002efaf8");
void moby_hold_state_if_grabbed() __asm__("FUN_L01_002f1c78");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void mushroom_underside_update() __asm__("FUN_L01_00307c48");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void novalis_elevator_update() __asm__("FUN_L01_002b9eb0");
void particle_spawner_smoke_fire_update() __asm__("FUN_L00_002d6f28");
void peckbot_update() __asm__("FUN_L01_002efc60");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pipe_update() __asm__("FUN_L01_002ff860");
void piston_platform_update() __asm__("FUN_L01_002f95c0");
void pool_tiles_and_effects_update() __asm__("FUN_L01_002fd0e8");
void process_amoeboid_suck_cannon_suction() __asm__("FUN_L01_002efa88");
void pulsing_scroll_layers_update() __asm__("FUN_L01_00309c98");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void rail_hero_motion_update() __asm__("FUN_L01_002f6328");
void red_plant_update() __asm__("FUN_L00_002e3ca8");
void rippling_scroll_layers_update() __asm__("FUN_L01_002feb58");
void rising_debris_emitter_update() __asm__("FUN_L01_002f8c58");
void rock_formation_update() __asm__("FUN_L01_002f9810");
void rock_update() __asm__("FUN_L01_002f8718");
void run_moby_helper_and_set_state_7() __asm__("FUN_L01_002f1dc8");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void sand_mouse_update() __asm__("FUN_L01_002e3f20");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_projectile_update() __asm__("FUN_L01_0030c9a8");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void smoke_emitter_update() __asm__("FUN_L01_002ffb28");
void smoke_fire_controller_update() __asm__("FUN_L00_002a4cd8");
void smoke_trail_projectile_update() __asm__("FUN_L01_002eda00");
void spark_particle_emitter_update() __asm__("FUN_L01_002bd100");
void spawn_point_update() __asm__("FUN_L01_00300220");
void stationary_ship_update() __asm__("FUN_L01_002f4960");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void tall_shroom_update() __asm__("FUN_L01_0030d0f0");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void the_plumber_update() __asm__("FUN_L01_002ff118");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_listener_emitter() __asm__("FUN_L01_0031a078");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_sound_emitter_playback_at_random_position() __asm__("FUN_L00_002edad8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void veldin_ship_update() __asm__("FUN_L00_002d1e80");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void visibomb_zone_timer_update() __asm__("FUN_L01_00302648");
void walloper_update() __asm__("FUN_L00_002bb558");
void water_current_update() __asm__("FUN_L01_002f3120");
void water_lily_update() __asm__("FUN_L01_002f8268");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L01_0020C3AC")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L01_0020C3C4[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

void *D_L01_0020C3DC[6]

    __attribute__((section("lvl.vtbl"))) = {process_amoeboid_suck_cannon_suction,
                                            moby_hold_state_if_found,
                                            moby_hold_state_if_found,
                                            cancel_suck_cannon_suction_and_set_state_14,
                                            get_moby_data_plus_0xc0,
                                            hud_counter_update};

void *D_L01_0020C3F4[6]

    __attribute__((section("lvl.vtbl"))) = {moby_hold_state_if_grabbed,
                                            moby_hold_state_if_grabbed,
                                            launch_suck_cannon_projectile_and_sync_moby_state,
                                            run_moby_helper_and_set_state_7,
                                            get_moby_data_plus_0x60,
                                            hud_counter_update};

void *D_L01_0020C40C[6]

    __attribute__((section("lvl.vtbl"))) = {process_amoeboid_suck_cannon_suction,
                                            moby_hold_state_if_found,
                                            moby_hold_state_if_found,
                                            cancel_suck_cannon_suction_and_set_state_14,
                                            get_moby_data_plus_0xc0,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[185] __asm__("D_L01_0020BB00")

    __attribute__((section("lvl.vtbl"))) = {
        { 726,            novalis_elevator_update, default_moby_methods},
        { 809,       smoke_fire_controller_update, default_moby_methods},
        {1007,               camera_object_update, default_moby_methods},
        {  11,                      vendor_update, default_moby_methods},
        {  13,                        bolt_update, default_moby_methods},
        {  14,                        bolt_update, default_moby_methods},
        {  15,                        bolt_update, default_moby_methods},
        {  16,                        bolt_update, default_moby_methods},
        {  27,      spark_particle_emitter_update, default_moby_methods},
        {  71,                      wrench_update, default_moby_methods},
        {  74,                        mine_update, default_moby_methods},
        { 112,               glow_particle_update, default_moby_methods},
        { 121,                        bomb_update, default_moby_methods},
        { 122,                      debris_update, default_moby_methods},
        { 149,     nanotech_crate_fragment_update, default_moby_methods},
        { 153,     gold_devastator_missile_update, default_moby_methods},
        { 156,        billboard_projectile_update, default_moby_methods},
        { 157,                  devastator_update, default_moby_methods},
        { 163,                    visibomb_update, default_moby_methods},
        { 166,          linked_moby_states_update, default_moby_methods},
        { 167,         thruster_pack_flame_update, default_moby_methods},
        { 168,                     blaster_update, default_moby_methods},
        { 172,            visibomb_missile_update, default_moby_methods},
        { 175,                     taunter_update, default_moby_methods},
        { 176,                   pyrocitor_update, default_moby_methods},
        { 177,                  tesla_claw_update, default_moby_methods},
        { 179,              trailing_flame_update, default_moby_methods},
        { 180,                    walloper_update, default_moby_methods},
        { 185,                   morphoray_update, default_moby_methods},
        { 186,                    doom_bot_update, default_moby_methods},
        { 188,                  trespasser_update, default_moby_methods},
        { 190,                  mine_glove_update, default_moby_methods},
        { 192,                  bomb_glove_update, default_moby_methods},
        { 203,                       decoy_update, default_moby_methods},
        { 204,             devestator_ammo_update, default_moby_methods},
        { 208,                   swingshot_update, default_moby_methods},
        { 209,              swingshot_hook_update, default_moby_methods},
        { 213,             devestator_ammo_update, default_moby_methods},
        { 214,             devestator_ammo_update, default_moby_methods},
        { 222,             devestator_ammo_update, default_moby_methods},
        { 223,             devestator_ammo_update, default_moby_methods},
        { 225,             devestator_ammo_update, default_moby_methods},
        { 226,             devestator_ammo_update, default_moby_methods},
        { 229,               glove_of_doom_update, default_moby_methods},
        { 230,            doom_bot_spawner_update, default_moby_methods},
        { 270,           morphoray_chicken_update,       D_L01_0020C3C4},
        { 280,                  bolt_crank_update, default_moby_methods},
        { 304,            gold_weapon_case_update, default_moby_methods},
        { 305,                 homing_shot_update, default_moby_methods},
        { 348,     nanotech_crate_fragment_update, default_moby_methods},
        { 349,     nanotech_crate_fragment_update, default_moby_methods},
        { 354,     nanotech_crate_fragment_update, default_moby_methods},
        { 355,     nanotech_crate_fragment_update, default_moby_methods},
        { 356,     nanotech_crate_fragment_update, default_moby_methods},
        { 357,     nanotech_crate_fragment_update, default_moby_methods},
        { 358,     nanotech_crate_fragment_update, default_moby_methods},
        { 359,     nanotech_crate_fragment_update, default_moby_methods},
        { 363,     nanotech_crate_fragment_update, default_moby_methods},
        { 364,     nanotech_crate_fragment_update, default_moby_methods},
        { 428,         damped_falling_moby_update, default_moby_methods},
        { 433,                 noop_main_callback, default_moby_methods},
        { 437,                  sand_mouse_update, default_moby_methods},
        { 454,                        ryno_update, default_moby_methods},
        { 457,                 ryno_rocket_update, default_moby_methods},
        { 459,                     birdbot_update, default_moby_methods},
        { 479,                       drone_update, default_moby_methods},
        { 483,                                  0, default_moby_methods},
        { 500,                       crate_update, default_moby_methods},
        { 501,                       crate_update, default_moby_methods},
        { 502,                       crate_update, default_moby_methods},
        { 505,                       crate_update, default_moby_methods},
        { 511,                       crate_update, default_moby_methods},
        { 530,                 veldin_ship_update, default_moby_methods},
        { 531,                 noop_main_callback, default_moby_methods},
        { 562,                 decoy_glove_update, default_moby_methods},
        { 567,      smoke_trail_projectile_update, default_moby_methods},
        { 572,              large_amoeboid_update,       D_L01_0020C3DC},
        { 577,                     peckbot_update,       D_L01_0020C3F4},
        { 585,              metal_detector_update, default_moby_methods},
        { 604,             sandmouse_house_update, default_moby_methods},
        { 605,                buried_bolts_update, default_moby_methods},
        { 607,                    helipack_update, default_moby_methods},
        { 613,               water_current_update, default_moby_methods},
        { 618,                 noop_main_callback, default_moby_methods},
        { 634,                                  0, default_moby_methods},
        { 639,             explosion_light_update, default_moby_methods},
        { 641,                   cave_door_update, default_moby_methods},
        { 660,                 fighter_jet_update, default_moby_methods},
        { 665,                   door_half_update, default_moby_methods},
        { 666,             stationary_ship_update, default_moby_methods},
        { 676,                        err_message, default_moby_methods},
        { 678,       bounded_scroll_layers_update, default_moby_methods},
        { 679,            rail_hero_motion_update, default_moby_methods},
        { 686,        explosive_projectile_update, default_moby_methods},
        { 688,                  bomber_jet_update, default_moby_methods},
        { 695,                  water_lily_update, default_moby_methods},
        { 696,                        rock_update, default_moby_methods},
        { 697,                        rock_update, default_moby_methods},
        { 698,                        rock_update, default_moby_methods},
        { 700,       rising_debris_emitter_update, default_moby_methods},
        { 701,                falling_rock_update, default_moby_methods},
        { 703,             piston_platform_update, default_moby_methods},
        { 704,              rock_formation_update, default_moby_methods},
        { 705,                         fan_update, default_moby_methods},
        { 709,              bridge_piece_1_update, default_moby_methods},
        { 710,              bridge_piece_1_update, default_moby_methods},
        { 711,              bridge_piece_1_update, default_moby_methods},
        { 715,             piston_platform_update, default_moby_methods},
        { 722,          birdbot_projectile_update, default_moby_methods},
        { 724,                   red_plant_update, default_moby_methods},
        { 725,                   red_plant_update, default_moby_methods},
        { 729,       destroyable_cave_wall_update, default_moby_methods},
        { 730,           agnogg_ship_front_update, default_moby_methods},
        { 731,           agnogg_ship_front_update, default_moby_methods},
        { 737,            cutscene_trigger_update, default_moby_methods},
        { 746,                 bridge_half_update, default_moby_methods},
        { 750,                     infobot_update, default_moby_methods},
        { 751,      pool_tiles_and_effects_update, default_moby_methods},
        { 754,              green_mushroom_update, default_moby_methods},
        { 760, particle_spawner_smoke_fire_update, default_moby_methods},
        { 761,      rippling_scroll_layers_update, default_moby_methods},
        { 768,                   door_left_update, default_moby_methods},
        { 769,                   door_left_update, default_moby_methods},
        { 774,                 the_plumber_update, default_moby_methods},
        { 775,           grow_and_fade_out_update, default_moby_methods},
        { 778,                        pipe_update, default_moby_methods},
        { 779,               smoke_emitter_update, default_moby_methods},
        { 787,         falling_splash_moby_update, default_moby_methods},
        { 790,           agnogg_ship_front_update, default_moby_methods},
        { 804,     morphoray_target_effect_update, default_moby_methods},
        { 805,                 spawn_point_update, default_moby_methods},
        { 806,               nanotech_ball_update, default_moby_methods},
        { 815,                     beehive_update, default_moby_methods},
        { 832,         visibomb_zone_timer_update, default_moby_methods},
        { 849,                  suckcannon_update, default_moby_methods},
        { 865,              large_amoeboid_update, default_moby_methods},
        { 866,              large_amoeboid_update,       D_L01_0020C40C},
        {1006,             devestator_ammo_update, default_moby_methods},
        {1042,          mushroom_underside_update, default_moby_methods},
        {1134,                   gold_bolt_update, default_moby_methods},
        {1135,                  teleporter_update, default_moby_methods},
        {1138,                   quad_moby_update, default_moby_methods},
        {1143,                  vendor_orb_update, default_moby_methods},
        {1192,               glow_particle_update, default_moby_methods},
        {1225,       pulsing_scroll_layers_update, default_moby_methods},
        {1251,              hydrodisplacer_update, default_moby_methods},
        {1289,                 noop_main_callback, default_moby_methods},
        {1290,               pilots_helmet_update, default_moby_methods},
        {1341,                  help_hints_update, default_moby_methods},
        {1438,             devestator_ammo_update, default_moby_methods},
        {1447,             devestator_ammo_update, default_moby_methods},
        {1449,             devestator_ammo_update, default_moby_methods},
        {1456,            gold_weapon_case_update, default_moby_methods},
        {1457,            gold_weapon_case_update, default_moby_methods},
        {1458,            gold_weapon_case_update, default_moby_methods},
        {1459,            gold_weapon_case_update, default_moby_methods},
        {1460,            gold_weapon_case_update, default_moby_methods},
        {1461,            gold_weapon_case_update, default_moby_methods},
        {1462,            gold_weapon_case_update, default_moby_methods},
        {1463,            gold_weapon_case_update, default_moby_methods},
        {1464,            gold_weapon_case_update, default_moby_methods},
        {1465,            gold_weapon_case_update, default_moby_methods},
        {1504,      flickering_point_light_update, default_moby_methods},
        {1510,              burning_debris_update, default_moby_methods},
        {1546,    level01_cutscene_effects_update, default_moby_methods},
        {1633,        sandmouse_projectile_update, default_moby_methods},
        {1736,               tumbling_part_update, default_moby_methods},
        {1737,               tumbling_part_update, default_moby_methods},
        {1738,               tumbling_part_update, default_moby_methods},
        {1747,               tumbling_part_update, default_moby_methods},
        {1748,               tumbling_part_update, default_moby_methods},
        {1749,               tumbling_part_update, default_moby_methods},
        {1761,               tumbling_part_update, default_moby_methods},
        {1762,               tumbling_part_update, default_moby_methods},
        {1763,               tumbling_part_update, default_moby_methods},
        {1770,               tumbling_part_update, default_moby_methods},
        {1813,                 tall_shroom_update, default_moby_methods},
        {1814,               tumbling_part_update, default_moby_methods},
        {1815,               tumbling_part_update, default_moby_methods},
        {1816,                 noop_main_callback, default_moby_methods},
        {1817,               tumbling_part_update, default_moby_methods},
        {1818,                   sandmouse_update, default_moby_methods},
        {1848,      environment_map_scroll_update, default_moby_methods},
        {1900,                       decoy_update, default_moby_methods},
        {  -1,                                  0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L01_0020C480") __attribute__((section("lvl.camvtbl"))) = {
    { 0,noop_callback_l,                   follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                     },
    { 3,
     noop_callback_l, initialize_camera_transform_and_paths,
     update_camera_position_and_orientation_from_paths,              update_camera_mode_from_preset                                    },
    { 4,
     camera_transition_update,                      FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                },
    { 5,
     noop_callback_l,        initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                           },
    { 6,      noop_callback_l,     initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l,     initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                        },
    {17,     FUN_L00_002ed230,                    noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                    0,                                     0,                    0,                                           0},
};

struct SoundHandler sound_handlers[7] __asm__("D_L01_0020C580")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0,        update_sound_emitter_playback_by_distance},
        { 1,                               update_box_emitter},
        { 2, update_sound_emitter_playback_at_random_position},
        { 3,                        update_reverb_box_emitter},
        { 5,                          update_listener_emitter},
        { 6,                      update_music_switch_emitter},
        {-1,                                                0},
};
