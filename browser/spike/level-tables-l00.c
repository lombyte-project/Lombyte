/* Veldin's handler tables, for the spike build only.

   The level program's vtbl, camvtbl and sndvtbl records hold console code addresses. The
   executable's code looks handlers up in them through executable-named globals that the RAM image
   pins at those records, so after the level program data is copied in, an indirect call would use
   a console address as a WebAssembly table index. src/overlays/l00/level_class_tables.c defines
   the same tables in C; compiled here, their entries are WebAssembly function references.
   spike_install_level_tables copies the C tables over the loaded records, so both views agree.

   Entry layouts are 32-bit words and pointers on both targets. The record sizes come from
   config/us/overlays/level_00/manifest.json and the element counts from level_class_tables.c. */
#include "types.h"

extern u32 l00_moby_classes[101 * 3] __asm__("D_L00_001EA300");
extern u32 l00_default_moby_methods[6] __asm__("D_L00_001EA7BC");
extern u32 l00_camera_types[8 * 5] __asm__("D_L00_001EA880");
extern u32 l00_sound_handlers[3 * 2] __asm__("D_L00_001EA980");

void spike_install_level_tables(void) {
    __builtin_memcpy((void *)0x001EA300, l00_moby_classes, sizeof(l00_moby_classes));
    __builtin_memcpy((void *)0x001EA7BC, l00_default_moby_methods, sizeof(l00_default_moby_methods));
    __builtin_memcpy((void *)0x001EA880, l00_camera_types, sizeof(l00_camera_types));
    __builtin_memcpy((void *)0x001EA980, l00_sound_handlers, sizeof(l00_sound_handlers));
}
