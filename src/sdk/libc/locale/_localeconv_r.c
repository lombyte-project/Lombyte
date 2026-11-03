#include "types.h"
/* newlib _localeconv_r: the one static struct lconv. */

#define CHAR_MAX 0x7F

struct lconv {
    const char *decimal_point;
    const char *thousands_sep;
    const char *grouping;
    const char *int_curr_symbol;
    const char *currency_symbol;
    const char *mon_decimal_point;
    const char *mon_thousands_sep;
    const char *mon_grouping;
    const char *positive_sign;
    const char *negative_sign;
    char        int_frac_digits;
    char        frac_digits;
    char        p_cs_precedes;
    char        p_sep_by_space;
    char        n_cs_precedes;
    char        n_sep_by_space;
    char        p_sign_posn;
    char        n_sign_posn;
};

int *_localeconv_r(void *state_resource) __asm__("func_001144D8");

extern const char lconv_empty_string[] __asm__("D_001523A0");
extern const char lconv_decimal_point[] __asm__("D_001523A8");

/* The "C" locale: only the decimal point is set. */
const struct lconv lconv __asm__("D_00152370") = {
    lconv_decimal_point,
    lconv_empty_string,
    lconv_empty_string,
    lconv_empty_string,
    lconv_empty_string,
    lconv_empty_string,
    lconv_empty_string,
    lconv_empty_string,
    lconv_empty_string,
    lconv_empty_string,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
};

/* Its strings, in .rodata like the literals they were (they would go to .sdata). */
const char lconv_empty_string[] __asm__("D_001523A0") __attribute__((section(".rodata"))) = "";
const char lconv_decimal_point[] __asm__("D_001523A8") __attribute__((section(".rodata"))) = ".";

int *_localeconv_r(void *state_resource) {
    return (int *)&lconv;
}
