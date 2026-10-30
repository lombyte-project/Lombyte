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
void FUN_L09_002c26c8();
void FUN_L09_002edc30();
void airship_chain_link_update() __asm__("FUN_L09_00304360");
void asteroid_update() __asm__("FUN_L09_002eb970");
void background_ship_1_update() __asm__("FUN_L02_002ec4b8");
void background_ship_2_update() __asm__("FUN_L02_002ece18");
void barrier_update() __asm__("FUN_L09_003082d8");
void blarg_paratrooper_update() __asm__("FUN_L08_00301158");
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
void elevator_update() __asm__("FUN_L09_003033a0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void lamp_update() __asm__("FUN_L09_0030a678");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_hold_state_if_grabbed() __asm__("FUN_L01_002f1c78");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void platform_update() __asm__("FUN_L09_002f86a0");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void refueling_station_update() __asm__("FUN_L09_00303d10");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void run_moby_helper_and_set_state_7() __asm__("FUN_L01_002f1dc8");
void seeker_update() __asm__("FUN_L09_002c5990");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void shuttle_update() __asm__("FUN_L09_00309c08");
void spawn_point_update() __asm__("FUN_L01_00300220");
void spawner_egg_update() __asm__("FUN_L09_0030b3b0");
void spawner_update() __asm__("FUN_L09_0030ab80");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void trespasser_update() __asm__("FUN_L00_002c1718");
void turret_update() __asm__("FUN_L09_00304c80");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_billboard_projectile() __asm__("FUN_L01_002c7530");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_damage_causer() __asm__("FUN_L09_00305a28");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_destructible_prop() __asm__("FUN_L09_002ee110");
void update_drifting_moby() __asm__("FUN_L09_002f0040");
void update_drone() __asm__("FUN_L00_002ce358");
void update_fire_bolt() __asm__("FUN_L08_00307298");
void update_floating_ship_piece() __asm__("FUN_L09_003091b0");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_ground_melee_enemy() __asm__("FUN_L09_002e27d8");
void update_help_hints() __asm__("FUN_L09_00300888");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_moby_draw_registration() __asm__("FUN_L07_002f8058");
void update_moby_spatial_trigger() __asm__("FUN_L05_002f5200");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void update_shrinking_tumbling_debris() __asm__("FUN_L09_002ef9b0");
void update_smoke_trail_projectile() __asm__("FUN_L01_002eda00");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_tumbling_debris() __asm__("FUN_L09_002ef430");
void update_turret_shot() __asm__("FUN_L09_00307d68");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L09_00209AE8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L09_00209B00[6] __attribute__((section("lvl.vtbl"))) = {moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                run_moby_helper_and_set_state_7,
                                                                get_moby_data_plus_0x60,
                                                                mine_blend_color};

void *D_L09_00209B18[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[158] __asm__("D_L09_00209380")

    __attribute__((section("lvl.vtbl"))) = {
        {0x3ef,                      camera_object_update, default_moby_methods},
        {0x4a0,                          FUN_L09_002c26c8, default_moby_methods},
        {  0xb,                             vendor_update, default_moby_methods},
        {  0xd,                               bolt_update, default_moby_methods},
        {  0xe,                               bolt_update, default_moby_methods},
        {  0xf,                               bolt_update, default_moby_methods},
        { 0x10,                               bolt_update, default_moby_methods},
        { 0x34,                             seeker_update, default_moby_methods},
        { 0x47,                             wrench_update, default_moby_methods},
        { 0x4a,                               mine_update, default_moby_methods},
        { 0x70,                      update_glow_particle, default_moby_methods},
        { 0x79,                               bomb_update, default_moby_methods},
        { 0x7a,                             debris_update, default_moby_methods},
        { 0x95,            nanotech_crate_fragment_update, default_moby_methods},
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
        { 0xc1,                 update_ground_melee_enemy,       D_L09_00209B00},
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
        {0x107,                           asteroid_update, default_moby_methods},
        {0x108,                           asteroid_update, default_moby_methods},
        {0x109,                           asteroid_update, default_moby_methods},
        {0x10a,                           asteroid_update, default_moby_methods},
        {0x10b,                           asteroid_update, default_moby_methods},
        {0x10e,                  update_morphoray_chicken,       D_L09_00209B18},
        {0x114,                          FUN_L09_002edc30, default_moby_methods},
        {0x117,                  update_destructible_prop, default_moby_methods},
        {0x11c,                        noop_main_callback, default_moby_methods},
        {0x131,                        update_homing_shot, default_moby_methods},
        {0x139,                    update_tumbling_debris, default_moby_methods},
        {0x13d,             update_moby_draw_registration, default_moby_methods},
        {0x140,          update_shrinking_tumbling_debris, default_moby_methods},
        {0x141,          update_shrinking_tumbling_debris, default_moby_methods},
        {0x144,                        noop_main_callback, default_moby_methods},
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
        {0x1a1,                      update_drifting_moby, default_moby_methods},
        {0x1ac,                update_damped_falling_moby, default_moby_methods},
        {0x1b1,                        noop_main_callback, default_moby_methods},
        {0x1c6,                          FUN_L00_002cc608, default_moby_methods},
        {0x1c9,                        update_ryno_rocket, default_moby_methods},
        {0x1df,                              update_drone, default_moby_methods},
        {0x1e3,                                         0, default_moby_methods},
        {0x1f4,                              crate_update, default_moby_methods},
        {0x1f5,                              crate_update, default_moby_methods},
        {0x1f6,                              crate_update, default_moby_methods},
        {0x1f9,                              crate_update, default_moby_methods},
        {0x1ff,                              crate_update, default_moby_methods},
        {0x232,                        decoy_glove_update, default_moby_methods},
        {0x237,             update_smoke_trail_projectile, default_moby_methods},
        {0x249,                     metal_detector_update, default_moby_methods},
        {0x25d,        update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                           helipack_update, default_moby_methods},
        {0x26a,                        noop_main_callback, default_moby_methods},
        {0x27a,                                         0, default_moby_methods},
        {0x27f,          update_animated_point_light_moby, default_moby_methods},
        {0x298,                           platform_update, default_moby_methods},
        {0x307,                  grow_and_fade_out_update, default_moby_methods},
        {0x323,              swingshot_versatarget_update, default_moby_methods},
        {0x324, update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                        spawn_point_update, default_moby_methods},
        {0x326,                      nanotech_ball_update, default_moby_methods},
        {0x340,        update_target_outside_zone_timeout, default_moby_methods},
        {0x351,                         suckcannon_update, default_moby_methods},
        {0x3e8,                         update_help_hints, default_moby_methods},
        {0x3ee,                    devestator_ammo_update, default_moby_methods},
        {0x3ff,                  blarg_paratrooper_update, default_moby_methods},
        {0x46e,                          gold_bolt_update, default_moby_methods},
        {0x472,                          quad_moby_update, default_moby_methods},
        {0x477,                         vendor_orb_update, default_moby_methods},
        {0x47e,                           elevator_update, default_moby_methods},
        {0x47f,                           elevator_update, default_moby_methods},
        {0x494,                  refueling_station_update, default_moby_methods},
        {0x49d,                 airship_chain_link_update, default_moby_methods},
        {0x49e,                          FUN_L09_002c26c8, default_moby_methods},
        {0x49f,                          FUN_L09_002c26c8, default_moby_methods},
        {0x4a1,                          FUN_L09_002c26c8, default_moby_methods},
        {0x4a2,                          FUN_L09_002c26c8, default_moby_methods},
        {0x4a3,                          FUN_L09_002c26c8, default_moby_methods},
        {0x4a4,                          FUN_L09_002c26c8, default_moby_methods},
        {0x4a5,                          FUN_L09_002c26c8, default_moby_methods},
        {0x4a8,                      update_glow_particle, default_moby_methods},
        {0x4b1,                             turret_update, default_moby_methods},
        {0x4b6,                      update_damage_causer, default_moby_methods},
        {0x4bc,                  background_ship_1_update, default_moby_methods},
        {0x4bd,                  background_ship_2_update, default_moby_methods},
        {0x4e3,                     hydrodisplacer_update, default_moby_methods},
        {0x4ea,                        update_turret_shot, default_moby_methods},
        {0x505,                            barrier_update, default_moby_methods},
        {0x506,                            barrier_update, default_moby_methods},
        {0x507,                            barrier_update, default_moby_methods},
        {0x508,                            barrier_update, default_moby_methods},
        {0x509,                        noop_main_callback, default_moby_methods},
        {0x50a,                      pilots_helmet_update, default_moby_methods},
        {0x50c,                          update_fire_bolt, default_moby_methods},
        {0x50d,                update_floating_ship_piece, default_moby_methods},
        {0x512,                          FUN_L09_002edc30, default_moby_methods},
        {0x513,                          FUN_L09_002edc30, default_moby_methods},
        {0x514,                        noop_main_callback, default_moby_methods},
        {0x528,                update_floating_ship_piece, default_moby_methods},
        {0x59e,                    devestator_ammo_update, default_moby_methods},
        {0x5a7,                    devestator_ammo_update, default_moby_methods},
        {0x5a9,                    devestator_ammo_update, default_moby_methods},
        {0x5f2,                          robot_arm_update, default_moby_methods},
        {0x5f5,                          robot_arm_update, default_moby_methods},
        {0x5f6,                          robot_arm_update, default_moby_methods},
        {0x5f8,                          robot_arm_update, default_moby_methods},
        {0x5f9,                          robot_arm_update, default_moby_methods},
        {0x69c,                          robot_arm_update, default_moby_methods},
        {0x69d,                          robot_arm_update, default_moby_methods},
        {0x69e,                          robot_arm_update, default_moby_methods},
        {0x6c5,                          robot_arm_update, default_moby_methods},
        {0x6c6,                          robot_arm_update, default_moby_methods},
        {0x6c7,                          robot_arm_update, default_moby_methods},
        {0x6e6,                            shuttle_update, default_moby_methods},
        {0x739,                               lamp_update, default_moby_methods},
        {0x73a,                        noop_main_callback, default_moby_methods},
        {0x73b,                          robot_arm_update, default_moby_methods},
        {0x75d,                            spawner_update, default_moby_methods},
        {0x75e,                        spawner_egg_update, default_moby_methods},
        {0x76c,                              decoy_update, default_moby_methods},
        {0x790,                          robot_arm_update, default_moby_methods},
        {   -1,                                         0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L09_00209B80") __attribute__((section("lvl.camvtbl"))) = {
    { 0,noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                 },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                            },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                       },
    { 6,      noop_callback_l, initialize_orbit_camera_from_hero,     mine_blend_color,                          noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                    },
    {17,     FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,  volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                    0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[4] __asm__("D_L09_00209C80")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 1,                        update_box_emitter},
        { 3,                 update_reverb_box_emitter},
        {-1,                                         0},
};
