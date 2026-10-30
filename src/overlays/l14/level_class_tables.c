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
void FUN_L03_002bef68();
void FUN_L03_002c4718();
void FUN_L10_002f5f18();
void FUN_L14_002f0538();
void FUN_L14_002fdbb8();
void FUN_L14_00305758();
void update_unfolding_column_piece() __asm__("FUN_L14_003061d8");
void FUN_L14_00306ee0();
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_thief_update() __asm__("FUN_L14_002fe2a0");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void button_update() __asm__("FUN_L14_00306b78");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void check_path_camera_priority() __asm__("FUN_L14_00315920");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void elevator_update() __asm__("FUN_L14_002ee8d8");
void enter_scoutbot_return_home_state() __asm__("FUN_L08_002d4050");
void evaluate_descriptor_eligibility() __asm__("FUN_L10_002f65f8");
void check_path_key_camera_priority() __asm__("FUN_L14_00316748");
void exit_path_key_camera() __asm__("FUN_L14_003167e0");
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
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_path_camera() __asm__("FUN_L10_002f5b58");
void initialize_path_key_camera() __asm__("FUN_L14_003159d8");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void machinegun_turret_update() __asm__("FUN_L14_002b3bf0");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void minibomber_update() __asm__("FUN_L14_002b46e8");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void particle_spawner_smoke_fire_update() __asm__("FUN_L00_002d6f28");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void plasmabot_hatch_update() __asm__("FUN_L14_002d96e0");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void qwark_oltanis_update() __asm__("FUN_L14_002fba20");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void sam_update() __asm__("FUN_L14_002fefe0");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void scatter_ground_particles() __asm__("FUN_L08_00307cf0");
void scoutbot_update() __asm__("FUN_L08_002d2af0");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void shuttle_update() __asm__("FUN_L14_002e7bd8");
void spawn_moby_radial_particles() __asm__("FUN_L08_003085f0");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void trespasser_update() __asm__("FUN_L00_002c1718");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_beam_slots() __asm__("FUN_L14_003039e0");
void update_blarg_heavy_interceptor() __asm__("FUN_L14_002ac618");
void update_bouncing_spark() __asm__("FUN_L14_00300e00");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_drone() __asm__("FUN_L00_002ce358");
void enemy_fighter_update() __asm__("FUN_L14_002fc0f0");
void rocket_strike_fighter_update() __asm__("FUN_L14_002fcbb0");
void update_explosive_projectile() __asm__("FUN_L01_002f6a30");
void update_bolt_thief_mine() __asm__("FUN_L14_002ff640");
void update_flying_bolt() __asm__("FUN_L14_002ece00");
void update_flying_debris() __asm__("FUN_L14_002bac78");
void update_frame_timing_and_wrapped_value() __asm__("FUN_L00_002a4cd8");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_ground_height_modulated_voice() __asm__("FUN_L08_00316fa8");
void update_group_clear_trigger() __asm__("FUN_L14_003087c0");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_level14_racer() __asm__("FUN_L14_002d5f40");
void update_level_effects() __asm__("FUN_L14_00307a80");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_looping_path_mine() __asm__("FUN_L14_002ec810");
void update_audio_stream_callback_keeper() __asm__("FUN_L14_00303370");
void update_moby_glow_and_contact_explosion() __asm__("FUN_L05_0031c8e0");
void update_moby_spatial_trigger() __asm__("FUN_L05_002f5200");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void setup_path_camera() __asm__("FUN_L14_00314f00");
void update_path_camera() __asm__("FUN_L14_00315290");
void update_path_key_camera() __asm__("FUN_L14_00315d48");
void update_path_lightning_controller() __asm__("FUN_L14_002ed280");
void update_path_mover() __asm__("FUN_L14_002aba80");
void update_path_or_ballistic_debris() __asm__("FUN_L14_002dfd10");
void update_plasmabot() __asm__("FUN_L14_002b17d8");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_ribbon_effect_moby() __asm__("FUN_L14_002dee28");
void update_sandmouse_projectile() __asm__("FUN_L01_0030c9a8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_spinning_moby_with_particle_trail() __asm__("FUN_L14_002de670");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_tilting_platform() __asm__("FUN_L14_002de1f8");
void update_unfolding_column() __asm__("FUN_L14_00305d28");
void update_unlock_sequence_controller() __asm__("FUN_L14_00305408");
void update_electric_beam() __asm__("FUN_L14_003015d0");
void update_wind_control() __asm__("FUN_L14_002eaf88");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L14_001FD9B8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L14_001FD9D0[6] __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                enter_scoutbot_return_home_state,
                                                                get_moby_data_plus_0x60,
                                                                mine_blend_color};

