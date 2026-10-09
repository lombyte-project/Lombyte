/* Ported from rac1-decomp (src/game/camera.c, func_001EC5B8). */
#include "sda.h"
#include "rnc/gameplay/hero.h"
typedef struct {
    char unk_00[0x10];
    int unk10;
    char unk14[8];
    int unk1C;
} CamRec20;
extern CamRec20 *D_0015EF90 MACRO_ADDR;
extern CamRec20 *D_0015EF40 MACRO_ADDR;
extern char D_0013F3D0[];
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
typedef struct {
    char unk_00[4];
    int (*fn_04)(void *, void *);
    char unk_08[0xC];
} CamPrioHook;
extern CamPrioHook D_001E8C00[];
/* Camera_ActivationCheckPriority(cur, other): whether camera `cur` should
   take over from `other`. An inactive camera (+0x7C) never does; the camera
   type's +4 hook (D_001E8C00) may decide first (-1 no, 1 yes); otherwise
   the mode at +0x74 decides: 0 (and 1/2 once +0x7D is set) by priority
   byte, 4 through func_00214720 on the target record, 7 by the level's
   mode and target. Each case ends in `if (x) return 1;` falling out to the
   one shared `return 0;`, which is what lets cross-jumping and reorg give
   retail's branches; case 7's final test shares its `return 1` with the
   g < 0 exit so its `$v0 = 1` is not hoisted above the load. */
int camera_activation_check_priority(void *cur, void *other) __asm__("FUN_001ec210");

int camera_activation_check_priority(void *cur, void *other) {
    char *c = (char *)cur;
    char *o = (char *)other;
    unsigned char *state = (unsigned char *)(c + 0x74);

    if (*(unsigned char *)(c + 0x7C) == 0) {
        return 0;
    }
    {
        int (*fn)(void *, void *) = D_001E8C00[*(short *)(c + 0x8C)].fn_04;
        if (fn != 0) {
            switch (fn(cur, other)) {
            case -1:
                return 0;
            case 1:
                return 1;
            }
        }
    }
    switch (*(int *)state) {
    case 1:
    case 2:
        if (state[9] == 0) {
            return 0;
        }
        /* fallthrough */
    case 0:
        if (o == 0) {
            return 1;
        }
        if (*(short *)(o + 0x7E) != 0) {
            return 1;
        }
        if (state[8] > *(unsigned char *)(o + 0x7C)) {
            return 1;
        }
        break;
    case 4: {
        char *p = (char *)D_0015EF90[*(short *)(c + 0x84)].unk1C;

        if (o != 0 && *(short *)(o + 0x7E) == 0 && !(state[8] > *(unsigned char *)(o + 0x7C))) {
            return 0;
        }
        if (is_point_inside_clip_volume(D_0013F3D0, *(int *)(p + 0xC))) {
            return 1;
        }
        break;
    }
    case 7: {
        struct Hero *base = &hero;
        short e = *(short *)(c + 0x86);

        if (e != base->unk2284) {
            return 0;
        }
        if (*(short *)(o + 0x7E) == 0 && !(state[8] > *(unsigned char *)(o + 0x7C))) {
            return 0;
        }
        if (e != 3) {
            return 1;
        }
        {
            CamRec20 *rec = &D_0015EF90[*(short *)(c + 0x84)];
            int g = *(int *)((char *)rec->unk1C + 0x24);

            if (g < 0 ||
                (base->unk560 == D_0015EF40[g].unk10 && base->unk570 == 0)) {
                return 1;
            }
        }
        break;
    }
    }
    return 0;
}

extern __typeof__(camera_activation_check_priority) func_001EC210
    __attribute__((alias("FUN_001ec210")));

/* Defined below their only users, so retail reaches them with lui. */
CamRec20 *D_0015EF40 MACRO_ADDR = 0;
