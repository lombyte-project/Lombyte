/* Clear the active field and reset its associated state word. */

typedef unsigned int u32;

void fun_0011ad90(u32 *p) __asm__("func_0011AD90");

void fun_0011ad90(u32 *packet) {
    u32 v = packet[4];
    packet[6] = 0;
    packet[4] = v & 0xfffffffeu;
}
