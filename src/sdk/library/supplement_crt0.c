#include "rnc/sdk/library/sdk_state.h"
extern int CreateSema(int *parameters) __asm__("CreateSema");

void SupplementCrt0(void) __asm__("supplement_crt0");

void SupplementCrt0(void) {
    int parameters[16];
    int first;
    int second;

    parameters[1] = 1;
    parameters[2] = 1;
    parameters[9] = 1;
    parameters[10] = 1;
    first = CreateSema(parameters);
    FirstSemaphore = first;
    second = CreateSema(&parameters[8]);
    SecondSemaphore = second;
}
