#include "types.h"
#include "asm.h"

#include "types.h"
#include "qcopy.h"
#include "rnc/rendering/rotated_sprite_queue.h"

extern s32 camera_secondary_mode[] __asm__("D_001413D4");
extern s32 screen_offsets[] __asm__("D_0013E500");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void project_to_screen(f32 *, void *) __asm__("func_001F2070");
extern void spawn_particle_burst(void *, f32, f32) __asm__("func_001EE008");

/* Draws every queued sprite with a resource outside states 0xFE and 0xFD:
   at its projected screen position (func_001F2070, then relative to the
   D_0013E500 offset in 1/16 units) or, for unprojected ones, at the
   screen centre; then empties the queue. Camera mode 0x72 discards it. */
void render_queued_rotated_sprites(void) __asm__("FUN_001ee338");

void render_queued_rotated_sprites(void) {
    f32 screen_center[4];
    f32 screen_position[4];
    s32 record_index;

    if (camera_secondary_mode[0] == 0x72) {
        rotated_sprite_queue.count = 0;
    }
    if (rotated_sprite_queue.count != 0) {
        screen_center[0] = convert_integer_to_float(screen_offsets[2]);
        screen_center[1] = convert_integer_to_float(screen_offsets[3]);
        for (record_index = 0; record_index < rotated_sprite_queue.count; record_index++) {
            QueuedRotatedSprite *record = &rotated_sprite_queue.records[record_index];
            s32 resource_state;

            if (record->resource == 0) {
                continue;
            }
            resource_state = record->resource[0x20];
            if (resource_state == 0xFE) {
                continue;
            }
            if (resource_state == 0xFD) {
                continue;
            }
            if (record->project_position != 0) {
                project_to_screen(screen_position, record);
                screen_position[0] =
                    (screen_position[0] - convert_integer_to_float(screen_offsets[4])) * 0.0625f;
                screen_position[1] =
                    (screen_position[1] - convert_integer_to_float(screen_offsets[5])) * 0.0625f;
            } else {
                qcopy(screen_position, screen_center);
            }
            spawn_particle_burst(record, screen_position[0], screen_position[1]);
        }
        rotated_sprite_queue.count = 0;
    }
}
extern __typeof__(render_queued_rotated_sprites) func_001EE338
    __attribute__((alias("FUN_001ee338")));
