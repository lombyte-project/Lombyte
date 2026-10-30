/* The level's moby classes, camera types and sound emitter types: the tables
   the executable's code looks ids up in (sections lvl.vtbl, lvl.camvtbl and
   lvl.sndvtbl). */
#include "types.h"
#include "rnc/audio/sound_handlers.h"
#include "rnc/camera/update_cam.h"
#include "rnc/mobys/moby_class_tables.h"

void FUN_L00_002b6140();
void FUN_L00_002c89e8();
void FUN_L00_002caf40();
void FUN_L00_002cc608();
void FUN_L00_002ea7a0();
void FUN_L00_002ed230();
void FUN_L01_002b9eb0();
void FUN_L01_002f8718();
void FUN_L06_002ea6a8();
void FUN_L10_002be858();
void FUN_L10_002cc2e8();
void FUN_L10_002d92b8();
void FUN_L10_002f5f18();
void background_ship_2_update() __asm__("FUN_L02_002ece18");
void blarg_ship_update() __asm__("FUN_L10_002e4a88");
void blarg_space_commando_update() __asm__("FUN_L06_002fdbd0");
void blaster_update() __asm__("FUN_L00_002b49a8");
void block_cave_update() __asm__("FUN_L10_002dd650");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void bombthrower_bomb_update() __asm__("FUN_L10_002d85c8");
void bombthrower_grass_update() __asm__("FUN_L10_002d8c00");
void bot_door_lock_update() __asm__("FUN_L06_00307d30");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_moby_suction_and_save_state() __asm__("FUN_L06_002ea7c8");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void darter_update() __asm__("FUN_L10_002e01a8");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void energy_sphere_holder_update() __asm__("FUN_L06_002f7288");
void energy_sphere_update() __asm__("FUN_L06_002f7ab8");
void evaluate_descriptor_eligibility() __asm__("FUN_L10_002f65f8");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void fred_1_update() __asm__("FUN_L03_002dc310");
void frog_update() __asm__("FUN_L06_002e8678");
void gadgebot_spawn_container_update() __asm__("FUN_L06_002d85a0");
void gadgebot_spawn_pad_update() __asm__("FUN_L06_002d8c20");
void gadgebot_update() __asm__("FUN_L06_002f0040");
void gate_1_update() __asm__("FUN_L10_002e9648");
void gate_2_update() __asm__("FUN_L10_002e9990");
void gate_update() __asm__("FUN_L10_002befe8");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void gun_update() __asm__("FUN_L10_002e8ab0");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_path_camera() __asm__("FUN_L10_002f5b58");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void lab_door_update() __asm__("FUN_L10_002d90a8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void magnetboots_update() __asm__("FUN_L10_00298668");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void nanotech_upgrade_vendor_update() __asm__("FUN_L10_002e8358");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pterodactyl_update() __asm__("FUN_L00_002e3a88");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void screamer_update() __asm__("FUN_L10_002df270");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void spawn_point_update() __asm__("FUN_L01_00300220");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void toxic_crab_update() __asm__("FUN_L10_002e1d38");
void trespasser_update() __asm__("FUN_L00_002c1718");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_breakable_prop() __asm__("FUN_L10_002eb7e0");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_configurable_particle_emitter() __asm__("FUN_L10_002ea1f0");
void update_cutscene_camera_target() __asm__("FUN_L10_002eacd8");
void update_cutscene_trigger() __asm__("FUN_L01_002fb5b0");
void update_cycling_platform() __asm__("FUN_L10_00295a38");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_data_processor_linked_effect() __asm__("FUN_L10_002da2c8");
void update_destructible_object() __asm__("FUN_L10_002d9eb8");
void update_directional_particle_emitter() __asm__("FUN_L10_002e5be0");
void update_directional_spark_emitter() __asm__("FUN_L08_002f5830");
void update_drone() __asm__("FUN_L00_002ce358");
void update_falling_spinning_moby() __asm__("FUN_L06_00300b90");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_height_adjusted_projectile() __asm__("FUN_L10_002d7b80");
void update_help_hints() __asm__("FUN_L10_002e85b8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_hero_flag_from_moby_local_box() __asm__("FUN_L10_002e9028");
void update_hero_relocation_trigger() __asm__("FUN_L10_00298b68");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_jitter_strand_grid() __asm__("FUN_L10_002dcc58");
void update_linked_moby_hitbox() __asm__("FUN_L10_002c9c70");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_moby_grab_state() __asm__("FUN_L06_002ea5e0");
void update_moby_spatial_trigger() __asm__("FUN_L05_002f5200");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_path_spark_effect() __asm__("FUN_L10_002dd3d8");
void update_path_spark_emitter() __asm__("FUN_L10_002d8d30");
void update_reverb_box_emitter() __asm__("FUN_L01_00319f18");
void update_ryno_rocket() __asm__("FUN_L00_002cd1f0");
void update_shattering_prop() __asm__("FUN_L10_002eb6f0");
void update_sinking_platform() __asm__("FUN_L10_00295c20");
void update_sound_emitter_playback_by_distance() __asm__("FUN_L00_002ed950");
void update_target_outside_zone_timeout() __asm__("FUN_L01_00302648");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_timed_effect_emitter() __asm__("FUN_L10_002c7a20");
void update_turret_shot() __asm__("FUN_L09_00307d68");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void volume_camera_check() __asm__("FUN_L00_002ed8b0");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L10_001DDE28")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L10_001DDE40[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

void *D_L10_001DDE58[6] __attribute__((section("lvl.vtbl"))) = {update_moby_grab_state,
                                                                FUN_L06_002ea6a8,
                                                                FUN_L06_002ea6a8,
                                                                cancel_moby_suction_and_save_state,
                                                                get_moby_data_plus_0x60,
                                                                mine_blend_color};

struct MobyClassUpdate moby_classes[174] __asm__("D_L10_001DD600")

    __attribute__((section("lvl.vtbl"))) = {
        {0x3ef,                      camera_object_update, default_moby_methods},
        {0x409,                   update_cycling_platform, default_moby_methods},
        {0x45d,                   update_sinking_platform, default_moby_methods},
        {  0xb,                             vendor_update, default_moby_methods},
        {  0xd,                               bolt_update, default_moby_methods},
        {  0xe,                               bolt_update, default_moby_methods},
        {  0xf,                               bolt_update, default_moby_methods},
        { 0x10,                               bolt_update, default_moby_methods},
        { 0x12,                        magnetboots_update, default_moby_methods},
        { 0x16,            update_hero_relocation_trigger, default_moby_methods},
        { 0x47,                             wrench_update, default_moby_methods},
        { 0x4a,                               mine_update, default_moby_methods},
        { 0x57,                        noop_main_callback, default_moby_methods},
        { 0x70,                      update_glow_particle, default_moby_methods},
        { 0x79,                               bomb_update, default_moby_methods},
        { 0x7a,                             debris_update, default_moby_methods},
        { 0x95,            nanotech_crate_fragment_update, default_moby_methods},
        { 0x99,            gold_devastator_missile_update, default_moby_methods},
        { 0x9d,                         devastator_update, default_moby_methods},
        { 0xa3,                           visibomb_update, default_moby_methods},
        { 0xa6,                 update_linked_moby_states, default_moby_methods},
        { 0xa7,                update_thruster_pack_flame, default_moby_methods},
        { 0xa8,                            blaster_update, default_moby_methods},
        { 0xac,                          FUN_L00_002b6140, default_moby_methods},
        { 0xaf,                            taunter_update, default_moby_methods},
        { 0xb0,                          pyrocitor_update, default_moby_methods},
        { 0xb1,                         tesla_claw_update, default_moby_methods},
        { 0xb3,     update_linked_moby_scale_and_rotation, default_moby_methods},
        { 0xb4,                           walloper_update, default_moby_methods},
        { 0xb9,                          morphoray_update, default_moby_methods},
        { 0xba,                           doom_bot_update, default_moby_methods},
        { 0xbc,                         trespasser_update, default_moby_methods},
        { 0xbe,                         mine_glove_update, default_moby_methods},
        { 0xc0,                         bomb_glove_update, default_moby_methods},
        { 0xcb,                              decoy_update, default_moby_methods},
        { 0xcc,                    devestator_ammo_update, default_moby_methods},
        { 0xd0,                          swingshot_update, default_moby_methods},
        { 0xd1,                     swingshot_hook_update, default_moby_methods},
        { 0xd5,                    devestator_ammo_update, default_moby_methods},
        { 0xd6,                    devestator_ammo_update, default_moby_methods},
        { 0xde,                    devestator_ammo_update, default_moby_methods},
        { 0xdf,                    devestator_ammo_update, default_moby_methods},
        { 0xe1,                    devestator_ammo_update, default_moby_methods},
        { 0xe2,                    devestator_ammo_update, default_moby_methods},
        { 0xe5,                      glove_of_doom_update, default_moby_methods},
        { 0xe6,                          FUN_L00_002c89e8, default_moby_methods},
        {0x102,               update_moby_spatial_trigger, default_moby_methods},
        {0x10e,                  update_morphoray_chicken,       D_L10_001DDE40},
        {0x12e,           gadgebot_spawn_container_update, default_moby_methods},
        {0x12f,                 gadgebot_spawn_pad_update, default_moby_methods},
        {0x131,                        update_homing_shot, default_moby_methods},
        {0x15c,            nanotech_crate_fragment_update, default_moby_methods},
        {0x15d,            nanotech_crate_fragment_update, default_moby_methods},
        {0x15f,                          FUN_L10_002be858, default_moby_methods},
        {0x161,                               gate_update, default_moby_methods},
        {0x162,            nanotech_crate_fragment_update, default_moby_methods},
        {0x163,            nanotech_crate_fragment_update, default_moby_methods},
        {0x164,            nanotech_crate_fragment_update, default_moby_methods},
        {0x165,            nanotech_crate_fragment_update, default_moby_methods},
        {0x166,            nanotech_crate_fragment_update, default_moby_methods},
        {0x167,            nanotech_crate_fragment_update, default_moby_methods},
        {0x16b,            nanotech_crate_fragment_update, default_moby_methods},
        {0x16c,            nanotech_crate_fragment_update, default_moby_methods},
        {0x1ac,                update_damped_falling_moby, default_moby_methods},
        {0x1b1,                        noop_main_callback, default_moby_methods},
        {0x1c6,                          FUN_L00_002cc608, default_moby_methods},
        {0x1c9,                        update_ryno_rocket, default_moby_methods},
        {0x1df,                              update_drone, default_moby_methods},
        {0x1e3,                                         0, default_moby_methods},
        {0x1f4,                              crate_update, default_moby_methods},
        {0x1f5,                              crate_update, default_moby_methods},
        {0x1f6,                              crate_update, default_moby_methods},
        {0x1f9,                              crate_update, default_moby_methods},
        {0x1ff,                              crate_update, default_moby_methods},
        {0x214,                        noop_main_callback, default_moby_methods},
        {0x232,                        decoy_glove_update, default_moby_methods},
        {0x249,                     metal_detector_update, default_moby_methods},
        {0x25d,        update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                           helipack_update, default_moby_methods},
        {0x26a,                        noop_main_callback, default_moby_methods},
        {0x27a,                                         0, default_moby_methods},
        {0x27f,          update_animated_point_light_moby, default_moby_methods},
        {0x288,          update_directional_spark_emitter, default_moby_methods},
        {0x2be,               update_timed_effect_emitter, default_moby_methods},
        {0x2e1,                   update_cutscene_trigger, default_moby_methods},
        {0x2ee,                            infobot_update, default_moby_methods},
        {0x2f6,              swingshot_versatarget_update, default_moby_methods},
        {0x307,                  grow_and_fade_out_update, default_moby_methods},
        {0x31a,                 update_linked_moby_hitbox, default_moby_methods},
        {0x323,              swingshot_versatarget_update, default_moby_methods},
        {0x324, update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                        spawn_point_update, default_moby_methods},
        {0x326,                      nanotech_ball_update, default_moby_methods},
        {0x333,                          FUN_L10_002cc2e8, default_moby_methods},
        {0x33b,                               frog_update,       D_L10_001DDE58},
        {0x340,        update_target_outside_zone_timeout, default_moby_methods},
        {0x351,                         suckcannon_update, default_moby_methods},
        {0x359,                           gadgebot_update, default_moby_methods},
        {0x35b,         update_height_adjusted_projectile, default_moby_methods},
        {0x393,                             fred_1_update, default_moby_methods},
        {0x3aa,                   bombthrower_bomb_update, default_moby_methods},
        {0x3ab,                  bombthrower_grass_update, default_moby_methods},
        {0x3b3,                 update_path_spark_emitter, default_moby_methods},
        {0x3ee,                    devestator_ammo_update, default_moby_methods},
        {0x3f7,                           lab_door_update, default_moby_methods},
        {0x407,                          FUN_L10_002d92b8, default_moby_methods},
        {0x40e,               energy_sphere_holder_update, default_moby_methods},
        {0x410,                      energy_sphere_update, default_moby_methods},
        {0x417,                update_destructible_object, default_moby_methods},
        {0x42b,       update_data_processor_linked_effect, default_moby_methods},
        {0x42c,               blarg_space_commando_update, default_moby_methods},
        {0x431,                 update_jitter_strand_grid, default_moby_methods},
        {0x43d,              update_falling_spinning_moby, default_moby_methods},
        {0x43e,              update_falling_spinning_moby, default_moby_methods},
        {0x43f,              update_falling_spinning_moby, default_moby_methods},
        {0x440,              update_falling_spinning_moby, default_moby_methods},
        {0x441,              update_falling_spinning_moby, default_moby_methods},
        {0x442,                  update_path_spark_effect, default_moby_methods},
        {0x44c,                         block_cave_update, default_moby_methods},
        {0x462,                        noop_main_callback, default_moby_methods},
        {0x46e,                          gold_bolt_update, default_moby_methods},
        {0x472,                          quad_moby_update, default_moby_methods},
        {0x475,                          FUN_L01_002b9eb0, default_moby_methods},
        {0x477,                         vendor_orb_update, default_moby_methods},
        {0x4a8,                      update_glow_particle, default_moby_methods},
        {0x4ac,                           screamer_update, default_moby_methods},
        {0x4af,                             darter_update, default_moby_methods},
        {0x4b2,                         toxic_crab_update, default_moby_methods},
        {0x4cd,                         blarg_ship_update, default_moby_methods},
        {0x4d8,       update_directional_particle_emitter, default_moby_methods},
        {0x4e3,                     hydrodisplacer_update, default_moby_methods},
        {0x4ea,                        update_turret_shot, default_moby_methods},
        {0x502,                           lab_door_update, default_moby_methods},
        {0x509,                        noop_main_callback, default_moby_methods},
        {0x50a,                      pilots_helmet_update, default_moby_methods},
        {0x515,                          FUN_L10_002be858, default_moby_methods},
        {0x516,                      bot_door_lock_update, default_moby_methods},
        {0x52e,            nanotech_upgrade_vendor_update, default_moby_methods},
        {0x540,                         update_help_hints, default_moby_methods},
        {0x542,                                gun_update, default_moby_methods},
        {0x562,      update_hero_flag_from_moby_local_box, default_moby_methods},
        {0x58d,                             gate_1_update, default_moby_methods},
        {0x590,                             gate_2_update, default_moby_methods},
        {0x59e,                    devestator_ammo_update, default_moby_methods},
        {0x5a7,                    devestator_ammo_update, default_moby_methods},
        {0x5a9,                    devestator_ammo_update, default_moby_methods},
        {0x5f6,                          robot_arm_update, default_moby_methods},
        {0x5f8,                          robot_arm_update, default_moby_methods},
        {0x5f9,                          robot_arm_update, default_moby_methods},
        {0x5fd,                          robot_arm_update, default_moby_methods},
        {0x5fe,                          robot_arm_update, default_moby_methods},
        {0x601,                          robot_arm_update, default_moby_methods},
        {0x608,      update_configurable_particle_emitter, default_moby_methods},
        {0x613,             update_cutscene_camera_target, default_moby_methods},
        {0x61c,                        pterodactyl_update, default_moby_methods},
        {0x623,                          robot_arm_update, default_moby_methods},
        {0x624,                          robot_arm_update, default_moby_methods},
        {0x6a6,                          robot_arm_update, default_moby_methods},
        {0x6a7,                          robot_arm_update, default_moby_methods},
        {0x6a8,                          robot_arm_update, default_moby_methods},
        {0x70e,                          FUN_L01_002f8718, default_moby_methods},
        {0x70f,                          FUN_L01_002f8718, default_moby_methods},
        {0x73f,                    update_shattering_prop, default_moby_methods},
        {0x740,                     update_breakable_prop, default_moby_methods},
        {0x741,                        noop_main_callback, default_moby_methods},
        {0x742,                          robot_arm_update, default_moby_methods},
        {0x76c,                              decoy_update, default_moby_methods},
        {0x775,                          robot_arm_update, default_moby_methods},
        {0x77a,                          robot_arm_update, default_moby_methods},
        {0x77b,                          robot_arm_update, default_moby_methods},
        {0x77c,                          robot_arm_update, default_moby_methods},
        {0x781,                        noop_main_callback, default_moby_methods},
        {0x7b5,                  background_ship_2_update, default_moby_methods},
        {   -1,                                         0, default_moby_methods},
};

struct CameraType level_camera_types[9] __asm__("D_L10_001DDE80") __attribute__((section("lvl.camvtbl"))) = {
    { 0,     noop_callback_l,               follow_camera_setup,follow_camera_update,                          noop_main_callback                                                                        },
    { 4,
     camera_transition_update,                  FUN_L00_002ea7a0,
     update_hero_aligned_camera_from_input, select_camera_mode_by_hero_vector_alignment                                                   },
    { 5,
     noop_callback_l,    initialize_camera_pose_springs,
     update_camera_pose_with_optional_collision,                          noop_main_callback                                              },
    { 6,             noop_callback_l, initialize_orbit_camera_from_hero,     mine_blend_color,                          noop_main_callback},
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
