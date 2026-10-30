#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/weapons/hand_gadget.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/ui/menus/item_preview/clear_preview_animation_queue.h"

extern u8   D_00186310[];
extern int  active_class_resource_index __asm__("D_0015FF4C");
extern void gadget_moby_update_noop() __asm__("FUN_00224b60");
extern int  noop_callback_e() __asm__("func_001E9410");
extern int  update_menu_preview_animation_pose() __asm__("FUN_00224fc0");
extern int  fill_transfer_words() __asm__("func_001F97E8");
extern int  create_menu_preview_moby() __asm__("func_00225490");
extern int  initialize_menu_stream_buffers() __asm__("FUN_00225ac0");
extern s32  acquire_menu_stream_buffer() __asm__("FUN_00225c18");

int initialize_menu_preview_objects(char *preview) __asm__("FUN_002240c8");

int initialize_menu_preview_objects(char *preview) {
    char *preview_moby;
    int  *resource_buffer_slot;
    int  *object_variables;
    char *held_flag;
    int   i, j;

    initialize_menu_stream_buffers(1);
    clear_preview_animation_queue();
    {
        struct MenuSystem *g = &menu_system;

        active_class_resource_index = -1;
        g->unk11C = -1;
        g->unk120 = -1;
        g->stream_buffer[0] = acquire_menu_stream_buffer(1);
        g->stream_buffer[1] = acquire_menu_stream_buffer(1);
        g->loaded_animation[0] = 0xFF;
        g->loaded_animation[1] = 0xFF;
        g->read_buffer_index = 0;
        resource_buffer_slot = g->unkB0;
        for (i = 2; i >= 0; i--) {
            *resource_buffer_slot = acquire_menu_stream_buffer(0);
            resource_buffer_slot++;
        }
    }
    {
        struct MenuSystem *g = &menu_system;

        class_pose_manipulator.active = 0;
        g->current_gadget = -1;
        first_attachment_manipulator.active = 0;
        second_attachment_manipulator.active = 0;
    }
    preview_moby = create_menu_preview_moby(0);
    held_flag = preview + 0xBB;
    for (j = 23; j >= 0; j--) {
        *held_flag = 0;
        held_flag--;
    }
    if (preview_moby != 0) {
        char              *camera = (char *)&preview_camera;
        struct MenuSystem *g;

        *(char **)(preview + 0x44) = preview_moby;
        *(short *)(preview_moby + 0x34) = 0;
        *(float *)(preview_moby + 0x10) = *(float *)(camera + 0x140) + 4.0f;
        *(float *)(preview_moby + 0x14) = *(float *)(camera + 0x144);
        *(float *)(preview_moby + 0x18) = *(float *)(camera + 0x148) - 0.6f;
        *(float *)(preview_moby + 0x48) = 3.1415927f;
        *(void **)(preview_moby + 0x74) = gadget_moby_update_noop;
        object_variables = *(int **)(preview_moby + 0x78);
        object_variables[0] = (int)preview;
        object_variables[1] = 0;
        object_variables[2] = 0;
        g = &menu_system;
        g->unkC0 = -1;
        fill_transfer_words(D_00186310, 0, 0x40);
        noop_callback_e(preview_moby);
    }
    preview_moby = create_menu_preview_moby(0x259);
    if (preview_moby != 0) {
        **(int **)(preview_moby + 0x78) = (int)preview;
        *(void **)(preview_moby + 0x74) = update_menu_preview_animation_pose;
        *(short *)(preview_moby + 0x34) = 4;
    }
    *(char **)(preview + 0x48) = preview_moby;
    return 0;
}

extern __typeof__(initialize_menu_preview_objects) func_002240C8
    __attribute__((alias("FUN_002240c8")));
