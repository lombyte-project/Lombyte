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

#include "rnc/rendering/texture/parse_particle_textures_globals.h"
extern u8 D_001CE180[];
extern struct ParticleTex D_001CD980[];
extern void copy_memory_aligned(void *, void *, s32) __asm__("FUN_001f9838");
extern s32 log2dim(s32) __asm__("FUN_001f97a0");

void parse_particle_textures(struct ParticleHdr *hdr, s32 base, s32 *src,
                             s32 count) __asm__("FUN_002026c8");

void parse_particle_textures(struct ParticleHdr *hdr, s32 base, s32 *range_source_cursor, s32 count) {
    s32 relocation_count;
    s32 offset;
    s32 size;
    s32 i;
    s32 start_page;
    s32 start_offset;
    s32 end_page;
    s32 end_extent;
    s32 *relocation_cursor;

    relocation_cursor = hdr->ptrs;
    relocation_count = hdr->count;
    offset = hdr->data_offset;
    size = hdr->data_size;
    for (i = 0; i < relocation_count; i = i + 1, relocation_cursor = relocation_cursor + 1) {
        s32 relocation_word;
        relocation_word = *relocation_cursor;
        if (relocation_word == 0) {
            D_001CDF80[i] = (s32)D_001CE180;
        } else {
            D_001CDF80[i] = relocation_word - (offset - (s32)D_001CE180);
        }
    }
    copy_memory_aligned(D_001CE180, (u8 *)hdr + offset, size);
    particle_texture_range_count = 0;
    while (particle_texture_range_count < count) {
        start_page = base + *range_source_cursor++;
        start_offset = *range_source_cursor++;
        end_page = base + *range_source_cursor++;
        end_extent = *range_source_cursor++;
        D_001CD980[particle_texture_range_count].tex0 = (start_page << 4) + start_offset;
        D_001CD980[particle_texture_range_count].tex1 = (end_page << 4) + log2dim(end_extent);
        particle_texture_range_count = particle_texture_range_count + 1;
    }
}

extern __typeof__(parse_particle_textures) func_002026C8 __attribute__((alias("FUN_002026c8")));
