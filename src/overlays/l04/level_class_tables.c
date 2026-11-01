#include "rnc/mobys/quad_moby_update.h"
#include "rnc/overlay/l04/mobys/big_axebot.h"
#include "rnc/overlay/l04/mobys/blarg_trooper.h"
#include "rnc/overlay/l04/mobys/butterfly_swarm.h"
#include "rnc/overlay/l04/mobys/circular_saw_enemy.h"
#include "rnc/overlay/l04/mobys/cutscene_camera_target.h"
#include "rnc/overlay/l04/mobys/eudora_bridge.h"
#include "rnc/overlay/l04/mobys/floating_platform.h"
#include "rnc/overlay/l04/mobys/help_hints.h"
#include "rnc/overlay/l04/mobys/leaf.h"
#include "rnc/overlay/l04/mobys/lieutenant.h"
#include "rnc/overlay/l04/mobys/path_progress_mover.h"
#include "rnc/overlay/l04/mobys/pipe.h"
#include "rnc/overlay/l04/mobys/platform_booster.h"
#include "rnc/overlay/l04/mobys/random_interval_animation.h"
#include "rnc/overlay/l04/mobys/snow_plow.h"
#include "rnc/overlay/l04/mobys/suck_cannon.h"
#include "rnc/overlay/l04/mobys/translation_delta_matrix.h"
#include "rnc/overlay/l04/mobys/transport_ship.h"
#include "rnc/overlay/l04/mobys/upper_door.h"
#include "rnc/overlay/l04/mobys/woodcut_bot.h"
#include "rnc/overlay/l04/mobys/woodcut_bot_debris.h"
#include "rnc/overlay/shared/audio/emitters/music_switch_emitter.h"
#include "rnc/overlay/shared/camera/aim_volume_camera.h"
#include "rnc/overlay/shared/camera/clip_volume_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_setup.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/camera/hero_aligned_camera.h"
#include "rnc/overlay/shared/camera/orbit_camera.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/swing_volume_camera.h"
#include "rnc/overlay/shared/camera/target_tracking_camera.h"
#include "rnc/overlay/shared/enemies/blarg_trooper_projectile.h"
#include "rnc/overlay/shared/enemies/horny_toad.h"
#include "rnc/overlay/shared/enemies/sand_mouse.h"
#include "rnc/overlay/shared/enemies/sandmouse.h"
#include "rnc/overlay/shared/enemies/sandmouse_house.h"
#include "rnc/overlay/shared/enemies/sandmouse_projectile.h"
#include "rnc/overlay/shared/mobys/camera_object.h"
#include "rnc/overlay/shared/mobys/suck_cannon_methods.h"
#include "rnc/overlay/shared/pickups/buried_bolts.h"
#include "rnc/overlay/shared/pickups/crate.h"
#include "rnc/overlay/shared/pickups/crate_fragment.h"
#include "rnc/overlay/shared/pickups/infobot.h"
#include "rnc/overlay/shared/pickups/nanotech_ball.h"
#include "rnc/overlay/shared/pickups/pilots_helmet.h"
#include "rnc/overlay/shared/pickups/weapon_ammo.h"
#include "rnc/overlay/shared/props/bolt_crank.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/overlay/shared/props/hydrodisplacer.h"
#include "rnc/overlay/shared/props/rock.h"
#include "rnc/overlay/shared/props/swingshot_versatarget.h"
#include "rnc/overlay/shared/props/trespasser_pad.h"
#include "rnc/overlay/shared/props/tumbling_part.h"
#include "rnc/overlay/shared/rendering/effects/debris.h"
#include "rnc/overlay/shared/rendering/effects/explosion_light.h"
#include "rnc/overlay/shared/rendering/effects/glow_particle.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"
#include "rnc/overlay/shared/vendor/vendor.h"
#include "rnc/overlay/shared/vendor/vendor_orb.h"
#include "rnc/overlay/shared/weapons/blaster.h"
#include "rnc/overlay/shared/weapons/blaster_shot.h"
#include "rnc/overlay/shared/weapons/bomb_glove.h"
#include "rnc/overlay/shared/weapons/bomb_glove_bomb.h"
#include "rnc/overlay/shared/weapons/decoy.h"
#include "rnc/overlay/shared/weapons/decoy_glove.h"
#include "rnc/overlay/shared/weapons/devastator.h"
#include "rnc/overlay/shared/weapons/doom_bot.h"
#include "rnc/overlay/shared/weapons/drone.h"
#include "rnc/overlay/shared/weapons/glove_of_doom.h"
#include "rnc/overlay/shared/weapons/glove_of_doom_bomb.h"
#include "rnc/overlay/shared/weapons/gold_devastator_missile.h"
#include "rnc/overlay/shared/weapons/helipack.h"
#include "rnc/overlay/shared/weapons/linked_moby_states_update.h"
#include "rnc/overlay/shared/weapons/metal_detector.h"
#include "rnc/overlay/shared/weapons/mine.h"
#include "rnc/overlay/shared/weapons/mine_glove.h"
#include "rnc/overlay/shared/weapons/morphoray.h"
#include "rnc/overlay/shared/weapons/morphoray_chicken.h"
#include "rnc/overlay/shared/weapons/morphoray_chicken_feather.h"
#include "rnc/overlay/shared/weapons/morphoray_target_effect.h"
#include "rnc/overlay/shared/weapons/pyrocitor.h"
#include "rnc/overlay/shared/weapons/ryno.h"
#include "rnc/overlay/shared/weapons/ryno_rocket.h"
#include "rnc/overlay/shared/weapons/suckcannon.h"
#include "rnc/overlay/shared/weapons/swingshot.h"
#include "rnc/overlay/shared/weapons/swingshot_hook.h"
#include "rnc/overlay/shared/weapons/taunter.h"
#include "rnc/overlay/shared/weapons/tesla_claw.h"
#include "rnc/overlay/shared/weapons/thruster_pack_flame.h"
#include "rnc/overlay/shared/weapons/trailing_flame.h"
#include "rnc/overlay/shared/weapons/trespasser.h"
#include "rnc/overlay/shared/weapons/visibomb.h"
#include "rnc/overlay/shared/weapons/visibomb_missile.h"
#include "rnc/overlay/shared/weapons/walloper.h"
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
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L04_001DD0EC")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L04_001DD104[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

