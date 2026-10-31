/* Level 06 moby class 0x680: breakable prop variant; when hit it drops bolts, throws two class 0x682 pieces and leaves a class 0x681 part. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "sda.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/mobys/moby.h"

/* Same source as the exact FUN_L06_00309860, with two 0x682 effects and spawned class 0x681. */
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

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *create_moby(int);
extern void  FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
void         mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");
extern char *create_moby(int) __asm__("FUN_0020c4f8");

void breakable_prop_variant_update(char *self) __asm__("FUN_L06_00309fa8");

/* Moby class 1664 */
void breakable_prop_variant_update(char *spawner_moby) {
    enum {
        INITIALIZE = 0,
        WAIT_FOR_HIT = 1,
        SPAWN_AND_REMOVE = 2
    };
    int   flag = 0;
    char *hit_moby = find_moby_hit(spawner_moby, 0x10000, 0);
    switch (((unsigned char *)spawner_moby)[0x20]) {
    case INITIALIZE:
        spawner_moby[0x20] = 1;
        break;
    case WAIT_FOR_HIT:
        if (hit_moby != 0 && MOBY(hit_moby)->scale > 0.0f) {
            flag = 1;
        }
        if (flag) {
            spawner_moby[0x20] = 2;
        }
        break;
    case SPAWN_AND_REMOVE: {
        char *spawned_moby;
        allocate_voice_for_target_entry(0, 0, (int)spawner_moby);
        spawn_proximity_gated_bolts(spawner_moby);
        FUN_L00_00263fd8(spawner_moby,
                         0x682,
                         spawner_moby + 0x10,
                         spawner_moby + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L06_0015F580,
                         D_L06_0015F580,
                         D_L06_0015F580);
        FUN_L00_00263fd8(spawner_moby,
                         0x682,
                         spawner_moby + 0x10,
                         spawner_moby + 0x40,
                         0,
                         0,
                         0.0f,
                         D_L06_0015F580,
                         D_L06_0015F580,
                         D_L06_0015F580);
        spawned_moby = create_moby(0x681);
        if (spawned_moby != 0) {
            spawned_moby[0x31] = 1;
            MOBY(spawned_moby)->draw_dist = 0xFF;
            qcopy(spawned_moby + 0x10, spawner_moby + 0x10);
            qcopy(spawned_moby + 0x40, spawner_moby + 0x40);
            MOBY(spawned_moby)->spawn_frame = *(long *)(spawner_moby + 0x38);
            update_entity(spawned_moby);
        }
        mark_moby_for_removal(spawner_moby);
        break;
    }
    }
}
