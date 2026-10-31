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
void background_ship_2_update() __asm__("FUN_L02_002ece18");
void blarg_ship_rocket_update() __asm__("FUN_L10_002cc2e8");
void blarg_ship_update() __asm__("FUN_L10_002e4a88");
void blarg_space_commando_update() __asm__("FUN_L06_002fdbd0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void bombthrower_bomb_update() __asm__("FUN_L10_002d85c8");
void bombthrower_grass_update() __asm__("FUN_L10_002d8c00");
void bot_door_lock_update() __asm__("FUN_L06_00307d30");
void breakable_prop_update() __asm__("FUN_L10_002eb7e0");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void button_update() __asm__("FUN_L10_002d92b8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_moby_suction_and_save_state() __asm__("FUN_L06_002ea7c8");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void configurable_particle_emitter_update() __asm__("FUN_L10_002ea1f0");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_camera_target_update() __asm__("FUN_L10_002eacd8");
void cutscene_trigger_update() __asm__("FUN_L01_002fb5b0");
void cycling_platform_update() __asm__("FUN_L10_00295a38");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void darter_update() __asm__("FUN_L10_002e01a8");
void data_processor_linked_effect_update() __asm__("FUN_L10_002da2c8");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void destructible_object_update() __asm__("FUN_L10_002d9eb8");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void directional_particle_emitter_update() __asm__("FUN_L10_002e5be0");
void directional_spark_emitter_update() __asm__("FUN_L08_002f5830");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drone_update() __asm__("FUN_L00_002ce358");
void energy_sphere_holder_update() __asm__("FUN_L06_002f7288");
void energy_sphere_update() __asm__("FUN_L06_002f7ab8");
void evaluate_descriptor_eligibility() __asm__("FUN_L10_002f65f8");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void falling_spinning_moby_update() __asm__("FUN_L06_00300b90");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void fred_1_update() __asm__("FUN_L03_002dc310");
void frog_update() __asm__("FUN_L06_002e8678");
void gadgebot_spawn_container_update() __asm__("FUN_L06_002d85a0");
void gadgebot_spawn_pad_update() __asm__("FUN_L06_002d8c20");
void gadgebot_telepot_pad_update() __asm__("FUN_L10_002be858");
void gadgebot_update() __asm__("FUN_L06_002f0040");
void gate_1_update() __asm__("FUN_L10_002e9648");
void gate_2_update() __asm__("FUN_L10_002e9990");
void gate_update() __asm__("FUN_L10_002befe8");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void gun_update() __asm__("FUN_L10_002e8ab0");
void height_adjusted_projectile_update() __asm__("FUN_L10_002d7b80");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L10_002e85b8");
void hero_relocation_trigger_update() __asm__("FUN_L10_00298b68");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_path_camera() __asm__("FUN_L10_002f5b58");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void jitter_strand_grid_update() __asm__("FUN_L10_002dcc58");
void lab_door_update() __asm__("FUN_L10_002d90a8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void linked_moby_hitbox_update() __asm__("FUN_L10_002c9c70");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void magnetboots_update() __asm__("FUN_L10_00298668");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void moby_spatial_trigger_update() __asm__("FUN_L05_002f5200");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void nanotech_upgrade_vendor_update() __asm__("FUN_L10_002e8358");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void novalis_elevator_update() __asm__("FUN_L01_002b9eb0");
void path_spark_effect_update() __asm__("FUN_L10_002dd3d8");
void path_spark_emitter_update() __asm__("FUN_L10_002d8d30");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pterodactyl_update() __asm__("FUN_L00_002e3a88");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void rock_update() __asm__("FUN_L01_002f8718");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void screamer_update() __asm__("FUN_L10_002df270");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void shattering_prop_update() __asm__("FUN_L10_002eb6f0");
void sinking_platform_update() __asm__("FUN_L10_00295c20");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void timed_effect_emitter_update() __asm__("FUN_L10_002c7a20");
void toxic_crab_update() __asm__("FUN_L10_002e1d38");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void tunnel_barrier_update() __asm__("FUN_L10_002dd650");
void turret_shot_update() __asm__("FUN_L09_00307d68");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_moby_grab_state() __asm__("FUN_L06_002ea5e0");
void update_moby_suck_cannon_storage_state() __asm__("FUN_L06_002ea6a8");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void updraft_vent_update() __asm__("FUN_L10_002e9028");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void visibomb_zone_timer_update() __asm__("FUN_L01_00302648");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L10_001DDE28")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L10_001DDE40[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

void *D_L10_001DDE58[6]

    __attribute__((section("lvl.vtbl"))) = {update_moby_grab_state,
                                            update_moby_suck_cannon_storage_state,
                                            update_moby_suck_cannon_storage_state,
                                            cancel_moby_suction_and_save_state,
                                            get_moby_data_plus_0x60,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[174] __asm__("D_L10_001DD600")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,                 camera_object_update, default_moby_methods},
        {1033,              cycling_platform_update, default_moby_methods},
        {1117,              sinking_platform_update, default_moby_methods},
        {  11,                        vendor_update, default_moby_methods},
        {  13,                          bolt_update, default_moby_methods},
        {  14,                          bolt_update, default_moby_methods},
        {  15,                          bolt_update, default_moby_methods},
        {  16,                          bolt_update, default_moby_methods},
        {  18,                   magnetboots_update, default_moby_methods},
        {  22,       hero_relocation_trigger_update, default_moby_methods},
        {  71,                        wrench_update, default_moby_methods},
        {  74,                          mine_update, default_moby_methods},
        {  87,                   noop_main_callback, default_moby_methods},
        { 112,                 glow_particle_update, default_moby_methods},
        { 121,                          bomb_update, default_moby_methods},
        { 122,                        debris_update, default_moby_methods},
        { 149,       nanotech_crate_fragment_update, default_moby_methods},
        { 153,       gold_devastator_missile_update, default_moby_methods},
        { 157,                    devastator_update, default_moby_methods},
        { 163,                      visibomb_update, default_moby_methods},
        { 166,            linked_moby_states_update, default_moby_methods},
        { 167,           thruster_pack_flame_update, default_moby_methods},
        { 168,                       blaster_update, default_moby_methods},
        { 172,              visibomb_missile_update, default_moby_methods},
        { 175,                       taunter_update, default_moby_methods},
        { 176,                     pyrocitor_update, default_moby_methods},
        { 177,                    tesla_claw_update, default_moby_methods},
        { 179,                trailing_flame_update, default_moby_methods},
        { 180,                      walloper_update, default_moby_methods},
        { 185,                     morphoray_update, default_moby_methods},
        { 186,                      doom_bot_update, default_moby_methods},
        { 188,                    trespasser_update, default_moby_methods},
        { 190,                    mine_glove_update, default_moby_methods},
        { 192,                    bomb_glove_update, default_moby_methods},
        { 203,                         decoy_update, default_moby_methods},
        { 204,               devestator_ammo_update, default_moby_methods},
        { 208,                     swingshot_update, default_moby_methods},
        { 209,                swingshot_hook_update, default_moby_methods},
        { 213,               devestator_ammo_update, default_moby_methods},
        { 214,               devestator_ammo_update, default_moby_methods},
        { 222,               devestator_ammo_update, default_moby_methods},
        { 223,               devestator_ammo_update, default_moby_methods},
        { 225,               devestator_ammo_update, default_moby_methods},
        { 226,               devestator_ammo_update, default_moby_methods},
        { 229,                 glove_of_doom_update, default_moby_methods},
        { 230,              doom_bot_spawner_update, default_moby_methods},
        { 258,          moby_spatial_trigger_update, default_moby_methods},
        { 270,             morphoray_chicken_update,       D_L10_001DDE40},
        { 302,      gadgebot_spawn_container_update, default_moby_methods},
        { 303,            gadgebot_spawn_pad_update, default_moby_methods},
        { 305,                   homing_shot_update, default_moby_methods},
        { 348,       nanotech_crate_fragment_update, default_moby_methods},
        { 349,       nanotech_crate_fragment_update, default_moby_methods},
        { 351,          gadgebot_telepot_pad_update, default_moby_methods},
        { 353,                          gate_update, default_moby_methods},
        { 354,       nanotech_crate_fragment_update, default_moby_methods},
        { 355,       nanotech_crate_fragment_update, default_moby_methods},
        { 356,       nanotech_crate_fragment_update, default_moby_methods},
        { 357,       nanotech_crate_fragment_update, default_moby_methods},
        { 358,       nanotech_crate_fragment_update, default_moby_methods},
        { 359,       nanotech_crate_fragment_update, default_moby_methods},
        { 363,       nanotech_crate_fragment_update, default_moby_methods},
        { 364,       nanotech_crate_fragment_update, default_moby_methods},
        { 428,           damped_falling_moby_update, default_moby_methods},
        { 433,                   noop_main_callback, default_moby_methods},
        { 454,                          ryno_update, default_moby_methods},
        { 457,                   ryno_rocket_update, default_moby_methods},
        { 479,                         drone_update, default_moby_methods},
        { 483,                                    0, default_moby_methods},
        { 500,                         crate_update, default_moby_methods},
        { 501,                         crate_update, default_moby_methods},
        { 502,                         crate_update, default_moby_methods},
        { 505,                         crate_update, default_moby_methods},
        { 511,                         crate_update, default_moby_methods},
        { 532,                   noop_main_callback, default_moby_methods},
        { 562,                   decoy_glove_update, default_moby_methods},
        { 585,                metal_detector_update, default_moby_methods},
        { 605,                  buried_bolts_update, default_moby_methods},
        { 607,                      helipack_update, default_moby_methods},
        { 618,                   noop_main_callback, default_moby_methods},
        { 634,                                    0, default_moby_methods},
        { 639,               explosion_light_update, default_moby_methods},
        { 648,     directional_spark_emitter_update, default_moby_methods},
        { 702,          timed_effect_emitter_update, default_moby_methods},
        { 737,              cutscene_trigger_update, default_moby_methods},
        { 750,                       infobot_update, default_moby_methods},
        { 758,         swingshot_versatarget_update, default_moby_methods},
        { 775,             grow_and_fade_out_update, default_moby_methods},
        { 794,            linked_moby_hitbox_update, default_moby_methods},
        { 803,         swingshot_versatarget_update, default_moby_methods},
        { 804,       morphoray_target_effect_update, default_moby_methods},
        { 805,                   spawn_point_update, default_moby_methods},
        { 806,                 nanotech_ball_update, default_moby_methods},
        { 819,             blarg_ship_rocket_update, default_moby_methods},
        { 827,                          frog_update,       D_L10_001DDE58},
        { 832,           visibomb_zone_timer_update, default_moby_methods},
        { 849,                    suckcannon_update, default_moby_methods},
        { 857,                      gadgebot_update, default_moby_methods},
        { 859,    height_adjusted_projectile_update, default_moby_methods},
        { 915,                        fred_1_update, default_moby_methods},
        { 938,              bombthrower_bomb_update, default_moby_methods},
        { 939,             bombthrower_grass_update, default_moby_methods},
        { 947,            path_spark_emitter_update, default_moby_methods},
        {1006,               devestator_ammo_update, default_moby_methods},
        {1015,                      lab_door_update, default_moby_methods},
        {1031,                        button_update, default_moby_methods},
        {1038,          energy_sphere_holder_update, default_moby_methods},
        {1040,                 energy_sphere_update, default_moby_methods},
        {1047,           destructible_object_update, default_moby_methods},
        {1067,  data_processor_linked_effect_update, default_moby_methods},
        {1068,          blarg_space_commando_update, default_moby_methods},
        {1073,            jitter_strand_grid_update, default_moby_methods},
        {1085,         falling_spinning_moby_update, default_moby_methods},
        {1086,         falling_spinning_moby_update, default_moby_methods},
        {1087,         falling_spinning_moby_update, default_moby_methods},
        {1088,         falling_spinning_moby_update, default_moby_methods},
        {1089,         falling_spinning_moby_update, default_moby_methods},
        {1090,             path_spark_effect_update, default_moby_methods},
        {1100,                tunnel_barrier_update, default_moby_methods},
        {1122,                   noop_main_callback, default_moby_methods},
        {1134,                     gold_bolt_update, default_moby_methods},
        {1138,                     quad_moby_update, default_moby_methods},
        {1141,              novalis_elevator_update, default_moby_methods},
        {1143,                    vendor_orb_update, default_moby_methods},
        {1192,                 glow_particle_update, default_moby_methods},
        {1196,                      screamer_update, default_moby_methods},
        {1199,                        darter_update, default_moby_methods},
        {1202,                    toxic_crab_update, default_moby_methods},
        {1229,                    blarg_ship_update, default_moby_methods},
        {1240,  directional_particle_emitter_update, default_moby_methods},
        {1251,                hydrodisplacer_update, default_moby_methods},
        {1258,                   turret_shot_update, default_moby_methods},
        {1282,                      lab_door_update, default_moby_methods},
        {1289,                   noop_main_callback, default_moby_methods},
        {1290,                 pilots_helmet_update, default_moby_methods},
        {1301,          gadgebot_telepot_pad_update, default_moby_methods},
        {1302,                 bot_door_lock_update, default_moby_methods},
        {1326,       nanotech_upgrade_vendor_update, default_moby_methods},
        {1344,                    help_hints_update, default_moby_methods},
        {1346,                           gun_update, default_moby_methods},
        {1378,                  updraft_vent_update, default_moby_methods},
        {1421,                        gate_1_update, default_moby_methods},
        {1424,                        gate_2_update, default_moby_methods},
        {1438,               devestator_ammo_update, default_moby_methods},
        {1447,               devestator_ammo_update, default_moby_methods},
        {1449,               devestator_ammo_update, default_moby_methods},
        {1526,                 tumbling_part_update, default_moby_methods},
        {1528,                 tumbling_part_update, default_moby_methods},
        {1529,                 tumbling_part_update, default_moby_methods},
        {1533,                 tumbling_part_update, default_moby_methods},
        {1534,                 tumbling_part_update, default_moby_methods},
        {1537,                 tumbling_part_update, default_moby_methods},
        {1544, configurable_particle_emitter_update, default_moby_methods},
        {1555,        cutscene_camera_target_update, default_moby_methods},
        {1564,                   pterodactyl_update, default_moby_methods},
        {1571,                 tumbling_part_update, default_moby_methods},
        {1572,                 tumbling_part_update, default_moby_methods},
        {1702,                 tumbling_part_update, default_moby_methods},
        {1703,                 tumbling_part_update, default_moby_methods},
        {1704,                 tumbling_part_update, default_moby_methods},
        {1806,                          rock_update, default_moby_methods},
        {1807,                          rock_update, default_moby_methods},
        {1855,               shattering_prop_update, default_moby_methods},
        {1856,                breakable_prop_update, default_moby_methods},
        {1857,                   noop_main_callback, default_moby_methods},
        {1858,                 tumbling_part_update, default_moby_methods},
        {1900,                         decoy_update, default_moby_methods},
        {1909,                 tumbling_part_update, default_moby_methods},
        {1914,                 tumbling_part_update, default_moby_methods},
        {1915,                 tumbling_part_update, default_moby_methods},
        {1916,                 tumbling_part_update, default_moby_methods},
        {1921,                   noop_main_callback, default_moby_methods},
        {1973,             background_ship_2_update, default_moby_methods},
        {  -1,                                    0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L10_001DDE80") __attribute__((section("lvl.camvtbl"))) = {
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
    {23,         volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                           0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[3] __asm__("D_L10_001DDF80")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 3,                 update_reverb_box_emitter},
        {-1,                                         0},
};
