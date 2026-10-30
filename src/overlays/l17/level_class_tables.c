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
void FUN_L00_002cd1f0();
void FUN_L00_002ea7a0();
void FUN_L00_002ed230();
void FUN_L10_002cc2e8();
void update_ship_rocket() __asm__("FUN_L11_003094f8");
void FUN_L17_002e87d8();
void ammocrate_update() __asm__("FUN_L13_002e1cc8");
void blarg_ship_update() __asm__("FUN_L17_002f40d8");
void blaster_update() __asm__("FUN_L00_002b49a8");
void bolt_update() __asm__("FUN_L00_002a5dd8");
void bomb_glove_update() __asm__("FUN_L00_002c26c8");
void bomb_update() __asm__("FUN_L00_002adb30");
void camera_object_update() __asm__("FUN_L00_002a4e48");
void camera_transition_update() __asm__("FUN_L00_002ea488");
void cancel_suck_cannon_suction_and_set_moby_state_5() __asm__("FUN_L00_002caf10");
void cancel_suck_cannon_suction_and_set_state_12() __asm__("FUN_L17_002f0040");
void codebot_update() __asm__("FUN_L15_002ebd78");
void conditionally_set_camera_state_three() __asm__("FUN_L00_002ec7b0");
void crate_update() __asm__("FUN_L00_002cf218");
void debris_update() __asm__("FUN_L00_002af5b8");
void decoy_glove_update() __asm__("FUN_L00_002d2670");
void decoy_update() __asm__("FUN_L00_002c4380");
void devastator_update() __asm__("FUN_L00_002b2100");
void devestator_ammo_update() __asm__("FUN_L00_002c53c0");
void doom_bot_update() __asm__("FUN_L00_002bffa8");
void underwater_door_update() __asm__("FUN_L15_002bddb0");
void dreks_fleet_turret_update() __asm__("FUN_L17_002cb310");
void electric_water_update() __asm__("FUN_L15_002d6810");
void enemy_stun_update() __asm__("FUN_L17_002efea8");
void energy_sphere_holder_update() __asm__("FUN_L06_002f7288");
void energy_sphere_update() __asm__("FUN_L06_002f7ab8");
void fighter_jet_update() __asm__("FUN_L17_002ed018");
void floor_button_update() __asm__("FUN_L05_0030c5b0");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void follow_camera_update() __asm__("FUN_L00_002e9c28");
void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void glove_of_doom_update() __asm__("FUN_L00_002c7a58");
void gold_bolt_update() __asm__("FUN_L01_00307ca0");
void gold_devastator_missile_update() __asm__("FUN_L00_002b03a0");
void grow_and_fade_out_update() __asm__("FUN_L00_002d7f38");
void helipack_update() __asm__("FUN_L00_002d37d0");
void hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void infobot_update() __asm__("FUN_L00_002d5de8");
void initialize_camera_pose_springs() __asm__("FUN_L00_002ead60");
void initialize_camera_transform_and_paths() __asm__("FUN_L01_00315de0");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void initialize_target_tracking_camera() __asm__("FUN_L00_002ebbd8");
void launch_suck_cannon_projectile_and_update_moby_state() __asm__("FUN_L00_002cae60");
void magnet_elevator_update() __asm__("FUN_L17_002ee2b0");
void metal_detector_update() __asm__("FUN_L00_002d2ee8");
void mine_blend_color() __asm__("FUN_L00_002371e0");
void mine_glove_update() __asm__("FUN_L00_002c1ad0");
void mine_update() __asm__("FUN_L00_002aa670");
void moby_set_state_from_check() __asm__("FUN_L00_002cadc0");
void moby_update_draw_only() __asm__("FUN_L16_002e9df0");
void morphoray_update() __asm__("FUN_L00_002bc7e8");
void nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
void noop_callback_l() __asm__("FUN_001e9448");
void noop_main_callback() __asm__("FUN_001e93e8");
void pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void pyrocitor_update() __asm__("FUN_L00_002b77f0");
void quad_moby_update() __asm__("FUN_0023a2c0");
void ripple_strips_moby_update() __asm__("FUN_L15_002eb0a0");
void robot_arm_update() __asm__("FUN_L00_002e3d88");
void robot_dog_update() __asm__("FUN_L17_002eeb68");
void update_blarg_elite_commando_rocket() __asm__("FUN_L15_0029ac30");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void sentrybot_alarm_update() __asm__("FUN_L15_002cb4c8");
void sentrybot_update() __asm__("FUN_L15_002979d8");
void shuttle_update() __asm__("FUN_L17_002f1940");
void spawn_point_update() __asm__("FUN_L01_00300220");
void spiderbot_update() __asm__("FUN_L15_002a6e68");
void suckcannon_update() __asm__("FUN_L00_002da790");
void swingshot_hook_update() __asm__("FUN_L00_002c7070");
void swingshot_update() __asm__("FUN_L00_002c6158");
void swingshot_versatarget_update() __asm__("FUN_L02_002e0450");
void taunter_update() __asm__("FUN_L00_002b6f10");
void teleporter_update() __asm__("FUN_L01_00308bd8");
void tesla_claw_update() __asm__("FUN_L00_002b87e0");
void trespasser_update() __asm__("FUN_L00_002c1718");
void underwater_ring_update() __asm__("FUN_L15_002e5958");
void update_animated_point_light_moby() __asm__("FUN_L00_002d3a10");
void update_thruster_pack_flame() __asm__("FUN_L00_002b4198");
void update_breakaway_prop() __asm__("FUN_L17_002f5390");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_pose_with_optional_collision() __asm__("FUN_L00_002eb218");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");
void update_cutscene_camera_target() __asm__("FUN_L17_002f2e78");
void update_damped_falling_moby() __asm__("FUN_L00_002cc210");
void update_drone() __asm__("FUN_L00_002ce358");
void update_drone_flight_and_sparks() __asm__("FUN_L17_002e8e08");
void update_fleet_backdrop_animation() __asm__("FUN_L17_002f3848");
void update_fleet_beam() __asm__("FUN_L17_002a8da0");
void update_fleet_camera_box_watcher() __asm__("FUN_L17_002f26d0");
void update_fleet_rotating_radial_effect() __asm__("FUN_L17_002d77f0");
void update_floating_mine() __asm__("FUN_L17_002dc3f8");
void update_glow_particle() __asm__("FUN_L00_002acad8");
void update_hero_aligned_camera_from_input() __asm__("FUN_L00_002e9f38");
void update_hero_flag_from_moby_local_box() __asm__("FUN_L10_002e9028");
void update_hero_seeking_group_moby() __asm__("FUN_L15_002a2488");
void update_fighter_jet_missile() __asm__("FUN_L11_003089d0");
void update_homing_shot() __asm__("FUN_L00_002cb418");
void update_linked_moby_scale_and_rotation() __asm__("FUN_L00_002bb400");
void update_linked_moby_states() __asm__("FUN_L00_002b3458");
void update_linked_moby_transform_and_lifetime() __asm__("FUN_L00_002d7f88");
void update_listener_emitter() __asm__("FUN_L01_0031a078");
void update_smoke_trail_projectile() __asm__("FUN_L01_002eda00");
void update_billboard_projectile() __asm__("FUN_L01_002c7530");
void update_moby_rotation_group() __asm__("FUN_L06_002f4f00");
void update_moby_spatial_trigger() __asm__("FUN_L05_002f5200");
void update_morphoray_chicken() __asm__("FUN_L00_002c97e0");
void update_nearest_area_moby_selection() __asm__("FUN_L01_002f2eb8");
void update_blarg_ship_shot() __asm__("FUN_L11_00309098");
void update_shattering_prop() __asm__("FUN_L17_002f5260");
void update_fighter_jet_rocket() __asm__("FUN_L13_002e6c08");
void update_linked_spark_emitter() __asm__("FUN_L15_002a2bf0");
void update_sentrybot_projectile() __asm__("FUN_L15_002e4380");
void update_target_tracking_camera() __asm__("FUN_L00_002ec6c0");
void update_electric_beam() __asm__("FUN_L14_003015d0");
void vendor_orb_update() __asm__("FUN_L00_002df730");
void vendor_update() __asm__("FUN_L00_002a57a8");
void visibomb_update() __asm__("FUN_L00_002b3058");
void walloper_update() __asm__("FUN_L00_002bb558");
void wrench_update() __asm__("FUN_L00_002a84c8");

