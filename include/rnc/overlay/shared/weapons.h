#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_H

#include "rnc/overlay/shared/math/quad.h"
#include "rnc/overlay/shared/weapons/mine.h"
#include "rnc/sdk/eetypes.h"
#include "types.h"

struct Moby;

void blaster_update(struct Moby *) __asm__("FUN_L00_002b49a8");
void homing_shot_update() __asm__("FUN_L00_002cb418");
void bomb_glove_update(struct Moby *) __asm__("FUN_L00_002c26c8");
void decoy_update(char *) __asm__("FUN_L00_002c4380");
void release_moby_with_collision_check(float *, char *, void *) __asm__("FUN_L00_002c3888");
void decoy_glove_update(unsigned char *) __asm__("FUN_L00_002d2670");
void doom_bot_update(struct Moby *) __asm__("FUN_L00_002bffa8");
unsigned char *spawn_doom_bot(int, u128 *, u128 *) __asm__("FUN_L00_002bfe08");
void           drone_update(struct Moby *) __asm__("FUN_L00_002ce358");
void           glove_of_doom_update(struct Moby *) __asm__("FUN_L00_002c7a58");
void           spawn_moby_collision_particles(unsigned char *) __asm__("FUN_L00_002c8078");
void           doom_bot_spawner_update(struct Moby *) __asm__("FUN_L00_002c89e8");
void           gold_devastator_missile_update(M910 *) __asm__("FUN_L00_002b03a0");
void           helipack_update(char *) __asm__("FUN_L00_002d37d0");
void           linked_moby_states_update(unsigned char *) __asm__("FUN_L00_002b3458");
void           metal_detector_update(struct Moby *) __asm__("FUN_L00_002d2ee8");
void           mine_glove_update(struct Moby *) __asm__("FUN_L00_002c1ad0");
void           morphoray_update() __asm__("FUN_L00_002bc7e8");
s32            FUN_L00_002caf40(s32);
int  cancel_suck_cannon_suction_and_set_moby_state_5(struct Moby *) __asm__("FUN_L00_002caf10");
int  launch_suck_cannon_projectile_and_update_moby_state(struct Moby *,
                                                         int,
                                                         int,
                                                         float) __asm__("FUN_L00_002cae60");
int  moby_set_state_from_check(struct Moby *) __asm__("FUN_L00_002cadc0");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
struct Moby *spawn_morphoray_chicken(void *) __asm__("FUN_L00_002c9348");
void         damped_falling_moby_update(unsigned char *) __asm__("FUN_L00_002cc210");
void         morphoray_target_effect_update() __asm__("FUN_L00_002d7f88");
void         pyrocitor_update(struct Moby *) __asm__("FUN_L00_002b77f0");
void         ryno_update(struct Moby *) __asm__("FUN_L00_002cc608");
void         ryno_rocket_update(char *) __asm__("FUN_L00_002cd1f0");
void         swingshot_update() __asm__("FUN_L00_002c6158");
void         taunter_update() __asm__("FUN_L00_002b6f10");
void         tesla_claw_update(struct Moby *) __asm__("FUN_L00_002b87e0");
void         thruster_pack_flame_update(void *) __asm__("FUN_L00_002b4198");
void         trailing_flame_update(struct Moby *) __asm__("FUN_L00_002bb400");
void         trespasser_update(struct Moby *) __asm__("FUN_L00_002c1718");
void         turret_rocket_update(char *) __asm__("FUN_L08_002e8210");
char        *spawn_turret_rocket(int, float *, float *, int) __asm__("FUN_L08_002e7eb0");
void         turret_shot_update(struct Moby *) __asm__("FUN_L09_00307d68");
struct Moby *spawn_turret_shot(void *, OvlVec4 *, OvlVec4 *, s32) __asm__("FUN_L09_00307ba8");
void         visibomb_update() __asm__("FUN_L00_002b3058");
void         visibomb_missile_update(struct Moby *) __asm__("FUN_L00_002b6140");
s32          spawn_visibomb_missile(void *, void *, f32, f32) __asm__("FUN_L00_002b58d8");
void         terminate_visibomb_missile(void *, char **) __asm__("FUN_L00_002b5b20");
void         walloper_update(char *) __asm__("FUN_L00_002bb558");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_H */
