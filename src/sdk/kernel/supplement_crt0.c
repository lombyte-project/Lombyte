#include "rnc/sdk/library/sdk_state.h"
#include "sda.h"
#include "types.h"

extern int CreateSema(int *parameters) __asm__("CreateSema");

void SupplementCrt0(void) __asm__("supplement_crt0");

void SupplementCrt0(void) {
    int parameters[16];
    int first_semaphore_id;
    int second_semaphore_id;

    parameters[1] = 1;
    parameters[2] = 1;
    parameters[9] = 1;
    parameters[10] = 1;
    first_semaphore_id = CreateSema(parameters);
    FirstSemaphore = first_semaphore_id;
    second_semaphore_id = CreateSema(&parameters[8]);
    SecondSemaphore = second_semaphore_id;
}

s32 FirstSemaphore NOT_SDA = 0;

s32 SecondSemaphore NOT_SDA = 0;
