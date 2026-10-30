/* Level 06 moby class 0x67c: breakable prop; when hit it drops bolts, throws two class 0x67e pieces and leaves a class 0x67d part. */
#include "types.h"
#include "asm.h"

#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "qcopy.h"
#include "rnc/overlay/entities.h"

/* Same source as the exact FUN_L06_00309860, with two 0x67E effects and spawned class 0x67D. */
typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            f90;
} Obj;

extern char *create_moby(int);
extern float D_L06_0015F580[] MACRO_ADDR;
extern void  FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
void         mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");
extern char *create_moby(int) __asm__("FUN_0020c4f8");

void update_breakable_prop(char *self) __asm__("FUN_L06_00309e08");

void update_breakable_prop(char *self) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_WAIT_FOR_HIT = 1,
        MOBY_SPAWN_AND_REMOVE = 2
    };
    int          flag = 0;
    struct Moby *hit_moby = find_moby_hit(self, 0x10000, 0);
    switch (((unsigned char *)self)[0x20]) {
    case MOBY_INITIALIZE:
        self[0x20] = 1;
        break;
    case MOBY_WAIT_FOR_HIT:
        if (hit_moby != 0 && hit_moby->scale > 0.0f) {
            flag = 1;
        }
        if (flag) {
            self[0x20] = 2;
        }
        break;
    case MOBY_SPAWN_AND_REMOVE: {
        char *spawned_moby;
        allocate_voice_for_target_entry(0, 0, (int)self);
        spawn_proximity_gated_bolts(self);
        FUN_L00_00263fd8(self,
                         0x67E,
                         self + 0x10,
                         self + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L06_0015F580,
                         D_L06_0015F580,
                         D_L06_0015F580);
        FUN_L00_00263fd8(self,
                         0x67E,
                         self + 0x10,
                         self + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L06_0015F580,
                         D_L06_0015F580,
                         D_L06_0015F580);
        spawned_moby = create_moby(0x67D);
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
