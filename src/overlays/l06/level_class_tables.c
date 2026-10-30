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
void FUN_L06_002ea6a8();
void FUN_L06_00301070();
void FUN_L06_00302578();
void alien_queen_update() __asm__("FUN_L06_002f9a28");
void alien_snapper_update() __asm__("FUN_L06_002f7d78");
void blarg_space_commando_update() __asm__("FUN_L06_002fdbd0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void bot_door_lock_update() __asm__("FUN_L06_00307d30");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_moby_suction_and_save_state() __asm__("FUN_L06_002ea7c8");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void energy_sphere_holder_update() __asm__("FUN_L06_002f7288");
void energy_sphere_update() __asm__("FUN_L06_002f7ab8");
void enqueue_moby_draw_callbacks_by_clip_volume() __asm__("FUN_L06_002fd088");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void frog_update() __asm__("FUN_L06_002e8678");
void gadgebot_spawn_container_update() __asm__("FUN_L06_002d85a0");
void gadgebot_spawn_pad_update() __asm__("FUN_L06_002d8c20");
void gadgebot_update() __asm__("FUN_L06_002f0040");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void hangar_door_update() __asm__("FUN_L06_002b4770");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
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
void red_button_update() __asm__("FUN_L06_00304098");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void rocket_update() __asm__("FUN_L00_002cd1f0");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void toxic_gas_vent_update() __asm__("FUN_L06_002f3ad8");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void tresspasser_update() __asm__("FUN_L02_002ea210");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_beam_barrier() __asm__("FUN_L06_002f6470");
void update_beam_effect_lifecycle() __asm__("FUN_L00_002b4198");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_clip_volume_moby_activation_trigger() __asm__("FUN_L06_002fda30");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_drawbridge() __asm__("FUN_L06_002f55a0");
void update_drone() __asm__("FUN_L00_002ce358");
void update_falling_spinning_moby() __asm__("FUN_L06_00300b90");
void update_gated_moby_effects() __asm__("FUN_L06_00309348");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_hero_trigger_volume_monitor() __asm__("FUN_L06_002f7930");
void update_hero_triggered_moby_height_cycle() __asm__("FUN_L06_002fc640");
void update_hit_triggered_class_0681_spawner() __asm__("FUN_L06_00309fa8");
void update_hit_triggered_moby_breakup() __asm__("FUN_L06_00309860");
void update_hit_triggered_moby_replacement() __asm__("FUN_L06_00309e08");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_level06_help_hints() __asm__("FUN_L06_003083b0");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_moby_bobbing_and_motion() __asm__("FUN_L06_00300df0");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_moby_camera_sequence_and_draw_selection() __asm__("FUN_L06_002ffcb0");
void update_moby_grab_state() __asm__("FUN_L06_002ea5e0");
void update_moby_interpolated_point_light() __asm__("FUN_L01_0030b618");
void update_moby_linked_sequence_camera_and_timer() __asm__("FUN_L06_00301b90");
void update_moby_motion_collision_and_lifetime() __asm__("FUN_L01_002c7530");
void update_moby_pair_transforms() __asm__("FUN_L06_002fbfb0");
void update_moby_position_interpolation() __asm__("FUN_L06_002d9d10");
void update_moby_rotation_group() __asm__("FUN_L06_002f4f00");
void update_moby_vector_streams_and_particle_burst() __asm__("FUN_L06_003049f8");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_sandmouse_projectile() __asm__("FUN_L01_0030c9a8");
void update_spark_emitter() __asm__("FUN_L06_00308c68");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_timed_sprite_effect() __asm__("FUN_L06_0030be60");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L06_00202384")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L06_0020239C[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

void *D_L06_002023B4[6] __attribute__((section("lvl.vtbl"))) = {update_moby_grab_state,
                                                                FUN_L06_002ea6a8,
                                                                FUN_L06_002ea6a8,
                                                                cancel_moby_suction_and_save_state,
                                                                get_moby_data_plus_0x60,
                                                                mine_blend_color};

struct MobyClassUpdate moby_classes[171] __asm__("D_L06_00201B80")

    __attribute__((section("lvl.vtbl"))) = {
        {0x3ef,                           camera_object_update, default_moby_methods},
        {  0xb,                                  vendor_update, default_moby_methods},
        {  0xd,                                    bolt_update, default_moby_methods},
        {  0xe,                                    bolt_update, default_moby_methods},
        {  0xf,                                    bolt_update, default_moby_methods},
        { 0x10,                                    bolt_update, default_moby_methods},
        { 0x37,                             hangar_door_update, default_moby_methods},
        { 0x47,                                  wrench_update, default_moby_methods},
        { 0x4a,                                    mine_update, default_moby_methods},
        { 0x57,                             noop_main_callback, default_moby_methods},
        { 0x70,                           update_glow_particle, default_moby_methods},
        { 0x79,                                    bomb_update, default_moby_methods},
        { 0x7a,                                  debris_update, default_moby_methods},
        { 0x95,                 nanotech_crate_fragment_update, default_moby_methods},
        { 0x99,                 gold_devastator_missile_update, default_moby_methods},
        { 0x9c,      update_moby_motion_collision_and_lifetime, default_moby_methods},
        { 0x9d,                              devastator_update, default_moby_methods},
        { 0xa3,                                visibomb_update, default_moby_methods},
        { 0xa6,                      update_linked_moby_states, default_moby_methods},
        { 0xa7,                   update_beam_effect_lifecycle, default_moby_methods},
        { 0xa8,                                 blaster_update, default_moby_methods},
        { 0xac,                               FUN_L00_002b6140, default_moby_methods},
        { 0xaf,                                 taunter_update, default_moby_methods},
        { 0xb0,                               pyrocitor_update, default_moby_methods},
        { 0xb1,                              tesla_claw_update, default_moby_methods},
        { 0xb3,          update_linked_moby_scale_and_rotation, default_moby_methods},
        { 0xb4,                                walloper_update, default_moby_methods},
        { 0xb9,                               morphoray_update, default_moby_methods},
        { 0xba,                                doom_bot_update, default_moby_methods},
        { 0xbc,                              trespasser_update, default_moby_methods},
        { 0xbe,                              mine_glove_update, default_moby_methods},
        { 0xc0,                              bomb_glove_update, default_moby_methods},
        { 0xcb,                                   decoy_update, default_moby_methods},
        { 0xcc,                         devestator_ammo_update, default_moby_methods},
        { 0xd0,                               swingshot_update, default_moby_methods},
        { 0xd1,                          swingshot_hook_update, default_moby_methods},
        { 0xd5,                         devestator_ammo_update, default_moby_methods},
        { 0xd6,                         devestator_ammo_update, default_moby_methods},
        { 0xde,                         devestator_ammo_update, default_moby_methods},
        { 0xdf,                         devestator_ammo_update, default_moby_methods},
        { 0xe1,                         devestator_ammo_update, default_moby_methods},
        { 0xe2,                         devestator_ammo_update, default_moby_methods},
        { 0xe5,                           glove_of_doom_update, default_moby_methods},
        { 0xe6,                               FUN_L00_002c89e8, default_moby_methods},
        {0x10e,                       update_morphoray_chicken,       D_L06_0020239C},
        {0x12e,                gadgebot_spawn_container_update, default_moby_methods},
        {0x12f,                      gadgebot_spawn_pad_update, default_moby_methods},
        {0x131,                             update_homing_shot, default_moby_methods},
        {0x15c,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x15d,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x162,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x163,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x164,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x165,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x166,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x167,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x16b,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x16c,                 nanotech_crate_fragment_update, default_moby_methods},
        {0x16f,             update_moby_position_interpolation, default_moby_methods},
        {0x1ac,                     update_damped_falling_moby, default_moby_methods},
        {0x1b1,                             noop_main_callback, default_moby_methods},
        {0x1c6,                               FUN_L00_002cc608, default_moby_methods},
        {0x1c9,                                  rocket_update, default_moby_methods},
        {0x1df,                                   update_drone, default_moby_methods},
        {0x1e3,                                              0, default_moby_methods},
        {0x1f4,                                   crate_update, default_moby_methods},
        {0x1f5,                                   crate_update, default_moby_methods},
        {0x1f6,                                   crate_update, default_moby_methods},
        {0x1f9,                                   crate_update, default_moby_methods},
        {0x1ff,                                   crate_update, default_moby_methods},
        {0x232,                             decoy_glove_update, default_moby_methods},
        {0x249,                          metal_detector_update, default_moby_methods},
        {0x25c,                         sandmouse_house_update, default_moby_methods},
        {0x25d,             update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                                helipack_update, default_moby_methods},
        {0x267,                         tresspasser_pad_update, default_moby_methods},
        {0x26a,                             noop_main_callback, default_moby_methods},
        {0x27a,                                              0, default_moby_methods},
        {0x27f,               update_animated_point_light_moby, default_moby_methods},
        {0x2ee,                                 infobot_update, default_moby_methods},
        {0x307,                       grow_and_fade_out_update, default_moby_methods},
        {0x323,                   swingshot_versatarget_update, default_moby_methods},
        {0x324,      update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                             spawn_point_update, default_moby_methods},
        {0x326,                           nanotech_ball_update, default_moby_methods},
        {0x33b,                                    frog_update,       D_L06_002023B4},
        {0x340,             update_target_outside_zone_timeout, default_moby_methods},
        {0x351,                              suckcannon_update, default_moby_methods},
        {0x359,                                gadgebot_update, default_moby_methods},
        {0x38f,                          toxic_gas_vent_update, default_moby_methods},
        {0x3ee,                         devestator_ammo_update, default_moby_methods},
        {0x3f8,                             tresspasser_update, default_moby_methods},
        {0x3fd,                     update_moby_rotation_group, default_moby_methods},
        {0x404,                              update_drawbridge, default_moby_methods},
        {0x40b,                            update_beam_barrier, default_moby_methods},
        {0x40e,                    energy_sphere_holder_update, default_moby_methods},
        {0x40f,             update_hero_trigger_volume_monitor, default_moby_methods},
        {0x410,                           energy_sphere_update, default_moby_methods},
        {0x418,                           alien_snapper_update, default_moby_methods},
        {0x41b,                             alien_queen_update, default_moby_methods},
        {0x41e,                    update_moby_pair_transforms, default_moby_methods},
        {0x425,        update_hero_triggered_moby_height_cycle, default_moby_methods},
        {0x426,     enqueue_moby_draw_callbacks_by_clip_volume, default_moby_methods},
        {0x42a,     update_clip_volume_moby_activation_trigger, default_moby_methods},
        {0x42c,                    blarg_space_commando_update, default_moby_methods},
        {0x43b, update_moby_camera_sequence_and_draw_selection, default_moby_methods},
        {0x43d,                   update_falling_spinning_moby, default_moby_methods},
        {0x43e,                   update_falling_spinning_moby, default_moby_methods},
        {0x43f,                   update_falling_spinning_moby, default_moby_methods},
        {0x440,                   update_falling_spinning_moby, default_moby_methods},
        {0x441,                   update_falling_spinning_moby, default_moby_methods},
        {0x443,                 update_moby_bobbing_and_motion, default_moby_methods},
        {0x444,                 update_moby_bobbing_and_motion, default_moby_methods},
        {0x445,                 update_moby_bobbing_and_motion, default_moby_methods},
        {0x446,                 update_moby_bobbing_and_motion, default_moby_methods},
        {0x447,                 update_moby_bobbing_and_motion, default_moby_methods},
        {0x448,                 update_moby_bobbing_and_motion, default_moby_methods},
        {0x449,                 update_moby_bobbing_and_motion, default_moby_methods},
        {0x44a,                 update_moby_bobbing_and_motion, default_moby_methods},
        {0x44f,                 update_moby_bobbing_and_motion, default_moby_methods},
        {0x451,                               FUN_L06_00301070, default_moby_methods},
        {0x454,   update_moby_linked_sequence_camera_and_timer, default_moby_methods},
        {0x455,                               FUN_L06_00302578, default_moby_methods},
        {0x45e,                              red_button_update, default_moby_methods},
        {0x463,  update_moby_vector_streams_and_particle_burst, default_moby_methods},
        {0x46e,                               gold_bolt_update, default_moby_methods},
        {0x472,                               quad_moby_update, default_moby_methods},
        {0x477,                              vendor_orb_update, default_moby_methods},
        {0x4a8,                           update_glow_particle, default_moby_methods},
        {0x4e3,                          hydrodisplacer_update, default_moby_methods},
        {0x509,                             noop_main_callback, default_moby_methods},
        {0x50a,                           pilots_helmet_update, default_moby_methods},
        {0x516,                           bot_door_lock_update, default_moby_methods},
        {0x544,                      update_level06_help_hints, default_moby_methods},
        {0x59e,                         devestator_ammo_update, default_moby_methods},
        {0x5a7,                         devestator_ammo_update, default_moby_methods},
        {0x5a9,                         devestator_ammo_update, default_moby_methods},
        {0x5e0,           update_moby_interpolated_point_light, default_moby_methods},
        {0x5e8,                           update_spark_emitter, default_moby_methods},
        {0x60f,                      update_gated_moby_effects, default_moby_methods},
        {0x661,                    update_sandmouse_projectile, default_moby_methods},
        {0x677,              update_hit_triggered_moby_breakup, default_moby_methods},
        {0x678,                               robot_arm_update, default_moby_methods},
        {0x679,                               robot_arm_update, default_moby_methods},
        {0x67a,                               robot_arm_update, default_moby_methods},
        {0x67b,                               robot_arm_update, default_moby_methods},
        {0x67c,          update_hit_triggered_moby_replacement, default_moby_methods},
        {0x67d,                               robot_arm_update, default_moby_methods},
        {0x67e,                               robot_arm_update, default_moby_methods},
        {0x680,        update_hit_triggered_class_0681_spawner, default_moby_methods},
        {0x681,                               robot_arm_update, default_moby_methods},
        {0x682,                               robot_arm_update, default_moby_methods},
        {0x688,                             noop_main_callback, default_moby_methods},
        {0x689,                               robot_arm_update, default_moby_methods},
        {0x68a,                               robot_arm_update, default_moby_methods},
        {0x68b,                               robot_arm_update, default_moby_methods},
        {0x68c,                               robot_arm_update, default_moby_methods},
        {0x69f,                               robot_arm_update, default_moby_methods},
        {0x6a0,                               robot_arm_update, default_moby_methods},
        {0x6a1,                               robot_arm_update, default_moby_methods},
        {0x6a2,                               robot_arm_update, default_moby_methods},
        {0x6a3,                               robot_arm_update, default_moby_methods},
        {0x6a4,                               robot_arm_update, default_moby_methods},
        {0x6a5,                               robot_arm_update, default_moby_methods},
        {0x6a6,                               robot_arm_update, default_moby_methods},
        {0x6a7,                               robot_arm_update, default_moby_methods},
        {0x6a8,                               robot_arm_update, default_moby_methods},
        {0x71a,                               FUN_L01_0030df40, default_moby_methods},
        {0x76a,                     update_timed_sprite_effect, default_moby_methods},
        {0x76c,                                   decoy_update, default_moby_methods},
        {   -1,                                              0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L06_00202400") __attribute__((section("lvl.camvtbl"))) = {
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
    {17,           FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {18, update_moby_camera_control,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,        volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                          0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[1] __asm__("D_L06_00202500")

    __attribute__((section("lvl.sndvtbl"))) = {
        {-1, 0},
};
