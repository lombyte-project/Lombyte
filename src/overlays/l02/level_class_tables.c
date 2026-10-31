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
void background_ship_1_update() __asm__("FUN_L02_002ec4b8");
void background_ship_2_update() __asm__("FUN_L02_002ece18");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void bulldozer_update() __asm__("FUN_L02_002dca10");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_moby_state_9() __asm__("FUN_L00_002d57d0");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void constructorbot_update() __asm__("FUN_L02_002d7748");
void crane_update() __asm__("FUN_L02_002df1a8");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void elevator_update() __asm__("FUN_L02_002dbd38");
void escalator_update() __asm__("FUN_L02_002dc6b0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grab_crane_update() __asm__("FUN_L02_002dd370");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void handle_sand_shark_suck_cannon_storage() __asm__("FUN_L02_002d6210");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void liftcar_platform_update() __asm__("FUN_L02_002e2228");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void ocean_aridia_update() __asm__("FUN_L02_002ea198");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void platform_update() __asm__("FUN_L02_002ddc00");
void prepare_sand_shark_for_relaunch_or_remove() __asm__("FUN_L02_002d6340");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void red_orb_update() __asm__("FUN_L00_002df4f8");
void red_plant_update() __asm__("FUN_L00_002e3ca8");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void rock_update() __asm__("FUN_L01_002f8718");
void rocket_door1_update() __asm__("FUN_L02_002df948");
void rocket_launcher_update() __asm__("FUN_L02_002df3d8");
void rocks_update() __asm__("FUN_L02_002e0720");
void ryno_update() __asm__("FUN_L00_002cc608");
void sand_mouse_update() __asm__("FUN_L01_002e3f20");
void sand_shark_nest_update() __asm__("FUN_L02_002dcb38");
void sand_shark_update() __asm__("FUN_L02_002d3e50");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void sandshark_egg_update() __asm__("FUN_L02_002e0280");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void skid_mcmarx_update() __asm__("FUN_L02_002e1950");
void skids_agent_update() __asm__("FUN_L02_002e0dc0");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void trackdrive_left_update() __asm__("FUN_L02_002cb978");
void transport_pod_update() __asm__("FUN_L02_002ddc88");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_door1_update() __asm__("FUN_L02_002dffc8");
void tresspasser_door2_update() __asm__("FUN_L02_002e0138");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void tresspasser_update() __asm__("FUN_L02_002ea210");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_cutscene_trigger() __asm__("FUN_L01_002fb5b0");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_doom_bot_spawner() __asm__("FUN_L00_002c89e8");
void update_drone() __asm__("FUN_L00_002ce358");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_help_hints() __asm__("FUN_L02_002ee890");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_moby_suck_cannon_state() __asm__("FUN_L02_002d61c0");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_rocket_launcher_projectile() __asm__("FUN_L02_002ebf20");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void update_sandmouse_projectile() __asm__("FUN_L01_0030c9a8");
void update_smoke_vent() __asm__("FUN_L02_002ef020");
void update_sound_emitter_playback_at_random_position() __asm__("FUN_L00_002edad8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void walloper_update() __asm__("FUN_L00_002bb558");
void white_flower_update() __asm__("FUN_L02_002f1f60");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L02_001FB6AC")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L02_001FB6C4[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

void *D_L02_001FB6DC[6]

    __attribute__((section("lvl.vtbl"))) = {update_moby_suck_cannon_state,
                                            handle_sand_shark_suck_cannon_storage,
                                            update_moby_suck_cannon_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_9,
                                            get_moby_data_plus_0x60,
                                            prepare_sand_shark_for_relaunch_or_remove};

struct MobyClassUpdate moby_classes[153] __asm__("D_L02_001FAF80")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,                      camera_object_update, default_moby_methods},
        {  11,                             vendor_update, default_moby_methods},
        {  13,                               bolt_update, default_moby_methods},
        {  14,                               bolt_update, default_moby_methods},
        {  15,                               bolt_update, default_moby_methods},
        {  16,                               bolt_update, default_moby_methods},
        {  71,                             wrench_update, default_moby_methods},
        {  74,                               mine_update, default_moby_methods},
        { 112,                      update_glow_particle, default_moby_methods},
        { 121,                               bomb_update, default_moby_methods},
        { 122,                             debris_update, default_moby_methods},
        { 149,            nanotech_crate_fragment_update, default_moby_methods},
        { 153,            gold_devastator_missile_update, default_moby_methods},
        { 157,                         devastator_update, default_moby_methods},
        { 163,                           visibomb_update, default_moby_methods},
        { 166,                 update_linked_moby_states, default_moby_methods},
        { 167,                update_thruster_pack_flame, default_moby_methods},
        { 168,                            blaster_update, default_moby_methods},
        { 172,                   visibomb_missile_update, default_moby_methods},
        { 175,                            taunter_update, default_moby_methods},
        { 176,                          pyrocitor_update, default_moby_methods},
        { 177,                         tesla_claw_update, default_moby_methods},
        { 179,     update_linked_moby_scale_and_rotation, default_moby_methods},
        { 180,                           walloper_update, default_moby_methods},
        { 185,                          morphoray_update, default_moby_methods},
        { 186,                           doom_bot_update, default_moby_methods},
        { 188,                         trespasser_update, default_moby_methods},
        { 190,                         mine_glove_update, default_moby_methods},
        { 192,                         bomb_glove_update, default_moby_methods},
        { 203,                              decoy_update, default_moby_methods},
        { 204,                    devestator_ammo_update, default_moby_methods},
        { 208,                          swingshot_update, default_moby_methods},
        { 209,                     swingshot_hook_update, default_moby_methods},
        { 213,                    devestator_ammo_update, default_moby_methods},
        { 214,                    devestator_ammo_update, default_moby_methods},
        { 222,                    devestator_ammo_update, default_moby_methods},
        { 223,                    devestator_ammo_update, default_moby_methods},
        { 225,                    devestator_ammo_update, default_moby_methods},
        { 226,                    devestator_ammo_update, default_moby_methods},
        { 229,                      glove_of_doom_update, default_moby_methods},
        { 230,                   update_doom_bot_spawner, default_moby_methods},
        { 270,                  update_morphoray_chicken,       D_L02_001FB6C4},
        { 305,                        update_homing_shot, default_moby_methods},
        { 331,                    trackdrive_left_update, default_moby_methods},
        { 348,            nanotech_crate_fragment_update, default_moby_methods},
        { 349,            nanotech_crate_fragment_update, default_moby_methods},
        { 354,            nanotech_crate_fragment_update, default_moby_methods},
        { 355,            nanotech_crate_fragment_update, default_moby_methods},
        { 356,            nanotech_crate_fragment_update, default_moby_methods},
        { 357,            nanotech_crate_fragment_update, default_moby_methods},
        { 358,            nanotech_crate_fragment_update, default_moby_methods},
        { 359,            nanotech_crate_fragment_update, default_moby_methods},
        { 363,            nanotech_crate_fragment_update, default_moby_methods},
        { 364,            nanotech_crate_fragment_update, default_moby_methods},
        { 428,                update_damped_falling_moby, default_moby_methods},
        { 433,                        noop_main_callback, default_moby_methods},
        { 437,                         sand_mouse_update, default_moby_methods},
        { 454,                               ryno_update, default_moby_methods},
        { 457,                        update_ryno_rocket, default_moby_methods},
        { 479,                              update_drone, default_moby_methods},
        { 483,                                         0, default_moby_methods},
        { 500,                              crate_update, default_moby_methods},
        { 501,                              crate_update, default_moby_methods},
        { 505,                              crate_update, default_moby_methods},
        { 511,                              crate_update, default_moby_methods},
        { 562,                        decoy_glove_update, default_moby_methods},
        { 580,                         sand_shark_update,       D_L02_001FB6DC},
        { 585,                     metal_detector_update, default_moby_methods},
        { 604,                    sandmouse_house_update, default_moby_methods},
        { 605,        update_nearest_area_moby_selection, default_moby_methods},
        { 607,                           helipack_update, default_moby_methods},
        { 612,                     constructorbot_update, default_moby_methods},
        { 615,                    tresspasser_pad_update, default_moby_methods},
        { 618,                        noop_main_callback, default_moby_methods},
        { 634,                                         0, default_moby_methods},
        { 639,          update_animated_point_light_moby, default_moby_methods},
        { 651,                           elevator_update, default_moby_methods},
        { 652,                          escalator_update, default_moby_methods},
        { 653,                          escalator_update, default_moby_methods},
        { 656,                          bulldozer_update, default_moby_methods},
        { 668,                    sand_shark_nest_update, default_moby_methods},
        { 675,                         grab_crane_update, default_moby_methods},
        { 690,                        noop_main_callback, default_moby_methods},
        { 696,                               rock_update, default_moby_methods},
        { 697,                               rock_update, default_moby_methods},
        { 698,                               rock_update, default_moby_methods},
        { 707,                           platform_update, default_moby_methods},
        { 713,                      transport_pod_update, default_moby_methods},
        { 732,                              crane_update, default_moby_methods},
        { 733,                    rocket_launcher_update, default_moby_methods},
        { 734,                           platform_update, default_moby_methods},
        { 735,                       rocket_door1_update, default_moby_methods},
        { 736,                       rocket_door1_update, default_moby_methods},
        { 737,                   update_cutscene_trigger, default_moby_methods},
        { 743,                  tresspasser_door1_update, default_moby_methods},
        { 744,                  tresspasser_door2_update, default_moby_methods},
        { 755,                      sandshark_egg_update, default_moby_methods},
        { 758,              swingshot_versatarget_update, default_moby_methods},
        { 762,                              rocks_update, default_moby_methods},
        { 766,                          red_plant_update, default_moby_methods},
        { 767,                          red_plant_update, default_moby_methods},
        { 775,                  grow_and_fade_out_update, default_moby_methods},
        { 786,                        skids_agent_update, default_moby_methods},
        { 788,                        skid_mcmarx_update, default_moby_methods},
        { 789,                        noop_main_callback, default_moby_methods},
        { 792,                   liftcar_platform_update, default_moby_methods},
        { 803,              swingshot_versatarget_update, default_moby_methods},
        { 804, update_linked_moby_transform_and_lifetime, default_moby_methods},
        { 805,                        spawn_point_update, default_moby_methods},
        { 806,                      nanotech_ball_update, default_moby_methods},
        { 832,        update_target_outside_zone_timeout, default_moby_methods},
        { 849,                         suckcannon_update, default_moby_methods},
        { 854,                       ocean_aridia_update, default_moby_methods},
        {1005,                        tresspasser_update, default_moby_methods},
        {1006,                    devestator_ammo_update, default_moby_methods},
        {1060,                            red_orb_update, default_moby_methods},
        {1134,                          gold_bolt_update, default_moby_methods},
        {1138,                          quad_moby_update, default_moby_methods},
        {1143,                         vendor_orb_update, default_moby_methods},
        {1192,                      update_glow_particle, default_moby_methods},
        {1208,         update_rocket_launcher_projectile, default_moby_methods},
        {1212,                  background_ship_1_update, default_moby_methods},
        {1213,                  background_ship_2_update, default_moby_methods},
        {1251,                     hydrodisplacer_update, default_moby_methods},
        {1289,                        noop_main_callback, default_moby_methods},
        {1290,                      pilots_helmet_update, default_moby_methods},
        {1324,                         update_help_hints, default_moby_methods},
        {1438,                    devestator_ammo_update, default_moby_methods},
        {1447,                    devestator_ammo_update, default_moby_methods},
        {1449,                    devestator_ammo_update, default_moby_methods},
        {1479,                         update_smoke_vent, default_moby_methods},
        {1505,                          robot_arm_update, default_moby_methods},
        {1506,                          robot_arm_update, default_moby_methods},
        {1522,                          robot_arm_update, default_moby_methods},
        {1525,                          robot_arm_update, default_moby_methods},
        {1526,                          robot_arm_update, default_moby_methods},
        {1528,                          robot_arm_update, default_moby_methods},
        {1529,                          robot_arm_update, default_moby_methods},
        {1633,               update_sandmouse_projectile, default_moby_methods},
        {1751,                          robot_arm_update, default_moby_methods},
        {1752,                          robot_arm_update, default_moby_methods},
        {1753,                          robot_arm_update, default_moby_methods},
        {1764,                          robot_arm_update, default_moby_methods},
        {1765,                          robot_arm_update, default_moby_methods},
        {1769,                          robot_arm_update, default_moby_methods},
        {1818,                          sandmouse_update, default_moby_methods},
        {1819,                       white_flower_update, default_moby_methods},
        {1820,                        noop_main_callback, default_moby_methods},
        {1821,                          robot_arm_update, default_moby_methods},
        {1900,                              decoy_update, default_moby_methods},
        {1922,                          robot_arm_update, default_moby_methods},
        {1936,                          robot_arm_update, default_moby_methods},
        {  -1,                                         0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L02_001FB700") __attribute__((section("lvl.camvtbl"))) = {
    { 0,    noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                       },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                  },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                             },
    { 6,            noop_callback_l, initialize_orbit_camera_from_hero,     mine_blend_color,                          noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                          },
    {17,           FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {18, update_moby_camera_control,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                          0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[5] __asm__("D_L02_001FB800")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0,        update_sound_emitter_playback_by_distance},
        { 2, update_sound_emitter_playback_at_random_position},
        { 3,                        update_reverb_box_emitter},
        { 6,                      update_music_switch_emitter},
        {-1,                                                0},
};
