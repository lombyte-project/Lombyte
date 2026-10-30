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
void FUN_L01_002b9eb0();
void FUN_L01_0030df40();
void FUN_L03_00292e98();
void FUN_L03_00294c08();
void FUN_L03_002bef68();
void FUN_L03_002c4718();
void FUN_L03_002c5bb8();
void FUN_L03_002cbea8();
void FUN_L03_002d3198();
void FUN_L03_002da870();
void FUN_L03_002db558();
void FUN_L03_002dbd90();
void FUN_L03_002dca30();
void FUN_L03_002e01d0();
void FUN_L03_002eb978();
void FUN_L03_002ebd70();
void blarg_helicopter_update() __asm__("FUN_L03_002cca00");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
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
void evaluate_clip_volume_candidate() __asm__("FUN_L03_002e8870");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void fred_1_update() __asm__("FUN_L03_002dc310");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void gunship_follow_path() __asm__("FUN_L01_002f5168");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void lightpost_update() __asm__("FUN_L03_002e2948");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void metropolis_train_carriage_update() __asm__("FUN_L03_002953f8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_layer_update() __asm__("FUN_L03_002c9eb8");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void qwark_rilgar_update() __asm__("FUN_L03_002dc478");
void red_ship_1_update() __asm__("FUN_L03_0029dba8");
void robo_shack_sign_update() __asm__("FUN_L03_002d3e58");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void rocket_update() __asm__("FUN_L00_002cd1f0");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void set_transform_state_three_when_outside_clip_volume() __asm__("FUN_L03_002e8980");
void spawn_point_update() __asm__("FUN_L01_00300220");
void statue_update() __asm__("FUN_L03_002d3cd8");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void trespasser_update() __asm__("FUN_L00_002c1718");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_beam_effect_lifecycle() __asm__("FUN_L00_002b4198");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation() __asm__("FUN_L03_002ebdb0");
void update_camera_state_outside_clip_volume() __asm__("FUN_L03_002ebde8");
void update_cutscene_trigger() __asm__("FUN_L01_002fb5b0");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_drone() __asm__("FUN_L00_002ce358");
void update_falling_moby() __asm__("FUN_L03_002de370");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_homing_rocket() __asm__("FUN_L03_002d43c8");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_homing_spark_projectile() __asm__("FUN_L03_002d47c0");
void update_kerwan_path_enemy() __asm__("FUN_L03_002c6fd0");
void update_kerwan_tutorial_hints() __asm__("FUN_L03_002df520");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_moby_draw_callback() __asm__("FUN_L03_002dc9b0");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_box_exit_trigger() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_parent_following_effect() __asm__("FUN_L03_002bae48");
void update_path_mover() __asm__("FUN_L03_002db020");
void update_path_mover_spawner() __asm__("FUN_L03_002db280");
void update_rail_lift() __asm__("FUN_L03_002dccc0");
void update_requested_state_from_box_position() __asm__("FUN_L01_00319f18");
void update_sandmouse_projectile() __asm__("FUN_L01_0030c9a8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L03_001E40FC")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L03_001E4114[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[181] __asm__("D_L03_001E3880")

    __attribute__((section("lvl.vtbl"))) = {
        {0x2d6,                          FUN_L01_002b9eb0, default_moby_methods},
        {0x336,                          FUN_L03_00292e98, default_moby_methods},
        {0x364,                          FUN_L03_00294c08, default_moby_methods},
        {0x3ef,                      camera_object_update, default_moby_methods},
        {0x4ba,          metropolis_train_carriage_update, default_moby_methods},
        {  0xb,                             vendor_update, default_moby_methods},
        {  0xd,                               bolt_update, default_moby_methods},
        {  0xe,                               bolt_update, default_moby_methods},
        {  0xf,                               bolt_update, default_moby_methods},
        { 0x10,                               bolt_update, default_moby_methods},
        { 0x47,                             wrench_update, default_moby_methods},
        { 0x4a,                               mine_update, default_moby_methods},
        { 0x4b,                         red_ship_1_update, default_moby_methods},
        { 0x70,                      update_glow_particle, default_moby_methods},
        { 0x73,                         red_ship_1_update, default_moby_methods},
        { 0x74,                         red_ship_1_update, default_moby_methods},
        { 0x75,                         red_ship_1_update, default_moby_methods},
        { 0x76,                         red_ship_1_update, default_moby_methods},
        { 0x77,                         red_ship_1_update, default_moby_methods},
        { 0x78,                         red_ship_1_update, default_moby_methods},
        { 0x79,                               bomb_update, default_moby_methods},
        { 0x7a,                             debris_update, default_moby_methods},
        { 0x84,                         red_ship_1_update, default_moby_methods},
        { 0x95,            nanotech_crate_fragment_update, default_moby_methods},
        { 0x99,            gold_devastator_missile_update, default_moby_methods},
        { 0x9d,                         devastator_update, default_moby_methods},
        { 0xa3,                           visibomb_update, default_moby_methods},
        { 0xa6,                 update_linked_moby_states, default_moby_methods},
        { 0xa7,              update_beam_effect_lifecycle, default_moby_methods},
        { 0xa8,                            blaster_update, default_moby_methods},
        { 0xac,                          FUN_L00_002b6140, default_moby_methods},
        { 0xaf,                            taunter_update, default_moby_methods},
        { 0xb0,                          pyrocitor_update, default_moby_methods},
        { 0xb1,                         tesla_claw_update, default_moby_methods},
        { 0xb3,     update_linked_moby_scale_and_rotation, default_moby_methods},
        { 0xb4,                           walloper_update, default_moby_methods},
        { 0xb9,                          morphoray_update, default_moby_methods},
        { 0xba,                           doom_bot_update, default_moby_methods},
        { 0xbc,                         trespasser_update, default_moby_methods},
        { 0xbe,                         mine_glove_update, default_moby_methods},
        { 0xc0,                         bomb_glove_update, default_moby_methods},
        { 0xcb,                              decoy_update, default_moby_methods},
        { 0xcc,                    devestator_ammo_update, default_moby_methods},
        { 0xd0,                          swingshot_update, default_moby_methods},
        { 0xd1,                     swingshot_hook_update, default_moby_methods},
        { 0xd5,                    devestator_ammo_update, default_moby_methods},
        { 0xd6,                    devestator_ammo_update, default_moby_methods},
        { 0xde,                    devestator_ammo_update, default_moby_methods},
        { 0xdf,                    devestator_ammo_update, default_moby_methods},
        { 0xe1,                    devestator_ammo_update, default_moby_methods},
        { 0xe2,                    devestator_ammo_update, default_moby_methods},
        { 0xe5,                      glove_of_doom_update, default_moby_methods},
        { 0xe6,                          FUN_L00_002c89e8, default_moby_methods},
        { 0xeb,            update_parent_following_effect, default_moby_methods},
        {0x10e,                  update_morphoray_chicken,       D_L03_001E4114},
        {0x131,                        update_homing_shot, default_moby_methods},
        {0x15c,            nanotech_crate_fragment_update, default_moby_methods},
        {0x15d,            nanotech_crate_fragment_update, default_moby_methods},
        {0x162,            nanotech_crate_fragment_update, default_moby_methods},
        {0x163,            nanotech_crate_fragment_update, default_moby_methods},
        {0x164,            nanotech_crate_fragment_update, default_moby_methods},
        {0x165,            nanotech_crate_fragment_update, default_moby_methods},
        {0x166,            nanotech_crate_fragment_update, default_moby_methods},
        {0x167,            nanotech_crate_fragment_update, default_moby_methods},
        {0x16b,            nanotech_crate_fragment_update, default_moby_methods},
        {0x16c,            nanotech_crate_fragment_update, default_moby_methods},
        {0x1ac,                update_damped_falling_moby, default_moby_methods},
        {0x1b1,                        noop_main_callback, default_moby_methods},
        {0x1c6,                          FUN_L00_002cc608, default_moby_methods},
        {0x1c7,                          FUN_L03_002bef68, default_moby_methods},
        {0x1c9,                             rocket_update, default_moby_methods},
        {0x1df,                              update_drone, default_moby_methods},
        {0x1e3,                                         0, default_moby_methods},
        {0x1f4,                              crate_update, default_moby_methods},
        {0x1f5,                              crate_update, default_moby_methods},
        {0x1f9,                              crate_update, default_moby_methods},
        {0x1ff,                              crate_update, default_moby_methods},
        {0x221,                          FUN_L03_002c4718, default_moby_methods},
        {0x232,                        decoy_glove_update, default_moby_methods},
        {0x23d,                          FUN_L03_002c5bb8, default_moby_methods},
        {0x23e,                  update_kerwan_path_enemy, default_moby_methods},
        {0x242,                         mine_layer_update, default_moby_methods},
        {0x249,                     metal_detector_update, default_moby_methods},
        {0x25c,                    sandmouse_house_update, default_moby_methods},
        {0x25d,        update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                           helipack_update, default_moby_methods},
        {0x26a,                        noop_main_callback, default_moby_methods},
        {0x273,                          FUN_L03_002cbea8, default_moby_methods},
        {0x277,                   blarg_helicopter_update, default_moby_methods},
        {0x27a,                                         0, default_moby_methods},
        {0x27f,          update_animated_point_light_moby, default_moby_methods},
        {0x2e1,                   update_cutscene_trigger, default_moby_methods},
        {0x2ee,                            infobot_update, default_moby_methods},
        {0x2f6,              swingshot_versatarget_update, default_moby_methods},
        {0x307,                  grow_and_fade_out_update, default_moby_methods},
        {0x31b,                         red_ship_1_update, default_moby_methods},
        {0x323,              swingshot_versatarget_update, default_moby_methods},
        {0x324, update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                        spawn_point_update, default_moby_methods},
        {0x326,                      nanotech_ball_update, default_moby_methods},
        {0x330,                          FUN_L03_002d3198, default_moby_methods},
        {0x331,                             statue_update, default_moby_methods},
        {0x339,                    robo_shack_sign_update, default_moby_methods},
        {0x340,        update_target_outside_zone_timeout, default_moby_methods},
        {0x341,                      update_homing_rocket, default_moby_methods},
        {0x34d,                       gunship_follow_path, default_moby_methods},
        {0x350,            update_homing_spark_projectile, default_moby_methods},
        {0x351,                         suckcannon_update, default_moby_methods},
        {0x37a,                          FUN_L03_002da870, default_moby_methods},
        {0x382,                         update_path_mover, default_moby_methods},
        {0x383,                 update_path_mover_spawner, default_moby_methods},
        {0x389,                          FUN_L03_00294c08, default_moby_methods},
        {0x38d,                          FUN_L03_002db558, default_moby_methods},
        {0x392,                          FUN_L03_002dbd90, default_moby_methods},
        {0x393,                             fred_1_update, default_moby_methods},
        {0x394,                             fred_1_update, default_moby_methods},
        {0x395,                             fred_1_update, default_moby_methods},
        {0x398,                       qwark_rilgar_update, default_moby_methods},
        {0x3a0,                          FUN_L03_00294c08, default_moby_methods},
        {0x3e2,                 update_moby_draw_callback, default_moby_methods},
        {0x3e5,                          FUN_L03_002dca30, default_moby_methods},
        {0x3ee,                    devestator_ammo_update, default_moby_methods},
        {0x3f4,                          update_rail_lift, default_moby_methods},
        {0x46e,                          gold_bolt_update, default_moby_methods},
        {0x472,                          quad_moby_update, default_moby_methods},
        {0x477,                         vendor_orb_update, default_moby_methods},
        {0x4a8,                      update_glow_particle, default_moby_methods},
        {0x4c5,                       update_falling_moby, default_moby_methods},
        {0x4c6,                       update_falling_moby, default_moby_methods},
        {0x4e3,                     hydrodisplacer_update, default_moby_methods},
        {0x509,                        noop_main_callback, default_moby_methods},
        {0x50a,                      pilots_helmet_update, default_moby_methods},
        {0x53e,              update_kerwan_tutorial_hints, default_moby_methods},
        {0x59e,                    devestator_ammo_update, default_moby_methods},
        {0x5a7,                    devestator_ammo_update, default_moby_methods},
        {0x5a9,                    devestator_ammo_update, default_moby_methods},
        {0x60c,                          FUN_L03_002e01d0, default_moby_methods},
        {0x637,                          robot_arm_update, default_moby_methods},
        {0x638,                          robot_arm_update, default_moby_methods},
        {0x639,                          robot_arm_update, default_moby_methods},
        {0x661,               update_sandmouse_projectile, default_moby_methods},
        {0x6a9,                          robot_arm_update, default_moby_methods},
        {0x6aa,                          robot_arm_update, default_moby_methods},
        {0x6ab,                          robot_arm_update, default_moby_methods},
        {0x6ac,                          robot_arm_update, default_moby_methods},
        {0x6ad,                          robot_arm_update, default_moby_methods},
        {0x6ae,                          robot_arm_update, default_moby_methods},
        {0x6af,                          robot_arm_update, default_moby_methods},
        {0x6b0,                          robot_arm_update, default_moby_methods},
        {0x6b1,                          robot_arm_update, default_moby_methods},
        {0x6b2,                          robot_arm_update, default_moby_methods},
        {0x6b3,                          robot_arm_update, default_moby_methods},
        {0x6b4,                          robot_arm_update, default_moby_methods},
        {0x6b5,                          robot_arm_update, default_moby_methods},
        {0x6b6,                          robot_arm_update, default_moby_methods},
        {0x6b7,                          robot_arm_update, default_moby_methods},
        {0x6b8,                          robot_arm_update, default_moby_methods},
        {0x6b9,                          robot_arm_update, default_moby_methods},
        {0x6ba,                          robot_arm_update, default_moby_methods},
        {0x6bc,                          robot_arm_update, default_moby_methods},
        {0x6be,                          robot_arm_update, default_moby_methods},
        {0x6bf,                          robot_arm_update, default_moby_methods},
        {0x6c5,                          robot_arm_update, default_moby_methods},
        {0x6c6,                          robot_arm_update, default_moby_methods},
        {0x6c7,                          robot_arm_update, default_moby_methods},
        {0x6ce,                          robot_arm_update, default_moby_methods},
        {0x6cf,                          robot_arm_update, default_moby_methods},
        {0x6d0,                          robot_arm_update, default_moby_methods},
        {0x6d1,                          robot_arm_update, default_moby_methods},
        {0x6d2,                          robot_arm_update, default_moby_methods},
        {0x71a,                          FUN_L01_0030df40, default_moby_methods},
        {0x723,                          lightpost_update, default_moby_methods},
        {0x724,                        noop_main_callback, default_moby_methods},
        {0x725,                          robot_arm_update, default_moby_methods},
        {0x726,                          robot_arm_update, default_moby_methods},
        {0x727,                          robot_arm_update, default_moby_methods},
        {0x728,                          robot_arm_update, default_moby_methods},
        {0x729,                          robot_arm_update, default_moby_methods},
        {0x76c,                              decoy_update, default_moby_methods},
        {0x77d,                          robot_arm_update, default_moby_methods},
        {   -1,                                         0, default_moby_methods},
};

struct CameraType level_camera_types[10] __asm__("D_L03_001E4180") __attribute__((section("lvl.camvtbl"))) = {
    { 0,    noop_callback_l,               follow_camera_setup,follow_camera_update,                                 noop_main_callback                                                                       },
    { 1,
     evaluate_clip_volume_candidate,                  mine_blend_color,
     mine_blend_color, set_transform_state_three_when_outside_clip_volume                                                                       },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input,        select_camera_mode_by_hero_vector_alignment                                                  },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                                 noop_main_callback                                             },
    { 6,            noop_callback_l, initialize_orbit_camera_from_hero,     mine_blend_color,                                 noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,               conditionally_set_camera_state_three                                                          },
    {14,
     FUN_L03_002ebd70,                  FUN_L03_002eb978,
     update_camera_position_and_orientation,            update_camera_state_outside_clip_volume                                                 },
    {17,           FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {18, update_moby_camera_control,                noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {-1,                          0,                                 0,                    0,                                                  0},
};

struct SoundHandler sound_handlers[4] __asm__("D_L03_001E4280")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 3,  update_requested_state_from_box_position},
        { 6,             update_music_box_exit_trigger},
        {-1,                                         0},
};
