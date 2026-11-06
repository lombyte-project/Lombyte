#include "rnc/mobys.h"
#include "rnc/overlay/l05/mobys.h"
#include "rnc/overlay/shared/audio/emitters.h"
#include "rnc/overlay/shared/camera.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/swing_volume_camera.h"
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
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"
#include "rnc/overlay/shared/vehicles.h"
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
void refresh_point_light() __asm__("FUN_00201f58");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L05_002193B8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L05_002193D0[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            get_morphoray_chicken_slot_record,
                                            hud_counter_update};

void *D_L05_002193E8[6]

    __attribute__((section("lvl.vtbl"))) = {process_amoeboid_suck_cannon_suction,
                                            moby_hold_state_if_found,
                                            moby_hold_state_if_found,
                                            cancel_suck_cannon_suction_and_set_state_14,
                                            get_moby_data_plus_0xc0,
                                            hud_counter_update};

void *D_L05_00219400[6]

    __attribute__((section("lvl.vtbl"))) = {process_amoeboid_suck_cannon_suction,
                                            moby_hold_state_if_found,
                                            moby_hold_state_if_found,
                                            cancel_suck_cannon_suction_and_set_state_14,
                                            get_moby_data_plus_0xc0,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[154] __asm__("D_L05_00218C80")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,            camera_object_update, default_moby_methods},
        {  11,                   vendor_update, default_moby_methods},
        {  13,                     bolt_update, default_moby_methods},
        {  14,                     bolt_update, default_moby_methods},
        {  15,                     bolt_update, default_moby_methods},
        {  16,                     bolt_update, default_moby_methods},
        {  35,              sharkigator_update, default_moby_methods},
        {  71,                   wrench_update, default_moby_methods},
        {  74,                     mine_update, default_moby_methods},
        {  79,             scenery_ship_update, default_moby_methods},
        { 112,            glow_particle_update, default_moby_methods},
        { 121,                     bomb_update, default_moby_methods},
        { 122,                   debris_update, default_moby_methods},
        { 133,                 amoeboid_update, default_moby_methods},
        { 149,  nanotech_crate_fragment_update, default_moby_methods},
        { 153,  gold_devastator_missile_update, default_moby_methods},
        { 157,               devastator_update, default_moby_methods},
        { 163,                 visibomb_update, default_moby_methods},
        { 166,       linked_moby_states_update, default_moby_methods},
        { 167,      thruster_pack_flame_update, default_moby_methods},
        { 168,                  blaster_update, default_moby_methods},
        { 172,         visibomb_missile_update, default_moby_methods},
        { 175,                  taunter_update, default_moby_methods},
        { 176,                pyrocitor_update, default_moby_methods},
        { 177,               tesla_claw_update, default_moby_methods},
        { 179,           trailing_flame_update, default_moby_methods},
        { 180,                 walloper_update, default_moby_methods},
        { 185,                morphoray_update, default_moby_methods},
        { 186,                 doom_bot_update, default_moby_methods},
        { 188,               trespasser_update, default_moby_methods},
        { 190,               mine_glove_update, default_moby_methods},
        { 192,               bomb_glove_update, default_moby_methods},
        { 203,                    decoy_update, default_moby_methods},
        { 204,          devestator_ammo_update, default_moby_methods},
        { 208,                swingshot_update, default_moby_methods},
        { 209,           swingshot_hook_update, default_moby_methods},
        { 213,          devestator_ammo_update, default_moby_methods},
        { 214,          devestator_ammo_update, default_moby_methods},
        { 222,          devestator_ammo_update, default_moby_methods},
        { 223,          devestator_ammo_update, default_moby_methods},
        { 225,          devestator_ammo_update, default_moby_methods},
        { 226,          devestator_ammo_update, default_moby_methods},
        { 229,            glove_of_doom_update, default_moby_methods},
        { 230,         doom_bot_spawner_update, default_moby_methods},
        { 258,     moby_spatial_trigger_update, default_moby_methods},
        { 270,        morphoray_chicken_update,       D_L05_002193D0},
        { 305,              homing_shot_update, default_moby_methods},
        { 341,    hydrodisplacer_faucet_update, default_moby_methods},
        { 348,  nanotech_crate_fragment_update, default_moby_methods},
        { 349,  nanotech_crate_fragment_update, default_moby_methods},
        { 354,  nanotech_crate_fragment_update, default_moby_methods},
        { 355,  nanotech_crate_fragment_update, default_moby_methods},
        { 356,  nanotech_crate_fragment_update, default_moby_methods},
        { 357,  nanotech_crate_fragment_update, default_moby_methods},
        { 358,  nanotech_crate_fragment_update, default_moby_methods},
        { 359,  nanotech_crate_fragment_update, default_moby_methods},
        { 363,  nanotech_crate_fragment_update, default_moby_methods},
        { 364,  nanotech_crate_fragment_update, default_moby_methods},
        { 428,      damped_falling_moby_update, default_moby_methods},
        { 433,              noop_main_callback, default_moby_methods},
        { 439,               hoverboard_update, default_moby_methods},
        { 454,                     ryno_update, default_moby_methods},
        { 457,              ryno_rocket_update, default_moby_methods},
        { 479,                    drone_update, default_moby_methods},
        { 483,                               0, default_moby_methods},
        { 500,                    crate_update, default_moby_methods},
        { 501,                    crate_update, default_moby_methods},
        { 502,                    crate_update, default_moby_methods},
        { 505,                    crate_update, default_moby_methods},
        { 511,                    crate_update, default_moby_methods},
        { 562,              decoy_glove_update, default_moby_methods},
        { 572,           large_amoeboid_update,       D_L05_002193E8},
        { 585,           metal_detector_update, default_moby_methods},
        { 604,          sandmouse_house_update, default_moby_methods},
        { 605,             buried_bolts_update, default_moby_methods},
        { 607,                 helipack_update, default_moby_methods},
        { 618,              noop_main_callback, default_moby_methods},
        { 623,               extermibot_update, default_moby_methods},
        { 625,              extermitank_update, default_moby_methods},
        { 634,                               0, default_moby_methods},
        { 639,          explosion_light_update, default_moby_methods},
        { 679,         rail_hero_motion_update, default_moby_methods},
        { 717,              skid_mcmarx_update, default_moby_methods},
        { 750,                  infobot_update, default_moby_methods},
        { 758,    swingshot_versatarget_update, default_moby_methods},
        { 775,        grow_and_fade_out_update, default_moby_methods},
        { 803,    swingshot_versatarget_update, default_moby_methods},
        { 804,  morphoray_target_effect_update, default_moby_methods},
        { 805,              spawn_point_update, default_moby_methods},
        { 806,            nanotech_ball_update, default_moby_methods},
        { 810,        floating_platform_update, default_moby_methods},
        { 812,                      box_update, default_moby_methods},
        { 823,              moby_wobble_update, default_moby_methods},
        { 830,             floor_button_update, default_moby_methods},
        { 831,                    water_update, default_moby_methods},
        { 832,      visibomb_zone_timer_update, default_moby_methods},
        { 838,            electric_gate_update, default_moby_methods},
        { 841,                 fog_zone_update, default_moby_methods},
        { 843,            sliding_block_update, default_moby_methods},
        { 844,              drain_cover_update, default_moby_methods},
        { 846,     pipe_mounted_grating_update, default_moby_methods},
        { 849,               suckcannon_update, default_moby_methods},
        { 852,              rising_gate_update, default_moby_methods},
        { 853,              rising_gate_update, default_moby_methods},
        { 855,              water_spout_update, default_moby_methods},
        { 865,           large_amoeboid_update, default_moby_methods},
        { 866,           large_amoeboid_update,       D_L05_00219400},
        { 877,     elevator_with_button_update, default_moby_methods},
        { 879,             ocean_rilgar_update, default_moby_methods},
        { 893,             hinged_hatch_update, default_moby_methods},
        { 895,            swinging_flap_update, default_moby_methods},
        { 918,          hoverboard_girl_update, default_moby_methods},
        { 919,           qwarks_bouncer_update, default_moby_methods},
        { 920,             qwark_rilgar_update, default_moby_methods},
        { 925,           shady_salesman_update, default_moby_methods},
        { 980,                blarg_gun_update, default_moby_methods},
        { 981,                blarg_gun_update, default_moby_methods},
        { 982, hero_clip_volume_outputs_update, default_moby_methods},
        { 984,               television_update, default_moby_methods},
        { 998,                     taxi_update, default_moby_methods},
        {1006,          devestator_ammo_update, default_moby_methods},
        {1060,                  red_orb_update, default_moby_methods},
        {1099,           qwarks_trailer_update, default_moby_methods},
        {1134,                gold_bolt_update, default_moby_methods},
        {1138,                quad_moby_update, default_moby_methods},
        {1139,          hoverboard_ring_update, default_moby_methods},
        {1140,              noop_main_callback, default_moby_methods},
        {1143,               vendor_orb_update, default_moby_methods},
        {1192,            glow_particle_update, default_moby_methods},
        {1251,           hydrodisplacer_update, default_moby_methods},
        {1289,              noop_main_callback, default_moby_methods},
        {1290,            pilots_helmet_update, default_moby_methods},
        {1347,               help_hints_update, default_moby_methods},
        {1438,          devestator_ammo_update, default_moby_methods},
        {1447,          devestator_ammo_update, default_moby_methods},
        {1449,          devestator_ammo_update, default_moby_methods},
        {1511,           breakable_lamp_update, default_moby_methods},
        {1514,              noop_main_callback, default_moby_methods},
        {1517,            tumbling_part_update, default_moby_methods},
        {1518,            tumbling_part_update, default_moby_methods},
        {1550,   cutscene_camera_target_update, default_moby_methods},
        {1633,     sandmouse_projectile_update, default_moby_methods},
        {1754,            tumbling_part_update, default_moby_methods},
        {1755,            tumbling_part_update, default_moby_methods},
        {1756,            tumbling_part_update, default_moby_methods},
        {1757,            tumbling_part_update, default_moby_methods},
        {1759,            tumbling_part_update, default_moby_methods},
        {1760,            tumbling_part_update, default_moby_methods},
        {1790,            tumbling_part_update, default_moby_methods},
        {1791,            tumbling_part_update, default_moby_methods},
        {1818,                sandmouse_update, default_moby_methods},
        {1838,            tumbling_part_update, default_moby_methods},
        {1900,                    decoy_update, default_moby_methods},
        {  -1,                               0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L05_00219480") __attribute__((section("lvl.camvtbl"))) = {
    { 0,   noop_callback_l,                   follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                          },
    { 3,
     noop_callback_l, initialize_camera_transform_and_paths,
     update_camera_position_and_orientation_from_paths,              update_camera_mode_from_preset                                         },
    { 4,
     camera_transition_update,   initialize_hero_aligned_camera_mode,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                     },
    { 5,
     noop_callback_l,        initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                                },
    { 6,           noop_callback_l,     initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l,     initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                             },
    { 8,
     noop_callback_l,    reset_camera_from_source_transform,
     refresh_point_light,                          noop_main_callback                                                                       },
    {17, volume_camera_swing_check,                    noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                         0,                                     0,                    0,                                           0},
};

struct SoundHandler sound_handlers[7] __asm__("D_L05_00219580")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0,        update_sound_emitter_playback_by_distance},
        { 1,                               update_box_emitter},
        { 2, update_sound_emitter_playback_at_random_position},
        { 3,                        update_reverb_box_emitter},
        { 5,                          update_listener_emitter},
        { 6,                      update_music_switch_emitter},
        {-1,                                                0},
};
