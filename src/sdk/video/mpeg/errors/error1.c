extern int FormatErrorMessage(char *buffer, const char *format, ...) __asm__("sprintf");
extern void ReportError(void *error_state, char *message) __asm__("_Error");

void ErrorWithMessage(void *error_state, const char *format) __asm__("_Error1");

void ErrorWithMessage(void *error_state, const char *format) {
    char message[0x100];
    FormatErrorMessage(message, format);
    ReportError(error_state, message);
}
