/* Shared moby class statue (and seashell): breaks apart when hit. */
#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "qcopy.h"
#include "rnc/overlay/entities.h"

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

void         mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);

extern float D_L03_0015F580[] MACRO_ADDR;

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_0031BDB8.c: func_L07_00320B50), where it is exact; names translated to the US level program. */

void statue_update(unsigned char *m) __asm__("FUN_L03_002d3cd8");

void statue_update(unsigned char *statue_moby) {
    enum {
        STATUE_INITIALIZE = 0,
        STATUE_WAITING_FOR_HIT = 1,
        STATUE_BREAK_APART = 2
    };
    int   flag = 0;
    float z;
    char *zero_vector, *statue_position, *statue_rotation;
    char *x = find_moby_hit(statue_moby, 0x10000, 0);
    switch (statue_moby[0x20]) {
    case STATUE_INITIALIZE:
        statue_moby[0x20] = 1;
        break;
    case STATUE_WAITING_FOR_HIT:
        if (x != 0) {
            if (*(float *)(x + 0x2C) > 0.0f)
                flag = 1;
        }
        if (flag)
            statue_moby[0x20] = 2;
        break;
    case STATUE_BREAK_APART:
        allocate_voice_for_target_entry(0, 0, (int)statue_moby);
        z = 0.0f;
        zero_vector = D_L03_0015F580;
        statue_position = statue_moby + 0x10;
        spawn_proximity_gated_bolts(statue_moby);
        statue_rotation = statue_moby + 0x40;
        FUN_L00_00263fd8(statue_moby,
                         0x727,
                         statue_position,
                         statue_rotation,
                         0,
                         0,
                         z,
                         zero_vector,
                         zero_vector,
                         zero_vector);
        FUN_L00_00263fd8(statue_moby,
                         0x728,
                         statue_position,
                         statue_rotation,
                         0,
                         0,
                         z,
                         zero_vector,
                         zero_vector,
                         zero_vector);
        FUN_L00_00263fd8(statue_moby,
                         0x729,
                         statue_position,
                         statue_rotation,
                         0,
                         0,
                         z,
                         zero_vector,
                         zero_vector,
                         zero_vector);
        mark_moby_for_removal(statue_moby);
        break;
    }
}
