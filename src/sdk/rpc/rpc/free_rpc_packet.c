/* Clear the active field and reset its associated state word. */

typedef unsigned int u32;

void free_rpc_packet(u32 *p) __asm__("func_0011AD90");

void free_rpc_packet(u32 *packet) {
    u32 packet_flags = packet[4];
    packet[6] = 0;
    packet[4] = packet_flags & 0xfffffffeu;
}
