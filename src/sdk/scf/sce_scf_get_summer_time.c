#include "types.h"

extern u8   t10k_summer_time_setting[] __asm__("D_001330D6");
extern void GetOsdConfigParam();
extern s32  GetOsdConfigParam2();
extern s32  IsT10K();

struct OsdSummerConfig {
    u32 config;
    u8  summer;
};

u8 sceScfGetSummerTime(void) {
    struct OsdSummerConfig work;
    u8                     result;

    if (IsT10K() != 0) {
        result = t10k_summer_time_setting[0];
    } else {
        GetOsdConfigParam(&work.config);
        if (((work.config >> 13) & 7) == 0) {
            result = 0;
        } else {
            GetOsdConfigParam2(&work.summer, 1, 1);
            result = (work.summer >> 4) & 1;
        }
    }
    return result;
}
