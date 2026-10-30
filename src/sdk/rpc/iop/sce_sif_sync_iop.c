int  sceSifGetReg(int register_number) __asm__("sceSifGetReg");
void clear_tty_initialized(void) __asm__("func_00118BC0");

int sceSifSyncIop(void) __asm__("sceSifSyncIop");

int sceSifSyncIop(void) {
    if (sceSifGetReg(4) & 0x40000) {
        clear_tty_initialized();
        return 1;
    }
    return 0;
}
