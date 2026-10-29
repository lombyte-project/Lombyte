extern int IsT10K(void);
extern void GetOsdConfigParam(int *config);
extern short D_001330D0[] __attribute__((section(".data")));

int sceScfGetTimeZone(void) {
    int config;
    int timezone;

    if (IsT10K() != 0) {
        timezone = D_001330D0[0];
    } else {
        GetOsdConfigParam(&config);
        timezone = config >> 21;
        if ((((unsigned int)config >> 13) & 7) == 0) {
            timezone = 0x21C;
        }
    }
    return timezone;
}