void *D_L04_001DD11C[6]

    __attribute__((section("lvl.vtbl"))) = {update_moby_suck_cannon_state,
                                            update_moby_suck_cannon_state,
                                            launch_circular_saw_enemy_from_suck_cannon,
                                            cancel_suck_cannon_suction_and_set_moby_state_9,
                                            get_circular_saw_enemy_suck_cannon_record,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[169] __asm__("D_L04_001DC900")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,             camera_object_update, default_moby_methods},
        {  11,                    vendor_update, default_moby_methods},
        {  13,                      bolt_update, default_moby_methods},
        {  14,                      bolt_update, default_moby_methods},
        {  15,                      bolt_update, default_moby_methods},
        {  16,                      bolt_update, default_moby_methods},
        {  54,                                0, default_moby_methods},
        {  71,                    wrench_update, default_moby_methods},
        {  74,                      mine_update, default_moby_methods},
        {  85,               noop_main_callback, default_moby_methods},
        {  86,           butterfly_swarm_update, default_moby_methods},
        { 112,             glow_particle_update, default_moby_methods},
        { 121,                      bomb_update, default_moby_methods},
        { 122,                    debris_update, default_moby_methods},
        { 149,   nanotech_crate_fragment_update, default_moby_methods},
        { 153,   gold_devastator_missile_update, default_moby_methods},
        { 157,                devastator_update, default_moby_methods},
        { 161, random_interval_animation_update, default_moby_methods},
        { 163,                  visibomb_update, default_moby_methods},
        { 166,        linked_moby_states_update, default_moby_methods},
        { 167,       thruster_pack_flame_update, default_moby_methods},
        { 168,                   blaster_update, default_moby_methods},
        { 172,          visibomb_missile_update, default_moby_methods},
        { 175,                   taunter_update, default_moby_methods},
        { 176,                 pyrocitor_update, default_moby_methods},
        { 177,                tesla_claw_update, default_moby_methods},
        { 179,            trailing_flame_update, default_moby_methods},
        { 180,                  walloper_update, default_moby_methods},
        { 184,  blarg_trooper_projectile_update, default_moby_methods},
        { 185,                 morphoray_update, default_moby_methods},
        { 186,                  doom_bot_update, default_moby_methods},
        { 188,                trespasser_update, default_moby_methods},
        { 190,                mine_glove_update, default_moby_methods},
        { 192,                bomb_glove_update, default_moby_methods},
        { 203,                     decoy_update, default_moby_methods},
        { 204,           devestator_ammo_update, default_moby_methods},
        { 208,                 swingshot_update, default_moby_methods},
        { 209,            swingshot_hook_update, default_moby_methods},
        { 213,           devestator_ammo_update, default_moby_methods},
        { 214,           devestator_ammo_update, default_moby_methods},
        { 217,                big_axebot_update, default_moby_methods},
        { 222,           devestator_ammo_update, default_moby_methods},
        { 223,           devestator_ammo_update, default_moby_methods},
        { 225,           devestator_ammo_update, default_moby_methods},
        { 226,           devestator_ammo_update, default_moby_methods},
        { 229,             glove_of_doom_update, default_moby_methods},
        { 230,          doom_bot_spawner_update, default_moby_methods},
        { 270,         morphoray_chicken_update,       D_L04_001DD104},
        { 280,                bolt_crank_update, default_moby_methods},
        { 305,               homing_shot_update, default_moby_methods},
        { 340,        circular_saw_enemy_update,       D_L04_001DD11C},
        { 348,   nanotech_crate_fragment_update, default_moby_methods},
        { 349,   nanotech_crate_fragment_update, default_moby_methods},
        { 354,   nanotech_crate_fragment_update, default_moby_methods},
        { 355,   nanotech_crate_fragment_update, default_moby_methods},
        { 356,   nanotech_crate_fragment_update, default_moby_methods},
        { 357,   nanotech_crate_fragment_update, default_moby_methods},
        { 358,   nanotech_crate_fragment_update, default_moby_methods},
        { 359,   nanotech_crate_fragment_update, default_moby_methods},
        { 363,   nanotech_crate_fragment_update, default_moby_methods},
        { 364,   nanotech_crate_fragment_update, default_moby_methods},
        { 427,             blarg_trooper_update, default_moby_methods},
        { 428,       damped_falling_moby_update, default_moby_methods},
        { 432,             eudora_bridge_update, default_moby_methods},
        { 433,               noop_main_callback, default_moby_methods},
        { 434,          platform_booster_update, default_moby_methods},
        { 437,                sand_mouse_update, default_moby_methods},
        { 454,                      ryno_update, default_moby_methods},
        { 457,               ryno_rocket_update, default_moby_methods},
        { 466,                 snow_plow_update, default_moby_methods},
        { 479,                     drone_update, default_moby_methods},
        { 480,                 snow_plow_update, default_moby_methods},
        { 481,         floating_platform_update, default_moby_methods},
        { 483,                                0, default_moby_methods},
        { 484,            transport_ship_update, default_moby_methods},
        { 485,                 snow_plow_update, default_moby_methods},
        { 486,                 snow_plow_update, default_moby_methods},
        { 488,                 snow_plow_update, default_moby_methods},
        { 490,                 snow_plow_update, default_moby_methods},
        { 493,                 snow_plow_update, default_moby_methods},
        { 494,                 snow_plow_update, default_moby_methods},
        { 495,                 snow_plow_update, default_moby_methods},
        { 498,                 snow_plow_update, default_moby_methods},
        { 499,               noop_main_callback, default_moby_methods},
        { 500,                     crate_update, default_moby_methods},
        { 501,                     crate_update, default_moby_methods},
        { 505,                     crate_update, default_moby_methods},
        { 511,                     crate_update, default_moby_methods},
        { 555,                 snow_plow_update, default_moby_methods},
        { 562,               decoy_glove_update, default_moby_methods},
        { 563,               woodcut_bot_update, default_moby_methods},
        { 570,               noop_main_callback, default_moby_methods},
        { 584,  translation_delta_matrix_update, default_moby_methods},
        { 585,            metal_detector_update, default_moby_methods},
        { 604,           sandmouse_house_update, default_moby_methods},
        { 605,              buried_bolts_update, default_moby_methods},
        { 607,                  helipack_update, default_moby_methods},
        { 615,           tresspasser_pad_update, default_moby_methods},
        { 617,       path_progress_mover_update, default_moby_methods},
        { 618,               noop_main_callback, default_moby_methods},
        { 634,                                0, default_moby_methods},
        { 639,           explosion_light_update, default_moby_methods},
        { 642,                      leaf_update, default_moby_methods},
        { 750,                   infobot_update, default_moby_methods},
        { 758,     swingshot_versatarget_update, default_moby_methods},
        { 775,         grow_and_fade_out_update, default_moby_methods},
        { 803,     swingshot_versatarget_update, default_moby_methods},
        { 804,   morphoray_target_effect_update, default_moby_methods},
        { 805,               spawn_point_update, default_moby_methods},
        { 806,             nanotech_ball_update, default_moby_methods},
        { 832,       visibomb_zone_timer_update, default_moby_methods},
        { 849,                suckcannon_update, default_moby_methods},
        {1006,           devestator_ammo_update, default_moby_methods},
        {1052,             eudora_bridge_update, default_moby_methods},
        {1101,                upper_door_update, default_moby_methods},
        {1102,                upper_door_update, default_moby_methods},
        {1120,               suck_cannon_update, default_moby_methods},
        {1134,                 gold_bolt_update, default_moby_methods},
        {1138,                 quad_moby_update, default_moby_methods},
        {1143,                vendor_orb_update, default_moby_methods},
        {1190,                lieutenant_update, default_moby_methods},
        {1192,             glow_particle_update, default_moby_methods},
        {1251,            hydrodisplacer_update, default_moby_methods},
        {1289,               noop_main_callback, default_moby_methods},
        {1290,             pilots_helmet_update, default_moby_methods},
        {1343,                help_hints_update, default_moby_methods},
        {1438,           devestator_ammo_update, default_moby_methods},
        {1447,           devestator_ammo_update, default_moby_methods},
        {1449,           devestator_ammo_update, default_moby_methods},
        {1516,        woodcut_bot_debris_update, default_moby_methods},
        {1531,                upper_door_update, default_moby_methods},
        {1532,                upper_door_update, default_moby_methods},
        {1549,    cutscene_camera_target_update, default_moby_methods},
        {1599,             tumbling_part_update, default_moby_methods},
        {1601,             tumbling_part_update, default_moby_methods},
        {1602,             tumbling_part_update, default_moby_methods},
        {1603,             tumbling_part_update, default_moby_methods},
        {1604,             tumbling_part_update, default_moby_methods},
        {1605,             tumbling_part_update, default_moby_methods},
        {1606,             tumbling_part_update, default_moby_methods},
        {1607,             tumbling_part_update, default_moby_methods},
        {1608,             tumbling_part_update, default_moby_methods},
        {1609,             tumbling_part_update, default_moby_methods},
        {1613,             tumbling_part_update, default_moby_methods},
        {1614,             tumbling_part_update, default_moby_methods},
        {1615,             tumbling_part_update, default_moby_methods},
        {1616,             tumbling_part_update, default_moby_methods},
        {1617,             tumbling_part_update, default_moby_methods},
        {1618,             tumbling_part_update, default_moby_methods},
        {1619,             tumbling_part_update, default_moby_methods},
        {1620,             tumbling_part_update, default_moby_methods},
        {1624,             tumbling_part_update, default_moby_methods},
        {1633,      sandmouse_projectile_update, default_moby_methods},
        {1739,             tumbling_part_update, default_moby_methods},
        {1740,             tumbling_part_update, default_moby_methods},
        {1741,             tumbling_part_update, default_moby_methods},
        {1818,                 sandmouse_update, default_moby_methods},
        {1834,                      pipe_update, default_moby_methods},
        {1835,               noop_main_callback, default_moby_methods},
        {1836,             tumbling_part_update, default_moby_methods},
        {1900,                     decoy_update, default_moby_methods},
        {1939,             tumbling_part_update, default_moby_methods},
        {1940,             tumbling_part_update, default_moby_methods},
        {1941,             tumbling_part_update, default_moby_methods},
        {1942,             tumbling_part_update, default_moby_methods},
        {1944,             tumbling_part_update, default_moby_methods},
        {1945,             tumbling_part_update, default_moby_methods},
        {1946,             tumbling_part_update, default_moby_methods},
        {  -1,                                0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L04_001DD180") __attribute__((section("lvl.camvtbl"))) = {
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
    {17,  volume_camera_swing_check,                  noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {23,        volume_camera_check,                  noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {-1,                          0,                                   0,                    0,                                                  0},
};

struct SoundHandler sound_handlers[2] __asm__("D_L04_001DD280")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 6, update_music_switch_emitter},
        {-1,                           0},
};
