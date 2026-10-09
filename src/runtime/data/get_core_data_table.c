/* Return the fixed core-data table address used by parser helpers. */

#include "types.h"
#include "rnc/sdk/sce_gs_gparam.h"

void *GetCoreDataTable(void) __asm__("GetCoreDataTable");

void *GetCoreDataTable(void) {
    return &CoreDataTable;
}

sceGsGParam CoreDataTable = {1, 2, 1, 3, 0, 0};
