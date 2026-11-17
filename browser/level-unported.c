/* Functions the L00 class table (src/overlays/l00/level_class_tables.c) references
   that the browser has not ported yet. Calling one stops the module and records
   its symbol; port it by linking the original source and deleting its line here.
   81 of these have recovered C in src/; the rest are marked "no recovered C". */
extern void level_unported(const char *name);
#define UNPORTED(symbol, label) \
    void symbol(void) __asm__(#symbol); \
    void symbol(void) { level_unported(#symbol label); }
UNPORTED(FUN_001e93e8, " (no recovered C)")
UNPORTED(FUN_001e9448, " (no recovered C)")
UNPORTED(FUN_0023a2c0, " (src/mobys/quad_moby_update.c)")
UNPORTED(FUN_L00_002371e0, " (src/overlays/shared/ui/hud/hud_keep_visible.c)")
UNPORTED(FUN_L00_002a4cd8, " (src/overlays/shared/props/particle_spawner_smoke_fire_controller.c)")
UNPORTED(FUN_L00_002a4e48, " (src/overlays/shared/mobys/camera_object.c)")
UNPORTED(FUN_L00_002a57a8, " (src/overlays/shared/vendor/vendor.c)")
UNPORTED(FUN_L00_002a5dd8, " (src/overlays/shared/pickups/bolt.c)")
UNPORTED(FUN_L00_002a84c8, " (src/overlays/shared/weapons/wrench.c)")
UNPORTED(FUN_L00_002aa670, " (src/overlays/shared/weapons/mine.c)")
UNPORTED(FUN_L00_002acad8, " (src/overlays/shared/rendering/effects/glow_particle.c)")
UNPORTED(FUN_L00_002adb30, " (src/overlays/shared/weapons/bomb_glove_bomb.c)")
UNPORTED(FUN_L00_002af5b8, " (src/overlays/shared/rendering/effects/debris.c)")
UNPORTED(FUN_L00_002afa48, " (src/overlays/shared/pickups/crate_fragment.c)")
UNPORTED(FUN_L00_002b03a0, " (src/overlays/shared/weapons/gold_devastator_missile.c)")
UNPORTED(FUN_L00_002b2100, " (src/overlays/shared/weapons/devastator.c)")
UNPORTED(FUN_L00_002b3058, " (src/overlays/shared/weapons/visibomb.c)")
UNPORTED(FUN_L00_002b3458, " (src/overlays/shared/weapons/linked_moby_states_update.c)")
UNPORTED(FUN_L00_002b4198, " (src/overlays/shared/weapons/thruster_pack_flame.c)")
UNPORTED(FUN_L00_002b49a8, " (src/overlays/shared/weapons/blaster.c)")
UNPORTED(FUN_L00_002b6140, " (no recovered C)")
UNPORTED(FUN_L00_002b6f10, " (src/overlays/shared/weapons/taunter.c)")
UNPORTED(FUN_L00_002b77f0, " (src/overlays/shared/weapons/pyrocitor.c)")
UNPORTED(FUN_L00_002b87e0, " (src/overlays/shared/weapons/tesla_claw.c)")
UNPORTED(FUN_L00_002bb400, " (src/overlays/shared/weapons/trailing_flame.c)")
UNPORTED(FUN_L00_002bb558, " (src/overlays/shared/weapons/walloper.c)")
UNPORTED(FUN_L00_002bc7e8, " (src/overlays/shared/weapons/morphoray.c)")
UNPORTED(FUN_L00_002bffa8, " (no recovered C)")
UNPORTED(FUN_L00_002c1718, " (src/overlays/shared/weapons/trespasser.c)")
UNPORTED(FUN_L00_002c1ad0, " (src/overlays/shared/weapons/mine_glove.c)")
UNPORTED(FUN_L00_002c26c8, " (src/overlays/shared/weapons/bomb_glove.c)")
UNPORTED(FUN_L00_002c4380, " (src/overlays/shared/weapons/decoy.c)")
UNPORTED(FUN_L00_002c53c0, " (src/overlays/shared/pickups/weapon_ammo.c)")
UNPORTED(FUN_L00_002c6158, " (src/overlays/shared/weapons/swingshot.c)")
UNPORTED(FUN_L00_002c7070, " (src/overlays/shared/weapons/swingshot_hook.c)")
UNPORTED(FUN_L00_002c7a58, " (src/overlays/shared/weapons/glove_of_doom.c)")
UNPORTED(FUN_L00_002c89e8, " (no recovered C)")
UNPORTED(FUN_L00_002c97e0, " (src/overlays/shared/weapons/morphoray_chicken.c)")
UNPORTED(FUN_L00_002cadc0, " (src/overlays/shared/weapons/morphoray_chicken.c)")
UNPORTED(FUN_L00_002cae60, " (src/overlays/shared/weapons/morphoray_chicken.c)")
UNPORTED(FUN_L00_002caf10, " (src/overlays/shared/weapons/morphoray_chicken.c)")
UNPORTED(FUN_L00_002caf40, " (src/overlays/shared/weapons/morphoray_chicken.c)")
UNPORTED(FUN_L00_002cb418, " (src/overlays/shared/weapons/blaster_shot.c)")
UNPORTED(FUN_L00_002cc210, " (src/overlays/shared/weapons/morphoray_chicken_feather.c)")
UNPORTED(FUN_L00_002cc608, " (no recovered C)")
UNPORTED(FUN_L00_002cd1f0, " (src/overlays/shared/weapons/ryno_rocket.c)")
UNPORTED(FUN_L00_002ce358, " (src/overlays/shared/weapons/drone.c)")
UNPORTED(FUN_L00_002cf218, " (src/overlays/shared/pickups/crate.c)")
UNPORTED(FUN_L00_002d1e80, " (src/overlays/shared/vehicles/veldin_ship.c)")
UNPORTED(FUN_L00_002d2670, " (src/overlays/shared/weapons/decoy_glove.c)")
UNPORTED(FUN_L00_002d2ee8, " (no recovered C)")
UNPORTED(FUN_L00_002d37d0, " (src/overlays/shared/weapons/helipack.c)")
UNPORTED(FUN_L00_002d3a10, " (src/overlays/shared/rendering/effects/explosion_light.c)")
UNPORTED(FUN_L00_002d4610, " (src/overlays/shared/enemies/horny_toad.c)")
UNPORTED(FUN_L00_002d56e0, " (src/overlays/shared/enemies/horny_toad.c)")
UNPORTED(FUN_L00_002d57d0, " (src/overlays/shared/enemies/horny_toad.c)")
UNPORTED(FUN_L00_002d5800, " (src/overlays/shared/enemies/horny_toad.c)")
UNPORTED(FUN_L00_002d5de8, " (src/overlays/shared/pickups/infobot.c)")
UNPORTED(FUN_L00_002d6f28, " (no recovered C)")
UNPORTED(FUN_L00_002d7f38, " (src/overlays/shared/props/grow_and_fade_out.c)")
UNPORTED(FUN_L00_002d7f88, " (src/overlays/shared/weapons/morphoray_target_effect.c)")
UNPORTED(FUN_L00_002d89f0, " (src/overlays/shared/pickups/nanotech_ball.c)")
UNPORTED(FUN_L00_002d9dc8, " (src/overlays/l00/mobys/clank_cutscene_trigger.c)")
UNPORTED(FUN_L00_002da790, " (src/overlays/shared/weapons/suckcannon.c)")
UNPORTED(FUN_L00_002df4f8, " (src/overlays/shared/props/red_orb.c)")
UNPORTED(FUN_L00_002df730, " (src/overlays/shared/vendor/vendor_orb.c)")
UNPORTED(FUN_L00_002df9a0, " (src/overlays/shared/props/hydrodisplacer.c)")
UNPORTED(FUN_L00_002e03a0, " (src/overlays/shared/pickups/pilots_helmet.c)")
UNPORTED(FUN_L00_002e0988, " (src/overlays/l00/mobys/help_hints.c)")
UNPORTED(FUN_L00_002e0b88, " (src/overlays/l00/mobys/veldin_robot_enemy.c)")
UNPORTED(FUN_L00_002e1df0, " (src/overlays/l00/mobys/beam_slot_controller.c)")
UNPORTED(FUN_L00_002e3800, " (src/overlays/l00/mobys/cutscene_effects.c)")
UNPORTED(FUN_L00_002e3ca8, " (no recovered C)")
UNPORTED(FUN_L00_002e3d88, " (src/overlays/shared/props/tumbling_part.c)")
UNPORTED(FUN_L00_002e6d60, " (src/overlays/shared/camera/follow_camera_setup.c)")
UNPORTED(FUN_L00_002e9c28, " (src/overlays/shared/camera/follow_camera_update.c)")
UNPORTED(FUN_L00_002e9f38, " (src/overlays/shared/camera/hero_aligned_camera.c)")
UNPORTED(FUN_L00_002ea488, " (src/overlays/shared/camera/hero_aligned_camera.c)")
UNPORTED(FUN_L00_002ea7a0, " (src/overlays/shared/camera/hero_aligned_camera.c)")
UNPORTED(FUN_L00_002ea810, " (src/overlays/shared/camera/hero_aligned_camera.c)")
UNPORTED(FUN_L00_002ead60, " (src/overlays/shared/camera/scripted_camera.c)")
UNPORTED(FUN_L00_002eb218, " (src/overlays/shared/camera/scripted_camera.c)")
UNPORTED(FUN_L00_002ebb00, " (src/overlays/shared/camera/orbit_camera.c)")
UNPORTED(FUN_L00_002ebbd8, " (src/overlays/shared/camera/target_tracking_camera.c)")
UNPORTED(FUN_L00_002ec6c0, " (src/overlays/shared/camera/target_tracking_camera.c)")
UNPORTED(FUN_L00_002ec7b0, " (src/overlays/shared/camera/target_tracking_camera.c)")
UNPORTED(FUN_L00_002ed230, " (no recovered C)")
UNPORTED(FUN_L00_002ed8b0, " (src/overlays/shared/camera/aim_volume_camera.c)")
UNPORTED(FUN_L00_002ed950, " (src/overlays/shared/audio/emitters/distance_emitter.c)")
UNPORTED(FUN_L00_002edad8, " (src/overlays/shared/audio/emitters/random_position_emitter.c)")

/* Helpers that linked class updates call but this module has not ported.
   These keep the recovered signatures: under LTO a call through a different
   function type becomes a trap that records no name. */
struct Moby;
void mark_moby_for_removal_unported(struct Moby *moby) __asm__("FUN_0020c828");
void mark_moby_for_removal_unported(struct Moby *moby) {
    (void)moby;
    level_unported("FUN_0020c828 (src/mobys/mark_moby_for_removal.c; needs update_moby_grids)");
}
/* blend_moby_animation (src/mobys/animation/blend_moby_animation.c) is linked. Before it switches
   sequence it parks the current pose in a render slot so the blend starts from it: that needs
   0x20ede8 (553 instructions of EE/VU0 assembly, not decoded). The slot allocator answers "no
   slot", the branch the original takes when every slot is busy, so the blend switches without
   that snapshot. Each such answer is counted (runtime_level_blend_snapshots_skipped). */
static int blend_snapshots_skipped;
int runtime_level_blend_snapshots_skipped(void) { return blend_snapshots_skipped; }
int find_or_allocate_moby_render_slot_none(int moby) __asm__("func_0020CC18");
int find_or_allocate_moby_render_slot_none(int moby) {
    (void)moby;
    blend_snapshots_skipped++;
    return -1;
}
void park_moby_pose_unported(void *moby, int slot) __asm__("FUN_0020ede8");
void park_moby_pose_unported(void *moby, int slot) {
    (void)moby, (void)slot;
    level_unported("FUN_0020ede8 (assembly only: parks a moby pose in a render slot)");
}
