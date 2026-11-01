#include "rnc/mobys/quad_moby_update.h"
#include "rnc/overlay/l09/mobys/airship_chain_link.h"
#include "rnc/overlay/l09/mobys/airship_part.h"
#include "rnc/overlay/l09/mobys/asteroid.h"
#include "rnc/overlay/l09/mobys/barrier.h"
#include "rnc/overlay/l09/mobys/damage_causer.h"
#include "rnc/overlay/l09/mobys/destructible_prop.h"
#include "rnc/overlay/l09/mobys/display.h"
#include "rnc/overlay/l09/mobys/drifting_moby.h"
#include "rnc/overlay/l09/mobys/elevator.h"
#include "rnc/overlay/l09/mobys/floating_ship_piece.h"
#include "rnc/overlay/l09/mobys/help_hints.h"
#include "rnc/overlay/l09/mobys/lamp.h"
#include "rnc/overlay/l09/mobys/platform.h"
#include "rnc/overlay/l09/mobys/refueling_station.h"
#include "rnc/overlay/l09/mobys/shrinking_tumbling_debris.h"
#include "rnc/overlay/l09/mobys/shuttle.h"
#include "rnc/overlay/l09/mobys/tumbling_debris.h"
#include "rnc/overlay/l09/mobys/turret.h"
#include "rnc/overlay/shared/audio/emitters/box_emitter.h"
#include "rnc/overlay/shared/audio/emitters/distance_emitter.h"
#include "rnc/overlay/shared/audio/emitters/reverb_box_emitter.h"
#include "rnc/overlay/shared/camera/aim_volume_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_setup.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/camera/hero_aligned_camera.h"
#include "rnc/overlay/shared/camera/orbit_camera.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/swing_volume_camera.h"
#include "rnc/overlay/shared/camera/target_tracking_camera.h"
#include "rnc/overlay/shared/enemies/billboard_projectile.h"
#include "rnc/overlay/shared/enemies/blarg_paratrooper.h"
#include "rnc/overlay/shared/enemies/ground_melee_enemy.h"
#include "rnc/overlay/shared/enemies/paratrooper_fire_bolt.h"
#include "rnc/overlay/shared/enemies/seeker.h"
#include "rnc/overlay/shared/enemies/smoke_trail_projectile.h"
#include "rnc/overlay/shared/enemies/spawner.h"
#include "rnc/overlay/shared/enemies/spawner_egg.h"
#include "rnc/overlay/shared/mobys/camera_object.h"
#include "rnc/overlay/shared/mobys/moby_draw_registration_update.h"
#include "rnc/overlay/shared/pickups/buried_bolts.h"
#include "rnc/overlay/shared/pickups/crate.h"
#include "rnc/overlay/shared/pickups/crate_fragment.h"
#include "rnc/overlay/shared/pickups/nanotech_ball.h"
#include "rnc/overlay/shared/pickups/pilots_helmet.h"
#include "rnc/overlay/shared/pickups/weapon_ammo.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/overlay/shared/props/hydrodisplacer.h"
#include "rnc/overlay/shared/props/moby_state_trigger.h"
#include "rnc/overlay/shared/props/rock.h"
#include "rnc/overlay/shared/props/swingshot_versatarget.h"
#include "rnc/overlay/shared/props/tumbling_part.h"
#include "rnc/overlay/shared/rendering/effects/debris.h"
#include "rnc/overlay/shared/rendering/effects/explosion_light.h"
#include "rnc/overlay/shared/rendering/effects/glow_particle.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"
#include "rnc/overlay/shared/vehicles/background_ship_1.h"
#include "rnc/overlay/shared/vehicles/background_ship_2.h"
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
#include "rnc/overlay/shared/weapons/turret_shot.h"
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

void *default_moby_methods[6] __asm__("D_L09_00209AE8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L09_00209B00[6] __attribute__((section("lvl.vtbl"))) = {moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                run_moby_helper_and_set_state_7,
                                                                get_moby_data_plus_0x60,
                                                                hud_counter_update};

