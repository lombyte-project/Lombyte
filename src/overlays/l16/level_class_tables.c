#include "rnc/mobys.h"
#include "rnc/overlay/l16/mobys.h"
#include "rnc/overlay/l16/mobys/door.h"
#include "rnc/overlay/l16/mobys/reflection_mesh_trio.h"
#include "rnc/overlay/l16/mobys/test_dummy.h"
#include "rnc/overlay/l16/mobys/test_dummy_door.h"
#include "rnc/overlay/shared/audio/emitters.h"
#include "rnc/overlay/shared/camera.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/enemies.h"
#include "rnc/overlay/shared/enemies/blarg_elite_commando_rocket.h"
#include "rnc/overlay/shared/mobys.h"
#include "rnc/overlay/shared/pickups.h"
#include "rnc/overlay/shared/props.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/overlay/shared/props/rock.h"
#include "rnc/overlay/shared/rendering/effects.h"
#include "rnc/overlay/shared/rendering/effects/explosion_light.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"
#include "rnc/overlay/shared/vehicles.h"
#include "rnc/overlay/shared/vendor.h"
#include "rnc/overlay/shared/weapons.h"
#include "rnc/overlay/shared/weapons/bomb_glove_bomb.h"
#include "rnc/overlay/shared/weapons/devastator.h"
#include "rnc/overlay/shared/weapons/mine.h"
#include "rnc/overlay/shared/weapons/suckcannon.h"
#include "rnc/overlay/shared/weapons/swingshot_hook.h"
/* The level's moby classes, camera types and sound emitter types: the tables
   the executable's code looks ids up in (sections lvl.vtbl, lvl.camvtbl and
   lvl.sndvtbl). */
#include "types.h"
#include "rnc/audio/sound_handlers.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby_class_tables.h"

void bolt_update() __asm__("FUN_L00_002a5dd8");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void refresh_point_light() __asm__("FUN_00201f58");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L16_001E9020")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L16_001E9038[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

void *D_L16_001E9050[6] __attribute__((section("lvl.vtbl"))) = {moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                run_moby_helper_and_set_state_7,
                                                                get_moby_data_plus_0x60,
                                                                hud_counter_update};

