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
    struct OsdSummerConfig osd_summer_config;
    u8                     summer_time_setting;

    if (IsT10K() != 0) {
        summer_time_setting = t10k_summer_time_setting[0];
    } else {
        GetOsdConfigParam(&osd_summer_config.config);
        if (((osd_summer_config.config >> 13) & 7) == 0) {
            summer_time_setting = 0;
        } else {
            GetOsdConfigParam2(&osd_summer_config.summer, 1, 1);
            summer_time_setting = (osd_summer_config.summer >> 4) & 1;
        }
    }
    return summer_time_setting;
}