void *D_L14_001FD9E8[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[154] __asm__("D_L14_001FD280")

    __attribute__((section("lvl.vtbl"))) = {
        {0x329,     update_frame_timing_and_wrapped_value, default_moby_methods},
        {0x387,                         update_path_mover, default_moby_methods},
        {0x3ef,                      camera_object_update, default_moby_methods},
        {  0x8,            update_blarg_heavy_interceptor, default_moby_methods},
        {  0xb,                             vendor_update, default_moby_methods},
        {  0xd,                               bolt_update, default_moby_methods},
        {  0xe,                               bolt_update, default_moby_methods},
        {  0xf,                               bolt_update, default_moby_methods},
        { 0x10,                               bolt_update, default_moby_methods},
        { 0x1c,                          update_plasmabot, default_moby_methods},
        { 0x1e,                  machinegun_turret_update, default_moby_methods},
        { 0x1f,                         minibomber_update, default_moby_methods},
        { 0x47,                             wrench_update, default_moby_methods},
        { 0x4a,                               mine_update, default_moby_methods},
        { 0x51,                      update_flying_debris, default_moby_methods},
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
        { 0xd3,                      update_level14_racer, default_moby_methods},
        { 0xd5,                    devestator_ammo_update, default_moby_methods},
        { 0xd6,                    devestator_ammo_update, default_moby_methods},
        { 0xde,                    devestator_ammo_update, default_moby_methods},
        { 0xdf,                    devestator_ammo_update, default_moby_methods},
        { 0xe1,                    devestator_ammo_update, default_moby_methods},
        { 0xe2,                    devestator_ammo_update, default_moby_methods},
        { 0xe5,                      glove_of_doom_update, default_moby_methods},
        { 0xe6,                          FUN_L00_002c89e8, default_moby_methods},
        { 0xfa,                    plasmabot_hatch_update, default_moby_methods},
        { 0xfc,                           scoutbot_update,       D_L14_001FD9D0},
        {0x102,               update_moby_spatial_trigger, default_moby_methods},
        {0x10e,                  update_morphoray_chicken,       D_L14_001FD9E8},
        {0x131,                        update_homing_shot, default_moby_methods},
        {0x135,                   update_tilting_platform, default_moby_methods},
        {0x145,  update_spinning_moby_with_particle_trail, default_moby_methods},
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
        {0x182,                 update_ribbon_effect_moby, default_moby_methods},
        {0x193,           update_path_or_ballistic_debris, default_moby_methods},
        {0x1ac,                update_damped_falling_moby, default_moby_methods},
        {0x1b1,                        noop_main_callback, default_moby_methods},
        {0x1c6,                          FUN_L00_002cc608, default_moby_methods},
        {0x1c7,                          FUN_L03_002bef68, default_moby_methods},
        {0x1c9,                        update_ryno_rocket, default_moby_methods},
        {0x1df,                              update_drone, default_moby_methods},
        {0x1e3,                                         0, default_moby_methods},
        {0x1f4,                              crate_update, default_moby_methods},
        {0x1f5,                              crate_update, default_moby_methods},
        {0x1f6,                              crate_update, default_moby_methods},
        {0x1f9,                              crate_update, default_moby_methods},
        {0x1ff,                              crate_update, default_moby_methods},
        {0x213,                        noop_main_callback, default_moby_methods},
        {0x221,                          FUN_L03_002c4718, default_moby_methods},
        {0x22d,                            shuttle_update, default_moby_methods},
        {0x232,                        decoy_glove_update, default_moby_methods},
        {0x249,                     metal_detector_update, default_moby_methods},
        {0x25c,                    sandmouse_house_update, default_moby_methods},
        {0x25d,        update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                           helipack_update, default_moby_methods},
        {0x262,                       update_wind_control, default_moby_methods},
        {0x26a,                        noop_main_callback, default_moby_methods},
        {0x27a,                                         0, default_moby_methods},
        {0x27f,          update_animated_point_light_moby, default_moby_methods},
        {0x283,                  update_looping_path_mine, default_moby_methods},
        {0x2a9,                        update_flying_bolt, default_moby_methods},
        {0x2ac,          update_path_lightning_controller, default_moby_methods},
        {0x2ad,                           elevator_update, default_moby_methods},
        {0x2ae,               update_explosive_projectile, default_moby_methods},
        {0x2c8,                          FUN_L14_002f0538, default_moby_methods},
        {0x2ee,                            infobot_update, default_moby_methods},
        {0x2f6,              swingshot_versatarget_update, default_moby_methods},
        {0x2f8,        particle_spawner_smoke_fire_update, default_moby_methods},
        {0x2fd,                        noop_main_callback, default_moby_methods},
        {0x307,                  grow_and_fade_out_update, default_moby_methods},
        {0x323,              swingshot_versatarget_update, default_moby_methods},
        {0x324, update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                        spawn_point_update, default_moby_methods},
        {0x326,                      nanotech_ball_update, default_moby_methods},
        {0x340,        update_target_outside_zone_timeout, default_moby_methods},
        {0x351,                         suckcannon_update, default_moby_methods},
        {0x353,                      qwark_oltanis_update, default_moby_methods},
        {0x38c,                      enemy_fighter_update, default_moby_methods},
        {0x391,                        noop_main_callback, default_moby_methods},
        {0x399,              rocket_strike_fighter_update, default_moby_methods},
        {0x39a,                          FUN_L14_002fdbb8, default_moby_methods},
        {0x39b,                         bolt_thief_update, default_moby_methods},
        {0x39c,                                sam_update, default_moby_methods},
        {0x3a5,                    update_bolt_thief_mine, default_moby_methods},
        {0x3ee,                    devestator_ammo_update, default_moby_methods},
        {0x46e,                          gold_bolt_update, default_moby_methods},
        {0x472,                          quad_moby_update, default_moby_methods},
        {0x477,                         vendor_orb_update, default_moby_methods},
        {0x4a8,                      update_glow_particle, default_moby_methods},
        {0x4a9,                     update_bouncing_spark, default_moby_methods},
        {0x4c8,                      update_electric_beam, default_moby_methods},
        {0x4e3,                     hydrodisplacer_update, default_moby_methods},
        {0x4ec,       update_audio_stream_callback_keeper, default_moby_methods},
        {0x509,                        noop_main_callback, default_moby_methods},
        {0x50a,                      pilots_helmet_update, default_moby_methods},
        {0x533,                         update_beam_slots, default_moby_methods},
        {0x548,         update_unlock_sequence_controller, default_moby_methods},
        {0x54a,                          FUN_L14_00305758, default_moby_methods},
        {0x573,                   update_unfolding_column, default_moby_methods},
        {0x574,                        noop_main_callback, default_moby_methods},
        {0x575,             update_unfolding_column_piece, default_moby_methods},
        {0x578,                  scatter_ground_particles, default_moby_methods},
        {0x588,                             button_update, default_moby_methods},
        {0x589,                          FUN_L14_00306ee0, default_moby_methods},
        {0x58a,                      update_level_effects, default_moby_methods},
        {0x59e,                    devestator_ammo_update, default_moby_methods},
        {0x5a7,                    devestator_ammo_update, default_moby_methods},
        {0x5a9,                    devestator_ammo_update, default_moby_methods},
        {0x5e7,    update_moby_glow_and_contact_explosion, default_moby_methods},
        {0x5ea,                        noop_main_callback, default_moby_methods},
        {0x5ed,                          robot_arm_update, default_moby_methods},
        {0x5ee,                          robot_arm_update, default_moby_methods},
        {0x617,                update_group_clear_trigger, default_moby_methods},
        {0x65d,               spawn_moby_radial_particles, default_moby_methods},
        {0x661,               update_sandmouse_projectile, default_moby_methods},
        {0x6c5,                          robot_arm_update, default_moby_methods},
        {0x6c6,                          robot_arm_update, default_moby_methods},
        {0x6c7,                          robot_arm_update, default_moby_methods},
        {0x71a,                          FUN_L01_0030df40, default_moby_methods},
        {0x76c,                              decoy_update, default_moby_methods},
        {   -1,                                         0, default_moby_methods},
};

struct CameraType level_camera_types[10] __asm__("D_L14_001FDA00") __attribute__((section("lvl.camvtbl"))) = {
    { 0,     noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                        },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                   },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                              },
    { 6,             noop_callback_l, initialize_orbit_camera_from_hero,     mine_blend_color,                          noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                           },
    {17,            FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {19,
     evaluate_descriptor_eligibility,            initialize_path_camera,
     FUN_L10_002f5f18,                          noop_main_callback                                                                        },
    {20,  check_path_camera_priority,                 setup_path_camera,   update_path_camera,                          noop_main_callback},
    {21,
     check_path_key_camera_priority,        initialize_path_key_camera,
     update_path_key_camera,                        exit_path_key_camera                                                                  },
    {-1,                           0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[6] __asm__("D_L14_001FDB00")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 1,                        update_box_emitter},
        { 3,                 update_reverb_box_emitter},
        { 4,      update_ground_height_modulated_voice},
        { 6,               update_music_switch_emitter},
        {-1,                                         0},
};
