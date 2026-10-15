#include "types.h"
extern s32 D_001A0118 __attribute__((section(".data")));
extern s32 decode_compressed_occlusion_map() __asm__("func_00207C28");
extern s32 expand_map_coverage() __asm__("func_00207E58");
extern void noop_memory_callback() __asm__("func_00208810");
void load_map_chunk(s32 arg0, u8 *src, s32 arg2) __asm__("FUN_00207bb0");

void load_map_chunk(s32 arg0, u8 *src, s32 arg2) {
    noop_memory_callback();
    if (D_001A0118 != 0) {
        if (*src & 1) {
            decode_compressed_occlusion_map(arg0, src, arg2);
        } else {
            expand_map_coverage(arg0, src);
        }
    }
}