void *default_moby_methods[6] __asm__("D_L17_001E7824")

    __attribute__((section("lvl.vtbl"))) = {noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            noop_callback_l,
                                            mine_blend_color};

void *D_L17_001E783C[6]

    __attribute__((section("lvl.vtbl"))) = {moby_set_state_from_check,
                                            moby_set_state_from_check,
                                            launch_suck_cannon_projectile_and_update_moby_state,
                                            cancel_suck_cannon_suction_and_set_moby_state_5,
                                            FUN_L00_002caf40,
                                            mine_blend_color};

void *D_L17_001E7854[6]

    __attribute__((section("lvl.vtbl"))) = {enemy_stun_update,
                                            enemy_stun_update,
                                            enemy_stun_update,
                                            cancel_suck_cannon_suction_and_set_state_12,
                                            get_moby_data_plus_0x60,
                                            mine_blend_color};

struct MobyClassUpdate moby_classes[163] __asm__("D_L17_001E7080")

    __attribute__((section("lvl.vtbl"))) = {
        {0x3ef,                      camera_object_update, default_moby_methods},
        {  0xb,                             vendor_update, default_moby_methods},
        {  0xd,                               bolt_update, default_moby_methods},
        {  0xe,                               bolt_update, default_moby_methods},
        {  0xf,                               bolt_update, default_moby_methods},
        { 0x10,                               bolt_update, default_moby_methods},
        { 0x2c,                          sentrybot_update, default_moby_methods},
        { 0x31,        update_blarg_elite_commando_rocket, default_moby_methods},
        { 0x47,                             wrench_update, default_moby_methods},
        { 0x4a,                               mine_update, default_moby_methods},
        { 0x4d,            update_hero_seeking_group_moby, default_moby_methods},
        { 0x4e,               update_linked_spark_emitter, default_moby_methods},
        { 0x63,                         update_fleet_beam, default_moby_methods},
        { 0x70,                      update_glow_particle, default_moby_methods},
        { 0x79,                               bomb_update, default_moby_methods},
        { 0x7a,                             debris_update, default_moby_methods},
        { 0x7b,                          spiderbot_update, default_moby_methods},
        { 0x95,            nanotech_crate_fragment_update, default_moby_methods},
        { 0x99,            gold_devastator_missile_update, default_moby_methods},
        { 0x9c,               update_billboard_projectile, default_moby_methods},
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
        { 0xc5,                    underwater_door_update, default_moby_methods},
        { 0xcb,                              decoy_update, default_moby_methods},
        { 0xcc,                    devestator_ammo_update, default_moby_methods},
        { 0xd0,                          swingshot_update, default_moby_methods},
        { 0xd1,                     swingshot_hook_update, default_moby_methods},
        { 0xd5,                    devestator_ammo_update, default_moby_methods},
        { 0xd6,                    devestator_ammo_update, default_moby_methods},
        { 0xde,                    devestator_ammo_update, default_moby_methods},
        { 0xdf,                    devestator_ammo_update, default_moby_methods},
        { 0xe0,                          ammocrate_update, default_moby_methods},
        { 0xe1,                    devestator_ammo_update, default_moby_methods},
        { 0xe2,                    devestator_ammo_update, default_moby_methods},
        { 0xe4,                          ammocrate_update, default_moby_methods},
        { 0xe5,                      glove_of_doom_update, default_moby_methods},
        { 0xe6,                          FUN_L00_002c89e8, default_moby_methods},
        {0x102,               update_moby_spatial_trigger, default_moby_methods},
        {0x10e,                  update_morphoray_chicken,       D_L17_001E783C},
        {0x127,                 update_fighter_jet_rocket, default_moby_methods},
        {0x131,                        update_homing_shot, default_moby_methods},
        {0x15b,                 dreks_fleet_turret_update, default_moby_methods},
        {0x15c,            nanotech_crate_fragment_update, default_moby_methods},
        {0x15d,            nanotech_crate_fragment_update, default_moby_methods},
        {0x162,            nanotech_crate_fragment_update, default_moby_methods},
        {0x163,            nanotech_crate_fragment_update, default_moby_methods},
        {0x164,            nanotech_crate_fragment_update, default_moby_methods},
        {0x165,            nanotech_crate_fragment_update, default_moby_methods},
        {0x166,            nanotech_crate_fragment_update, default_moby_methods},
        {0x167,            nanotech_crate_fragment_update, default_moby_methods},
        {0x16b,            nanotech_crate_fragment_update, default_moby_methods},
        {0x16c,            nanotech_crate_fragment_update, default_moby_methods},
        {0x198,                    sentrybot_alarm_update, default_moby_methods},
        {0x1ac,                update_damped_falling_moby, default_moby_methods},
        {0x1b1,                        noop_main_callback, default_moby_methods},
        {0x1c6,                          FUN_L00_002cc608, default_moby_methods},
        {0x1c9,                          FUN_L00_002cd1f0, default_moby_methods},
        {0x1df,                              update_drone, default_moby_methods},
        {0x1e3,                                         0, default_moby_methods},
        {0x1f4,                              crate_update, default_moby_methods},
        {0x1f5,                              crate_update, default_moby_methods},
        {0x1f9,                              crate_update, default_moby_methods},
        {0x1ff,                              crate_update, default_moby_methods},
        {0x232,                        decoy_glove_update, default_moby_methods},
        {0x237,             update_smoke_trail_projectile, default_moby_methods},
        {0x249,                     metal_detector_update, default_moby_methods},
        {0x25d,        update_nearest_area_moby_selection, default_moby_methods},
        {0x25f,                           helipack_update, default_moby_methods},
        {0x26a,                        noop_main_callback, default_moby_methods},
        {0x27a,                                         0, default_moby_methods},
        {0x27f,          update_animated_point_light_moby, default_moby_methods},
        {0x28f,                     electric_water_update, default_moby_methods},
        {0x29d,       update_fleet_rotating_radial_effect, default_moby_methods},
        {0x2ee,                            infobot_update, default_moby_methods},
        {0x2f6,              swingshot_versatarget_update, default_moby_methods},
        {0x307,                  grow_and_fade_out_update, default_moby_methods},
        {0x324, update_linked_moby_transform_and_lifetime, default_moby_methods},
        {0x325,                        spawn_point_update, default_moby_methods},
        {0x326,                      nanotech_ball_update, default_moby_methods},
        {0x333,                          FUN_L10_002cc2e8, default_moby_methods},
        {0x33e,                       floor_button_update, default_moby_methods},
        {0x343,                      update_floating_mine, default_moby_methods},
        {0x351,                         suckcannon_update, default_moby_methods},
        {0x3a6,               update_sentrybot_projectile, default_moby_methods},
        {0x3ee,                    devestator_ammo_update, default_moby_methods},
        {0x3f1,                update_fighter_jet_missile, default_moby_methods},
        {0x3f9,                    update_blarg_ship_shot, default_moby_methods},
        {0x3fd,                update_moby_rotation_group, default_moby_methods},
        {0x40a,                        update_ship_rocket, default_moby_methods},
        {0x40e,               energy_sphere_holder_update, default_moby_methods},
        {0x410,                      energy_sphere_update, default_moby_methods},
        {0x46e,                          gold_bolt_update, default_moby_methods},
        {0x46f,                         teleporter_update, default_moby_methods},
        {0x472,                          quad_moby_update, default_moby_methods},
        {0x477,                         vendor_orb_update, default_moby_methods},
        {0x4a8,                      update_glow_particle, default_moby_methods},
        {0x4b9,                    underwater_ring_update, default_moby_methods},
        {0x4c8,                      update_electric_beam, default_moby_methods},
        {0x4e3,                     hydrodisplacer_update, default_moby_methods},
        {0x509,                        noop_main_callback, default_moby_methods},
        {0x50a,                      pilots_helmet_update, default_moby_methods},
        {0x54f,                          FUN_L17_002e87d8, default_moby_methods},
        {0x550,                          FUN_L17_002e87d8, default_moby_methods},
        {0x551,                          FUN_L17_002e87d8, default_moby_methods},
        {0x552,                          FUN_L17_002e87d8, default_moby_methods},
        {0x553,                          FUN_L17_002e87d8, default_moby_methods},
        {0x554,                          FUN_L17_002e87d8, default_moby_methods},
        {0x557,                          FUN_L17_002e87d8, default_moby_methods},
        {0x558,            update_drone_flight_and_sparks, default_moby_methods},
        {0x559,                          FUN_L17_002e87d8, default_moby_methods},
        {0x55c,                          FUN_L17_002e87d8, default_moby_methods},
        {0x55d,                          FUN_L17_002e87d8, default_moby_methods},
        {0x562,      update_hero_flag_from_moby_local_box, default_moby_methods},
        {0x563,                        fighter_jet_update, default_moby_methods},
        {0x564,                    magnet_elevator_update, default_moby_methods},
        {0x566,                          robot_dog_update,       D_L17_001E7854},
        {0x57d,                 ripple_strips_moby_update, default_moby_methods},
        {0x57e,                 ripple_strips_moby_update, default_moby_methods},
        {0x57f,                 ripple_strips_moby_update, default_moby_methods},
        {0x592,                 ripple_strips_moby_update, default_moby_methods},
        {0x594,                            codebot_update, default_moby_methods},
        {0x59e,                    devestator_ammo_update, default_moby_methods},
        {0x5a7,                    devestator_ammo_update, default_moby_methods},
        {0x5a8,                            shuttle_update, default_moby_methods},
        {0x5a9,                    devestator_ammo_update, default_moby_methods},
        {0x5be,           update_fleet_camera_box_watcher, default_moby_methods},
        {0x61a,             update_cutscene_camera_target, default_moby_methods},
        {0x63a,                          robot_arm_update, default_moby_methods},
        {0x63b,                          robot_arm_update, default_moby_methods},
        {0x6ce,                          robot_arm_update, default_moby_methods},
        {0x6cf,                          robot_arm_update, default_moby_methods},
        {0x6ec,           update_fleet_backdrop_animation, default_moby_methods},
        {0x733,                         blarg_ship_update, default_moby_methods},
        {0x751,                    update_shattering_prop, default_moby_methods},
        {0x752,                        noop_main_callback, default_moby_methods},
        {0x753,                          robot_arm_update, default_moby_methods},
        {0x754,                     update_breakaway_prop, default_moby_methods},
        {0x755,                        noop_main_callback, default_moby_methods},
        {0x756,                          robot_arm_update, default_moby_methods},
        {0x76c,                              decoy_update, default_moby_methods},
        {0x773,                     moby_update_draw_only, default_moby_methods},
        {0x774,                     moby_update_draw_only, default_moby_methods},
        {0x776,                     moby_update_draw_only, default_moby_methods},
        {0x777,                     moby_update_draw_only, default_moby_methods},
        {0x779,                     moby_update_draw_only, default_moby_methods},
        {0x77d,                          robot_arm_update, default_moby_methods},
        {0x77e,                     moby_update_draw_only, default_moby_methods},
        {0x7a6,                    underwater_door_update, default_moby_methods},
        {   -1,                                         0, default_moby_methods},
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
    { 6,      noop_callback_l,     initialize_orbit_camera_from_hero,     mine_blend_color,                          noop_main_callback},
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
