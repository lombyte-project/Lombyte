typedef signed char s8;

int Deci2Call(int function, void *args) __asm__("Deci2Call");

int CallDebugCharacter(int value, int character) __asm__("CallDebugCharacter");

int CallDebugCharacter(int socket_id, int character) {
    int args[2] = {socket_id, (s8)character};
    return Deci2Call(3, args);
}
