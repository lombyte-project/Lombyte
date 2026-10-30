/* Text: prints text with effect texture 3. */
#include "types.h"
#include "asm.h"

#include "sda.h"

extern char D_L00_001C3890[] NOT_SDA;
extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void font_print(int, int, int, int, int, int, void *) __asm__("FUN_001f62b0");

/* font_print with effect texture 3 and D_L00_001C3890. */
void print_text_with_texture_3(int x, int y, int color, int text, int character_limit) __asm__("FUN_L00_001fb470");

void print_text_with_texture_3(int a, int b, int c, int d, int e) __asm__("FUN_L00_001fb470");

void print_text_with_texture_3(int x, int y, int color, int text, int character_limit) {
    font_print(x, y, color, text, character_limit, get_effect_texture(3), D_L00_001C3890);
}

char D_L01_001C3D10 NOT_SDA = 0;
char D_L02_001C3E90 NOT_SDA = 0;
char D_L03_001C3990 NOT_SDA = 0;
char D_L04_001C3A10 NOT_SDA = 0;
char D_L05_001C3D10 NOT_SDA = 0;
char D_L06_001C4010 NOT_SDA = 0;
char D_L07_001C3910 NOT_SDA = 0;
char D_L08_001C4090 NOT_SDA = 0;
char D_L09_001C3A10 NOT_SDA = 0;
char D_L10_001C3D10 NOT_SDA = 0;
char D_L11_001C4290 NOT_SDA = 0;
char D_L12_001C3D10 NOT_SDA = 0;
char D_L13_001C3B90 NOT_SDA = 0;
char D_L14_001C3F90 NOT_SDA = 0;
char D_L15_001C3E90 NOT_SDA = 0;
char D_L16_001C3D10 NOT_SDA = 0;
char D_L17_001C4190 NOT_SDA = 0;
char D_L18_001C4290 NOT_SDA = 0;
