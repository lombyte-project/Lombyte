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
void FUN_L01_002f8718();
void FUN_L07_002cd2b0();
void FUN_L07_0030f5f0();
void FUN_L07_00319040();
void FUN_L07_0031d2e0();
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
void seashell_update() __asm__("FUN_L07_0031f780");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
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
void update_drone() __asm__("FUN_L00_002ce358");
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
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_umbris_scene_progression() __asm__("FUN_L07_002f5ba0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
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
        { 0x26,                          FUN_L07_002cd2b0, default_moby_methods},
        {0x3ef,                      camera_object_update, default_moby_methods},
        {0x5c2,        update_mission_gated_taxi_platform, default_moby_methods},
        {  0xb,                             vendor_update, default_moby_methods},
        {  0xd,                               bolt_update, default_moby_methods},
        {  0xe,                               bolt_update, default_moby_methods},
        {  0xf,                               bolt_update, default_moby_methods},
        { 0x10,                               bolt_update, default_moby_methods},
        { 0x47,                             wrench_update, default_moby_methods},
        { 0x4a,                               mine_update, default_moby_methods},
        { 0x70,                      update_glow_particle, default_moby_methods},
        { 0x79,                               bomb_update, default_moby_methods},
        { 0x7a,                             debris_update, default_moby_methods},
        { 0x95,            nanotech_crate_fragment_update, default_moby_methods},
        { 0x99,            gold_devastator_missile_update, default_moby_methods},
        { 0x9d,                         devastator_update, default_moby_methods},
        { 0xa3,                           visibomb_update, default_moby_methods},
        { 0xa6,                 update_linked_moby_states, default_moby_methods},
        { 0xa7,                update_thruster_pack_flame, default_moby_methods},
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
        {0x102,               update_moby_spatial_trigger, default_moby_methods},
        {0x10e,                  update_morphoray_chicken,       D_L07_002125DC},
        {0x131,                        update_homing_shot, default_moby_methods},
        {0x155,              hydrodisplacer_faucet_update, default_moby_methods},
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
        {0x1b4,           update_umbris_scene_progression, default_moby_methods},
        {0x1bf,                       qwark_rilgar_update, default_moby_methods},
        {0x1c6,                          FUN_L00_002cc608, default_moby_methods},
        {0x1c9,                        update_ryno_rocket, default_moby_methods},
        {0x1cc,             update_moby_draw_registration, default_moby_methods},
        {0x1df,                              update_drone, default_moby_methods},
        {0x1e3,                                         0, default_moby_methods},
        {0x1f4,                              crate_update, default_moby_methods},
        {0x1f5,                              crate_update, default_moby_methods},
        {0x1f6,                              crate_update, default_moby_methods},
        {0x1f9,                              crate_update, default_moby_methods},
        {0x1ff,                              crate_update, default_moby_methods},
        {0x211,                        purple_ship_update, default_moby_methods},
        {0x214,                        noop_main_callback, default_moby_methods},
        {0x232,                        decoy_glove_update, default_moby_methods},
        {0x249,                     metal_detector_update, default_moby_methods},
        {0x25d,        update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                           helipack_update, default_moby_methods},
        {0x26a,                        noop_main_callback, default_moby_methods},
        {0x277,                   blarg_helicopter_update, default_moby_methods},
        {0x27a,                                         0, default_moby_methods},
        {0x27f,          update_animated_point_light_moby, default_moby_methods},
        {0x2b8,                          FUN_L01_002f8718, default_moby_methods},
        {0x2b9,                          FUN_L01_002f8718, default_moby_methods},
        {0x2ba,                          FUN_L01_002f8718, default_moby_methods},
        {0x2e1,                   update_cutscene_trigger, default_moby_methods},
        {0x2ee,                            infobot_update, default_moby_methods},
        {0x2f6,              swingshot_versatarget_update, default_moby_methods},
        {0x307,                  grow_and_fade_out_update, default_moby_methods},
        {0x323,              swingshot_versatarget_update, default_moby_methods},
        {0x324, update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                        spawn_point_update, default_moby_methods},
        {0x326,                      nanotech_ball_update, default_moby_methods},
        {0x337,                        update_moby_wobble, default_moby_methods},
        {0x340,        update_target_outside_zone_timeout, default_moby_methods},
        {0x350,            update_homing_spark_projectile, default_moby_methods},
        {0x351,                         suckcannon_update, default_moby_methods},
        {0x367,                         update_mine_group, default_moby_methods},
        {0x370,       update_machinegun_turret_projectile, default_moby_methods},
        {0x372,                               update_bomb, default_moby_methods},
        {0x376,              timed_activation_pads_update, default_moby_methods},
        {0x386,                              water_update, default_moby_methods},
        {0x398,                       qwark_rilgar_update, default_moby_methods},
        {0x3ad,                              water_update, default_moby_methods},
        {0x3af,                              water_update, default_moby_methods},
        {0x3b0,                              water_update, default_moby_methods},
        {0x3b4,                              water_update, default_moby_methods},
        {0x3b5,                              water_update, default_moby_methods},
        {0x3b6,                              water_update, default_moby_methods},
        {0x3b7,                              water_update, default_moby_methods},
        {0x3b8,                              water_update, default_moby_methods},
        {0x3b9,                              water_update, default_moby_methods},
        {0x3ba,                              water_update, default_moby_methods},
        {0x3bb,                              water_update, default_moby_methods},
        {0x3be,                              water_update, default_moby_methods},
        {0x3c1,                              water_update, default_moby_methods},
        {0x3c2,                              water_update, default_moby_methods},
        {0x3c7,                              water_update, default_moby_methods},
        {0x3c8,                              water_update, default_moby_methods},
        {0x3c9,                              water_update, default_moby_methods},
        {0x3ca,                              water_update, default_moby_methods},
        {0x3cb,                              water_update, default_moby_methods},
        {0x3ee,                    devestator_ammo_update, default_moby_methods},
        {0x3f5,      update_linked_state_sliding_platform, default_moby_methods},
        {0x3f6,      update_linked_state_sliding_platform, default_moby_methods},
        {0x3fa,                       ocean_umbris_update, default_moby_methods},
        {0x411,                         bombomatic_update, default_moby_methods},
        {0x416,                 update_fading_ring_effect, default_moby_methods},
        {0x419,             update_growing_ballistic_moby, default_moby_methods},
        {0x423,                          FUN_L07_0030f5f0, default_moby_methods},
        {0x427,                               door_update, default_moby_methods},
        {0x428,      update_linked_state_sliding_platform, default_moby_methods},
        {0x429,      update_linked_state_sliding_platform, default_moby_methods},
        {0x42d, update_command_triggered_falling_platform, default_moby_methods},
        {0x436,                               door_update, default_moby_methods},
        {0x437,                               door_update, default_moby_methods},
        {0x438,    update_hero_triggered_falling_platform, default_moby_methods},
        {0x450,                        noop_main_callback, default_moby_methods},
        {0x452,                       snagglebeast_update, default_moby_methods},
        {0x456,                          FUN_L07_00319040, default_moby_methods},
        {0x458,                               update_mine, default_moby_methods},
        {0x459,                     update_breakable_prop, default_moby_methods},
        {0x45a,                     update_breakable_prop, default_moby_methods},
        {0x45b,                     update_breakable_prop, default_moby_methods},
        {0x45c,                     update_breakable_prop, default_moby_methods},
        {0x466,                  machinegun_turret_update, default_moby_methods},
        {0x468,                       update_sliding_gate, default_moby_methods},
        {0x469,                             bridge_update, default_moby_methods},
        {0x46b,                               door_update, default_moby_methods},
        {0x46d,                         update_swing_gate, default_moby_methods},
        {0x46e,                          gold_bolt_update, default_moby_methods},
        {0x46f,                         teleporter_update, default_moby_methods},
        {0x472,                          quad_moby_update, default_moby_methods},
        {0x476,                          FUN_L07_0031d2e0, default_moby_methods},
        {0x477,                         vendor_orb_update, default_moby_methods},
        {0x4a8,                      update_glow_particle, default_moby_methods},
        {0x4c5,                       update_falling_moby, default_moby_methods},
        {0x4c6,                       update_falling_moby, default_moby_methods},
        {0x4e3,                     hydrodisplacer_update, default_moby_methods},
        {0x509,                        noop_main_callback, default_moby_methods},
        {0x50a,                      pilots_helmet_update, default_moby_methods},
        {0x59e,                    devestator_ammo_update, default_moby_methods},
        {0x5a7,                    devestator_ammo_update, default_moby_methods},
        {0x5a9,                    devestator_ammo_update, default_moby_methods},
        {0x610,                   update_cutscene_effects, default_moby_methods},
        {0x61c,                        pterodactyl_update, default_moby_methods},
        {0x621,                          robot_arm_update, default_moby_methods},
        {0x622,                          robot_arm_update, default_moby_methods},
        {0x637,                          robot_arm_update, default_moby_methods},
        {0x638,                          robot_arm_update, default_moby_methods},
        {0x639,                          robot_arm_update, default_moby_methods},
        {0x672,                           mushroom_update, default_moby_methods},
        {0x673,                        noop_main_callback, default_moby_methods},
        {0x674,                          robot_arm_update, default_moby_methods},
        {0x675,                          robot_arm_update, default_moby_methods},
        {0x676,                          robot_arm_update, default_moby_methods},
        {0x67f,                         robot_head_update, default_moby_methods},
        {0x68d,                         robot_head_update, default_moby_methods},
        {0x68e,                          robot_arm_update, default_moby_methods},
        {0x68f,                          robot_arm_update, default_moby_methods},
        {0x690,                          robot_arm_update, default_moby_methods},
        {0x696,                           seashell_update, default_moby_methods},
        {0x698,                          robot_arm_update, default_moby_methods},
        {0x699,                          robot_arm_update, default_moby_methods},
        {0x69a,                          robot_arm_update, default_moby_methods},
        {0x6fd,              update_cutscene_joint_trails, default_moby_methods},
        {0x76c,                              decoy_update, default_moby_methods},
        {   -1,                                         0, default_moby_methods},
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
