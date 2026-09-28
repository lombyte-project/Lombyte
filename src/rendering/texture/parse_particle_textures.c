#include "types.h"

struct ParticleHdr { s32 count; s32 pad4; s32 data_offset; s32 data_size; s32 ptrs[1]; };
struct ParticleTex { s32 tex0; s32 tex1; };

extern s32 D_001600C0;
extern u8 D_001CE180[];
extern s32 D_001CDF80[];
extern struct ParticleTex D_001CD980[];
extern void FUN_001f9838(void *, void *, s32);
extern s32 FUN_001f97a0(s32);

void parse_particle_textures(struct ParticleHdr *hdr, s32 base, s32 *src, s32 count) __asm__("FUN_002026c8");

void parse_particle_textures(struct ParticleHdr *hdr, s32 base, s32 *src, s32 count) {
    s32 n;
    s32 offset;
    s32 size;
    s32 i;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 *p;

    p = hdr->ptrs;
    n = hdr->count;
    offset = hdr->data_offset;
    size = hdr->data_size;
    for (i = 0; i < n; i = i + 1, p = p + 1) {
        s32 v;
        v = *p;
        if (v == 0) {
            D_001CDF80[i] = (s32)D_001CE180;
        } else {
            D_001CDF80[i] = v - (offset - (s32)D_001CE180);
        }
    }
    FUN_001f9838(D_001CE180, (u8 *)hdr + offset, size);
    D_001600C0 = 0;
    while (D_001600C0 < count) {
        a = base + *src++;
        b = *src++;
        c = base + *src++;
        d = *src++;
        D_001CD980[D_001600C0].tex0 = (a << 4) + b;
        D_001CD980[D_001600C0].tex1 = (c << 4) + FUN_001f97a0(d);
        D_001600C0 = D_001600C0 + 1;
    }
}

extern __typeof__(parse_particle_textures) func_002026C8 __attribute__((alias("FUN_002026c8")));
