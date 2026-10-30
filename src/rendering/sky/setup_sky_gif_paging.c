#include "types.h"

#include "rnc/rendering/dma_tag.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

struct SkyVisibilityEntry {
    u64 flags;
    u8  pad8[8];
};

struct SkyVisibilityList {
    u8                         pad0[0xC];
    s16                        count;
    u8                         padE[2];
    struct SkyVisibilityEntry *points;
};

extern struct DmaTag            *D_00160470;
extern u8                        sky_textures_label[] __asm__("D_00160450");
extern struct SkyVisibilityList *level_sky_effect_data __asm__("D_0016045C");
extern void                      noop_core_callback_secondary(void *, s32) __asm__("func_001F21B8");

void setup_sky_gif_paging(void) __asm__("FUN_0022b4c8");

void setup_sky_gif_paging(void) {
    struct DmaTag            *tag;
    struct SkyVisibilityList *visibility_list;
    s32                       entry_index;

    tag = render_packet_cursor.tag;
    D_00160470 = tag;
    tag = tag + 1;
    render_packet_cursor.tag = tag;
    noop_core_callback_secondary(sky_textures_label, 1);
    visibility_list = level_sky_effect_data;
    gs_texture_allocation_cursor = gs_texture_allocation_start;
    pending_texture_upload_count = 0;
    for (entry_index = 0; entry_index < visibility_list->count; entry_index++) {
        visibility_list->points[entry_index].flags = 0;
    }
}

extern __typeof__(setup_sky_gif_paging) func_0022B4C8 __attribute__((alias("FUN_0022b4c8")));
