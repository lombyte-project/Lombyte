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
void FUN_L10_002f5f18();
void audio_stream_callback_keeper_update() __asm__("FUN_L14_00303370");
void beam_slots_update() __asm__("FUN_L14_003039e0");
void blarg_heavy_interceptor_update() __asm__("FUN_L14_002ac618");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_thief_mine_update() __asm__("FUN_L14_002ff640");
void bolt_thief_update() __asm__("FUN_L14_002fe2a0");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_spawner_bomb_update() __asm__("FUN_L03_002c4718");
void bomb_spawner_update() __asm__("FUN_L03_002bef68");
void bomb_update() __asm__("FUN_L00_002adb30");
void bouncing_spark_update() __asm__("FUN_L14_00300e00");
void breakable_lamp_update() __asm__("FUN_L05_0031c8e0");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void button_update() __asm__("FUN_L14_00306b78");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void check_path_camera_priority() __asm__("FUN_L14_00315920");
void check_path_key_camera_priority() __asm__("FUN_L14_00316748");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drone_update() __asm__("FUN_L00_002ce358");
void electric_beam_update() __asm__("FUN_L14_003015d0");
void elevator_update() __asm__("FUN_L14_002ee8d8");
void enemy_fighter_update() __asm__("FUN_L14_002fc0f0");
void enter_scoutbot_return_home_state() __asm__("FUN_L08_002d4050");
void evaluate_descriptor_eligibility() __asm__("FUN_L10_002f65f8");
void exit_path_key_camera() __asm__("FUN_L14_003167e0");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void explosive_projectile_update() __asm__("FUN_L01_002f6a30");
void flying_bolt_update() __asm__("FUN_L14_002ece00");
void flying_car_update() __asm__("FUN_L14_00306ee0");
void flying_debris_update() __asm__("FUN_L14_002bac78");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void ground_particle_scatter_update() __asm__("FUN_L08_00307cf0");
void group_clear_trigger_update() __asm__("FUN_L14_003087c0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_path_camera() __asm__("FUN_L10_002f5b58");
void initialize_path_key_camera() __asm__("FUN_L14_003159d8");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void level14_racer_update() __asm__("FUN_L14_002d5f40");
void level_effects_update() __asm__("FUN_L14_00307a80");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void looping_path_mine_update() __asm__("FUN_L14_002ec810");
void machinegun_turret_update() __asm__("FUN_L14_002b3bf0");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void minibomber_update() __asm__("FUN_L14_002b46e8");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void moby_spatial_trigger_update() __asm__("FUN_L05_002f5200");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_pickup_update() __asm__("FUN_L14_00305758");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void particle_spawner_smoke_fire_update() __asm__("FUN_L00_002d6f28");
void path_lightning_controller_update() __asm__("FUN_L14_002ed280");
void path_mover_update() __asm__("FUN_L14_002aba80");
void path_or_ballistic_debris_update() __asm__("FUN_L14_002dfd10");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void plasmabot_death_debris_update() __asm__("FUN_L14_002de670");
void plasmabot_hatch_update() __asm__("FUN_L14_002d96e0");
void plasmabot_update() __asm__("FUN_L14_002b17d8");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void qwark_oltanis_update() __asm__("FUN_L14_002fba20");
void radial_particle_emitter_update() __asm__("FUN_L08_003085f0");
void ribbon_effect_moby_update() __asm__("FUN_L14_002dee28");
void rocket_strike_fighter_update() __asm__("FUN_L14_002fcbb0");
void rocket_update() __asm__("FUN_L14_002fdbb8");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void sam_update() __asm__("FUN_L14_002fefe0");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_projectile_update() __asm__("FUN_L01_0030c9a8");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void scoutbot_update() __asm__("FUN_L08_002d2af0");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void setup_path_camera() __asm__("FUN_L14_00314f00");
void shuttle_update() __asm__("FUN_L14_002e7bd8");
void smoke_fire_controller_update() __asm__("FUN_L00_002a4cd8");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void taxi_update() __asm__("FUN_L14_002f0538");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void tilting_platform_update() __asm__("FUN_L14_002de1f8");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void unfolding_column_piece_update() __asm__("FUN_L14_003061d8");
void unfolding_column_update() __asm__("FUN_L14_00305d28");
void unlock_sequence_controller_update() __asm__("FUN_L14_00305408");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_ground_height_modulated_voice() __asm__("FUN_L08_00316fa8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_path_camera() __asm__("FUN_L14_00315290");
void update_path_key_camera() __asm__("FUN_L14_00315d48");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void visibomb_zone_timer_update() __asm__("FUN_L01_00302648");
void walloper_update() __asm__("FUN_L00_002bb558");
void wind_control_update() __asm__("FUN_L14_002eaf88");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L14_001FD9B8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L14_001FD9D0[6] __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                moby_set_state_from_check,
                                                                enter_scoutbot_return_home_state,
                                                                get_moby_data_plus_0x60,
                                                                hud_counter_update};

void *D_L14_001FD9E8[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[154] __asm__("D_L14_001FD280")

    __attribute__((section("lvl.vtbl"))) = {
        { 809,        smoke_fire_controller_update, default_moby_methods},
        { 903,                   path_mover_update, default_moby_methods},
        {1007,                camera_object_update, default_moby_methods},
        {   8,      blarg_heavy_interceptor_update, default_moby_methods},
        {  11,                       vendor_update, default_moby_methods},
        {  13,                         bolt_update, default_moby_methods},
        {  14,                         bolt_update, default_moby_methods},
        {  15,                         bolt_update, default_moby_methods},
        {  16,                         bolt_update, default_moby_methods},
        {  28,                    plasmabot_update, default_moby_methods},
        {  30,            machinegun_turret_update, default_moby_methods},
        {  31,                   minibomber_update, default_moby_methods},
        {  71,                       wrench_update, default_moby_methods},
        {  74,                         mine_update, default_moby_methods},
        {  81,                flying_debris_update, default_moby_methods},
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
        { 211,                level14_racer_update, default_moby_methods},
        { 213,              devestator_ammo_update, default_moby_methods},
        { 214,              devestator_ammo_update, default_moby_methods},
        { 222,              devestator_ammo_update, default_moby_methods},
        { 223,              devestator_ammo_update, default_moby_methods},
        { 225,              devestator_ammo_update, default_moby_methods},
        { 226,              devestator_ammo_update, default_moby_methods},
        { 229,                glove_of_doom_update, default_moby_methods},
        { 230,             doom_bot_spawner_update, default_moby_methods},
        { 250,              plasmabot_hatch_update, default_moby_methods},
        { 252,                     scoutbot_update,       D_L14_001FD9D0},
        { 258,         moby_spatial_trigger_update, default_moby_methods},
        { 270,            morphoray_chicken_update,       D_L14_001FD9E8},
        { 305,                  homing_shot_update, default_moby_methods},
        { 309,             tilting_platform_update, default_moby_methods},
        { 325,       plasmabot_death_debris_update, default_moby_methods},
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
        { 386,           ribbon_effect_moby_update, default_moby_methods},
        { 403,     path_or_ballistic_debris_update, default_moby_methods},
        { 428,          damped_falling_moby_update, default_moby_methods},
        { 433,                  noop_main_callback, default_moby_methods},
        { 454,                         ryno_update, default_moby_methods},
        { 455,                 bomb_spawner_update, default_moby_methods},
        { 457,                  ryno_rocket_update, default_moby_methods},
        { 479,                        drone_update, default_moby_methods},
        { 483,                                   0, default_moby_methods},
        { 500,                        crate_update, default_moby_methods},
        { 501,                        crate_update, default_moby_methods},
        { 502,                        crate_update, default_moby_methods},
        { 505,                        crate_update, default_moby_methods},
        { 511,                        crate_update, default_moby_methods},
        { 531,                  noop_main_callback, default_moby_methods},
        { 545,            bomb_spawner_bomb_update, default_moby_methods},
        { 557,                      shuttle_update, default_moby_methods},
        { 562,                  decoy_glove_update, default_moby_methods},
        { 585,               metal_detector_update, default_moby_methods},
        { 604,              sandmouse_house_update, default_moby_methods},
        { 605,                 buried_bolts_update, default_moby_methods},
        { 607,                     helipack_update, default_moby_methods},
        { 610,                 wind_control_update, default_moby_methods},
        { 618,                  noop_main_callback, default_moby_methods},
        { 634,                                   0, default_moby_methods},
        { 639,              explosion_light_update, default_moby_methods},
        { 643,            looping_path_mine_update, default_moby_methods},
        { 681,                  flying_bolt_update, default_moby_methods},
        { 684,    path_lightning_controller_update, default_moby_methods},
        { 685,                     elevator_update, default_moby_methods},
        { 686,         explosive_projectile_update, default_moby_methods},
        { 712,                         taxi_update, default_moby_methods},
        { 750,                      infobot_update, default_moby_methods},
        { 758,        swingshot_versatarget_update, default_moby_methods},
        { 760,  particle_spawner_smoke_fire_update, default_moby_methods},
        { 765,                  noop_main_callback, default_moby_methods},
        { 775,            grow_and_fade_out_update, default_moby_methods},
        { 803,        swingshot_versatarget_update, default_moby_methods},
        { 804,      morphoray_target_effect_update, default_moby_methods},
        { 805,                  spawn_point_update, default_moby_methods},
        { 806,                nanotech_ball_update, default_moby_methods},
        { 832,          visibomb_zone_timer_update, default_moby_methods},
        { 849,                   suckcannon_update, default_moby_methods},
        { 851,                qwark_oltanis_update, default_moby_methods},
        { 908,                enemy_fighter_update, default_moby_methods},
        { 913,                  noop_main_callback, default_moby_methods},
        { 921,        rocket_strike_fighter_update, default_moby_methods},
        { 922,                       rocket_update, default_moby_methods},
        { 923,                   bolt_thief_update, default_moby_methods},
        { 924,                          sam_update, default_moby_methods},
        { 933,              bolt_thief_mine_update, default_moby_methods},
        {1006,              devestator_ammo_update, default_moby_methods},
        {1134,                    gold_bolt_update, default_moby_methods},
        {1138,                    quad_moby_update, default_moby_methods},
        {1143,                   vendor_orb_update, default_moby_methods},
        {1192,                glow_particle_update, default_moby_methods},
        {1193,               bouncing_spark_update, default_moby_methods},
        {1224,                electric_beam_update, default_moby_methods},
        {1251,               hydrodisplacer_update, default_moby_methods},
        {1260, audio_stream_callback_keeper_update, default_moby_methods},
        {1289,                  noop_main_callback, default_moby_methods},
        {1290,                pilots_helmet_update, default_moby_methods},
        {1331,                   beam_slots_update, default_moby_methods},
        {1352,   unlock_sequence_controller_update, default_moby_methods},
        {1354,             morphoray_pickup_update, default_moby_methods},
        {1395,             unfolding_column_update, default_moby_methods},
        {1396,                  noop_main_callback, default_moby_methods},
        {1397,       unfolding_column_piece_update, default_moby_methods},
        {1400,      ground_particle_scatter_update, default_moby_methods},
        {1416,                       button_update, default_moby_methods},
        {1417,                   flying_car_update, default_moby_methods},
        {1418,                level_effects_update, default_moby_methods},
        {1438,              devestator_ammo_update, default_moby_methods},
        {1447,              devestator_ammo_update, default_moby_methods},
        {1449,              devestator_ammo_update, default_moby_methods},
        {1511,               breakable_lamp_update, default_moby_methods},
        {1514,                  noop_main_callback, default_moby_methods},
        {1517,                tumbling_part_update, default_moby_methods},
        {1518,                tumbling_part_update, default_moby_methods},
        {1559,          group_clear_trigger_update, default_moby_methods},
        {1629,      radial_particle_emitter_update, default_moby_methods},
        {1633,         sandmouse_projectile_update, default_moby_methods},
        {1733,                tumbling_part_update, default_moby_methods},
        {1734,                tumbling_part_update, default_moby_methods},
        {1735,                tumbling_part_update, default_moby_methods},
        {1818,                    sandmouse_update, default_moby_methods},
        {1900,                        decoy_update, default_moby_methods},
        {  -1,                                   0, default_moby_methods},
};

struct CameraType level_camera_types[10] __asm__("D_L14_001FDA00") __attribute__((section("lvl.camvtbl"))) = {
    { 0,     noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                        },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                   },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                              },
    { 6,             noop_callback_l, initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                           },
    {17,            FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {19,
     evaluate_descriptor_eligibility,            initialize_path_camera,
     FUN_L10_002f5f18,                          noop_main_callback                                                                        },
    {20,  check_path_camera_priority,                 setup_path_camera,   update_path_camera,                          noop_main_callback},
    {21,
     check_path_key_camera_priority,        initialize_path_key_camera,
     update_path_key_camera,                        exit_path_key_camera                                                                  },
    {-1,                           0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[6] __asm__("D_L14_001FDB00")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 1,                        update_box_emitter},
        { 3,                 update_reverb_box_emitter},
        { 4,      update_ground_height_modulated_voice},
        { 6,               update_music_switch_emitter},
        {-1,                                         0},
};
