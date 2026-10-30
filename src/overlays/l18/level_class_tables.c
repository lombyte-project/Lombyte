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
void FUN_L00_002cd1f0();
void FUN_L00_002ea7a0();
void FUN_L01_002f8718();
void FUN_L16_002a0a98();
void FUN_L18_002d82c0();
void armed_transport_update() __asm__("FUN_L16_002e22d8");
void blarg_elite_commando_update() __asm__("FUN_L16_002d2cf0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_moby_state_9() __asm__("FUN_L00_002d57d0");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drek_fight_missile_update() __asm__("FUN_L18_002efb88");
void drek_final_boss_robot_update() __asm__("FUN_L18_002f2bf0");
void floor_button_update() __asm__("FUN_L05_0030c5b0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void gadgebot_update() __asm__("FUN_L18_002fbb98");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void get_moby_private_data_d0_address() __asm__("FUN_L00_002d5800");
void giant_clank_pad_update() __asm__("FUN_L15_002ed068");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grindrail_barrier_update() __asm__("FUN_L16_002e5708");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hide_moby_and_clear_collision() __asm__("FUN_L18_002fcd00");
void hide_moby_and_disable_collision() __asm__("FUN_L18_002d6280");
void horny_toad_update() __asm__("FUN_L00_002d4610");
void hydrodisplacer_faucet_update() __asm__("FUN_L05_002f8080");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void update_inert_moby() __asm__("FUN_L18_002f7ab0");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_hold_state_while_active() __asm__("FUN_L18_002d6190");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void process_suck_cannon_suction_and_update_moby_state() __asm__("FUN_L00_002d56e0");
void pterodactyl_update() __asm__("FUN_L00_002e3a88");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void red_orb_update() __asm__("FUN_L00_002df4f8");
void register_audio_stream_callback() __asm__("FUN_00215420");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void rocket_tank_update() __asm__("FUN_L18_002f7c40");
void update_blarg_elite_commando_rocket() __asm__("FUN_L15_0029ac30");
void run_moby_helper_and_set_state_7() __asm__("FUN_L01_002f1dc8");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_lock_green_update() __asm__("FUN_L18_002d6600");
void thruster_pack_lock_red_update() __asm__("FUN_L18_002d7b20");
void trackdrive_left_update() __asm__("FUN_L02_002cb978");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_drek_boss_ring_projectile() __asm__("FUN_L18_002db460");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_giant_clank_charged_blast() __asm__("FUN_L15_0029edb0");
void update_mine() __asm__("FUN_L18_002d5918");
void update_drek_boss_damage_ring() __asm__("FUN_L18_002dc260");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_deplanetizer_gun_sequence_effects() __asm__("FUN_L18_002f88e8");
void update_drone() __asm__("FUN_L00_002ce358");
void update_trespasser_barrier() __asm__("FUN_L18_002f1e68");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_hero_trigger_volume_monitor() __asm__("FUN_L06_002f7930");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_lift_state_and_queue_draw() __asm__("FUN_L18_002f2310");
void update_linked_moby_group_transform_and_fall() __asm__("FUN_L18_002f16f0");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_listener_emitter() __asm__("FUN_L01_0031a078");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_moby_hit_response_burst_and_ballistic_flight() __asm__("FUN_L18_002e9768");
void update_moby_rotation_group() __asm__("FUN_L06_002f4f00");
void update_moby_scene_visibility_and_height_reflection() __asm__("FUN_L18_002f7ad8");
void update_moby_state_pause() __asm__("FUN_L18_002d6108");
void update_moby_suck_cannon_state() __asm__("FUN_L18_002fcbb0");
void update_moby_target_travel_and_electric_effects() __asm__("FUN_L18_002e9e70");
void update_moby_transform_and_child() __asm__("FUN_L18_002fa728");
void update_moby_visibility_and_half_turn() __asm__("FUN_L18_002fa640");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_grind_rail_junction() __asm__("FUN_L18_002d62e8");
void update_camera_transition() __asm__("FUN_L18_002df608");
void update_projectile_motion_collision_and_trail() __asm__("FUN_L18_002a7220");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_rocket_moby() __asm__("FUN_L18_002d5050");
void update_giant_clank_hit_sphere() __asm__("FUN_L15_0029ead0");
void update_cutscene_joint_trail() __asm__("FUN_L18_002fad28");
void update_sound_emitter_playback_at_random_position() __asm__("FUN_L00_002edad8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_giant_clank_missile() __asm__("FUN_L15_0029d6c0");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_timed_sprite_effect() __asm__("FUN_L06_0030be60");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L18_001F3468")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L18_001F3480[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

void *D_L18_001F3498[6] __attribute__((section("lvl.vtbl"))) = {update_moby_state_pause,
                                                                moby_hold_state_while_active,
                                                                moby_hold_state_while_active,
                                                                mine_blend_color,
                                                                get_moby_data_plus_0x60,
                                                                hide_moby_and_disable_collision};

void *D_L18_001F34B0[6]

    __attribute__((section("lvl.vtbl"))) = {process_suck_cannon_suction_and_update_moby_state,
                                            process_suck_cannon_suction_and_update_moby_state,
                                            process_suck_cannon_suction_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_9,
                                            get_moby_private_data_d0_address,
                                            mine_blend_color};

void *D_L18_001F34C8[6] __attribute__((section("lvl.vtbl"))) = {update_moby_suck_cannon_state,
                                                                update_moby_suck_cannon_state,
                                                                update_moby_suck_cannon_state,
                                                                run_moby_helper_and_set_state_7,
                                                                get_moby_private_data_d0_address,
                                                                hide_moby_and_clear_collision};

struct MobyClassUpdate moby_classes[158] __asm__("D_L18_001F2D00")

    __attribute__((section("lvl.vtbl"))) = {
        {0x3ef,                                camera_object_update, default_moby_methods},
        {  0xb,                                       vendor_update, default_moby_methods},
        {  0xd,                                         bolt_update, default_moby_methods},
        {  0xe,                                         bolt_update, default_moby_methods},
        {  0xf,                                         bolt_update, default_moby_methods},
        { 0x10,                                         bolt_update, default_moby_methods},
        { 0x29,        update_projectile_motion_collision_and_trail, default_moby_methods},
        { 0x31,                  update_blarg_elite_commando_rocket, default_moby_methods},
        { 0x32,                                    FUN_L16_002a0a98, default_moby_methods},
        { 0x47,                                       wrench_update, default_moby_methods},
        { 0x4a,                                         mine_update, default_moby_methods},
        { 0x70,                                update_glow_particle, default_moby_methods},
        { 0x79,                                         bomb_update, default_moby_methods},
        { 0x7a,                                       debris_update, default_moby_methods},
        { 0x95,                      nanotech_crate_fragment_update, default_moby_methods},
        { 0x99,                      gold_devastator_missile_update, default_moby_methods},
        { 0x9d,                                   devastator_update, default_moby_methods},
        { 0xa3,                                     visibomb_update, default_moby_methods},
        { 0xa6,                           update_linked_moby_states, default_moby_methods},
        { 0xa7,                          update_thruster_pack_flame, default_moby_methods},
        { 0xa8,                                      blaster_update, default_moby_methods},
        { 0xac,                                    FUN_L00_002b6140, default_moby_methods},
        { 0xaf,                                      taunter_update, default_moby_methods},
        { 0xb0,                                    pyrocitor_update, default_moby_methods},
        { 0xb1,                                   tesla_claw_update, default_moby_methods},
        { 0xb3,               update_linked_moby_scale_and_rotation, default_moby_methods},
        { 0xb4,                                     walloper_update, default_moby_methods},
        { 0xb9,                                    morphoray_update, default_moby_methods},
        { 0xba,                                     doom_bot_update, default_moby_methods},
        { 0xbc,                                   trespasser_update, default_moby_methods},
        { 0xbe,                                   mine_glove_update, default_moby_methods},
        { 0xc0,                                   bomb_glove_update, default_moby_methods},
        { 0xcb,                                        decoy_update, default_moby_methods},
        { 0xcc,                              devestator_ammo_update, default_moby_methods},
        { 0xd0,                                    swingshot_update, default_moby_methods},
        { 0xd1,                               swingshot_hook_update, default_moby_methods},
        { 0xd5,                              devestator_ammo_update, default_moby_methods},
        { 0xd6,                              devestator_ammo_update, default_moby_methods},
        { 0xde,                              devestator_ammo_update, default_moby_methods},
        { 0xdf,                              devestator_ammo_update, default_moby_methods},
        { 0xe1,                              devestator_ammo_update, default_moby_methods},
        { 0xe2,                              devestator_ammo_update, default_moby_methods},
        { 0xe5,                                glove_of_doom_update, default_moby_methods},
        { 0xe6,                                    FUN_L00_002c89e8, default_moby_methods},
        {0x100,                          update_giant_clank_missile, default_moby_methods},
        {0x10e,                            update_morphoray_chicken,       D_L18_001F3480},
        {0x131,                                  update_homing_shot, default_moby_methods},
        {0x14b,                              trackdrive_left_update, default_moby_methods},
        {0x155,                        hydrodisplacer_faucet_update, default_moby_methods},
        {0x15c,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x15d,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x162,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x163,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x164,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x165,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x166,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x167,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x16b,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x16c,                      nanotech_crate_fragment_update, default_moby_methods},
        {0x1a3,                                  noop_main_callback, default_moby_methods},
        {0x1ac,                          update_damped_falling_moby, default_moby_methods},
        {0x1b1,                                  noop_main_callback, default_moby_methods},
        {0x1c6,                                    FUN_L00_002cc608, default_moby_methods},
        {0x1c9,                                    FUN_L00_002cd1f0, default_moby_methods},
        {0x1df,                                        update_drone, default_moby_methods},
        {0x1e3,                                                   0, default_moby_methods},
        {0x1f4,                                        crate_update, default_moby_methods},
        {0x1f5,                                        crate_update, default_moby_methods},
        {0x1f6,                                        crate_update, default_moby_methods},
        {0x1f9,                                        crate_update, default_moby_methods},
        {0x1ff,                                        crate_update, default_moby_methods},
        {0x232,                                  decoy_glove_update, default_moby_methods},
        {0x234,                                  update_rocket_moby, default_moby_methods},
        {0x238,                                         update_mine,       D_L18_001F3498},
        {0x246,                          update_grind_rail_junction, default_moby_methods},
        {0x247,                     thruster_pack_lock_green_update, default_moby_methods},
        {0x249,                               metal_detector_update, default_moby_methods},
        {0x24a,                       thruster_pack_lock_red_update, default_moby_methods},
        {0x24b,                                    FUN_L18_002d82c0, default_moby_methods},
        {0x25d,                  update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                                     helipack_update, default_moby_methods},
        {0x267,                              tresspasser_pad_update, default_moby_methods},
        {0x26a,                                  noop_main_callback, default_moby_methods},
        {0x270,                    update_drek_boss_ring_projectile, default_moby_methods},
        {0x274,                        update_drek_boss_damage_ring, default_moby_methods},
        {0x27a,                                                   0, default_moby_methods},
        {0x27e,                         blarg_elite_commando_update, default_moby_methods},
        {0x27f,                    update_animated_point_light_moby, default_moby_methods},
        {0x284,                            update_camera_transition, default_moby_methods},
        {0x2b8,                                    FUN_L01_002f8718, default_moby_methods},
        {0x2ed,                                   horny_toad_update,       D_L18_001F34B0},
        {0x2f6,                        swingshot_versatarget_update, default_moby_methods},
        {0x307,                            grow_and_fade_out_update, default_moby_methods},
        {0x323,                        swingshot_versatarget_update, default_moby_methods},
        {0x324,           update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                                  spawn_point_update, default_moby_methods},
        {0x326,                                nanotech_ball_update, default_moby_methods},
        {0x33e,                                 floor_button_update, default_moby_methods},
        {0x340,                  update_target_outside_zone_timeout, default_moby_methods},
        {0x351,                                   suckcannon_update, default_moby_methods},
        {0x375, update_moby_hit_response_burst_and_ballistic_flight, default_moby_methods},
        {0x378, update_moby_hit_response_burst_and_ballistic_flight, default_moby_methods},
        {0x37b, update_moby_hit_response_burst_and_ballistic_flight, default_moby_methods},
        {0x37c, update_moby_hit_response_burst_and_ballistic_flight, default_moby_methods},
        {0x37e, update_moby_hit_response_burst_and_ballistic_flight, default_moby_methods},
        {0x384, update_moby_hit_response_burst_and_ballistic_flight, default_moby_methods},
        {0x385, update_moby_hit_response_burst_and_ballistic_flight, default_moby_methods},
        {0x3a8, update_moby_hit_response_burst_and_ballistic_flight, default_moby_methods},
        {0x3d7,      update_moby_target_travel_and_electric_effects, default_moby_methods},
        {0x3ee,                              devestator_ammo_update, default_moby_methods},
        {0x3fd,                          update_moby_rotation_group, default_moby_methods},
        {0x40f,                  update_hero_trigger_volume_monitor, default_moby_methods},
        {0x424,                                      red_orb_update, default_moby_methods},
        {0x46e,                                    gold_bolt_update, default_moby_methods},
        {0x46f,                                   teleporter_update, default_moby_methods},
        {0x472,                                    quad_moby_update, default_moby_methods},
        {0x477,                                   vendor_orb_update, default_moby_methods},
        {0x4a8,                                update_glow_particle, default_moby_methods},
        {0x4e1,                                  noop_main_callback, default_moby_methods},
        {0x4e3,                               hydrodisplacer_update, default_moby_methods},
        {0x509,                                  noop_main_callback, default_moby_methods},
        {0x50a,                                pilots_helmet_update, default_moby_methods},
        {0x54b,                           drek_fight_missile_update, default_moby_methods},
        {0x54c,                              armed_transport_update, default_moby_methods},
        {0x565,         update_linked_moby_group_transform_and_fall, default_moby_methods},
        {0x570,                           update_trespasser_barrier, default_moby_methods},
        {0x57a,                    update_lift_state_and_queue_draw, default_moby_methods},
        {0x58e,                        drek_final_boss_robot_update, default_moby_methods},
        {0x593,                       update_giant_clank_hit_sphere, default_moby_methods},
        {0x598,                                   update_inert_moby, default_moby_methods},
        {0x59a,  update_moby_scene_visibility_and_height_reflection, default_moby_methods},
        {0x59b,  update_moby_scene_visibility_and_height_reflection, default_moby_methods},
        {0x59e,                              devestator_ammo_update, default_moby_methods},
        {0x5a7,                              devestator_ammo_update, default_moby_methods},
        {0x5a9,                              devestator_ammo_update, default_moby_methods},
        {0x5ae,                                  rocket_tank_update, default_moby_methods},
        {0x5f3,                    update_giant_clank_charged_blast, default_moby_methods},
        {0x61b,            update_deplanetizer_gun_sequence_effects, default_moby_methods},
        {0x61c,                                  pterodactyl_update, default_moby_methods},
        {0x620,                update_moby_visibility_and_half_turn, default_moby_methods},
        {0x630,                     update_moby_transform_and_child, default_moby_methods},
        {0x634,                                    robot_arm_update, default_moby_methods},
        {0x635,                                    robot_arm_update, default_moby_methods},
        {0x636,                                    robot_arm_update, default_moby_methods},
        {0x6d6,                      register_audio_stream_callback, default_moby_methods},
        {0x6fe,                                    robot_arm_update, default_moby_methods},
        {0x6ff,                                    robot_arm_update, default_moby_methods},
        {0x700,                                    robot_arm_update, default_moby_methods},
        {0x701,                                    robot_arm_update, default_moby_methods},
        {0x702,                                    robot_arm_update, default_moby_methods},
        {0x704,                                    robot_arm_update, default_moby_methods},
        {0x707,                         update_cutscene_joint_trail, default_moby_methods},
        {0x762,                            grindrail_barrier_update, default_moby_methods},
        {0x76a,                          update_timed_sprite_effect, default_moby_methods},
        {0x76b,                              giant_clank_pad_update, default_moby_methods},
        {0x76c,                                        decoy_update, default_moby_methods},
        {0x772,                                     gadgebot_update,       D_L18_001F34C8},
        {   -1,                                                   0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L18_001F3500") __attribute__((section("lvl.camvtbl"))) = {
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

struct SoundHandler sound_handlers[6] __asm__("D_L18_001F3600")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0,        update_sound_emitter_playback_by_distance},
        { 2, update_sound_emitter_playback_at_random_position},
        { 3,                        update_reverb_box_emitter},
        { 5,                          update_listener_emitter},
        { 6,                      update_music_switch_emitter},
        {-1,                                                0},
};
