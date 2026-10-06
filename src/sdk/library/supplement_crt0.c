extern int CreateSema(int *parameters) __asm__("CreateSema");
extern int FirstSemaphore __asm__("D_00130320") __attribute__((section(".data")));
extern int SecondSemaphore __asm__("D_00130324") __attribute__((section(".data")));

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
