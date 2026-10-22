#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
struct Point {
    u64 flags;
    s16 y;
    s16 x;
    s16 u;
    s16 v;
};
extern struct Point D_0018D440[];
extern s16 log2dim(s32) __asm__("FUN_001f97a0");
void unpack_point_records(s32 *src, s32 count) __asm__("FUN_00202800");

void unpack_point_records(s32 *serialized_texture_records, s32 count) {
    s32 x;
    s32 y;
    s32 u;
    s32 v;

    for (effect_tex_cnt = 0; effect_tex_cnt < count; effect_tex_cnt++) {
        x = *serialized_texture_records++;
        y = *serialized_texture_records++;
        u = *serialized_texture_records++;
        v = *serialized_texture_records++;
        D_0018D440[effect_tex_cnt].x = x >> 4;
        D_0018D440[effect_tex_cnt].y = y >> 4;
        D_0018D440[effect_tex_cnt].u = log2dim(u);
        D_0018D440[effect_tex_cnt].v = log2dim(v);
        D_0018D440[effect_tex_cnt].flags = 0;
    }
}

extern __typeof__(unpack_point_records) func_00202800 __attribute__((alias("FUN_00202800")));
