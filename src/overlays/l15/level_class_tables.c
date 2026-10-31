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
void beam_slots_update() __asm__("FUN_L14_003039e0");
void billboard_projectile_update() __asm__("FUN_L01_002c7530");
void blarg_commander_update() __asm__("FUN_L12_00308918");
void blarg_elite_commando_rocket_update() __asm__("FUN_L15_0029ac30");
void blarg_ship_rocket_update() __asm__("FUN_L10_002cc2e8");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_grabber_update() __asm__("FUN_L15_002ea748");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void codebot_slot_update() __asm__("FUN_L15_002ec030");
void codebot_update() __asm__("FUN_L15_002ebd78");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void construction_arm_update() __asm__("FUN_L15_002c2938");
void conveyor_belt_update() __asm__("FUN_L15_002e73c0");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_camera_target_update() __asm__("FUN_L15_002edb20");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void dispatch_moby_follow_camera_update() __asm__("FUN_L15_002f8ba8");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drone_update() __asm__("FUN_L00_002ce358");
void electric_beam_update() __asm__("FUN_L14_003015d0");
void electric_water_update() __asm__("FUN_L15_002d6810");
void evaluate_descriptor_eligibility() __asm__("FUN_L10_002f65f8");
void exploding_prop_update() __asm__("FUN_L15_002aa280");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void falling_moby_update() __asm__("FUN_L03_002de370");
void floor_button_update() __asm__("FUN_L05_0030c5b0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void giant_clank_charged_blast_update() __asm__("FUN_L15_0029edb0");
void giant_clank_hit_sphere_update() __asm__("FUN_L15_0029ead0");
void giant_clank_missile_update() __asm__("FUN_L15_0029d6c0");
void giant_clank_pad_update() __asm__("FUN_L15_002ed068");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void ground_melee_enemy_update() __asm__("FUN_L09_002e27d8");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helicopter_spark_update() __asm__("FUN_L03_002d47c0");
void helicopter_update() __asm__("FUN_L15_002c5630");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L15_002ed398");
void hero_seeking_mine_update() __asm__("FUN_L15_002a2488");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_path_camera() __asm__("FUN_L10_002f5b58");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void linked_spark_emitter_update() __asm__("FUN_L15_002a2bf0");
void linked_turning_moby_update() __asm__("FUN_L15_002e46b0");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_hold_state_if_grabbed() __asm__("FUN_L01_002f1c78");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void moby_spatial_trigger_update() __asm__("FUN_L05_002f5200");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void path_or_ballistic_debris_update() __asm__("FUN_L14_002dfd10");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void plasmabot_death_debris_update() __asm__("FUN_L14_002de670");
void plasmabot_hatch_update() __asm__("FUN_L14_002d96e0");
void plasmabot_update() __asm__("FUN_L14_002b17d8");
void poison_vessel_spawner_update() __asm__("FUN_L15_002eabd8");
void poison_vessel_update() __asm__("FUN_L15_002e7f68");
void puff_emitter_update() __asm__("FUN_L15_002eb928");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void quartu_cutscene_trigger_update() __asm__("FUN_L15_002eb4c0");
void radial_exploding_prop_update() __asm__("FUN_L15_002a7880");
void rail_hero_motion_update() __asm__("FUN_L01_002f6328");
void ripple_strips_moby_update() __asm__("FUN_L15_002eb0a0");
void rock_update() __asm__("FUN_L01_002f8718");
void run_moby_helper_and_set_state_7() __asm__("FUN_L01_002f1dc8");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void sentrybot_alarm_update() __asm__("FUN_L15_002cb4c8");
void sentrybot_projectile_update() __asm__("FUN_L15_002e4380");
void sentrybot_update() __asm__("FUN_L15_002979d8");
void sliding_gate_update() __asm__("FUN_L15_0029aff0");
void smoke_trail_projectile_update() __asm__("FUN_L01_002eda00");
void spawn_point_update() __asm__("FUN_L01_00300220");
void spiderbot_update() __asm__("FUN_L15_002a6e68");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swinging_door_update() __asm__("FUN_L15_002a3ba8");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void thruster_pack_lock_update() __asm__("FUN_L11_0030ee00");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void ultramech_scientist_update() __asm__("FUN_L15_002ec760");
void ultramech_unlimited_update() __asm__("FUN_L15_002cf3a8");
void underwater_door_update() __asm__("FUN_L15_002bddb0");
void underwater_ring_update() __asm__("FUN_L15_002e5958");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_listener_emitter() __asm__("FUN_L01_0031a078");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void vertical_gate_update() __asm__("FUN_L15_002a36d0");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void visibomb_zone_timer_update() __asm__("FUN_L01_00302648");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L15_001E3E84")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L15_001E3E9C[6] __attribute__((section("lvl.vtbl"))) = {moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                moby_hold_state_if_grabbed,
                                                                run_moby_helper_and_set_state_7,
                                                                get_moby_data_plus_0x60,
                                                                hud_counter_update};

