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
void birdbot_update() __asm__("FUN_L01_002e6bf0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_crank_update() __asm__("FUN_L01_002e0c68");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void bomber_jet_update() __asm__("FUN_L01_002f7728");
void bridge_half_update() __asm__("FUN_L01_002fb8a8");
void bridge_piece_1_update() __asm__("FUN_L01_002f9d80");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_state_14() __asm__("FUN_L01_002efb98");
void cave_door_update() __asm__("FUN_L01_002f4348");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void destroyable_cave_wall_update() __asm__("FUN_L01_002fa800");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void door_half_update() __asm__("FUN_L01_002f4710");
void door_left_update() __asm__("FUN_L01_002fed68");
void err_message() __asm__("FUN_0023ab78");
void falling_rock_update() __asm__("FUN_L01_002f9080");
void fan_update() __asm__("FUN_L01_002f9bf0");
void fighter_jet_update() __asm__("FUN_L01_002f4428");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void get_moby_data_plus_0xc0() __asm__("FUN_L01_002efbc8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void gold_weapon_case_update() __asm__("FUN_L01_002e1ac0");
void green_mushroom_update() __asm__("FUN_L01_002fd9a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_camera_transform_and_paths() __asm__("FUN_L01_00315de0");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void large_amoeboid_update() __asm__("FUN_L01_002edca0");
void launch_suck_cannon_projectile_and_sync_moby_state() __asm__("FUN_L01_002f1d18");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_hold_state_if_found() __asm__("FUN_L01_002efaf8");
void moby_hold_state_if_grabbed() __asm__("FUN_L01_002f1c78");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
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
void process_amoeboid_suck_cannon_suction() __asm__("FUN_L01_002efa88");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void red_plant_update() __asm__("FUN_L00_002e3ca8");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void rock_formation_update() __asm__("FUN_L01_002f9810");
void rock_update() __asm__("FUN_L01_002f8718");
void run_moby_helper_and_set_state_7() __asm__("FUN_L01_002f1dc8");
void ryno_update() __asm__("FUN_L00_002cc608");
void sand_mouse_update() __asm__("FUN_L01_002e3f20");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
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
void trespasser_update() __asm__("FUN_L00_002c1718");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_billboard_projectile() __asm__("FUN_L01_002c7530");
void update_birdbot_projectile() __asm__("FUN_L01_002fa4c0");
void update_bounded_scroll_layers() __asm__("FUN_L01_002f6180");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_burning_debris() __asm__("FUN_L01_0030ba18");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");
void update_cutscene_trigger() __asm__("FUN_L01_002fb5b0");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_doom_bot_spawner() __asm__("FUN_L00_002c89e8");
void update_drone() __asm__("FUN_L00_002ce358");
void update_explosive_projectile() __asm__("FUN_L01_002f6a30");
void update_falling_splash_moby() __asm__("FUN_L01_002ffdc0");
void update_frame_timing_and_wrapped_value() __asm__("FUN_L00_002a4cd8");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_help_hints() __asm__("FUN_L01_0030acb8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_level01_cutscene_effects() __asm__("FUN_L01_0030c190");
void update_level01_environment_map_uv_scroll() __asm__("FUN_L01_0030f208");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_listener_emitter() __asm__("FUN_L01_0031a078");
void update_moby_interpolated_point_light() __asm__("FUN_L01_0030b618");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_pool_tiles_and_effects() __asm__("FUN_L01_002fd0e8");
void update_pulsing_scroll_layers() __asm__("FUN_L01_00309c98");
void update_rail_hero_motion() __asm__("FUN_L01_002f6328");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_rippling_scroll_layers() __asm__("FUN_L01_002feb58");
void update_rising_particle_and_debris_emitter() __asm__("FUN_L01_002f8c58");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void update_sandmouse_projectile() __asm__("FUN_L01_0030c9a8");
void update_smoke_emitter() __asm__("FUN_L01_002ffb28");
void update_smoke_trail_projectile() __asm__("FUN_L01_002eda00");
void update_sound_emitter_playback_at_random_position() __asm__("FUN_L00_002edad8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_spark_particle_emitter() __asm__("FUN_L01_002bd100");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void veldin_ship_update() __asm__("FUN_L00_002d1e80");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
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
                                            mine_blend_color};

void *D_L01_0020C3C4[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

void *D_L01_0020C3DC[6]

    __attribute__((section("lvl.vtbl"))) = {process_amoeboid_suck_cannon_suction,
                                            moby_hold_state_if_found,
                                            moby_hold_state_if_found,
                                            cancel_suck_cannon_suction_and_set_state_14,
                                            get_moby_data_plus_0xc0,
                                            mine_blend_color};

void *D_L01_0020C3F4[6]

    __attribute__((section("lvl.vtbl"))) = {moby_hold_state_if_grabbed,
                                            moby_hold_state_if_grabbed,
                                            launch_suck_cannon_projectile_and_sync_moby_state,
                                            run_moby_helper_and_set_state_7,
                                            get_moby_data_plus_0x60,
                                            mine_blend_color};

void *D_L01_0020C40C[6]

    __attribute__((section("lvl.vtbl"))) = {process_amoeboid_suck_cannon_suction,
                                            moby_hold_state_if_found,
                                            moby_hold_state_if_found,
                                            cancel_suck_cannon_suction_and_set_state_14,
                                            get_moby_data_plus_0xc0,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[185] __asm__("D_L01_0020BB00")

    __attribute__((section("lvl.vtbl"))) = {
        { 726,                   novalis_elevator_update, default_moby_methods},
        { 809,     update_frame_timing_and_wrapped_value, default_moby_methods},
        {1007,                      camera_object_update, default_moby_methods},
        {  11,                             vendor_update, default_moby_methods},
        {  13,                               bolt_update, default_moby_methods},
        {  14,                               bolt_update, default_moby_methods},
        {  15,                               bolt_update, default_moby_methods},
        {  16,                               bolt_update, default_moby_methods},
        {  27,             update_spark_particle_emitter, default_moby_methods},
        {  71,                             wrench_update, default_moby_methods},
        {  74,                               mine_update, default_moby_methods},
        { 112,                      update_glow_particle, default_moby_methods},
        { 121,                               bomb_update, default_moby_methods},
        { 122,                             debris_update, default_moby_methods},
        { 149,            nanotech_crate_fragment_update, default_moby_methods},
        { 153,            gold_devastator_missile_update, default_moby_methods},
        { 156,               update_billboard_projectile, default_moby_methods},
        { 157,                         devastator_update, default_moby_methods},
        { 163,                           visibomb_update, default_moby_methods},
        { 166,                 update_linked_moby_states, default_moby_methods},
        { 167,                update_thruster_pack_flame, default_moby_methods},
        { 168,                            blaster_update, default_moby_methods},
        { 172,                   visibomb_missile_update, default_moby_methods},
        { 175,                            taunter_update, default_moby_methods},
        { 176,                          pyrocitor_update, default_moby_methods},
        { 177,                         tesla_claw_update, default_moby_methods},
        { 179,     update_linked_moby_scale_and_rotation, default_moby_methods},
        { 180,                           walloper_update, default_moby_methods},
        { 185,                          morphoray_update, default_moby_methods},
        { 186,                           doom_bot_update, default_moby_methods},
        { 188,                         trespasser_update, default_moby_methods},
        { 190,                         mine_glove_update, default_moby_methods},
        { 192,                         bomb_glove_update, default_moby_methods},
        { 203,                              decoy_update, default_moby_methods},
        { 204,                    devestator_ammo_update, default_moby_methods},
        { 208,                          swingshot_update, default_moby_methods},
        { 209,                     swingshot_hook_update, default_moby_methods},
        { 213,                    devestator_ammo_update, default_moby_methods},
        { 214,                    devestator_ammo_update, default_moby_methods},
        { 222,                    devestator_ammo_update, default_moby_methods},
        { 223,                    devestator_ammo_update, default_moby_methods},
        { 225,                    devestator_ammo_update, default_moby_methods},
        { 226,                    devestator_ammo_update, default_moby_methods},
        { 229,                      glove_of_doom_update, default_moby_methods},
        { 230,                   update_doom_bot_spawner, default_moby_methods},
        { 270,                  update_morphoray_chicken,       D_L01_0020C3C4},
        { 280,                         bolt_crank_update, default_moby_methods},
        { 304,                   gold_weapon_case_update, default_moby_methods},
        { 305,                        update_homing_shot, default_moby_methods},
        { 348,            nanotech_crate_fragment_update, default_moby_methods},
        { 349,            nanotech_crate_fragment_update, default_moby_methods},
        { 354,            nanotech_crate_fragment_update, default_moby_methods},
        { 355,            nanotech_crate_fragment_update, default_moby_methods},
        { 356,            nanotech_crate_fragment_update, default_moby_methods},
        { 357,            nanotech_crate_fragment_update, default_moby_methods},
        { 358,            nanotech_crate_fragment_update, default_moby_methods},
        { 359,            nanotech_crate_fragment_update, default_moby_methods},
        { 363,            nanotech_crate_fragment_update, default_moby_methods},
        { 364,            nanotech_crate_fragment_update, default_moby_methods},
        { 428,                update_damped_falling_moby, default_moby_methods},
        { 433,                        noop_main_callback, default_moby_methods},
        { 437,                         sand_mouse_update, default_moby_methods},
        { 454,                               ryno_update, default_moby_methods},
        { 457,                        update_ryno_rocket, default_moby_methods},
        { 459,                            birdbot_update, default_moby_methods},
        { 479,                              update_drone, default_moby_methods},
        { 483,                                         0, default_moby_methods},
        { 500,                              crate_update, default_moby_methods},
        { 501,                              crate_update, default_moby_methods},
        { 502,                              crate_update, default_moby_methods},
        { 505,                              crate_update, default_moby_methods},
        { 511,                              crate_update, default_moby_methods},
        { 530,                        veldin_ship_update, default_moby_methods},
        { 531,                        noop_main_callback, default_moby_methods},
        { 562,                        decoy_glove_update, default_moby_methods},
        { 567,             update_smoke_trail_projectile, default_moby_methods},
        { 572,                     large_amoeboid_update,       D_L01_0020C3DC},
        { 577,                            peckbot_update,       D_L01_0020C3F4},
        { 585,                     metal_detector_update, default_moby_methods},
        { 604,                    sandmouse_house_update, default_moby_methods},
        { 605,        update_nearest_area_moby_selection, default_moby_methods},
        { 607,                           helipack_update, default_moby_methods},
        { 613,                      water_current_update, default_moby_methods},
        { 618,                        noop_main_callback, default_moby_methods},
        { 634,                                         0, default_moby_methods},
        { 639,          update_animated_point_light_moby, default_moby_methods},
        { 641,                          cave_door_update, default_moby_methods},
        { 660,                        fighter_jet_update, default_moby_methods},
        { 665,                          door_half_update, default_moby_methods},
        { 666,                    stationary_ship_update, default_moby_methods},
        { 676,                               err_message, default_moby_methods},
        { 678,              update_bounded_scroll_layers, default_moby_methods},
        { 679,                   update_rail_hero_motion, default_moby_methods},
        { 686,               update_explosive_projectile, default_moby_methods},
        { 688,                         bomber_jet_update, default_moby_methods},
        { 695,                         water_lily_update, default_moby_methods},
        { 696,                               rock_update, default_moby_methods},
        { 697,                               rock_update, default_moby_methods},
        { 698,                               rock_update, default_moby_methods},
        { 700, update_rising_particle_and_debris_emitter, default_moby_methods},
        { 701,                       falling_rock_update, default_moby_methods},
        { 703,                    piston_platform_update, default_moby_methods},
        { 704,                     rock_formation_update, default_moby_methods},
        { 705,                                fan_update, default_moby_methods},
        { 709,                     bridge_piece_1_update, default_moby_methods},
        { 710,                     bridge_piece_1_update, default_moby_methods},
        { 711,                     bridge_piece_1_update, default_moby_methods},
        { 715,                    piston_platform_update, default_moby_methods},
        { 722,                 update_birdbot_projectile, default_moby_methods},
        { 724,                          red_plant_update, default_moby_methods},
        { 725,                          red_plant_update, default_moby_methods},
        { 729,              destroyable_cave_wall_update, default_moby_methods},
        { 730,                  agnogg_ship_front_update, default_moby_methods},
        { 731,                  agnogg_ship_front_update, default_moby_methods},
        { 737,                   update_cutscene_trigger, default_moby_methods},
        { 746,                        bridge_half_update, default_moby_methods},
        { 750,                            infobot_update, default_moby_methods},
        { 751,             update_pool_tiles_and_effects, default_moby_methods},
        { 754,                     green_mushroom_update, default_moby_methods},
        { 760,        particle_spawner_smoke_fire_update, default_moby_methods},
        { 761,             update_rippling_scroll_layers, default_moby_methods},
        { 768,                          door_left_update, default_moby_methods},
        { 769,                          door_left_update, default_moby_methods},
        { 774,                        the_plumber_update, default_moby_methods},
        { 775,                  grow_and_fade_out_update, default_moby_methods},
        { 778,                               pipe_update, default_moby_methods},
        { 779,                      update_smoke_emitter, default_moby_methods},
        { 787,                update_falling_splash_moby, default_moby_methods},
        { 790,                  agnogg_ship_front_update, default_moby_methods},
        { 804, update_linked_moby_transform_and_lifetime, default_moby_methods},
        { 805,                        spawn_point_update, default_moby_methods},
        { 806,                      nanotech_ball_update, default_moby_methods},
        { 815,                            beehive_update, default_moby_methods},
        { 832,        update_target_outside_zone_timeout, default_moby_methods},
        { 849,                         suckcannon_update, default_moby_methods},
        { 865,                     large_amoeboid_update, default_moby_methods},
        { 866,                     large_amoeboid_update,       D_L01_0020C40C},
        {1006,                    devestator_ammo_update, default_moby_methods},
        {1042,                 mushroom_underside_update, default_moby_methods},
        {1134,                          gold_bolt_update, default_moby_methods},
        {1135,                         teleporter_update, default_moby_methods},
        {1138,                          quad_moby_update, default_moby_methods},
        {1143,                         vendor_orb_update, default_moby_methods},
        {1192,                      update_glow_particle, default_moby_methods},
        {1225,              update_pulsing_scroll_layers, default_moby_methods},
        {1251,                     hydrodisplacer_update, default_moby_methods},
        {1289,                        noop_main_callback, default_moby_methods},
        {1290,                      pilots_helmet_update, default_moby_methods},
        {1341,                         update_help_hints, default_moby_methods},
        {1438,                    devestator_ammo_update, default_moby_methods},
        {1447,                    devestator_ammo_update, default_moby_methods},
        {1449,                    devestator_ammo_update, default_moby_methods},
        {1456,                   gold_weapon_case_update, default_moby_methods},
        {1457,                   gold_weapon_case_update, default_moby_methods},
        {1458,                   gold_weapon_case_update, default_moby_methods},
        {1459,                   gold_weapon_case_update, default_moby_methods},
        {1460,                   gold_weapon_case_update, default_moby_methods},
        {1461,                   gold_weapon_case_update, default_moby_methods},
        {1462,                   gold_weapon_case_update, default_moby_methods},
        {1463,                   gold_weapon_case_update, default_moby_methods},
        {1464,                   gold_weapon_case_update, default_moby_methods},
        {1465,                   gold_weapon_case_update, default_moby_methods},
        {1504,      update_moby_interpolated_point_light, default_moby_methods},
        {1510,                     update_burning_debris, default_moby_methods},
        {1546,           update_level01_cutscene_effects, default_moby_methods},
        {1633,               update_sandmouse_projectile, default_moby_methods},
        {1736,                          robot_arm_update, default_moby_methods},
        {1737,                          robot_arm_update, default_moby_methods},
        {1738,                          robot_arm_update, default_moby_methods},
        {1747,                          robot_arm_update, default_moby_methods},
        {1748,                          robot_arm_update, default_moby_methods},
        {1749,                          robot_arm_update, default_moby_methods},
        {1761,                          robot_arm_update, default_moby_methods},
        {1762,                          robot_arm_update, default_moby_methods},
        {1763,                          robot_arm_update, default_moby_methods},
        {1770,                          robot_arm_update, default_moby_methods},
        {1813,                        tall_shroom_update, default_moby_methods},
        {1814,                          robot_arm_update, default_moby_methods},
        {1815,                          robot_arm_update, default_moby_methods},
        {1816,                        noop_main_callback, default_moby_methods},
        {1817,                          robot_arm_update, default_moby_methods},
        {1818,                          sandmouse_update, default_moby_methods},
        {1848,  update_level01_environment_map_uv_scroll, default_moby_methods},
        {1900,                              decoy_update, default_moby_methods},
        {  -1,                                         0, default_moby_methods},
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
    { 6,      noop_callback_l,     initialize_orbit_camera_from_hero,     mine_blend_color,                          noop_main_callback},
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
