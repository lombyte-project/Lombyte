/* Shared moby class 0x324: an effect that follows a morph-o-ray target (class 0xB9) and fades out. */

#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"

typedef struct {
    u8      pad[0x20];
    u8      b20;
    u8      pad2[0x40 - 0x21];
    OvlQuad q40;
    u8      pad3[0x78 - 0x50];
    void   *p78;
    u8      pad4[0xA6 - 0x7C];
    s16     hA6;
} O_002d7f88;

typedef struct {
    O_002d7f88 *o;
    f32         f4;
    u8          v8[4];
    f32         fC;
} D_002d7f88;

typedef struct {
    u8          pad[0x10];
    u8          v10[4];
    f32         f14;
    u8          pad2[0x23 - 0x18];
    u8          b23;
    u8          pad3[0x2C - 0x24];
    f32         f2C;
    u8          pad4[0x40 - 0x30];
    OvlQuad     q40;
    u8          pad5[0x78 - 0x50];
    D_002d7f88 *p78;
} S_002d7f88;

extern f32 D_L00_001616DC;

f32 D_L01_0016170C = 8.0f;
f32 D_L02_001616AC = 8.0f;
f32 D_L03_001616BC = 8.0f;
f32 D_L04_001616AC = 8.0f;
f32 D_L05_001616FC = 8.0f;
f32 D_L06_001616DC = 8.0f;
f32 D_L07_001616AC = 8.0f;
f32 D_L08_001616FC = 8.0f;
f32 D_L09_0016176C = 8.0f;
f32 D_L10_001616BC = 8.0f;
f32 D_L11_0016173C = 8.0f;
f32 D_L12_001616BC = 8.0f;
f32 D_L13_0016178C = 8.0f;
f32 D_L14_001618CC = 8.0f;
f32 D_L15_0016186C = 8.0f;
f32 D_L16_0016176C = 8.0f;
f32 D_L17_001617BC = 8.0f;

void update_linked_moby_transform_and_lifetime(S_002d7f88 *self) __asm__("FUN_L00_002d7f88");

void update_linked_moby_transform_and_lifetime(S_002d7f88 *self) {
    D_002d7f88 *moby_data = self->p78;
    O_002d7f88 *o;
    void       *linked_moby_data;
    f32         scale_coefficient;
    moby_data->f4 += (D_L00_001616DC + D_L00_001616DC) * frame_time;
    if (fast_dec_timer(moby_data->v8) != 0 || D_L00_001616DC * 1.1f <= moby_data->f4)
        goto kill;
    o = moby_data->o;
    if (o == 0 || o->hA6 != 0xB9 || o->b20 == 0xFE || o->b20 == 0xFD) {
    kill:
        mark_moby_for_removal(self);
        return;
    }
    linked_moby_data = o->p78;
    clear_u64_value(self->v10);
    self->f14 = -moby_data->f4;
    transform_vector_by_basis(self->v10, self->v10, (u8 *)moby_data->o + 0xC0);
    fast_vec_add(self->v10, self->v10, linked_moby_data);
    qcopy(&self->q40, &moby_data->o->q40);
    if (moby_data->f4 > D_L00_001616DC) {
        self->b23 =
            truncate_float_to_s32((1.0f - (moby_data->f4 - D_L00_001616DC) * 10.0f) * 127.0f);
    }
    scale_coefficient = moby_data->fC;
    self->f2C = scale_coefficient * 0.1f + scale_coefficient * moby_data->f4 / D_L00_001616DC;
}
