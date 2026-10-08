#include "types.h"

#include "rnc/rendering/dma_tag.h"
#include "rnc/globals.h"

extern s32 D_0015EE84;

void draw_boot_image(u32 image_address) __asm__("FUN_002012b8");

void draw_boot_image(u32 image_address) {
    struct DmaTag *tag;
    struct DmaTag *next;
    u64 *register_words;
    s32 remaining_rows;
    s32 destination_block;
    s64 upload_rows;
    s32 rows_after_upload;
    s64 image_quadword_count;

    remaining_rows = pal_mode ? 0x1C0 : 0x1A0;
    destination_block = D_0015EE84 >> 8;
    do {
        rows_after_upload = remaining_rows - 0x80;
        upload_rows = 0x80;
        if (rows_after_upload < 0) {
            upload_rows = remaining_rows;
        }
        render_packet_cursor.tag->tag = 0x10000006;
        render_packet_cursor.tag->addr = 0;
        render_packet_cursor.tag->vif0 = 0;
        render_packet_cursor.tag->vif1 = 0x50000006;
        tag = render_packet_cursor.tag;
        next = tag + 7;
        render_packet_cursor.tag = tag + 1;
        register_words = (u64 *)(tag + 1);
        register_words[0] = 0x4000000000000001;
        register_words[1] = 0xEEEEEEE;
        register_words[2] = ((u64)destination_block << 32) | 0x0008000000000000;
        register_words[3] = 0x50;
        register_words[4] = 0;
        register_words[5] = 0x51;
        register_words[6] = ((u64)upload_rows << 32) | 0x200;
        register_words[7] = 0x52;
        register_words[8] = 0;
        register_words[9] = 0x53;
        image_quadword_count = upload_rows << 7;
        register_words[10] = ((u64)image_quadword_count) | 0x0800000000008000;
        register_words[11] = 0;
        render_packet_cursor.tag = next;
        render_packet_cursor.tag->tag = image_quadword_count | 0x30000000;
        render_packet_cursor.tag->addr = image_address;
        {
            s64 upload_bytes = upload_rows << 11;
            image_address += upload_bytes;
        }
        render_packet_cursor.tag->vif0 = 0;
        render_packet_cursor.tag->vif1 = (s32)image_quadword_count | 0x50000000;
        render_packet_cursor.tag++;
        {
            s64 uploaded_blocks = upload_rows << 3;
            destination_block += uploaded_blocks;
        }
        remaining_rows = rows_after_upload;
    } while (remaining_rows > 0);
    /* Finish the upload chain with a TEXFLUSH register write. */
    {
        struct DmaTag *final_tag;
        struct DmaTag *final_next;
        u64 *final_words;
        render_packet_cursor.tag->tag = 0x10000002;
        render_packet_cursor.tag->addr = 0;
        render_packet_cursor.tag->vif0 = 0;
        render_packet_cursor.tag->vif1 = 0x50000002;
        final_tag = render_packet_cursor.tag;
        final_next = final_tag + 3;
        final_words = (u64 *)(final_tag + 1);
        render_packet_cursor.tag = final_tag + 1;
        final_words[0] = 0x1000000000008001;
        final_words[1] = 0xE;
        final_words[2] = 0;
        final_words[3] = 0x3F;
        render_packet_cursor.tag = final_next;
    }
}

extern __typeof__(draw_boot_image) func_002012B8 __attribute__((alias("FUN_002012b8")));
