/* Menus: builds the reversed list of active messages. */
#include "types.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/ui/help/message_state.h"

typedef struct {
    s16 a;
    s16 b;
    s32 c;
    s32 d;
} E_27adb0;

extern u16      message_list_value_lookup[] __asm__("D_L00_00179452");
extern E_27adb0 D_L00_001B3D40[];
/* Fills D_L00_001B3D40 from the D_00141E08 table in reverse order, then ends it with 0. */
s32 build_reversed_active_message_list(void) __asm__("FUN_L00_0027adb0");

s32 build_reversed_active_message_list(void) {
    s32 i, j;
    for (i = 0; i < active_message_record_count; i = j) {
        j = i + 1;
        D_L00_001B3D40[i].b = 1;
        D_L00_001B3D40[i].a = *(u16 *)((char *)message_list_value_lookup +
                                       active_message_records[active_message_record_count - j] * 4);
    }
    D_L00_001B3D40[*(volatile s32 *)&active_message_record_count].a = 0;
    return 0;
}

E_27adb0 D_L01_001B41C0 = {0x5277, 1, 0, 0};

E_27adb0 D_L02_001B4340 = {0x5277, 1, 0, 0};

E_27adb0 D_L03_001B3E40 = {0x5277, 1, 0, 0};

E_27adb0 D_L04_001B3EC0 = {0x5277, 1, 0, 0};

E_27adb0 D_L05_001B41C0 = {0x5277, 1, 0, 0};

E_27adb0 D_L06_001B44C0 = {0x5277, 1, 0, 0};

E_27adb0 D_L07_001B3DC0 = {0x5277, 1, 0, 0};

E_27adb0 D_L08_001B4540 = {0x5277, 1, 0, 0};

E_27adb0 D_L09_001B3EC0 = {0x5277, 1, 0, 0};

E_27adb0 D_L10_001B41C0 = {0x5277, 1, 0, 0};

E_27adb0 D_L11_001B4740 = {0x5277, 1, 0, 0};

E_27adb0 D_L12_001B41C0 = {0x5277, 1, 0, 0};

E_27adb0 D_L13_001B4040 = {0x5277, 1, 0, 0};

E_27adb0 D_L14_001B4440 = {0x5277, 1, 0, 0};

E_27adb0 D_L15_001B4340 = {0x5277, 1, 0, 0};

E_27adb0 D_L16_001B41C0 = {0x5277, 1, 0, 0};

E_27adb0 D_L17_001B4640 = {0x5277, 1, 0, 0};

E_27adb0 D_L18_001B4740 = {0x5277, 1, 0, 0};
