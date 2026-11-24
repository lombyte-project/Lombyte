// Functions the boot and level programs call that the browser handles outside this build: the PS2
// renderer's vertex-lighting bakes, render-slot pose caching, screen fades and audio. Each call is
// counted and answered with 0 (the caller continues), and each entry says what is not done.
// Shared by browser/spike/run-closure.mjs (through scenario-veldin-level.mjs) and boot-runtime.mjs.
export const deferredFunctions = {
  FUN_00237370: 'light_ties: bakes directional-light vertex colours for the PS2 tie renderer (assembly-only)',
  FUN_0022a5e0: 'the same bake for shrub instances (assembly-only, unnamed)',
  FUN_L00_00251ce8: 'the level copy of executable 0x20ede8 (3,236 bytes of EE/VU0 assembly, unnamed): called as (moby, slot | 0x300) when an animation is set; it fills the moby\'s cached pose in that render slot. Not decoded, so no joint matrices exist for any animated model.',
  FUN_00234f98: 'light_tfrags: the same bake for terrain fragments (assembly-only)',
  FUN_001f4a58: 'fade_to_black: screen fade (renderer)',
  FUN_L00_0028dbe8: 'audio: the level copy of allocate_voice_for_target_entry (0x22da68), called by advance_moby_animation when an animation trigger is crossed; starts the trigger\'s sound (no browser audio backend; the result is not read)',
  FUN_L00_0024ec80: 'audio: the level copy of update_moby_voice (0x20c940), called by advance_moby_animation for a moby with a voice; updates its looping sound (no browser audio backend)',
  FUN_L00_001f2868: 'collect_mobies_in_sphere (about 2,300 bytes of EE/VU0 assembly from 0x1f2868, not decoded): NOT output-only. It searches the moby collision grid, whose entries update_entity (FUN_L00_00250df8, below) has not built, and is answered as 0 mobys found. Wherever a moby is in range, callers take the nothing-found branch the console would not.',
};
