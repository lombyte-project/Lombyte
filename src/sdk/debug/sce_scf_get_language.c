typedef unsigned char u8;

extern void GetOsdConfigParam(int *config);
extern int IsT10K(void);
extern u8 ScfLanguage[1] __asm__("D_001330D4") __attribute__((section(".data")));

int sceScfGetLanguage(void) {
    int config;
    int language;

    GetOsdConfigParam(&config);
    if (IsT10K() != 0) {
        language = ScfLanguage[0];
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