struct MobyClassUpdate moby_classes[152] __asm__("D_L16_001E8900")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,                camera_object_update, default_moby_methods},
        {  11,                       vendor_update, default_moby_methods},
        {  13,                         bolt_update, default_moby_methods},
        {  14,                         bolt_update, default_moby_methods},
        {  15,                         bolt_update, default_moby_methods},
        {  16,                         bolt_update, default_moby_methods},
        {  49,  blarg_elite_commando_rocket_update, default_moby_methods},
        {  50,       armed_transport_rocket_update, default_moby_methods},
        {  52,                       seeker_update, default_moby_methods},
        {  71,                       wrench_update, default_moby_methods},
        {  74,                         mine_update, default_moby_methods},
        { 112,                glow_particle_update, default_moby_methods},
        { 121,                         bomb_update, default_moby_methods},
        { 122,                       debris_update, default_moby_methods},
        { 149,      nanotech_crate_fragment_update, default_moby_methods},
        { 153,      gold_devastator_missile_update, default_moby_methods},
        { 157,                   devastator_update, default_moby_methods},
        { 163,                     visibomb_update, default_moby_methods},
        { 166,           linked_moby_states_update, default_moby_methods},
        { 167,          thruster_pack_flame_update, default_moby_methods},
        { 168,                      blaster_update, default_moby_methods},
        { 172,             visibomb_missile_update, default_moby_methods},
        { 175,                      taunter_update, default_moby_methods},
        { 176,                    pyrocitor_update, default_moby_methods},
        { 177,                   tesla_claw_update, default_moby_methods},
        { 179,               trailing_flame_update, default_moby_methods},
        { 180,                     walloper_update, default_moby_methods},
        { 185,                    morphoray_update, default_moby_methods},
        { 186,                     doom_bot_update, default_moby_methods},
        { 188,                   trespasser_update, default_moby_methods},
        { 190,                   mine_glove_update, default_moby_methods},
        { 192,                   bomb_glove_update, default_moby_methods},
        { 203,                        decoy_update, default_moby_methods},
        { 204,              devestator_ammo_update, default_moby_methods},
        { 208,                    swingshot_update, default_moby_methods},
        { 209,               swingshot_hook_update, default_moby_methods},
        { 213,              devestator_ammo_update, default_moby_methods},
        { 214,              devestator_ammo_update, default_moby_methods},
        { 222,              devestator_ammo_update, default_moby_methods},
        { 223,              devestator_ammo_update, default_moby_methods},
        { 225,              devestator_ammo_update, default_moby_methods},
        { 226,              devestator_ammo_update, default_moby_methods},
        { 229,                glove_of_doom_update, default_moby_methods},
        { 230,             doom_bot_spawner_update, default_moby_methods},
        { 235,      parent_following_effect_update, default_moby_methods},
        { 257,        path_following_rocket_update, default_moby_methods},
        { 270,            morphoray_chicken_update,       D_L16_001E9038},
        { 281,                  thrown_bomb_update, default_moby_methods},
        { 305,                  homing_shot_update, default_moby_methods},
        { 348,      nanotech_crate_fragment_update, default_moby_methods},
        { 349,      nanotech_crate_fragment_update, default_moby_methods},
        { 354,      nanotech_crate_fragment_update, default_moby_methods},
        { 355,      nanotech_crate_fragment_update, default_moby_methods},
        { 356,      nanotech_crate_fragment_update, default_moby_methods},
        { 357,      nanotech_crate_fragment_update, default_moby_methods},
        { 358,      nanotech_crate_fragment_update, default_moby_methods},
        { 359,      nanotech_crate_fragment_update, default_moby_methods},
        { 363,      nanotech_crate_fragment_update, default_moby_methods},
        { 364,      nanotech_crate_fragment_update, default_moby_methods},
        { 428,          damped_falling_moby_update, default_moby_methods},
        { 433,                  noop_main_callback, default_moby_methods},
        { 439,                   hoverboard_update, default_moby_methods},
        { 454,                         ryno_update, default_moby_methods},
        { 457,                  ryno_rocket_update, default_moby_methods},
        { 470,       spinning_sprite_effect_update, default_moby_methods},
        { 471,                conveyor_belt_update, default_moby_methods},
        { 479,                        drone_update, default_moby_methods},
        { 482,                  hud_counter_update, default_moby_methods},
        { 483,                                   0, default_moby_methods},
        { 500,                        crate_update, default_moby_methods},
        { 501,                        crate_update, default_moby_methods},
        { 502,                        crate_update, default_moby_methods},
        { 505,                        crate_update, default_moby_methods},
        { 511,                        crate_update, default_moby_methods},
        { 541,                   test_dummy_update, default_moby_methods},
        { 546,                  wall_button_update, default_moby_methods},
        { 552,          linked_pair_ribbons_update, default_moby_methods},
        { 556,     test_dummy_on_hoverboard_update, default_moby_methods},
        { 562,                  decoy_glove_update, default_moby_methods},
        { 585,               metal_detector_update, default_moby_methods},
        { 605,                 buried_bolts_update, default_moby_methods},
        { 607,                     helipack_update, default_moby_methods},
        { 618,                  noop_main_callback, default_moby_methods},
        { 634,                                   0, default_moby_methods},
        { 638,         blarg_elite_commando_update, default_moby_methods},
        { 639,              explosion_light_update, default_moby_methods},
        { 647,                         door_update, default_moby_methods},
        { 650,              test_dummy_door_update, default_moby_methods},
        { 654,         test_dummy_challenge_update, default_moby_methods},
        { 758,        swingshot_versatarget_update, default_moby_methods},
        { 775,            grow_and_fade_out_update, default_moby_methods},
        { 800,               breakable_prop_update, default_moby_methods},
        { 803,        swingshot_versatarget_update, default_moby_methods},
        { 804,      morphoray_target_effect_update, default_moby_methods},
        { 805,                  spawn_point_update, default_moby_methods},
        { 806,                nanotech_ball_update, default_moby_methods},
        { 849,                   suckcannon_update, default_moby_methods},
        { 876,                  noop_main_callback, default_moby_methods},
        { 933,              bolt_thief_mine_update, default_moby_methods},
        {1006,              devestator_ammo_update, default_moby_methods},
        {1111,               ocean_pokitaru_update, default_moby_methods},
        {1134,                    gold_bolt_update, default_moby_methods},
        {1135,                   teleporter_update, default_moby_methods},
        {1138,                    quad_moby_update, default_moby_methods},
        {1139,              hoverboard_ring_update, default_moby_methods},
        {1140,                  noop_main_callback, default_moby_methods},
        {1143,                   vendor_orb_update, default_moby_methods},
        {1192,                glow_particle_update, default_moby_methods},
        {1251,               hydrodisplacer_update, default_moby_methods},
        {1289,                  noop_main_callback, default_moby_methods},
        {1290,                pilots_helmet_update, default_moby_methods},
        {1356,              armed_transport_update, default_moby_methods},
        {1377,               help_desk_lady_update, default_moby_methods},
        {1387,          wall_button_blocker_update, default_moby_methods},
        {1401,                   bolt_thief_update, default_moby_methods},
        {1410,                         taxi_update, default_moby_methods},
        {1438,              devestator_ammo_update, default_moby_methods},
        {1439,               grindrail_gate_update, default_moby_methods},
        {1441, local_bounds_command_trigger_update, default_moby_methods},
        {1442,             grindrail_button_update, default_moby_methods},
        {1443,            grindrail_barrier_update, default_moby_methods},
        {1445,                      chomper_update,       D_L16_001E9050},
        {1447,              devestator_ammo_update, default_moby_methods},
        {1449,              devestator_ammo_update, default_moby_methods},
        {1455,                gadgetron_ceo_update, default_moby_methods},
        {1475,            homing_projectile_update, default_moby_methods},
        {1561, clip_volume_dialogue_trigger_update, default_moby_methods},
        {1588,                tumbling_part_update, default_moby_methods},
        {1589,                tumbling_part_update, default_moby_methods},
        {1590,                tumbling_part_update, default_moby_methods},
        {1621,                tumbling_part_update, default_moby_methods},
        {1622,                tumbling_part_update, default_moby_methods},
        {1623,                tumbling_part_update, default_moby_methods},
        {1667,                  path_walker_update, default_moby_methods},
        {1668,                  path_walker_update, default_moby_methods},
        {1669,                  path_walker_update, default_moby_methods},
        {1670,                  path_walker_update, default_moby_methods},
        {1671,                  path_walker_update, default_moby_methods},
        {1786,                tumbling_part_update, default_moby_methods},
        {1787,                tumbling_part_update, default_moby_methods},
        {1812,        cutscene_joint_trails_update, default_moby_methods},
        {1826,                     elevator_update, default_moby_methods},
        {1870,                tumbling_part_update, default_moby_methods},
        {1891,               gadgetron_sign_update, default_moby_methods},
        {1900,                        decoy_update, default_moby_methods},
        {1923,            chicken_relocator_update, default_moby_methods},
        {1943,         reflection_mesh_trio_update, default_moby_methods},
        {1947,              reflection_mesh_update, default_moby_methods},
        {1948,         reflection_mesh_pair_update, default_moby_methods},
        {1952,               draw_only_moby_update, default_moby_methods},
        {1953,       reflection_mesh_trio_b_update, default_moby_methods},
        {  -1,                                   0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L16_001E9080") __attribute__((section("lvl.camvtbl"))) = {
    { 0,noop_callback_l,                   follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                     },
    { 3,
     noop_callback_l, initialize_camera_transform_and_paths,
     update_camera_position_and_orientation_from_paths,              update_camera_mode_from_preset                                    },
    { 4,
     camera_transition_update,   initialize_hero_aligned_camera_mode,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                },
    { 5,
     noop_callback_l,        initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                           },
    { 6,      noop_callback_l,     initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l,     initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                        },
    { 8,
     noop_callback_l,    reset_camera_from_source_transform,
     refresh_point_light,                          noop_main_callback                                                                  },
    {23,  volume_camera_check,                    noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                    0,                                     0,                    0,                                           0},
};

struct SoundHandler sound_handlers[4] __asm__("D_L16_001E9180")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 1,          update_box_emitter},
        { 3,   update_reverb_box_emitter},
        { 6, update_music_switch_emitter},
        {-1,                           0},
};
