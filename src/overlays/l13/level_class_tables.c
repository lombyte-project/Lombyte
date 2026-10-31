/* The level's moby classes, camera types and sound emitter types: the tables
   the executable's code looks ids up in (sections lvl.vtbl, lvl.camvtbl and
   lvl.sndvtbl). */
#include "types.h"
#include "rnc/audio/sound_handlers.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby_class_tables.h"

void FUN_L00_002caf40();
void FUN_L00_002ea7a0();
void FUN_L10_002f5f18();
void ammocrate_update() __asm__("FUN_L13_002e1cc8");
void asteroid_update() __asm__("FUN_L13_002e1638");
void attached_rocket_update() __asm__("FUN_L13_00305a58");
void blarg_gunner_update() __asm__("FUN_L13_002b4c80");
void blarg_ship_shot_update() __asm__("FUN_L11_00309098");
void blarg_ship_update() __asm__("FUN_L13_0030cdb8");
void blarg_space_commando_update() __asm__("FUN_L13_00307e98");
void blarg_space_fighter_update() __asm__("FUN_L13_002c4428");
void blarg_turret_update() __asm__("FUN_L13_002b41b8");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void bridge_update() __asm__("FUN_L07_0031aee0");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void controller_linked_gate_update() __asm__("FUN_L13_002f8a78");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_camera_target_update() __asm__("FUN_L13_0030bdf0");
void cutscene_trigger_update() __asm__("FUN_L01_002fb5b0");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void door_update() __asm__("FUN_L07_00310df0");
void drone_update() __asm__("FUN_L00_002ce358");
void elevator_update() __asm__("FUN_L13_002b39e0");
void evaluate_descriptor_eligibility() __asm__("FUN_L10_002f65f8");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void falling_spinning_moby_update() __asm__("FUN_L06_00300b90");
void fighter_jet_missile_update() __asm__("FUN_L11_003089d0");
void fighter_jet_rocket_update() __asm__("FUN_L13_002e6c08");
void fighter_jet_update() __asm__("FUN_L13_002bb068");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void forcefield_tower_update() __asm__("FUN_L13_002cefc0");
void forcefield_update() __asm__("FUN_L13_002ed4a8");
void gemlik_bobbing_pool_tile_update() __asm__("FUN_L13_00309e88");
void gemlik_projectile_update() __asm__("FUN_L13_00306300");
void gemlik_scene_controller_update() __asm__("FUN_L13_0030b628");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void gold_weapon_case_update() __asm__("FUN_L01_002e1ac0");
void group_clear_trigger_update() __asm__("FUN_L13_0030af50");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L13_002f3778");
void hit_proxy_update() __asm__("FUN_L13_0030c060");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hovering_rocket_attacker_update() __asm__("FUN_L13_002c25b8");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_path_camera() __asm__("FUN_L10_002f5b58");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_sync_moby_state() __asm__("FUN_L01_002f1d18");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void level_13_homing_rocket_update() __asm__("FUN_L13_002c1528");
void level_13_mine_update() __asm__("FUN_L13_002c2048");
void lift_update() __asm__("FUN_L13_0030a6d8");
void linked_moby_rotation_update() __asm__("FUN_L13_002c7f38");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void linked_switch_group_update() __asm__("FUN_L13_002e4448");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_hold_state_if_grabbed() __asm__("FUN_L01_002f1c78");
void moby_path_follower_update() __asm__("FUN_L13_0030bb70");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void moby_spatial_trigger_update() __asm__("FUN_L05_002f5200");
void moby_state_relay_update() __asm__("FUN_L13_002f8880");
void moby_volume_trigger_update() __asm__("FUN_L13_002b0e80");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void novalis_elevator_update() __asm__("FUN_L01_002b9eb0");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void poison_vessel_fragment_update() __asm__("FUN_L13_0030c3b8");
void poison_vessel_update() __asm__("FUN_L13_00307b10");
void purple_ship_update() __asm__("FUN_L13_002f3750");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void qwark_rilgar_update() __asm__("FUN_L03_002dc478");
void qwarks_starfighter_part_update() __asm__("FUN_L13_002ece00");
void qwarks_starfighter_shield_update() __asm__("FUN_L13_002e84d8");
void qwarks_starfighter_update() __asm__("FUN_L13_002eb098");
void respawn_volume_update() __asm__("FUN_L06_002f7930");
void robosquawker_update() __asm__("FUN_L13_002b50d8");
void run_moby_helper_and_set_state_7() __asm__("FUN_L01_002f1dc8");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void ship_rocket_update() __asm__("FUN_L11_003094f8");
void spawn_point_update() __asm__("FUN_L01_00300220");
void spawner_egg_update() __asm__("FUN_L09_0030b3b0");
void spawner_update() __asm__("FUN_L09_0030ab80");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void tracked_group_watcher_update() __asm__("FUN_L13_0030be60");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void visibomb_zone_timer_update() __asm__("FUN_L01_00302648");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L13_001F615C")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L13_001F6174[6]

    __attribute__((section("lvl.vtbl"))) = {moby_hold_state_if_grabbed,
                                            moby_hold_state_if_grabbed,
                                            launch_suck_cannon_projectile_and_sync_moby_state,
                                            run_moby_helper_and_set_state_7,
                                            get_moby_data_plus_0x60,
                                            hud_counter_update};

