#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/frame/log2dim.h"

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

extern u8                 resident_lookup_payload[] __asm__("D_001CE180");
extern struct ParticleTex resident_particle_texture_ranges[] __asm__("D_001CD980");

void parse_particle_textures(struct ParticleHdr *hdr,
                             s32                 base,
                             s32                *src,
                             s32                 count) __asm__("FUN_002026c8");

void parse_particle_textures(struct ParticleHdr *hdr,
                             s32                 range_page_base,
                             s32                *range_source_cursor,

                             s32 count) {
    s32  relocation_count;
    s32  offset;
    s32  size;
    s32  i;
    s32  start_page;
    s32  start_offset;
    s32  end_page;
    s32  end_extent;
    s32 *relocation_cursor;

    relocation_cursor = hdr->ptrs;
    relocation_count = hdr->count;
    offset = hdr->data_offset;
    size = hdr->data_size;
    for (i = 0; i < relocation_count; i = i + 1, relocation_cursor = relocation_cursor + 1) {
        s32 relocation_word;
        relocation_word = *relocation_cursor;
        if (relocation_word == 0) {
            D_001CDF80[i] = (s32)resident_lookup_payload;
        } else {
            D_001CDF80[i] = relocation_word - (offset - (s32)resident_lookup_payload);
        }
    }
    copy_memory_aligned(resident_lookup_payload, (u8 *)hdr + offset, size);
    particle_texture_range_count = 0;
    while (particle_texture_range_count < count) {
        start_page = range_page_base + *range_source_cursor++;
        start_offset = *range_source_cursor++;
        end_page = range_page_base + *range_source_cursor++;
        end_extent = *range_source_cursor++;
        resident_particle_texture_ranges[particle_texture_range_count].tex0 =
            (start_page << 4) + start_offset;
        resident_particle_texture_ranges[particle_texture_range_count].tex1 =
            (end_page << 4) + log2dim(end_extent);
        particle_texture_range_count = particle_texture_range_count + 1;
    }
}

extern __typeof__(parse_particle_textures) func_002026C8 __attribute__((alias("FUN_002026c8")));
