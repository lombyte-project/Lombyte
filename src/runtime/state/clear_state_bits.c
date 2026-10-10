/* Clear the active field and reset its associated state word. */

typedef unsigned int u32;

void fun_0011ad90(u32 *p) __asm__("func_0011AD90");

void fun_0011ad90(u32 *p) {
    u32 v = p[4];
    p[6] = 0;
    p[4] = v & 0xfffffffeu;
}
