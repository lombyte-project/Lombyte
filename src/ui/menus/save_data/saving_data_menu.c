
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef volatile s8 vs8;
typedef volatile u8 vu8;
typedef volatile s16 vs16;
typedef volatile u16 vu16;
typedef volatile s32 vs32;
typedef volatile u32 vu32;
typedef volatile s64 vs64;
typedef volatile u64 vu64;
typedef float f32;
typedef double f64;
typedef s32 b32;
struct PadState {
    u8 pad_0[0x1A4];
    s32 unk1A4;
    u8 pad_1A8[0xC];
    s32 unk1B4;
    u8 pad_1B8[0xC];
    s32 unk1C4;
};
struct SaveSlotTable {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x8];
    s32 unk14;
    u8 pad_18[0xBC];
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
    u8 pad_E0[0x4];
    s32 unkE4;
};
struct Globals_001D5BF0 {
    u8 pad_0[0x4];
    struct ModeEntry *unk4;
    s32 unk8;
    u8 pad_C[0xC4];
    struct ModeTarget *unkD0;
    s32 unkD4;
    u8 pad_D8[0x4C];
    s32 unk124;
    s32 unk128;
    s32 unk12C;
};
struct ModeEntry {
    u8 pad_0[0x38];
    s32 unk38;
    u8 pad_3C[0x4];
    s32 unk40;
};
struct ModeTarget {
    u8 pad_0[0x84];
    s32 unk84;
};
struct MenuScreen {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x18];
    s32 unk30;
    u8 pad_34[0xC];
    s32 unk40;
    u8 pad_44[0x4];
    s32 unk48;
    s32 unk4C;
};
typedef struct __attribute__((packed)) {
    s64 v;
} Unaligned64;
extern struct PadState D_0013C940;
extern Unaligned64 D_0015EE98[1];
extern struct SaveSlotTable D_0013D290;
extern s32 D_0015ED84;
extern s32 D_0015ED98;
extern s32 D_0015EE20;
extern s32 D_0015EE24;
extern s32 D_0015EE34;
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
extern u8 D_001D2578[];
extern s32 D_001D2640[];
extern struct Globals_001D5BF0 D_001D5BF0;
extern void mode_freeze_init() __asm__("func_001FBAB8");
extern s32 prepare_save_game() __asm__("FUN_002269c0");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 saving_data_menu(struct MenuScreen *menu) __asm__("FUN_00222f88");

s32 saving_data_menu(struct MenuScreen *menu) {
    u8 *new_var;
    s32 *temp_2_126;
    s32 prev_slot;
    s32 save_state;
    s32 slot;
    s32 selected_slot;
    s32 pad_buttons;
    if (D_001D5BF0.unk4->unk40 == menu) {
        if (((menu->unk4C == 0) && (D_001D5BF0.unkD0 == D_001D2578)) &&
            (D_001D5BF0.unkD0->unk84 != 0)) {
            menu->unk4C = 1;
        }
        save_state = menu->unk4C;
        if (save_state == 1) {
            prepare_save_game(menu->unk48, menu->unk40);
            D_001D5BF0.unk128 = save_state;
            D_001D5BF0.unk12C = 0x4FB5;
        }
        menu->unk4C = 2;
        prev_slot = menu->unk40;
        if (D_001D5BF0.unk128 != 0) {
            if ((D_0013D290.unkD4 < 3) && (D_0013D290.unkDC < 0)) {
                D_001D5BF0.unk128 = 0;
                if (D_0013D290.unkE4 != 0) {
                    D_0015EEB4 |= 0x80;
                    mode_freeze_init(3, D_001D5BF0.unk4);
                    goto block_46;
                }
                *((s32 *)((((u8 *)(&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x24)) =
                    (s32)D_0015ED98;
                *((s32 *)((((u8 *)(&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x20)) =
                    (s32)D_0015ED84;
                *((s32 *)((((u8 *)(&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x2C)) =
                    (s32)D_0015EE24;
                *((Unaligned64 *)((((u8 *)(&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x30)) =
                    D_0015EE98[0];
                *((s32 *)((((u8 *)(&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x28)) =
                    (s32)D_0015EE20;
                goto block_13;
            }
            return 0;
        }
    block_13:
        if (0xD00 & D_0013C940.unk1C4) {
            if (D_001D5BF0.unk124 == 0) {
                return 1;
            }
            goto block_17;
        }

    block_17:
        if (D_0013C940.unk1C4 & 0x10) {
            temp_2_126 = D_001D5BF0.unk4->unk38;
            if (temp_2_126 != 0) {
                D_001D5BF0.unk8 = temp_2_126;
                goto block_23;
            }
            if (D_001D5BF0.unk124 == 0) {
                return -1;
            }
            goto block_24;
        }

    block_23:
    block_24:
        if ((D_0015EEB0 != 0x10) && (D_0015EEB0 != 1)) {
            D_001D5BF0.unk8 = (void *)D_001D5BF0.unk4->unk38;
            return 0;
        }

        if (((D_0013D290.unkD4 < 3) && (D_0013D290.unkDC < 0)) && (D_0013D290.unk8 == 2)) {
            if (menu->unk30 & 1) {
                pad_buttons = D_0013C940.unk1B4;
            } else {
                pad_buttons = D_0013C940.unk1A4;
            }
            menu->unk40 = (s32)D_0015EE34;
            if ((pad_buttons & 0x1000) && (D_0015EE34 != 0)) {
                menu->unk40 = (s32)(D_0015EE34 - 1);
            }
            if (pad_buttons & 0x4000) {
                slot = menu->unk40;
                if (slot < 4) {
                    menu->unk40 = (s32)(slot + 1);
                }
            }
            selected_slot = menu->unk40;
            D_0015EE34 = selected_slot;
            if ((pad_buttons & 0x40) && (D_0013D290.unk8 == 2)) {
                new_var = (((u8 *)(&D_0013D290)) + (selected_slot * 0x1C)) + 0x20;
                if ((*((s32 *)new_var)) != (-1)) {
                    D_001D5BF0.unkD4 = 0;
                    D_001D5BF0.unk8 = (new_var = D_001D2578);
                    D_001D2640[0] = menu->unk40;
                } else {
                    menu->unk4C = 1;
                }
            }
            if (menu->unk40 != prev_slot) {
                allocate_voice_for_target_entry(1, 0x11, menu->unk14);
            }
        block_46:;

            ;
        }
        return 0;
    }
    return 0;
}

extern __typeof__(saving_data_menu) func_00222F88 __attribute__((alias("FUN_00222f88")));