void *D_L13_001F618C[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[189] __asm__("D_L13_001F5880")

    __attribute__((section("lvl.vtbl"))) = {
        { 726,          novalis_elevator_update, default_moby_methods},
        {1007,             camera_object_update, default_moby_methods},
        {   6,       moby_volume_trigger_update, default_moby_methods},
        {  11,                    vendor_update, default_moby_methods},
        {  13,                      bolt_update, default_moby_methods},
        {  14,                      bolt_update, default_moby_methods},
        {  15,                      bolt_update, default_moby_methods},
        {  16,                      bolt_update, default_moby_methods},
        {  21,                  elevator_update, default_moby_methods},
        {  29,              blarg_turret_update, default_moby_methods},
        {  36,              blarg_gunner_update, default_moby_methods},
        {  63,              robosquawker_update,       D_L13_001F6174},
        {  69,               fighter_jet_update, default_moby_methods},
        {  71,                    wrench_update, default_moby_methods},
        {  74,                      mine_update, default_moby_methods},
        {  82,    level_13_homing_rocket_update, default_moby_methods},
        {  83,             level_13_mine_update, default_moby_methods},
        { 101,  hovering_rocket_attacker_update, default_moby_methods},
        { 104,                    bridge_update, default_moby_methods},
        { 106,                    bridge_update, default_moby_methods},
        { 111,       blarg_space_fighter_update, default_moby_methods},
        { 112,             glow_particle_update, default_moby_methods},
        { 121,                      bomb_update, default_moby_methods},
        { 122,                    debris_update, default_moby_methods},
        { 127,      linked_moby_rotation_update, default_moby_methods},
        { 128,      linked_moby_rotation_update, default_moby_methods},
        { 129,                      door_update, default_moby_methods},
        { 130,                      door_update, default_moby_methods},
        { 149,   nanotech_crate_fragment_update, default_moby_methods},
        { 153,   gold_devastator_missile_update, default_moby_methods},
        { 157,                devastator_update, default_moby_methods},
        { 159,      linked_moby_rotation_update, default_moby_methods},
        { 163,                  visibomb_update, default_moby_methods},
        { 166,        linked_moby_states_update, default_moby_methods},
        { 167,       thruster_pack_flame_update, default_moby_methods},
        { 168,                   blaster_update, default_moby_methods},
        { 169,      linked_moby_rotation_update, default_moby_methods},
        { 170,          forcefield_tower_update, default_moby_methods},
        { 172,          visibomb_missile_update, default_moby_methods},
        { 175,                   taunter_update, default_moby_methods},
        { 176,                 pyrocitor_update, default_moby_methods},
        { 177,                tesla_claw_update, default_moby_methods},
        { 179,            trailing_flame_update, default_moby_methods},
        { 180,                  walloper_update, default_moby_methods},
        { 182,                      door_update, default_moby_methods},
        { 183,                      door_update, default_moby_methods},
        { 185,                 morphoray_update, default_moby_methods},
        { 186,                  doom_bot_update, default_moby_methods},
        { 188,                trespasser_update, default_moby_methods},
        { 190,                mine_glove_update, default_moby_methods},
        { 192,                bomb_glove_update, default_moby_methods},
        { 203,                     decoy_update, default_moby_methods},
        { 204,           devestator_ammo_update, default_moby_methods},
        { 208,                 swingshot_update, default_moby_methods},
        { 209,            swingshot_hook_update, default_moby_methods},
        { 212,                  asteroid_update, default_moby_methods},
        { 213,           devestator_ammo_update, default_moby_methods},
        { 214,           devestator_ammo_update, default_moby_methods},
        { 222,           devestator_ammo_update, default_moby_methods},
        { 223,           devestator_ammo_update, default_moby_methods},
        { 224,                 ammocrate_update, default_moby_methods},
        { 225,           devestator_ammo_update, default_moby_methods},
        { 226,           devestator_ammo_update, default_moby_methods},
        { 228,                 ammocrate_update, default_moby_methods},
        { 229,             glove_of_doom_update, default_moby_methods},
        { 230,          doom_bot_spawner_update, default_moby_methods},
        { 231,       linked_switch_group_update, default_moby_methods},
        { 244,                  elevator_update, default_moby_methods},
        { 258,      moby_spatial_trigger_update, default_moby_methods},
        { 270,         morphoray_chicken_update,       D_L13_001F618C},
        { 295,        fighter_jet_rocket_update, default_moby_methods},
        { 304,          gold_weapon_case_update, default_moby_methods},
        { 305,               homing_shot_update, default_moby_methods},
        { 348,   nanotech_crate_fragment_update, default_moby_methods},
        { 349,   nanotech_crate_fragment_update, default_moby_methods},
        { 352, qwarks_starfighter_shield_update, default_moby_methods},
        { 354,   nanotech_crate_fragment_update, default_moby_methods},
        { 355,   nanotech_crate_fragment_update, default_moby_methods},
        { 356,   nanotech_crate_fragment_update, default_moby_methods},
        { 357,   nanotech_crate_fragment_update, default_moby_methods},
        { 358,   nanotech_crate_fragment_update, default_moby_methods},
        { 359,   nanotech_crate_fragment_update, default_moby_methods},
        { 360,                      door_update, default_moby_methods},
        { 363,   nanotech_crate_fragment_update, default_moby_methods},
        { 364,   nanotech_crate_fragment_update, default_moby_methods},
        { 388,        qwarks_starfighter_update, default_moby_methods},
        { 389,   qwarks_starfighter_part_update, default_moby_methods},
        { 390,   qwarks_starfighter_part_update, default_moby_methods},
        { 391,   qwarks_starfighter_part_update, default_moby_methods},
        { 392,   qwarks_starfighter_part_update, default_moby_methods},
        { 393,   qwarks_starfighter_part_update, default_moby_methods},
        { 394,   qwarks_starfighter_part_update, default_moby_methods},
        { 395,   qwarks_starfighter_part_update, default_moby_methods},
        { 396,   qwarks_starfighter_part_update, default_moby_methods},
        { 399,   qwarks_starfighter_part_update, default_moby_methods},
        { 400,   qwarks_starfighter_part_update, default_moby_methods},
        { 401,   qwarks_starfighter_part_update, default_moby_methods},
        { 404,                forcefield_update, default_moby_methods},
        { 405,                forcefield_update, default_moby_methods},
        { 428,       damped_falling_moby_update, default_moby_methods},
        { 433,               noop_main_callback, default_moby_methods},
        { 447,              qwark_rilgar_update, default_moby_methods},
        { 454,                      ryno_update, default_moby_methods},
        { 457,               ryno_rocket_update, default_moby_methods},
        { 479,                     drone_update, default_moby_methods},
        { 483,                                0, default_moby_methods},
        { 500,                     crate_update, default_moby_methods},
        { 501,                     crate_update, default_moby_methods},
        { 505,                     crate_update, default_moby_methods},
        { 511,                     crate_update, default_moby_methods},
        { 533,               purple_ship_update, default_moby_methods},
        { 558,                help_hints_update, default_moby_methods},
        { 562,               decoy_glove_update, default_moby_methods},
        { 585,            metal_detector_update, default_moby_methods},
        { 605,              buried_bolts_update, default_moby_methods},
        { 607,                  helipack_update, default_moby_methods},
        { 615,           tresspasser_pad_update, default_moby_methods},
        { 618,               noop_main_callback, default_moby_methods},
        { 634,                                0, default_moby_methods},
        { 639,           explosion_light_update, default_moby_methods},
        { 667,          moby_state_relay_update, default_moby_methods},
        { 674,    controller_linked_gate_update, default_moby_methods},
        { 677,    controller_linked_gate_update, default_moby_methods},
        { 680,    controller_linked_gate_update, default_moby_methods},
        { 737,          cutscene_trigger_update, default_moby_methods},
        { 750,                   infobot_update, default_moby_methods},
        { 758,     swingshot_versatarget_update, default_moby_methods},
        { 775,         grow_and_fade_out_update, default_moby_methods},
        { 803,     swingshot_versatarget_update, default_moby_methods},
        { 804,   morphoray_target_effect_update, default_moby_methods},
        { 805,               spawn_point_update, default_moby_methods},
        { 806,             nanotech_ball_update, default_moby_methods},
        { 832,       visibomb_zone_timer_update, default_moby_methods},
        { 849,                suckcannon_update, default_moby_methods},
        { 913,               noop_main_callback, default_moby_methods},
        {1006,           devestator_ammo_update, default_moby_methods},
        {1009,       fighter_jet_missile_update, default_moby_methods},
        {1017,           blarg_ship_shot_update, default_moby_methods},
        {1034,               ship_rocket_update, default_moby_methods},
        {1039,            respawn_volume_update, default_moby_methods},
        {1085,     falling_spinning_moby_update, default_moby_methods},
        {1134,                 gold_bolt_update, default_moby_methods},
        {1135,                teleporter_update, default_moby_methods},
        {1138,                 quad_moby_update, default_moby_methods},
        {1143,                vendor_orb_update, default_moby_methods},
        {1192,             glow_particle_update, default_moby_methods},
        {1233,           attached_rocket_update, default_moby_methods},
        {1238,         gemlik_projectile_update, default_moby_methods},
        {1251,            hydrodisplacer_update, default_moby_methods},
        {1261,             poison_vessel_update, default_moby_methods},
        {1262,      blarg_space_commando_update, default_moby_methods},
        {1263,  gemlik_bobbing_pool_tile_update, default_moby_methods},
        {1270,                      lift_update, default_moby_methods},
        {1271,       group_clear_trigger_update, default_moby_methods},
        {1289,               noop_main_callback, default_moby_methods},
        {1290,             pilots_helmet_update, default_moby_methods},
        {1353,   gemlik_scene_controller_update, default_moby_methods},
        {1393,  gemlik_bobbing_pool_tile_update, default_moby_methods},
        {1403,        moby_path_follower_update, default_moby_methods},
        {1412,                  asteroid_update, default_moby_methods},
        {1438,           devestator_ammo_update, default_moby_methods},
        {1447,           devestator_ammo_update, default_moby_methods},
        {1449,           devestator_ammo_update, default_moby_methods},
        {1456,          gold_weapon_case_update, default_moby_methods},
        {1457,          gold_weapon_case_update, default_moby_methods},
        {1458,          gold_weapon_case_update, default_moby_methods},
        {1459,          gold_weapon_case_update, default_moby_methods},
        {1460,          gold_weapon_case_update, default_moby_methods},
        {1461,          gold_weapon_case_update, default_moby_methods},
        {1462,          gold_weapon_case_update, default_moby_methods},
        {1463,          gold_weapon_case_update, default_moby_methods},
        {1464,          gold_weapon_case_update, default_moby_methods},
        {1465,          gold_weapon_case_update, default_moby_methods},
        {1558,    cutscene_camera_target_update, default_moby_methods},
        {1577,     tracked_group_watcher_update, default_moby_methods},
        {1632,                 hit_proxy_update, default_moby_methods},
        {1634,    poison_vessel_fragment_update, default_moby_methods},
        {1733,             tumbling_part_update, default_moby_methods},
        {1734,             tumbling_part_update, default_moby_methods},
        {1735,             tumbling_part_update, default_moby_methods},
        {1801,             tumbling_part_update, default_moby_methods},
        {1802,             tumbling_part_update, default_moby_methods},
        {1803,             tumbling_part_update, default_moby_methods},
        {1804,             tumbling_part_update, default_moby_methods},
        {1805,                blarg_ship_update, default_moby_methods},
        {1885,                   spawner_update, default_moby_methods},
        {1886,               spawner_egg_update, default_moby_methods},
        {1900,                     decoy_update, default_moby_methods},
        {  -1,                                0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L13_001F6200") __attribute__((section("lvl.camvtbl"))) = {
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
    {19,
     evaluate_descriptor_eligibility,            initialize_path_camera,
     FUN_L10_002f5f18,                          noop_main_callback                                                                        },
    {23,         volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                           0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[3] __asm__("D_L13_001F6300")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 1,                        update_box_emitter},
        {-1,                                         0},
};
