/* Offset checks for map_state.h and map_icon.h; `make check` compiles this
   file with the game compiler. A negative array size fails the build. */
#include "rnc/ui/map/map_state.h"
#include "rnc/ui/map/map_icon.h"

#define CHECK(type, name, field, off) \
    typedef char check_##name[ \
        ((unsigned long)&((struct type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(type, size) \
    typedef char size_check_##type[(sizeof(struct type) == (size)) ? 1 : -1]

CHECK(MapState, mask, mask, 0xC);
CHECK(MapState, markers, markers, 0x1C);
CHECK(MapState, icons, icons, 0x20);
CHECK(MapState, unk24, unk24, 0x24);
CHECK(MapState, unk30, unk30, 0x30);
CHECK(MapState, marker_count, marker_count, 0xB0);
CHECK(MapState, zoom, zoom, 0xB4);
CHECK(MapState, pan_x, pan_x, 0x104);
CHECK(MapState, level, level, 0x224);
CHECK(MapState, loaded, loaded, 0x228);
CHECK(MapState, hdr, hdr, 0x23C);
CHECK(MapState, map_image_vram, map_image_vram, 0x240);
CHECK(MapState, tex2_vram, tex2_vram, 0x250);
CHECK(MapState, tex0, tex0, 0x258);
CHECK(MapState, slot, slot, 0x278);
CHECK(MapState, slot_id, slot_id, 0x28C);
CHECK(MapState, sel, sel, 0x2A0);
CHECK(MapState, slot_size, slot_size, 0x2A4);
SIZE_CHECK(MapState, 0x2B8);

CHECK(MapIcon, label_text_id, label_text_id, 0x0A);
CHECK(MapIcon, label_item, label_item, 0x0C);
CHECK(MapIcon, x, x, 0x18);
CHECK(MapIcon, active, active, 0x24);
SIZE_CHECK(MapIcon, 0x28);
SIZE_CHECK(MapMarker, 0x10);
