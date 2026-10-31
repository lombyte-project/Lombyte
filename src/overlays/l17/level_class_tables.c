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
void ammocrate_update() __asm__("FUN_L13_002e1cc8");
void billboard_projectile_update() __asm__("FUN_L01_002c7530");
void blarg_elite_commando_rocket_update() __asm__("FUN_L15_0029ac30");
void blarg_ship_rocket_update() __asm__("FUN_L10_002cc2e8");
void blarg_ship_shot_update() __asm__("FUN_L11_00309098");
void blarg_ship_update() __asm__("FUN_L17_002f40d8");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void breakaway_prop_update() __asm__("FUN_L17_002f5390");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_state_12() __asm__("FUN_L17_002f0040");
void codebot_update() __asm__("FUN_L15_002ebd78");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_camera_target_update() __asm__("FUN_L17_002f2e78");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void door_update() __asm__("FUN_L17_002e87d8");
void draw_only_moby_update() __asm__("FUN_L16_002e9df0");
void dreks_fleet_turret_update() __asm__("FUN_L17_002cb310");
void drone_update() __asm__("FUN_L00_002ce358");
void electric_beam_update() __asm__("FUN_L14_003015d0");
void electric_water_update() __asm__("FUN_L15_002d6810");
void enemy_stun_update() __asm__("FUN_L17_002efea8");
void energy_sphere_holder_update() __asm__("FUN_L06_002f7288");
void energy_sphere_update() __asm__("FUN_L06_002f7ab8");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void fighter_jet_missile_update() __asm__("FUN_L11_003089d0");
void fighter_jet_rocket_update() __asm__("FUN_L13_002e6c08");
void fighter_jet_update() __asm__("FUN_L17_002ed018");
void fleet_backdrop_animation_update() __asm__("FUN_L17_002f3848");
void fleet_beam_update() __asm__("FUN_L17_002a8da0");
void fleet_camera_box_watcher_update() __asm__("FUN_L17_002f26d0");
void fleet_rotating_radial_effect_update() __asm__("FUN_L17_002d77f0");
void fleet_turret_shot_update() __asm__("FUN_L17_002e8e08");
void floating_mine_update() __asm__("FUN_L17_002dc3f8");
void floor_button_update() __asm__("FUN_L05_0030c5b0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hero_seeking_mine_update() __asm__("FUN_L15_002a2488");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_camera_transform_and_paths() __asm__("FUN_L01_00315de0");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void linked_spark_emitter_update() __asm__("FUN_L15_002a2bf0");
void magnet_elevator_update() __asm__("FUN_L17_002ee2b0");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_rotation_group_update() __asm__("FUN_L06_002f4f00");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void moby_spatial_trigger_update() __asm__("FUN_L05_002f5200");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void ripple_strips_moby_update() __asm__("FUN_L15_002eb0a0");
void robot_dog_update() __asm__("FUN_L17_002eeb68");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void sentrybot_alarm_update() __asm__("FUN_L15_002cb4c8");
void sentrybot_projectile_update() __asm__("FUN_L15_002e4380");
void sentrybot_update() __asm__("FUN_L15_002979d8");
void shattering_prop_update() __asm__("FUN_L17_002f5260");
void ship_rocket_update() __asm__("FUN_L11_003094f8");
void shuttle_update() __asm__("FUN_L17_002f1940");
void smoke_trail_projectile_update() __asm__("FUN_L01_002eda00");
void spawn_point_update() __asm__("FUN_L01_00300220");
void spiderbot_update() __asm__("FUN_L15_002a6e68");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void underwater_door_update() __asm__("FUN_L15_002bddb0");
void underwater_ring_update() __asm__("FUN_L15_002e5958");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_listener_emitter() __asm__("FUN_L01_0031a078");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void updraft_vent_update() __asm__("FUN_L10_002e9028");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L17_001E7824")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L17_001E783C[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

void *D_L17_001E7854[6]

    __attribute__((section("lvl.vtbl"))) = {enemy_stun_update,
                                            enemy_stun_update,
                                            enemy_stun_update,
                                            cancel_suck_cannon_suction_and_set_state_12,
                                            get_moby_data_plus_0x60,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[163] __asm__("D_L17_001E7080")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,                camera_object_update, default_moby_methods},
        {  11,                       vendor_update, default_moby_methods},
        {  13,                         bolt_update, default_moby_methods},
        {  14,                         bolt_update, default_moby_methods},
        {  15,                         bolt_update, default_moby_methods},
        {  16,                         bolt_update, default_moby_methods},
        {  44,                    sentrybot_update, default_moby_methods},
        {  49,  blarg_elite_commando_rocket_update, default_moby_methods},
        {  71,                       wrench_update, default_moby_methods},
        {  74,                         mine_update, default_moby_methods},
        {  77,            hero_seeking_mine_update, default_moby_methods},
        {  78,         linked_spark_emitter_update, default_moby_methods},
        {  99,                   fleet_beam_update, default_moby_methods},
        { 112,                glow_particle_update, default_moby_methods},
        { 121,                         bomb_update, default_moby_methods},
        { 122,                       debris_update, default_moby_methods},
        { 123,                    spiderbot_update, default_moby_methods},
        { 149,      nanotech_crate_fragment_update, default_moby_methods},
        { 153,      gold_devastator_missile_update, default_moby_methods},
        { 156,         billboard_projectile_update, default_moby_methods},
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
        { 197,              underwater_door_update, default_moby_methods},
        { 203,                        decoy_update, default_moby_methods},
        { 204,              devestator_ammo_update, default_moby_methods},
        { 208,                    swingshot_update, default_moby_methods},
        { 209,               swingshot_hook_update, default_moby_methods},
        { 213,              devestator_ammo_update, default_moby_methods},
        { 214,              devestator_ammo_update, default_moby_methods},
        { 222,              devestator_ammo_update, default_moby_methods},
        { 223,              devestator_ammo_update, default_moby_methods},
        { 224,                    ammocrate_update, default_moby_methods},
        { 225,              devestator_ammo_update, default_moby_methods},
        { 226,              devestator_ammo_update, default_moby_methods},
        { 228,                    ammocrate_update, default_moby_methods},
        { 229,                glove_of_doom_update, default_moby_methods},
        { 230,             doom_bot_spawner_update, default_moby_methods},
        { 258,         moby_spatial_trigger_update, default_moby_methods},
        { 270,            morphoray_chicken_update,       D_L17_001E783C},
        { 295,           fighter_jet_rocket_update, default_moby_methods},
        { 305,                  homing_shot_update, default_moby_methods},
        { 347,           dreks_fleet_turret_update, default_moby_methods},
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
        { 408,              sentrybot_alarm_update, default_moby_methods},
        { 428,          damped_falling_moby_update, default_moby_methods},
        { 433,                  noop_main_callback, default_moby_methods},
        { 454,                         ryno_update, default_moby_methods},
        { 457,                  ryno_rocket_update, default_moby_methods},
        { 479,                        drone_update, default_moby_methods},
        { 483,                                   0, default_moby_methods},
        { 500,                        crate_update, default_moby_methods},
        { 501,                        crate_update, default_moby_methods},
        { 505,                        crate_update, default_moby_methods},
        { 511,                        crate_update, default_moby_methods},
        { 562,                  decoy_glove_update, default_moby_methods},
        { 567,       smoke_trail_projectile_update, default_moby_methods},
        { 585,               metal_detector_update, default_moby_methods},
        { 605,                 buried_bolts_update, default_moby_methods},
        { 607,                     helipack_update, default_moby_methods},
        { 618,                  noop_main_callback, default_moby_methods},
        { 634,                                   0, default_moby_methods},
        { 639,              explosion_light_update, default_moby_methods},
        { 655,               electric_water_update, default_moby_methods},
        { 669, fleet_rotating_radial_effect_update, default_moby_methods},
        { 750,                      infobot_update, default_moby_methods},
        { 758,        swingshot_versatarget_update, default_moby_methods},
        { 775,            grow_and_fade_out_update, default_moby_methods},
        { 804,      morphoray_target_effect_update, default_moby_methods},
        { 805,                  spawn_point_update, default_moby_methods},
        { 806,                nanotech_ball_update, default_moby_methods},
        { 819,            blarg_ship_rocket_update, default_moby_methods},
        { 830,                 floor_button_update, default_moby_methods},
        { 835,                floating_mine_update, default_moby_methods},
        { 849,                   suckcannon_update, default_moby_methods},
        { 934,         sentrybot_projectile_update, default_moby_methods},
        {1006,              devestator_ammo_update, default_moby_methods},
        {1009,          fighter_jet_missile_update, default_moby_methods},
        {1017,              blarg_ship_shot_update, default_moby_methods},
        {1021,          moby_rotation_group_update, default_moby_methods},
        {1034,                  ship_rocket_update, default_moby_methods},
        {1038,         energy_sphere_holder_update, default_moby_methods},
        {1040,                energy_sphere_update, default_moby_methods},
        {1134,                    gold_bolt_update, default_moby_methods},
        {1135,                   teleporter_update, default_moby_methods},
        {1138,                    quad_moby_update, default_moby_methods},
        {1143,                   vendor_orb_update, default_moby_methods},
        {1192,                glow_particle_update, default_moby_methods},
        {1209,              underwater_ring_update, default_moby_methods},
        {1224,                electric_beam_update, default_moby_methods},
        {1251,               hydrodisplacer_update, default_moby_methods},
        {1289,                  noop_main_callback, default_moby_methods},
        {1290,                pilots_helmet_update, default_moby_methods},
        {1359,                         door_update, default_moby_methods},
        {1360,                         door_update, default_moby_methods},
        {1361,                         door_update, default_moby_methods},
        {1362,                         door_update, default_moby_methods},
        {1363,                         door_update, default_moby_methods},
        {1364,                         door_update, default_moby_methods},
        {1367,                         door_update, default_moby_methods},
        {1368,            fleet_turret_shot_update, default_moby_methods},
        {1369,                         door_update, default_moby_methods},
        {1372,                         door_update, default_moby_methods},
        {1373,                         door_update, default_moby_methods},
        {1378,                 updraft_vent_update, default_moby_methods},
        {1379,                  fighter_jet_update, default_moby_methods},
        {1380,              magnet_elevator_update, default_moby_methods},
        {1382,                    robot_dog_update,       D_L17_001E7854},
        {1405,           ripple_strips_moby_update, default_moby_methods},
        {1406,           ripple_strips_moby_update, default_moby_methods},
        {1407,           ripple_strips_moby_update, default_moby_methods},
        {1426,           ripple_strips_moby_update, default_moby_methods},
        {1428,                      codebot_update, default_moby_methods},
        {1438,              devestator_ammo_update, default_moby_methods},
        {1447,              devestator_ammo_update, default_moby_methods},
        {1448,                      shuttle_update, default_moby_methods},
        {1449,              devestator_ammo_update, default_moby_methods},
        {1470,     fleet_camera_box_watcher_update, default_moby_methods},
        {1562,       cutscene_camera_target_update, default_moby_methods},
        {1594,                tumbling_part_update, default_moby_methods},
        {1595,                tumbling_part_update, default_moby_methods},
        {1742,                tumbling_part_update, default_moby_methods},
        {1743,                tumbling_part_update, default_moby_methods},
        {1772,     fleet_backdrop_animation_update, default_moby_methods},
        {1843,                   blarg_ship_update, default_moby_methods},
        {1873,              shattering_prop_update, default_moby_methods},
        {1874,                  noop_main_callback, default_moby_methods},
        {1875,                tumbling_part_update, default_moby_methods},
        {1876,               breakaway_prop_update, default_moby_methods},
        {1877,                  noop_main_callback, default_moby_methods},
        {1878,                tumbling_part_update, default_moby_methods},
        {1900,                        decoy_update, default_moby_methods},
        {1907,               draw_only_moby_update, default_moby_methods},
        {1908,               draw_only_moby_update, default_moby_methods},
        {1910,               draw_only_moby_update, default_moby_methods},
        {1911,               draw_only_moby_update, default_moby_methods},
        {1913,               draw_only_moby_update, default_moby_methods},
        {1917,                tumbling_part_update, default_moby_methods},
        {1918,               draw_only_moby_update, default_moby_methods},
        {1958,              underwater_door_update, default_moby_methods},
        {  -1,                                   0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L17_001E7880") __attribute__((section("lvl.camvtbl"))) = {
    { 0,noop_callback_l,                   follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                     },
    { 3,
     noop_callback_l, initialize_camera_transform_and_paths,
     update_camera_position_and_orientation_from_paths,              update_camera_mode_from_preset                                    },
    { 4,
     camera_transition_update,                      FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                },
    { 5,
     noop_callback_l,        initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                           },
    { 6,      noop_callback_l,     initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l,     initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                        },
    {17,     FUN_L00_002ed230,                    noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                    0,                                     0,                    0,                                           0},
};

struct SoundHandler sound_handlers[2] __asm__("D_L17_001E7980")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 5, update_listener_emitter},
        {-1,                       0},
};
