/* The level's moby classes, camera types and sound emitter types: the tables
   the executable's code looks ids up in (sections lvl.vtbl, lvl.camvtbl and
   lvl.sndvtbl). */
#include "types.h"
#include "rnc/audio/sound_handlers.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby_class_tables.h"

void FUN_L00_002caf40();
void FUN_L00_002ea7a0();
void blarg_helicopter_update() __asm__("FUN_L03_002cca00");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void bombomatic_update() __asm__("FUN_L07_0030d4d8");
void bridge_update() __asm__("FUN_L07_0031aee0");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void door_update() __asm__("FUN_L07_00310df0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void homing_mine_update() __asm__("FUN_L07_00319040");
void hydrodisplacer_faucet_update() __asm__("FUN_L05_002f8080");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void machinegun_turret_update() __asm__("FUN_L07_0031a250");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void mushroom_update() __asm__("FUN_L07_0031f3d8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void ocean_umbris_update() __asm__("FUN_L07_0030d440");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pterodactyl_update() __asm__("FUN_L00_002e3a88");
void purple_ship_update() __asm__("FUN_L07_002fbdb8");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void qwark_rilgar_update() __asm__("FUN_L03_002dc478");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void robot_head_update() __asm__("FUN_L07_0031f5b0");
void rock_update() __asm__("FUN_L01_002f8718");
void ryno_update() __asm__("FUN_L00_002cc608");
void seashell_update() __asm__("FUN_L07_0031f780");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void sharkigator_update() __asm__("FUN_L07_0030f5f0");
void snagglebeast_update() __asm__("FUN_L07_00314150");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void timed_activation_pads_update() __asm__("FUN_L07_0030bf90");
void trespasser_update() __asm__("FUN_L00_002c1718");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_bomb() __asm__("FUN_L07_0030bbc8");
void update_breakable_prop() __asm__("FUN_L07_00319f48");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_command_triggered_falling_platform() __asm__("FUN_L07_003112c8");
void update_cutscene_effects() __asm__("FUN_L07_0031eba8");
void update_cutscene_joint_trails() __asm__("FUN_L07_0031f900");
void update_cutscene_trigger() __asm__("FUN_L01_002fb5b0");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_doom_bot_spawner() __asm__("FUN_L00_002c89e8");
void update_drone() __asm__("FUN_L00_002ce358");
void update_equalizer_ammo_spawner() __asm__("FUN_L07_0031d2e0");
void update_fading_ring_effect() __asm__("FUN_L07_0030e1f8");
void update_falling_moby() __asm__("FUN_L03_002de370");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_growing_ballistic_moby() __asm__("FUN_L07_0030e458");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_hero_triggered_falling_platform() __asm__("FUN_L07_00311bc8");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_homing_spark_projectile() __asm__("FUN_L03_002d47c0");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_linked_state_sliding_platform() __asm__("FUN_L07_0030cf90");
void update_machinegun_turret_projectile() __asm__("FUN_L07_0030b6a8");
void update_mine() __asm__("FUN_L07_003196e0");
void update_mine_group() __asm__("FUN_L07_0030b000");
void update_mission_gated_taxi_platform() __asm__("FUN_L07_002cdb28");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_moby_draw_registration() __asm__("FUN_L07_002f8058");
void update_moby_spatial_trigger() __asm__("FUN_L05_002f5200");
void update_moby_wobble() __asm__("FUN_L05_0030c0a8");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void update_sliding_gate() __asm__("FUN_L07_0031a9e8");
void update_swing_gate() __asm__("FUN_L07_0031b3f0");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_taxi_platform() __asm__("FUN_L07_002cd2b0");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_umbris_scene_progression() __asm__("FUN_L07_002f5ba0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void water_update() __asm__("FUN_L07_0030c508");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L07_002125C4")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L07_002125DC[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[187] __asm__("D_L07_00211D00")

    __attribute__((section("lvl.vtbl"))) = {
        {  38,                      update_taxi_platform, default_moby_methods},
        {1007,                      camera_object_update, default_moby_methods},
        {1474,        update_mission_gated_taxi_platform, default_moby_methods},
        {  11,                             vendor_update, default_moby_methods},
        {  13,                               bolt_update, default_moby_methods},
        {  14,                               bolt_update, default_moby_methods},
        {  15,                               bolt_update, default_moby_methods},
        {  16,                               bolt_update, default_moby_methods},
        {  71,                             wrench_update, default_moby_methods},
        {  74,                               mine_update, default_moby_methods},
        { 112,                      update_glow_particle, default_moby_methods},
        { 121,                               bomb_update, default_moby_methods},
        { 122,                             debris_update, default_moby_methods},
        { 149,            nanotech_crate_fragment_update, default_moby_methods},
        { 153,            gold_devastator_missile_update, default_moby_methods},
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
        { 258,               update_moby_spatial_trigger, default_moby_methods},
        { 270,                  update_morphoray_chicken,       D_L07_002125DC},
        { 305,                        update_homing_shot, default_moby_methods},
        { 341,              hydrodisplacer_faucet_update, default_moby_methods},
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
        { 436,           update_umbris_scene_progression, default_moby_methods},
        { 447,                       qwark_rilgar_update, default_moby_methods},
        { 454,                               ryno_update, default_moby_methods},
        { 457,                        update_ryno_rocket, default_moby_methods},
        { 460,             update_moby_draw_registration, default_moby_methods},
        { 479,                              update_drone, default_moby_methods},
        { 483,                                         0, default_moby_methods},
        { 500,                              crate_update, default_moby_methods},
        { 501,                              crate_update, default_moby_methods},
        { 502,                              crate_update, default_moby_methods},
        { 505,                              crate_update, default_moby_methods},
        { 511,                              crate_update, default_moby_methods},
        { 529,                        purple_ship_update, default_moby_methods},
        { 532,                        noop_main_callback, default_moby_methods},
        { 562,                        decoy_glove_update, default_moby_methods},
        { 585,                     metal_detector_update, default_moby_methods},
        { 605,        update_nearest_area_moby_selection, default_moby_methods},
        { 607,                           helipack_update, default_moby_methods},
        { 618,                        noop_main_callback, default_moby_methods},
        { 631,                   blarg_helicopter_update, default_moby_methods},
        { 634,                                         0, default_moby_methods},
        { 639,          update_animated_point_light_moby, default_moby_methods},
        { 696,                               rock_update, default_moby_methods},
        { 697,                               rock_update, default_moby_methods},
        { 698,                               rock_update, default_moby_methods},
        { 737,                   update_cutscene_trigger, default_moby_methods},
        { 750,                            infobot_update, default_moby_methods},
        { 758,              swingshot_versatarget_update, default_moby_methods},
        { 775,                  grow_and_fade_out_update, default_moby_methods},
        { 803,              swingshot_versatarget_update, default_moby_methods},
        { 804, update_linked_moby_transform_and_lifetime, default_moby_methods},
        { 805,                        spawn_point_update, default_moby_methods},
        { 806,                      nanotech_ball_update, default_moby_methods},
        { 823,                        update_moby_wobble, default_moby_methods},
        { 832,        update_target_outside_zone_timeout, default_moby_methods},
        { 848,            update_homing_spark_projectile, default_moby_methods},
        { 849,                         suckcannon_update, default_moby_methods},
        { 871,                         update_mine_group, default_moby_methods},
        { 880,       update_machinegun_turret_projectile, default_moby_methods},
        { 882,                               update_bomb, default_moby_methods},
        { 886,              timed_activation_pads_update, default_moby_methods},
        { 902,                              water_update, default_moby_methods},
        { 920,                       qwark_rilgar_update, default_moby_methods},
        { 941,                              water_update, default_moby_methods},
        { 943,                              water_update, default_moby_methods},
        { 944,                              water_update, default_moby_methods},
        { 948,                              water_update, default_moby_methods},
        { 949,                              water_update, default_moby_methods},
        { 950,                              water_update, default_moby_methods},
        { 951,                              water_update, default_moby_methods},
        { 952,                              water_update, default_moby_methods},
        { 953,                              water_update, default_moby_methods},
        { 954,                              water_update, default_moby_methods},
        { 955,                              water_update, default_moby_methods},
        { 958,                              water_update, default_moby_methods},
        { 961,                              water_update, default_moby_methods},
        { 962,                              water_update, default_moby_methods},
        { 967,                              water_update, default_moby_methods},
        { 968,                              water_update, default_moby_methods},
        { 969,                              water_update, default_moby_methods},
        { 970,                              water_update, default_moby_methods},
        { 971,                              water_update, default_moby_methods},
        {1006,                    devestator_ammo_update, default_moby_methods},
        {1013,      update_linked_state_sliding_platform, default_moby_methods},
        {1014,      update_linked_state_sliding_platform, default_moby_methods},
        {1018,                       ocean_umbris_update, default_moby_methods},
        {1041,                         bombomatic_update, default_moby_methods},
        {1046,                 update_fading_ring_effect, default_moby_methods},
        {1049,             update_growing_ballistic_moby, default_moby_methods},
        {1059,                        sharkigator_update, default_moby_methods},
        {1063,                               door_update, default_moby_methods},
        {1064,      update_linked_state_sliding_platform, default_moby_methods},
        {1065,      update_linked_state_sliding_platform, default_moby_methods},
        {1069, update_command_triggered_falling_platform, default_moby_methods},
        {1078,                               door_update, default_moby_methods},
        {1079,                               door_update, default_moby_methods},
        {1080,    update_hero_triggered_falling_platform, default_moby_methods},
        {1104,                        noop_main_callback, default_moby_methods},
        {1106,                       snagglebeast_update, default_moby_methods},
        {1110,                        homing_mine_update, default_moby_methods},
        {1112,                               update_mine, default_moby_methods},
        {1113,                     update_breakable_prop, default_moby_methods},
        {1114,                     update_breakable_prop, default_moby_methods},
        {1115,                     update_breakable_prop, default_moby_methods},
        {1116,                     update_breakable_prop, default_moby_methods},
        {1126,                  machinegun_turret_update, default_moby_methods},
        {1128,                       update_sliding_gate, default_moby_methods},
        {1129,                             bridge_update, default_moby_methods},
        {1131,                               door_update, default_moby_methods},
        {1133,                         update_swing_gate, default_moby_methods},
        {1134,                          gold_bolt_update, default_moby_methods},
        {1135,                         teleporter_update, default_moby_methods},
        {1138,                          quad_moby_update, default_moby_methods},
        {1142,             update_equalizer_ammo_spawner, default_moby_methods},
        {1143,                         vendor_orb_update, default_moby_methods},
        {1192,                      update_glow_particle, default_moby_methods},
        {1221,                       update_falling_moby, default_moby_methods},
        {1222,                       update_falling_moby, default_moby_methods},
        {1251,                     hydrodisplacer_update, default_moby_methods},
        {1289,                        noop_main_callback, default_moby_methods},
        {1290,                      pilots_helmet_update, default_moby_methods},
        {1438,                    devestator_ammo_update, default_moby_methods},
        {1447,                    devestator_ammo_update, default_moby_methods},
        {1449,                    devestator_ammo_update, default_moby_methods},
        {1552,                   update_cutscene_effects, default_moby_methods},
        {1564,                        pterodactyl_update, default_moby_methods},
        {1569,                          robot_arm_update, default_moby_methods},
        {1570,                          robot_arm_update, default_moby_methods},
        {1591,                          robot_arm_update, default_moby_methods},
        {1592,                          robot_arm_update, default_moby_methods},
        {1593,                          robot_arm_update, default_moby_methods},
        {1650,                           mushroom_update, default_moby_methods},
        {1651,                        noop_main_callback, default_moby_methods},
        {1652,                          robot_arm_update, default_moby_methods},
        {1653,                          robot_arm_update, default_moby_methods},
        {1654,                          robot_arm_update, default_moby_methods},
        {1663,                         robot_head_update, default_moby_methods},
        {1677,                         robot_head_update, default_moby_methods},
        {1678,                          robot_arm_update, default_moby_methods},
        {1679,                          robot_arm_update, default_moby_methods},
        {1680,                          robot_arm_update, default_moby_methods},
        {1686,                           seashell_update, default_moby_methods},
        {1688,                          robot_arm_update, default_moby_methods},
        {1689,                          robot_arm_update, default_moby_methods},
        {1690,                          robot_arm_update, default_moby_methods},
        {1789,              update_cutscene_joint_trails, default_moby_methods},
        {1900,                              decoy_update, default_moby_methods},
        {  -1,                                         0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L07_00212600") __attribute__((section("lvl.camvtbl"))) = {
    { 0,    noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                       },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                  },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                             },
    { 6,            noop_callback_l, initialize_orbit_camera_from_hero,     mine_blend_color,                          noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                          },
    {18, update_moby_camera_control,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,        volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                          0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[3] __asm__("D_L07_00212700")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 3,   update_reverb_box_emitter},
        { 6, update_music_switch_emitter},
        {-1,                           0},
};
