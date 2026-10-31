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
void amoeboid_update() __asm__("FUN_L05_002dac80");
void blarg_gun_update() __asm__("FUN_L05_003186f8");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void box_update() __asm__("FUN_L05_0030bf98");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_state_14() __asm__("FUN_L01_002efb98");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drain_cover_update() __asm__("FUN_L05_0030e7f8");
void electric_gate_update() __asm__("FUN_L05_0030dc68");
void elevator_with_button_update() __asm__("FUN_L05_003156d0");
void extermibot_update() __asm__("FUN_L05_00301f48");
void extermitank_update() __asm__("FUN_L05_00304320");
void floating_platform_update() __asm__("FUN_L05_0030bdd8");
void floor_button_update() __asm__("FUN_L05_0030c5b0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0xc0() __asm__("FUN_L01_002efbc8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hoverboard_ring_update() __asm__("FUN_L05_0031abe0");
void hoverboard_update() __asm__("FUN_L05_002f87a8");
void hydrodisplacer_faucet_update() __asm__("FUN_L05_002f8080");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_camera_transform_and_paths() __asm__("FUN_L01_00315de0");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void large_amoeboid_update() __asm__("FUN_L01_002edca0");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_hold_state_if_found() __asm__("FUN_L01_002efaf8");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void ocean_rilgar_update() __asm__("FUN_L05_00316110");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pipe_mounted_grating_update() __asm__("FUN_L05_0030f5c8");
void process_amoeboid_suck_cannon_suction() __asm__("FUN_L01_002efa88");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void qwark_rilgar_update() __asm__("FUN_L03_002dc478");
void qwarks_bouncer_update() __asm__("FUN_L05_00317470");
void qwarks_trailer_update() __asm__("FUN_L05_00319c28");
void red_orb_update() __asm__("FUN_L00_002df4f8");
void refresh_point_light() __asm__("FUN_00201f58");
void reset_camera_from_source_transform() __asm__("FUN_L05_0032a6e0");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void ryno_update() __asm__("FUN_L00_002cc608");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void scenery_ship_update() __asm__("FUN_L05_002d7140");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void shady_salesman_update() __asm__("FUN_L05_003180a0");
void sharkigator_update() __asm__("FUN_L05_002d1688");
void skid_mcmarx_update() __asm__("FUN_L05_00307910");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void taxi_update() __asm__("FUN_L05_00318c78");
void television_update() __asm__("FUN_L05_00318b98");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void trespasser_update() __asm__("FUN_L00_002c1718");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");
void update_cutscene_camera_target() __asm__("FUN_L05_0031d5c8");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_doom_bot_spawner() __asm__("FUN_L00_002c89e8");
void update_drone() __asm__("FUN_L00_002ce358");
void update_fog_zone() __asm__("FUN_L05_0030e1f8");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_help_hints() __asm__("FUN_L05_0031bf40");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_hero_clip_volume_outputs() __asm__("FUN_L05_00318a68");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_hoverboard_girl() __asm__("FUN_L05_00316ab8");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_listener_emitter() __asm__("FUN_L01_0031a078");
void update_moby_glow_and_contact_explosion() __asm__("FUN_L05_0031c8e0");
void update_moby_spatial_trigger() __asm__("FUN_L05_002f5200");
void update_moby_wobble() __asm__("FUN_L05_0030c0a8");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_rail_hero_motion() __asm__("FUN_L01_002f6328");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_rising_gate() __asm__("FUN_L05_00314eb0");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void update_sandmouse_projectile() __asm__("FUN_L01_0030c9a8");
void update_sound_emitter_playback_at_random_position() __asm__("FUN_L00_002edad8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_triggered_moby_motion_and_manipulator_angle() __asm__("FUN_L05_00316258");
void update_triggered_moby_slide_and_lowering() __asm__("FUN_L05_0030e508");
void update_triggered_moby_yaw() __asm__("FUN_L05_003166a0");
void update_triggered_particle_spray_emitter() __asm__("FUN_L05_003150f0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void walloper_update() __asm__("FUN_L00_002bb558");
void water_update() __asm__("FUN_L05_0030ca80");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L05_002193B8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L05_002193D0[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

void *D_L05_002193E8[6]

    __attribute__((section("lvl.vtbl"))) = {process_amoeboid_suck_cannon_suction,
                                            moby_hold_state_if_found,
                                            moby_hold_state_if_found,
                                            cancel_suck_cannon_suction_and_set_state_14,
                                            get_moby_data_plus_0xc0,
                                            mine_blend_color};

void *D_L05_00219400[6]

    __attribute__((section("lvl.vtbl"))) = {process_amoeboid_suck_cannon_suction,
                                            moby_hold_state_if_found,
                                            moby_hold_state_if_found,
                                            cancel_suck_cannon_suction_and_set_state_14,
                                            get_moby_data_plus_0xc0,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[154] __asm__("D_L05_00218C80")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,                               camera_object_update, default_moby_methods},
        {  11,                                      vendor_update, default_moby_methods},
        {  13,                                        bolt_update, default_moby_methods},
        {  14,                                        bolt_update, default_moby_methods},
        {  15,                                        bolt_update, default_moby_methods},
        {  16,                                        bolt_update, default_moby_methods},
        {  35,                                 sharkigator_update, default_moby_methods},
        {  71,                                      wrench_update, default_moby_methods},
        {  74,                                        mine_update, default_moby_methods},
        {  79,                                scenery_ship_update, default_moby_methods},
        { 112,                               update_glow_particle, default_moby_methods},
        { 121,                                        bomb_update, default_moby_methods},
        { 122,                                      debris_update, default_moby_methods},
        { 133,                                    amoeboid_update, default_moby_methods},
        { 149,                     nanotech_crate_fragment_update, default_moby_methods},
        { 153,                     gold_devastator_missile_update, default_moby_methods},
        { 157,                                  devastator_update, default_moby_methods},
        { 163,                                    visibomb_update, default_moby_methods},
        { 166,                          update_linked_moby_states, default_moby_methods},
        { 167,                         update_thruster_pack_flame, default_moby_methods},
        { 168,                                     blaster_update, default_moby_methods},
        { 172,                            visibomb_missile_update, default_moby_methods},
        { 175,                                     taunter_update, default_moby_methods},
        { 176,                                   pyrocitor_update, default_moby_methods},
        { 177,                                  tesla_claw_update, default_moby_methods},
        { 179,              update_linked_moby_scale_and_rotation, default_moby_methods},
        { 180,                                    walloper_update, default_moby_methods},
        { 185,                                   morphoray_update, default_moby_methods},
        { 186,                                    doom_bot_update, default_moby_methods},
        { 188,                                  trespasser_update, default_moby_methods},
        { 190,                                  mine_glove_update, default_moby_methods},
        { 192,                                  bomb_glove_update, default_moby_methods},
        { 203,                                       decoy_update, default_moby_methods},
        { 204,                             devestator_ammo_update, default_moby_methods},
        { 208,                                   swingshot_update, default_moby_methods},
        { 209,                              swingshot_hook_update, default_moby_methods},
        { 213,                             devestator_ammo_update, default_moby_methods},
        { 214,                             devestator_ammo_update, default_moby_methods},
        { 222,                             devestator_ammo_update, default_moby_methods},
        { 223,                             devestator_ammo_update, default_moby_methods},
        { 225,                             devestator_ammo_update, default_moby_methods},
        { 226,                             devestator_ammo_update, default_moby_methods},
        { 229,                               glove_of_doom_update, default_moby_methods},
        { 230,                            update_doom_bot_spawner, default_moby_methods},
        { 258,                        update_moby_spatial_trigger, default_moby_methods},
        { 270,                           update_morphoray_chicken,       D_L05_002193D0},
        { 305,                                 update_homing_shot, default_moby_methods},
        { 341,                       hydrodisplacer_faucet_update, default_moby_methods},
        { 348,                     nanotech_crate_fragment_update, default_moby_methods},
        { 349,                     nanotech_crate_fragment_update, default_moby_methods},
        { 354,                     nanotech_crate_fragment_update, default_moby_methods},
        { 355,                     nanotech_crate_fragment_update, default_moby_methods},
        { 356,                     nanotech_crate_fragment_update, default_moby_methods},
        { 357,                     nanotech_crate_fragment_update, default_moby_methods},
        { 358,                     nanotech_crate_fragment_update, default_moby_methods},
        { 359,                     nanotech_crate_fragment_update, default_moby_methods},
        { 363,                     nanotech_crate_fragment_update, default_moby_methods},
        { 364,                     nanotech_crate_fragment_update, default_moby_methods},
        { 428,                         update_damped_falling_moby, default_moby_methods},
        { 433,                                 noop_main_callback, default_moby_methods},
        { 439,                                  hoverboard_update, default_moby_methods},
        { 454,                                        ryno_update, default_moby_methods},
        { 457,                                 update_ryno_rocket, default_moby_methods},
        { 479,                                       update_drone, default_moby_methods},
        { 483,                                                  0, default_moby_methods},
        { 500,                                       crate_update, default_moby_methods},
        { 501,                                       crate_update, default_moby_methods},
        { 502,                                       crate_update, default_moby_methods},
        { 505,                                       crate_update, default_moby_methods},
        { 511,                                       crate_update, default_moby_methods},
        { 562,                                 decoy_glove_update, default_moby_methods},
        { 572,                              large_amoeboid_update,       D_L05_002193E8},
        { 585,                              metal_detector_update, default_moby_methods},
        { 604,                             sandmouse_house_update, default_moby_methods},
        { 605,                 update_nearest_area_moby_selection, default_moby_methods},
        { 607,                                    helipack_update, default_moby_methods},
        { 618,                                 noop_main_callback, default_moby_methods},
        { 623,                                  extermibot_update, default_moby_methods},
        { 625,                                 extermitank_update, default_moby_methods},
        { 634,                                                  0, default_moby_methods},
        { 639,                   update_animated_point_light_moby, default_moby_methods},
        { 679,                            update_rail_hero_motion, default_moby_methods},
        { 717,                                 skid_mcmarx_update, default_moby_methods},
        { 750,                                     infobot_update, default_moby_methods},
        { 758,                       swingshot_versatarget_update, default_moby_methods},
        { 775,                           grow_and_fade_out_update, default_moby_methods},
        { 803,                       swingshot_versatarget_update, default_moby_methods},
        { 804,          update_linked_moby_transform_and_lifetime, default_moby_methods},
        { 805,                                 spawn_point_update, default_moby_methods},
        { 806,                               nanotech_ball_update, default_moby_methods},
        { 810,                           floating_platform_update, default_moby_methods},
        { 812,                                         box_update, default_moby_methods},
        { 823,                                 update_moby_wobble, default_moby_methods},
        { 830,                                floor_button_update, default_moby_methods},
        { 831,                                       water_update, default_moby_methods},
        { 832,                 update_target_outside_zone_timeout, default_moby_methods},
        { 838,                               electric_gate_update, default_moby_methods},
        { 841,                                    update_fog_zone, default_moby_methods},
        { 843,           update_triggered_moby_slide_and_lowering, default_moby_methods},
        { 844,                                 drain_cover_update, default_moby_methods},
        { 846,                        pipe_mounted_grating_update, default_moby_methods},
        { 849,                                  suckcannon_update, default_moby_methods},
        { 852,                                 update_rising_gate, default_moby_methods},
        { 853,                                 update_rising_gate, default_moby_methods},
        { 855,            update_triggered_particle_spray_emitter, default_moby_methods},
        { 865,                              large_amoeboid_update, default_moby_methods},
        { 866,                              large_amoeboid_update,       D_L05_00219400},
        { 877,                        elevator_with_button_update, default_moby_methods},
        { 879,                                ocean_rilgar_update, default_moby_methods},
        { 893, update_triggered_moby_motion_and_manipulator_angle, default_moby_methods},
        { 895,                          update_triggered_moby_yaw, default_moby_methods},
        { 918,                             update_hoverboard_girl, default_moby_methods},
        { 919,                              qwarks_bouncer_update, default_moby_methods},
        { 920,                                qwark_rilgar_update, default_moby_methods},
        { 925,                              shady_salesman_update, default_moby_methods},
        { 980,                                   blarg_gun_update, default_moby_methods},
        { 981,                                   blarg_gun_update, default_moby_methods},
        { 982,                    update_hero_clip_volume_outputs, default_moby_methods},
        { 984,                                  television_update, default_moby_methods},
        { 998,                                        taxi_update, default_moby_methods},
        {1006,                             devestator_ammo_update, default_moby_methods},
        {1060,                                     red_orb_update, default_moby_methods},
        {1099,                              qwarks_trailer_update, default_moby_methods},
        {1134,                                   gold_bolt_update, default_moby_methods},
        {1138,                                   quad_moby_update, default_moby_methods},
        {1139,                             hoverboard_ring_update, default_moby_methods},
        {1140,                                 noop_main_callback, default_moby_methods},
        {1143,                                  vendor_orb_update, default_moby_methods},
        {1192,                               update_glow_particle, default_moby_methods},
        {1251,                              hydrodisplacer_update, default_moby_methods},
        {1289,                                 noop_main_callback, default_moby_methods},
        {1290,                               pilots_helmet_update, default_moby_methods},
        {1347,                                  update_help_hints, default_moby_methods},
        {1438,                             devestator_ammo_update, default_moby_methods},
        {1447,                             devestator_ammo_update, default_moby_methods},
        {1449,                             devestator_ammo_update, default_moby_methods},
        {1511,             update_moby_glow_and_contact_explosion, default_moby_methods},
        {1514,                                 noop_main_callback, default_moby_methods},
        {1517,                                   robot_arm_update, default_moby_methods},
        {1518,                                   robot_arm_update, default_moby_methods},
        {1550,                      update_cutscene_camera_target, default_moby_methods},
        {1633,                        update_sandmouse_projectile, default_moby_methods},
        {1754,                                   robot_arm_update, default_moby_methods},
        {1755,                                   robot_arm_update, default_moby_methods},
        {1756,                                   robot_arm_update, default_moby_methods},
        {1757,                                   robot_arm_update, default_moby_methods},
        {1759,                                   robot_arm_update, default_moby_methods},
        {1760,                                   robot_arm_update, default_moby_methods},
        {1790,                                   robot_arm_update, default_moby_methods},
        {1791,                                   robot_arm_update, default_moby_methods},
        {1818,                                   sandmouse_update, default_moby_methods},
        {1838,                                   robot_arm_update, default_moby_methods},
        {1900,                                       decoy_update, default_moby_methods},
        {  -1,                                                  0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L05_00219480") __attribute__((section("lvl.camvtbl"))) = {
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
    { 8,
     noop_callback_l,    reset_camera_from_source_transform,
     refresh_point_light,                          noop_main_callback                                                                  },
    {17,     FUN_L00_002ed230,                    noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                    0,                                     0,                    0,                                           0},
};

struct SoundHandler sound_handlers[7] __asm__("D_L05_00219580")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0,        update_sound_emitter_playback_by_distance},
        { 1,                               update_box_emitter},
        { 2, update_sound_emitter_playback_at_random_position},
        { 3,                        update_reverb_box_emitter},
        { 5,                          update_listener_emitter},
        { 6,                      update_music_switch_emitter},
        {-1,                                                0},
};
