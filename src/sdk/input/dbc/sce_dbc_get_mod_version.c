#include "rnc/sdk/library/sdk_globals.h"
#include "sda.h"
/* Query the Deci2 module and return the version written to its work buffer. */

extern unsigned char RpcReceiveData[1] __asm__("D_0015B080") NOT_SDA;

extern int SceSifCallRpc(
    void *, unsigned int, int, void *, int, void *, int, void *, void *) __asm__("sceSifCallRpc");

int SceDbcGetModVersion(void) __asm__("sceDbcGetModVersion");

int SceDbcGetModVersion(void) {
    SceSifCallRpc(
        &RpcQueueData, 0x80000963, 0, &RpcReceiveData[0], 0x400, &RpcReceiveData[0], 0x400, 0, 0);
    return *(int *)&RpcReceiveData[0];
}
