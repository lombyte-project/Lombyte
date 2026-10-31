/* The level's moby classes, camera types and sound emitter types: the tables
   the executable's code looks ids up in (sections lvl.vtbl, lvl.camvtbl and
   lvl.sndvtbl). */
#include "types.h"
#include "rnc/audio/sound_handlers.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby_class_tables.h"

void FUN_L00_002caf40();
void FUN_L00_002ea7a0();
void armed_transport_rocket_update() __asm__("FUN_L16_002a0a98");
void armed_transport_update() __asm__("FUN_L16_002e22d8");
void blarg_elite_commando_rocket_update() __asm__("FUN_L15_0029ac30");
void blarg_elite_commando_update() __asm__("FUN_L16_002d2cf0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void boss_camera_transition_update() __asm__("FUN_L18_002df608");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_moby_state_9() __asm__("FUN_L00_002d57d0");
void child_carrier_update() __asm__("FUN_L18_002fa728");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_joint_trail_update() __asm__("FUN_L18_002fad28");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void deplanetizer_effects_update() __asm__("FUN_L18_002f88e8");
void deplanetizer_gun_update() __asm__("FUN_L18_002f7ad8");
void destructible_scenery_update() __asm__("FUN_L18_002e9768");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drek_boss_damage_ring_update() __asm__("FUN_L18_002dc260");
void drek_boss_electric_ball_update() __asm__("FUN_L18_002e9e70");
void drek_boss_ring_projectile_update() __asm__("FUN_L18_002db460");
void drek_fight_missile_update() __asm__("FUN_L18_002efb88");
void drek_final_boss_robot_update() __asm__("FUN_L18_002f2bf0");
void drone_update() __asm__("FUN_L00_002ce358");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void falling_moby_group_update() __asm__("FUN_L18_002f16f0");
void floating_platform_update() __asm__("FUN_L18_002d82c0");
void floor_button_update() __asm__("FUN_L05_0030c5b0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void gadgebot_update() __asm__("FUN_L18_002fbb98");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void get_moby_private_data_d0_address() __asm__("FUN_L00_002d5800");
void giant_clank_charged_blast_update() __asm__("FUN_L15_0029edb0");
void giant_clank_hit_sphere_update() __asm__("FUN_L15_0029ead0");
void giant_clank_missile_update() __asm__("FUN_L15_0029d6c0");
void giant_clank_pad_update() __asm__("FUN_L15_002ed068");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grind_rail_junction_update() __asm__("FUN_L18_002d62e8");
void grindrail_barrier_update() __asm__("FUN_L16_002e5708");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hidden_deplanetizer_gun_update() __asm__("FUN_L18_002fa640");
void hide_moby_and_clear_collision() __asm__("FUN_L18_002fcd00");
void hide_moby_and_disable_collision() __asm__("FUN_L18_002d6280");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void horny_toad_update() __asm__("FUN_L00_002d4610");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_faucet_update() __asm__("FUN_L05_002f8080");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void inert_moby_update() __asm__("FUN_L18_002f7ab0");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void lift_update() __asm__("FUN_L18_002f2310");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_568_update() __asm__("FUN_L18_002d5918");
void moby_hold_state_while_active() __asm__("FUN_L18_002d6190");
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
void process_suck_cannon_suction_and_update_moby_state() __asm__("FUN_L00_002d56e0");
void pterodactyl_update() __asm__("FUN_L00_002e3a88");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void red_orb_update() __asm__("FUN_L00_002df4f8");
void register_audio_stream_callback() __asm__("FUN_00215420");
void respawn_volume_update() __asm__("FUN_L06_002f7930");
void rock_update() __asm__("FUN_L01_002f8718");
void rocket_moby_update() __asm__("FUN_L18_002d5050");
void rocket_tank_rocket_update() __asm__("FUN_L18_002a7220");
void rocket_tank_update() __asm__("FUN_L18_002f7c40");
void run_moby_helper_and_set_state_7() __asm__("FUN_L01_002f1dc8");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void thruster_pack_lock_green_update() __asm__("FUN_L18_002d6600");
void thruster_pack_lock_red_update() __asm__("FUN_L18_002d7b20");
void timed_sprite_effect_update() __asm__("FUN_L06_0030be60");
void trackdrive_left_update() __asm__("FUN_L02_002cb978");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_barrier_update() __asm__("FUN_L18_002f1e68");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_listener_emitter() __asm__("FUN_L01_0031a078");
void update_moby_camera_control() __asm__("FUN_L02_002fc298");
void update_moby_state_pause() __asm__("FUN_L18_002d6108");
void update_moby_suck_cannon_state() __asm__("FUN_L18_002fcbb0");
void update_music_switch_emitter() __asm__("FUN_L01_0031a128");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_sound_emitter_playback_at_random_position() __asm__("FUN_L00_002edad8");
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

void *default_moby_methods[6] __asm__("D_L18_001F3468")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L18_001F3480[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

void *D_L18_001F3498[6] __attribute__((section("lvl.vtbl"))) = {update_moby_state_pause,
                                                                moby_hold_state_while_active,
                                                                moby_hold_state_while_active,
                                                                hud_counter_update,
                                                                get_moby_data_plus_0x60,
                                                                hide_moby_and_disable_collision};

void *D_L18_001F34B0[6]

    __attribute__((section("lvl.vtbl"))) = {process_suck_cannon_suction_and_update_moby_state,
                                            process_suck_cannon_suction_and_update_moby_state,
                                            process_suck_cannon_suction_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_9,
                                            get_moby_private_data_d0_address,
                                            hud_counter_update};

void *D_L18_001F34C8[6] __attribute__((section("lvl.vtbl"))) = {update_moby_suck_cannon_state,
                                                                update_moby_suck_cannon_state,
                                                                update_moby_suck_cannon_state,
                                                                run_moby_helper_and_set_state_7,
                                                                get_moby_private_data_d0_address,
                                                                hide_moby_and_clear_collision};

struct MobyClassUpdate moby_classes[158] __asm__("D_L18_001F2D00")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,               camera_object_update, default_moby_methods},
        {  11,                      vendor_update, default_moby_methods},
        {  13,                        bolt_update, default_moby_methods},
        {  14,                        bolt_update, default_moby_methods},
        {  15,                        bolt_update, default_moby_methods},
        {  16,                        bolt_update, default_moby_methods},
        {  41,          rocket_tank_rocket_update, default_moby_methods},
        {  49, blarg_elite_commando_rocket_update, default_moby_methods},
        {  50,      armed_transport_rocket_update, default_moby_methods},
        {  71,                      wrench_update, default_moby_methods},
        {  74,                        mine_update, default_moby_methods},
        { 112,               glow_particle_update, default_moby_methods},
        { 121,                        bomb_update, default_moby_methods},
        { 122,                      debris_update, default_moby_methods},
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
        { 256,         giant_clank_missile_update, default_moby_methods},
        { 270,           morphoray_chicken_update,       D_L18_001F3480},
        { 305,                 homing_shot_update, default_moby_methods},
        { 331,             trackdrive_left_update, default_moby_methods},
        { 341,       hydrodisplacer_faucet_update, default_moby_methods},
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
        { 419,                 noop_main_callback, default_moby_methods},
        { 428,         damped_falling_moby_update, default_moby_methods},
        { 433,                 noop_main_callback, default_moby_methods},
        { 454,                        ryno_update, default_moby_methods},
        { 457,                 ryno_rocket_update, default_moby_methods},
        { 479,                       drone_update, default_moby_methods},
        { 483,                                  0, default_moby_methods},
        { 500,                       crate_update, default_moby_methods},
        { 501,                       crate_update, default_moby_methods},
        { 502,                       crate_update, default_moby_methods},
        { 505,                       crate_update, default_moby_methods},
        { 511,                       crate_update, default_moby_methods},
        { 562,                 decoy_glove_update, default_moby_methods},
        { 564,                 rocket_moby_update, default_moby_methods},
        { 568,                    moby_568_update,       D_L18_001F3498},
        { 582,         grind_rail_junction_update, default_moby_methods},
        { 583,    thruster_pack_lock_green_update, default_moby_methods},
        { 585,              metal_detector_update, default_moby_methods},
        { 586,      thruster_pack_lock_red_update, default_moby_methods},
        { 587,           floating_platform_update, default_moby_methods},
        { 605,                buried_bolts_update, default_moby_methods},
        { 607,                    helipack_update, default_moby_methods},
        { 615,             tresspasser_pad_update, default_moby_methods},
        { 618,                 noop_main_callback, default_moby_methods},
        { 624,   drek_boss_ring_projectile_update, default_moby_methods},
        { 628,       drek_boss_damage_ring_update, default_moby_methods},
        { 634,                                  0, default_moby_methods},
        { 638,        blarg_elite_commando_update, default_moby_methods},
        { 639,             explosion_light_update, default_moby_methods},
        { 644,      boss_camera_transition_update, default_moby_methods},
        { 696,                        rock_update, default_moby_methods},
        { 749,                  horny_toad_update,       D_L18_001F34B0},
        { 758,       swingshot_versatarget_update, default_moby_methods},
        { 775,           grow_and_fade_out_update, default_moby_methods},
        { 803,       swingshot_versatarget_update, default_moby_methods},
        { 804,     morphoray_target_effect_update, default_moby_methods},
        { 805,                 spawn_point_update, default_moby_methods},
        { 806,               nanotech_ball_update, default_moby_methods},
        { 830,                floor_button_update, default_moby_methods},
        { 832,         visibomb_zone_timer_update, default_moby_methods},
        { 849,                  suckcannon_update, default_moby_methods},
        { 885,        destructible_scenery_update, default_moby_methods},
        { 888,        destructible_scenery_update, default_moby_methods},
        { 891,        destructible_scenery_update, default_moby_methods},
        { 892,        destructible_scenery_update, default_moby_methods},
        { 894,        destructible_scenery_update, default_moby_methods},
        { 900,        destructible_scenery_update, default_moby_methods},
        { 901,        destructible_scenery_update, default_moby_methods},
        { 936,        destructible_scenery_update, default_moby_methods},
        { 983,     drek_boss_electric_ball_update, default_moby_methods},
        {1006,             devestator_ammo_update, default_moby_methods},
        {1021,         moby_rotation_group_update, default_moby_methods},
        {1039,              respawn_volume_update, default_moby_methods},
        {1060,                     red_orb_update, default_moby_methods},
        {1134,                   gold_bolt_update, default_moby_methods},
        {1135,                  teleporter_update, default_moby_methods},
        {1138,                   quad_moby_update, default_moby_methods},
        {1143,                  vendor_orb_update, default_moby_methods},
        {1192,               glow_particle_update, default_moby_methods},
        {1249,                 noop_main_callback, default_moby_methods},
        {1251,              hydrodisplacer_update, default_moby_methods},
        {1289,                 noop_main_callback, default_moby_methods},
        {1290,               pilots_helmet_update, default_moby_methods},
        {1355,          drek_fight_missile_update, default_moby_methods},
        {1356,             armed_transport_update, default_moby_methods},
        {1381,          falling_moby_group_update, default_moby_methods},
        {1392,          trespasser_barrier_update, default_moby_methods},
        {1402,                        lift_update, default_moby_methods},
        {1422,       drek_final_boss_robot_update, default_moby_methods},
        {1427,      giant_clank_hit_sphere_update, default_moby_methods},
        {1432,                  inert_moby_update, default_moby_methods},
        {1434,            deplanetizer_gun_update, default_moby_methods},
        {1435,            deplanetizer_gun_update, default_moby_methods},
        {1438,             devestator_ammo_update, default_moby_methods},
        {1447,             devestator_ammo_update, default_moby_methods},
        {1449,             devestator_ammo_update, default_moby_methods},
        {1454,                 rocket_tank_update, default_moby_methods},
        {1523,   giant_clank_charged_blast_update, default_moby_methods},
        {1563,        deplanetizer_effects_update, default_moby_methods},
        {1564,                 pterodactyl_update, default_moby_methods},
        {1568,     hidden_deplanetizer_gun_update, default_moby_methods},
        {1584,               child_carrier_update, default_moby_methods},
        {1588,               tumbling_part_update, default_moby_methods},
        {1589,               tumbling_part_update, default_moby_methods},
        {1590,               tumbling_part_update, default_moby_methods},
        {1750,     register_audio_stream_callback, default_moby_methods},
        {1790,               tumbling_part_update, default_moby_methods},
        {1791,               tumbling_part_update, default_moby_methods},
        {1792,               tumbling_part_update, default_moby_methods},
        {1793,               tumbling_part_update, default_moby_methods},
        {1794,               tumbling_part_update, default_moby_methods},
        {1796,               tumbling_part_update, default_moby_methods},
        {1799,        cutscene_joint_trail_update, default_moby_methods},
        {1890,           grindrail_barrier_update, default_moby_methods},
        {1898,         timed_sprite_effect_update, default_moby_methods},
        {1899,             giant_clank_pad_update, default_moby_methods},
        {1900,                       decoy_update, default_moby_methods},
        {1906,                    gadgebot_update,       D_L18_001F34C8},
        {  -1,                                  0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L18_001F3500") __attribute__((section("lvl.camvtbl"))) = {
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
    {18, update_moby_camera_control,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,        volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                          0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[6] __asm__("D_L18_001F3600")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0,        update_sound_emitter_playback_by_distance},
        { 2, update_sound_emitter_playback_at_random_position},
        { 3,                        update_reverb_box_emitter},
        { 5,                          update_listener_emitter},
        { 6,                      update_music_switch_emitter},
        {-1,                                                0},
};
