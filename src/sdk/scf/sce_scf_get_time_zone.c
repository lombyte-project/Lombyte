#include "sda.h"
#include "rnc/sdk/scf.h"

extern void  GetOsdConfigParam(int *config);
extern short t10k_time_zone __asm__("D_001330D0") NOT_SDA;

int sceScfGetTimeZone(void) {
    int config;
    int timezone;

    if (IsT10K() != 0) {
        timezone = t10k_time_zone;
    } else {
        GetOsdConfigParam(&config);
        timezone = config >> 21;
        if ((((unsigned int)config >> 13) & 7) == 0) {
            timezone = 0x21C;
        }
    }
    return timezone;
}

/* The T10K's time zone, in minutes from UTC (540: UTC+9), like the fallback above. */
short t10k_time_zone __asm__("D_001330D0") NOT_SDA = 0x21C;
