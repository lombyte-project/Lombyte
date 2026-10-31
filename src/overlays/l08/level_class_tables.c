/* The level's moby classes, camera types and sound emitter types: the tables
   the executable's code looks ids up in (sections lvl.vtbl, lvl.camvtbl and
   lvl.sndvtbl). */
#include "types.h"
#include "rnc/audio/sound_handlers.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby_class_tables.h"

void FUN_L00_002caf40();
void FUN_L00_002ea7a0();
void batalia_cannon_update() __asm__("FUN_L08_002e0328");
void batalia_deserter_update() __asm__("FUN_L08_00305270");
void batalia_textured_effect_update() __asm__("FUN_L08_003097b0");
void billboard_projectile_update() __asm__("FUN_L01_002c7530");
void blarg_bombthrower_update() __asm__("FUN_L08_002dabf0");
void blarg_gun_update() __asm__("FUN_L05_003186f8");
void blarg_paratrooper_update() __asm__("FUN_L08_00301158");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_crank_update() __asm__("FUN_L01_002e0c68");
void bolt_pickup_update() __asm__("FUN_L08_002b82a0");
void bolt_thief_update() __asm__("FUN_L08_002e2df0");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_spawner_bomb_update() __asm__("FUN_L03_002c4718");
void bomb_spawner_update() __asm__("FUN_L03_002bef68");
void bomb_update() __asm__("FUN_L00_002adb30");
void bomber_tank_bomb_update() __asm__("FUN_L08_002dc9a8");
void bomber_tank_update() __asm__("FUN_L08_002d42d8");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void commando_update() __asm__("FUN_L08_00302ce8");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_camera_target_update() __asm__("FUN_L08_003084d8");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void directional_spark_emitter_update() __asm__("FUN_L08_002f5830");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drone_update() __asm__("FUN_L00_002ce358");
void effect_emitter_update() __asm__("FUN_L08_00308c90");
void enter_scoutbot_return_home_state() __asm__("FUN_L08_002d4050");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void falling_debris_update() __asm__("FUN_L08_002e16c0");
void fire_bolt_update() __asm__("FUN_L08_00307298");
void flickering_mesh_effect_update() __asm__("FUN_L08_002f70a0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void ground_particle_scatter_update() __asm__("FUN_L08_00307cf0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void gunship_update() __asm__("FUN_L08_002e24e8");
void heavy_gunship_update() __asm__("FUN_L08_002e9b70");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L08_00307540");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_camera_transform_and_paths() __asm__("FUN_L01_00315de0");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void landmine_update() __asm__("FUN_L08_002f44c0");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void layered_mesh_effect_update() __asm__("FUN_L08_002f1548");
void lift_update() __asm__("FUN_L08_002ea398");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_248_update() __asm__("FUN_L08_002d23f0");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void path_following_moby_update() __asm__("FUN_L08_002de848");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void radial_particle_emitter_update() __asm__("FUN_L08_003085f0");
void rail_hero_motion_update() __asm__("FUN_L01_002f6328");
void red_plant_update() __asm__("FUN_L00_002e3ca8");
void respawn_volume_update() __asm__("FUN_L06_002f7930");
void rocket_update() __asm__("FUN_L08_002dd3c0");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_projectile_update() __asm__("FUN_L01_0030c9a8");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void scoutbot_update() __asm__("FUN_L08_002d2af0");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void shattering_prop_update() __asm__("FUN_L08_0030bc00");
void shrub_update() __asm__("FUN_L08_002e8cd0");
void shuttle_update() __asm__("FUN_L08_002dbdb0");
void smoke_trail_projectile_update() __asm__("FUN_L01_002eda00");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swing_gate_update() __asm__("FUN_L08_002ea4c0");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void the_plumber_batalia_update() __asm__("FUN_L08_003065d8");
void the_plumber_update() __asm__("FUN_L01_002ff118");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void trackdrive_left_update() __asm__("FUN_L02_002cb978");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void turret_rocket_update() __asm__("FUN_L08_002e8210");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");
void update_ground_height_modulated_voice() __asm__("FUN_L08_00316fa8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void visibomb_zone_timer_update() __asm__("FUN_L01_00302648");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void water_surface_update() __asm__("FUN_L08_002da0f0");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L08_001FD1E8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L08_001FD200[6] __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                enter_scoutbot_return_home_state,
                                                                get_moby_data_plus_0x60,
                                                                hud_counter_update};

void *D_L08_001FD218[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[190] __asm__("D_L08_001FC900")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,             camera_object_update, default_moby_methods},
        {  11,                    vendor_update, default_moby_methods},
        {  13,                      bolt_update, default_moby_methods},
        {  14,                      bolt_update, default_moby_methods},
        {  15,                      bolt_update, default_moby_methods},
        {  16,                      bolt_update, default_moby_methods},
        {  71,                    wrench_update, default_moby_methods},
        {  74,                      mine_update, default_moby_methods},
        { 112,             glow_particle_update, default_moby_methods},
        { 121,                      bomb_update, default_moby_methods},
        { 122,                    debris_update, default_moby_methods},
        { 149,   nanotech_crate_fragment_update, default_moby_methods},
        { 152,               bolt_pickup_update, default_moby_methods},
        { 153,   gold_devastator_missile_update, default_moby_methods},
        { 156,      billboard_projectile_update, default_moby_methods},
        { 157,                devastator_update, default_moby_methods},
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
        { 222,           devestator_ammo_update, default_moby_methods},
        { 223,           devestator_ammo_update, default_moby_methods},
        { 225,           devestator_ammo_update, default_moby_methods},
        { 226,           devestator_ammo_update, default_moby_methods},
        { 229,             glove_of_doom_update, default_moby_methods},
        { 230,          doom_bot_spawner_update, default_moby_methods},
        { 248,                  moby_248_update, default_moby_methods},
        { 252,                  scoutbot_update,       D_L08_001FD200},
        { 253,               bomber_tank_update, default_moby_methods},
        { 270,         morphoray_chicken_update,       D_L08_001FD218},
        { 280,                bolt_crank_update, default_moby_methods},
        { 305,               homing_shot_update, default_moby_methods},
        { 324,               noop_main_callback, default_moby_methods},
        { 327,             water_surface_update, default_moby_methods},
        { 331,           trackdrive_left_update, default_moby_methods},
        { 333,         blarg_bombthrower_update, default_moby_methods},
        { 344,                 blarg_gun_update, default_moby_methods},
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
        { 424,                   shuttle_update, default_moby_methods},
        { 425,          bomber_tank_bomb_update, default_moby_methods},
        { 428,       damped_falling_moby_update, default_moby_methods},
        { 433,               noop_main_callback, default_moby_methods},
        { 435,                    rocket_update, default_moby_methods},
        { 438,       path_following_moby_update, default_moby_methods},
        { 440,            batalia_cannon_update, default_moby_methods},
        { 441,            falling_debris_update, default_moby_methods},
        { 442,            falling_debris_update, default_moby_methods},
        { 443,            falling_debris_update, default_moby_methods},
        { 444,                   gunship_update, default_moby_methods},
        { 445,            falling_debris_update, default_moby_methods},
        { 448,            falling_debris_update, default_moby_methods},
        { 449,            falling_debris_update, default_moby_methods},
        { 450,            falling_debris_update, default_moby_methods},
        { 451,            falling_debris_update, default_moby_methods},
        { 452,                bolt_thief_update, default_moby_methods},
        { 454,                      ryno_update, default_moby_methods},
        { 455,              bomb_spawner_update, default_moby_methods},
        { 457,               ryno_rocket_update, default_moby_methods},
        { 458,             turret_rocket_update, default_moby_methods},
        { 462,                     shrub_update, default_moby_methods},
        { 463,             heavy_gunship_update, default_moby_methods},
        { 464,            falling_debris_update, default_moby_methods},
        { 465,            falling_debris_update, default_moby_methods},
        { 467,                      lift_update, default_moby_methods},
        { 468,                swing_gate_update, default_moby_methods},
        { 469,                swing_gate_update, default_moby_methods},
        { 472,                      lift_update, default_moby_methods},
        { 479,                     drone_update, default_moby_methods},
        { 483,                                0, default_moby_methods},
        { 492,                 red_plant_update, default_moby_methods},
        { 500,                     crate_update, default_moby_methods},
        { 501,                     crate_update, default_moby_methods},
        { 505,                     crate_update, default_moby_methods},
        { 511,                     crate_update, default_moby_methods},
        { 545,         bomb_spawner_bomb_update, default_moby_methods},
        { 547,                 blarg_gun_update, default_moby_methods},
        { 548,                 blarg_gun_update, default_moby_methods},
        { 549,                 blarg_gun_update, default_moby_methods},
        { 550,                 blarg_gun_update, default_moby_methods},
        { 551,                 blarg_gun_update, default_moby_methods},
        { 562,               decoy_glove_update, default_moby_methods},
        { 566,               noop_main_callback, default_moby_methods},
        { 567,    smoke_trail_projectile_update, default_moby_methods},
        { 585,            metal_detector_update, default_moby_methods},
        { 588,                 blarg_gun_update, default_moby_methods},
        { 589,                 blarg_gun_update, default_moby_methods},
        { 590,                 blarg_gun_update, default_moby_methods},
        { 591,                 blarg_gun_update, default_moby_methods},
        { 592,                 blarg_gun_update, default_moby_methods},
        { 593,                 blarg_gun_update, default_moby_methods},
        { 594,                 blarg_gun_update, default_moby_methods},
        { 595,                 blarg_gun_update, default_moby_methods},
        { 596,                 blarg_gun_update, default_moby_methods},
        { 597,                 blarg_gun_update, default_moby_methods},
        { 598,                 blarg_gun_update, default_moby_methods},
        { 600,       layered_mesh_effect_update, default_moby_methods},
        { 604,           sandmouse_house_update, default_moby_methods},
        { 605,              buried_bolts_update, default_moby_methods},
        { 607,                  helipack_update, default_moby_methods},
        { 615,           tresspasser_pad_update, default_moby_methods},
        { 618,               noop_main_callback, default_moby_methods},
        { 621,                  landmine_update, default_moby_methods},
        { 634,                                0, default_moby_methods},
        { 639,           explosion_light_update, default_moby_methods},
        { 648, directional_spark_emitter_update, default_moby_methods},
        { 671,    flickering_mesh_effect_update, default_moby_methods},
        { 679,          rail_hero_motion_update, default_moby_methods},
        { 750,                   infobot_update, default_moby_methods},
        { 774,               the_plumber_update, default_moby_methods},
        { 775,         grow_and_fade_out_update, default_moby_methods},
        { 782,                 blarg_gun_update, default_moby_methods},
        { 783,                 blarg_gun_update, default_moby_methods},
        { 784,                 blarg_gun_update, default_moby_methods},
        { 785,                 blarg_gun_update, default_moby_methods},
        { 804,   morphoray_target_effect_update, default_moby_methods},
        { 805,               spawn_point_update, default_moby_methods},
        { 806,             nanotech_ball_update, default_moby_methods},
        { 832,       visibomb_zone_timer_update, default_moby_methods},
        { 849,                suckcannon_update, default_moby_methods},
        {1006,           devestator_ammo_update, default_moby_methods},
        {1023,         blarg_paratrooper_update, default_moby_methods},
        {1039,            respawn_volume_update, default_moby_methods},
        {1130,                  commando_update, default_moby_methods},
        {1134,                 gold_bolt_update, default_moby_methods},
        {1135,                teleporter_update, default_moby_methods},
        {1138,                 quad_moby_update, default_moby_methods},
        {1143,                vendor_orb_update, default_moby_methods},
        {1144,          batalia_deserter_update, default_moby_methods},
        {1192,             glow_particle_update, default_moby_methods},
        {1251,            hydrodisplacer_update, default_moby_methods},
        {1283,       the_plumber_batalia_update, default_moby_methods},
        {1289,               noop_main_callback, default_moby_methods},
        {1290,             pilots_helmet_update, default_moby_methods},
        {1292,                 fire_bolt_update, default_moby_methods},
        {1349,                help_hints_update, default_moby_methods},
        {1400,   ground_particle_scatter_update, default_moby_methods},
        {1438,           devestator_ammo_update, default_moby_methods},
        {1447,           devestator_ammo_update, default_moby_methods},
        {1449,           devestator_ammo_update, default_moby_methods},
        {1553,    cutscene_camera_target_update, default_moby_methods},
        {1629,   radial_particle_emitter_update, default_moby_methods},
        {1633,      sandmouse_projectile_update, default_moby_methods},
        {1641,            effect_emitter_update, default_moby_methods},
        {1649,   batalia_textured_effect_update, default_moby_methods},
        {1692,             tumbling_part_update, default_moby_methods},
        {1693,             tumbling_part_update, default_moby_methods},
        {1694,             tumbling_part_update, default_moby_methods},
        {1728,             tumbling_part_update, default_moby_methods},
        {1729,             tumbling_part_update, default_moby_methods},
        {1730,             tumbling_part_update, default_moby_methods},
        {1731,             tumbling_part_update, default_moby_methods},
        {1732,             tumbling_part_update, default_moby_methods},
        {1733,             tumbling_part_update, default_moby_methods},
        {1734,             tumbling_part_update, default_moby_methods},
        {1735,             tumbling_part_update, default_moby_methods},
        {1790,             tumbling_part_update, default_moby_methods},
        {1791,             tumbling_part_update, default_moby_methods},
        {1818,                 sandmouse_update, default_moby_methods},
        {1844,           shattering_prop_update, default_moby_methods},
        {1845,               noop_main_callback, default_moby_methods},
        {1846,             tumbling_part_update, default_moby_methods},
        {1847,             tumbling_part_update, default_moby_methods},
        {1900,                     decoy_update, default_moby_methods},
        {  -1,                                0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L08_001FD280") __attribute__((section("lvl.camvtbl"))) = {
    { 0,    noop_callback_l,                   follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                           },
    { 3,
     noop_callback_l, initialize_camera_transform_and_paths,
     update_camera_position_and_orientation_from_paths,              update_camera_mode_from_preset                                          },
    { 4,
     camera_transition_update,                      FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                      },
    { 5,
     noop_callback_l,        initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                                 },
    { 6,            noop_callback_l,     initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l,     initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                              },
    {18, update_moby_camera_control,                    noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,        volume_camera_check,                    noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                          0,                                     0,                    0,                                           0},
};

struct SoundHandler sound_handlers[6] __asm__("D_L08_001FD380")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 1,                        update_box_emitter},
        { 3,                 update_reverb_box_emitter},
        { 4,      update_ground_height_modulated_voice},
        { 6,               update_music_switch_emitter},
        {-1,                                         0},
};
