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
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_moby_state_9() __asm__("FUN_L00_002d57d0");
void clank_cutscene_trigger_update() __asm__("FUN_L00_002d9dc8");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_private_data_d0_address() __asm__("FUN_L00_002d5800");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void horny_toad_update() __asm__("FUN_L00_002d4610");
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
void particle_spawner_smoke_fire_update() __asm__("FUN_L00_002d6f28");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void process_suck_cannon_suction_and_update_moby_state() __asm__("FUN_L00_002d56e0");
void pterodactyl_update() __asm__("FUN_L00_002e3a88");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void red_orb_update() __asm__("FUN_L00_002df4f8");
void red_plant_update() __asm__("FUN_L00_002e3ca8");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void trespasser_update() __asm__("FUN_L00_002c1718");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_beam_slot_controller() __asm__("FUN_L00_002e1df0");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_drone() __asm__("FUN_L00_002ce358");
void update_frame_timing_and_wrapped_value() __asm__("FUN_L00_002a4cd8");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_help_hints() __asm__("FUN_L00_002e0988");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_phase_effect_controller() __asm__("FUN_L00_002e3800");
void update_sound_emitter_playback_at_random_position() __asm__("FUN_L00_002edad8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void veldin_robot_enemy_update() __asm__("FUN_L00_002e0b88");
void veldin_ship_update() __asm__("FUN_L00_002d1e80");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L00_001EA7BC")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L00_001EA7D4[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

void *D_L00_001EA7EC[6]

    __attribute__((section("lvl.vtbl"))) = {process_suck_cannon_suction_and_update_moby_state,
                                            process_suck_cannon_suction_and_update_moby_state,
                                            process_suck_cannon_suction_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_9,
                                            get_moby_private_data_d0_address,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[101] __asm__("D_L00_001EA300")

    __attribute__((section("lvl.vtbl"))) = {
        {0x329,     update_frame_timing_and_wrapped_value, default_moby_methods},
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
        {0x10e,                  update_morphoray_chicken,       D_L00_001EA7D4},
        {0x131,                        update_homing_shot, default_moby_methods},
        {0x15c,            nanotech_crate_fragment_update, default_moby_methods},
        {0x15d,            nanotech_crate_fragment_update, default_moby_methods},
        {0x162,            nanotech_crate_fragment_update, default_moby_methods},
        {0x163,            nanotech_crate_fragment_update, default_moby_methods},
        {0x164,            nanotech_crate_fragment_update, default_moby_methods},
        {0x165,            nanotech_crate_fragment_update, default_moby_methods},
        {0x166,            nanotech_crate_fragment_update, default_moby_methods},
        {0x167,            nanotech_crate_fragment_update, default_moby_methods},
        {0x1ac,                update_damped_falling_moby, default_moby_methods},
        {0x1b1,                        noop_main_callback, default_moby_methods},
        {0x1c6,                          FUN_L00_002cc608, default_moby_methods},
        {0x1c9,                        update_ryno_rocket, default_moby_methods},
        {0x1df,                              update_drone, default_moby_methods},
        {0x1e3,                                         0, default_moby_methods},
        {0x1f4,                              crate_update, default_moby_methods},
        {0x1f5,                              crate_update, default_moby_methods},
        {0x1ff,                              crate_update, default_moby_methods},
        {0x212,                        veldin_ship_update, default_moby_methods},
        {0x232,                        decoy_glove_update, default_moby_methods},
        {0x249,                     metal_detector_update, default_moby_methods},
        {0x25f,                           helipack_update, default_moby_methods},
        {0x26a,                        noop_main_callback, default_moby_methods},
        {0x27a,                                         0, default_moby_methods},
        {0x27f,          update_animated_point_light_moby, default_moby_methods},
        {0x2ed,                         horny_toad_update,       D_L00_001EA7EC},
        {0x2ee,                            infobot_update, default_moby_methods},
        {0x2f8,        particle_spawner_smoke_fire_update, default_moby_methods},
        {0x307,                  grow_and_fade_out_update, default_moby_methods},
        {0x324, update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x326,                      nanotech_ball_update, default_moby_methods},
        {0x342,             clank_cutscene_trigger_update, default_moby_methods},
        {0x351,                         suckcannon_update, default_moby_methods},
        {0x3ee,                    devestator_ammo_update, default_moby_methods},
        {0x424,                            red_orb_update, default_moby_methods},
        {0x472,                          quad_moby_update, default_moby_methods},
        {0x477,                         vendor_orb_update, default_moby_methods},
        {0x4a8,                      update_glow_particle, default_moby_methods},
        {0x4e1,                        noop_main_callback, default_moby_methods},
        {0x4e3,                     hydrodisplacer_update, default_moby_methods},
        {0x509,                        noop_main_callback, default_moby_methods},
        {0x50a,                      pilots_helmet_update, default_moby_methods},
        {0x585,                         update_help_hints, default_moby_methods},
        {0x59e,                    devestator_ammo_update, default_moby_methods},
        {0x5a0,                 veldin_robot_enemy_update, default_moby_methods},
        {0x5a7,                    devestator_ammo_update, default_moby_methods},
        {0x5a9,                    devestator_ammo_update, default_moby_methods},
        {0x5bf,               update_beam_slot_controller, default_moby_methods},
        {0x609,            update_phase_effect_controller, default_moby_methods},
        {0x61c,                        pterodactyl_update, default_moby_methods},
        {0x6f5,                          red_plant_update, default_moby_methods},
        {0x6f6,                          red_plant_update, default_moby_methods},
        {0x76c,                              decoy_update, default_moby_methods},
        {0x78d,                          robot_arm_update, default_moby_methods},
        {0x78e,                          robot_arm_update, default_moby_methods},
        {0x78f,                          robot_arm_update, default_moby_methods},
        {0x7ab,                          robot_arm_update, default_moby_methods},
        {   -1,                                         0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L00_001EA880") __attribute__((section("lvl.camvtbl"))) = {
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

struct SoundHandler sound_handlers[3] __asm__("D_L00_001EA980")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0,        update_sound_emitter_playback_by_distance},
        { 2, update_sound_emitter_playback_at_random_position},
        {-1,                                                0},
};
