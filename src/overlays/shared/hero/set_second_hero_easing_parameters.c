/* Hero easing: sets the second, third and fourth hero ease, and clears the mapped moby animation state. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"

extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
/* Sets the accel and max_speed of one hero ease. */
void set_second_hero_easing_parameters(float a, float b) __asm__("FUN_L00_0020a1f8");

void set_second_hero_easing_parameters(float a, float b) {
    struct HeroEase *destination = hero_eases;
    destination[1].accel = a;
    destination[1].max_speed = b;
}

extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
/* Sets the accel and max_speed of one hero ease. */
void set_third_hero_easing_parameters(float a, float b) __asm__("FUN_L00_0020a210");

void set_third_hero_easing_parameters(float a, float b) {
    struct HeroEase *destination = hero_eases;
    destination[2].accel = a;
    destination[2].max_speed = b;
}

extern struct HeroEase hero_eases[] __asm__("D_L00_0017A680");
/* Sets the accel and max_speed of one hero ease. */
void set_fourth_hero_easing_parameters(float a, float b) __asm__("FUN_L00_0020a228");

void set_fourth_hero_easing_parameters(float a, float b) {
    struct HeroEase *destination = hero_eases;
    destination[3].accel = a;
    destination[3].max_speed = b;
}

extern int D_0013F350_002321a8[] __asm__("D_0013F350");
void       clear_hero_mapped_moby_animation_state(void) __asm__("FUN_L00_002321a8");

void clear_hero_mapped_moby_animation_state(void) {
    int *destination = D_0013F350_002321a8;
    destination[0x2278 / 4] = 0;
    destination[0x227C / 4] = 0;
}

f32 D_L00_0017A7D4[1] __attribute__((section(".data"))) = {0};

f32 D_L00_0017A884[1] __attribute__((section(".data"))) = {0};

f32 D_L00_0017A934[1] __attribute__((section(".data"))) = {0};

f32 D_L01_0017AC54[1] __attribute__((section(".data"))) = {0};

f32 D_L01_0017AD04[1] __attribute__((section(".data"))) = {0};

f32 D_L01_0017ADB4[1] __attribute__((section(".data"))) = {0};

f32 D_L02_0017ADD4[1] __attribute__((section(".data"))) = {0};

f32 D_L02_0017AE84[1] __attribute__((section(".data"))) = {0};

f32 D_L02_0017AF34[1] __attribute__((section(".data"))) = {0};

f32 D_L03_0017A8D4[1] __attribute__((section(".data"))) = {0};

f32 D_L03_0017A984[1] __attribute__((section(".data"))) = {0};

f32 D_L03_0017AA34[1] __attribute__((section(".data"))) = {0};

f32 D_L04_0017A954[1] __attribute__((section(".data"))) = {0};

f32 D_L04_0017AA04[1] __attribute__((section(".data"))) = {0};

f32 D_L04_0017AAB4[1] __attribute__((section(".data"))) = {0};

f32 D_L05_0017AC34[1] __attribute__((section(".data"))) = {0};

f32 D_L05_0017ACE4[1] __attribute__((section(".data"))) = {0};

f32 D_L05_0017AD94[1] __attribute__((section(".data"))) = {0};

f32 D_L06_0017AF54[1] __attribute__((section(".data"))) = {0};

f32 D_L06_0017B004[1] __attribute__((section(".data"))) = {0};

f32 D_L07_0017A854[1] __attribute__((section(".data"))) = {0};

f32 D_L07_0017A904[1] __attribute__((section(".data"))) = {0};

f32 D_L07_0017A9B4[1] __attribute__((section(".data"))) = {0};

f32 D_L08_0017AFD4[1] __attribute__((section(".data"))) = {0};

f32 D_L09_0017A954[1] __attribute__((section(".data"))) = {0};

f32 D_L09_0017AA04[1] __attribute__((section(".data"))) = {0};

f32 D_L09_0017AAB4[1] __attribute__((section(".data"))) = {0};

f32 D_L10_0017AC54[1] __attribute__((section(".data"))) = {0};

f32 D_L10_0017AD04[1] __attribute__((section(".data"))) = {0};

f32 D_L10_0017ADB4[1] __attribute__((section(".data"))) = {0};

f32 D_L12_0017AC54[1] __attribute__((section(".data"))) = {0};

f32 D_L12_0017AD04[1] __attribute__((section(".data"))) = {0};

f32 D_L12_0017ADB4[1] __attribute__((section(".data"))) = {0};

f32 D_L13_0017AAD4[1] __attribute__((section(".data"))) = {0};

f32 D_L13_0017AB84[1] __attribute__((section(".data"))) = {0};

f32 D_L13_0017AC34[1] __attribute__((section(".data"))) = {0};

f32 D_L14_0017AED4[1] __attribute__((section(".data"))) = {0};

f32 D_L14_0017AF84[1] __attribute__((section(".data"))) = {0};

f32 D_L14_0017B034[1] __attribute__((section(".data"))) = {0};

f32 D_L15_0017ADD4[1] __attribute__((section(".data"))) = {0};

f32 D_L15_0017AE84[1] __attribute__((section(".data"))) = {0};

f32 D_L15_0017AF34[1] __attribute__((section(".data"))) = {0};

f32 D_L16_0017AC34[1] __attribute__((section(".data"))) = {0};

f32 D_L16_0017ACE4[1] __attribute__((section(".data"))) = {0};

f32 D_L16_0017AD94[1] __attribute__((section(".data"))) = {0};
