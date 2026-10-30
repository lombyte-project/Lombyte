#include "types.h"
#include "rnc/globals.h"
#include "rnc/ui/map/map_state.h"

extern s32  max_coverage_save_result_by_level[] __asm__("D_0013D560");
extern void fill_transfer_words() __asm__("func_001F97E8");
extern s32  func_001FA860();
extern void downsample_map_coverage() __asm__("func_00208030");
extern void noop_memory_callback() __asm__("func_00208810");

void save_map_coverage(s32 buffer) __asm__("FUN_00207b08");

void save_map_coverage(s32 map_coverage_buffer) {
    s32 coverage_save_result;

    noop_memory_callback();
    if (level_map_selection.unk28 == 0) {
        fill_transfer_words(map_coverage_buffer, 0, 0x800);
        return;
    }
    coverage_save_result = func_001FA860(
        map_coverage_buffer, 0x800, level_map_selection.unk14, level_map_selection.mask);
    if (coverage_save_result == -1) {
        downsample_map_coverage(map_coverage_buffer);
    }
    if (max_coverage_save_result_by_level[current_level_index] < coverage_save_result) {
        max_coverage_save_result_by_level[current_level_index] = coverage_save_result;
    }
}

extern __typeof__(save_map_coverage) func_00207B08 __attribute__((alias("FUN_00207b08")));
