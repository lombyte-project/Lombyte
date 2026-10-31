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
void beam_slot_controller_update() __asm__("FUN_L00_002e1df0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_moby_state_9() __asm__("FUN_L00_002d57d0");
void clank_cutscene_trigger_update() __asm__("FUN_L00_002d9dc8");
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
void explosion_light_update() __asm__("FUN_L00_002d3a10");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_private_data_d0_address() __asm__("FUN_L00_002d5800");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void glow_particle_update() __asm__("FUN_L00_002acad8");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void help_hints_update() __asm__("FUN_L00_002e0988");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void horny_toad_update() __asm__("FUN_L00_002d4610");
void hud_counter_update() __asm__("FUN_L00_002371e0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void linked_moby_states_update() __asm__("FUN_L00_002b3458");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
void morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void particle_spawner_smoke_fire_update() __asm__("FUN_L00_002d6f28");
void phase_effect_controller_update() __asm__("FUN_L00_002e3800");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void process_suck_cannon_suction_and_update_moby_state() __asm__("FUN_L00_002d56e0");
void pterodactyl_update() __asm__("FUN_L00_002e3a88");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void red_orb_update() __asm__("FUN_L00_002df4f8");
void red_plant_update() __asm__("FUN_L00_002e3ca8");
void ryno_rocket_update() __asm__("FUN_L00_002cd1f0");
void ryno_update() __asm__("FUN_L00_002cc608");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void smoke_fire_controller_update() __asm__("FUN_L00_002a4cd8");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void thruster_pack_flame_update() __asm__("FUN_L00_002b4198");
void trailing_flame_update() __asm__("FUN_L00_002bb400");
void trespasser_update() __asm__("FUN_L00_002c1718");
void tumbling_part_update() __asm__("FUN_L00_002e3d88");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_sound_emitter_playback_at_random_position() __asm__("FUN_L00_002edad8");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void veldin_robot_enemy_update() __asm__("FUN_L00_002e0b88");
void veldin_ship_update() __asm__("FUN_L00_002d1e80");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_missile_update() __asm__("FUN_L00_002b6140");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L00_001EA7BC")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            hud_counter_update};

void *D_L00_001EA7D4[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            hud_counter_update};

void *D_L00_001EA7EC[6]

    __attribute__((section("lvl.vtbl"))) = {process_suck_cannon_suction_and_update_moby_state,
                                            process_suck_cannon_suction_and_update_moby_state,
                                            process_suck_cannon_suction_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_9,
                                            get_moby_private_data_d0_address,
                                            hud_counter_update};

struct MobyClassUpdate moby_classes[101] __asm__("D_L00_001EA300")

    __attribute__((section("lvl.vtbl"))) = {
        { 809,       smoke_fire_controller_update, default_moby_methods},
        {1007,               camera_object_update, default_moby_methods},
        {  11,                      vendor_update, default_moby_methods},
        {  13,                        bolt_update, default_moby_methods},
        {  14,                        bolt_update, default_moby_methods},
        {  15,                        bolt_update, default_moby_methods},
        {  16,                        bolt_update, default_moby_methods},
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
        { 270,           morphoray_chicken_update,       D_L00_001EA7D4},
        { 305,                 homing_shot_update, default_moby_methods},
        { 348,     nanotech_crate_fragment_update, default_moby_methods},
        { 349,     nanotech_crate_fragment_update, default_moby_methods},
        { 354,     nanotech_crate_fragment_update, default_moby_methods},
        { 355,     nanotech_crate_fragment_update, default_moby_methods},
        { 356,     nanotech_crate_fragment_update, default_moby_methods},
        { 357,     nanotech_crate_fragment_update, default_moby_methods},
        { 358,     nanotech_crate_fragment_update, default_moby_methods},
        { 359,     nanotech_crate_fragment_update, default_moby_methods},
        { 428,         damped_falling_moby_update, default_moby_methods},
        { 433,                 noop_main_callback, default_moby_methods},
        { 454,                        ryno_update, default_moby_methods},
        { 457,                 ryno_rocket_update, default_moby_methods},
        { 479,                       drone_update, default_moby_methods},
        { 483,                                  0, default_moby_methods},
        { 500,                       crate_update, default_moby_methods},
        { 501,                       crate_update, default_moby_methods},
        { 511,                       crate_update, default_moby_methods},
        { 530,                 veldin_ship_update, default_moby_methods},
        { 562,                 decoy_glove_update, default_moby_methods},
        { 585,              metal_detector_update, default_moby_methods},
        { 607,                    helipack_update, default_moby_methods},
        { 618,                 noop_main_callback, default_moby_methods},
        { 634,                                  0, default_moby_methods},
        { 639,             explosion_light_update, default_moby_methods},
        { 749,                  horny_toad_update,       D_L00_001EA7EC},
        { 750,                     infobot_update, default_moby_methods},
        { 760, particle_spawner_smoke_fire_update, default_moby_methods},
        { 775,           grow_and_fade_out_update, default_moby_methods},
        { 804,     morphoray_target_effect_update, default_moby_methods},
        { 806,               nanotech_ball_update, default_moby_methods},
        { 834,      clank_cutscene_trigger_update, default_moby_methods},
        { 849,                  suckcannon_update, default_moby_methods},
        {1006,             devestator_ammo_update, default_moby_methods},
        {1060,                     red_orb_update, default_moby_methods},
        {1138,                   quad_moby_update, default_moby_methods},
        {1143,                  vendor_orb_update, default_moby_methods},
        {1192,               glow_particle_update, default_moby_methods},
        {1249,                 noop_main_callback, default_moby_methods},
        {1251,              hydrodisplacer_update, default_moby_methods},
        {1289,                 noop_main_callback, default_moby_methods},
        {1290,               pilots_helmet_update, default_moby_methods},
        {1413,                  help_hints_update, default_moby_methods},
        {1438,             devestator_ammo_update, default_moby_methods},
        {1440,          veldin_robot_enemy_update, default_moby_methods},
        {1447,             devestator_ammo_update, default_moby_methods},
        {1449,             devestator_ammo_update, default_moby_methods},
        {1471,        beam_slot_controller_update, default_moby_methods},
        {1545,     phase_effect_controller_update, default_moby_methods},
        {1564,                 pterodactyl_update, default_moby_methods},
        {1781,                   red_plant_update, default_moby_methods},
        {1782,                   red_plant_update, default_moby_methods},
        {1900,                       decoy_update, default_moby_methods},
        {1933,               tumbling_part_update, default_moby_methods},
        {1934,               tumbling_part_update, default_moby_methods},
        {1935,               tumbling_part_update, default_moby_methods},
        {1963,               tumbling_part_update, default_moby_methods},
        {  -1,                                  0, default_moby_methods},
};

struct CameraType level_camera_types[8] __asm__("D_L00_001EA880") __attribute__((section("lvl.camvtbl"))) = {
    { 0,noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                 },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                            },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                       },
    { 6,      noop_callback_l, initialize_orbit_camera_from_hero,   hud_counter_update,                          noop_main_callback},
    { 7,
     noop_callback_l, initialize_target_tracking_camera,
     update_target_tracking_camera,        conditionally_set_camera_state_three                                                    },
    {17,     FUN_L00_002ed230,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {23,  volume_camera_check,                noop_main_callback,   noop_main_callback,                          noop_main_callback},
    {-1,                    0,                                 0,                    0,                                           0},
};

struct SoundHandler sound_handlers[3] __asm__("D_L00_001EA980")

    __attribute__((section("lvl.sndvtbl"))) = {
        { 0,        update_sound_emitter_playback_by_distance},
        { 2, update_sound_emitter_playback_at_random_position},
        {-1,                                                0},
};
