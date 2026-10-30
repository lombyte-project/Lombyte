/* Level 07 moby class 0x672: mushroom. */
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
void         mushroom_update(char *self) __asm__("FUN_L07_0031f3d8");

void mushroom_update(char *self) {
    enum {
        MUSHROOM_INITIALIZE = 0,
        MUSHROOM_WAIT_FOR_HIT = 1,
        MUSHROOM_SPAWN_AND_REMOVE = 2
    };
    int   flag = 0;
    char *hit_moby = find_moby_hit(self, 0x10000, 0);
    switch (((unsigned char *)self)[0x20]) {
    case MUSHROOM_INITIALIZE:
        self[0x20] = 1;
        break;
    case MUSHROOM_WAIT_FOR_HIT:
        if (hit_moby != 0 && MOBY(hit_moby)->scale > 0.0f) {
            flag = 1;
        }
        if (flag) {
            self[0x20] = 2;
        }
        break;
    case MUSHROOM_SPAWN_AND_REMOVE: {
        char *spawned_moby;
        allocate_voice_for_target_entry(0, 0, (int)self);
        spawn_proximity_gated_bolts(self);
        FUN_L00_00263fd8(self,
                         0x674,
                         self + 0x10,
                         self + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L07_0015F580,
                         D_L07_0015F580,
                         D_L07_0015F580);
        FUN_L00_00263fd8(self,
                         0x675,
                         self + 0x10,
                         self + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L07_0015F580,
                         D_L07_0015F580,
                         D_L07_0015F580);
        FUN_L00_00263fd8(self,
                         0x676,
                         self + 0x10,
                         self + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L07_0015F580,
                         D_L07_0015F580,
                         D_L07_0015F580);
        spawned_moby = create_moby(0x673);
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
