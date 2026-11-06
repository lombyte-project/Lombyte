#include "rnc/mobys.h"
#include "rnc/overlay/l08/mobys.h"
#include "rnc/overlay/l08/mobys/bomber_tank_bomb.h"
#include "rnc/overlay/l08/mobys/rocket.h"
#include "rnc/overlay/shared/audio/emitters.h"
#include "rnc/overlay/shared/camera.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/cutscenes.h"
#include "rnc/overlay/shared/enemies.h"
#include "rnc/overlay/shared/mobys.h"
#include "rnc/overlay/shared/pickups.h"
#include "rnc/overlay/shared/pickups/infobot.h"
#include "rnc/overlay/shared/props.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/overlay/shared/props/rock.h"
#include "rnc/overlay/shared/rendering/effects.h"
#include "rnc/overlay/shared/rendering/effects/explosion_light.h"
#include "rnc/overlay/shared/rendering/effects/ground_particle_scatter.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"
#include "rnc/overlay/shared/vendor.h"
#include "rnc/overlay/shared/weapons.h"

s32 get_morphoray_chicken_slot_record(s32) __asm__("FUN_L00_002caf40");

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
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L08_001FD1E8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L08_001FD200[6] __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                enter_scoutbot_return_home_state,
                                                                get_moby_data_plus_0x60,
                                                                hud_counter_update};

