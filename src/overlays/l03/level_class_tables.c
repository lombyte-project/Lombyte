#include "rnc/mobys.h"
#include "rnc/overlay/l03.h"
#include "rnc/overlay/l03/mobys.h"
#include "rnc/overlay/l03/mobys/homing_rocket.h"
#include "rnc/overlay/l03/mobys/robot_dog.h"
#include "rnc/overlay/l03/mobys/taxi.h"
#include "rnc/overlay/shared/audio/emitters.h"
#include "rnc/overlay/shared/camera.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/swing_volume_camera.h"
#include "rnc/overlay/shared/cutscenes.h"
#include "rnc/overlay/shared/enemies/blarg_helicopter.h"
#include "rnc/overlay/shared/enemies.h"
#include "rnc/overlay/shared/mobys.h"
#include "rnc/overlay/shared/pickups.h"
#include "rnc/overlay/shared/pickups/infobot.h"
#include "rnc/overlay/shared/props.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/overlay/shared/props/rock.h"
#include "rnc/overlay/shared/rendering/effects.h"
#include "rnc/overlay/shared/rendering/effects/explosion_light.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"
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
void cutscene_trigger_update() __asm__("FUN_L01_002fb5b0");
void gunship_update() __asm__("FUN_L01_002f5168");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L03_001E40FC")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L03_001E4114[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[181] __asm__("D_L03_001E3880")

    __attribute__((section("lvl.vtbl"))) = {
        { 726,            novalis_elevator_update, default_moby_methods},
        { 822, metropolis_train_locomotive_update, default_moby_methods},
        { 868,       moving_platform_small_update, default_moby_methods},
        {1007,               camera_object_update, default_moby_methods},
        {1210,   metropolis_train_carriage_update, default_moby_methods},
        {  11,                      vendor_update, default_moby_methods},
        {  13,                        bolt_update, default_moby_methods},
        {  14,                        bolt_update, default_moby_methods},
        {  15,                        bolt_update, default_moby_methods},
        {  16,                        bolt_update, default_moby_methods},
        {  71,                      wrench_update, default_moby_methods},
        {  74,                        mine_update, default_moby_methods},
        {  75,                  red_ship_1_update, default_moby_methods},
        { 112,               glow_particle_update, default_moby_methods},
        { 115,                  red_ship_1_update, default_moby_methods},
        { 116,                  red_ship_1_update, default_moby_methods},
        { 117,                  red_ship_1_update, default_moby_methods},
        { 118,                  red_ship_1_update, default_moby_methods},
        { 119,                  red_ship_1_update, default_moby_methods},
        { 120,                  red_ship_1_update, default_moby_methods},
        { 121,                        bomb_update, default_moby_methods},
        { 122,                      debris_update, default_moby_methods},
        { 132,                  red_ship_1_update, default_moby_methods},
        { 149,     nanotech_crate_fragment_update, default_moby_methods},
        { 153,     gold_devastator_missile_update, default_moby_methods},
        { 157,                  devastator_update, default_moby_methods},
        { 163,                    visibomb_update, default_moby_methods},
        { 166,          linked_moby_states_update, default_moby_methods},
        { 167,         thruster_pack_flame_update, default_moby_methods},
        { 168,                     blaster_update, default_moby_methods},
        { 172,            visibomb_missile_update, default_moby_methods},
        { 175,                     taunter_update, default_moby_methods},
        { 176,                   pyrocitor_update, default_moby_methods},
        { 177,                  tesla_claw_update, default_moby_methods},
        { 179,              trailing_flame_update, default_moby_methods},
        { 180,                    walloper_update, default_moby_methods},
        { 185,                   morphoray_update, default_moby_methods},
        { 186,                    doom_bot_update, default_moby_methods},
        { 188,                  trespasser_update, default_moby_methods},
        { 190,                  mine_glove_update, default_moby_methods},
        { 192,                  bomb_glove_update, default_moby_methods},
        { 203,                       decoy_update, default_moby_methods},
        { 204,             devestator_ammo_update, default_moby_methods},
        { 208,                   swingshot_update, default_moby_methods},
        { 209,              swingshot_hook_update, default_moby_methods},
        { 213,             devestator_ammo_update, default_moby_methods},
        { 214,             devestator_ammo_update, default_moby_methods},
        { 222,             devestator_ammo_update, default_moby_methods},
        { 223,             devestator_ammo_update, default_moby_methods},
        { 225,             devestator_ammo_update, default_moby_methods},
        { 226,             devestator_ammo_update, default_moby_methods},
        { 229,               glove_of_doom_update, default_moby_methods},
        { 230,            doom_bot_spawner_update, default_moby_methods},
        { 235,     parent_following_effect_update, default_moby_methods},
        { 270,           morphoray_chicken_update,       D_L03_001E4114},
        { 305,                 homing_shot_update, default_moby_methods},
        { 348,     nanotech_crate_fragment_update, default_moby_methods},
        { 349,     nanotech_crate_fragment_update, default_moby_methods},
        { 354,     nanotech_crate_fragment_update, default_moby_methods},
        { 355,     nanotech_crate_fragment_update, default_moby_methods},
        { 356,     nanotech_crate_fragment_update, default_moby_methods},
        { 357,     nanotech_crate_fragment_update, default_moby_methods},
        { 358,     nanotech_crate_fragment_update, default_moby_methods},
        { 359,     nanotech_crate_fragment_update, default_moby_methods},
        { 363,     nanotech_crate_fragment_update, default_moby_methods},
        { 364,     nanotech_crate_fragment_update, default_moby_methods},
        { 428,         damped_falling_moby_update, default_moby_methods},
        { 433,                 noop_main_callback, default_moby_methods},
        { 454,                        ryno_update, default_moby_methods},
        { 455,                bomb_spawner_update, default_moby_methods},
        { 457,                 ryno_rocket_update, default_moby_methods},
        { 479,                       drone_update, default_moby_methods},
        { 483,                                  0, default_moby_methods},
        { 500,                       crate_update, default_moby_methods},
        { 501,                       crate_update, default_moby_methods},
        { 505,                       crate_update, default_moby_methods},
        { 511,                       crate_update, default_moby_methods},
        { 545,           bomb_spawner_bomb_update, default_moby_methods},
        { 562,                 decoy_glove_update, default_moby_methods},
        { 573,                   robot_dog_update, default_moby_methods},
        { 574,           kerwan_path_enemy_update, default_moby_methods},
        { 578,                  mine_layer_update, default_moby_methods},
        { 585,              metal_detector_update, default_moby_methods},
        { 604,             sandmouse_house_update, default_moby_methods},
        { 605,                buried_bolts_update, default_moby_methods},
        { 607,                    helipack_update, default_moby_methods},
        { 618,                 noop_main_callback, default_moby_methods},
        { 627,             mine_layer_mine_update, default_moby_methods},
        { 631,            blarg_helicopter_update, default_moby_methods},
        { 634,                                  0, default_moby_methods},
        { 639,             explosion_light_update, default_moby_methods},
        { 737,            cutscene_trigger_update, default_moby_methods},
        { 750,                     infobot_update, default_moby_methods},
        { 758,       swingshot_versatarget_update, default_moby_methods},
        { 775,           grow_and_fade_out_update, default_moby_methods},
        { 795,                  red_ship_1_update, default_moby_methods},
        { 803,       swingshot_versatarget_update, default_moby_methods},
        { 804,     morphoray_target_effect_update, default_moby_methods},
        { 805,                 spawn_point_update, default_moby_methods},
        { 806,               nanotech_ball_update, default_moby_methods},
        { 816,                   rail_taxi_update, default_moby_methods},
        { 817,                      statue_update, default_moby_methods},
        { 825,             robo_shack_sign_update, default_moby_methods},
        { 832,         visibomb_zone_timer_update, default_moby_methods},
        { 833,               homing_rocket_update, default_moby_methods},
        { 845,                     gunship_update, default_moby_methods},
        { 848,            helicopter_spark_update, default_moby_methods},
        { 849,                  suckcannon_update, default_moby_methods},
        { 890,                       helga_update, default_moby_methods},
        { 898,                  path_mover_update, default_moby_methods},
        { 899,          path_mover_spawner_update, default_moby_methods},
        { 905,       moving_platform_small_update, default_moby_methods},
        { 909,                      big_al_update, default_moby_methods},
        { 914,                 robot_qwark_update, default_moby_methods},
        { 915,                      fred_1_update, default_moby_methods},
        { 916,                      fred_1_update, default_moby_methods},
        { 917,                      fred_1_update, default_moby_methods},
        { 920,                qwark_rilgar_update, default_moby_methods},
        { 928,       moving_platform_small_update, default_moby_methods},
        { 994,          moby_draw_callback_update, default_moby_methods},
        { 997,                 garage_door_update, default_moby_methods},
        {1006,             devestator_ammo_update, default_moby_methods},
        {1012,                   rail_lift_update, default_moby_methods},
        {1134,                   gold_bolt_update, default_moby_methods},
        {1138,                   quad_moby_update, default_moby_methods},
        {1143,                  vendor_orb_update, default_moby_methods},
        {1192,               glow_particle_update, default_moby_methods},
        {1221,                falling_moby_update, default_moby_methods},
        {1222,                falling_moby_update, default_moby_methods},
        {1251,              hydrodisplacer_update, default_moby_methods},
        {1289,                 noop_main_callback, default_moby_methods},
        {1290,               pilots_helmet_update, default_moby_methods},
        {1342,                  help_hints_update, default_moby_methods},
        {1438,             devestator_ammo_update, default_moby_methods},
        {1447,             devestator_ammo_update, default_moby_methods},
        {1449,             devestator_ammo_update, default_moby_methods},
        {1548,     kerwan_cutscene_effects_update, default_moby_methods},
        {1591,               tumbling_part_update, default_moby_methods},
        {1592,               tumbling_part_update, default_moby_methods},
        {1593,               tumbling_part_update, default_moby_methods},
        {1633,        sandmouse_projectile_update, default_moby_methods},
        {1705,               tumbling_part_update, default_moby_methods},
        {1706,               tumbling_part_update, default_moby_methods},
        {1707,               tumbling_part_update, default_moby_methods},
        {1708,               tumbling_part_update, default_moby_methods},
        {1709,               tumbling_part_update, default_moby_methods},
        {1710,               tumbling_part_update, default_moby_methods},
        {1711,               tumbling_part_update, default_moby_methods},
        {1712,               tumbling_part_update, default_moby_methods},
        {1713,               tumbling_part_update, default_moby_methods},
        {1714,               tumbling_part_update, default_moby_methods},
        {1715,               tumbling_part_update, default_moby_methods},
        {1716,               tumbling_part_update, default_moby_methods},
        {1717,               tumbling_part_update, default_moby_methods},
        {1718,               tumbling_part_update, default_moby_methods},
        {1719,               tumbling_part_update, default_moby_methods},
        {1720,               tumbling_part_update, default_moby_methods},
        {1721,               tumbling_part_update, default_moby_methods},
        {1722,               tumbling_part_update, default_moby_methods},
        {1724,               tumbling_part_update, default_moby_methods},
        {1726,               tumbling_part_update, default_moby_methods},
        {1727,               tumbling_part_update, default_moby_methods},
        {1733,               tumbling_part_update, default_moby_methods},
        {1734,               tumbling_part_update, default_moby_methods},
        {1735,               tumbling_part_update, default_moby_methods},
        {1742,               tumbling_part_update, default_moby_methods},
        {1743,               tumbling_part_update, default_moby_methods},
        {1744,               tumbling_part_update, default_moby_methods},
        {1745,               tumbling_part_update, default_moby_methods},
        {1746,               tumbling_part_update, default_moby_methods},
        {1818,                   sandmouse_update, default_moby_methods},
        {1827,                   lightpost_update, default_moby_methods},
        {1828,                 noop_main_callback, default_moby_methods},
        {1829,               tumbling_part_update, default_moby_methods},
        {1830,               tumbling_part_update, default_moby_methods},
        {1831,               tumbling_part_update, default_moby_methods},
        {1832,               tumbling_part_update, default_moby_methods},
        {1833,               tumbling_part_update, default_moby_methods},
        {1900,                       decoy_update, default_moby_methods},
        {1917,               tumbling_part_update, default_moby_methods},
        {  -1,                                  0, default_moby_methods},
};

