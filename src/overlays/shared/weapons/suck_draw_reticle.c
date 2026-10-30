/* Suck cannon: draws the reticle. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/quad.h"

#include "qcopy.h"
#include "rnc/overlay/level_globals.h"

typedef struct {
    OvlQuad pos;
    float   f10;
    int     f14;
    int     f18;
    float   f1c;
    int     f20;
    short   f24;
    short   f26;
    float   f28;
    int     f2c;
} E1edf60;

typedef struct {
    E1edf60 e[4];
    int     count;
} T1edf60;

extern T1edf60 l00_reticle_submission_queue __asm__("D_L00_00169040");

void suck_draw_reticle(int      a,
                       int      b,
                       OvlQuad *pos,
                       int      kind,
                       int      mode,
                       float    x,
                       float    y,
                       float    z) __asm__("FUN_L00_001edf60");
void suck_draw_reticle(int      source_entity,
                       int      queued_reticle_value,
                       OvlQuad *pos,
                       int      kind,
                       int      mode,
                       float    x,
                       float    y,

                       float z) {
    E1edf60 *e;
    if (D_L00_0015F5C4 == 2)
        return;
    if (l00_reticle_submission_queue.count >= 3)
        return;
    e = &l00_reticle_submission_queue.e[l00_reticle_submission_queue.count];
    e->f20 = source_entity;
    e->f14 = queued_reticle_value;
    e->f10 = x;
    e->f1c = y;
    e->f18 = kind;
    if (pos) {
        qcopy(&e->pos, pos);
        e->f24 = 1;
    } else {
        e->f24 = 0;
    }
    if (mode == -1) {
        if (kind != 13) {
            if (kind < 13)
                goto done;
            if (kind >= 40)
                goto done;
            if (kind < 31)
                goto done;
        }
        e->f2c = 2;
        e->f28 = 1.5707964f;
        e->f26 = 0;
    } else {
        e->f2c = mode;
        e->f28 = z;
        e->f26 = 4;
    }
done:
    l00_reticle_submission_queue.count++;
}

T1edf60 D_L01_001694C0 = {0};

T1edf60 D_L02_00169640 = {0};

T1edf60 D_L03_00169140 = {0};

T1edf60 D_L04_001691C0 = {0};

T1edf60 D_L05_00169440 = {0};

T1edf60 D_L06_001697C0 = {0};

T1edf60 D_L07_001690C0 = {0};

T1edf60 D_L08_00169840 = {0};

T1edf60 D_L09_001691C0 = {0};

T1edf60 D_L10_001694C0 = {0};

T1edf60 D_L11_00169A40 = {0};

T1edf60 D_L12_001694C0 = {0};

T1edf60 D_L13_00169340 = {0};

T1edf60 D_L14_00169740 = {0};

T1edf60 D_L15_00169640 = {0};

T1edf60 D_L16_00169440 = {0};

T1edf60 D_L17_00169940 = {0};

T1edf60 D_L18_00169A40 = {0};
