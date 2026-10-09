#include "rnc/sdk/library/sdk_state.h"
#include "sda.h"
#include "types.h"

extern void GetOsdConfigParam(int *config);
extern int IsT10K(void);

int sceScfGetLanguage(void) {
    int config;
    int language;

    GetOsdConfigParam(&config);
    if (IsT10K() != 0) {
        language = ScfLanguage;
    } else {
        GetOsdConfigParam(&config);
        if ((((unsigned int)config >> 13) & 7) == 0) {
            language = ((unsigned int)config >> 4) & 1;
        } else {
            language = ((unsigned int)config >> 16) & 0x1F;
        }
    }
    return language;
}

u8 ScfLanguage NOT_SDA = {0};