void *D_L08_001FD218[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            get_morphoray_chicken_slot_record,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[190] __asm__("D_L08_001FC900")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,             camera_object_update, default_moby_methods},
        {  11,                    vendor_update, default_moby_methods},
        {  13,                      bolt_update, default_moby_methods},
        {  14,                      bolt_update, default_moby_methods},
        {  15,                      bolt_update, default_moby_methods},
        {  16,                      bolt_update, default_moby_methods},
        {  71,                    wrench_update, default_moby_methods},
        {  74,                      mine_update, default_moby_methods},
        { 112,             glow_particle_update, default_moby_methods},
        { 121,                      bomb_update, default_moby_methods},
        { 122,                    debris_update, default_moby_methods},
        { 149,   nanotech_crate_fragment_update, default_moby_methods},
        { 152,               bolt_pickup_update, default_moby_methods},
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
        { 248,    blarg_bombthrower_bomb_update, default_moby_methods},
        { 252,                  scoutbot_update,       D_L08_001FD200},
        { 253,               bomber_tank_update, default_moby_methods},
        { 270,         morphoray_chicken_update,       D_L08_001FD218},
        { 280,                bolt_crank_update, default_moby_methods},
        { 305,               homing_shot_update, default_moby_methods},
        { 324,               noop_main_callback, default_moby_methods},
        { 327,             water_surface_update, default_moby_methods},
        { 331,           trackdrive_left_update, default_moby_methods},
        { 333,         blarg_bombthrower_update, default_moby_methods},
        { 344,                 blarg_gun_update, default_moby_methods},
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
        { 424,                   shuttle_update, default_moby_methods},
        { 425,          bomber_tank_bomb_update, default_moby_methods},
        { 428,       damped_falling_moby_update, default_moby_methods},
        { 433,               noop_main_callback, default_moby_methods},
        { 435,                    rocket_update, default_moby_methods},
        { 438,       path_following_moby_update, default_moby_methods},
        { 440,            batalia_cannon_update, default_moby_methods},
        { 441,            falling_debris_update, default_moby_methods},
        { 442,            falling_debris_update, default_moby_methods},
        { 443,            falling_debris_update, default_moby_methods},
        { 444,                   gunship_update, default_moby_methods},
        { 445,            falling_debris_update, default_moby_methods},
        { 448,            falling_debris_update, default_moby_methods},
        { 449,            falling_debris_update, default_moby_methods},
        { 450,            falling_debris_update, default_moby_methods},
        { 451,            falling_debris_update, default_moby_methods},
        { 452,                bolt_thief_update, default_moby_methods},
        { 454,                      ryno_update, default_moby_methods},
        { 455,              bomb_spawner_update, default_moby_methods},
        { 457,               ryno_rocket_update, default_moby_methods},
        { 458,             turret_rocket_update, default_moby_methods},
        { 462,                     shrub_update, default_moby_methods},
        { 463,             heavy_gunship_update, default_moby_methods},
        { 464,            falling_debris_update, default_moby_methods},
        { 465,            falling_debris_update, default_moby_methods},
        { 467,                      lift_update, default_moby_methods},
        { 468,                swing_gate_update, default_moby_methods},
        { 469,                swing_gate_update, default_moby_methods},
        { 472,                      lift_update, default_moby_methods},
        { 479,                     drone_update, default_moby_methods},
        { 483,                                0, default_moby_methods},
        { 492,                 red_plant_update, default_moby_methods},
        { 500,                     crate_update, default_moby_methods},
        { 501,                     crate_update, default_moby_methods},
        { 505,                     crate_update, default_moby_methods},
        { 511,                     crate_update, default_moby_methods},
        { 545,         bomb_spawner_bomb_update, default_moby_methods},
        { 547,                 blarg_gun_update, default_moby_methods},
        { 548,                 blarg_gun_update, default_moby_methods},
        { 549,                 blarg_gun_update, default_moby_methods},
        { 550,                 blarg_gun_update, default_moby_methods},
        { 551,                 blarg_gun_update, default_moby_methods},
        { 562,               decoy_glove_update, default_moby_methods},
        { 566,               noop_main_callback, default_moby_methods},
        { 567,    smoke_trail_projectile_update, default_moby_methods},
        { 585,            metal_detector_update, default_moby_methods},
        { 588,                 blarg_gun_update, default_moby_methods},
        { 589,                 blarg_gun_update, default_moby_methods},
        { 590,                 blarg_gun_update, default_moby_methods},
        { 591,                 blarg_gun_update, default_moby_methods},
        { 592,                 blarg_gun_update, default_moby_methods},
        { 593,                 blarg_gun_update, default_moby_methods},
        { 594,                 blarg_gun_update, default_moby_methods},
        { 595,                 blarg_gun_update, default_moby_methods},
        { 596,                 blarg_gun_update, default_moby_methods},
        { 597,                 blarg_gun_update, default_moby_methods},
        { 598,                 blarg_gun_update, default_moby_methods},
        { 600,       layered_mesh_effect_update, default_moby_methods},
        { 604,           sandmouse_house_update, default_moby_methods},
        { 605,              buried_bolts_update, default_moby_methods},
        { 607,                  helipack_update, default_moby_methods},
        { 615,           tresspasser_pad_update, default_moby_methods},
        { 618,               noop_main_callback, default_moby_methods},
        { 621,                  landmine_update, default_moby_methods},
        { 634,                                0, default_moby_methods},
        { 639,           explosion_light_update, default_moby_methods},
        { 648, directional_spark_emitter_update, default_moby_methods},
        { 671,    flickering_mesh_effect_update, default_moby_methods},
        { 679,          rail_hero_motion_update, default_moby_methods},
        { 750,                   infobot_update, default_moby_methods},
        { 774,               the_plumber_update, default_moby_methods},
        { 775,         grow_and_fade_out_update, default_moby_methods},
        { 782,                 blarg_gun_update, default_moby_methods},
        { 783,                 blarg_gun_update, default_moby_methods},
        { 784,                 blarg_gun_update, default_moby_methods},
        { 785,                 blarg_gun_update, default_moby_methods},
        { 804,   morphoray_target_effect_update, default_moby_methods},
        { 805,               spawn_point_update, default_moby_methods},
        { 806,             nanotech_ball_update, default_moby_methods},
        { 832,       visibomb_zone_timer_update, default_moby_methods},
        { 849,                suckcannon_update, default_moby_methods},
        {1006,           devestator_ammo_update, default_moby_methods},
        {1023,         blarg_paratrooper_update, default_moby_methods},
        {1039,            respawn_volume_update, default_moby_methods},
        {1130,                  commando_update, default_moby_methods},
        {1134,                 gold_bolt_update, default_moby_methods},
        {1135,                teleporter_update, default_moby_methods},
        {1138,                 quad_moby_update, default_moby_methods},
        {1143,                vendor_orb_update, default_moby_methods},
        {1144,          batalia_deserter_update, default_moby_methods},
        {1192,             glow_particle_update, default_moby_methods},
        {1251,            hydrodisplacer_update, default_moby_methods},
        {1283,       the_plumber_batalia_update, default_moby_methods},
        {1289,               noop_main_callback, default_moby_methods},
        {1290,             pilots_helmet_update, default_moby_methods},
        {1292,                 fire_bolt_update, default_moby_methods},
        {1349,                help_hints_update, default_moby_methods},
        {1400,   ground_particle_scatter_update, default_moby_methods},
        {1438,           devestator_ammo_update, default_moby_methods},
        {1447,           devestator_ammo_update, default_moby_methods},
        {1449,           devestator_ammo_update, default_moby_methods},
        {1553,    cutscene_camera_target_update, default_moby_methods},
        {1629,   radial_particle_emitter_update, default_moby_methods},
        {1633,      sandmouse_projectile_update, default_moby_methods},
        {1641,            effect_emitter_update, default_moby_methods},
        {1649,   batalia_textured_effect_update, default_moby_methods},
        {1692,             tumbling_part_update, default_moby_methods},
        {1693,             tumbling_part_update, default_moby_methods},
        {1694,             tumbling_part_update, default_moby_methods},
        {1728,             tumbling_part_update, default_moby_methods},
        {1729,             tumbling_part_update, default_moby_methods},
        {1730,             tumbling_part_update, default_moby_methods},
        {1731,             tumbling_part_update, default_moby_methods},
        {1732,             tumbling_part_update, default_moby_methods},
        {1733,             tumbling_part_update, default_moby_methods},
        {1734,             tumbling_part_update, default_moby_methods},
        {1735,             tumbling_part_update, default_moby_methods},
        {1790,             tumbling_part_update, default_moby_methods},
        {1791,             tumbling_part_update, default_moby_methods},
        {1818,                 sandmouse_update, default_moby_methods},
        {1844,           shattering_prop_update, default_moby_methods},
        {1845,               noop_main_callback, default_moby_methods},
        {1846,             tumbling_part_update, default_moby_methods},
        {1847,             tumbling_part_update, default_moby_methods},
        {1900,                     decoy_update, default_moby_methods},
        {  -1,                                0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L08_001FD280") __attribute__((section("lvl.camvtbl"))) = {
    { 0,    noop_callback_l,                   follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                           },
    { 3,
     noop_callback_l, initialize_camera_transform_and_paths,
     update_camera_position_and_orientation_from_paths,              update_camera_mode_from_preset                                          },
    { 4,
     camera_transition_update,   initialize_hero_aligned_camera_mode,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                      },
    { 5,
     noop_callback_l,        initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                                 },
    { 6,            noop_callback_l,     initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
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
