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
void airship_chain_link_update() __asm__("FUN_L09_00304360");
void asteroid_update() __asm__("FUN_L09_002eb970");
void background_ship_1_update() __asm__("FUN_L02_002ec4b8");
void background_ship_2_update() __asm__("FUN_L02_002ece18");
void barrier_update() __asm__("FUN_L09_003082d8");
void blarg_paratrooper_update() __asm__("FUN_L08_00301158");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void display_update() __asm__("FUN_L09_002edc30");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void elevator_update() __asm__("FUN_L09_003033a0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void lamp_update() __asm__("FUN_L09_0030a678");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_hold_state_if_grabbed() __asm__("FUN_L01_002f1c78");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void platform_update() __asm__("FUN_L09_002f86a0");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void refueling_station_update() __asm__("FUN_L09_00303d10");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void run_moby_helper_and_set_state_7() __asm__("FUN_L01_002f1dc8");
void ryno_update() __asm__("FUN_L00_002cc608");
void seeker_update() __asm__("FUN_L09_002c5990");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void shuttle_update() __asm__("FUN_L09_00309c08");
void spawn_point_update() __asm__("FUN_L01_00300220");
void spawner_egg_update() __asm__("FUN_L09_0030b3b0");
void spawner_update() __asm__("FUN_L09_0030ab80");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void trespasser_update() __asm__("FUN_L00_002c1718");
void turret_update() __asm__("FUN_L09_00304c80");
void update_airship_part() __asm__("FUN_L09_002c26c8");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_billboard_projectile() __asm__("FUN_L01_002c7530");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_damage_causer() __asm__("FUN_L09_00305a28");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_destructible_prop() __asm__("FUN_L09_002ee110");
void update_doom_bot_spawner() __asm__("FUN_L00_002c89e8");
void update_drifting_moby() __asm__("FUN_L09_002f0040");
void update_drone() __asm__("FUN_L00_002ce358");
void update_fire_bolt() __asm__("FUN_L08_00307298");
void update_floating_ship_piece() __asm__("FUN_L09_003091b0");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_ground_melee_enemy() __asm__("FUN_L09_002e27d8");
void update_help_hints() __asm__("FUN_L09_00300888");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_moby_draw_registration() __asm__("FUN_L07_002f8058");
void update_moby_spatial_trigger() __asm__("FUN_L05_002f5200");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void update_shrinking_tumbling_debris() __asm__("FUN_L09_002ef9b0");
void update_smoke_trail_projectile() __asm__("FUN_L01_002eda00");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_tumbling_debris() __asm__("FUN_L09_002ef430");
void update_turret_shot() __asm__("FUN_L09_00307d68");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L09_00209AE8")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L09_00209B00[6] __attribute__((section("lvl.vtbl"))) = {moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                run_moby_helper_and_set_state_7,
                                                                get_moby_data_plus_0x60,
                                                                mine_blend_color};

