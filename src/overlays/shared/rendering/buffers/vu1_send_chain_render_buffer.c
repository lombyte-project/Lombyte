/* VU1: sends the render buffer chain and waits for it. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_globals.h"
#include "sda.h"
#include "rnc/rendering/frame/reset_graphics.h"

extern volatile int D_L00_001611A0_gp __asm__("D_L00_001611A0") __attribute__((sda));
extern int          D_L00_001611C0_002a08c8 __asm__("D_L00_001611C0");
void                debug_print(char *) __asm__("FUN_001e93b0");
u32                *FUN_00122fe0_002a08c8(int) __asm__("FUN_00122fe0");
void                flush_cache(int) __asm__("FUN_00118a80");
void                FUN_001232c8_002a08c8(u32 *, u32) __asm__("FUN_001232c8");
extern u32          D_L00_001611B8[] NOT_SDA;

#include "rnc/overlay/shared/rendering/buffers/vu1_send_chain_render_buffer_globals.h"

extern char vu1_render_buffer_overflow_message[] __asm__("D_L00_001E9530");
void        vu1_send_chain_render_buffer(void) __asm__("FUN_L00_002a08c8");

void vu1_send_chain_render_buffer(void) {
    int  buffer_limit_exceeded;
    int  buffer_used_bytes;
    u32 *flag_word;
    buffer_used_bytes = D_L00_001611C0_002a08c8 - D_L00_001611B8[selected_render_buffer_index];
    D_L00_001611A0_gp |= 0x1F;
    buffer_limit_exceeded = 0;
    if (render_buffer_high_water_bytes < buffer_used_bytes) {
        render_buffer_high_water_bytes = buffer_used_bytes;
        if (D_L00_001611CC < buffer_used_bytes) {
            debug_print(vu1_render_buffer_overflow_message);
            buffer_limit_exceeded = 1;
        }
    }
    if (buffer_limit_exceeded == 0) {
        {
            int *buffer_write_word = (int *)D_L00_001611C0_002a08c8;
            *buffer_write_word = 0x70000000;
        }
        *(int *)(D_L00_001611C0_002a08c8 + 4) = 0;
        *(int *)(D_L00_001611C0_002a08c8 + 8) = 0;
        *(int *)(D_L00_001611C0_002a08c8 + 12) = 0;
        flag_word = FUN_00122fe0_002a08c8(1);
        *flag_word |= 0xC0;
        flush_cache(0);
        FUN_001232c8_002a08c8(flag_word, D_L00_001611B8[selected_render_buffer_index]);
    } else {
        D_L00_001611A0_gp = 0;
    }
}

extern int  D_L00_001611A0 __attribute__((sda));
extern char vu1_sync_timeout_message[] __asm__("D_L00_001E9548");
void        FUN_L00_001ff018(int);
/* Waits while D_L00_001611A0 has any mask bit; after 100000 tries reports and resets graphics. */
void vu1_sync_chain(int mask) __asm__("FUN_L00_002a09d8");

void vu1_sync_chain(int mask) {
    int i;
    for (i = 0; D_L00_001611A0 & mask; i++) {
        FUN_L00_001ff018(0x400);
        if (i > 100000) {
            debug_print(vu1_sync_timeout_message);
            reset_graphics();
            break;
        }
    }
}

int D_L01_001611A0 = 0;
u32 D_L01_001611B8 = 0;
int D_L02_001611A0 = 0;
int D_L03_001611A0 = 0;
u32 D_L03_001611B8 = 0;
int D_L04_001611A0 = 0;
u32 D_L04_001611B8 = 0;
int D_L05_001611A0 = 0;
u32 D_L05_001611B8 = 0;
int D_L06_001611A0 = 0;
u32 D_L06_001611B8 = 0;
int D_L07_001611A0 = 0;
u32 D_L07_001611B8 = 0;
int D_L08_001611A0 = 0;
u32 D_L08_001611B8 = 0;
int D_L09_001611A0 = 0;
int D_L10_001611A0 = 0;
int D_L10_001611C0 = 0;
u32 D_L10_001611B8 = 0;
int D_L11_001611A0 = 0;
u32 D_L11_001611B8 = 0;
int D_L12_001611A0 = 0;
u32 D_L12_001611B8 = 0;
int D_L13_001611A0 = 0;
u32 D_L13_001611B8 = 0;
int D_L14_001611A0 = 0;
u32 D_L14_001611B8 = 0;
int D_L15_001611A0 = 0;
int D_L15_001611C0 = 0;
u32 D_L15_001611B8 = 0;
int D_L16_001611A0 = 0;
int D_L16_001611C0 = 0;
u32 D_L16_001611B8 = 0;
int D_L17_001611A0 = 0;
u32 D_L17_001611B8 = 0;
int D_L18_001611A0 = 0;
u32 D_L18_001611B8 = 0;
