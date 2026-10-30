/* Draw buffers: transfers draw buffer chunks and appends their upload packets. */
#include "types.h"
#include "asm.h"

void FUN_00122518(void *, short, int, int, int, int, int, int);
void flush_cache(int) __asm__("FUN_00118a80");
void FUN_001227d8(void *, int);
void sceGsSyncPath(int, int) __asm__("FUN_00120558");
void transfer_draw_buffer_chunks(int dst,
                                 int draw_buffer_byte_address,
                                 int size) __asm__("FUN_L00_002a0f18");

void transfer_draw_buffer_chunks(int destination_byte_address,
                                 int draw_buffer_byte_address,

                                 int size) {
    char chunk_transfer_descriptor[0x70] __attribute__((aligned(16)));
    int  left = (size + 0x3FFF) & ~0x3FFF;
    int  transfer_byte_offset = 0;
    while (left > 0) {
        FUN_00122518(chunk_transfer_descriptor,
                     (draw_buffer_byte_address + transfer_byte_offset) >> 8,
                     1,
                     0,
                     0,
                     0,
                     0x40,
                     0x40);
        left -= 0x4000;
        flush_cache(0);
        FUN_001227d8(chunk_transfer_descriptor, destination_byte_address + transfer_byte_offset);
        transfer_byte_offset += 0x4000;
        sceGsSyncPath(0, 0);
    }
}

extern int D_L00_001611C0_002a0fc8 __asm__("D_L00_001611C0");
void       FUN_00122330_002a0fc8(int, short, int, int, int, int, int, int) __asm__("FUN_00122330");
void       append_draw_buffer_upload_packets(int dst,
                                             int draw_buffer_byte_address,
                                             int size) __asm__("FUN_L00_002a0fc8");

void append_draw_buffer_upload_packets(int upload_source_byte_address,
                                       int draw_buffer_byte_address,

                                       int size) {
    int left = (size + 0x3FFF) & ~0x3FFF;
    int transfer_byte_offset = 0;
    while (left > 0) {
        *(int *)D_L00_001611C0_002a0fc8 = 0x10000006;
        *(int *)(D_L00_001611C0_002a0fc8 + 4) = 0;
        *(int *)(D_L00_001611C0_002a0fc8 + 8) = 0;
        *(int *)(D_L00_001611C0_002a0fc8 + 12) = 0x50000006;
        D_L00_001611C0_002a0fc8 += 0x10;
        left -= 0x4000;
        FUN_00122330_002a0fc8(D_L00_001611C0_002a0fc8,
                              (draw_buffer_byte_address + transfer_byte_offset) >> 8,
                              1,
                              0,
                              0,
                              0,
                              0x40,
                              0x40);
        D_L00_001611C0_002a0fc8 += 0x60;
        *(int *)D_L00_001611C0_002a0fc8 = 0x30000400;
        *(int *)(D_L00_001611C0_002a0fc8 + 4) = upload_source_byte_address + transfer_byte_offset;
        *(int *)(D_L00_001611C0_002a0fc8 + 8) = 0;
        *(int *)(D_L00_001611C0_002a0fc8 + 12) = 0x50000400;
        D_L00_001611C0_002a0fc8 += 0x10;
        transfer_byte_offset += 0x4000;
    }
}