void *D_L09_00209B18[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[158] __asm__("D_L09_00209380")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,                      camera_object_update, default_moby_methods},
        {1184,                       update_airship_part, default_moby_methods},
        {  11,                             vendor_update, default_moby_methods},
        {  13,                               bolt_update, default_moby_methods},
        {  14,                               bolt_update, default_moby_methods},
        {  15,                               bolt_update, default_moby_methods},
        {  16,                               bolt_update, default_moby_methods},
        {  52,                             seeker_update, default_moby_methods},
        {  71,                             wrench_update, default_moby_methods},
        {  74,                               mine_update, default_moby_methods},
        { 112,                      update_glow_particle, default_moby_methods},
        { 121,                               bomb_update, default_moby_methods},
        { 122,                             debris_update, default_moby_methods},
        { 149,            nanotech_crate_fragment_update, default_moby_methods},
        { 153,            gold_devastator_missile_update, default_moby_methods},
        { 156,               update_billboard_projectile, default_moby_methods},
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
        { 193,                 update_ground_melee_enemy,       D_L09_00209B00},
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
        { 258,               update_moby_spatial_trigger, default_moby_methods},
        { 263,                           asteroid_update, default_moby_methods},
        { 264,                           asteroid_update, default_moby_methods},
        { 265,                           asteroid_update, default_moby_methods},
        { 266,                           asteroid_update, default_moby_methods},
        { 267,                           asteroid_update, default_moby_methods},
        { 270,                  update_morphoray_chicken,       D_L09_00209B18},
        { 276,                            display_update, default_moby_methods},
        { 279,                  update_destructible_prop, default_moby_methods},
        { 284,                        noop_main_callback, default_moby_methods},
        { 305,                        update_homing_shot, default_moby_methods},
        { 313,                    update_tumbling_debris, default_moby_methods},
        { 317,             update_moby_draw_registration, default_moby_methods},
        { 320,          update_shrinking_tumbling_debris, default_moby_methods},
        { 321,          update_shrinking_tumbling_debris, default_moby_methods},
        { 324,                        noop_main_callback, default_moby_methods},
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
        { 417,                      update_drifting_moby, default_moby_methods},
        { 428,                update_damped_falling_moby, default_moby_methods},
        { 433,                        noop_main_callback, default_moby_methods},
        { 454,                               ryno_update, default_moby_methods},
        { 457,                        update_ryno_rocket, default_moby_methods},
        { 479,                              update_drone, default_moby_methods},
        { 483,                                         0, default_moby_methods},
        { 500,                              crate_update, default_moby_methods},
        { 501,                              crate_update, default_moby_methods},
        { 502,                              crate_update, default_moby_methods},
        { 505,                              crate_update, default_moby_methods},
        { 511,                              crate_update, default_moby_methods},
        { 562,                        decoy_glove_update, default_moby_methods},
        { 567,             update_smoke_trail_projectile, default_moby_methods},
        { 585,                     metal_detector_update, default_moby_methods},
        { 605,        update_nearest_area_moby_selection, default_moby_methods},
        { 607,                           helipack_update, default_moby_methods},
        { 618,                        noop_main_callback, default_moby_methods},
        { 634,                                         0, default_moby_methods},
        { 639,          update_animated_point_light_moby, default_moby_methods},
        { 664,                           platform_update, default_moby_methods},
        { 775,                  grow_and_fade_out_update, default_moby_methods},
        { 803,              swingshot_versatarget_update, default_moby_methods},
        { 804, update_linked_moby_transform_and_lifetime, default_moby_methods},
        { 805,                        spawn_point_update, default_moby_methods},
        { 806,                      nanotech_ball_update, default_moby_methods},
        { 832,        update_target_outside_zone_timeout, default_moby_methods},
        { 849,                         suckcannon_update, default_moby_methods},
        {1000,                         update_help_hints, default_moby_methods},
        {1006,                    devestator_ammo_update, default_moby_methods},
        {1023,                  blarg_paratrooper_update, default_moby_methods},
        {1134,                          gold_bolt_update, default_moby_methods},
        {1138,                          quad_moby_update, default_moby_methods},
        {1143,                         vendor_orb_update, default_moby_methods},
        {1150,                           elevator_update, default_moby_methods},
        {1151,                           elevator_update, default_moby_methods},
        {1172,                  refueling_station_update, default_moby_methods},
        {1181,                 airship_chain_link_update, default_moby_methods},
        {1182,                       update_airship_part, default_moby_methods},
        {1183,                       update_airship_part, default_moby_methods},
        {1185,                       update_airship_part, default_moby_methods},
        {1186,                       update_airship_part, default_moby_methods},
        {1187,                       update_airship_part, default_moby_methods},
        {1188,                       update_airship_part, default_moby_methods},
        {1189,                       update_airship_part, default_moby_methods},
        {1192,                      update_glow_particle, default_moby_methods},
        {1201,                             turret_update, default_moby_methods},
        {1206,                      update_damage_causer, default_moby_methods},
        {1212,                  background_ship_1_update, default_moby_methods},
        {1213,                  background_ship_2_update, default_moby_methods},
        {1251,                     hydrodisplacer_update, default_moby_methods},
        {1258,                        update_turret_shot, default_moby_methods},
        {1285,                            barrier_update, default_moby_methods},
        {1286,                            barrier_update, default_moby_methods},
        {1287,                            barrier_update, default_moby_methods},
        {1288,                            barrier_update, default_moby_methods},
        {1289,                        noop_main_callback, default_moby_methods},
        {1290,                      pilots_helmet_update, default_moby_methods},
        {1292,                          update_fire_bolt, default_moby_methods},
        {1293,                update_floating_ship_piece, default_moby_methods},
        {1298,                            display_update, default_moby_methods},
        {1299,                            display_update, default_moby_methods},
        {1300,                        noop_main_callback, default_moby_methods},
        {1320,                update_floating_ship_piece, default_moby_methods},
        {1438,                    devestator_ammo_update, default_moby_methods},
        {1447,                    devestator_ammo_update, default_moby_methods},
        {1449,                    devestator_ammo_update, default_moby_methods},
        {1522,                      tumbling_part_update, default_moby_methods},
        {1525,                      tumbling_part_update, default_moby_methods},
        {1526,                      tumbling_part_update, default_moby_methods},
        {1528,                      tumbling_part_update, default_moby_methods},
        {1529,                      tumbling_part_update, default_moby_methods},
        {1692,                      tumbling_part_update, default_moby_methods},
        {1693,                      tumbling_part_update, default_moby_methods},
        {1694,                      tumbling_part_update, default_moby_methods},
        {1733,                      tumbling_part_update, default_moby_methods},
        {1734,                      tumbling_part_update, default_moby_methods},
        {1735,                      tumbling_part_update, default_moby_methods},
        {1766,                            shuttle_update, default_moby_methods},
        {1849,                               lamp_update, default_moby_methods},
        {1850,                        noop_main_callback, default_moby_methods},
        {1851,                      tumbling_part_update, default_moby_methods},
        {1885,                            spawner_update, default_moby_methods},
        {1886,                        spawner_egg_update, default_moby_methods},
        {1900,                              decoy_update, default_moby_methods},
        {1936,                      tumbling_part_update, default_moby_methods},
        {  -1,                                         0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L09_00209B80") __attribute__((section("lvl.camvtbl"))) = {
    { 0,noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                 },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                            },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                       },
    { 6,      noop_callback_l, initialize_orbit_camera_from_hero,     mine_blend_color,                          noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                    },
    {17,     FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,  volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                    0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[4] __asm__("D_L09_00209C80")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 1,                        update_box_emitter},
        { 3,                 update_reverb_box_emitter},
        {-1,                                         0},
};
