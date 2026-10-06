typedef char *va_list;

typedef struct PrintFile {
    unsigned char unused[0x54];
    void *data;
} PrintFile;

typedef struct PrintReent {
    unsigned char unused[8];
    PrintFile *stdout_file;
} PrintReent;

extern PrintReent *GlobalReent __asm__("D_0012F76C") __attribute__((section(".data")));
extern int Vfprintf(PrintFile *file, const char *format,
                    va_list arguments) __asm__("func_00116DA8");

int Printf(const char *format, ...) __asm__("printf");

int Printf(const char *format, ...) {
    va_list arguments;
    PrintReent *reent = GlobalReent;

    reent->stdout_file->data = reent;
    arguments = __builtin_next_arg(format) - 0x38;
    return Vfprintf(reent->stdout_file, format, arguments);
}
