#include "types.h"
#include "rnc/ui/menus/menu_system.h"

extern char D_00186310[];
extern char D_00186F40[];
extern char D_001D5DD0[];
extern char D_001D5E10[];
extern char D_001D5E50[];
extern int D_0015FF4C;
extern void FUN_00224b60();
extern int func_001E9410();
extern int update_menu_preview_animation_pose() __asm__("FUN_00224fc0");
extern int FillTransferWords();
extern int create_menu_preview_moby() __asm__("func_00225490");
extern int initialize_graphics_buffer_descriptors() __asm__("FUN_00225ac0");
extern s32 clear_preview_animation_queue(void) __asm__("FUN_00226718");
extern s32 select_next_stream_buffer() __asm__("FUN_00225c18");

int initialize_menu_preview_objects(char *preview) __asm__("FUN_002240c8");

int initialize_menu_preview_objects(char *preview) {
    char *object;
    int *p;
    int *object_variables;
    char *held_flag;
    int i, j;

    initialize_graphics_buffer_descriptors(1);
    clear_preview_animation_queue();
    {
        struct MenuSystem *g = &menu_system;

        D_0015FF4C = -1;
        g->unk11C = -1;
        g->unk120 = -1;
        g->stream_buffer[0] = select_next_stream_buffer(1);
        g->stream_buffer[1] = select_next_stream_buffer(1);
        g->loaded_animation[0] = 0xFF;
        g->loaded_animation[1] = 0xFF;
        g->read_buffer_index = 0;
        p = g->unkB0;
        for (i = 2; i >= 0; i--) {
            *p = select_next_stream_buffer(0);
            p++;
        }
    }
    {
        struct MenuSystem *g = &menu_system;

        D_001D5DD0[1] = 0;
        g->current_gadget = -1;
        D_001D5E10[1] = 0;
        D_001D5E50[1] = 0;
    }
    object = create_menu_preview_moby(0);
    held_flag = preview + 0xBB;
    for (j = 23; j >= 0; j--) {
        *held_flag = 0;
        held_flag--;
    }
    if (object != 0) {
        char *camera = D_00186F40;
        struct MenuSystem *g;

        *(char **)(preview + 0x44) = object;
        *(short *)(object + 0x34) = 0;
        *(float *)(object + 0x10) = *(float *)(camera + 0x140) + 4.0f;
        *(float *)(object + 0x14) = *(float *)(camera + 0x144);
        *(float *)(object + 0x18) = *(float *)(camera + 0x148) - 0.6f;
        *(float *)(object + 0x48) = 3.1415927f;
        *(void **)(object + 0x74) = FUN_00224b60;
        object_variables = *(int **)(object + 0x78);
        object_variables[0] = (int)preview;
        object_variables[1] = 0;
        object_variables[2] = 0;
        g = &menu_system;
        g->unkC0 = -1;
        FillTransferWords(D_00186310, 0, 0x40);
        func_001E9410(object);
    }
    object = create_menu_preview_moby(0x259);
    if (object != 0) {
        **(int **)(object + 0x78) = (int)preview;
        *(void **)(object + 0x74) = update_menu_preview_animation_pose;
        *(short *)(object + 0x34) = 4;
    }
    *(char **)(preview + 0x48) = object;
    return 0;
}

extern __typeof__(initialize_menu_preview_objects) func_002240C8
    __attribute__((alias("FUN_002240c8")));
