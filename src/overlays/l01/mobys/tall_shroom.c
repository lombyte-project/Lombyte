/* Level 01 moby class tall_shroom: waits for a hit, then bursts and is replaced by its placement moby. */
#include "types.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"

typedef struct {
    u8  pad0[0x2C];
    f32 f2c;
} TriggerHit;

typedef struct {
    u8 pad0[0x10];
    u8 pos[0x10];
    u8 state;
    u8 pad21[0x1F];
    u8 rot[0x10];
} TriggerMoby;

extern s32   D_L01_0015F580;
void        *find_moby_hit(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern void  spawn_proximity_gated_bolts() __asm__("FUN_L01_002787a0");
extern void *FUN_L00_00263fd8(void *, s32, void *, void *, s32, s32, void *, void *, f32, void *);
extern void  spawn_moby_from_placement() __asm__("FUN_L01_00278e20");

void tall_shroom_update(TriggerMoby *obj) __asm__("FUN_L01_0030d0f0");

void tall_shroom_update(TriggerMoby *shroom_moby) {
    enum {
        TALL_SHROOM_STATE_INITIALIZE = 0,
        TALL_SHROOM_STATE_WAIT_FOR_HIT = 1,
        TALL_SHROOM_STATE_REMOVE_WITH_EFFECTS = 2
    };
    TriggerHit *hit;
    s32         flag;

    flag = 0;
    hit = find_moby_hit(shroom_moby, 0x10000, 0);
    switch (shroom_moby->state) {
    case TALL_SHROOM_STATE_INITIALIZE:
        shroom_moby->state = 1;
        break;
    case TALL_SHROOM_STATE_WAIT_FOR_HIT:
        if (hit != 0 && hit->f2c > 0.0f) {
            flag = 1;
        }
        if (flag) {
            shroom_moby->state = 2;
        }
        break;
    case TALL_SHROOM_STATE_REMOVE_WITH_EFFECTS:
        allocate_voice_for_target_entry(0, 0, shroom_moby);
        spawn_proximity_gated_bolts(shroom_moby);
        FUN_L00_00263fd8(shroom_moby,
                         0x717,
                         shroom_moby->pos,
                         shroom_moby->rot,
                         0,
                         0,
                         &D_L01_0015F580,
                         &D_L01_0015F580,
                         0.0f,
                         &D_L01_0015F580);
        spawn_moby_from_placement(shroom_moby, 0x718);
        mark_moby_for_removal(shroom_moby);
        break;
    }
}