void *D_L15_001E3EB4[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[171] __asm__("D_L15_001E3680")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,               camera_object_update, default_moby_methods},
        {  11,                      vendor_update, default_moby_methods},
        {  13,                        bolt_update, default_moby_methods},
        {  14,                        bolt_update, default_moby_methods},
        {  15,                        bolt_update, default_moby_methods},
        {  16,                        bolt_update, default_moby_methods},
        {  28,                   plasmabot_update, default_moby_methods},
        {  44,                   sentrybot_update, default_moby_methods},
        {  49, blarg_elite_commando_rocket_update, default_moby_methods},
        {  67,                sliding_gate_update, default_moby_methods},
        {  71,                      wrench_update, default_moby_methods},
        {  74,                        mine_update, default_moby_methods},
        {  77,           hero_seeking_mine_update, default_moby_methods},
        {  78,        linked_spark_emitter_update, default_moby_methods},
        {  92,               vertical_gate_update, default_moby_methods},
        {  93,               swinging_door_update, default_moby_methods},
        { 112,               glow_particle_update, default_moby_methods},
        { 121,                        bomb_update, default_moby_methods},
        { 122,                      debris_update, default_moby_methods},
        { 123,                   spiderbot_update, default_moby_methods},
        { 148,       radial_exploding_prop_update, default_moby_methods},
        { 149,     nanotech_crate_fragment_update, default_moby_methods},
        { 153,     gold_devastator_missile_update, default_moby_methods},
        { 154,              exploding_prop_update, default_moby_methods},
        { 156,        billboard_projectile_update, default_moby_methods},
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
        { 193,          ground_melee_enemy_update,       D_L15_001E3E9C},
        { 196,             underwater_door_update, default_moby_methods},
        { 197,             underwater_door_update, default_moby_methods},
        { 200,                        rock_update, default_moby_methods},
        { 201,                        rock_update, default_moby_methods},
        { 203,                       decoy_update, default_moby_methods},
        { 204,             devestator_ammo_update, default_moby_methods},
        { 205,                        rock_update, default_moby_methods},
        { 206,                        rock_update, default_moby_methods},
        { 207,                        rock_update, default_moby_methods},
        { 208,                   swingshot_update, default_moby_methods},
        { 209,              swingshot_hook_update, default_moby_methods},
        { 210,                        rock_update, default_moby_methods},
        { 213,             devestator_ammo_update, default_moby_methods},
        { 214,             devestator_ammo_update, default_moby_methods},
        { 221,            construction_arm_update, default_moby_methods},
        { 222,             devestator_ammo_update, default_moby_methods},
        { 223,             devestator_ammo_update, default_moby_methods},
        { 225,             devestator_ammo_update, default_moby_methods},
        { 226,             devestator_ammo_update, default_moby_methods},
        { 229,               glove_of_doom_update, default_moby_methods},
        { 230,            doom_bot_spawner_update, default_moby_methods},
        { 233,                  helicopter_update, default_moby_methods},
        { 250,             plasmabot_hatch_update, default_moby_methods},
        { 255,       radial_exploding_prop_update, default_moby_methods},
        { 256,         giant_clank_missile_update, default_moby_methods},
        { 258,        moby_spatial_trigger_update, default_moby_methods},
        { 270,           morphoray_chicken_update,       D_L15_001E3EB4},
        { 283,                 noop_main_callback, default_moby_methods},
        { 305,                 homing_shot_update, default_moby_methods},
        { 325,      plasmabot_death_debris_update, default_moby_methods},
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
        { 403,    path_or_ballistic_debris_update, default_moby_methods},
        { 408,             sentrybot_alarm_update, default_moby_methods},
        { 419,                 noop_main_callback, default_moby_methods},
        { 428,         damped_falling_moby_update, default_moby_methods},
        { 433,                 noop_main_callback, default_moby_methods},
        { 454,                        ryno_update, default_moby_methods},
        { 457,                 ryno_rocket_update, default_moby_methods},
        { 479,                       drone_update, default_moby_methods},
        { 483,                                  0, default_moby_methods},
        { 491,         ultramech_unlimited_update, default_moby_methods},
        { 500,                       crate_update, default_moby_methods},
        { 501,                       crate_update, default_moby_methods},
        { 502,                       crate_update, default_moby_methods},
        { 505,                       crate_update, default_moby_methods},
        { 511,                       crate_update, default_moby_methods},
        { 531,                 noop_main_callback, default_moby_methods},
        { 562,                 decoy_glove_update, default_moby_methods},
        { 567,      smoke_trail_projectile_update, default_moby_methods},
        { 585,              metal_detector_update, default_moby_methods},
        { 605,                buried_bolts_update, default_moby_methods},
        { 607,                    helipack_update, default_moby_methods},
        { 618,                 noop_main_callback, default_moby_methods},
        { 634,                                  0, default_moby_methods},
        { 639,             explosion_light_update, default_moby_methods},
        { 655,              electric_water_update, default_moby_methods},
        { 679,            rail_hero_motion_update, default_moby_methods},
        { 750,                     infobot_update, default_moby_methods},
        { 758,       swingshot_versatarget_update, default_moby_methods},
        { 775,           grow_and_fade_out_update, default_moby_methods},
        { 803,       swingshot_versatarget_update, default_moby_methods},
        { 804,     morphoray_target_effect_update, default_moby_methods},
        { 805,                 spawn_point_update, default_moby_methods},
        { 806,               nanotech_ball_update, default_moby_methods},
        { 819,           blarg_ship_rocket_update, default_moby_methods},
        { 830,                floor_button_update, default_moby_methods},
        { 832,         visibomb_zone_timer_update, default_moby_methods},
        { 848,            helicopter_spark_update, default_moby_methods},
        { 849,                  suckcannon_update, default_moby_methods},
        { 934,        sentrybot_projectile_update, default_moby_methods},
        { 937,         linked_turning_moby_update, default_moby_methods},
        {1006,             devestator_ammo_update, default_moby_methods},
        {1134,                   gold_bolt_update, default_moby_methods},
        {1138,                   quad_moby_update, default_moby_methods},
        {1143,                  vendor_orb_update, default_moby_methods},
        {1179,          thruster_pack_lock_update, default_moby_methods},
        {1192,               glow_particle_update, default_moby_methods},
        {1209,             underwater_ring_update, default_moby_methods},
        {1221,                falling_moby_update, default_moby_methods},
        {1222,                falling_moby_update, default_moby_methods},
        {1224,               electric_beam_update, default_moby_methods},
        {1250,               conveyor_belt_update, default_moby_methods},
        {1251,              hydrodisplacer_update, default_moby_methods},
        {1257,               poison_vessel_update, default_moby_methods},
        {1289,                 noop_main_callback, default_moby_methods},
        {1290,               pilots_helmet_update, default_moby_methods},
        {1331,                  beam_slots_update, default_moby_methods},
        {1371,             blarg_commander_update, default_moby_methods},
        {1388,                bolt_grabber_update, default_moby_methods},
        {1394,       poison_vessel_spawner_update, default_moby_methods},
        {1408,          ripple_strips_moby_update, default_moby_methods},
        {1409,          ripple_strips_moby_update, default_moby_methods},
        {1419,     quartu_cutscene_trigger_update, default_moby_methods},
        {1425,                puff_emitter_update, default_moby_methods},
        {1427,      giant_clank_hit_sphere_update, default_moby_methods},
        {1428,                     codebot_update, default_moby_methods},
        {1430,                codebot_slot_update, default_moby_methods},
        {1438,             devestator_ammo_update, default_moby_methods},
        {1446,         ultramech_scientist_update, default_moby_methods},
        {1447,             devestator_ammo_update, default_moby_methods},
        {1449,             devestator_ammo_update, default_moby_methods},
        {1451,             giant_clank_pad_update, default_moby_methods},
        {1469,                  help_hints_update, default_moby_methods},
        {1523,   giant_clank_charged_blast_update, default_moby_methods},
        {1560,      cutscene_camera_target_update, default_moby_methods},
        {1565,          ripple_strips_moby_update, default_moby_methods},
        {1567,          ripple_strips_moby_update, default_moby_methods},
        {1591,               tumbling_part_update, default_moby_methods},
        {1592,               tumbling_part_update, default_moby_methods},
        {1593,               tumbling_part_update, default_moby_methods},
        {1610,               tumbling_part_update, default_moby_methods},
        {1611,               tumbling_part_update, default_moby_methods},
        {1612,               tumbling_part_update, default_moby_methods},
        {1867,               tumbling_part_update, default_moby_methods},
        {1868,               tumbling_part_update, default_moby_methods},
        {1869,               tumbling_part_update, default_moby_methods},
        {1900,                       decoy_update, default_moby_methods},
        {1961,               tumbling_part_update, default_moby_methods},
        {1962,               tumbling_part_update, default_moby_methods},
        {  -1,                                  0, default_moby_methods},
};

struct CameraType level_camera_types[10] __asm__("D_L15_001E3F00") __attribute__((section("lvl.camvtbl"))) = {
    { 0,        noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                           },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                      },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                                 },
    { 6,                noop_callback_l, initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                              },
    {17,               FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {19,
     evaluate_descriptor_eligibility,            initialize_path_camera,
     FUN_L10_002f5f18,                          noop_main_callback                                                                           },
    {22,
     dispatch_moby_follow_camera_update,                noop_main_callback,
     noop_main_callback,                          noop_main_callback                                                                         },
    {23,            volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                              0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[3] __asm__("D_L15_001E4000")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 3, update_reverb_box_emitter},
        { 5,   update_listener_emitter},
        {-1,                         0},
};