struct CameraType level_camera_types[10] __asm__("D_L03_001E4180") __attribute__((section("lvl.camvtbl"))) = {
    { 0,    noop_callback_l,                 follow_camera_setup,follow_camera_update,                                 noop_main_callback                                                                         },
    { 1,
     evaluate_clip_volume_candidate,                  hud_counter_update,
     hud_counter_update, set_transform_state_three_when_outside_clip_volume                                                                       },
    { 4,
     camera_transition_update, initialize_hero_aligned_camera_mode,
     update_hero_aligned_camera_from_input,        select_camera_mode_by_hero_vector_alignment                                                    },
    { 5,
     noop_callback_l,      initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                                 noop_main_callback                                               },
    { 6,            noop_callback_l,   initialize_orbit_camera_from_hero,   hud_counter_update,                                 noop_main_callback},
    { 7,
     noop_callback_l,   initialize_target_tracking_camera,
     update_target_tracking_camera,               conditionally_set_camera_state_three                                                            },
    {14,
     FUN_L03_002ebd70,     initialize_hero_relative_camera,
     update_camera_position_and_orientation,            update_camera_state_outside_clip_volume                                                   },
    {17,  volume_camera_swing_check,                  noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {18, update_moby_camera_control,                  noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {-1,                          0,                                   0,                    0,                                                  0},
};

struct SoundHandler sound_handlers[4] __asm__("D_L03_001E4280")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 3,                 update_reverb_box_emitter},
        { 6,               update_music_switch_emitter},
        {-1,                                         0},
};
