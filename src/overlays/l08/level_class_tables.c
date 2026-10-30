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
void FUN_L01_002e0c68();
void FUN_L01_0030df40();
void FUN_L03_002bef68();
void FUN_L03_002c4718();
void FUN_L08_002dbdb0();
void batalia_deserter_update() __asm__("FUN_L08_00305270");
void blarg_bombthrower_update() __asm__("FUN_L08_002dabf0");
void blarg_gun_update() __asm__("FUN_L05_003186f8");
void blarg_paratrooper_update() __asm__("FUN_L08_00301158");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_thief_update() __asm__("FUN_L08_002e2df0");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void commando_update() __asm__("FUN_L08_00302ce8");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void enter_scoutbot_return_home_state() __asm__("FUN_L08_002d4050");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_camera_transform_and_paths() __asm__("FUN_L01_00315de0");
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
void red_plant_update() __asm__("FUN_L00_002e3ca8");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void scatter_ground_particles() __asm__("FUN_L08_00307cf0");
void scoutbot_update() __asm__("FUN_L08_002d2af0");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void shrub_update() __asm__("FUN_L08_002e8cd0");
void spawn_moby_radial_particles() __asm__("FUN_L08_003085f0");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void the_plumber_batalia_update() __asm__("FUN_L08_003065d8");
void the_plumber_update() __asm__("FUN_L01_002ff118");
void trackdrive_left_update() __asm__("FUN_L02_002cb978");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_batalia_cannon() __asm__("FUN_L08_002e0328");
void update_batalia_textured_effect() __asm__("FUN_L08_003097b0");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_bolt_pickup() __asm__("FUN_L08_002b82a0");
void update_bomb() __asm__("FUN_L08_002d23f0");
void update_bomber_tank() __asm__("FUN_L08_002d42d8");
void update_bomber_tank_bomb() __asm__("FUN_L08_002dc9a8");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");
void update_cutscene_camera_target() __asm__("FUN_L08_003084d8");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_drone() __asm__("FUN_L00_002ce358");
void update_effect_emitter() __asm__("FUN_L08_00308c90");
void update_falling_debris() __asm__("FUN_L08_002e16c0");
void update_fire_bolt() __asm__("FUN_L08_00307298");
void update_flickering_mesh_effect() __asm__("FUN_L08_002f70a0");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_ground_height_modulated_voice() __asm__("FUN_L08_00316fa8");
void update_gunship() __asm__("FUN_L08_002e24e8");
void update_heavy_gunship() __asm__("FUN_L08_002e9b70");
void update_help_hints() __asm__("FUN_L08_00307540");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_hero_trigger_volume_monitor() __asm__("FUN_L06_002f7930");
void update_turret_rocket() __asm__("FUN_L08_002e8210");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_landmine() __asm__("FUN_L08_002f44c0");
void update_layered_mesh_effect() __asm__("FUN_L08_002f1548");
void update_lift() __asm__("FUN_L08_002ea398");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_smoke_trail_projectile() __asm__("FUN_L01_002eda00");
void update_billboard_projectile() __asm__("FUN_L01_002c7530");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_path_following_moby() __asm__("FUN_L08_002de848");
void update_rail_hero_motion() __asm__("FUN_L01_002f6328");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_rocket() __asm__("FUN_L08_002dd3c0");
void update_sandmouse_projectile() __asm__("FUN_L01_0030c9a8");
void update_shattering_prop() __asm__("FUN_L08_0030bc00");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_directional_spark_emitter() __asm__("FUN_L08_002f5830");
void update_swing_gate() __asm__("FUN_L08_002ea4c0");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_water_surface() __asm__("FUN_L08_002da0f0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L08_001FD1E8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L08_001FD200[6] __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                enter_scoutbot_return_home_state,
                                                                get_moby_data_plus_0x60,
                                                                mine_blend_color};

