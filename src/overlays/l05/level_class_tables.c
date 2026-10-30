/* The level's moby classes, camera types and sound emitter types: the tables
   the executable's code looks ids up in (sections lvl.vtbl, lvl.camvtbl and
   lvl.sndvtbl). */
#include "types.h"
#include "rnc/audio/sound_handlers.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby_class_tables.h"

void FUN_L00_002b6140();
void FUN_L00_002c89e8();
void FUN_L00_002caf40();
void FUN_L00_002cc608();
void FUN_L00_002ea7a0();
void FUN_L00_002ed230();
void FUN_L01_0030df40();
void FUN_L05_00317470();
void FUN_L05_0031bf40();
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
void qwarks_trailer_update() __asm__("FUN_L05_00319c28");
void red_orb_update() __asm__("FUN_L00_002df4f8");
void refresh_point_light() __asm__("FUN_00201f58");
void reset_camera_from_source_transform() __asm__("FUN_L05_0032a6e0");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void rocket_update() __asm__("FUN_L00_002cd1f0");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
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
void update_beam_effect_lifecycle() __asm__("FUN_L00_002b4198");
void update_box_positional_voice() __asm__("FUN_L01_00319928");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_drone() __asm__("FUN_L00_002ce358");
void update_fog_zone() __asm__("FUN_L05_0030e1f8");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_hero_clip_volume_outputs() __asm__("FUN_L05_00318a68");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_hoverboard_girl() __asm__("FUN_L05_00316ab8");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_listener_following_voice() __asm__("FUN_L01_0031a078");
void update_moby_glow_and_contact_explosion() __asm__("FUN_L05_0031c8e0");
void update_moby_spatial_trigger() __asm__("FUN_L05_002f5200");
void update_moby_wobble() __asm__("FUN_L05_0030c0a8");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_box_exit_trigger() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_rail_hero_motion() __asm__("FUN_L01_002f6328");
void update_requested_state_from_box_position() __asm__("FUN_L01_00319f18");
void update_sandmouse_projectile() __asm__("FUN_L01_0030c9a8");
void update_selected_moby_effects() __asm__("FUN_L05_0031d5c8");
void update_sound_emitter_playback_at_random_position() __asm__("FUN_L00_002edad8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_triggered_gate_motion() __asm__("FUN_L05_00314eb0");
void update_triggered_moby_motion_and_manipulator_angle() __asm__("FUN_L05_00316258");
void update_triggered_moby_slide_and_lowering() __asm__("FUN_L05_0030e508");
void update_triggered_moby_yaw() __asm__("FUN_L05_003166a0");
void update_triggered_particle_spray_emitter() __asm__("FUN_L05_003150f0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
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
        {0x3ef,                               camera_object_update, default_moby_methods},
        {  0xb,                                      vendor_update, default_moby_methods},
        {  0xd,                                        bolt_update, default_moby_methods},
        {  0xe,                                        bolt_update, default_moby_methods},
        {  0xf,                                        bolt_update, default_moby_methods},
        { 0x10,                                        bolt_update, default_moby_methods},
        { 0x23,                                 sharkigator_update, default_moby_methods},
        { 0x47,                                      wrench_update, default_moby_methods},
        { 0x4a,                                        mine_update, default_moby_methods},
        { 0x4f,                                scenery_ship_update, default_moby_methods},
        { 0x70,                               update_glow_particle, default_moby_methods},
        { 0x79,                                        bomb_update, default_moby_methods},
        { 0x7a,                                      debris_update, default_moby_methods},
        { 0x85,                                    amoeboid_update, default_moby_methods},
        { 0x95,                     nanotech_crate_fragment_update, default_moby_methods},
        { 0x99,                     gold_devastator_missile_update, default_moby_methods},
        { 0x9d,                                  devastator_update, default_moby_methods},
        { 0xa3,                                    visibomb_update, default_moby_methods},
        { 0xa6,                          update_linked_moby_states, default_moby_methods},
        { 0xa7,                       update_beam_effect_lifecycle, default_moby_methods},
        { 0xa8,                                     blaster_update, default_moby_methods},
        { 0xac,                                   FUN_L00_002b6140, default_moby_methods},
        { 0xaf,                                     taunter_update, default_moby_methods},
        { 0xb0,                                   pyrocitor_update, default_moby_methods},
        { 0xb1,                                  tesla_claw_update, default_moby_methods},
        { 0xb3,              update_linked_moby_scale_and_rotation, default_moby_methods},
        { 0xb4,                                    walloper_update, default_moby_methods},
        { 0xb9,                                   morphoray_update, default_moby_methods},
        { 0xba,                                    doom_bot_update, default_moby_methods},
        { 0xbc,                                  trespasser_update, default_moby_methods},
        { 0xbe,                                  mine_glove_update, default_moby_methods},
        { 0xc0,                                  bomb_glove_update, default_moby_methods},
        { 0xcb,                                       decoy_update, default_moby_methods},
        { 0xcc,                             devestator_ammo_update, default_moby_methods},
        { 0xd0,                                   swingshot_update, default_moby_methods},
        { 0xd1,                              swingshot_hook_update, default_moby_methods},
        { 0xd5,                             devestator_ammo_update, default_moby_methods},
        { 0xd6,                             devestator_ammo_update, default_moby_methods},
        { 0xde,                             devestator_ammo_update, default_moby_methods},
        { 0xdf,                             devestator_ammo_update, default_moby_methods},
        { 0xe1,                             devestator_ammo_update, default_moby_methods},
        { 0xe2,                             devestator_ammo_update, default_moby_methods},
        { 0xe5,                               glove_of_doom_update, default_moby_methods},
        { 0xe6,                                   FUN_L00_002c89e8, default_moby_methods},
        {0x102,                        update_moby_spatial_trigger, default_moby_methods},
        {0x10e,                           update_morphoray_chicken,       D_L05_002193D0},
        {0x131,                                 update_homing_shot, default_moby_methods},
        {0x155,                       hydrodisplacer_faucet_update, default_moby_methods},
        {0x15c,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x15d,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x162,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x163,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x164,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x165,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x166,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x167,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x16b,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x16c,                     nanotech_crate_fragment_update, default_moby_methods},
        {0x1ac,                         update_damped_falling_moby, default_moby_methods},
        {0x1b1,                                 noop_main_callback, default_moby_methods},
        {0x1b7,                                  hoverboard_update, default_moby_methods},
        {0x1c6,                                   FUN_L00_002cc608, default_moby_methods},
        {0x1c9,                                      rocket_update, default_moby_methods},
        {0x1df,                                       update_drone, default_moby_methods},
        {0x1e3,                                                  0, default_moby_methods},
        {0x1f4,                                       crate_update, default_moby_methods},
        {0x1f5,                                       crate_update, default_moby_methods},
        {0x1f6,                                       crate_update, default_moby_methods},
        {0x1f9,                                       crate_update, default_moby_methods},
        {0x1ff,                                       crate_update, default_moby_methods},
        {0x232,                                 decoy_glove_update, default_moby_methods},
        {0x23c,                              large_amoeboid_update,       D_L05_002193E8},
        {0x249,                              metal_detector_update, default_moby_methods},
        {0x25c,                             sandmouse_house_update, default_moby_methods},
        {0x25d,                 update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                                    helipack_update, default_moby_methods},
        {0x26a,                                 noop_main_callback, default_moby_methods},
        {0x26f,                                  extermibot_update, default_moby_methods},
        {0x271,                                 extermitank_update, default_moby_methods},
        {0x27a,                                                  0, default_moby_methods},
        {0x27f,                   update_animated_point_light_moby, default_moby_methods},
        {0x2a7,                            update_rail_hero_motion, default_moby_methods},
        {0x2cd,                                 skid_mcmarx_update, default_moby_methods},
        {0x2ee,                                     infobot_update, default_moby_methods},
        {0x2f6,                       swingshot_versatarget_update, default_moby_methods},
        {0x307,                           grow_and_fade_out_update, default_moby_methods},
        {0x323,                       swingshot_versatarget_update, default_moby_methods},
        {0x324,          update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                                 spawn_point_update, default_moby_methods},
        {0x326,                               nanotech_ball_update, default_moby_methods},
        {0x32a,                           floating_platform_update, default_moby_methods},
        {0x32c,                                         box_update, default_moby_methods},
        {0x337,                                 update_moby_wobble, default_moby_methods},
        {0x33e,                                floor_button_update, default_moby_methods},
        {0x33f,                                       water_update, default_moby_methods},
        {0x340,                 update_target_outside_zone_timeout, default_moby_methods},
        {0x346,                               electric_gate_update, default_moby_methods},
        {0x349,                                    update_fog_zone, default_moby_methods},
        {0x34b,           update_triggered_moby_slide_and_lowering, default_moby_methods},
        {0x34c,                                 drain_cover_update, default_moby_methods},
        {0x34e,                        pipe_mounted_grating_update, default_moby_methods},
        {0x351,                                  suckcannon_update, default_moby_methods},
        {0x354,                       update_triggered_gate_motion, default_moby_methods},
        {0x355,                       update_triggered_gate_motion, default_moby_methods},
        {0x357,            update_triggered_particle_spray_emitter, default_moby_methods},
        {0x361,                              large_amoeboid_update, default_moby_methods},
        {0x362,                              large_amoeboid_update,       D_L05_00219400},
        {0x36d,                        elevator_with_button_update, default_moby_methods},
        {0x36f,                                ocean_rilgar_update, default_moby_methods},
        {0x37d, update_triggered_moby_motion_and_manipulator_angle, default_moby_methods},
        {0x37f,                          update_triggered_moby_yaw, default_moby_methods},
        {0x396,                             update_hoverboard_girl, default_moby_methods},
        {0x397,                                   FUN_L05_00317470, default_moby_methods},
        {0x398,                                qwark_rilgar_update, default_moby_methods},
        {0x39d,                              shady_salesman_update, default_moby_methods},
        {0x3d4,                                   blarg_gun_update, default_moby_methods},
        {0x3d5,                                   blarg_gun_update, default_moby_methods},
        {0x3d6,                    update_hero_clip_volume_outputs, default_moby_methods},
        {0x3d8,                                  television_update, default_moby_methods},
        {0x3e6,                                        taxi_update, default_moby_methods},
        {0x3ee,                             devestator_ammo_update, default_moby_methods},
        {0x424,                                     red_orb_update, default_moby_methods},
        {0x44b,                              qwarks_trailer_update, default_moby_methods},
        {0x46e,                                   gold_bolt_update, default_moby_methods},
        {0x472,                                   quad_moby_update, default_moby_methods},
        {0x473,                             hoverboard_ring_update, default_moby_methods},
        {0x474,                                 noop_main_callback, default_moby_methods},
        {0x477,                                  vendor_orb_update, default_moby_methods},
        {0x4a8,                               update_glow_particle, default_moby_methods},
        {0x4e3,                              hydrodisplacer_update, default_moby_methods},
        {0x509,                                 noop_main_callback, default_moby_methods},
        {0x50a,                               pilots_helmet_update, default_moby_methods},
        {0x543,                                   FUN_L05_0031bf40, default_moby_methods},
        {0x59e,                             devestator_ammo_update, default_moby_methods},
        {0x5a7,                             devestator_ammo_update, default_moby_methods},
        {0x5a9,                             devestator_ammo_update, default_moby_methods},
        {0x5e7,             update_moby_glow_and_contact_explosion, default_moby_methods},
        {0x5ea,                                 noop_main_callback, default_moby_methods},
        {0x5ed,                                   robot_arm_update, default_moby_methods},
        {0x5ee,                                   robot_arm_update, default_moby_methods},
        {0x60e,                       update_selected_moby_effects, default_moby_methods},
        {0x661,                        update_sandmouse_projectile, default_moby_methods},
        {0x6da,                                   robot_arm_update, default_moby_methods},
        {0x6db,                                   robot_arm_update, default_moby_methods},
        {0x6dc,                                   robot_arm_update, default_moby_methods},
        {0x6dd,                                   robot_arm_update, default_moby_methods},
        {0x6df,                                   robot_arm_update, default_moby_methods},
        {0x6e0,                                   robot_arm_update, default_moby_methods},
        {0x6fe,                                   robot_arm_update, default_moby_methods},
        {0x6ff,                                   robot_arm_update, default_moby_methods},
        {0x71a,                                   FUN_L01_0030df40, default_moby_methods},
        {0x72e,                                   robot_arm_update, default_moby_methods},
        {0x76c,                                       decoy_update, default_moby_methods},
        {   -1,                                                  0, default_moby_methods},
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
        { 1,                      update_box_positional_voice},
        { 2, update_sound_emitter_playback_at_random_position},
        { 3,         update_requested_state_from_box_position},
        { 5,                  update_listener_following_voice},
        { 6,                    update_music_box_exit_trigger},
        {-1,                                                0},
};
