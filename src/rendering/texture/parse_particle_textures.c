#include "types.h"
#include "rnc/data_labels.h"

struct ParticleHdr {
    s32 count;
    s32 pad4;
    s32 data_offset;
    s32 data_size;
    s32 ptrs[1];
};
struct ParticleTex {
    s32 tex0;
    s32 tex1;
};

extern s32 D_001600C0;
extern u8 D_001CE180[];
extern struct ParticleTex D_001CD980[];
extern void copy_memory_aligned(void *, void *, s32) __asm__("FUN_001f9838");
extern s32 log2dim(s32) __asm__("FUN_001f97a0");

void parse_particle_textures(struct ParticleHdr *hdr, s32 base, s32 *src,
                             s32 count) __asm__("FUN_002026c8");

void parse_particle_textures(struct ParticleHdr *hdr, s32 base, s32 *src, s32 count) {
    s32 relocation_count;
    s32 offset;
    s32 size;
    s32 i;
    s32 start_page;
    s32 b;
    s32 end_page;
    s32 end_extent;
    s32 *relocation_cursor;

    relocation_cursor = hdr->ptrs;
    relocation_count = hdr->count;
    offset = hdr->data_offset;
    size = hdr->data_size;
    for (i = 0; i < relocation_count; i = i + 1, relocation_cursor = relocation_cursor + 1) {
        s32 v;
        v = *relocation_cursor;
        if (v == 0) {
            D_001CDF80[i] = (s32)D_001CE180;
        } else {
            D_001CDF80[i] = v - (offset - (s32)D_001CE180);
        }
    }
    copy_memory_aligned(D_001CE180, (u8 *)hdr + offset, size);
    D_001600C0 = 0;
    while (D_001600C0 < count) {
        start_page = base + *src++;
        b = *src++;
        end_page = base + *src++;
        end_extent = *src++;
        D_001CD980[D_001600C0].tex0 = (start_page << 4) + b;
        D_001CD980[D_001600C0].tex1 = (end_page << 4) + log2dim(end_extent);
        D_001600C0 = D_001600C0 + 1;
    }
}

extern __typeof__(parse_particle_textures) func_002026C8 __attribute__((alias("FUN_002026c8")));
