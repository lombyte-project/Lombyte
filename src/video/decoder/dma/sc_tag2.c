/* Packs three 32-bit components into the game's 64-bit state value. */

void sc_tag2(unsigned long long *output, unsigned int high_word, unsigned int middle_word,
                    unsigned int low_word) __asm__("func_0023BC20");

void sc_tag2(unsigned long long *output, unsigned int high_word, unsigned int middle_word,
                    unsigned int low_word) {
    *output = ((unsigned long long)high_word << 32) | ((unsigned long long)middle_word << 28) |
              (unsigned long long)low_word;
}
