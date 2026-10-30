/* Level 07 moby classes 0x67f, 0x68d: robot head. */
#include "types.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/mobys/moby.h"

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *create_moby(int);
extern void  FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
void         mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");
extern char *create_moby(int) __asm__("FUN_0020c4f8");

/* Stage machine for a three-state effect moby: wait, activate, burst and delete. */
void robot_head_update(char *self) __asm__("FUN_L07_0031f5b0");

void robot_head_update(char *self) {
    enum {
        ROBOT_HEAD_INITIALIZE = 0,
        ROBOT_HEAD_WAIT_FOR_HIT = 1,
        ROBOT_HEAD_SPAWN_AND_REMOVE = 2
    };
    int   flag = 0;
    char *hit_moby = find_moby_hit(self, 0x10000, 0);
    switch (((unsigned char *)self)[0x20]) {
    case ROBOT_HEAD_INITIALIZE:
        self[0x20] = 1;
        break;
    case ROBOT_HEAD_WAIT_FOR_HIT:
        if (hit_moby != 0 && MOBY(hit_moby)->scale > 0.0f) {
            flag = 1;
        }
        if (flag) {
            self[0x20] = 2;
        }
        break;
    case ROBOT_HEAD_SPAWN_AND_REMOVE: {
        char *spawned_moby;
        allocate_voice_for_target_entry(0, 0, (int)self);
        spawn_proximity_gated_bolts(self);
        FUN_L00_00263fd8(self,
                         0x68e,
                         self + 0x10,
                         self + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L07_0015F580,
                         D_L07_0015F580,
                         D_L07_0015F580);
        FUN_L00_00263fd8(self,
                         0x68f,
                         self + 0x10,
                         self + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L07_0015F580,
                         D_L07_0015F580,
                         D_L07_0015F580);
        FUN_L00_00263fd8(self,
                         0x690,
                         self + 0x10,
                         self + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L07_0015F580,
                         D_L07_0015F580,
                         D_L07_0015F580);
        spawned_moby = create_moby(0x68d);
        if (spawned_moby != 0) {
            spawned_moby[0x31] = 1;
            MOBY(spawned_moby)->draw_dist = 0xFF;
            qcopy(spawned_moby + 0x10, self + 0x10);
            qcopy(spawned_moby + 0x40, self + 0x40);
            MOBY(spawned_moby)->spawn_frame = *(long *)(self + 0x38);
            update_entity(spawned_moby);
        }
        mark_moby_for_removal(self);
        break;
    }
    }
}
