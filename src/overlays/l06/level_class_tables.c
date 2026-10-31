#include "rnc/mobys.h"
#include "rnc/overlay/l06/mobys/alien_queen.h"
#include "rnc/overlay/l06/mobys/alien_queen_slider.h"
#include "rnc/overlay/l06/mobys.h"
#include "rnc/overlay/shared/camera.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/swing_volume_camera.h"
#include "rnc/overlay/shared/enemies.h"
#include "rnc/overlay/shared/enemies/gadgebot.h"
#include "rnc/overlay/shared/enemies/gadgebot_spawn_container.h"
#include "rnc/overlay/shared/enemies/gadgebot_spawn_pad.h"
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
s32            get_morphoray_chicken_slot_record(s32) __asm__("FUN_L00_002caf40");
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

void *default_moby_methods[6] __asm__("D_L06_00202384")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L06_0020239C[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            get_morphoray_chicken_slot_record,
                                            hud_counter_update};

void *D_L06_002023B4[6]

    __attribute__((section("lvl.vtbl"))) = {update_moby_grab_state,
                                            update_moby_suck_cannon_storage_state,
                                            update_moby_suck_cannon_storage_state,
                                            cancel_moby_suction_and_save_state,
                                            get_moby_data_plus_0x60,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[171] __asm__("D_L06_00201B80")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,            camera_object_update, default_moby_methods},
        {  11,                   vendor_update, default_moby_methods},
        {  13,                     bolt_update, default_moby_methods},
        {  14,                     bolt_update, default_moby_methods},
        {  15,                     bolt_update, default_moby_methods},
        {  16,                     bolt_update, default_moby_methods},
        {  55,              hangar_door_update, default_moby_methods},
        {  71,                   wrench_update, default_moby_methods},
        {  74,                     mine_update, default_moby_methods},
        {  87,              noop_main_callback, default_moby_methods},
        { 112,            glow_particle_update, default_moby_methods},
        { 121,                     bomb_update, default_moby_methods},
        { 122,                   debris_update, default_moby_methods},
        { 149,  nanotech_crate_fragment_update, default_moby_methods},
        { 153,  gold_devastator_missile_update, default_moby_methods},
        { 156,     billboard_projectile_update, default_moby_methods},
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
        { 270,        morphoray_chicken_update,       D_L06_0020239C},
        { 302, gadgebot_spawn_container_update, default_moby_methods},
        { 303,       gadgebot_spawn_pad_update, default_moby_methods},
        { 305,              homing_shot_update, default_moby_methods},
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
        { 367,       alien_queen_slider_update, default_moby_methods},
        { 428,      damped_falling_moby_update, default_moby_methods},
        { 433,              noop_main_callback, default_moby_methods},
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
        { 585,           metal_detector_update, default_moby_methods},
        { 604,          sandmouse_house_update, default_moby_methods},
        { 605,             buried_bolts_update, default_moby_methods},
        { 607,                 helipack_update, default_moby_methods},
        { 615,          tresspasser_pad_update, default_moby_methods},
        { 618,              noop_main_callback, default_moby_methods},
        { 634,                               0, default_moby_methods},
        { 639,          explosion_light_update, default_moby_methods},
        { 750,                  infobot_update, default_moby_methods},
        { 775,        grow_and_fade_out_update, default_moby_methods},
        { 803,    swingshot_versatarget_update, default_moby_methods},
        { 804,  morphoray_target_effect_update, default_moby_methods},
        { 805,              spawn_point_update, default_moby_methods},
        { 806,            nanotech_ball_update, default_moby_methods},
        { 827,                     frog_update,       D_L06_002023B4},
        { 832,      visibomb_zone_timer_update, default_moby_methods},
        { 849,               suckcannon_update, default_moby_methods},
        { 857,                 gadgebot_update, default_moby_methods},
        { 911,           toxic_gas_vent_update, default_moby_methods},
        {1006,          devestator_ammo_update, default_moby_methods},
        {1016,              tresspasser_update, default_moby_methods},
        {1021,      moby_rotation_group_update, default_moby_methods},
        {1028,               drawbridge_update, default_moby_methods},
        {1035,             beam_barrier_update, default_moby_methods},
        {1038,     energy_sphere_holder_update, default_moby_methods},
        {1039,           respawn_volume_update, default_moby_methods},
        {1040,            energy_sphere_update, default_moby_methods},
        {1048,            alien_snapper_update, default_moby_methods},
        {1051,              alien_queen_update, default_moby_methods},
        {1054,               split_door_update, default_moby_methods},
        {1061,                 elevator_update, default_moby_methods},
        {1062,        dual_texture_mesh_update, default_moby_methods},
        {1066,    alien_snapper_spawner_update, default_moby_methods},
        {1068,     blarg_space_commando_update, default_moby_methods},
        {1083,         breakable_window_update, default_moby_methods},
        {1085,    falling_spinning_moby_update, default_moby_methods},
        {1086,    falling_spinning_moby_update, default_moby_methods},
        {1087,    falling_spinning_moby_update, default_moby_methods},
        {1088,    falling_spinning_moby_update, default_moby_methods},
        {1089,    falling_spinning_moby_update, default_moby_methods},
        {1091,          breakaway_piece_update, default_moby_methods},
        {1092,          breakaway_piece_update, default_moby_methods},
        {1093,          breakaway_piece_update, default_moby_methods},
        {1094,          breakaway_piece_update, default_moby_methods},
        {1095,          breakaway_piece_update, default_moby_methods},
        {1096,          breakaway_piece_update, default_moby_methods},
        {1097,          breakaway_piece_update, default_moby_methods},
        {1098,          breakaway_piece_update, default_moby_methods},
        {1103,          breakaway_piece_update, default_moby_methods},
        {1105,                     fred_update, default_moby_methods},
        {1108,       countdown_sequence_update, default_moby_methods},
        {1109,                  shuttle_update, default_moby_methods},
        {1118,               red_button_update, default_moby_methods},
        {1123,  switchable_beam_barrier_update, default_moby_methods},
        {1134,                gold_bolt_update, default_moby_methods},
        {1138,                quad_moby_update, default_moby_methods},
        {1143,               vendor_orb_update, default_moby_methods},
        {1192,            glow_particle_update, default_moby_methods},
        {1251,           hydrodisplacer_update, default_moby_methods},
        {1289,              noop_main_callback, default_moby_methods},
        {1290,            pilots_helmet_update, default_moby_methods},
        {1302,            bot_door_lock_update, default_moby_methods},
        {1348,               help_hints_update, default_moby_methods},
        {1438,          devestator_ammo_update, default_moby_methods},
        {1447,          devestator_ammo_update, default_moby_methods},
        {1449,          devestator_ammo_update, default_moby_methods},
        {1504,   flickering_point_light_update, default_moby_methods},
        {1512,            spark_emitter_update, default_moby_methods},
        {1551,   cutscene_camera_target_update, default_moby_methods},
        {1633,     sandmouse_projectile_update, default_moby_methods},
        {1655,             prop_breakup_update, default_moby_methods},
        {1656,            tumbling_part_update, default_moby_methods},
        {1657,            tumbling_part_update, default_moby_methods},
        {1658,            tumbling_part_update, default_moby_methods},
        {1659,            tumbling_part_update, default_moby_methods},
        {1660,           breakable_prop_update, default_moby_methods},
        {1661,            tumbling_part_update, default_moby_methods},
        {1662,            tumbling_part_update, default_moby_methods},
        {1664,   breakable_prop_variant_update, default_moby_methods},
        {1665,            tumbling_part_update, default_moby_methods},
        {1666,            tumbling_part_update, default_moby_methods},
        {1672,              noop_main_callback, default_moby_methods},
        {1673,            tumbling_part_update, default_moby_methods},
        {1674,            tumbling_part_update, default_moby_methods},
        {1675,            tumbling_part_update, default_moby_methods},
        {1676,            tumbling_part_update, default_moby_methods},
        {1695,            tumbling_part_update, default_moby_methods},
        {1696,            tumbling_part_update, default_moby_methods},
        {1697,            tumbling_part_update, default_moby_methods},
        {1698,            tumbling_part_update, default_moby_methods},
        {1699,            tumbling_part_update, default_moby_methods},
        {1700,            tumbling_part_update, default_moby_methods},
        {1701,            tumbling_part_update, default_moby_methods},
        {1702,            tumbling_part_update, default_moby_methods},
        {1703,            tumbling_part_update, default_moby_methods},
        {1704,            tumbling_part_update, default_moby_methods},
        {1818,                sandmouse_update, default_moby_methods},
        {1898,      timed_sprite_effect_update, default_moby_methods},
        {1900,                    decoy_update, default_moby_methods},
        {  -1,                               0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L06_00202400") __attribute__((section("lvl.camvtbl"))) = {
    { 0,    noop_callback_l,                 follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                         },
    { 4,
     camera_transition_update, initialize_hero_aligned_camera_mode,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                    },
    { 5,
     noop_callback_l,      initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                               },
    { 6,            noop_callback_l,   initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l,   initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                            },
    {17,  volume_camera_swing_check,                  noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {18, update_moby_camera_control,                  noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,        volume_camera_check,                  noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                          0,                                   0,                    0,                                           0},
};

struct SoundHandler sound_handlers[1] __asm__("D_L06_00202500")

    __attribute__((section("lvl.sndvtbl"))) = {
        {-1, 0},
};
