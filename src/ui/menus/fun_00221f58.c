/* Ported from rac1-decomp (src/game/pause.c, func_00222FA8). */
extern void setup_gif_paging(int) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern void func_001F68E8_c(int, int, long, void *, int) __asm__("func_001F6530");
extern void func_001F68E8_c(int, int, long, void *, int) __asm__("func_001F6530");
extern int get_icon_frame(int, int) __asm__("func_001FF960");
typedef struct {
    char c[2];
} Glyph2;
extern char D_001602F0[];
extern char D_001602F8[];
extern int get_frame_texture(int) __asm__("func_001FFA10");
extern void draw_rotated_sprite(float, float, int, int, int, float, float,
                                float) __asm__("func_00200600");
/* Draws the glyph "\x10" (menu->0x38 set) or "\x11" and a gauge sprite
   below it, rotated by pi in the second case. The glyph string is a
   2-byte char struct copied onto the stack (retail's lb/lb/sb/sb). */
int FUN_00221f58(char *menu) {
    Glyph2 buf;

    setup_gif_paging(0);
    if (*(int *)(menu + 0x38) != 0) {
        buf = *(Glyph2 *)D_001602F0;
        func_001F68E8_c(4, *(int *)(menu + 0x24) / 2 - 8, 0x80FFA888L, &buf, -1);
        draw_rotated_sprite(640.0f, (float)(*(int *)(menu + 0x24) << 3), 0x20, 0x10,
                            get_frame_texture(get_icon_frame(0xE99E, 6)), 128.0f,
                            256.0f, 0.0f);
    } else {
        buf = *(Glyph2 *)D_001602F8;
        func_001F68E8_c(*(int *)(menu + 0x20) - 0x18, *(int *)(menu + 0x24) / 2 - 8, 0x80FFA888L,
                        &buf, -1);
        draw_rotated_sprite(192.0f, (float)(*(int *)(menu + 0x24) << 3), 0x20, 0x10,
                            get_frame_texture(get_icon_frame(0xE99E, 6)), 128.0f,
                            256.0f, 3.14159274f);
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(FUN_00221f58) func_00221F58 __attribute__((alias("FUN_00221f58")));
