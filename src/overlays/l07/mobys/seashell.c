/* Level 07 moby class 0x696: seashell. */
#include "types.h"
#include "asm.h"
#include "sda.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
void         mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");
void         seashell_update(unsigned char *m) __asm__("FUN_L07_0031f780");

/* Moby class 1686 */
void seashell_update(unsigned char *seashell_moby) {
    enum {
        SEASHELL_INITIALIZE = 0,
        SEASHELL_WAIT_FOR_HIT = 1,
        SEASHELL_SPAWN_AND_REMOVE = 2
    };
    int   flag = 0;
    float z;
    char *zero_vector, *fragment_spawn_position, *source_rotation;
    char *x = find_moby_hit(seashell_moby, 0x10000, 0);
    switch (seashell_moby[0x20]) {
    case SEASHELL_INITIALIZE:
        seashell_moby[0x20] = 1;
        break;
    case SEASHELL_WAIT_FOR_HIT:
        if (x != 0) {
            if (*(float *)(x + 0x2C) > 0.0f)
                flag = 1;
        }
        if (flag)
            seashell_moby[0x20] = 2;
        break;
    case SEASHELL_SPAWN_AND_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)seashell_moby);
        z = 0.0f;
        zero_vector = D_L07_0015F580;
        fragment_spawn_position = seashell_moby + 0x10;
        spawn_proximity_gated_bolts(seashell_moby);
        source_rotation = seashell_moby + 0x40;
        FUN_L00_00263fd8(seashell_moby,
                         0x698,
                         fragment_spawn_position,
                         source_rotation,
                         0,
                         0,
                         z,
                         zero_vector,
                         zero_vector,
                         zero_vector);
        FUN_L00_00263fd8(seashell_moby,
                         0x699,
                         fragment_spawn_position,
                         source_rotation,
                         0,
                         0,
                         z,
                         zero_vector,
                         zero_vector,
                         zero_vector);
        FUN_L00_00263fd8(seashell_moby,
                         0x69a,
                         fragment_spawn_position,
                         source_rotation,
                         0,
                         0,
                         z,
                         zero_vector,
                         zero_vector,
                         zero_vector);
        mark_moby_for_removal(seashell_moby);
        break;
    }
}
