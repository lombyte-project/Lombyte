typedef unsigned short u16;
typedef unsigned int u32;

void font_set_window(u16 *packet, u16 word0, u16 word1, u16 word2, u16 word3, u16 word4,
                         u16 word5, u16 word6, u32 word7) __asm__("font_set_window");

void font_set_window(u16 *packet, u16 word0, u16 word1, u16 word2, u16 word3, u16 word4,
                         u16 word5, u16 word6, u32 word7) {
    packet[0] = word0;
    packet[1] = word1;
    packet[2] = word2;
    packet[3] = word3;
    packet[4] = word4;
    packet[5] = word5;
    packet[8] = word6;
    packet[9] = word7;
    packet[6] = 0;
    packet[7] = 0;
    packet[10] = 0;
    packet[11] = 0;
}
