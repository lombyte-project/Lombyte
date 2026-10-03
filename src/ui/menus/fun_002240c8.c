#include "types.h"

extern char D_00186310[];
extern char D_00186F40[];
extern char D_001D5BF0[];
extern char D_001D5DD0[];
extern char D_001D5E10[];
extern char D_001D5E50[];
extern int D_0015FF4C;
extern int D_00224B60[];
extern int func_001E9410();
extern int FUN_00224fc0();
extern int FillTransferWords();
extern int func_00225490();
extern int FUN_00225ac0();
extern s32 clear_preview_animation_queue(void) __asm__("FUN_00226718");
extern s32 select_next_stream_buffer() __asm__("FUN_00225c18");

int initialize_menu_preview_objects(char *preview) __asm__("FUN_002240c8");

int initialize_menu_preview_objects(char *preview) {
    char *object;
    int *p;
    int *object_variables;
    char *held_flag;
    int i, j;

    FUN_00225ac0(1);
    clear_preview_animation_queue();
    {
        unsigned char *g = D_001D5BF0;

        D_0015FF4C = -1;
        *(int *)(g + 0x11C) = -1;
        *(int *)(g + 0x120) = -1;
        *(int *)(g + 0xA0) = select_next_stream_buffer(1);
        *(int *)(g + 0xA4) = select_next_stream_buffer(1);
        g[0xC8] = 0xFF;
        g[0xC9] = 0xFF;
        g[0xCA] = 0;
        p = (int *)(g + 0xB0);
        for (i = 2; i >= 0; i--) {
            *p = select_next_stream_buffer(0);
            p++;
        }
    }
    {
        char *g = D_001D5BF0;

        D_001D5DD0[1] = 0;
        *(int *)(g + 0x1C) = -1;
        D_001D5E10[1] = 0;
        D_001D5E50[1] = 0;
    }
    object = func_00225490(0);
    held_flag = preview + 0xBB;
    for (j = 23; j >= 0; j--) {
        *held_flag = 0;
        held_flag--;
    }
    if (object != 0) {
        char *camera = D_00186F40;
        char *g;

        *(char **)(preview + 0x44) = object;
        *(short *)(object + 0x34) = 0;
        *(float *)(object + 0x10) = *(float *)(camera + 0x140) + 4.0f;
        *(float *)(object + 0x14) = *(float *)(camera + 0x144);
        *(float *)(object + 0x18) = *(float *)(camera + 0x148) - 0.6f;
        *(float *)(object + 0x48) = 3.1415927f;
        *(void **)(object + 0x74) = D_00224B60;
        object_variables = *(int **)(object + 0x78);
        object_variables[0] = (int)preview;
        object_variables[1] = 0;
        object_variables[2] = 0;
        g = D_001D5BF0;
        *(int *)(g + 0xC0) = -1;
        FillTransferWords(D_00186310, 0, 0x40);
        func_001E9410(object);
    }
    object = func_00225490(0x259);
    if (object != 0) {
        **(int **)(object + 0x78) = (int)preview;
        *(void **)(object + 0x74) = FUN_00224fc0;
        *(short *)(object + 0x34) = 4;
    }
    *(char **)(preview + 0x48) = object;
    return 0;
}

extern __typeof__(initialize_menu_preview_objects) func_002240C8 __attribute__((alias("FUN_002240c8")));
