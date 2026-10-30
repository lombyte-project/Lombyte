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
void FUN_L01_002e0c68();
void FUN_L01_0030df40();
void FUN_L04_002af058();
void FUN_L04_002c41b0();
void FUN_L04_002c6858();
void big_axebot_update() __asm__("FUN_L04_002ba520");
void blarg_trooper_update() __asm__("FUN_L04_002c4850");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_moby_state_9() __asm__("FUN_L00_002d57d0");
void circular_saw_enemy_update() __asm__("FUN_L04_002c2270");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void dispatch_mode_selected_slot() __asm__("FUN_L04_002e53a0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void evaluate_clip_volume_candidate() __asm__("FUN_L03_002e8870");
void floating_platform_update() __asm__("FUN_L04_002cdda0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void interpolate_moby_and_linked_moby_positions_on_path() __asm__("FUN_L04_002d6f68");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void leaf_update() __asm__("FUN_L04_002d7e90");
void lieutenant_update() __asm__("FUN_L04_002e3078");
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
void pipe_update() __asm__("FUN_L04_002e79a8");
void platform_booster_update() __asm__("FUN_L04_002c6bb8");
void process_moby_hit_state_and_voice() __asm__("FUN_L04_002c40d0");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void rocket_update() __asm__("FUN_L00_002cd1f0");
void sand_mouse_update() __asm__("FUN_L01_002e3f20");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void set_transform_state_three_when_outside_clip_volume() __asm__("FUN_L03_002e8980");
void snow_plow_update() __asm__("FUN_L04_002ca420");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suck_cannon_update() __asm__("FUN_L04_002e1a10");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void transport_ship_update() __asm__("FUN_L04_002ce060");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_beam_effect_lifecycle() __asm__("FUN_L00_002b4198");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_class_55_swarm_controller() __asm__("FUN_L04_0029ecf8");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_drone() __asm__("FUN_L00_002ce358");
void update_entity_bounce_and_decay() __asm__("FUN_L04_002e5178");
void update_eudora_tutorial_hints_and_stats() __asm__("FUN_L04_002e4418");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_moby_random_interval_animation() __asm__("FUN_L04_002a5a08");
void update_moby_suck_cannon_state() __asm__("FUN_L02_002d61c0");
void update_moby_translation_delta_matrix() __asm__("FUN_L04_002d3580");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_box_exit_trigger() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_sandmouse_projectile() __asm__("FUN_L01_0030c9a8");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void upper_door_update() __asm__("FUN_L04_002e17d8");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void woodcut_bot_update() __asm__("FUN_L04_002d16b8");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L04_001DD0EC")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L04_001DD104[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

void *D_L04_001DD11C[6]

    __attribute__((section("lvl.vtbl"))) = {update_moby_suck_cannon_state,
                                            update_moby_suck_cannon_state,
                                            process_moby_hit_state_and_voice,
                                            cancel_suck_cannon_suction_and_set_moby_state_9,
                                            FUN_L04_002c41b0,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[169] __asm__("D_L04_001DC900")

    __attribute__((section("lvl.vtbl"))) = {
        {0x3ef,                               camera_object_update, default_moby_methods},
        {  0xb,                                      vendor_update, default_moby_methods},
        {  0xd,                                        bolt_update, default_moby_methods},
        {  0xe,                                        bolt_update, default_moby_methods},
        {  0xf,                                        bolt_update, default_moby_methods},
        { 0x10,                                        bolt_update, default_moby_methods},
        { 0x36,                                                  0, default_moby_methods},
        { 0x47,                                      wrench_update, default_moby_methods},
        { 0x4a,                                        mine_update, default_moby_methods},
        { 0x55,                                 noop_main_callback, default_moby_methods},
        { 0x56,                   update_class_55_swarm_controller, default_moby_methods},
        { 0x70,                               update_glow_particle, default_moby_methods},
        { 0x79,                                        bomb_update, default_moby_methods},
        { 0x7a,                                      debris_update, default_moby_methods},
        { 0x95,                     nanotech_crate_fragment_update, default_moby_methods},
        { 0x99,                     gold_devastator_missile_update, default_moby_methods},
        { 0x9d,                                  devastator_update, default_moby_methods},
        { 0xa1,              update_moby_random_interval_animation, default_moby_methods},
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
        { 0xb8,                                   FUN_L04_002af058, default_moby_methods},
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
        { 0xd9,                                  big_axebot_update, default_moby_methods},
        { 0xde,                             devestator_ammo_update, default_moby_methods},
        { 0xdf,                             devestator_ammo_update, default_moby_methods},
        { 0xe1,                             devestator_ammo_update, default_moby_methods},
        { 0xe2,                             devestator_ammo_update, default_moby_methods},
        { 0xe5,                               glove_of_doom_update, default_moby_methods},
        { 0xe6,                                   FUN_L00_002c89e8, default_moby_methods},
        {0x10e,                           update_morphoray_chicken,       D_L04_001DD104},
        {0x118,                                   FUN_L01_002e0c68, default_moby_methods},
        {0x131,                                 update_homing_shot, default_moby_methods},
        {0x154,                          circular_saw_enemy_update,       D_L04_001DD11C},
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
        {0x1ab,                               blarg_trooper_update, default_moby_methods},
        {0x1ac,                         update_damped_falling_moby, default_moby_methods},
        {0x1b0,                                   FUN_L04_002c6858, default_moby_methods},
        {0x1b1,                                 noop_main_callback, default_moby_methods},
        {0x1b2,                            platform_booster_update, default_moby_methods},
        {0x1b5,                                  sand_mouse_update, default_moby_methods},
        {0x1c6,                                   FUN_L00_002cc608, default_moby_methods},
        {0x1c9,                                      rocket_update, default_moby_methods},
        {0x1d2,                                   snow_plow_update, default_moby_methods},
        {0x1df,                                       update_drone, default_moby_methods},
        {0x1e0,                                   snow_plow_update, default_moby_methods},
        {0x1e1,                           floating_platform_update, default_moby_methods},
        {0x1e3,                                                  0, default_moby_methods},
        {0x1e4,                              transport_ship_update, default_moby_methods},
        {0x1e5,                                   snow_plow_update, default_moby_methods},
        {0x1e6,                                   snow_plow_update, default_moby_methods},
        {0x1e8,                                   snow_plow_update, default_moby_methods},
        {0x1ea,                                   snow_plow_update, default_moby_methods},
        {0x1ed,                                   snow_plow_update, default_moby_methods},
        {0x1ee,                                   snow_plow_update, default_moby_methods},
        {0x1ef,                                   snow_plow_update, default_moby_methods},
        {0x1f2,                                   snow_plow_update, default_moby_methods},
        {0x1f3,                                 noop_main_callback, default_moby_methods},
        {0x1f4,                                       crate_update, default_moby_methods},
        {0x1f5,                                       crate_update, default_moby_methods},
        {0x1f9,                                       crate_update, default_moby_methods},
        {0x1ff,                                       crate_update, default_moby_methods},
        {0x22b,                                   snow_plow_update, default_moby_methods},
        {0x232,                                 decoy_glove_update, default_moby_methods},
        {0x233,                                 woodcut_bot_update, default_moby_methods},
        {0x23a,                                 noop_main_callback, default_moby_methods},
        {0x248,               update_moby_translation_delta_matrix, default_moby_methods},
        {0x249,                              metal_detector_update, default_moby_methods},
        {0x25c,                             sandmouse_house_update, default_moby_methods},
        {0x25d,                 update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                                    helipack_update, default_moby_methods},
        {0x267,                             tresspasser_pad_update, default_moby_methods},
        {0x269, interpolate_moby_and_linked_moby_positions_on_path, default_moby_methods},
        {0x26a,                                 noop_main_callback, default_moby_methods},
        {0x27a,                                                  0, default_moby_methods},
        {0x27f,                   update_animated_point_light_moby, default_moby_methods},
        {0x282,                                        leaf_update, default_moby_methods},
        {0x2ee,                                     infobot_update, default_moby_methods},
        {0x2f6,                       swingshot_versatarget_update, default_moby_methods},
        {0x307,                           grow_and_fade_out_update, default_moby_methods},
        {0x323,                       swingshot_versatarget_update, default_moby_methods},
        {0x324,          update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                                 spawn_point_update, default_moby_methods},
        {0x326,                               nanotech_ball_update, default_moby_methods},
        {0x340,                 update_target_outside_zone_timeout, default_moby_methods},
        {0x351,                                  suckcannon_update, default_moby_methods},
        {0x3ee,                             devestator_ammo_update, default_moby_methods},
        {0x41c,                                   FUN_L04_002c6858, default_moby_methods},
        {0x44d,                                  upper_door_update, default_moby_methods},
        {0x44e,                                  upper_door_update, default_moby_methods},
        {0x460,                                 suck_cannon_update, default_moby_methods},
        {0x46e,                                   gold_bolt_update, default_moby_methods},
        {0x472,                                   quad_moby_update, default_moby_methods},
        {0x477,                                  vendor_orb_update, default_moby_methods},
        {0x4a6,                                  lieutenant_update, default_moby_methods},
        {0x4a8,                               update_glow_particle, default_moby_methods},
        {0x4e3,                              hydrodisplacer_update, default_moby_methods},
        {0x509,                                 noop_main_callback, default_moby_methods},
        {0x50a,                               pilots_helmet_update, default_moby_methods},
        {0x53f,             update_eudora_tutorial_hints_and_stats, default_moby_methods},
        {0x59e,                             devestator_ammo_update, default_moby_methods},
        {0x5a7,                             devestator_ammo_update, default_moby_methods},
        {0x5a9,                             devestator_ammo_update, default_moby_methods},
        {0x5ec,                     update_entity_bounce_and_decay, default_moby_methods},
        {0x5fb,                                  upper_door_update, default_moby_methods},
        {0x5fc,                                  upper_door_update, default_moby_methods},
        {0x60d,                        dispatch_mode_selected_slot, default_moby_methods},
        {0x63f,                                   robot_arm_update, default_moby_methods},
        {0x641,                                   robot_arm_update, default_moby_methods},
        {0x642,                                   robot_arm_update, default_moby_methods},
        {0x643,                                   robot_arm_update, default_moby_methods},
        {0x644,                                   robot_arm_update, default_moby_methods},
        {0x645,                                   robot_arm_update, default_moby_methods},
        {0x646,                                   robot_arm_update, default_moby_methods},
        {0x647,                                   robot_arm_update, default_moby_methods},
        {0x648,                                   robot_arm_update, default_moby_methods},
        {0x649,                                   robot_arm_update, default_moby_methods},
        {0x64d,                                   robot_arm_update, default_moby_methods},
        {0x64e,                                   robot_arm_update, default_moby_methods},
        {0x64f,                                   robot_arm_update, default_moby_methods},
        {0x650,                                   robot_arm_update, default_moby_methods},
        {0x651,                                   robot_arm_update, default_moby_methods},
        {0x652,                                   robot_arm_update, default_moby_methods},
        {0x653,                                   robot_arm_update, default_moby_methods},
        {0x654,                                   robot_arm_update, default_moby_methods},
        {0x658,                                   robot_arm_update, default_moby_methods},
        {0x661,                        update_sandmouse_projectile, default_moby_methods},
        {0x6cb,                                   robot_arm_update, default_moby_methods},
        {0x6cc,                                   robot_arm_update, default_moby_methods},
        {0x6cd,                                   robot_arm_update, default_moby_methods},
        {0x71a,                                   FUN_L01_0030df40, default_moby_methods},
        {0x72a,                                        pipe_update, default_moby_methods},
        {0x72b,                                 noop_main_callback, default_moby_methods},
        {0x72c,                                   robot_arm_update, default_moby_methods},
        {0x76c,                                       decoy_update, default_moby_methods},
        {0x793,                                   robot_arm_update, default_moby_methods},
        {0x794,                                   robot_arm_update, default_moby_methods},
        {0x795,                                   robot_arm_update, default_moby_methods},
        {0x796,                                   robot_arm_update, default_moby_methods},
        {0x798,                                   robot_arm_update, default_moby_methods},
        {0x799,                                   robot_arm_update, default_moby_methods},
        {0x79a,                                   robot_arm_update, default_moby_methods},
        {   -1,                                                  0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L04_001DD180") __attribute__((section("lvl.camvtbl"))) = {
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
    {17,           FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {23,        volume_camera_check,                noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {-1,                          0,                                 0,                    0,                                                  0},
};

struct SoundHandler sound_handlers[2] __asm__("D_L04_001DD280")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 6, update_music_box_exit_trigger},
        {-1,                             0},
};
