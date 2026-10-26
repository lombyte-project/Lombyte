#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
struct ShrubLightLink {
    u8 pad_0[0x1B];
    u8 unk1B;
    u8 pad_1C[0x2];
    u16 packed_lights;
};

struct TfragLightLink {
    u8 pad_0[0x35];
    u8 unk35;
    u16 packed_lights;
};

struct TieLightLink {
    u8 pad_0[0x1B];
    u8 unk1B;
    u8 pad_1C[0x2];
    u16 packed_lights;
};

struct PointLightIndexRanges {
    s16 tie_start;
    s16 tie_count;
    s16 shrub_start;
    s16 shrub_count;
    s16 tfrag_start;
    s16 tfrag_count;
    s32 instance_indices_address;
};

extern u8 D_001E7B50[];
extern u8 D_001E7B70[];
extern u8 D_001E7B98[];
extern s32 DebugPrint();
void detach_point_light(s32 light_index) __asm__("FUN_00201f88");

void detach_point_light(s32 light_index) {
    s16 *tie_index_cursor;
    s16 *shrub_index_cursor;
    s16 *tfrag_index_cursor;
    s32 shrub_indices_end_address;
    s32 tie_indices_end_address;
    s32 tfrag_indices_end_address;
    s32 light_third_nibble;
    s32 light_second_nibble;
    s32 light_fourth_nibble;
    s32 light_low_nibble;
    s32 tfrag_first_light;
    s32 shrub_first_light;
    s32 tie_first_light;
    u32 shrub_packed_lights;
    u32 tie_packed_lights;
    u32 tfrag_packed_lights;
    struct ShrubLightLink *shrub_link;
    struct TfragLightLink *tfrag_link;
    struct TieLightLink *tie_link;
    struct PointLightIndexRanges *light_entry;

    light_low_nibble = light_index & 0xFFFF;
    light_entry = (light_index * 0x30) + point_light_data;
    light_second_nibble = (light_index * 0x10) & 0xFFFF;
    light_third_nibble = (light_index << 8) & 0xFFFF;
    tie_index_cursor = light_entry->instance_indices_address + (light_entry->tie_start * 2);
    tie_indices_end_address = tie_index_cursor + light_entry->tie_count;
    light_fourth_nibble = (light_index << 0xC) & 0xFFFF;
    if (tie_index_cursor != tie_indices_end_address) {
        do {
            tie_link = *(s32 *)0x160F50 + (*tie_index_cursor << 5);
            tie_packed_lights = tie_link->packed_lights;
            tie_first_light = tie_packed_lights & 0xF;
            if (tie_first_light == light_low_nibble) {
                tie_packed_lights = (tie_packed_lights >> 4) | 0xF000;
            } else if ((tie_packed_lights & 0xF0) == light_second_nibble) {
                tie_packed_lights = tie_first_light | ((tie_packed_lights >> 4) & 0xFF0) | 0xF000;
            } else if ((tie_packed_lights & 0xF00) == light_third_nibble) {
                tie_packed_lights = (tie_packed_lights & 0xFF) | ((tie_packed_lights >> 4) & 0xF00) | 0xF000;
            } else if ((tie_packed_lights & 0xF000) == light_fourth_nibble) {
                tie_packed_lights |= 0xF000;
            } else {
                DebugPrint(D_001E7B50);
            }
            tie_index_cursor += 1;
            if (tie_packed_lights == 0xFFFF) {
                tie_link->unk1B = 1;
            }
            tie_link->packed_lights = tie_packed_lights;
        } while (tie_index_cursor != tie_indices_end_address);
    }
    tfrag_index_cursor = light_entry->instance_indices_address + (light_entry->tfrag_start * 2);
    light_entry->tie_start = 0;
    tfrag_indices_end_address = tfrag_index_cursor + light_entry->tfrag_count;
    light_entry->tie_count = 0;
    if (tfrag_index_cursor != tfrag_indices_end_address) {
        do {
            tfrag_link = *(s32 *)0x160E8C + (*tfrag_index_cursor << 6);
            tfrag_packed_lights = tfrag_link->packed_lights;
            tfrag_first_light = tfrag_packed_lights & 0xF;
            if (tfrag_first_light == light_low_nibble) {
                tfrag_packed_lights = (tfrag_packed_lights >> 4) | 0xF000;
            } else if ((tfrag_packed_lights & 0xF0) == light_second_nibble) {
                tfrag_packed_lights = tfrag_first_light | ((tfrag_packed_lights >> 4) & 0xFF0) | 0xF000;
            } else if ((tfrag_packed_lights & 0xF00) == light_third_nibble) {
                tfrag_packed_lights = (tfrag_packed_lights & 0xFF) | ((tfrag_packed_lights >> 4) & 0xF00) | 0xF000;
            } else if ((tfrag_packed_lights & 0xF000) == light_fourth_nibble) {
                tfrag_packed_lights |= 0xF000;
            } else {
                DebugPrint(D_001E7B70);
            }
            tfrag_index_cursor += 1;
            if (tfrag_packed_lights == 0xFFFF) {
                tfrag_link->unk35 = 1;
            }
            tfrag_link->packed_lights = tfrag_packed_lights;
        } while (tfrag_index_cursor != tfrag_indices_end_address);
    }
    shrub_index_cursor = light_entry->instance_indices_address + (light_entry->shrub_start * 2);
    light_entry->tfrag_start = 0;
    shrub_indices_end_address = shrub_index_cursor + light_entry->shrub_count;
    light_entry->tfrag_count = 0;
    if (shrub_index_cursor != shrub_indices_end_address) {
        do {
            shrub_link = *(s32 *)0x1603D4 + (*shrub_index_cursor << 5);
            shrub_packed_lights = shrub_link->packed_lights;
            shrub_first_light = shrub_packed_lights & 0xF;
            if (shrub_first_light == light_low_nibble) {
                shrub_packed_lights = (shrub_packed_lights >> 4) | 0xF000;
            } else if ((shrub_packed_lights & 0xF0) == light_second_nibble) {
                shrub_packed_lights = shrub_first_light | ((shrub_packed_lights >> 4) & 0xFF0) | 0xF000;
            } else if ((shrub_packed_lights & 0xF00) == light_third_nibble) {
                shrub_packed_lights = (shrub_packed_lights & 0xFF) | ((shrub_packed_lights >> 4) & 0xF00) | 0xF000;
            } else if ((shrub_packed_lights & 0xF000) == light_fourth_nibble) {
                shrub_packed_lights |= 0xF000;
            } else {
                DebugPrint(D_001E7B98);
            }
            shrub_index_cursor += 1;
            if (shrub_packed_lights == 0xFFFF) {
                shrub_link->unk1B = 1;
            }
            shrub_link->packed_lights = shrub_packed_lights;
        } while (shrub_index_cursor != shrub_indices_end_address);
    }
    light_entry->shrub_start = 0;
    light_entry->shrub_count = 0;
}
