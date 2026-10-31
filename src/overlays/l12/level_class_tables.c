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
void blarg_commander_update() __asm__("FUN_L12_00308918");
void blarg_helicopter_update() __asm__("FUN_L12_002e9f68");
void blarg_trooper_projectile_update() __asm__("FUN_L04_002af058");
void blarg_trooper_update() __asm__("FUN_L12_002e72c0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void buried_bolts_update() __asm__("FUN_L01_002f2eb8");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_hoven_anklebiter_suck_cannon_suction() __asm__("FUN_L12_002e17f8");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void clip_volume_scene_trigger_update() __asm__("FUN_L12_003093c8");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crane_arm_update() __asm__("FUN_L12_002ec1d0");
void crate_update() __asm__("FUN_L00_002cf218");
void cutscene_camera_target_update() __asm__("FUN_L12_003094a0");
void damped_falling_moby_update() __asm__("FUN_L00_002cc210");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_spawner_update() __asm__("FUN_L00_002c89e8");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void drone_update() __asm__("FUN_L00_002ce358");
void edwina_update() __asm__("FUN_L12_002eb570");
void effects_draw_trigger_update() __asm__("FUN_L12_002e7208");
void escalator_update() __asm__("FUN_L02_002dc6b0");
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void falling_moby_update() __asm__("FUN_L03_002de370");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void ground_particle_scatter_update() __asm__("FUN_L08_00307cf0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helicopter_update() __asm__("FUN_L12_002ebea8");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L12_002ed280");
void homing_mine_spawner_update() __asm__("FUN_L12_00307840");
void homing_mine_update() __asm__("FUN_L12_00304e00");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void hoven_anklebiter_update() __asm__("FUN_L12_002e1be0");
void hoven_turret_update() __asm__("FUN_L12_00303540");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_faucet_update() __asm__("FUN_L05_002f8080");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void jittered_quad_layers_update() __asm__("FUN_L12_003081b0");
void lamp_update() __asm__("FUN_L12_0030b6c8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void level_12_water_tile_update() __asm__("FUN_L12_002bf140");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void miner_update() __asm__("FUN_L12_002e6c48");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void moby_spatial_trigger_update() __asm__("FUN_L05_002f5200");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void oscillating_target_link_update() __asm__("FUN_L12_00302f30");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void planetbuster_maximus_part_update() __asm__("FUN_L12_003069d0");
void platform_update() __asm__("FUN_L12_002e3ed8");
void process_hoven_anklebiter_suck_cannon_suction() __asm__("FUN_L12_002e16b8");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void remove_when_near_hero_update() __asm__("FUN_L11_002cb790");
void rocket_update() __asm__("FUN_L12_002ece00");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void sandmouse_house_update() __asm__("FUN_L01_002f2b68");
void sandmouse_projectile_update() __asm__("FUN_L01_0030c9a8");
void sandmouse_update() __asm__("FUN_L01_0030df40");
void scrolling_texture_layers_update() __asm__("FUN_L12_0030bff0");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void spawn_point_update() __asm__("FUN_L01_00300220");
void store_hoven_anklebiter_in_suck_cannon() __asm__("FUN_L12_002e1758");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void timed_activation_pads_update() __asm__("FUN_L07_0030bf90");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void turret_rocket_update() __asm__("FUN_L08_002e8210");
void update_box_emitter() __asm__("FUN_L01_00319928");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_listener_emitter() __asm__("FUN_L01_0031a078");
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
void water_reservoir_update() __asm__("FUN_L12_002ec720");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L12_0020D210")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L12_0020D228[6]

    __attribute__((section("lvl.vtbl"))) = {process_hoven_anklebiter_suck_cannon_suction,
                                            store_hoven_anklebiter_in_suck_cannon,
                                            store_hoven_anklebiter_in_suck_cannon,
                                            cancel_hoven_anklebiter_suck_cannon_suction,
                                            get_moby_data_plus_0x60,
                                            hud_counter_update};

void *D_L12_0020D240[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[140] __asm__("D_L12_0020CB80")

    __attribute__((section("lvl.vtbl"))) = {
        {1007,             camera_object_update, default_moby_methods},
        {  11,                    vendor_update, default_moby_methods},
        {  13,                      bolt_update, default_moby_methods},
        {  14,                      bolt_update, default_moby_methods},
        {  15,                      bolt_update, default_moby_methods},
        {  16,                      bolt_update, default_moby_methods},
        {  19,       level_12_water_tile_update, default_moby_methods},
        {  37,     remove_when_near_hero_update, default_moby_methods},
        {  71,                    wrench_update, default_moby_methods},
        {  74,                      mine_update, default_moby_methods},
        { 112,             glow_particle_update, default_moby_methods},
        { 121,                      bomb_update, default_moby_methods},
        { 122,                    debris_update, default_moby_methods},
        { 149,   nanotech_crate_fragment_update, default_moby_methods},
        { 153,   gold_devastator_missile_update, default_moby_methods},
        { 157,                devastator_update, default_moby_methods},
        { 163,                  visibomb_update, default_moby_methods},
        { 164,               noop_main_callback, default_moby_methods},
        { 166,        linked_moby_states_update, default_moby_methods},
        { 167,       thruster_pack_flame_update, default_moby_methods},
        { 168,                   blaster_update, default_moby_methods},
        { 172,          visibomb_missile_update, default_moby_methods},
        { 175,                   taunter_update, default_moby_methods},
        { 176,                 pyrocitor_update, default_moby_methods},
        { 177,                tesla_claw_update, default_moby_methods},
        { 178,               noop_main_callback, default_moby_methods},
        { 179,            trailing_flame_update, default_moby_methods},
        { 180,                  walloper_update, default_moby_methods},
        { 184,  blarg_trooper_projectile_update, default_moby_methods},
        { 185,                 morphoray_update, default_moby_methods},
        { 186,                  doom_bot_update, default_moby_methods},
        { 188,                trespasser_update, default_moby_methods},
        { 190,                mine_glove_update, default_moby_methods},
        { 192,                bomb_glove_update, default_moby_methods},
        { 203,                     decoy_update, default_moby_methods},
        { 204,           devestator_ammo_update, default_moby_methods},
        { 208,                 swingshot_update, default_moby_methods},
        { 209,            swingshot_hook_update, default_moby_methods},
        { 213,           devestator_ammo_update, default_moby_methods},
        { 214,           devestator_ammo_update, default_moby_methods},
        { 222,           devestator_ammo_update, default_moby_methods},
        { 223,           devestator_ammo_update, default_moby_methods},
        { 225,           devestator_ammo_update, default_moby_methods},
        { 226,           devestator_ammo_update, default_moby_methods},
        { 229,             glove_of_doom_update, default_moby_methods},
        { 230,          doom_bot_spawner_update, default_moby_methods},
        { 238,          hoven_anklebiter_update,       D_L12_0020D228},
        { 240,                  platform_update, default_moby_methods},
        { 258,      moby_spatial_trigger_update, default_moby_methods},
        { 270,         morphoray_chicken_update,       D_L12_0020D240},
        { 282,                     miner_update, default_moby_methods},
        { 293,      effects_draw_trigger_update, default_moby_methods},
        { 294,             blarg_trooper_update, default_moby_methods},
        { 296,                 escalator_update, default_moby_methods},
        { 305,               homing_shot_update, default_moby_methods},
        { 326,          blarg_helicopter_update, default_moby_methods},
        { 328,                    edwina_update, default_moby_methods},
        { 336,                helicopter_update, default_moby_methods},
        { 339,                 crane_arm_update, default_moby_methods},
        { 341,     hydrodisplacer_faucet_update, default_moby_methods},
        { 346,               noop_main_callback, default_moby_methods},
        { 348,   nanotech_crate_fragment_update, default_moby_methods},
        { 349,   nanotech_crate_fragment_update, default_moby_methods},
        { 354,   nanotech_crate_fragment_update, default_moby_methods},
        { 355,   nanotech_crate_fragment_update, default_moby_methods},
        { 356,   nanotech_crate_fragment_update, default_moby_methods},
        { 357,   nanotech_crate_fragment_update, default_moby_methods},
        { 358,   nanotech_crate_fragment_update, default_moby_methods},
        { 359,   nanotech_crate_fragment_update, default_moby_methods},
        { 363,   nanotech_crate_fragment_update, default_moby_methods},
        { 364,   nanotech_crate_fragment_update, default_moby_methods},
        { 384,           water_reservoir_update, default_moby_methods},
        { 409,                    rocket_update, default_moby_methods},
        { 422,                help_hints_update, default_moby_methods},
        { 428,       damped_falling_moby_update, default_moby_methods},
        { 433,               noop_main_callback, default_moby_methods},
        { 454,                      ryno_update, default_moby_methods},
        { 457,               ryno_rocket_update, default_moby_methods},
        { 458,             turret_rocket_update, default_moby_methods},
        { 479,                     drone_update, default_moby_methods},
        { 483,                                0, default_moby_methods},
        { 499,               noop_main_callback, default_moby_methods},
        { 500,                     crate_update, default_moby_methods},
        { 501,                     crate_update, default_moby_methods},
        { 502,                     crate_update, default_moby_methods},
        { 505,                     crate_update, default_moby_methods},
        { 511,                     crate_update, default_moby_methods},
        { 562,               decoy_glove_update, default_moby_methods},
        { 585,            metal_detector_update, default_moby_methods},
        { 604,           sandmouse_house_update, default_moby_methods},
        { 605,              buried_bolts_update, default_moby_methods},
        { 607,                  helipack_update, default_moby_methods},
        { 618,               noop_main_callback, default_moby_methods},
        { 634,                                0, default_moby_methods},
        { 639,           explosion_light_update, default_moby_methods},
        { 750,                   infobot_update, default_moby_methods},
        { 758,     swingshot_versatarget_update, default_moby_methods},
        { 775,         grow_and_fade_out_update, default_moby_methods},
        { 804,   morphoray_target_effect_update, default_moby_methods},
        { 805,               spawn_point_update, default_moby_methods},
        { 806,             nanotech_ball_update, default_moby_methods},
        { 832,       visibomb_zone_timer_update, default_moby_methods},
        { 849,                suckcannon_update, default_moby_methods},
        { 886,     timed_activation_pads_update, default_moby_methods},
        {1006,           devestator_ammo_update, default_moby_methods},
        {1134,                 gold_bolt_update, default_moby_methods},
        {1135,                teleporter_update, default_moby_methods},
        {1138,                 quad_moby_update, default_moby_methods},
        {1143,                vendor_orb_update, default_moby_methods},
        {1192,             glow_particle_update, default_moby_methods},
        {1221,              falling_moby_update, default_moby_methods},
        {1222,              falling_moby_update, default_moby_methods},
        {1251,            hydrodisplacer_update, default_moby_methods},
        {1259,   oscillating_target_link_update, default_moby_methods},
        {1267,              hoven_turret_update, default_moby_methods},
        {1269,               homing_mine_update, default_moby_methods},
        {1272,                                0, default_moby_methods},
        {1274, planetbuster_maximus_part_update, default_moby_methods},
        {1278,               noop_main_callback, default_moby_methods},
        {1280,               noop_main_callback, default_moby_methods},
        {1281,       homing_mine_spawner_update, default_moby_methods},
        {1289,               noop_main_callback, default_moby_methods},
        {1290,             pilots_helmet_update, default_moby_methods},
        {1322,               noop_main_callback, default_moby_methods},
        {1345,      jittered_quad_layers_update, default_moby_methods},
        {1371,           blarg_commander_update, default_moby_methods},
        {1400,   ground_particle_scatter_update, default_moby_methods},
        {1404, clip_volume_scene_trigger_update, default_moby_methods},
        {1438,           devestator_ammo_update, default_moby_methods},
        {1447,           devestator_ammo_update, default_moby_methods},
        {1449,           devestator_ammo_update, default_moby_methods},
        {1557,    cutscene_camera_target_update, default_moby_methods},
        {1633,      sandmouse_projectile_update, default_moby_methods},
        {1818,                 sandmouse_update, default_moby_methods},
        {1862,                      lamp_update, default_moby_methods},
        {1863,               noop_main_callback, default_moby_methods},
        {1864,             tumbling_part_update, default_moby_methods},
        {1900,                     decoy_update, default_moby_methods},
        {1901,  scrolling_texture_layers_update, default_moby_methods},
        {  -1,                                0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L12_0020D280") __attribute__((section("lvl.camvtbl"))) = {
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
    {-1,                          0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[6] __asm__("D_L12_0020D380")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0, update_sound_emitter_playback_by_distance},
        { 1,                        update_box_emitter},
        { 3,                 update_reverb_box_emitter},
        { 5,                   update_listener_emitter},
        { 6,               update_music_switch_emitter},
        {-1,                                         0},
};
