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
void FUN_L03_002eb978();
void FUN_L03_002ebd70();
void big_al_update() __asm__("FUN_L03_002db558");
void blarg_helicopter_update() __asm__("FUN_L03_002cca00");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_spawner_bomb_update() __asm__("FUN_L03_002c4718");
void bomb_spawner_update() __asm__("FUN_L03_002bef68");
void bomb_update() __asm__("FUN_L00_002adb30");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_trigger_update() __asm__("FUN_L01_002fb5b0");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drone_update() __asm__("FUN_L00_002ce358");
void evaluate_clip_volume_candidate() __asm__("FUN_L03_002e8870");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void falling_moby_update() __asm__("FUN_L03_002de370");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void fred_1_update() __asm__("FUN_L03_002dc310");
void garage_door_update() __asm__("FUN_L03_002dca30");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void gunship_update() __asm__("FUN_L01_002f5168");
void helga_update() __asm__("FUN_L03_002da870");
void helicopter_spark_update() __asm__("FUN_L03_002d47c0");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L03_002df520");
void homing_rocket_update() __asm__("FUN_L03_002d43c8");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void kerwan_cutscene_effects_update() __asm__("FUN_L03_002e01d0");
void kerwan_path_enemy_update() __asm__("FUN_L03_002c6fd0");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void lightpost_update() __asm__("FUN_L03_002e2948");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void metropolis_train_carriage_update() __asm__("FUN_L03_002953f8");
void metropolis_train_locomotive_update() __asm__("FUN_L03_00292e98");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_layer_mine_update() __asm__("FUN_L03_002cbea8");
void mine_layer_update() __asm__("FUN_L03_002c9eb8");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_draw_callback_update() __asm__("FUN_L03_002dc9b0");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void moving_platform_small_update() __asm__("FUN_L03_00294c08");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void novalis_elevator_update() __asm__("FUN_L01_002b9eb0");
void parent_following_effect_update() __asm__("FUN_L03_002bae48");
void path_mover_spawner_update() __asm__("FUN_L03_002db280");
void path_mover_update() __asm__("FUN_L03_002db020");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void qwark_rilgar_update() __asm__("FUN_L03_002dc478");
void rail_lift_update() __asm__("FUN_L03_002dccc0");
void rail_taxi_update() __asm__("FUN_L03_002d3198");
void red_ship_1_update() __asm__("FUN_L03_0029dba8");
void robo_shack_sign_update() __asm__("FUN_L03_002d3e58");
void robot_dog_update() __asm__("FUN_L03_002c5bb8");
void robot_qwark_update() __asm__("FUN_L03_002dbd90");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_projectile_update() __asm__("FUN_L01_0030c9a8");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void set_transform_state_three_when_outside_clip_volume() __asm__("FUN_L03_002e8980");
void spawn_point_update() __asm__("FUN_L01_00300220");
void statue_update() __asm__("FUN_L03_002d3cd8");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation() __asm__("FUN_L03_002ebdb0");
void update_camera_state_outside_clip_volume() __asm__("FUN_L03_002ebde8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void visibomb_zone_timer_update() __asm__("FUN_L01_00302648");
void walloper_update() __asm__("FUN_L00_002bb558");
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
    { 0,    noop_callback_l,               follow_camera_setup,follow_camera_update,                                 noop_main_callback                                                                       },
    { 1,
     evaluate_clip_volume_candidate,                hud_counter_update,
     hud_counter_update, set_transform_state_three_when_outside_clip_volume                                                                     },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input,        select_camera_mode_by_hero_vector_alignment                                                  },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                                 noop_main_callback                                             },
    { 6,            noop_callback_l, initialize_orbit_camera_from_hero,   hud_counter_update,                                 noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,               conditionally_set_camera_state_three                                                          },
    {14,
     FUN_L03_002ebd70,                  FUN_L03_002eb978,
     update_camera_position_and_orientation,            update_camera_state_outside_clip_volume                                                 },
    {17,           FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {18, update_moby_camera_control,                noop_main_callback,   noop_main_callback,                                 noop_main_callback},
    {-1,                          0,                                 0,                    0,                                                  0},
};

struct SoundHandler sound_handlers[4] __asm__("D_L03_001E4280")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 3,                 update_reverb_box_emitter},
        { 6,               update_music_switch_emitter},
        {-1,                                         0},
};
