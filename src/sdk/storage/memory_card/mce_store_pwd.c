#include "types.h"
#include "rnc/data_labels.h"
extern u32 strlen(const char *);
extern void *memcpy(void *, const void *, u32);

#define UNCACHED_PWD ((char *)((u32)D_00159FC0 | 0x20000000))

void mceStorePwd(char *pwd) {
    s32 len;

    if (pwd != 0) {
        len = (strlen(UNCACHED_PWD) < 1024) ? strlen(UNCACHED_PWD) : 1023;
        memcpy(pwd, UNCACHED_PWD, len);
        pwd[len] = 0;
    }
}