void *D_L08_001FD218[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[190] __asm__("D_L08_001FC900")

    __attribute__((section("lvl.vtbl"))) = {
        {0x3ef,                      camera_object_update, default_moby_methods},
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
        { 0x98,                        update_bolt_pickup, default_moby_methods},
        { 0x99,            gold_devastator_missile_update, default_moby_methods},
        { 0x9c,               update_billboard_projectile, default_moby_methods},
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
        { 0xf8,                               update_bomb, default_moby_methods},
        { 0xfc,                           scoutbot_update,       D_L08_001FD200},
        { 0xfd,                        update_bomber_tank, default_moby_methods},
        {0x10e,                  update_morphoray_chicken,       D_L08_001FD218},
        {0x118,                          FUN_L01_002e0c68, default_moby_methods},
        {0x131,                        update_homing_shot, default_moby_methods},
        {0x144,                        noop_main_callback, default_moby_methods},
        {0x147,                      update_water_surface, default_moby_methods},
        {0x14b,                    trackdrive_left_update, default_moby_methods},
        {0x14d,                  blarg_bombthrower_update, default_moby_methods},
        {0x158,                          blarg_gun_update, default_moby_methods},
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
        {0x1a8,                          FUN_L08_002dbdb0, default_moby_methods},
        {0x1a9,                   update_bomber_tank_bomb, default_moby_methods},
        {0x1ac,                update_damped_falling_moby, default_moby_methods},
        {0x1b1,                        noop_main_callback, default_moby_methods},
        {0x1b3,                             update_rocket, default_moby_methods},
        {0x1b6,                update_path_following_moby, default_moby_methods},
        {0x1b8,                     update_batalia_cannon, default_moby_methods},
        {0x1b9,                     update_falling_debris, default_moby_methods},
        {0x1ba,                     update_falling_debris, default_moby_methods},
        {0x1bb,                     update_falling_debris, default_moby_methods},
        {0x1bc,                            update_gunship, default_moby_methods},
        {0x1bd,                     update_falling_debris, default_moby_methods},
        {0x1c0,                     update_falling_debris, default_moby_methods},
        {0x1c1,                     update_falling_debris, default_moby_methods},
        {0x1c2,                     update_falling_debris, default_moby_methods},
        {0x1c3,                     update_falling_debris, default_moby_methods},
        {0x1c4,                         bolt_thief_update, default_moby_methods},
        {0x1c6,                          FUN_L00_002cc608, default_moby_methods},
        {0x1c7,                          FUN_L03_002bef68, default_moby_methods},
        {0x1c9,                        update_ryno_rocket, default_moby_methods},
        {0x1ca,                      update_turret_rocket, default_moby_methods},
        {0x1ce,                              shrub_update, default_moby_methods},
        {0x1cf,                      update_heavy_gunship, default_moby_methods},
        {0x1d0,                     update_falling_debris, default_moby_methods},
        {0x1d1,                     update_falling_debris, default_moby_methods},
        {0x1d3,                               update_lift, default_moby_methods},
        {0x1d4,                         update_swing_gate, default_moby_methods},
        {0x1d5,                         update_swing_gate, default_moby_methods},
        {0x1d8,                               update_lift, default_moby_methods},
        {0x1df,                              update_drone, default_moby_methods},
        {0x1e3,                                         0, default_moby_methods},
        {0x1ec,                          red_plant_update, default_moby_methods},
        {0x1f4,                              crate_update, default_moby_methods},
        {0x1f5,                              crate_update, default_moby_methods},
        {0x1f9,                              crate_update, default_moby_methods},
        {0x1ff,                              crate_update, default_moby_methods},
        {0x221,                          FUN_L03_002c4718, default_moby_methods},
        {0x223,                          blarg_gun_update, default_moby_methods},
        {0x224,                          blarg_gun_update, default_moby_methods},
        {0x225,                          blarg_gun_update, default_moby_methods},
        {0x226,                          blarg_gun_update, default_moby_methods},
        {0x227,                          blarg_gun_update, default_moby_methods},
        {0x232,                        decoy_glove_update, default_moby_methods},
        {0x236,                        noop_main_callback, default_moby_methods},
        {0x237,             update_smoke_trail_projectile, default_moby_methods},
        {0x249,                     metal_detector_update, default_moby_methods},
        {0x24c,                          blarg_gun_update, default_moby_methods},
        {0x24d,                          blarg_gun_update, default_moby_methods},
        {0x24e,                          blarg_gun_update, default_moby_methods},
        {0x24f,                          blarg_gun_update, default_moby_methods},
        {0x250,                          blarg_gun_update, default_moby_methods},
        {0x251,                          blarg_gun_update, default_moby_methods},
        {0x252,                          blarg_gun_update, default_moby_methods},
        {0x253,                          blarg_gun_update, default_moby_methods},
        {0x254,                          blarg_gun_update, default_moby_methods},
        {0x255,                          blarg_gun_update, default_moby_methods},
        {0x256,                          blarg_gun_update, default_moby_methods},
        {0x258,                update_layered_mesh_effect, default_moby_methods},
        {0x25c,                    sandmouse_house_update, default_moby_methods},
        {0x25d,        update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                           helipack_update, default_moby_methods},
        {0x267,                    tresspasser_pad_update, default_moby_methods},
        {0x26a,                        noop_main_callback, default_moby_methods},
        {0x26d,                           update_landmine, default_moby_methods},
        {0x27a,                                         0, default_moby_methods},
        {0x27f,          update_animated_point_light_moby, default_moby_methods},
        {0x288,          update_directional_spark_emitter, default_moby_methods},
        {0x29f,             update_flickering_mesh_effect, default_moby_methods},
        {0x2a7,                   update_rail_hero_motion, default_moby_methods},
        {0x2ee,                            infobot_update, default_moby_methods},
        {0x306,                        the_plumber_update, default_moby_methods},
        {0x307,                  grow_and_fade_out_update, default_moby_methods},
        {0x30e,                          blarg_gun_update, default_moby_methods},
        {0x30f,                          blarg_gun_update, default_moby_methods},
        {0x310,                          blarg_gun_update, default_moby_methods},
        {0x311,                          blarg_gun_update, default_moby_methods},
        {0x324, update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                        spawn_point_update, default_moby_methods},
        {0x326,                      nanotech_ball_update, default_moby_methods},
        {0x340,        update_target_outside_zone_timeout, default_moby_methods},
        {0x351,                         suckcannon_update, default_moby_methods},
        {0x3ee,                    devestator_ammo_update, default_moby_methods},
        {0x3ff,                  blarg_paratrooper_update, default_moby_methods},
        {0x40f,        update_hero_trigger_volume_monitor, default_moby_methods},
        {0x46a,                           commando_update, default_moby_methods},
        {0x46e,                          gold_bolt_update, default_moby_methods},
        {0x46f,                         teleporter_update, default_moby_methods},
        {0x472,                          quad_moby_update, default_moby_methods},
        {0x477,                         vendor_orb_update, default_moby_methods},
        {0x478,                   batalia_deserter_update, default_moby_methods},
        {0x4a8,                      update_glow_particle, default_moby_methods},
        {0x4e3,                     hydrodisplacer_update, default_moby_methods},
        {0x503,                the_plumber_batalia_update, default_moby_methods},
        {0x509,                        noop_main_callback, default_moby_methods},
        {0x50a,                      pilots_helmet_update, default_moby_methods},
        {0x50c,                          update_fire_bolt, default_moby_methods},
        {0x545,                         update_help_hints, default_moby_methods},
        {0x578,                  scatter_ground_particles, default_moby_methods},
        {0x59e,                    devestator_ammo_update, default_moby_methods},
        {0x5a7,                    devestator_ammo_update, default_moby_methods},
        {0x5a9,                    devestator_ammo_update, default_moby_methods},
        {0x611,             update_cutscene_camera_target, default_moby_methods},
        {0x65d,               spawn_moby_radial_particles, default_moby_methods},
        {0x661,               update_sandmouse_projectile, default_moby_methods},
        {0x669,                     update_effect_emitter, default_moby_methods},
        {0x671,            update_batalia_textured_effect, default_moby_methods},
        {0x69c,                          robot_arm_update, default_moby_methods},
        {0x69d,                          robot_arm_update, default_moby_methods},
        {0x69e,                          robot_arm_update, default_moby_methods},
        {0x6c0,                          robot_arm_update, default_moby_methods},
        {0x6c1,                          robot_arm_update, default_moby_methods},
        {0x6c2,                          robot_arm_update, default_moby_methods},
        {0x6c3,                          robot_arm_update, default_moby_methods},
        {0x6c4,                          robot_arm_update, default_moby_methods},
        {0x6c5,                          robot_arm_update, default_moby_methods},
        {0x6c6,                          robot_arm_update, default_moby_methods},
        {0x6c7,                          robot_arm_update, default_moby_methods},
        {0x6fe,                          robot_arm_update, default_moby_methods},
        {0x6ff,                          robot_arm_update, default_moby_methods},
        {0x71a,                          FUN_L01_0030df40, default_moby_methods},
        {0x734,                    update_shattering_prop, default_moby_methods},
        {0x735,                        noop_main_callback, default_moby_methods},
        {0x736,                          robot_arm_update, default_moby_methods},
        {0x737,                          robot_arm_update, default_moby_methods},
        {0x76c,                              decoy_update, default_moby_methods},
        {   -1,                                         0, default_moby_methods},
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
    { 6,            noop_callback_l,     initialize_orbit_camera_from_hero,     mine_blend_color,                          noop_main_callback},
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
