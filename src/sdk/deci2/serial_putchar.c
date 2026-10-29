extern int kputchar(int character);

void serialPutchar(int character) {
    if (character == 10) {
        kputchar(13);
        kputchar(10);
    } else {
        kputchar(character);
    }
}
