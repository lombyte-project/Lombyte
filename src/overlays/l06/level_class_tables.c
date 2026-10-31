/* The level's moby classes, camera types and sound emitter types: the tables
   the executable's code looks ids up in (sections lvl.vtbl, lvl.camvtbl and
   lvl.sndvtbl). */
#include "types.h"
#include "rnc/audio/sound_handlers.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby_class_tables.h"

void FUN_L00_002caf40();
void FUN_L00_002ea7a0();
void FUN_L00_002ed230();
void alien_queen_slider_update() __asm__("FUN_L06_002d9d10");
void alien_queen_update() __asm__("FUN_L06_002f9a28");
void alien_snapper_spawner_update() __asm__("FUN_L06_002fda30");
void alien_snapper_update() __asm__("FUN_L06_002f7d78");
void beam_barrier_update() __asm__("FUN_L06_002f6470");
void billboard_projectile_update() __asm__("FUN_L01_002c7530");
void blarg_space_commando_update() __asm__("FUN_L06_002fdbd0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void bot_door_lock_update() __asm__("FUN_L06_00307d30");
void breakable_prop_update() __asm__("FUN_L06_00309e08");
void breakable_prop_variant_update() __asm__("FUN_L06_00309fa8");
void breakable_window_update() __asm__("FUN_L06_002ffcb0");
void breakaway_piece_update() __asm__("FUN_L06_00300df0");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_moby_suction_and_save_state() __asm__("FUN_L06_002ea7c8");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void countdown_sequence_update() __asm__("FUN_L06_00301b90");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_camera_target_update() __asm__("FUN_L06_00309348");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drawbridge_update() __asm__("FUN_L06_002f55a0");
void drone_update() __asm__("FUN_L00_002ce358");
void dual_texture_mesh_update() __asm__("FUN_L06_002fd088");
void elevator_update() __asm__("FUN_L06_002fc640");
void energy_sphere_holder_update() __asm__("FUN_L06_002f7288");
void energy_sphere_update() __asm__("FUN_L06_002f7ab8");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void falling_spinning_moby_update() __asm__("FUN_L06_00300b90");
void flickering_point_light_update() __asm__("FUN_L01_0030b618");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void fred_update() __asm__("FUN_L06_00301070");
void frog_update() __asm__("FUN_L06_002e8678");
void gadgebot_spawn_container_update() __asm__("FUN_L06_002d85a0");
void gadgebot_spawn_pad_update() __asm__("FUN_L06_002d8c20");
void gadgebot_update() __asm__("FUN_L06_002f0040");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void hangar_door_update() __asm__("FUN_L06_002b4770");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L06_003083b0");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_rotation_group_update() __asm__("FUN_L06_002f4f00");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void prop_breakup_update() __asm__("FUN_L06_00309860");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void red_button_update() __asm__("FUN_L06_00304098");
void respawn_volume_update() __asm__("FUN_L06_002f7930");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_projectile_update() __asm__("FUN_L01_0030c9a8");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void shuttle_update() __asm__("FUN_L06_00302578");
void spark_emitter_update() __asm__("FUN_L06_00308c68");
void spawn_point_update() __asm__("FUN_L01_00300220");
void split_door_update() __asm__("FUN_L06_002fbfb0");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void switchable_beam_barrier_update() __asm__("FUN_L06_003049f8");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void timed_sprite_effect_update() __asm__("FUN_L06_0030be60");
void toxic_gas_vent_update() __asm__("FUN_L06_002f3ad8");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void tresspasser_update() __asm__("FUN_L02_002ea210");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_moby_grab_state() __asm__("FUN_L06_002ea5e0");
void update_moby_suck_cannon_storage_state() __asm__("FUN_L06_002ea6a8");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void visibomb_zone_timer_update() __asm__("FUN_L01_00302648");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
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
                                            FUN_L00_002caf40,
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
    { 0,    noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                       },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                  },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                             },
    { 6,            noop_callback_l, initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                          },
    {17,           FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {18, update_moby_camera_control,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,        volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                          0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[1] __asm__("D_L06_00202500")

    __attribute__((section("lvl.sndvtbl"))) = {
        {-1, 0},
};
