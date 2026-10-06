/* Query the Deci2 module and return the version written to its work buffer. */

extern unsigned char RpcQueueData[1] __asm__("D_0015B008") __attribute__((section(".data")));
extern unsigned char RpcReceiveData[1] __asm__("D_0015B080") __attribute__((section(".data")));

extern int SceSifCallRpc(void *, unsigned int, int, void *, int, void *, int, void *,
                         void *) __asm__("sceSifCallRpc");

int SceDbcGetModVersion(void) __asm__("sceDbcGetModVersion");

int SceDbcGetModVersion(void) {
    SceSifCallRpc(&RpcQueueData[0], 0x80000963, 0, &RpcReceiveData[0], 0x400, &RpcReceiveData[0],
                  0x400, 0, 0);
    return *(int *)&RpcReceiveData[0];
}