void *D_L09_00209B18[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[158] __asm__("D_L09_00209380")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,             camera_object_update, default_moby_methods},
        {1184,              airship_part_update, default_moby_methods},
        {  11,                    vendor_update, default_moby_methods},
        {  13,                      bolt_update, default_moby_methods},
        {  14,                      bolt_update, default_moby_methods},
        {  15,                      bolt_update, default_moby_methods},
        {  16,                      bolt_update, default_moby_methods},
        {  52,                    seeker_update, default_moby_methods},
        {  71,                    wrench_update, default_moby_methods},
        {  74,                      mine_update, default_moby_methods},
        { 112,             glow_particle_update, default_moby_methods},
        { 121,                      bomb_update, default_moby_methods},
        { 122,                    debris_update, default_moby_methods},
        { 149,   nanotech_crate_fragment_update, default_moby_methods},
        { 153,   gold_devastator_missile_update, default_moby_methods},
        { 156,      billboard_projectile_update, default_moby_methods},
        { 157,                devastator_update, default_moby_methods},
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
        { 185,                 morphoray_update, default_moby_methods},
        { 186,                  doom_bot_update, default_moby_methods},
        { 188,                trespasser_update, default_moby_methods},
        { 190,                mine_glove_update, default_moby_methods},
        { 192,                bomb_glove_update, default_moby_methods},
        { 193,        ground_melee_enemy_update,       D_L09_00209B00},
        { 203,                     decoy_update, default_moby_methods},
        { 204,           devestator_ammo_update, default_moby_methods},
        { 208,                 swingshot_update, default_moby_methods},
        { 209,            swingshot_hook_update, default_moby_methods},
        { 213,           devestator_ammo_update, default_moby_methods},
        { 214,           devestator_ammo_update, default_moby_methods},
        { 222,           devestator_ammo_update, default_moby_methods},
        { 223,           devestator_ammo_update, default_moby_methods},
        { 225,           devestator_ammo_update, default_moby_methods},
        { 226,           devestator_ammo_update, default_moby_methods},
        { 229,             glove_of_doom_update, default_moby_methods},
        { 230,          doom_bot_spawner_update, default_moby_methods},
        { 258,      moby_spatial_trigger_update, default_moby_methods},
        { 263,                  asteroid_update, default_moby_methods},
        { 264,                  asteroid_update, default_moby_methods},
        { 265,                  asteroid_update, default_moby_methods},
        { 266,                  asteroid_update, default_moby_methods},
        { 267,                  asteroid_update, default_moby_methods},
        { 270,         morphoray_chicken_update,       D_L09_00209B18},
        { 276,                   display_update, default_moby_methods},
        { 279,         destructible_prop_update, default_moby_methods},
        { 284,               noop_main_callback, default_moby_methods},
        { 305,               homing_shot_update, default_moby_methods},
        { 313,           tumbling_debris_update, default_moby_methods},
        { 317,    moby_draw_registration_update, default_moby_methods},
        { 320, shrinking_tumbling_debris_update, default_moby_methods},
        { 321, shrinking_tumbling_debris_update, default_moby_methods},
        { 324,               noop_main_callback, default_moby_methods},
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
        { 417,             drifting_moby_update, default_moby_methods},
        { 428,       damped_falling_moby_update, default_moby_methods},
        { 433,               noop_main_callback, default_moby_methods},
        { 454,                      ryno_update, default_moby_methods},
        { 457,               ryno_rocket_update, default_moby_methods},
        { 479,                     drone_update, default_moby_methods},
        { 483,                                0, default_moby_methods},
        { 500,                     crate_update, default_moby_methods},
        { 501,                     crate_update, default_moby_methods},
        { 502,                     crate_update, default_moby_methods},
        { 505,                     crate_update, default_moby_methods},
        { 511,                     crate_update, default_moby_methods},
        { 562,               decoy_glove_update, default_moby_methods},
        { 567,    smoke_trail_projectile_update, default_moby_methods},
        { 585,            metal_detector_update, default_moby_methods},
        { 605,              buried_bolts_update, default_moby_methods},
        { 607,                  helipack_update, default_moby_methods},
        { 618,               noop_main_callback, default_moby_methods},
        { 634,                                0, default_moby_methods},
        { 639,           explosion_light_update, default_moby_methods},
        { 664,                  platform_update, default_moby_methods},
        { 775,         grow_and_fade_out_update, default_moby_methods},
        { 803,     swingshot_versatarget_update, default_moby_methods},
        { 804,   morphoray_target_effect_update, default_moby_methods},
        { 805,               spawn_point_update, default_moby_methods},
        { 806,             nanotech_ball_update, default_moby_methods},
        { 832,       visibomb_zone_timer_update, default_moby_methods},
        { 849,                suckcannon_update, default_moby_methods},
        {1000,                help_hints_update, default_moby_methods},
        {1006,           devestator_ammo_update, default_moby_methods},
        {1023,         blarg_paratrooper_update, default_moby_methods},
        {1134,                 gold_bolt_update, default_moby_methods},
        {1138,                 quad_moby_update, default_moby_methods},
        {1143,                vendor_orb_update, default_moby_methods},
        {1150,                  elevator_update, default_moby_methods},
        {1151,                  elevator_update, default_moby_methods},
        {1172,         refueling_station_update, default_moby_methods},
        {1181,        airship_chain_link_update, default_moby_methods},
        {1182,              airship_part_update, default_moby_methods},
        {1183,              airship_part_update, default_moby_methods},
        {1185,              airship_part_update, default_moby_methods},
        {1186,              airship_part_update, default_moby_methods},
        {1187,              airship_part_update, default_moby_methods},
        {1188,              airship_part_update, default_moby_methods},
        {1189,              airship_part_update, default_moby_methods},
        {1192,             glow_particle_update, default_moby_methods},
        {1201,                    turret_update, default_moby_methods},
        {1206,             damage_causer_update, default_moby_methods},
        {1212,         background_ship_1_update, default_moby_methods},
        {1213,         background_ship_2_update, default_moby_methods},
        {1251,            hydrodisplacer_update, default_moby_methods},
        {1258,               turret_shot_update, default_moby_methods},
        {1285,                   barrier_update, default_moby_methods},
        {1286,                   barrier_update, default_moby_methods},
        {1287,                   barrier_update, default_moby_methods},
        {1288,                   barrier_update, default_moby_methods},
        {1289,               noop_main_callback, default_moby_methods},
        {1290,             pilots_helmet_update, default_moby_methods},
        {1292,                 fire_bolt_update, default_moby_methods},
        {1293,       floating_ship_piece_update, default_moby_methods},
        {1298,                   display_update, default_moby_methods},
        {1299,                   display_update, default_moby_methods},
        {1300,               noop_main_callback, default_moby_methods},
        {1320,       floating_ship_piece_update, default_moby_methods},
        {1438,           devestator_ammo_update, default_moby_methods},
        {1447,           devestator_ammo_update, default_moby_methods},
        {1449,           devestator_ammo_update, default_moby_methods},
        {1522,             tumbling_part_update, default_moby_methods},
        {1525,             tumbling_part_update, default_moby_methods},
        {1526,             tumbling_part_update, default_moby_methods},
        {1528,             tumbling_part_update, default_moby_methods},
        {1529,             tumbling_part_update, default_moby_methods},
        {1692,             tumbling_part_update, default_moby_methods},
        {1693,             tumbling_part_update, default_moby_methods},
        {1694,             tumbling_part_update, default_moby_methods},
        {1733,             tumbling_part_update, default_moby_methods},
        {1734,             tumbling_part_update, default_moby_methods},
        {1735,             tumbling_part_update, default_moby_methods},
        {1766,                   shuttle_update, default_moby_methods},
        {1849,                      lamp_update, default_moby_methods},
        {1850,               noop_main_callback, default_moby_methods},
        {1851,             tumbling_part_update, default_moby_methods},
        {1885,                   spawner_update, default_moby_methods},
        {1886,               spawner_egg_update, default_moby_methods},
        {1900,                     decoy_update, default_moby_methods},
        {1936,             tumbling_part_update, default_moby_methods},
        {  -1,                                0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L09_00209B80") __attribute__((section("lvl.camvtbl"))) = {
    { 0,   noop_callback_l,                 follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                        },
    { 4,
     camera_transition_update, initialize_hero_aligned_camera_mode,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                   },
    { 5,
     noop_callback_l,      initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                              },
    { 6,           noop_callback_l,   initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l,   initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                           },
    {17, volume_camera_swing_check,                  noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,       volume_camera_check,                  noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                         0,                                   0,                    0,                                           0},
};

struct SoundHandler sound_handlers[4] __asm__("D_L09_00209C80")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 1,                        update_box_emitter},
        { 3,                 update_reverb_box_emitter},
        {-1,                                         0},
};
